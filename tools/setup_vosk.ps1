param(
    [string]$PythonPath = "",
    [string]$ProjectRoot = "",
    [string]$ModelPath = ""
)

$ErrorActionPreference = "Stop"

function Resolve-ExistingPath([string]$PathValue) {
    if ([string]::IsNullOrWhiteSpace($PathValue)) {
        return $null
    }
    if (Test-Path $PathValue) {
        return (Resolve-Path $PathValue).Path
    }
    return $null
}

function Resolve-Executable([string]$Name) {
    if ([string]::IsNullOrWhiteSpace($Name)) {
        return $null
    }
    $cmd = Get-Command $Name -ErrorAction SilentlyContinue | Select-Object -First 1
    if ($cmd -and $cmd.Source -and (Test-Path $cmd.Source)) {
        return (Resolve-Path $cmd.Source).Path
    }
    return $null
}

function Test-UsablePython([string]$ExecutablePath) {
    if ([string]::IsNullOrWhiteSpace($ExecutablePath)) {
        return $false
    }

    if ($ExecutablePath -like "*\Microsoft\WindowsApps\python*.exe") {
        return $false
    }

    try {
        & $ExecutablePath --version *> $null
        return ($LASTEXITCODE -eq 0)
    } catch {
        return $false
    }
}

function Add-Candidate([System.Collections.Generic.List[string]]$List,
                       [System.Collections.Generic.HashSet[string]]$Seen,
                       [string]$Candidate) {
    if ([string]::IsNullOrWhiteSpace($Candidate)) {
        return
    }
    $resolved = Resolve-ExistingPath $Candidate
    if (-not $resolved) {
        $resolved = Resolve-Executable $Candidate
    }
    if (-not $resolved) {
        return
    }
    if (-not (Test-UsablePython $resolved)) {
        return
    }
    if ($Seen.Add($resolved)) {
        $List.Add($resolved)
    }
}

if ([string]::IsNullOrWhiteSpace($ProjectRoot)) {
    $ProjectRoot = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
}
if ([string]::IsNullOrWhiteSpace($ModelPath)) {
    $ModelPath = Join-Path $ProjectRoot "models\vosk-model-small-fr-0.22"
}

$candidates = New-Object System.Collections.Generic.List[string]
$seen = New-Object System.Collections.Generic.HashSet[string]

Add-Candidate $candidates $seen $PythonPath
Add-Candidate $candidates $seen $env:VOSK_PYTHON_EXECUTABLE
Add-Candidate $candidates $seen "python"
Add-Candidate $candidates $seen "python3"
Add-Candidate $candidates $seen "$ProjectRoot\.venv\Scripts\python.exe"
Add-Candidate $candidates $seen "$ProjectRoot\venv\Scripts\python.exe"
Add-Candidate $candidates $seen "$env:USERPROFILE\AppData\Local\Programs\Python\Python312\python.exe"
Add-Candidate $candidates $seen "$env:USERPROFILE\AppData\Local\Programs\Python\Python311\python.exe"
Add-Candidate $candidates $seen "$env:USERPROFILE\AppData\Local\Programs\Python\Python310\python.exe"
Add-Candidate $candidates $seen "$env:USERPROFILE\AppData\Local\Programs\Python\Python39\python.exe"
Add-Candidate $candidates $seen "$env:USERPROFILE\AppData\Local\Programs\Python\Python38\python.exe"
Add-Candidate $candidates $seen "$env:USERPROFILE\AppData\Local\Programs\Python\Python37\python.exe"
Add-Candidate $candidates $seen "C:\Program Files (x86)\ThonnyTN\python.exe"
Add-Candidate $candidates $seen "C:\Program Files\Autodesk\3ds Max 2021\Python37\python.exe"

if ($candidates.Count -eq 0) {
    throw "Aucun Python exploitable trouve. Installez Python 3.8+ puis relancez ce script."
}

$selectedPython = $candidates[0]
Write-Host "Python selectionne: $selectedPython"

& $selectedPython -m pip install --upgrade pip setuptools wheel
if ($LASTEXITCODE -ne 0) {
    throw "Echec de mise a jour de pip/setuptools/wheel."
}

& $selectedPython -m pip install vosk
if ($LASTEXITCODE -ne 0) {
    throw "Echec de l'installation du module vosk."
}

& $selectedPython -c "import sys,vosk;print(sys.executable);print('VOSK_OK')"
if ($LASTEXITCODE -ne 0) {
    throw "Le module vosk n'est pas importable avec ce Python."
}

if (Test-Path $ModelPath) {
    & $selectedPython -c "from vosk import Model; Model(r'$ModelPath'); print('MODEL_OK')"
    if ($LASTEXITCODE -ne 0) {
        throw "Le modele Vosk n'a pas pu etre charge depuis: $ModelPath"
    }
} else {
    Write-Warning "Modele Vosk introuvable: $ModelPath"
    Write-Warning "Placez un modele dans le dossier models/ ou configurez VOSK_MODEL_PATH."
}

Write-Host ""
Write-Host "Configuration terminee."
Write-Host "Optionnel (PowerShell courant):"
Write-Host "  `$env:VOSK_PYTHON_EXECUTABLE = '$selectedPython'"
Write-Host "  `$env:VOSK_MODEL_PATH = '$ModelPath'"
