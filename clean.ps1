#! /usr/bin/env pwsh

<#

.SYNOPSIS
        clean
        Created By: Stefano Sinigardi
        Created Date: February 18, 2019
        Last Modified Date: October 6, 2022

.DESCRIPTION
Clean up VS projects and solutions

.EXAMPLE
.\clean

#>

<#
Copyright (c) Stefano Sinigardi

MIT License

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED *AS IS*, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
#>

Get-ChildItem -Path $PSScriptRoot -Include *.sln -File -Recurse | foreach { $_.Delete()}
Get-ChildItem -Path $PSScriptRoot -Include *.dsp -File -Recurse | foreach { $_.Delete()}
Get-ChildItem -Path $PSScriptRoot -Include *.dsw -File -Recurse | foreach { $_.Delete()}
Get-ChildItem -Path $PSScriptRoot -Include *.vcproj -File -Recurse | foreach { $_.Delete()}
Get-ChildItem -Path $PSScriptRoot -Include *.vcxproj -File -Recurse | foreach { $_.Delete()}
Get-ChildItem -Path $PSScriptRoot -Include *.vcxproj.filters -File -Recurse | foreach { $_.Delete()}

if (Test-Path $PSScriptRoot/release) {
  Rename-Item $PSScriptRoot/release $PSScriptRoot/bin
}
Remove-Item -Force -Recurse -ErrorAction SilentlyContinue $PSScriptRoot/debug/
