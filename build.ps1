#!/usr/bin/env pwsh

param (
  [switch]$DisableInteractive = $false,
  [switch]$DoNotUpdateTOOL = $false,
  [switch]$DoNotDeleteBuildFolder = $false,
  [switch]$ForceStaticLib = $false,
  [switch]$ForceTriangle32bit = $false,
  [switch]$DisableLaTeX = $false,
  [Int32]$ForceGCCVersion = 0,
  [Int32]$BuildArch = 64,
  [Int32]$NumberOfBuildWorkers = 8,
  [string]$AdditionalBuildSetup = ""  # "-DCMAKE_CUDA_ARCHITECTURES=30"
)

$build_ps1_version = "2.0.0"

$ErrorActionPreference = "SilentlyContinue"
Stop-Transcript | out-null
$ErrorActionPreference = "Continue"
Start-Transcript -Path $PSScriptRoot/build.log

Function MyThrow ($Message) {
  if ($DisableInteractive) {
    Write-Host $Message -ForegroundColor Red
    throw
  }
  else {
    # Check if running in PowerShell ISE
    if ($psISE) {
      # "ReadKey" not supported in PowerShell ISE.
      # Show MessageBox UI
      $Shell = New-Object -ComObject "WScript.Shell"
      $Shell.Popup($Message, 0, "OK", 0)
      throw
    }

    $Ignore =
    16, # Shift (left or right)
    17, # Ctrl (left or right)
    18, # Alt (left or right)
    20, # Caps lock
    91, # Windows key (left)
    92, # Windows key (right)
    93, # Menu key
    144, # Num lock
    145, # Scroll lock
    166, # Back
    167, # Forward
    168, # Refresh
    169, # Stop
    170, # Search
    171, # Favorites
    172, # Start/Home
    173, # Mute
    174, # Volume Down
    175, # Volume Up
    176, # Next Track
    177, # Previous Track
    178, # Stop Media
    179, # Play
    180, # Mail
    181, # Select Media
    182, # Application 1
    183  # Application 2

    Write-Host $Message -ForegroundColor Red
    Write-Host -NoNewline "Press any key to continue..."
    while (($null -eq $KeyInfo.VirtualKeyCode) -or ($Ignore -contains $KeyInfo.VirtualKeyCode)) {
      $KeyInfo = $Host.UI.RawUI.ReadKey("NoEcho, IncludeKeyDown")
    }
    Write-Host ""
    throw
  }
}

Write-Host "Build script version ${build_ps1_version}"

Write-Host "Setting parallelism to $env:NUMBER_OF_PROCESSORS"
$env:CMAKE_BUILD_PARALLEL_LEVEL = $env:NUMBER_OF_PROCESSORS

Write-Host -NoNewLine "PowerShell version:"
$PSVersionTable.PSVersion

if ($PSVersionTable.PSVersion.Major -eq 5) {
  $IsWindowsPowerShell = $true
}

if ($PSVersionTable.PSVersion.Major -lt 5) {
  MyThrow("Your PowerShell version is too old, please update it.")
}


if ($ForceStaticLib) {
  Write-Host "Forced CMake to produce a static library"
  $AdditionalBuildSetup = $AdditionalBuildSetup + " -DBUILD_SHARED_LIBS=OFF "
}

Push-Location $PSScriptRoot

$GIT_EXE = Get-Command "git" -ErrorAction SilentlyContinue | Select-Object -ExpandProperty Definition
if (-Not $GIT_EXE) {
  MyThrow("Could not find git, please install it")
}
else {
  Write-Host "Using git from ${GIT_EXE}"
}

if (Test-Path "$PSScriptRoot/.git") {
  Write-Host "This tool has been cloned with git and supports self-updating mechanism"
  if ($DoNotUpdateTOOL) {
    Write-Host "This tool will not self-update sources" -ForegroundColor Yellow
  }
  else {
    Write-Host "This tool will self-update sources, please pass -DoNotUpdateTOOL to the script to disable"
    $proc = Start-Process -NoNewWindow -PassThru -FilePath $GIT_EXE -ArgumentList "pull"
    $handle = $proc.Handle
    $proc.WaitForExit()
    $exitCode = $proc.ExitCode
    if (-Not ($exitCode -eq 0)) {
      MyThrow("Updating this tool sources failed! Exited with error code $exitCode.")
    }
  }
}

