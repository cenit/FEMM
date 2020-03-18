# Microsoft Developer Studio Project File - Name="liblua" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=liblua - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "liblua.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "liblua.mak" CFG="liblua - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "liblua - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "liblua - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "liblua - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "liblua - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MDd /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "liblua - Win32 Release"
# Name "liblua - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\COMPLEX.CPP
# End Source File
# Begin Source File

SOURCE=.\lapi.cpp
# End Source File
# Begin Source File

SOURCE=.\lauxlib.cpp
# End Source File
# Begin Source File

SOURCE=.\lbaselib.cpp
# End Source File
# Begin Source File

SOURCE=.\lcode.cpp
# End Source File
# Begin Source File

SOURCE=.\ldblib.cpp
# End Source File
# Begin Source File

SOURCE=.\ldebug.cpp
# End Source File
# Begin Source File

SOURCE=.\ldo.cpp
# End Source File
# Begin Source File

SOURCE=.\lfunc.cpp
# End Source File
# Begin Source File

SOURCE=.\lgc.cpp
# End Source File
# Begin Source File

SOURCE=.\liolib.cpp
# End Source File
# Begin Source File

SOURCE=.\llex.cpp
# End Source File
# Begin Source File

SOURCE=.\lmathlib.cpp
# End Source File
# Begin Source File

SOURCE=.\lmem.cpp
# End Source File
# Begin Source File

SOURCE=.\lobject.cpp
# End Source File
# Begin Source File

SOURCE=.\lparser.cpp
# End Source File
# Begin Source File

SOURCE=.\lstate.cpp
# End Source File
# Begin Source File

SOURCE=.\lstring.cpp
# End Source File
# Begin Source File

SOURCE=.\lstrlib.cpp
# End Source File
# Begin Source File

SOURCE=.\ltable.cpp
# End Source File
# Begin Source File

SOURCE=.\ltests.cpp
# End Source File
# Begin Source File

SOURCE=.\ltm.cpp
# End Source File
# Begin Source File

SOURCE=.\lundump.cpp
# End Source File
# Begin Source File

SOURCE=.\lvm.cpp
# End Source File
# Begin Source File

SOURCE=.\lzio.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\COMPLEX.H
# End Source File
# Begin Source File

SOURCE=.\lapi.h
# End Source File
# Begin Source File

SOURCE=.\lauxlib.h
# End Source File
# Begin Source File

SOURCE=.\lcode.h
# End Source File
# Begin Source File

SOURCE=.\ldebug.h
# End Source File
# Begin Source File

SOURCE=.\ldo.h
# End Source File
# Begin Source File

SOURCE=.\lfunc.h
# End Source File
# Begin Source File

SOURCE=.\lgc.h
# End Source File
# Begin Source File

SOURCE=.\llex.h
# End Source File
# Begin Source File

SOURCE=.\llimits.h
# End Source File
# Begin Source File

SOURCE=.\lmem.h
# End Source File
# Begin Source File

SOURCE=.\lobject.h
# End Source File
# Begin Source File

SOURCE=.\lopcodes.h
# End Source File
# Begin Source File

SOURCE=.\lparser.h
# End Source File
# Begin Source File

SOURCE=.\lstate.h
# End Source File
# Begin Source File

SOURCE=.\lstring.h
# End Source File
# Begin Source File

SOURCE=.\ltable.h
# End Source File
# Begin Source File

SOURCE=.\ltm.h
# End Source File
# Begin Source File

SOURCE=.\lua.h
# End Source File
# Begin Source File

SOURCE=.\luadebug.h
# End Source File
# Begin Source File

SOURCE=.\lualib.h
# End Source File
# Begin Source File

SOURCE=.\lundump.h
# End Source File
# Begin Source File

SOURCE=.\lvm.h
# End Source File
# Begin Source File

SOURCE=.\lzio.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
