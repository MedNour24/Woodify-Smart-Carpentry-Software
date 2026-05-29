$repo = Get-Location
Write-Output "Repository: $repo"

$branch = git rev-parse --abbrev-ref HEAD 2>$null
if ($LASTEXITCODE -ne 0) { $branch = '' }
Write-Output "Current branch: $branch"

$changes = git status --porcelain
if ($changes) {
    Write-Output "Uncommitted changes detected."
    git checkout -B Bois
    Write-Output "Switched/created branch Bois."
    git add -A
    git commit -m "WIP: save changes"
    git push -u origin Bois
} else {
    Write-Output "No changes to commit."
    git checkout Bois 2>$null
    if ($LASTEXITCODE -ne 0) {
        git checkout -b Bois
        Write-Output "Created branch Bois."
    } else {
        Write-Output "Switched to branch Bois."
    }
    git push -u origin Bois
}