$CMAKE_EXE = Get-Command "cmake" -ErrorAction SilentlyContinue | Select-Object -ExpandProperty Definition
if (-Not $CMAKE_EXE) {
  MyThrow("Could not find CMake, please install it")
}
else {
  Write-Host "Using CMake from ${CMAKE_EXE}"
  $proc = Start-Process -NoNewWindow -PassThru -FilePath ${CMAKE_EXE} -ArgumentList "--version"
  $handle = $proc.Handle
  $proc.WaitForExit()
  $exitCode = $proc.ExitCode
  if (-Not ($exitCode -eq 0)) {
    MyThrow("CMake version check failed! Exited with error code $exitCode.")
  }
}

function getProgramFiles32bit() {
  $out = ${env:PROGRAMFILES(X86)}
  if ($null -eq $out) {
    $out = ${env:PROGRAMFILES}
  }

  if ($null -eq $out) {
    MyThrow("Could not find [Program Files 32-bit]")
  }

  return $out
}

function getLatestVisualStudioWithDesktopWorkloadPath() {
  $programFiles = getProgramFiles32bit
  $vswhereExe = "$programFiles\Microsoft Visual Studio\Installer\vswhere.exe"
  if (Test-Path $vswhereExe) {
    $output = & $vswhereExe -products * -latest -requires Microsoft.VisualStudio.Workload.NativeDesktop -format xml
    [xml]$asXml = $output
    foreach ($instance in $asXml.instances.instance) {
      $installationPath = $instance.InstallationPath -replace "\\$" # Remove potential trailing backslash
    }
    if (!$installationPath) {
      Write-Host "Warning: no full Visual Studio setup has been found, extending search to include also partial installations" -ForegroundColor Yellow
      $output = & $vswhereExe -products * -latest -format xml
      [xml]$asXml = $output
      foreach ($instance in $asXml.instances.instance) {
        $installationPath = $instance.InstallationPath -replace "\\$" # Remove potential trailing backslash
      }
    }
    if (!$installationPath) {
      Write-Host "Warning: no full Visual Studio setup has been found, extending search to include also pre-release installations" -ForegroundColor Yellow
      $output = & $vswhereExe -prerelease -products * -latest -format xml
      [xml]$asXml = $output
      foreach ($instance in $asXml.instances.instance) {
        $installationPath = $instance.InstallationPath -replace "\\$" # Remove potential trailing backslash
      }
    }
    if (!$installationPath) {
      MyThrow("Could not locate any installation of Visual Studio")
    }
  }
  else {
    MyThrow("Could not locate vswhere at $vswhereExe")
  }
  return $installationPath
}


function getLatestVisualStudioWithDesktopWorkloadVersion() {
  $programFiles = getProgramFiles32bit
  $vswhereExe = "$programFiles\Microsoft Visual Studio\Installer\vswhere.exe"
  if (Test-Path $vswhereExe) {
    $output = & $vswhereExe -products * -latest -requires Microsoft.VisualStudio.Workload.NativeDesktop -format xml
    [xml]$asXml = $output
    foreach ($instance in $asXml.instances.instance) {
      $installationVersion = $instance.InstallationVersion
    }
    if (!$installationVersion) {
      Write-Host "Warning: no full Visual Studio setup has been found, extending search to include also partial installations" -ForegroundColor Yellow
      $output = & $vswhereExe -products * -latest -format xml
      [xml]$asXml = $output
      foreach ($instance in $asXml.instances.instance) {
        $installationVersion = $instance.installationVersion
      }
    }
    if (!$installationVersion) {
      Write-Host "Warning: no full Visual Studio setup has been found, extending search to include also pre-release installations" -ForegroundColor Yellow
      $output = & $vswhereExe -prerelease -products * -latest -format xml
      [xml]$asXml = $output
      foreach ($instance in $asXml.instances.instance) {
        $installationVersion = $instance.installationVersion
      }
    }
    if (!$installationVersion) {
      MyThrow("Could not locate any installation of Visual Studio")
    }
  }
  else {
    MyThrow("Could not locate vswhere at $vswhereExe")
  }
  return $installationVersion
}

