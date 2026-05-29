param(
    [switch]$ConfigureOnly,
    [switch]$Clean,
    [switch]$SkipDeploy,
    [string]$ProjectFile = "Smartcarpentry.pro",
    [string]$BuildMode = "debug"
)

$ErrorActionPreference = "Stop"

function Find-FirstExistingPath([string[]]$Candidates) {
    foreach ($candidate in $Candidates) {
        if (-not [string]::IsNullOrWhiteSpace($candidate) -and (Test-Path $candidate)) {
            return (Resolve-Path $candidate).Path
        }
    }
    return $null
}

function Resolve-CommandPath([string]$Name) {
    if ([string]::IsNullOrWhiteSpace($Name)) {
        return $null
    }

    $command = Get-Command $Name -ErrorAction SilentlyContinue | Select-Object -First 1
    if ($command -and $command.Source -and (Test-Path $command.Source)) {
        return (Resolve-Path $command.Source).Path
    }

    return $null
}

function Get-QtQmakeCandidates {
    $candidates = @(
        $env:QMAKE,
        (Resolve-CommandPath "qmake.exe"),
        (Resolve-CommandPath "qmake"),
        "C:/Qt/6.8.3/mingw_64/bin/qmake.exe",
        "C:/Qt/6.8.3/llvm-mingw_64/bin/qmake.exe",
        "C:/Qt/6.8.3/msvc2022_64/bin/qmake.exe",
        "C:/Qt/6.8.2/mingw_64/bin/qmake.exe",
        "C:/Qt/6.8.2/llvm-mingw_64/bin/qmake.exe",
        "C:/Qt/6.8.2/msvc2022_64/bin/qmake.exe",
        "C:/Qt/6.8.1/mingw_64/bin/qmake.exe",
        "C:/Qt/6.8.1/llvm-mingw_64/bin/qmake.exe",
        "C:/Qt/6.8.1/msvc2022_64/bin/qmake.exe",
        "C:/Qt/6.8.0/mingw_64/bin/qmake.exe",
        "C:/Qt/6.8.0/llvm-mingw_64/bin/qmake.exe",
        "C:/Qt/6.8.0/msvc2022_64/bin/qmake.exe",
        "C:/Qt/6.7.3/mingw_64/bin/qmake.exe",
        "C:/Qt/6.7.3/llvm-mingw_64/bin/qmake.exe",
        "C:/Qt/6.7.3/msvc2022_64/bin/qmake.exe",
        "C:/Qt/Tools/QtCreator/bin/qmake.exe"
    )

    if (Test-Path "C:/Qt") {
        $discovered = Get-ChildItem -Path "C:/Qt/*/*/bin/qmake.exe" -File -ErrorAction SilentlyContinue |
            Sort-Object FullName -Descending |
            Select-Object -ExpandProperty FullName
        $candidates += $discovered
    }

    return $candidates
}

function Get-ToolCandidates([string[]]$Paths, [string[]]$Commands) {
    $resolvedCommands = @()
    foreach ($cmd in $Commands) {
        $resolved = Resolve-CommandPath $cmd
        if ($resolved) {
            $resolvedCommands += $resolved
        }
    }
    return @($Paths + $resolvedCommands)
}

function Get-WinDeployQtCandidates([string]$QtBinDir) {
    $candidates = @(
        $env:WINDEPLOYQT,
        (Resolve-CommandPath "windeployqt.exe"),
        (Resolve-CommandPath "windeployqt"),
        (Resolve-CommandPath "windeployqt6.exe"),
        (Resolve-CommandPath "windeployqt6"),
        (Join-Path $QtBinDir "windeployqt.exe")
    )

    if (Test-Path "C:/Qt") {
        $discovered = Get-ChildItem -Path "C:/Qt/*/*/bin/windeployqt.exe" -File -ErrorAction SilentlyContinue |
            Sort-Object FullName -Descending |
            Select-Object -ExpandProperty FullName
        $candidates += $discovered
    }

    return $candidates
}

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$projectRoot = Resolve-Path (Join-Path $scriptDir "..")
Set-Location $projectRoot

if (-not (Test-Path $ProjectFile)) {
    throw "Project file not found: $ProjectFile"
}

$qmake = Find-FirstExistingPath (Get-QtQmakeCandidates)
if (-not $qmake) {
    throw @"
No qmake executable found.
Install Qt (including a desktop kit), then either:
 - add qmake.exe to PATH, or
 - set the QMAKE environment variable to the full qmake.exe path.
"@
}

$qtBinDir = Split-Path -Parent $qmake
$qmakeSpec = (& $qmake -query QMAKE_SPEC 2>$null | Out-String).Trim().ToLowerInvariant()
$isMsvcKit = $qmakeSpec -match "msvc"
$isMingwKit = $qmakeSpec -match "mingw"

$makeTool = $null
$ccCompiler = $null
$cxxCompiler = $null

