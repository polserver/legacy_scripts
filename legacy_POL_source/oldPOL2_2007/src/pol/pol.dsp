# Microsoft Developer Studio Project File - Name="pol" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=pol - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "pol.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "pol.mak" CFG="pol - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "pol - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "pol - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "pol - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /I ".." /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D for="if(0); else for" /D "NOMINMAX" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /map /debug /machine:I386 /out:"C:\cvs3\src\pol\Release\pol.exe"

!ELSEIF  "$(CFG)" == "pol - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /GX /Zi /Od /I ".." /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D for="if(0); else for" /D "NOMINMAX" /D INC_PASSERT=1 /FR /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
# SUBTRACT RSC /x
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /map /debug /machine:I386 /out:"D:\pol094comp\pol.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "pol - Win32 Release"
# Name "pol - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\account.cpp
# End Source File
# Begin Source File

SOURCE=.\accounts.cpp
# End Source File
# Begin Source File

SOURCE=.\acscrobj.cpp
# End Source File
# Begin Source File

SOURCE=.\allocd.cpp
# End Source File
# Begin Source File

SOURCE=.\armor.cpp
# End Source File
# Begin Source File

SOURCE=.\attack.cpp
# End Source File
# Begin Source File

SOURCE=.\attribute.cpp
# End Source File
# Begin Source File

SOURCE=.\auxclient.cpp
# End Source File
# Begin Source File

SOURCE=.\bannedips.cpp
# End Source File
# Begin Source File

SOURCE=.\boat.cpp
# End Source File
# Begin Source File

SOURCE=.\boatcomp.cpp
# End Source File
# Begin Source File

SOURCE=.\boatemod.cpp
# End Source File
# Begin Source File

SOURCE=.\bowsalut.cpp
# End Source File
# Begin Source File

SOURCE=.\cfgemod.cpp
# End Source File
# Begin Source File

SOURCE=.\cfgrepos.cpp
# End Source File
# Begin Source File

SOURCE=.\cgdata.cpp
# End Source File
# Begin Source File

SOURCE=.\charactr.cpp
# End Source File
# Begin Source File

SOURCE=.\checkpnt.cpp
# End Source File
# Begin Source File

SOURCE=.\chrcast.cpp
# End Source File
# Begin Source File

SOURCE=.\chrituse.cpp
# End Source File
# Begin Source File

SOURCE=.\chrskuse.cpp
# End Source File
# Begin Source File

SOURCE=.\client.cpp
# End Source File
# Begin Source File

SOURCE=.\clientio.cpp
# End Source File
# Begin Source File

SOURCE=.\cliface.cpp
# End Source File
# Begin Source File

SOURCE=.\cmbtcfg.cpp
# End Source File
# Begin Source File

SOURCE=.\cmdlevel.cpp
# End Source File
# Begin Source File

SOURCE=.\console.cpp
# End Source File
# Begin Source File

SOURCE=.\containr.cpp
# End Source File
# Begin Source File

SOURCE=.\core.cpp
# End Source File
# Begin Source File

SOURCE=.\corpse.cpp
# End Source File
# Begin Source File

SOURCE=.\create.cpp
# End Source File
# Begin Source File

SOURCE=.\cryptengine.cpp
# End Source File
# Begin Source File

SOURCE=.\ctable.cpp
# End Source File
# Begin Source File

SOURCE=.\datastore.cpp
# End Source File
# Begin Source File

SOURCE=.\dblclick.cpp
# End Source File
# Begin Source File

SOURCE=.\decay.cpp
# End Source File
# Begin Source File

SOURCE=.\dice.cpp
# End Source File
# Begin Source File

SOURCE=.\door.cpp
# End Source File
# Begin Source File

SOURCE=.\dropitem.cpp
# End Source File
# Begin Source File

SOURCE=.\dye.cpp
# End Source File
# Begin Source File

SOURCE=.\eqpitem.cpp
# End Source File
# Begin Source File

SOURCE=.\equipdsc.cpp
# End Source File
# Begin Source File

SOURCE=.\equipmnt.cpp
# End Source File
# Begin Source File

SOURCE=.\exscrobj.cpp
# End Source File
# Begin Source File

SOURCE=.\extcmd.cpp
# End Source File
# Begin Source File

SOURCE=.\fnsearch.cpp
# End Source File
# Begin Source File

SOURCE=.\gameclck.cpp
# End Source File
# Begin Source File