if (-Not $DoNotSetupVS) {
  $CL_EXE = Get-Command "cl" -ErrorAction SilentlyContinue | Select-Object -ExpandProperty Definition
  if ((-Not $CL_EXE) -or ($CL_EXE -match "HostX86\\x86") -or ($CL_EXE -match "HostX64\\x86")) {
    $vsfound = getLatestVisualStudioWithDesktopWorkloadPath
    Write-Host "Found VS in ${vsfound}"
    Push-Location "${vsfound}\Common7\Tools"
    cmd.exe /c "VsDevCmd.bat -arch=x64 & set" |
    ForEach-Object {
      if ($_ -match "=") {
        $v = $_.split("="); Set-Item -force -path "ENV:\$($v[0])"  -value "$($v[1])"
      }
    }
    Pop-Location
    Write-Host "Visual Studio Command Prompt variables set"
  }

  $tokens = getLatestVisualStudioWithDesktopWorkloadVersion
  $tokens = $tokens.split('.')
  if ($DoNotUseNinja) {
    $dllfolder = "Release"
    $selectConfig = " --config Release "
    if ($tokens[0] -eq "14") {
      $generator = "Visual Studio 14 2015"
      $AdditionalBuildSetup = $AdditionalBuildSetup + " -T `"host=x64`" -A `"x64`""
    }
    elseif ($tokens[0] -eq "15") {
      $generator = "Visual Studio 15 2017"
      $AdditionalBuildSetup = $AdditionalBuildSetup + " -T `"host=x64`" -A `"x64`""
    }
    elseif ($tokens[0] -eq "16") {
      $generator = "Visual Studio 16 2019"
      $AdditionalBuildSetup = $AdditionalBuildSetup + " -T `"host=x64`" -A `"x64`""
    }
    elseif ($tokens[0] -eq "17") {
      $generator = "Visual Studio 17 2022"
      $AdditionalBuildSetup = $AdditionalBuildSetup + " -T `"host=x64`" -A `"x64`""
    }
    else {
      MyThrow("Unknown Visual Studio version, unsupported configuration")
    }
  }
  if (-Not $UseVCPKG) {
    $dllfolder = ""
  }
}
Write-Host "Setting up environment to use CMake generator: $generator"

if (-Not $DoNotDeleteBuildFolder) {
  Write-Host "Removing build folders" -ForegroundColor Yellow
  Remove-Item -Force -Recurse -ErrorAction SilentlyContinue .\build_win_release64
  Remove-Item -Force -Recurse -ErrorAction SilentlyContinue .\build_win_release32_triangle
  Remove-Item -Force -Recurse -ErrorAction SilentlyContinue .\build_win_release32
}

if (($null -eq (Get-Command "latex" -ErrorAction SilentlyContinue)) -or $DisableLaTeX) {
  Write-Host "LaTeX has not been found!" -ForegroundColor Yellow
}
else {
  Write-Host "LaTeX has been found! Building manuals" -ForegroundColor Yellow
  $latexFOUND = "-DBUILD_MANUAL=ON"
}

