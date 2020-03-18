# Microsoft Developer Studio Project File - Name="femm" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=femm - Win32 Debug with Mathematica
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "femm.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "femm.mak" CFG="femm - Win32 Debug with Mathematica"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "femm - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "femm - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "femm - Win32 Release with Mathematica" (based on "Win32 (x86) Application")
!MESSAGE "femm - Win32 Debug with Mathematica" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "femm - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 ..\liblua\Release\liblua.lib ..\ResizableLib\Release\ResizableLib.lib /nologo /subsystem:windows /map /machine:I386 /out:"..\Release/femm_nomath.exe"

!ELSEIF  "$(CFG)" == "femm - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ..\liblua\Debug\liblua.lib ..\ResizableLib\debug\ResizableLib.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:"..\Debug/femm.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "femm - Win32 Release with Mathematica"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "femm___Win32_Release_with_Mathematica"
# PROP BASE Intermediate_Dir "femm___Win32_Release_with_Mathematica"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "femm___Win32_Release_with_Mathematica"
# PROP Intermediate_Dir "femm___Win32_Release_with_Mathematica"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "MATHLINK" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\liblua\Release\liblua.lib ..\ResizableLib\Release\ResizableLib.lib /nologo /subsystem:windows /map /machine:I386 /out:"..\Release/femm.exe"
# ADD LINK32 "c:\Program Files\Microsoft Visual Studio\VC98\Lib\ml32i2m.lib" ..\liblua\Release\liblua.lib ..\ResizableLib\Release\ResizableLib.lib /nologo /subsystem:windows /map /machine:I386 /out:"..\Release/femm_math.exe"

!ELSEIF  "$(CFG)" == "femm - Win32 Debug with Mathematica"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "femm___Win32_Debug_with_Mathematica"
# PROP BASE Intermediate_Dir "femm___Win32_Debug_with_Mathematica"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "femm___Win32_Debug_with_Mathematica"
# PROP Intermediate_Dir "femm___Win32_Debug_with_Mathematica"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /D "MATHLINK" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\liblua\Debug\liblua.lib ..\ResizableLib\debug\ResizableLib.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:"..\Debug/femm.exe" /pdbtype:sept
# ADD LINK32 "c:\Program Files\Microsoft Visual Studio\VC98\Lib\ml32i2m.lib" ..\liblua\Debug\liblua.lib ..\ResizableLib\debug\ResizableLib.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:"..\Debug/femm.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "femm - Win32 Release"
# Name "femm - Win32 Debug"
# Name "femm - Win32 Release with Mathematica"
# Name "femm - Win32 Debug with Mathematica"
# Begin Group "femme"

# PROP Default_Filter ""
# Begin Group "Femme Source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BdryDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BHData.cpp
# End Source File
# Begin Source File

SOURCE=.\BHDatafile.cpp
# End Source File
# Begin Source File

SOURCE=.\CircProp.cpp
# End Source File
# Begin Source File

SOURCE=.\CopyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DXFImport.cpp
# End Source File
# Begin Source File

SOURCE=.\fe_libdlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FemmeDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\femmeLua.cpp
# End Source File
# Begin Source File

SOURCE=.\FemmeView.cpp
# End Source File
# Begin Source File

SOURCE=.\GroupNumber.cpp
# End Source File
# Begin Source File

SOURCE=.\MatDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MirrorDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MOVECOPY.CPP
# End Source File
# Begin Source File

SOURCE=.\NodeProp.cpp
# End Source File
# Begin Source File

SOURCE=.\NOSEBL.CPP
# End Source File
# Begin Source File

SOURCE=.\OpArcSegDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\OpBlkDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\OpGrp.cpp
# End Source File
# Begin Source File

SOURCE=.\OpNodeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\OpSegDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Pref.cpp
# End Source File
# Begin Source File

SOURCE=.\probdlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PtProp.cpp
# End Source File
# Begin Source File

