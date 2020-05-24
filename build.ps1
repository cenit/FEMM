#!/usr/bin/env pwsh

$number_of_build_workers = 8
$disableLatex = $true
$build_arch = 32

if (($null -eq (Get-Command "latex" -ErrorAction SilentlyContinue)) -or $disableLatex) {
  Write-Host "LaTeX has not been found!" -ForegroundColor Yellow
}
else {
  Write-Host "LaTeX has been found! Building manuals" -ForegroundColor Yellow
  $latexFOUND = "-DBUILD_MANUAL=ON"
}

if ($build_arch -eq "64") {
  New-Item -Path .\build_win_release64 -ItemType directory -Force
  Set-Location build_win_release64
  cmake -A "x64" $latexFOUND ${install_prefix} ..
}

if ($build_arch -eq "32") {
  New-Item -Path .\build_win_release32 -ItemType directory -Force
  Set-Location build_win_release32
  cmake -A "Win32" $latexFOUND ${install_prefix} ..
}

cmake --build . --config Release --parallel ${number_of_build_workers} --target install
Set-Location ..
