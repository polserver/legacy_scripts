# Microsoft Developer Studio Project File - Name="clib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=clib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "clib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "clib.mak" CFG="clib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "clib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "clib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "clib - Win32 Release"

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
# ADD CPP /nologo /MT /W3 /GX /Zd /O2 /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "NOMINMAX" /D for="if(0); else for" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "clib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "NOMINMAX" /D for="if(0); else for" /FR /FD /GZ /c
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

# Name "clib - Win32 Release"
# Name "clib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\cfgfile.cpp

!IF  "$(CFG)" == "clib - Win32 Release"

!ELSEIF  "$(CFG)" == "clib - Win32 Debug"

# ADD CPP /Yu"stl_inc.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\cfgsect.cpp
# End Source File
# Begin Source File

SOURCE=.\cmdargs.cpp
# End Source File
# Begin Source File

SOURCE=.\dirfunc.cpp
# End Source File
# Begin Source File

SOURCE=.\dirlist.cpp
# End Source File
# Begin Source File

SOURCE=.\dualbuf.cpp
# End Source File
# Begin Source File

SOURCE=.\endian.cpp
# End Source File
# Begin Source File

SOURCE=.\esignal.cpp
# End Source File
# Begin Source File

SOURCE=.\fdump.cpp
# End Source File
# Begin Source File

SOURCE=.\filecont.cpp
# End Source File
# Begin Source File

SOURCE=.\fileutil.cpp
# End Source File
# Begin Source File

SOURCE=.\forspcnt.cpp
# End Source File
# Begin Source File

SOURCE=.\logfile.cpp
# End Source File
# Begin Source File

SOURCE=.\mdump.cpp
# End Source File
# Begin Source File

SOURCE=.\mlog.cpp
# End Source File
# Begin Source File

SOURCE=.\msjexhnd.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\nlogfile.cpp
# End Source File
# Begin Source File

SOURCE=.\opnew.cpp
# End Source File
# Begin Source File

SOURCE=.\passert.cpp
# End Source File
# Begin Source File

SOURCE=.\progver.cpp
# End Source File
# Begin Source File

SOURCE=.\random.cpp
# End Source File
# Begin Source File

SOURCE=.\rarray.cpp
# End Source File
# Begin Source File

SOURCE=.\sckutil.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "clib - Win32 Release"

# SUBTRACT CPP /YX /Yc

!ELSEIF  "$(CFG)" == "clib - Win32 Debug"

# ADD CPP /Yc"stl_inc.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\stracpy.cpp
# End Source File
# Begin Source File

SOURCE=.\strexcpt.cpp
# End Source File
# Begin Source File

SOURCE=.\strset.cpp
# End Source File
# Begin Source File

SOURCE=.\strutil.cpp
# End Source File
# Begin Source File

SOURCE=.\tracebuf.cpp
# End Source File
# Begin Source File

SOURCE=.\wnsckt.cpp
# End Source File
# Begin Source File

SOURCE=.\xmain.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\cfgelem.h
# End Source File
# Begin Source File

SOURCE=.\cfgfile.h
# End Source File
# Begin Source File

SOURCE=.\cfgsect.h
# End Source File
# Begin Source File

SOURCE=.\cistring.h
# End Source File
# Begin Source File

SOURCE=.\clib.h
# End Source File
# Begin Source File

SOURCE=.\clibopt.h
# End Source File
# Begin Source File

SOURCE=.\dirlist.h
# End Source File
# Begin Source File

SOURCE=.\drarray.h
# End Source File
# Begin Source File

SOURCE=.\dualbuf.h
# End Source File
# Begin Source File

SOURCE=.\endian.h
# End Source File
# Begin Source File

SOURCE=.\esignal.h
# End Source File
# Begin Source File

SOURCE=.\fdump.h
# End Source File
# Begin Source File

SOURCE=.\fileutil.h
# End Source File
# Begin Source File

SOURCE=.\fixalloc.h
# End Source File
# Begin Source File

SOURCE=.\logfile.h
# End Source File
# Begin Source File

SOURCE=.\maputil.h
# End Source File
# Begin Source File

SOURCE=.\mdump.h
# End Source File
# Begin Source File

SOURCE=.\mlog.h
# End Source File
# Begin Source File

SOURCE=.\msjexhnd.h
# End Source File
# Begin Source File

SOURCE=.\nlogfile.h
# End Source File
# Begin Source File

SOURCE=.\opnew.h
# End Source File
# Begin Source File

SOURCE=.\passert.h
# End Source File
# Begin Source File

SOURCE=.\progver.h
# End Source File
# Begin Source File

SOURCE=.\random.h
# End Source File
# Begin Source File

SOURCE=.\rarray.h
# End Source File
# Begin Source File

SOURCE=.\rawtypes.h
# End Source File
# Begin Source File

SOURCE=.\RefPtr.h
# End Source File
# Begin Source File

SOURCE=.\sckutil.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\stl_inc.h
# End Source File
# Begin Source File

SOURCE=.\stlutil.h
# End Source File
# Begin Source File

SOURCE=.\strexcpt.h
# End Source File
# Begin Source File

SOURCE=.\strset.h
# End Source File
# Begin Source File

SOURCE=.\strutil.h
# End Source File
# Begin Source File

SOURCE=.\tracebuf.h
# End Source File
# Begin Source File

SOURCE=.\unicode.h
# End Source File
# Begin Source File

SOURCE=.\unittest.h
# End Source File
# Begin Source File

SOURCE=.\weakptr.h
# End Source File
# Begin Source File

SOURCE=.\wnsckt.h
# End Source File
# Begin Source File

SOURCE=.\xmain.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Readme.txt
# End Source File
# End Target
# End Project
