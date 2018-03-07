# Microsoft Developer Studio Project File - Name="LSPC" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=LSPC - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "LSPC.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "LSPC.mak" CFG="LSPC - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "LSPC - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "LSPC - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/LSPC", KSBAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
F90=df.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "LSPC - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# SUBTRACT F90 /browser
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "NEWNETWORK" /D "LIMITEDVERSION" /D "PPP" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /incremental:yes /machine:I386 /out:"Release/LSPCModel.exe"
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "LSPC - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# SUBTRACT F90 /browser
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /Gm /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"Debug/LSPCModel.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "LSPC - Win32 Release"
# Name "LSPC - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "dbf"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SRC\ALXParser.cpp
# End Source File
# Begin Source File

SOURCE=.\SRC\ALXParserException.cpp
# End Source File
# Begin Source File

SOURCE=.\SRC\ALXParserFuncDef.cpp
# End Source File
# Begin Source File

SOURCE=.\SRC\ALXParserOperDef.cpp
# End Source File
# Begin Source File

SOURCE=.\SRC\ALXSyntaxEditCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\SRC\DBFCursorSet.cpp
# End Source File
# Begin Source File

SOURCE=.\SRC\DBFException.cpp
# End Source File
# Begin Source File

SOURCE=.\SRC\DBFFields.cpp
# End Source File
# Begin Source File

SOURCE=.\SRC\DBFMemCursorSet.cpp
# End Source File
# Begin Source File

SOURCE=.\SRC\DBFRecordset.cpp
# End Source File
# Begin Source File

SOURCE=.\SRC\DBFTableDef.cpp
# End Source File
# Begin Source File

SOURCE=.\SRC\DBTMemoFile.cpp
# End Source File
# Begin Source File

SOURCE=.\SRC\FPTMemoFile.cpp
# End Source File
# Begin Source File

SOURCE=.\SRC\MemoFile.cpp
# End Source File
# Begin Source File

SOURCE=.\SRC\SMTMemoFile.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\CommentsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DirDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCrossSection.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgWQ.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\GridDropTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\InPlaceEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\InPlaceList.cpp
# End Source File
# Begin Source File

SOURCE=.\Landuse.cpp
# End Source File
# Begin Source File

SOURCE=.\LeftView.cpp
# End Source File
# Begin Source File

SOURCE=.\LSPC.cpp
# End Source File
# Begin Source File

SOURCE=.\LSPCDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\LSPCMODEL.cpp
# End Source File
# Begin Source File

SOURCE=.\LSPCView.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MyPropertySheet1.cpp
# End Source File
# Begin Source File

SOURCE=.\MyPropertySheet2.cpp
# End Source File
# Begin Source File

SOURCE=.\Output.cpp
# End Source File
# Begin Source File

SOURCE=.\Page0.cpp
# End Source File
# Begin Source File

SOURCE=.\Page10.cpp
# End Source File
# Begin Source File

SOURCE=.\Page100.cpp
# End Source File
# Begin Source File

SOURCE=.\Page110.cpp
# End Source File
# Begin Source File

SOURCE=.\Page120.cpp
# End Source File
# Begin Source File

SOURCE=.\Page130.cpp
# End Source File
# Begin Source File

SOURCE=.\Page140.cpp
# End Source File
# Begin Source File

SOURCE=.\Page15.cpp
# End Source File
# Begin Source File

SOURCE=.\Page150.cpp
# End Source File
# Begin Source File

SOURCE=.\Page160.cpp
# End Source File
# Begin Source File

SOURCE=.\Page170.cpp
# End Source File
# Begin Source File

SOURCE=.\Page180.cpp
# End Source File
# Begin Source File

SOURCE=.\Page190.cpp
# End Source File
# Begin Source File

SOURCE=.\Page20.cpp
# End Source File
# Begin Source File

SOURCE=.\Page200.cpp
# End Source File
# Begin Source File

SOURCE=.\Page201.cpp
# End Source File
# Begin Source File

SOURCE=.\Page202.cpp
# End Source File
# Begin Source File

SOURCE=.\Page203.cpp
# End Source File
# Begin Source File