SOURCE=.\ScaleDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\writepoly.cpp
# End Source File
# End Group
# Begin Group "Femme Header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BdryDlg.h
# End Source File
# Begin Source File

SOURCE=.\BHData.h
# End Source File
# Begin Source File

SOURCE=.\BHDatafile.h
# End Source File
# Begin Source File

SOURCE=.\CircProp.h
# End Source File
# Begin Source File

SOURCE=.\CopyDlg.h
# End Source File
# Begin Source File

SOURCE=.\fe_libdlg.h
# End Source File
# Begin Source File

SOURCE=.\FemmeDoc.h
# End Source File
# Begin Source File

SOURCE=.\FemmeView.h
# End Source File
# Begin Source File

SOURCE=.\GroupNumber.h
# End Source File
# Begin Source File

SOURCE=.\MatDlg.h
# End Source File
# Begin Source File

SOURCE=.\MirrorDlg.h
# End Source File
# Begin Source File

SOURCE=.\NodeProp.h
# End Source File
# Begin Source File

SOURCE=.\NOSEBL.H
# End Source File
# Begin Source File

SOURCE=.\OpArcSegDlg.h
# End Source File
# Begin Source File

SOURCE=.\OpBlkDlg.h
# End Source File
# Begin Source File

SOURCE=.\OpGrp.h
# End Source File
# Begin Source File

SOURCE=.\OpNodeDlg.h
# End Source File
# Begin Source File

SOURCE=.\OpSegDlg.h
# End Source File
# Begin Source File

SOURCE=.\Pref.h
# End Source File
# Begin Source File

SOURCE=.\probdlg.h
# End Source File
# Begin Source File

SOURCE=.\PtProp.h
# End Source File
# Begin Source File

SOURCE=.\ScaleDlg.h
# End Source File
# End Group
# End Group
# Begin Group "common"

# PROP Default_Filter ""
# Begin Group "Common Source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ArcDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\bhplot.cpp
# End Source File
# Begin Source File

SOURCE=.\EnterPt.cpp
# End Source File
# Begin Source File

SOURCE=.\ExteriorProps.cpp
# End Source File
# Begin Source File

SOURCE=.\fullmatrix.cpp
# End Source File
# Begin Source File

SOURCE=.\GeneralPrefs.cpp
# End Source File
# Begin Source File

SOURCE=.\GRIDDLG.CPP
# End Source File
# Begin Source File

SOURCE=.\KbdZoom.cpp
# End Source File
# Begin Source File

SOURCE=.\LuaConsoleDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\luaDDX.cpp
# End Source File
# Begin Source File

SOURCE=.\LuaEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\MyTabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\OutBox.cpp
# End Source File
# Begin Source File

SOURCE=.\PromptBox.cpp
# End Source File
# Begin Source File

SOURCE=.\spars.cpp
# End Source File
# Begin Source File

SOURCE=.\Xyplot.cpp
# End Source File
# End Group
# Begin Group "Common Header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ArcDlg.h
# End Source File
# Begin Source File

SOURCE=.\bhplot.h
# End Source File
# Begin Source File

SOURCE=.\EnterPt.h
# End Source File
# Begin Source File

SOURCE=.\ExteriorProps.h
# End Source File
# Begin Source File

SOURCE=.\fullmatrix.h
# End Source File
# Begin Source File

SOURCE=.\GeneralPrefs.h
# End Source File
# Begin Source File

SOURCE=.\GRIDDLG.H
# End Source File
# Begin Source File

SOURCE=.\KbdZoom.h
# End Source File
# Begin Source File

SOURCE=.\LuaConsoleDlg.h
# End Source File
# Begin Source File

SOURCE=.\luaDDX.h
# End Source File
# Begin Source File

SOURCE=.\LuaEdit.h
# End Source File
# Begin Source File

SOURCE=.\MyTabCtrl.h
# End Source File
# Begin Source File

SOURCE=.\OutBox.h
# End Source File
# Begin Source File

