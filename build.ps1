#!/usr/bin/env pwsh

$number_of_build_workers = 8
$disableLatex = $true
$build_arch = 64
$force_triangle_32 = $false

if (($null -eq (Get-Command "latex" -ErrorAction SilentlyContinue)) -or $disableLatex) {
  Write-Host "LaTeX has not been found!" -ForegroundColor Yellow
}
else {
  Write-Host "LaTeX has been found! Building manuals" -ForegroundColor Yellow
  $latexFOUND = "-DBUILD_MANUAL=ON"
}

if ($build_arch -eq "64") {
  if($force_triangle_32) {
    Write-Host "Triangle will be built as a 32 bit executable!" -ForegroundColor Yellow
    New-Item -Path .\build_win_release64 -ItemType directory -Force
    Set-Location build_win_release64_notriangle
    cmake -A "x64" $latexFOUND ${install_prefix} -DSKIP_triangle:BOOL=ON ..
    cmake --build . --config Release --parallel ${number_of_build_workers} --target install
    Set-Location ..
    New-Item -Path .\build_win_release32_triangle -ItemType directory -Force
    Set-Location build_win_release32_triangle
    cmake -A "Win32" $latexFOUND ${install_prefix} -DSKIP_belasolv:BOOL=ON -DSKIP_csolv:BOOL=ON -DSKIP_liblua:BOOL=ON -DSKIP_ResizableLib:BOOL=ON -DSKIP_femm:BOOL=ON -DSKIP_femmplot:BOOL=ON   -DSKIP_fkn:BOOL=ON -DSKIP_hsolv:BOOL=ON -DSKIP_scifemm:BOOL=ON ..
    cmake --build . --config Release --parallel ${number_of_build_workers} --target install
    Set-Location ..
  }
  else {
    New-Item -Path .\build_win_release64 -ItemType directory -Force
    Set-Location build_win_release64
    cmake -A "x64" $latexFOUND ${install_prefix} ..
    cmake --build . --config Release --parallel ${number_of_build_workers} --target install
    Set-Location ..
  }
}

if ($build_arch -eq "32") {
  New-Item -Path .\build_win_release32 -ItemType directory -Force
  Set-Location build_win_release32
  cmake -A "Win32" $latexFOUND ${install_prefix} ..
  cmake --build . --config Release --parallel ${number_of_build_workers} --target install
  Set-Location ..
}