SOURCE=.\Page204.cpp
# End Source File
# Begin Source File

SOURCE=.\Page205.cpp
# End Source File
# Begin Source File

SOURCE=.\Page250.cpp
# End Source File
# Begin Source File

SOURCE=.\Page255.cpp
# End Source File
# Begin Source File

SOURCE=.\Page260.cpp
# End Source File
# Begin Source File

SOURCE=.\Page270.cpp
# End Source File
# Begin Source File

SOURCE=.\Page271.cpp
# End Source File
# Begin Source File

SOURCE=.\Page272.cpp
# End Source File
# Begin Source File

SOURCE=.\Page273.cpp
# End Source File
# Begin Source File

SOURCE=.\Page274.cpp
# End Source File
# Begin Source File

SOURCE=.\Page275.cpp
# End Source File
# Begin Source File

SOURCE=.\Page276.cpp
# End Source File
# Begin Source File

SOURCE=.\Page280.cpp
# End Source File
# Begin Source File

SOURCE=.\Page281.cpp
# End Source File
# Begin Source File

SOURCE=.\Page282.cpp
# End Source File
# Begin Source File

SOURCE=.\Page283.cpp
# End Source File
# Begin Source File

SOURCE=.\Page285.cpp
# End Source File
# Begin Source File

SOURCE=.\Page286.cpp
# End Source File
# Begin Source File

SOURCE=.\Page287.cpp
# End Source File
# Begin Source File

SOURCE=.\Page288.cpp
# End Source File
# Begin Source File

SOURCE=.\Page289.cpp
# End Source File
# Begin Source File

SOURCE=.\Page30.cpp
# End Source File
# Begin Source File

SOURCE=.\Page32.cpp
# End Source File
# Begin Source File

SOURCE=.\Page40.cpp
# End Source File
# Begin Source File

SOURCE=.\Page400.cpp
# End Source File
# Begin Source File

SOURCE=.\Page401.cpp
# End Source File
# Begin Source File

SOURCE=.\Page405.cpp
# End Source File
# Begin Source File

SOURCE=.\Page410.cpp
# End Source File
# Begin Source File

SOURCE=.\Page420.cpp
# End Source File
# Begin Source File

SOURCE=.\Page425.cpp
# End Source File
# Begin Source File

SOURCE=.\Page430.cpp
# End Source File
# Begin Source File

SOURCE=.\Page435.cpp
# End Source File
# Begin Source File

SOURCE=.\Page440.cpp
# End Source File
# Begin Source File

SOURCE=.\Page445.cpp
# End Source File
# Begin Source File

SOURCE=.\Page446.cpp
# End Source File
# Begin Source File

SOURCE=.\Page450.cpp
# End Source File
# Begin Source File

SOURCE=.\Page451.cpp
# End Source File
# Begin Source File

SOURCE=.\Page452.cpp
# End Source File
# Begin Source File

SOURCE=.\Page453.cpp
# End Source File
# Begin Source File

SOURCE=.\Page454.cpp
# End Source File
# Begin Source File

SOURCE=.\Page455.cpp
# End Source File
# Begin Source File

SOURCE=.\Page456.cpp
# End Source File
# Begin Source File

SOURCE=.\Page457.cpp
# End Source File
# Begin Source File

SOURCE=.\Page460.cpp
# End Source File
# Begin Source File

SOURCE=.\Page461.cpp
# End Source File
# Begin Source File

SOURCE=.\Page462.cpp
# End Source File
# Begin Source File

SOURCE=.\Page463.cpp
# End Source File
# Begin Source File

SOURCE=.\Page464.cpp
# End Source File
# Begin Source File

SOURCE=.\Page465.cpp
# End Source File
# Begin Source File

SOURCE=.\Page466.cpp
# End Source File
# Begin Source File

SOURCE=.\Page467.cpp
# End Source File
# Begin Source File

SOURCE=.\Page470.cpp
# End Source File
# Begin Source File

SOURCE=.\Page475.cpp
# End Source File
# Begin Source File

SOURCE=.\Page480.cpp
# End Source File
# Begin Source File