SOURCE=.\PromptBox.h
# End Source File
# Begin Source File

SOURCE=.\ResizableDialog.h
# End Source File
# Begin Source File

SOURCE=.\ResizableGrip.h
# End Source File
# Begin Source File

SOURCE=.\ResizableLayout.h
# End Source File
# Begin Source File

SOURCE=.\ResizableMinMax.h
# End Source File
# Begin Source File

SOURCE=.\ResizableState.h
# End Source File
# Begin Source File

SOURCE=.\spars.h
# End Source File
# Begin Source File

SOURCE=.\Xyplot.h
# End Source File
# End Group
# End Group
# Begin Group "femmview"

# PROP Default_Filter ""
# Begin Group "Femmview Source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BendContourDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BlockInt.cpp
# End Source File
# Begin Source File

SOURCE=.\CircDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CPlotDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CplotDlg2.cpp
# End Source File
# Begin Source File

SOURCE=.\FemmviewDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\femmviewLua.cpp
# End Source File
# Begin Source File

SOURCE=.\FemmviewView.cpp
# End Source File
# Begin Source File

SOURCE=.\GridMod.cpp
# End Source File
# Begin Source File

SOURCE=.\LIntDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\makemask.cpp
# End Source File
# Begin Source File

SOURCE=.\MaskProgress.cpp
# End Source File
# Begin Source File

SOURCE=.\MyMsgBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Problem.cpp
# End Source File
# Begin Source File

SOURCE=.\viewpref.cpp
# End Source File
# Begin Source File

SOURCE=.\VPlotDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\XYPlotDlg.cpp
# End Source File
# End Group
# Begin Group "Femmview Header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BendContourDlg.h
# End Source File
# Begin Source File

SOURCE=.\BlockInt.h
# End Source File
# Begin Source File

SOURCE=.\CircDlg.h
# End Source File
# Begin Source File

SOURCE=.\CPlotDlg.h
# End Source File
# Begin Source File

SOURCE=.\CplotDlg2.h
# End Source File
# Begin Source File

SOURCE=.\DPlotDlg1.h
# End Source File
# Begin Source File

SOURCE=.\DPlotDlg2.h
# End Source File
# Begin Source File

SOURCE=.\FemmviewDoc.h
# End Source File
# Begin Source File

SOURCE=.\FemmviewView.h
# End Source File
# Begin Source File

SOURCE=.\GridMod.h
# End Source File
# Begin Source File

SOURCE=.\LIntDlg.h
# End Source File
# Begin Source File

SOURCE=.\MaskProgress.h
# End Source File
# Begin Source File

SOURCE=.\MyMsgBox.h
# End Source File
# Begin Source File

SOURCE=.\Problem.h
# End Source File
# Begin Source File

SOURCE=.\viewpref.h
# End Source File
# Begin Source File

SOURCE=.\VPlotDlg.h
# End Source File
# Begin Source File

SOURCE=.\XYPlotDlg.h
# End Source File
# End Group
# End Group
# Begin Group "MDI Shell"

# PROP Default_Filter ""
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ActiveFEMM.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\femm.cpp
# End Source File
# Begin Source File

SOURCE=.\femm.odl

!IF  "$(CFG)" == "femm - Win32 Release"

# ADD MTL /tlb "..\Release/femm.tlb"

!ELSEIF  "$(CFG)" == "femm - Win32 Debug"

# ADD MTL /tlb "..\Debug/femm.tlb"

!ELSEIF  "$(CFG)" == "femm - Win32 Release with Mathematica"

# ADD BASE MTL /tlb "..\Release/femm.tlb"
# ADD MTL /tlb "..\Release/femm.tlb"

!ELSEIF  "$(CFG)" == "femm - Win32 Debug with Mathematica"

# ADD BASE MTL /tlb "..\Debug/femm.tlb"
# ADD MTL /tlb "..\Debug/femm.tlb"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\femm.rc
# End Source File
# Begin Source File