SOURCE=.\getitem.cpp
# End Source File
# Begin Source File

SOURCE=.\getmsg.cpp
# End Source File
# Begin Source File

SOURCE=.\gflag.cpp
# End Source File
# Begin Source File

SOURCE=.\gprepos.cpp
# End Source File
# Begin Source File

SOURCE=.\guardrgn.cpp
# End Source File
# Begin Source File

SOURCE=.\guilds.cpp
# End Source File
# Begin Source File

SOURCE=.\heaven.cpp
# End Source File
# Begin Source File

SOURCE=.\help.cpp
# End Source File
# Begin Source File

SOURCE=.\house.cpp
# End Source File
# Begin Source File

SOURCE=.\iostats.cpp
# End Source File
# Begin Source File

SOURCE=.\irequest.cpp
# End Source File
# Begin Source File

SOURCE=.\item.cpp
# End Source File
# Begin Source File

SOURCE=.\item00.cpp
# End Source File
# Begin Source File

SOURCE=.\itemcr.cpp
# End Source File
# Begin Source File

SOURCE=.\itemdesc.cpp
# End Source File
# Begin Source File

SOURCE=.\listenpt.cpp
# End Source File
# Begin Source File

SOURCE=.\loadunld.cpp
# End Source File
# Begin Source File

SOURCE=.\lockable.cpp
# End Source File
# Begin Source File

SOURCE=.\logfiles.cpp
# End Source File
# Begin Source File

SOURCE=.\login.cpp
# End Source File
# Begin Source File

SOURCE=.\los.cpp
# End Source File
# Begin Source File

SOURCE=.\los2.cpp
# End Source File
# Begin Source File

SOURCE=.\menu.cpp
# End Source File
# Begin Source File

SOURCE=.\menuinst.cpp
# End Source File
# Begin Source File

SOURCE=.\menusel.cpp
# End Source File
# Begin Source File

SOURCE=.\miscmsg.cpp
# End Source File
# Begin Source File

SOURCE=.\miscrgn.cpp
# End Source File
# Begin Source File

SOURCE=.\mkscrobj.cpp
# End Source File
# Begin Source File

SOURCE=.\movecost.cpp
# End Source File
# Begin Source File

SOURCE=.\movement.cpp
# End Source File
# Begin Source File

SOURCE=.\msgfiltr.cpp
# End Source File
# Begin Source File

SOURCE=.\multicfg.cpp
# End Source File
# Begin Source File

SOURCE=.\multicr.cpp
# End Source File
# Begin Source File

SOURCE=.\multidef.cpp
# End Source File
# Begin Source File

SOURCE=.\multis.cpp
# End Source File
# Begin Source File

SOURCE=.\musicrgn.cpp
# End Source File
# Begin Source File

SOURCE=.\npc.cpp
# End Source File
# Begin Source File

SOURCE=.\npctemplates.cpp
# End Source File
# Begin Source File

SOURCE=.\npctmpl.cpp
# End Source File
# Begin Source File

SOURCE=.\objecthash.cpp
# End Source File
# Begin Source File

SOURCE=.\pol.cpp
# End Source File
# Begin Source File

SOURCE=.\polcfg.cpp
# End Source File
# Begin Source File

SOURCE=.\polclock.cpp
# End Source File
# Begin Source File

SOURCE=.\poldbg.cpp
# End Source File
# Begin Source File

SOURCE=.\polfile1.cpp
# End Source File
# Begin Source File

SOURCE=.\polsem.cpp
# End Source File
# Begin Source File

SOURCE=.\polservice.cpp
# End Source File
# Begin Source File

SOURCE=.\polsig.cpp
# End Source File
# Begin Source File

SOURCE=.\polstats.cpp
# End Source File
# Begin Source File

SOURCE=.\polsystememod.cpp
# End Source File
# Begin Source File

SOURCE=.\poltest.cpp
# End Source File
# Begin Source File

SOURCE=.\polvar.cpp
# End Source File
# Begin Source File

SOURCE=.\polwww.cpp
# End Source File
# Begin Source File

SOURCE=.\profile.cpp
# End Source File
# Begin Source File

SOURCE=.\proplist.cpp
# End Source File
# Begin Source File

SOURCE=.\readcfg.cpp
# End Source File
# Begin Source File

SOURCE=.\realms.cpp
# End Source File
# Begin Source File

