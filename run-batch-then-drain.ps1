
param(
  [string]$Profile = "batch_then_drain",
  [switch]$ForceRegenerate
)

# Detect repo root (where CMakeLists.txt lives)
$RepoRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
if (-not (Test-Path (Join-Path $RepoRoot "CMakeLists.txt"))) {
  throw "Run this script from the repo root."
}

# Find the newest harness and generator exes
$harness = Get-ChildItem -Path $RepoRoot -Filter "harness.exe" -Recurse -File |
           Sort-Object LastWriteTime -Descending | Select-Object -First 1
$gen = Get-ChildItem -Path $RepoRoot -Filter "batch_then_drain_gen.exe" -Recurse -File |
       Sort-Object LastWriteTime -Descending | Select-Object -First 1
if (-not $harness) { throw "Could not find harness.exe. Build first." }
if (-not $gen)     { throw "Could not find batch_then_drain_gen.exe. Build first." }

$buildDir  = $harness.Directory.FullName
$tracesDir = Join-Path $RepoRoot ("traces\" + $Profile)
$resultsDir = Join-Path $RepoRoot "results"
New-Item -ItemType Directory -Force -Path $tracesDir,$resultsDir | Out-Null

# Generate traces if missing or if -ForceRegenerate
$needGen = $ForceRegenerate -or -not (Get-ChildItem $tracesDir -Filter *.trace -ErrorAction SilentlyContinue)
if ($needGen) {
  Write-Host "Generating traces into $tracesDir ..."
  Push-Location $RepoRoot
  & $gen.FullName
  Pop-Location
}

# Run harness from its build dir so ../../traces/... resolves
$csv = Join-Path $resultsDir ($Profile + ".csv")
Write-Host "Running harness -> $csv"
Push-Location $buildDir
& $harness.FullName | Set-Content -Encoding ascii $csv
Pop-Location

Write-Host "Wrote $csv"
Get-Content $csv -TotalCount 12