SOURCE=.\Page485.cpp
# End Source File
# Begin Source File

SOURCE=.\Page50.cpp
# End Source File
# Begin Source File

SOURCE=.\Page500.cpp
# End Source File
# Begin Source File

SOURCE=.\Page501.cpp
# End Source File
# Begin Source File

SOURCE=.\Page502.cpp
# End Source File
# Begin Source File

SOURCE=.\Page503.cpp
# End Source File
# Begin Source File

SOURCE=.\Page504.cpp
# End Source File
# Begin Source File

SOURCE=.\Page505.cpp
# End Source File
# Begin Source File

SOURCE=.\Page506.cpp
# End Source File
# Begin Source File

SOURCE=.\Page507.cpp
# End Source File
# Begin Source File

SOURCE=.\Page510.cpp
# End Source File
# Begin Source File

SOURCE=.\Page511.cpp
# End Source File
# Begin Source File

SOURCE=.\Page512.cpp
# End Source File
# Begin Source File

SOURCE=.\Page513.cpp
# End Source File
# Begin Source File

SOURCE=.\Page514.cpp
# End Source File
# Begin Source File

SOURCE=.\Page520.cpp
# End Source File
# Begin Source File

SOURCE=.\Page521.cpp
# End Source File
# Begin Source File

SOURCE=.\Page522.cpp
# End Source File
# Begin Source File

SOURCE=.\Page523.cpp
# End Source File
# Begin Source File

SOURCE=.\Page524.cpp
# End Source File
# Begin Source File

SOURCE=.\Page530.cpp
# End Source File
# Begin Source File

SOURCE=.\Page531.cpp
# End Source File
# Begin Source File

SOURCE=.\Page532.cpp
# End Source File
# Begin Source File

SOURCE=.\Page533.cpp
# End Source File
# Begin Source File

SOURCE=.\Page534.cpp
# End Source File
# Begin Source File

SOURCE=.\Page535.cpp
# End Source File
# Begin Source File

SOURCE=.\Page540.cpp
# End Source File
# Begin Source File

SOURCE=.\Page541.cpp
# End Source File
# Begin Source File

SOURCE=.\Page542.cpp
# End Source File
# Begin Source File

SOURCE=.\Page543.cpp
# End Source File
# Begin Source File

SOURCE=.\Page544.cpp
# End Source File
# Begin Source File

SOURCE=.\Page60.cpp
# End Source File
# Begin Source File

SOURCE=.\page600.cpp
# End Source File
# Begin Source File

SOURCE=.\Page605.cpp
# End Source File
# Begin Source File

SOURCE=.\Page610.cpp
# End Source File
# Begin Source File

SOURCE=.\Page660.cpp
# End Source File
# Begin Source File

SOURCE=.\Page670.cpp
# End Source File
# Begin Source File

SOURCE=.\Page680.cpp
# End Source File
# Begin Source File

SOURCE=.\Page685.cpp
# End Source File
# Begin Source File

SOURCE=.\Page690.cpp
# End Source File
# Begin Source File

SOURCE=.\Page70.cpp
# End Source File
# Begin Source File

SOURCE=.\Page80.cpp
# End Source File
# Begin Source File

SOURCE=.\Page90.cpp
# End Source File
# Begin Source File

SOURCE=.\Page92.cpp
# End Source File
# Begin Source File

SOURCE=.\Page93.cpp
# End Source File
# Begin Source File

SOURCE=.\Page94.cpp
# End Source File
# Begin Source File

SOURCE=.\Page96.cpp
# End Source File
# Begin Source File

SOURCE=.\PageAdd.cpp
# End Source File
# Begin Source File

SOURCE=.\PageNoAdd.cpp
# End Source File
# Begin Source File

SOURCE=.\PageOutput.cpp
# End Source File
# Begin Source File

SOURCE=.\picture.cpp
# End Source File
# Begin Source File

SOURCE=.\poly.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgressWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\RecordsetEx.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StringToken.cpp
# End Source File
# Begin Source File

SOURCE=.\TitleTip.cpp
# End Source File
# Begin Source File