if ($isMsvcKit) {
    $makeTool = Find-FirstExistingPath (
        Get-ToolCandidates `
            -Paths @(
                (Join-Path $qtBinDir "jom.exe"),
                (Join-Path $qtBinDir "nmake.exe")
            ) `
            -Commands @("jom.exe", "jom", "nmake.exe", "nmake")
    )

    if (-not $makeTool) {
        throw "MSVC Qt kit detected ('$qmakeSpec') but neither jom nor nmake was found."
    }
} else {
    $makeTool = Find-FirstExistingPath (
        Get-ToolCandidates `
            -Paths @(
                (Join-Path $qtBinDir "mingw32-make.exe"),
                "C:/Qt/Tools/mingw1120_64/bin/mingw32-make.exe",
                "C:/Qt/Tools/mingw1310_64/bin/mingw32-make.exe",
                "C:/Qt/6.7.3/mingw_64/bin/mingw32-make.exe"
            ) `
            -Commands @("mingw32-make.exe", "mingw32-make", "make.exe", "make")
    )

    if (-not $makeTool) {
        throw "No make tool found (expected mingw32-make for this Qt kit)."
    }

    $cxxCompiler = Find-FirstExistingPath (
        Get-ToolCandidates `
            -Paths @(
                (Join-Path $qtBinDir "g++.exe"),
                "C:/Qt/Tools/mingw1120_64/bin/g++.exe",
                "C:/Qt/Tools/mingw1310_64/bin/g++.exe",
                "C:/Qt/6.7.3/mingw_64/bin/g++.exe"
            ) `
            -Commands @("g++.exe", "g++")
    )
    $ccCompiler = Find-FirstExistingPath (
        Get-ToolCandidates `
            -Paths @(
                (Join-Path $qtBinDir "gcc.exe"),
                "C:/Qt/Tools/mingw1120_64/bin/gcc.exe",
                "C:/Qt/Tools/mingw1310_64/bin/gcc.exe",
                "C:/Qt/6.7.3/mingw_64/bin/gcc.exe"
            ) `
            -Commands @("gcc.exe", "gcc")
    )

    if ($cxxCompiler -and $ccCompiler) {
        $compilerBin = Split-Path -Parent $cxxCompiler
        if (-not $env:PATH.StartsWith("$compilerBin;")) {
            $env:PATH = "$compilerBin;$env:PATH"
        }
        $env:CC = $ccCompiler
        $env:CXX = $cxxCompiler
    }
}

if ($Clean) {
    $makefiles = @("Makefile", "Makefile.Debug", "Makefile.Release", ".qmake.stash")
    foreach ($file in $makefiles) {
        if (Test-Path $file) {
            Remove-Item -Force $file
        }
    }
}

Write-Host "Using qmake: $qmake"
Write-Host "Detected qmake spec: $qmakeSpec"
Write-Host "Using make tool: $makeTool"
if ($ccCompiler -and $cxxCompiler) {
    Write-Host "Using C compiler: $ccCompiler"
    Write-Host "Using C++ compiler: $cxxCompiler"
}

$qmakeArgs = @($ProjectFile)
if ($ccCompiler -and $cxxCompiler) {
    $qmakeArgs += "QMAKE_CC=$ccCompiler"
    $qmakeArgs += "QMAKE_CXX=$cxxCompiler"
}

& $qmake @qmakeArgs
if ($LASTEXITCODE -ne 0) {
    throw "qmake failed with exit code $LASTEXITCODE"
}

if ($ConfigureOnly) {
    Write-Host "qmake configure completed successfully."
    exit 0
}

if ($BuildMode -ieq "release") {
    & $makeTool "release"
} else {
    & $makeTool "debug"
}

if ($LASTEXITCODE -ne 0) {
    throw "Build failed with exit code $LASTEXITCODE"
}

Write-Host "Build completed successfully ($BuildMode)."

if (-not $SkipDeploy) {
    $builtExeCandidates = @()
    if ($BuildMode -ieq "release") {
        $builtExeCandidates += Join-Path $projectRoot "release\Smartcarpentry.exe"
        $builtExeCandidates += Join-Path $projectRoot "Smartcarpentry.exe"
    } else {
        $builtExeCandidates += Join-Path $projectRoot "debug\Smartcarpentry.exe"
        $builtExeCandidates += Join-Path $projectRoot "Smartcarpentry.exe"
    }

    $builtExe = Find-FirstExistingPath $builtExeCandidates
    if ($builtExe) {
        $windeployqt = Find-FirstExistingPath (Get-WinDeployQtCandidates -QtBinDir $qtBinDir)
        if ($windeployqt) {
            $deployModeArg = if ($BuildMode -ieq "release") { "--release" } else { "--debug" }
            Write-Host "Deploying runtime dependencies with: $windeployqt"
            & $windeployqt $deployModeArg "--compiler-runtime" $builtExe
            if ($LASTEXITCODE -ne 0) {
                Write-Warning "windeployqt failed with exit code $LASTEXITCODE. The app may fail to launch due to missing DLLs."
            } else {
                Write-Host "Runtime dependencies deployed to: $(Split-Path -Parent $builtExe)"
            }
        } else {
            Write-Warning "windeployqt not found; skipping runtime deployment. Add Qt bin to PATH or set WINDEPLOYQT."
        }
    } else {
        Write-Warning "Built executable not found for mode '$BuildMode'; skipped runtime deployment."
    }
}
