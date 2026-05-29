import argparse
import json
import os
import re
import sys
from array import array

TARGET_RATE = 16000


def emit(payload, exit_code=0):
    sys.stdout.write(json.dumps(payload, ensure_ascii=False))
    sys.stdout.flush()
    raise SystemExit(exit_code)


def downmix_to_mono(pcm_bytes, channels):
    if channels < 1:
        raise ValueError("Le nombre de canaux audio est invalide.")

    samples = array("h")
    samples.frombytes(pcm_bytes)
    if sys.byteorder != "little":
        samples.byteswap()

    if channels <= 1:
        return samples

    frame_count = len(samples) // channels
    mono = array("h")
    for frame_index in range(frame_count):
        base = frame_index * channels
        total = 0
        for channel_index in range(channels):
            total += samples[base + channel_index]
        mono.append(int(total / channels))

    return mono


def compute_peak(samples):
    return max((abs(sample) for sample in samples), default=0)


def trim_silence(samples, sample_rate):
    if not samples:
        return samples

    peak = compute_peak(samples)
    if peak < 180:
        return array("h")

    threshold = max(180, int(peak * 0.18))
    start = 0
    end = len(samples) - 1

    while start < len(samples) and abs(samples[start]) < threshold:
        start += 1

    while end >= start and abs(samples[end]) < threshold:
        end -= 1

    if end < start:
        return array("h")

    padding = max(1, sample_rate // 5)
    start = max(0, start - padding)
    end = min(len(samples), end + padding + 1)
    return array("h", samples[start:end])


def normalize_peak(samples, target_peak=22000):
    peak = compute_peak(samples)
    if peak <= 0:
        return samples

    gain = min(5.0, float(target_peak) / float(peak))
    if gain <= 1.05:
        return samples

    normalized = array("h")
    for sample in samples:
        value = int(round(sample * gain))
        value = max(-32768, min(32767, value))
        normalized.append(value)
    return normalized


def resample_linear(samples, src_rate, dst_rate):
    if src_rate <= 0 or dst_rate <= 0:
        raise ValueError("Frequence audio invalide.")
    if src_rate == dst_rate or len(samples) < 2:
        return samples

    output_length = max(1, int(round(len(samples) * float(dst_rate) / float(src_rate))))
    result = array("h")
    step = float(src_rate) / float(dst_rate)
    position = 0.0

    for _ in range(output_length):
        left_index = int(position)
        right_index = min(left_index + 1, len(samples) - 1)
        fraction = position - left_index
        left_value = samples[left_index]
        right_value = samples[right_index]
        interpolated = int(round(left_value * (1.0 - fraction) + right_value * fraction))
        result.append(max(-32768, min(32767, interpolated)))
        position += step

    return result


def number_word_value(token):
    values = {
        "zero": 0,
        "un": 1,
        "une": 1,
        "deux": 2,
        "trois": 3,
        "quatre": 4,
        "cinq": 5,
        "six": 6,
        "sept": 7,
        "huit": 8,
        "neuf": 9,
        "dix": 10,
        "onze": 11,
        "douze": 12,
        "treize": 13,
        "quatorze": 14,
        "quinze": 15,
        "seize": 16,
        "vingt": 20,
        "trente": 30,
        "quarante": 40,
        "cinquante": 50,
        "soixante": 60,
        "quatrevingt": 80,
    }
    return values.get(token, -1)


def is_number_token(token):
    return token in {"cent", "cents", "mille", "et"} or number_word_value(token) >= 0


def parse_number_sequence(tokens, start_index):
    total = 0
    current = 0
    index = start_index
    seen_any = False

    while index < len(tokens):
        token = tokens[index]
        if not is_number_token(token):
            break

        if token == "et":
            if not seen_any:
                break
            index += 1
            continue

        if token in {"cent", "cents"}:
            current = max(1, current) * 100
            seen_any = True
            index += 1
            continue

        if token == "mille":
            total += max(1, current) * 1000
            current = 0
            seen_any = True
            index += 1
            continue

        value = number_word_value(token)
        if value < 0:
            break

        current += value
        seen_any = True
        index += 1

    if not seen_any:
        return None, 0

    return total + current, index - start_index


def normalize_transcript_text(text):
    normalized = text.lower().strip()
    if not normalized:
        return normalized

    normalized = re.sub(r"[\r\n\t]+", " ", normalized)
    normalized = re.sub(r"\bm\s*d\s*f\b", "mdf", normalized)
    normalized = re.sub(r"\bh\s*d\s*f\b", "hdf", normalized)
    normalized = re.sub(r"\bo\s*s\s*b\b", "osb", normalized)
    normalized = re.sub(r"\bcontre\s+plaque\b", "contreplaque", normalized)
    normalized = normalized.replace("×", " x ").replace("-", " ")
    normalized = re.sub(r"\s+", " ", normalized).strip()

    raw_tokens = normalized.split()
    merged = []
    index = 0
    while index < len(raw_tokens):
        token = raw_tokens[index]
        if token == "quatre" and index + 1 < len(raw_tokens) and raw_tokens[index + 1].startswith("vingt"):
            merged.append("quatrevingt")
            index += 2
            continue
        merged.append(token)
        index += 1

    rebuilt = []
    index = 0
    while index < len(merged):
        value, consumed = parse_number_sequence(merged, index)
        if consumed > 0:
            rebuilt.append(str(value))
            index += consumed
            continue
        rebuilt.append(merged[index])
        index += 1

    normalized = " ".join(rebuilt)
    normalized = re.sub(r"\b(\d+(?:[.,]\d+)?)\s*(?:par|sur|fois)\s*(\d+(?:[.,]\d+)?)\b", r"\1 x \2", normalized)
    normalized = re.sub(r"\b(\d+)\s*millimetres?\b", r"\1 mm", normalized)
    normalized = re.sub(r"\b(\d+)\s*centimetres?\b", r"\1 cm", normalized)
    normalized = re.sub(r"\b(\d+)\s*metres?\b", r"\1 m", normalized)
    return re.sub(r"\s+", " ", normalized).strip()


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--model", required=True)
    parser.add_argument("--pcm", required=True)
    parser.add_argument("--rate", required=True, type=int)
    parser.add_argument("--channels", required=True, type=int)
    args = parser.parse_args()

    if not os.path.isdir(args.model):
        emit({"ok": False, "error": "Le modele Vosk est introuvable."}, 2)
    if not os.path.isfile(args.pcm):
        emit({"ok": False, "error": "Le fichier audio temporaire est introuvable."}, 2)

    try:
        from vosk import KaldiRecognizer, Model, SetLogLevel
    except Exception as exc:
        emit({"ok": False, "error": f"Le module Python vosk n'est pas disponible: {exc}"}, 2)

    try:
        SetLogLevel(-1)
        with open(args.pcm, "rb") as audio_file:
            pcm_bytes = audio_file.read()
        mono_samples = downmix_to_mono(pcm_bytes, args.channels)
        mono_samples = trim_silence(mono_samples, args.rate)
        if not mono_samples:
            emit({"ok": False, "error": "Le signal micro est trop faible ou ne contient pas de voix exploitable."}, 3)

        mono_samples = normalize_peak(mono_samples)
        mono_samples = resample_linear(mono_samples, args.rate, TARGET_RATE)
        if sys.byteorder != "little":
            mono_samples.byteswap()
        mono_bytes = mono_samples.tobytes()

        model = Model(args.model)
        recognizer = KaldiRecognizer(model, float(TARGET_RATE))

        texts = []
        chunk_size = 4000
        for offset in range(0, len(mono_bytes), chunk_size):
            chunk = mono_bytes[offset : offset + chunk_size]
            if recognizer.AcceptWaveform(chunk):
                result = json.loads(recognizer.Result())
                text = result.get("text", "").strip()
                if text:
                    texts.append(text)

        final_result = json.loads(recognizer.FinalResult())
        final_text = final_result.get("text", "").strip()
        if final_text:
            texts.append(final_text)

        transcript = " ".join(part for part in texts if part).strip()
        if not transcript:
            emit({"ok": False, "error": "Aucune phrase n'a ete reconnue par Vosk."}, 3)

        transcript = normalize_transcript_text(transcript)

        emit({"ok": True, "text": transcript}, 0)
    except Exception as exc:
        emit({"ok": False, "error": f"Echec de transcription Vosk: {exc}"}, 4)


if __name__ == "__main__":
    main()