SOURCE=.\Weather.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CommentsDlg.h
# End Source File
# Begin Source File

SOURCE=.\DirDialog.h
# End Source File
# Begin Source File

SOURCE=.\DlgCrossSection.h
# End Source File
# Begin Source File

SOURCE=.\DlgWQ.h
# End Source File
# Begin Source File

SOURCE=.\eta.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl.h
# End Source File
# Begin Source File

SOURCE=.\GridDropTarget.h
# End Source File
# Begin Source File

SOURCE=.\InPlaceEdit.h
# End Source File
# Begin Source File

SOURCE=.\InPlaceList.h
# End Source File
# Begin Source File

SOURCE=.\Landuse.h
# End Source File
# Begin Source File

SOURCE=.\LeftView.h
# End Source File
# Begin Source File

SOURCE=.\LSPC.h
# End Source File
# Begin Source File

SOURCE=.\LSPCDoc.h
# End Source File
# Begin Source File

SOURCE=.\LSPCMODEL.h
# End Source File
# Begin Source File

SOURCE=.\LSPCView.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MemDC.h
# End Source File
# Begin Source File

SOURCE=.\MyPropertySheet1.h
# End Source File
# Begin Source File

SOURCE=.\MyPropertySheet2.h
# End Source File
# Begin Source File

SOURCE=.\Output.h
# End Source File
# Begin Source File

SOURCE=.\page0.h
# End Source File
# Begin Source File

SOURCE=.\Page10.h
# End Source File
# Begin Source File

SOURCE=.\Page100.h
# End Source File
# Begin Source File

SOURCE=.\Page110.h
# End Source File
# Begin Source File

SOURCE=.\Page120.h
# End Source File
# Begin Source File

SOURCE=.\Page130.h
# End Source File
# Begin Source File

SOURCE=.\Page140.h
# End Source File
# Begin Source File

SOURCE=.\Page15.h
# End Source File
# Begin Source File

SOURCE=.\Page150.h
# End Source File
# Begin Source File

SOURCE=.\Page160.h
# End Source File
# Begin Source File

SOURCE=.\Page170.h
# End Source File
# Begin Source File

SOURCE=.\Page180.h
# End Source File
# Begin Source File

SOURCE=.\Page190.h
# End Source File
# Begin Source File

SOURCE=.\Page20.h
# End Source File
# Begin Source File

SOURCE=.\Page200.h
# End Source File
# Begin Source File

SOURCE=.\Page201.h
# End Source File
# Begin Source File

SOURCE=.\Page202.h
# End Source File
# Begin Source File

SOURCE=.\Page203.h
# End Source File
# Begin Source File

SOURCE=.\Page204.h
# End Source File
# Begin Source File

SOURCE=.\Page205.h
# End Source File
# Begin Source File

SOURCE=.\Page250.h
# End Source File
# Begin Source File

SOURCE=.\Page255.h
# End Source File
# Begin Source File

SOURCE=.\Page260.h
# End Source File
# Begin Source File

SOURCE=.\Page270.h
# End Source File
# Begin Source File

SOURCE=.\Page271.h
# End Source File
# Begin Source File

SOURCE=.\Page272.h
# End Source File
# Begin Source File

SOURCE=.\Page273.h
# End Source File
# Begin Source File

SOURCE=.\Page274.h
# End Source File
# Begin Source File

SOURCE=.\Page275.h
# End Source File
# Begin Source File

SOURCE=.\Page276.h
# End Source File
# Begin Source File

SOURCE=.\Page280.h
# End Source File
# Begin Source File

SOURCE=.\Page281.h
# End Source File
# Begin Source File

SOURCE=.\Page282.h
# End Source File
# Begin Source File

SOURCE=.\Page283.h
# End Source File
# Begin Source File

SOURCE=.\Page285.h
# End Source File
# Begin Source File

SOURCE=.\Page286.h
# End Source File
# Begin Source File

SOURCE=.\Page287.h
# End Source File
# Begin Source File

SOURCE=.\Page288.h
# End Source File
# Begin Source File

SOURCE=.\Page289.h
# End Source File
# Begin Source File

