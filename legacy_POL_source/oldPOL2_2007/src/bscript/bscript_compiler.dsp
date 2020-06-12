# Microsoft Developer Studio Project File - Name="bscript_compiler" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=bscript_compiler - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "bscript_compiler.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "bscript_compiler.mak" CFG="bscript_compiler - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "bscript_compiler - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "bscript_compiler - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "bscript_compiler - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I ".." /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D for="if(0); else for" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "bscript_compiler - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Compiler\Debug"
# PROP Intermediate_Dir "Compiler\Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I ".." /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D for="if(0); else for" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "bscript_compiler - Win32 Release"
# Name "bscript_compiler - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\compctx.cpp
# End Source File
# Begin Source File

SOURCE=.\compiler.cpp
# End Source File
# Begin Source File

SOURCE=.\compilercfg.cpp
# End Source File
# Begin Source File

SOURCE=.\eprog.cpp
# End Source File
# Begin Source File

SOURCE=.\eprog2.cpp
# End Source File
# Begin Source File

SOURCE=.\eprog3.cpp
# End Source File
# Begin Source File

SOURCE=.\eprog_read.cpp
# End Source File
# Begin Source File

SOURCE=.\escriptv.cpp
# End Source File
# Begin Source File

SOURCE=.\fmodule.cpp
# End Source File
# Begin Source File

SOURCE=.\parser.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "bscript_compiler - Win32 Release"

# SUBTRACT CPP /YX /Yc

!ELSEIF  "$(CFG)" == "bscript_compiler - Win32 Debug"

# ADD CPP /Yc"clib/stl_inc.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\symcont.cpp
# End Source File
# Begin Source File

SOURCE=.\tkn_strm.cpp
# End Source File
# Begin Source File

SOURCE=.\token.cpp
# End Source File
# Begin Source File

SOURCE=.\userfunc.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\compctx.h
# End Source File
# Begin Source File

SOURCE=.\compiler.h
# End Source File
# Begin Source File

SOURCE=.\compilercfg.h
# End Source File
# Begin Source File

SOURCE=.\compmodl.h
# End Source File
# Begin Source File

SOURCE=.\eprog.h
# End Source File
# Begin Source File

SOURCE=.\escript.h
# End Source File
# Begin Source File

SOURCE=.\escriptv.h
# End Source File
# Begin Source File

SOURCE=.\exectype.h
# End Source File
# Begin Source File

SOURCE=.\executor.h
# End Source File
# Begin Source File

SOURCE=.\executortype.h
# End Source File
# Begin Source File

SOURCE=.\parser.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\symcont.h
# End Source File
# Begin Source File

SOURCE=.\token.h
# End Source File
# Begin Source File

SOURCE=.\tokens.h
# End Source File
# Begin Source File

SOURCE=.\userfunc.h
# End Source File
# Begin Source File

SOURCE=.\verbtbl.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Readme.txt
# End Source File
# End Target
# End Project