if ($BuildArch -eq "64") {
  if ($ForceTriangle32bit) {
    Write-Host "Triangle will be built as a 32 bit executable!" -ForegroundColor Yellow
    New-Item -Path .\build_win_release64_notriangle -ItemType directory -Force | Out-Null
    Set-Location build_win_release64_notriangle
    $cmake_args = "-A x64 $latexFOUND ${install_prefix} -DSKIP_triangle:BOOL=ON .."
    Write-Host "Configuring CMake project" -ForegroundColor Green
    Write-Host "CMake args: $cmake_args"
    $proc = Start-Process -NoNewWindow -PassThru -FilePath $CMAKE_EXE -ArgumentList $cmake_args
    $handle = $proc.Handle
    $proc.WaitForExit()
    $exitCode = $proc.ExitCode
    if (-Not ($exitCode -eq 0)) {
      MyThrow("Config failed! Exited with error code $exitCode.")
    }
    Write-Host "Building CMake project" -ForegroundColor Green
    $cmake_args = "--build . ${selectConfig} --parallel ${NumberOfBuildWorkers} --target install"
    Write-Host "CMake args: $cmake_args"
    $proc = Start-Process -NoNewWindow -PassThru -FilePath $CMAKE_EXE -ArgumentList $cmake_args
    $handle = $proc.Handle
    $proc.WaitForExit()
    $exitCode = $proc.ExitCode
    if (-Not ($exitCode -eq 0)) {
      MyThrow("Build failed! Exited with error code $exitCode.")
    }
    Set-Location ..
    New-Item -Path .\build_win_release32_triangle -ItemType directory -Force | Out-Null
    Set-Location build_win_release32_triangle
    $cmake_args = "-A Win32 $latexFOUND ${install_prefix} -DSKIP_belasolv:BOOL=ON -DSKIP_csolv:BOOL=ON -DSKIP_liblua:BOOL=ON -DSKIP_ResizableLib:BOOL=ON -DSKIP_femm:BOOL=ON -DSKIP_femmplot:BOOL=ON   -DSKIP_fkn:BOOL=ON -DSKIP_hsolv:BOOL=ON -DSKIP_scifemm:BOOL=ON .."
    Write-Host "Configuring CMake project" -ForegroundColor Green
    Write-Host "CMake args: $cmake_args"
    $proc = Start-Process -NoNewWindow -PassThru -FilePath $CMAKE_EXE -ArgumentList $cmake_args
    $handle = $proc.Handle
    $proc.WaitForExit()
    $exitCode = $proc.ExitCode
    if (-Not ($exitCode -eq 0)) {
      MyThrow("Config failed! Exited with error code $exitCode.")
    }
    Write-Host "Building CMake project" -ForegroundColor Green
    $cmake_args = "--build . ${selectConfig} --parallel ${NumberOfBuildWorkers} --target install"
    Write-Host "CMake args: $cmake_args"
    $proc = Start-Process -NoNewWindow -PassThru -FilePath $CMAKE_EXE -ArgumentList $cmake_args
    $handle = $proc.Handle
    $proc.WaitForExit()
    $exitCode = $proc.ExitCode
    if (-Not ($exitCode -eq 0)) {
      MyThrow("Build failed! Exited with error code $exitCode.")
    }
  }
  else {
    New-Item -Path .\build_win_release64 -ItemType directory -Force | Out-Null
    Set-Location build_win_release64
    $cmake_args = "-A x64 $latexFOUND ${install_prefix} .."
    Write-Host "Configuring CMake project" -ForegroundColor Green
    Write-Host "CMake args: $cmake_args"
    $proc = Start-Process -NoNewWindow -PassThru -FilePath $CMAKE_EXE -ArgumentList $cmake_args
    $handle = $proc.Handle
    $proc.WaitForExit()
    $exitCode = $proc.ExitCode
    if (-Not ($exitCode -eq 0)) {
      MyThrow("Config failed! Exited with error code $exitCode.")
    }
    Write-Host "Building CMake project" -ForegroundColor Green
    $cmake_args = "--build . ${selectConfig} --parallel ${NumberOfBuildWorkers} --target install"
    Write-Host "CMake args: $cmake_args"
    $proc = Start-Process -NoNewWindow -PassThru -FilePath $CMAKE_EXE -ArgumentList $cmake_args
    $handle = $proc.Handle
    $proc.WaitForExit()
    $exitCode = $proc.ExitCode
    if (-Not ($exitCode -eq 0)) {
      MyThrow("Build failed! Exited with error code $exitCode.")
    }
  }
}

if ($BuildArch -eq "32") {
  New-Item -Path .\build_win_release32 -ItemType directory -Force | Out-Null
  Set-Location build_win_release32
  $cmake_args = "-A Win32 $latexFOUND ${install_prefix} .."
  Write-Host "Configuring CMake project" -ForegroundColor Green
  Write-Host "CMake args: $cmake_args"
  $proc = Start-Process -NoNewWindow -PassThru -FilePath $CMAKE_EXE -ArgumentList $cmake_args
  $handle = $proc.Handle
  $proc.WaitForExit()
  $exitCode = $proc.ExitCode
  if (-Not ($exitCode -eq 0)) {
    MyThrow("Config failed! Exited with error code $exitCode.")
  }
  Write-Host "Building CMake project" -ForegroundColor Green
  $cmake_args = "--build . ${selectConfig} --parallel ${NumberOfBuildWorkers} --target install"
  Write-Host "CMake args: $cmake_args"
  $proc = Start-Process -NoNewWindow -PassThru -FilePath $CMAKE_EXE -ArgumentList $cmake_args
  $handle = $proc.Handle
  $proc.WaitForExit()
  $exitCode = $proc.ExitCode
  if (-Not ($exitCode -eq 0)) {
    MyThrow("Build failed! Exited with error code $exitCode.")
  }
}

Set-Location ..

if (-Not $DoNotDeleteBuildFolder) {
  Write-Host "Removing build folders" -ForegroundColor Yellow
  Remove-Item -Force -Recurse -ErrorAction SilentlyContinue .\build_win_release64
  Remove-Item -Force -Recurse -ErrorAction SilentlyContinue .\build_win_release32_triangle
  Remove-Item -Force -Recurse -ErrorAction SilentlyContinue .\build_win_release32
}

Write-Host "Build complete!" -ForegroundColor Green
Pop-Location

$ErrorActionPreference = "SilentlyContinue"
Stop-Transcript | out-null
$ErrorActionPreference = "Continue"