SOURCE=.\Page30.h
# End Source File
# Begin Source File

SOURCE=.\Page32.h
# End Source File
# Begin Source File

SOURCE=.\Page40.h
# End Source File
# Begin Source File

SOURCE=.\Page400.h
# End Source File
# Begin Source File

SOURCE=.\Page401.h
# End Source File
# Begin Source File

SOURCE=.\Page405.h
# End Source File
# Begin Source File

SOURCE=.\Page410.h
# End Source File
# Begin Source File

SOURCE=.\Page420.h
# End Source File
# Begin Source File

SOURCE=.\Page425.h
# End Source File
# Begin Source File

SOURCE=.\Page430.h
# End Source File
# Begin Source File

SOURCE=.\Page435.h
# End Source File
# Begin Source File

SOURCE=.\Page440.h
# End Source File
# Begin Source File

SOURCE=.\Page445.h
# End Source File
# Begin Source File

SOURCE=.\Page446.h
# End Source File
# Begin Source File

SOURCE=.\Page450.h
# End Source File
# Begin Source File

SOURCE=.\Page451.h
# End Source File
# Begin Source File

SOURCE=.\Page452.h
# End Source File
# Begin Source File

SOURCE=.\Page453.h
# End Source File
# Begin Source File

SOURCE=.\Page454.h
# End Source File
# Begin Source File

SOURCE=.\Page455.h
# End Source File
# Begin Source File

SOURCE=.\Page456.h
# End Source File
# Begin Source File

SOURCE=.\Page457.h
# End Source File
# Begin Source File

SOURCE=.\Page460.h
# End Source File
# Begin Source File

SOURCE=.\Page461.h
# End Source File
# Begin Source File

SOURCE=.\Page462.h
# End Source File
# Begin Source File

SOURCE=.\Page463.h
# End Source File
# Begin Source File

SOURCE=.\Page464.h
# End Source File
# Begin Source File

SOURCE=.\Page465.h
# End Source File
# Begin Source File

SOURCE=.\Page466.h
# End Source File
# Begin Source File

SOURCE=.\Page467.h
# End Source File
# Begin Source File

SOURCE=.\Page470.h
# End Source File
# Begin Source File

SOURCE=.\Page475.h
# End Source File
# Begin Source File

SOURCE=.\Page480.h
# End Source File
# Begin Source File

SOURCE=.\Page485.h
# End Source File
# Begin Source File

SOURCE=.\Page50.h
# End Source File
# Begin Source File

SOURCE=.\Page500.h
# End Source File
# Begin Source File

SOURCE=.\Page501.h
# End Source File
# Begin Source File

SOURCE=.\Page502.h
# End Source File
# Begin Source File

SOURCE=.\Page503.h
# End Source File
# Begin Source File

SOURCE=.\Page504.h
# End Source File
# Begin Source File

SOURCE=.\Page505.h
# End Source File
# Begin Source File

SOURCE=.\Page506.h
# End Source File
# Begin Source File

SOURCE=.\Page507.h
# End Source File
# Begin Source File

SOURCE=.\Page510.h
# End Source File
# Begin Source File

SOURCE=.\Page511.h
# End Source File
# Begin Source File

SOURCE=.\Page512.h
# End Source File
# Begin Source File

SOURCE=.\Page513.h
# End Source File
# Begin Source File

SOURCE=.\Page514.h
# End Source File
# Begin Source File

SOURCE=.\Page520.h
# End Source File
# Begin Source File

SOURCE=.\Page521.h
# End Source File
# Begin Source File

SOURCE=.\Page522.h
# End Source File
# Begin Source File

SOURCE=.\Page523.h
# End Source File
# Begin Source File

SOURCE=.\Page524.h
# End Source File
# Begin Source File

SOURCE=.\Page530.h
# End Source File
# Begin Source File

SOURCE=.\Page531.h
# End Source File
# Begin Source File

SOURCE=.\Page532.h
# End Source File
# Begin Source File

SOURCE=.\Page533.h
# End Source File
# Begin Source File

SOURCE=.\Page534.h
# End Source File
# Begin Source File

SOURCE=.\Page535.h
# End Source File
# Begin Source File