SOURCE=.\reftypes.cpp
# End Source File
# Begin Source File

SOURCE=.\region.cpp
# End Source File
# Begin Source File

SOURCE=.\repsys.cpp
# End Source File
# Begin Source File

SOURCE=.\resource.cpp
# End Source File
# Begin Source File

SOURCE=.\schedule.cpp
# End Source File
# Begin Source File

SOURCE=.\scrdef.cpp
# End Source File
# Begin Source File

SOURCE=.\scrsched.cpp
# End Source File
# Begin Source File

SOURCE=.\scrstore.cpp
# End Source File
# Begin Source File

SOURCE=.\skilladv.cpp
# End Source File
# Begin Source File

SOURCE=.\sngclick.cpp
# End Source File
# Begin Source File

SOURCE=.\sockio.cpp
# End Source File
# Begin Source File

SOURCE=.\speech.cpp
# End Source File
# Begin Source File

SOURCE=.\spelbook.cpp
# End Source File
# Begin Source File

SOURCE=.\spells.cpp
# End Source File
# Begin Source File

SOURCE=.\ssopt.cpp
# End Source File
# Begin Source File

SOURCE=.\stackcfg.cpp
# End Source File
# Begin Source File

SOURCE=.\startloc.cpp
# End Source File
# Begin Source File

SOURCE=.\statmsg.cpp
# End Source File
# Begin Source File

SOURCE=.\storage.cpp
# End Source File
# Begin Source File

SOURCE=.\syshook.cpp
# End Source File
# Begin Source File

SOURCE=.\systemhookscript.cpp
# End Source File
# Begin Source File

SOURCE=.\target.cpp
# End Source File
# Begin Source File

SOURCE=.\tasks.cpp
# End Source File
# Begin Source File

SOURCE=.\testenv.cpp
# End Source File
# Begin Source File

SOURCE=.\textcmd.cpp
# End Source File
# Begin Source File

SOURCE=.\tildecmd.cpp
# End Source File
# Begin Source File

SOURCE=.\tiplstwn.cpp
# End Source File
# Begin Source File

SOURCE=.\tips.cpp
# End Source File
# Begin Source File

SOURCE=.\tmpvars.cpp
# End Source File
# Begin Source File

SOURCE=.\tooltips.cpp
# End Source File
# Begin Source File

SOURCE=.\ucrypto.cpp
# End Source File
# Begin Source File

SOURCE=.\ufunc.cpp
# End Source File
# Begin Source File

SOURCE=.\ufunc2.cpp
# End Source File
# Begin Source File

SOURCE=.\uimport.cpp
# End Source File
# Begin Source File

SOURCE=.\umap.cpp
# End Source File
# Begin Source File

SOURCE=.\uniemod.cpp
# End Source File
# Begin Source File

SOURCE=.\uobjcnt.cpp
# End Source File
# Begin Source File

SOURCE=.\uobject.cpp
# End Source File
# Begin Source File

SOURCE=.\uobjhelp.cpp
# End Source File
# Begin Source File

SOURCE=.\uoclient.cpp
# End Source File
# Begin Source File

SOURCE=.\uoemod.cpp
# End Source File
# Begin Source File

SOURCE=.\uoemod2.cpp
# End Source File
# Begin Source File

SOURCE=.\uoemod3.cpp
# End Source File
# Begin Source File

SOURCE=.\uoexhelp.cpp
# End Source File
# Begin Source File

SOURCE=.\uofile00.cpp
# End Source File
# Begin Source File

SOURCE=.\uofile01.cpp
# End Source File
# Begin Source File

SOURCE=.\uofile02.cpp
# End Source File
# Begin Source File

SOURCE=.\uofile03.cpp
# End Source File
# Begin Source File

SOURCE=.\uoscrobj.cpp
# End Source File
# Begin Source File

SOURCE=.\uoskills.cpp
# End Source File
# Begin Source File

SOURCE=.\useskill.cpp
# End Source File
# Begin Source File

SOURCE=.\utilemod.cpp
# End Source File
# Begin Source File

SOURCE=.\uvars.cpp
# End Source File
# Begin Source File

SOURCE=.\uworld.cpp
# End Source File
# Begin Source File

SOURCE=.\vital.cpp
# End Source File
# Begin Source File

SOURCE=.\weapon.cpp
# End Source File
# Begin Source File