SOURCE=.\LibFolderInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MDITabs.cpp
# End Source File
# Begin Source File

SOURCE=.\MyCommandLineInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\MyRecentFileList.cpp
# End Source File
# Begin Source File

SOURCE=.\NewDocDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Preferences.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ActiveFEMM.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\COMPLEX.H
# End Source File
# Begin Source File

SOURCE=.\DXFImport.h
# End Source File
# Begin Source File

SOURCE=.\femm.h
# End Source File
# Begin Source File

SOURCE=.\LibFolderInfo.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MDITabs.h
# End Source File
# Begin Source File

SOURCE=.\MyCommandLineInfo.h
# End Source File
# Begin Source File

SOURCE=.\MyRecentFileList.h
# End Source File
# Begin Source File

SOURCE=.\NewDocDlg.h
# End Source File
# Begin Source File

SOURCE=.\Preferences.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00005.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00006.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00007.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00013.bmp
# End Source File
# Begin Source File

SOURCE=.\res\femm.rc2
# End Source File
# Begin Source File

SOURCE=.\res\femmetyp.bmp
# End Source File
# Begin Source File

SOURCE=.\res\femmtype.bmp
# End Source File
# Begin Source File

SOURCE=.\res\femmview.bmp
# End Source File
# Begin Source File

SOURCE=.\res\hdrawtyp.bmp
# End Source File
# Begin Source File

SOURCE=.\res\hviewtyp.bmp
# End Source File
# Begin Source File

SOURCE=.\res\idr_beladrawtype.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_belaviewtype.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_cdrawtype.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_cviewtype.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_femmetype.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_femmplottype.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_femmviewtype.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_hdra.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_hdrawtype.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_hvie.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_hviewtype.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_mainframe.ico
# End Source File
# Begin Source File

SOURCE=.\res\left1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\left2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\main1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\main2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tree.bmp
# End Source File
# Begin Source File

SOURCE=.\res\viewtool.bmp
# End Source File
# End Group
# End Group
# Begin Group "beladraw"

# PROP Default_Filter ""
# Begin Group "beladraw source"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=.\bd_BdryDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\bd_CircProp.cpp
# End Source File
# Begin Source File

SOURCE=.\bd_libdlg.cpp
# End Source File
# Begin Source File

SOURCE=.\bd_MatDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\bd_movecopy.cpp
# End Source File
# Begin Source File

SOURCE=.\bd_NodeProp.cpp
# End Source File
# Begin Source File

SOURCE=.\bd_nosebl.cpp
# End Source File
# Begin Source File

SOURCE=.\bd_OpArcSegDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\bd_OpBlkDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\bd_OpNodeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\bd_OpSegDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\bd_Pref.cpp
# End Source File
# Begin Source File

SOURCE=.\bd_probdlg.cpp
# End Source File
# Begin Source File

SOURCE=.\bd_PtProp.cpp
# End Source File
# Begin Source File

SOURCE=.\bd_writepoly.cpp
# End Source File
# Begin Source File

SOURCE=.\beladrawDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\beladrawLua.cpp
# End Source File
# Begin Source File

SOURCE=.\beladrawView.cpp
# End Source File
# End Group
# Begin Group "beladraw header"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\bd_BdryDlg.h
# End Source File
# Begin Source File

SOURCE=.\bd_CircProp.h
# End Source File
# Begin Source File

SOURCE=.\bd_libdlg.h
# End Source File
# Begin Source File

SOURCE=.\bd_MatDlg.h
# End Source File
# Begin Source File

SOURCE=.\bd_NodeProp.h
# End Source File
# Begin Source File

SOURCE=.\bd_nosebl.h
# End Source File
# Begin Source File

SOURCE=.\bd_OpArcSegDlg.h
# End Source File
# Begin Source File

SOURCE=.\bd_OpBlkDlg.h
# End Source File
# Begin Source File

SOURCE=.\bd_OpNodeDlg.h
# End Source File
# Begin Source File