SOURCE=.\Page540.h
# End Source File
# Begin Source File

SOURCE=.\Page541.h
# End Source File
# Begin Source File

SOURCE=.\Page542.h
# End Source File
# Begin Source File

SOURCE=.\Page543.h
# End Source File
# Begin Source File

SOURCE=.\Page544.h
# End Source File
# Begin Source File

SOURCE=.\Page60.h
# End Source File
# Begin Source File

SOURCE=.\page600.h
# End Source File
# Begin Source File

SOURCE=.\Page605.h
# End Source File
# Begin Source File

SOURCE=.\Page610.h
# End Source File
# Begin Source File

SOURCE=.\Page660.h
# End Source File
# Begin Source File

SOURCE=.\Page670.h
# End Source File
# Begin Source File

SOURCE=.\Page680.h
# End Source File
# Begin Source File

SOURCE=.\Page685.h
# End Source File
# Begin Source File

SOURCE=.\Page690.h
# End Source File
# Begin Source File

SOURCE=.\Page70.h
# End Source File
# Begin Source File

SOURCE=.\Page80.h
# End Source File
# Begin Source File

SOURCE=.\Page90.h
# End Source File
# Begin Source File

SOURCE=.\Page92.h
# End Source File
# Begin Source File

SOURCE=.\Page93.h
# End Source File
# Begin Source File

SOURCE=.\Page94.h
# End Source File
# Begin Source File

SOURCE=.\Page96.h
# End Source File
# Begin Source File

SOURCE=.\PageAdd.h
# End Source File
# Begin Source File

SOURCE=.\PageNoAdd.h
# End Source File
# Begin Source File

SOURCE=.\PageOutput.h
# End Source File
# Begin Source File

SOURCE=.\picture.h
# End Source File
# Begin Source File

SOURCE=.\poly.h
# End Source File
# Begin Source File

SOURCE=.\ProgressWnd.h
# End Source File
# Begin Source File

SOURCE=.\RecordsetEx.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StringToken.h
# End Source File
# Begin Source File

SOURCE=.\TitleTip.h
# End Source File
# Begin Source File

SOURCE=.\Weather.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\LSPC.ico
# End Source File
# Begin Source File

SOURCE=.\res\LSPC.rc2
# End Source File
# Begin Source File

SOURCE=.\res\LSPCDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\INCLUDE\ALXParserExceptionRes.h
# End Source File
# Begin Source File

SOURCE=.\INCLUDE\ALXSyntaxEditCtrl.h
# End Source File
# Begin Source File

SOURCE=.\CellRange.h
# End Source File
# Begin Source File

SOURCE=.\INCLUDE\DBFCursorSet.h
# End Source File
# Begin Source File

SOURCE=.\INCLUDE\DBFExceptionRes.h
# End Source File
# Begin Source File

SOURCE=.\INCLUDE\DBFMemCursorSet.h
# End Source File
# Begin Source File

SOURCE=.\LSPC.rc
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\eta.lib
# End Source File
# Begin Source File

SOURCE=.\alloc.lib
# End Source File
# End Target
# End Project
# Section LSPC : {20DD1B9E-87C4-11D1-8BE3-0000F8754DA1}
# 	2:21:DefaultSinkHeaderFile:dtpicker.h
# 	2:16:DefaultSinkClass:CDTPicker
# End Section
# Section LSPC : {BEF6E003-A874-101A-8BBA-00AA00300CAB}
# 	2:5:Class:COleFont
# 	2:10:HeaderFile:font.h
# 	2:8:ImplFile:font.cpp
# End Section
# Section LSPC : {20DD1B9B-87C4-11D1-8BE3-0000F8754DA1}
# 	2:5:Class:CDTPicker
# 	2:10:HeaderFile:dtpicker.h
# 	2:8:ImplFile:dtpicker.cpp
# End Section
# Section LSPC : {7BF80981-BF32-101A-8BBB-00AA00300CAB}
# 	2:5:Class:CPicture
# 	2:10:HeaderFile:picture.h
# 	2:8:ImplFile:picture.cpp
# End Section