SOURCE=.\zone.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\account.h
# End Source File
# Begin Source File

SOURCE=.\accounts.h
# End Source File
# Begin Source File

SOURCE=.\acscrobj.h
# End Source File
# Begin Source File

SOURCE=.\action.h
# End Source File
# Begin Source File

SOURCE=.\allocd.h
# End Source File
# Begin Source File

SOURCE=.\anim.h
# End Source File
# Begin Source File

SOURCE=.\armor.h
# End Source File
# Begin Source File

SOURCE=.\armrtmpl.h
# End Source File
# Begin Source File

SOURCE=.\attribute.h
# End Source File
# Begin Source File

SOURCE=.\boat.h
# End Source File
# Begin Source File

SOURCE=.\boatcomp.h
# End Source File
# Begin Source File

SOURCE=.\boatemod.h
# End Source File
# Begin Source File

SOURCE=.\cfgemod.h
# End Source File
# Begin Source File

SOURCE=.\cfgrepos.h
# End Source File
# Begin Source File

SOURCE=.\cgdata.h
# End Source File
# Begin Source File

SOURCE=.\charactr.h
# End Source File
# Begin Source File

SOURCE=.\checkpnt.h
# End Source File
# Begin Source File

SOURCE=.\clidata.h
# End Source File
# Begin Source File

SOURCE=.\client.h
# End Source File
# Begin Source File

SOURCE=.\cliface.h
# End Source File
# Begin Source File

SOURCE=.\cmbtcfg.h
# End Source File
# Begin Source File

SOURCE=.\cmdlevel.h
# End Source File
# Begin Source File

SOURCE=.\config.h
# End Source File
# Begin Source File

SOURCE=.\containr.h
# End Source File
# Begin Source File

SOURCE=.\core.h
# End Source File
# Begin Source File

SOURCE=.\cryptengine.h
# End Source File
# Begin Source File

SOURCE=.\crypwrap.h
# End Source File
# Begin Source File

SOURCE=.\ctable.h
# End Source File
# Begin Source File

SOURCE=.\datastore.h
# End Source File
# Begin Source File

SOURCE=.\decay.h
# End Source File
# Begin Source File

SOURCE=.\dice.h
# End Source File
# Begin Source File

SOURCE=.\door.h
# End Source File
# Begin Source File

SOURCE=.\dtrace.h
# End Source File
# Begin Source File

SOURCE=.\dye.h
# End Source File
# Begin Source File

SOURCE=.\equipdsc.h
# End Source File
# Begin Source File

SOURCE=.\equipmnt.h
# End Source File
# Begin Source File

SOURCE=.\esignal.h
# End Source File
# Begin Source File

SOURCE=.\eventid.h
# End Source File
# Begin Source File

SOURCE=.\exscrobj.h
# End Source File
# Begin Source File

SOURCE=.\extcmd.h
# End Source File
# Begin Source File

SOURCE=.\fnsearch.h
# End Source File
# Begin Source File

SOURCE=.\gameclck.h
# End Source File
# Begin Source File

SOURCE=.\getitem.h
# End Source File
# Begin Source File

SOURCE=.\gflag.h
# End Source File
# Begin Source File

SOURCE=.\gprepos.h
# End Source File
# Begin Source File

SOURCE=.\gprops.h
# End Source File
# Begin Source File

SOURCE=.\guardrgn.h
# End Source File
# Begin Source File

SOURCE=.\guilds.h
# End Source File
# Begin Source File

SOURCE=.\heaven.h
# End Source File
# Begin Source File

SOURCE=.\house.h
# End Source File
# Begin Source File

SOURCE=.\iostats.h
# End Source File
# Begin Source File

SOURCE=.\item.h
# End Source File
# Begin Source File

SOURCE=.\itemdesc.h
# End Source File
# Begin Source File

SOURCE=.\layers.h
# End Source File
# Begin Source File

SOURCE=.\lightlvl.h
# End Source File
# Begin Source File

SOURCE=.\listenpt.h
# End Source File
# Begin Source File

SOURCE=.\loadunld.h
# End Source File
# Begin Source File

SOURCE=.\lockable.h
# End Source File
# Begin Source File

SOURCE=.\logfiles.h
# End Source File
# Begin Source File

SOURCE=.\los.h
# End Source File
# Begin Source File

SOURCE=.\mdelta.h
# End Source File
# Begin Source File

