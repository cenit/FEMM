!define PROJECT_NAME "FEMM"
!define PROJECT_REG_UNINSTALL_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PROJECT_NAME}"
!define PROJECT_UNINSTALL_EXE "uninstall.exe"
!include LogicLib.nsh

Unicode True
Page Directory
Page InstFiles

# define name of installer
OutFile "${PROJECT_NAME}_installer.exe"

# define installation directory
InstallDir "$APPDATA\${PROJECT_NAME}"

# We do not need any admin privilege
RequestExecutionLevel user

# start default section
Section
    ClearErrors
    EnumRegKey $0 HKCU "${PROJECT_REG_UNINSTALL_KEY}" "QuietUninstallString"
    IfErrors ContinueInstall KeyExist
    KeyExist:
        ReadRegStr $R0 HKCU "${PROJECT_REG_UNINSTALL_KEY}" "QuietUninstallString"
        ExecWait "$R0"

    ContinueInstall:
    # set the installation directory as the destination for the following actions
    SetOutPath "$INSTDIR"
    File "bin\belasolv.exe"
    File "bin\condlib.dat"
    File "bin\csolv.exe"
    File "bin\femm.exe"
    File "bin\femmplot.exe"
    File "bin\fkn.exe"
    File "bin\heatlib.dat"
    File "bin\hsolv.exe"
    File "bin\init.lua"
    File "bin\matlib.dat"
    File "bin\statlib.dat"
    File "bin\triangle.exe"

    # create the uninstaller and a link to it in the start menu
    WriteUninstaller "$INSTDIR\${PROJECT_UNINSTALL_EXE}"

    SetOutPath "$STARTMENU\Programs\${PROJECT_NAME}"
    CreateShortcut "$SMPROGRAMS\${PROJECT_NAME}\FEMM.lnk" "$INSTDIR\femm.exe"
    CreateShortcut "$SMPROGRAMS\${PROJECT_NAME}\Uninstall.lnk" "$INSTDIR\${PROJECT_UNINSTALL_EXE}"

    WriteRegStr HKCU "${PROJECT_REG_UNINSTALL_KEY}" "UninstallString" '"$INSTDIR\${PROJECT_UNINSTALL_EXE}" _?=$INSTDIR'
    WriteRegStr HKCU "${PROJECT_REG_UNINSTALL_KEY}" "QuietUninstallString" '"$INSTDIR\${PROJECT_UNINSTALL_EXE}" /S _?=$INSTDIR'
SectionEnd

# uninstaller section start
Section "uninstall"
    # delete the EXEs and the DLLs
    Delete "$INSTDIR\belasolv.exe"
    Delete "$INSTDIR\condlib.dat"
    Delete "$INSTDIR\csolv.exe"
    Delete "$INSTDIR\femm.exe"
    Delete "$INSTDIR\femmplot.exe"
    Delete "$INSTDIR\fkn.exe"
    Delete "$INSTDIR\heatlib.dat"
    Delete "$INSTDIR\hsolv.exe"
    Delete "$INSTDIR\init.lua"
    Delete "$INSTDIR\license.txt"
    Delete "$INSTDIR\matlib.dat"
    Delete "$INSTDIR\statlib.dat"
    Delete "$INSTDIR\triangle.exe"

    # delete the uninstaller
    Delete "$INSTDIR\${PROJECT_UNINSTALL_EXE}"

    # remove the links from the start menu
    Delete "$SMPROGRAMS\${PROJECT_NAME}\FEMM.lnk"
    Delete "$SMPROGRAMS\${PROJECT_NAME}\Uninstall.lnk"
    RMDir "$STARTMENU\Programs\${PROJECT_NAME}"

    # remove the installdir
    RMDir "$INSTDIR"

    DeleteRegKey HKCU "${PROJECT_REG_UNINSTALL_KEY}"
SectionEnd
