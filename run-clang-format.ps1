#! /usr/bin/env pwsh

Push-Location $PSScriptRoot

Get-ChildItem -Path . -Directory -Recurse |
ForEach-Object {
    $num_cpp = (Get-ChildItem -Path $_ -force | Where-Object Extension -in ('.cpp') | Measure-Object).Count
    if ($num_cpp -gt 0) {
        Write-Host "Formatting $num_cpp cpp files in $_"
        Set-Location $_.FullName
        & clang-format -i -style=file *.cpp
    }
    $num_c = (Get-ChildItem -Path $_ -force | Where-Object Extension -in ('.c') | Measure-Object).Count
    if ($num_c -gt 0) {
        Write-Host "Formatting $num_c c files in $_"
        Set-Location $_.FullName
        & clang-format -i -style=file *.c
    }
    $num_h = (Get-ChildItem -Path $_ -force | Where-Object Extension -in ('.h') | Measure-Object).Count
    if ($num_h -gt 0) {
        Write-Host "Formatting $num_h h files in $_"
        Set-Location $_.FullName
        & clang-format -i -style=file *.h
    }
}

Pop-Location