SOURCE=.\menu.h
# End Source File
# Begin Source File

SOURCE=.\menuinst.h
# End Source File
# Begin Source File

SOURCE=.\miscrgn.h
# End Source File
# Begin Source File

SOURCE=.\mkscrobj.h
# End Source File
# Begin Source File

SOURCE=.\movecost.h
# End Source File
# Begin Source File

SOURCE=.\msgfiltr.h
# End Source File
# Begin Source File

SOURCE=.\msghandl.h
# End Source File
# Begin Source File

SOURCE=.\multi.h
# End Source File
# Begin Source File

SOURCE=.\multidef.h
# End Source File
# Begin Source File

SOURCE=.\musicrgn.h
# End Source File
# Begin Source File

SOURCE=.\nmsgtype.h
# End Source File
# Begin Source File

SOURCE=.\npc.h
# End Source File
# Begin Source File

SOURCE=.\npcemod.h
# End Source File
# Begin Source File

SOURCE=.\npctmpl.h
# End Source File
# Begin Source File

SOURCE=.\objecthash.h
# End Source File
# Begin Source File

SOURCE=.\objtype.h
# End Source File
# Begin Source File

SOURCE=.\osemod.h
# End Source File
# Begin Source File

SOURCE=.\polcfg.h
# End Source File
# Begin Source File

SOURCE=.\polclass.h
# End Source File
# Begin Source File

SOURCE=.\polclock.h
# End Source File
# Begin Source File

SOURCE=.\poldbg.h
# End Source File
# Begin Source File

SOURCE=.\polfile.h
# End Source File
# Begin Source File

SOURCE=.\polopt.h
# End Source File
# Begin Source File

SOURCE=.\polresource.h
# End Source File
# Begin Source File

SOURCE=.\polsem.h
# End Source File
# Begin Source File

SOURCE=.\polsig.h
# End Source File
# Begin Source File

SOURCE=.\polstats.h
# End Source File
# Begin Source File

SOURCE=.\polsystememod.h
# End Source File
# Begin Source File

SOURCE=.\poltest.h
# End Source File
# Begin Source File

SOURCE=.\poltimer.h
# End Source File
# Begin Source File

SOURCE=.\poltype.h
# End Source File
# Begin Source File

SOURCE=.\polvar.h
# End Source File
# Begin Source File

SOURCE=.\polwww.h
# End Source File
# Begin Source File

SOURCE=.\profile.h
# End Source File
# Begin Source File

SOURCE=.\proplist.h
# End Source File
# Begin Source File

SOURCE=.\readcfg.h
# End Source File
# Begin Source File

SOURCE=.\reagents.h
# End Source File
# Begin Source File

SOURCE=.\realms.h
# End Source File
# Begin Source File

SOURCE=.\reftypes.h
# End Source File
# Begin Source File

SOURCE=.\region.h
# End Source File
# Begin Source File

SOURCE=.\repsys.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\schedule.h
# End Source File
# Begin Source File

SOURCE=.\scrdef.h
# End Source File
# Begin Source File

SOURCE=.\scrsched.h
# End Source File
# Begin Source File

SOURCE=.\scrstore.h
# End Source File
# Begin Source File

SOURCE=.\servdesc.h
# End Source File
# Begin Source File

SOURCE=.\sfx.h
# End Source File
# Begin Source File

SOURCE=.\skilladv.h
# End Source File
# Begin Source File

SOURCE=.\skillid.h
# End Source File
# Begin Source File

SOURCE=.\skills.h
# End Source File
# Begin Source File

SOURCE=.\sockets.h
# End Source File
# Begin Source File

SOURCE=.\sockio.h
# End Source File
# Begin Source File

SOURCE=.\spelbook.h
# End Source File
# Begin Source File

SOURCE=.\spells.h
# End Source File
# Begin Source File

SOURCE=.\ssopt.h
# End Source File
# Begin Source File

SOURCE=.\stackcfg.h
# End Source File
# Begin Source File

SOURCE=.\startloc.h
# End Source File
# Begin Source File

SOURCE=.\statmsg.h
# End Source File
# Begin Source File

SOURCE=.\storage.h
# End Source File
# Begin Source File

SOURCE=.\stubdata.h
# End Source File
# Begin Source File

SOURCE=.\syshook.h
# End Source File
# Begin Source File

SOURCE=.\syshookscript.h
# End Source File
# Begin Source File

