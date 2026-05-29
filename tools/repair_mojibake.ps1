$root = (Resolve-Path (Join-Path $PSScriptRoot '..')).Path
$files = Get-ChildItem -Path $root -Recurse -File -Include *.cpp,*.h,*.ui,*.sql,*.pro,*.md | Where-Object { $_.Name -ne 'repair_mojibake.ps1' }
$utf8NoBom = New-Object System.Text.UTF8Encoding($false)
$latin1 = [System.Text.Encoding]::GetEncoding(28591)

function Get-MojibakeScore {
    param([string]$Text)

    $score = 0
    foreach ($character in $Text.ToCharArray()) {
        if ($character -eq [char]0x00C3 -or $character -eq [char]0x00C2 -or $character -eq [char]0x00E2) {
            $score++
        }
    }

    return $score
}

function Repair-MojibakeLine {
    param([string]$Text)

    $current = $Text
    $bestScore = Get-MojibakeScore -Text $current

    for ($iteration = 0; $iteration -lt 3; $iteration++) {
        $bytes = $latin1.GetBytes($current)
        $candidate = [System.Text.Encoding]::UTF8.GetString($bytes)
        $candidateScore = Get-MojibakeScore -Text $candidate

        if ($candidate -eq $current -or $candidateScore -ge $bestScore) {
            break
        }

        $current = $candidate
        $bestScore = $candidateScore
    }

    return $current
}

foreach ($file in $files) {
    $lines = [System.IO.File]::ReadAllLines($file.FullName)
    $changed = $false

    for ($i = 0; $i -lt $lines.Length; $i++) {
        $line = $lines[$i]
        if ($line -match 'value\.replace\(QString::fromUtf8\(' -or $line -match 'QRegularExpression\("Ã—"\)') {
            continue
        }

        $updated = Repair-MojibakeLine -Text $line

        if ($updated -ne $line) {
            $lines[$i] = $updated
            $changed = $true
        }
    }

    if ($changed) {
        [System.IO.File]::WriteAllLines($file.FullName, $lines, $utf8NoBom)
    }
}