SOURCE=.\bd_OpSegDlg.h
# End Source File
# Begin Source File

SOURCE=.\bd_Pref.h
# End Source File
# Begin Source File

SOURCE=.\bd_probdlg.h
# End Source File
# Begin Source File

SOURCE=.\bd_PtProp.h
# End Source File
# Begin Source File

SOURCE=.\beladrawDoc.h
# End Source File
# Begin Source File

SOURCE=.\beladrawView.h
# End Source File
# End Group
# End Group
# Begin Group "belaview"

# PROP Default_Filter ""
# Begin Group "belaview source"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\belaviewDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\belaviewLua.cpp
# End Source File
# Begin Source File

SOURCE=.\belaviewView.cpp
# End Source File
# Begin Source File

SOURCE=.\bv_BlockInt.cpp
# End Source File
# Begin Source File

SOURCE=.\bv_CircDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\bv_CplotDlg2.cpp
# End Source File
# Begin Source File

SOURCE=.\bv_DPlotDlg2.cpp
# End Source File
# Begin Source File

SOURCE=.\bv_LIntDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\bv_makemask.cpp
# End Source File
# Begin Source File

SOURCE=.\bv_Pref.cpp
# End Source File
# Begin Source File

SOURCE=.\bv_problem.cpp
# End Source File
# Begin Source File

SOURCE=.\bv_VPlotDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\bv_XYPlotDlg.cpp
# End Source File
# End Group
# Begin Group "belaview header"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\belaviewDoc.h
# End Source File
# Begin Source File

SOURCE=.\belaviewView.h
# End Source File
# Begin Source File

SOURCE=.\bv_BlockInt.h
# End Source File
# Begin Source File

SOURCE=.\bv_CircDlg.h
# End Source File
# Begin Source File

SOURCE=.\bv_CplotDlg2.h
# End Source File
# Begin Source File

SOURCE=.\bv_DPlotDlg2.h
# End Source File
# Begin Source File

SOURCE=.\bv_LIntDlg.h
# End Source File
# Begin Source File

SOURCE=.\bv_Pref.h
# End Source File
# Begin Source File

SOURCE=.\bv_problem.h
# End Source File
# Begin Source File

SOURCE=.\bv_VPlotDlg.h
# End Source File
# Begin Source File

SOURCE=.\bv_XYPlotDlg.h
# End Source File
# End Group
# End Group
# Begin Group "femmplot"

# PROP Default_Filter ""
# Begin Group "femmplot source"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=.\femmplotDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\femmplotView.cpp
# End Source File
# End Group
# Begin Group "femmplot header"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\femmplotDoc.h
# End Source File
# Begin Source File

SOURCE=.\femmplotView.h
# End Source File
# End Group
# End Group
# Begin Group "hdraw"

# PROP Default_Filter ""
# Begin Group "hdraw source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\hd_BdryDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\hd_CircProp.cpp
# End Source File
# Begin Source File

SOURCE=.\hd_libdlg.cpp
# End Source File
# Begin Source File

SOURCE=.\hd_MatDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\hd_movecopy.cpp
# End Source File
# Begin Source File

SOURCE=.\hd_NodeProp.cpp
# End Source File
# Begin Source File

SOURCE=.\hd_nosebl.cpp
# End Source File
# Begin Source File

SOURCE=.\hd_OpArcSegDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\hd_OpBlkDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\hd_OpNodeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\hd_OpSegDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\HD_PREF.CPP
# End Source File
# Begin Source File

SOURCE=.\hd_probdlg.cpp
# End Source File
# Begin Source File

SOURCE=.\hd_PtProp.cpp
# End Source File
# Begin Source File

SOURCE=.\hd_writepoly.cpp
# End Source File
# Begin Source File

SOURCE=.\HDRAWDOC.CPP
# End Source File
# Begin Source File

SOURCE=.\HDRAWLUA.CPP
# End Source File
# Begin Source File

SOURCE=.\hdrawView.cpp
# End Source File
# Begin Source File