SOURCE=.\target.h
# End Source File
# Begin Source File

SOURCE=.\tasks.h
# End Source File
# Begin Source File

SOURCE=.\textcmd.h
# End Source File
# Begin Source File

SOURCE=.\tildecmd.h
# End Source File
# Begin Source File

SOURCE=.\tiplist.h
# End Source File
# Begin Source File

SOURCE=.\tmpvars.h
# End Source File
# Begin Source File

SOURCE=.\ucfg.h
# End Source File
# Begin Source File

SOURCE=.\uconst.h
# End Source File
# Begin Source File

SOURCE=.\ucrypto.h
# End Source File
# Begin Source File

SOURCE=.\udatfile.h
# End Source File
# Begin Source File

SOURCE=.\ufunc.h
# End Source File
# Begin Source File

SOURCE=.\ufuncinl.h
# End Source File
# Begin Source File

SOURCE=.\ufuncstd.h
# End Source File
# Begin Source File

SOURCE=.\uimport.h
# End Source File
# Begin Source File

SOURCE=.\umanip.h
# End Source File
# Begin Source File

SOURCE=.\umap.h
# End Source File
# Begin Source File

SOURCE=.\umsgtype.h
# End Source File
# Begin Source File

SOURCE=.\uniemod.h
# End Source File
# Begin Source File

SOURCE=.\uobjcnt.h
# End Source File
# Begin Source File

SOURCE=.\uobject.h
# End Source File
# Begin Source File

SOURCE=.\uobjhelp.h
# End Source File
# Begin Source File

SOURCE=.\uoclient.h
# End Source File
# Begin Source File

SOURCE=.\uoemod.h
# End Source File
# Begin Source File

SOURCE=.\uoexec.h
# End Source File
# Begin Source File

SOURCE=.\uoexhelp.h
# End Source File
# Begin Source File

SOURCE=.\uofile.h
# End Source File
# Begin Source File

SOURCE=.\uofilei.h
# End Source File
# Begin Source File

SOURCE=.\uoscrobj.h
# End Source File
# Begin Source File

SOURCE=.\uoskills.h
# End Source File
# Begin Source File

SOURCE=.\useskill.h
# End Source File
# Begin Source File

SOURCE=.\ustruct.h
# End Source File
# Begin Source File

SOURCE=.\utilemod.h
# End Source File
# Begin Source File

SOURCE=.\utype.h
# End Source File
# Begin Source File

SOURCE=.\uvars.h
# End Source File
# Begin Source File

SOURCE=.\uworld.h
# End Source File
# Begin Source File

SOURCE=.\vital.h
# End Source File
# Begin Source File

SOURCE=.\watch.h
# End Source File
# Begin Source File

SOURCE=.\weapon.h
# End Source File
# Begin Source File

SOURCE=.\wepntmpl.h
# End Source File
# Begin Source File

SOURCE=.\wrldsize.h
# End Source File
# Begin Source File

SOURCE=.\wthrtype.h
# End Source File
# Begin Source File

SOURCE=.\xbuffer.h
# End Source File
# Begin Source File

SOURCE=.\zone.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\pol.rc
# End Source File
# Begin Source File

SOURCE=.\poltray.ico
# End Source File
# End Group
# Begin Group "crypt"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\crypt\blowfish.cpp
# End Source File
# Begin Source File

SOURCE=.\crypt\blowfish.h
# End Source File
# Begin Source File

SOURCE=.\crypt\crypt200.cpp
# End Source File
# Begin Source File

SOURCE=.\crypt\cryptbase.cpp
# End Source File
# Begin Source File

SOURCE=.\crypt\cryptbase.h
# End Source File
# Begin Source File

SOURCE=.\crypt\cryptengine.h
# End Source File
# Begin Source File

SOURCE=.\crypt\cryptkey.cpp
# End Source File
# Begin Source File

SOURCE=.\crypt\cryptkey.h
# End Source File
# Begin Source File

SOURCE=.\crypt\nocrypt.cpp
# End Source File
# Begin Source File

SOURCE=.\crypt\nocrypt.h
# End Source File
# Begin Source File

SOURCE=.\crypt\twofish.cpp
# End Source File
# Begin Source File

SOURCE=.\crypt\twofish.h
# End Source File
# End Group
# Begin Source File

SOURCE="..\doc\core-changes.txt"
# End Source File
# End Target
# End Project
