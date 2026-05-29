#!/usr/bin/env python3
import hashlib
import json
import os
import re
import sys
from typing import Dict, Optional, Tuple


TYPE_CONFIG = {
    "chair": {"price": (90, 220), "days": (2, 4), "dims": ("45x50x90 cm", "55x55x95 cm")},
    "table": {"price": (180, 650), "days": (3, 6), "dims": ("120x70x75 cm", "180x90x78 cm")},
    "sofa": {"price": (600, 2200), "days": (6, 12), "dims": ("180x90x85 cm", "260x100x90 cm")},
    "bed": {"price": (500, 1800), "days": (5, 10), "dims": ("200x160x110 cm", "210x180x115 cm")},
    "desk": {"price": (240, 780), "days": (3, 7), "dims": ("120x60x75 cm", "160x75x78 cm")},
    "cabinet": {"price": (450, 1600), "days": (5, 11), "dims": ("100x45x200 cm", "180x60x230 cm")},
}

LABEL_TO_TYPE = {
    "chair": "chair",
    "armchair": "chair",
    "rocking chair": "chair",
    "folding chair": "chair",
    "table": "table",
    "dining table": "table",
    "coffee table": "table",
    "desk": "desk",
    "writing desk": "desk",
    "sofa": "sofa",
    "couch": "sofa",
    "studio couch": "sofa",
    "day bed": "bed",
    "bed": "bed",
    "wardrobe": "cabinet",
    "bookcase": "cabinet",
    "cabinet": "cabinet",
}


def emit(payload: Dict) -> None:
    sys.stdout.write(json.dumps(payload, ensure_ascii=False))


def hash_ratio(image_path: str) -> float:
    digest = hashlib.sha256(image_path.encode("utf-8")).hexdigest()
    return int(digest[:8], 16) / float(0xFFFFFFFF)


def detect_by_filename(image_path: str) -> Tuple[Optional[str], float]:
    name = os.path.basename(image_path).lower()
    for key, typ in {
        "chair": "chair",
        "chaise": "chair",
        "table": "table",
        "bureau": "desk",
        "desk": "desk",
        "canape": "sofa",
        "sofa": "sofa",
        "lit": "bed",
        "bed": "bed",
        "armoire": "cabinet",
        "cabinet": "cabinet",
    }.items():
        if key in name:
            return typ, 0.62
    return None, 0.0


def detect_with_mobilenet(image_path: str) -> Tuple[Optional[str], float]:
    try:
        import torch
        from PIL import Image
        from torchvision.models import MobileNet_V2_Weights, mobilenet_v2
    except Exception:
        return None, 0.0

    try:
        model = mobilenet_v2(weights=MobileNet_V2_Weights.DEFAULT)
        model.eval()
        preprocess = MobileNet_V2_Weights.DEFAULT.transforms()
        classes = MobileNet_V2_Weights.DEFAULT.meta.get("categories", [])

        image = Image.open(image_path).convert("RGB")
        batch = preprocess(image).unsqueeze(0)
        with torch.no_grad():
            logits = model(batch)[0]
            probs = torch.nn.functional.softmax(logits, dim=0)
            top_probs, top_idx = torch.topk(probs, k=5)
        best_type = None
        best_score = 0.0
        for prob, idx in zip(top_probs.tolist(), top_idx.tolist()):
            label = classes[idx].lower() if idx < len(classes) else ""
            for furniture_label, furniture_type in LABEL_TO_TYPE.items():
                if furniture_label in label and prob > best_score:
                    best_type = furniture_type
                    best_score = float(prob)
        return best_type, best_score
    except Exception:
        return None, 0.0


def estimate_output(furniture_type: str, image_path: str) -> Dict:
    cfg = TYPE_CONFIG.get(furniture_type, TYPE_CONFIG["table"])
    ratio = hash_ratio(image_path)

    min_price, max_price = cfg["price"]
    price_val = int(round(min_price + ratio * (max_price - min_price)))

    min_days, max_days = cfg["days"]
    day_val = int(round(min_days + ratio * (max_days - min_days)))

    dims_a = cfg["dims"][0]
    dims_b = cfg["dims"][1]
    dims = dims_a if ratio < 0.5 else dims_b

    return {
        "status": "ok",
        "type": furniture_type,
        "prix": f"{price_val} TND",
        "fabrication_time": f"{max(1, day_val)} days",
        "dimensions": dims,
    }


def main() -> int:
    if len(sys.argv) < 2:
        emit({"status": "error", "message": "Missing image path"})
        return 0

    image_path = sys.argv[1]
    if not os.path.exists(image_path):
        emit({"status": "error", "message": "Image file not found"})
        return 0

    extension = os.path.splitext(image_path)[1].lower()
    if extension not in {".png", ".jpg", ".jpeg", ".bmp", ".webp"}:
        emit({"status": "error", "message": "Unsupported image format"})
        return 0

    furniture_type, score = detect_with_mobilenet(image_path)
    if not furniture_type:
        furniture_type, score = detect_by_filename(image_path)

    if not furniture_type or score < 0.35:
        emit({"status": "error", "message": "Not a furniture object"})
        return 0

    emit(estimate_output(furniture_type, image_path))
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except Exception as exc:
        clean_msg = re.sub(r"\s+", " ", str(exc)).strip()
        emit({"status": "error", "message": clean_msg or "Inference failed"})
        raise SystemExit(0)