SOURCE=.\KCurve.cpp
# End Source File
# Begin Source File

SOURCE=.\TKDatafile.cpp
# End Source File
# End Group
# Begin Group "hdraw headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\hd_BdryDlg.h
# End Source File
# Begin Source File

SOURCE=.\hd_CircProp.h
# End Source File
# Begin Source File

SOURCE=.\hd_libdlg.h
# End Source File
# Begin Source File

SOURCE=.\hd_MatDlg.h
# End Source File
# Begin Source File

SOURCE=.\hd_NodeProp.h
# End Source File
# Begin Source File

SOURCE=.\hd_nosebl.h
# End Source File
# Begin Source File

SOURCE=.\hd_OpArcSegDlg.h
# End Source File
# Begin Source File

SOURCE=.\hd_OpBlkDlg.h
# End Source File
# Begin Source File

SOURCE=.\hd_OpNodeDlg.h
# End Source File
# Begin Source File

SOURCE=.\hd_OpSegDlg.h
# End Source File
# Begin Source File

SOURCE=.\HD_PREF.H
# End Source File
# Begin Source File

SOURCE=.\hd_probdlg.h
# End Source File
# Begin Source File

SOURCE=.\hd_PtProp.h
# End Source File
# Begin Source File

SOURCE=.\HDRAWDOC.H
# End Source File
# Begin Source File

SOURCE=.\hdrawView.h
# End Source File
# Begin Source File

SOURCE=.\KCurve.h
# End Source File
# Begin Source File

SOURCE=.\TKDatafile.h
# End Source File
# End Group
# End Group
# Begin Group "hview"

# PROP Default_Filter ""
# Begin Group "hview source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\hv_BlockInt.cpp
# End Source File
# Begin Source File

SOURCE=.\hv_CircDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\hv_CplotDlg2.cpp
# End Source File
# Begin Source File

SOURCE=.\hv_DPlotDlg2.cpp
# End Source File
# Begin Source File

SOURCE=.\hv_LIntDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\HV_PREF.CPP
# End Source File
# Begin Source File

SOURCE=.\hv_problem.cpp
# End Source File
# Begin Source File

SOURCE=.\hv_VPlotDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\hv_XYPlotDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\hviewDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\hviewLua.cpp
# End Source File
# Begin Source File

SOURCE=.\hviewView.cpp
# End Source File
# End Group
# Begin Group "hview headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\hv_BlockInt.h
# End Source File
# Begin Source File

SOURCE=.\hv_CircDlg.h
# End Source File
# Begin Source File

SOURCE=.\hv_CplotDlg2.h
# End Source File
# Begin Source File

SOURCE=.\hv_DPlotDlg2.h
# End Source File
# Begin Source File

SOURCE=.\hv_LIntDlg.h
# End Source File
# Begin Source File

SOURCE=.\HV_PREF.H
# End Source File
# Begin Source File

SOURCE=.\hv_problem.h
# End Source File
# Begin Source File

SOURCE=.\hv_VPlotDlg.h
# End Source File
# Begin Source File

SOURCE=.\hv_XYPlotDlg.h
# End Source File
# Begin Source File

SOURCE=.\hviewDoc.h
# End Source File
# Begin Source File

SOURCE=.\hviewView.h
# End Source File
# End Group
# End Group
# Begin Group "cdraw"

# PROP Default_Filter ""
# Begin Group "cdraw source"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=.\cd_BdryDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\cd_CircProp.cpp
# End Source File
# Begin Source File

SOURCE=.\cd_libdlg.cpp
# End Source File
# Begin Source File

SOURCE=.\cd_MatDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\cd_movecopy.cpp
# End Source File
# Begin Source File

SOURCE=.\cd_NodeProp.cpp
# End Source File
# Begin Source File

SOURCE=.\cd_nosebl.cpp
# End Source File
# Begin Source File

SOURCE=.\cd_OpArcSegDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\cd_OpBlkDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\cd_OpNodeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\cd_OpSegDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CD_PREF.CPP
# End Source File
# Begin Source File

SOURCE=.\cd_probdlg.cpp
# End Source File
# Begin Source File

SOURCE=.\cd_PtProp.cpp
# End Source File
# Begin Source File

SOURCE=.\cd_writepoly.cpp
# End Source File
# Begin Source File

SOURCE=.\CDRAWDOC.CPP
# End Source File
# Begin Source File

SOURCE=.\CDRAWLUA.CPP
# End Source File
# Begin Source File

SOURCE=.\cdrawView.cpp
# End Source File
# End Group
# Begin Group "cdraw headers"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\cd_BdryDlg.h
# End Source File
# Begin Source File

SOURCE=.\cd_CircProp.h
# End Source File
# Begin Source File

SOURCE=.\cd_libdlg.h
# End Source File
# Begin Source File

SOURCE=.\cd_MatDlg.h
# End Source File
# Begin Source File

SOURCE=.\cd_NodeProp.h
# End Source File
# Begin Source File

SOURCE=.\cd_nosebl.h
# End Source File
# Begin Source File

SOURCE=.\cd_OpArcSegDlg.h
# End Source File
# Begin Source File

SOURCE=.\cd_OpBlkDlg.h
# End Source File
# Begin Source File

SOURCE=.\cd_OpNodeDlg.h
# End Source File
# Begin Source File

SOURCE=.\cd_OpSegDlg.h
# End Source File
# Begin Source File

SOURCE=.\CD_PREF.H
# End Source File
# Begin Source File

SOURCE=.\cd_probdlg.h
# End Source File
# Begin Source File

SOURCE=.\cd_PtProp.h
# End Source File
# Begin Source File

SOURCE=.\CDRAWDOC.H
# End Source File
# Begin Source File

SOURCE=.\cdrawView.h
# End Source File
# End Group
# End Group
# Begin Group "cview"

# PROP Default_Filter ""
# Begin Group "cview source"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=.\cv_BlockInt.cpp
# End Source File
# Begin Source File

SOURCE=.\cv_CircDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\cv_CplotDlg2.cpp
# End Source File
# Begin Source File

SOURCE=.\cv_DPlotDlg2.cpp
# End Source File
# Begin Source File

SOURCE=.\cv_LIntDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\cv_makemask.cpp
# End Source File
# Begin Source File

SOURCE=.\CV_PREF.CPP
# End Source File
# Begin Source File

SOURCE=.\cv_problem.cpp
# End Source File
# Begin Source File

SOURCE=.\cv_VPlotDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\cv_XYPlotDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CVIEWDOC.CPP
# End Source File
# Begin Source File

SOURCE=.\CVIEWLUA.CPP
# End Source File
# Begin Source File

SOURCE=.\cviewView.cpp
# End Source File
# End Group
# Begin Group "cview headers"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\cv_BlockInt.h
# End Source File
# Begin Source File

SOURCE=.\cv_CircDlg.h
# End Source File
# Begin Source File

SOURCE=.\cv_CplotDlg2.h
# End Source File
# Begin Source File

SOURCE=.\cv_DPlotDlg2.h
# End Source File
# Begin Source File

SOURCE=.\cv_LIntDlg.h
# End Source File
# Begin Source File

SOURCE=.\CV_PREF.H
# End Source File
# Begin Source File

SOURCE=.\cv_problem.h
# End Source File
# Begin Source File

SOURCE=.\cv_VPlotDlg.h
# End Source File
# Begin Source File

SOURCE=.\cv_XYPlotDlg.h
# End Source File
# Begin Source File

SOURCE=.\CVIEWDOC.H
# End Source File
# Begin Source File

SOURCE=.\cviewView.h
# End Source File
# End Group
# End Group
# Begin Source File

SOURCE=.\femm.reg
# End Source File
# Begin Source File

SOURCE=.\res\femm.tlb
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
