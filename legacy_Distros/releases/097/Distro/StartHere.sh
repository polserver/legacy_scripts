#! /bin/sh
# PenUltima Online Manager v0.0.2
# Original tool by Austin Heilman
# Ported to linux by Axel DominatoR

#
# Changes:
#
# TODO:
#	- Add a deep check the first time POL is started.
#		Maybe check even for .exe versions, etc.
#	- Add saved configuration for POLManager
#	- Add pol.cfg gui configurator
#	- .ecl/.src file checking is somewhat bad. Best to find another way...
#
# 0.0.2 -
#	- Added startup check
#	- GUI remade. Now is more "interactive"
#	- General code cleanup
#	- Menu is now case insensitive
#
# 0.0.1 -
#	- First linux release
#

POLMANAGER_VER="0.0.2"

POL_EXE="pol"
ECOMPILE_EXE="scripts/ecompile"
UOCONVERT_EXE="uoconvert"
MUL_DIR="MUL"

CHECK_REALMS=0
CHECK_CFG=0
CHECK_MULTIS_CFG=0
CHECK_TILES_CFG=0
CHECK_LANDTILES_CFG=0
CHECK_ECL=0
CHECK_NEEDCLEAN=0

#
# Main program routines
#
echo
echo -e "\E[31;1mPenUltima Online \E[33mManager \E[0;38mv$POLMANAGER_VER\E[0m"
echo -e "by \E[31mAxel DominatoR ^^^ HC\E[0m"
echo -e "Original made by Austin Heilman, special thanks to him for such a tool!"

Brief_Check()
{
	CHECK_REALMS=0
	CHECK_CFG=0
	CHECK_MULTIS_CFG=0
	CHECK_TILES_CFG=0
	CHECK_LANDTILES_CFG=0
	echo
	echo -e "* Analyzing setup in \E[33m`pwd`\E[0m"

# Check if realm dir is present and if at least one realm is generated
	if [ -d "realm" ]; then
		if [ $(ls -1A realm | wc -l) -eq 0 ]; then
			echo -e "- No Realms generated! \E[31mThat's bad!\E[0m"
			CHECK_REALMS=1
		else
			echo -e "- At least one Realm is present. \E[32mGood!\E[0m"
		fi
	else
		echo -e "- Realm dir does not exists, creating it."
		mkdir realm
		echo -e "  Now you need to \E[31mgenerate Realms!\E[0m"
		CHECK_REALMS=1
	fi
# Check for autogenerated .cfg files
	if [ -e "config/multis.cfg" ]; then
		echo -e "- multis.cfg is present. \E[32mGood!\E[0m"
	else
		echo -e "- multis.cfg is not present! \E[31mThat's bad!\E[0m"
		CHECK_CFG=1
		CHECK_MULTIS_CFG=1
	fi
	if [ -e "config/tiles.cfg" ]; then
		echo -e "- tiles.cfg is present. \E[32mGood!\E[0m"
	else
		echo -e "- tiles.cfg is not present! \E[31mThat's bad!\E[0m"
		CHECK_CFG=1
		CHECK_TILES_CFG=1
	fi
	if [ -e "config/landtiles.cfg" ]; then
		echo -e "- landtiles.cfg is present. \E[32mGood!\E[0m"
	else
		echo -e "- landtiles.cfg is not present! \E[31mThat's bad!\E[0m"
		CHECK_CFG=1
		CHECK_LANDTILES_CFG=1
	fi
}

Scripts_Check()
{
	CHECK_ECL=0
	CHECK_NEEDCLEAN=0

# Check for uncompiled scriptfiles
	SRC_FILENUM=`find -iname *.src | wc -l`
	ECL_FILENUM=`find -iname *.ecl | wc -l`
	echo -e "  Found $SRC_FILENUM .src and $ECL_FILENUM .ecl"
	if [ $SRC_FILENUM -eq $ECL_FILENUM ]; then
		echo -e "- All scripts are compiled. \E[32mGood!\E[0m"
	else
		if [ $SRC_FILENUM -gt $ECL_FILENUM ]; then
			echo -e "- Some scripts are not compiled. \E[31mCheck them ASAP!\E[0m"
			CHECK_ECL=1
		else
			echo -e "- More .ecl exists than .src. A \E[31mclean\E[0m is reccomended."
			CHECK_NEEDCLEAN=1
		fi
	fi
}

Startup_Check()
{
	Brief_Check
	Scripts_Check

	echo -e "Press ENTER to continue..."
	read line
}

Menu_Data()
{
	Build_Britannia()
	{
		clear
		echo -e "================================================================================"
		echo -e " \E[31;1mPOLManager v$POLMANAGER_VER\E[0m   \E[0m[ \E[33;1mRealm Generation \E[0m]"
		echo -e "================================================================================"
		echo -e "[ \E[33;1mA\E[0m ] - \E[32m\E[1mT2A\E[0;32m - \E[1mUOSE\E[0;32m Britannia Map\E[0m"
		echo -e "[ \E[33;1mB\E[0m ] - \E[32m\E[1mMondain's Legacy\E[0;32m extended Britannia Map\E[0m"
		echo
		echo -e "[ \E[31;1mX\E[0m ] - \E[31mBack\E[0m"
		echo -e "================================================================================"
		echo -n "> "
		read line
		line=`echo $line | tr "[:upper:]" "[:lower:]"`
		if [ "$line" = "a" ]; then ./$UOCONVERT_EXE map realm=britannia mapid=0 usedif=1 width=6144 height=4096
		elif [ "$line" = "b" ]; then ./$UOCONVERT_EXE map realm=britannia mapid=0 usedif=1 width=7168 height=4096
		else
			Menu_Data
		fi
		./$UOCONVERT_EXE statics realm=britannia
		./$UOCONVERT_EXE maptile realm=britannia
	}

	Build_Britannia_Alt()
	{
		./$UOCONVERT_EXE map     realm=britannia_alt mapid=1 usedif=1 width=6144 height=4096
		./$UOCONVERT_EXE statics realm=britannia_alt
		./$UOCONVERT_EXE maptile realm=britannia_alt
	}

	Build_Ilshenar()
	{
		./$UOCONVERT_EXE map     realm=ilshenar      mapid=2 usedif=1 width=2304 height=1600
		./$UOCONVERT_EXE statics realm=ilshenar
		./$UOCONVERT_EXE maptile realm=ilshenar
	}

	Build_Malas()
	{
		./$UOCONVERT_EXE map     realm=malas         mapid=3 usedif=1 width=2560 height=2048
		./$UOCONVERT_EXE statics realm=malas
		./$UOCONVERT_EXE maptile realm=malas
	}

	Build_Tokuno()
	{
		./$UOCONVERT_EXE map     realm=tokuno        mapid=4 usedif=1 width=1448 height=1448
		./$UOCONVERT_EXE statics realm=tokuno
		./$UOCONVERT_EXE maptile realm=tokuno
	}

	clear
	echo -e "================================================================================"
	echo -e " \E[31;1mPOLManager v$POLMANAGER_VER\E[0m   \E[0m[ \E[33;1mData files \E[0m]"
	echo -e "================================================================================"
	echo -e "[ \E[33;1mA\E[0m ] - \E[32mCopy needed client files to pol/MUL (move this to cfg section )\E[0m"
	echo
	echo -e "[ \E[33;1mB\E[0m ] - \E[32mBuild all needed config files\E[0m"

	echo -en "[ \E[33;1mC\E[0m ] - "
	if [ "$CHECK_LANDTILES_CFG" -ne "0" ]; then echo -en "\E[33m"
	else echo -en "\E[32m"
	fi
	echo -e "Build \E[1mlandtiles.cfg\E[0m"

	echo -en "[ \E[33;1mD\E[0m ] - "
	if [ "$CHECK_MULTIS_CFG" -ne "0" ]; then echo -en "\E[33m"
	else echo -en "\E[32m"
	fi
	echo -e "Build \E[1mmultis.cfg\E[0m"

	echo -en "[ \E[33;1mE\E[0m ] - "
	if [ "$CHECK_TILES_CFG" -ne "0" ]; then echo -en "\E[33m"
	else echo -en "\E[32m"
	fi
	echo -e "Build \E[1mtiles.cfg\E[0m"
	echo
	echo -e "[ \E[33;1mF\E[0m ] - \E[32mBuild all realms\t\t\E[0m(Takes a very long time!)"
	echo -e "[ \E[33;1mG\E[0m ] - \E[32mBuild \E[1mBritannia\E[0;32m realm\t\t\E[0m(mapid=\E[36;1m0\E[0m)"
	echo -e "[ \E[33;1mH\E[0m ] - \E[32mBuild \E[1mBritannia Alt\E[0;32m realm\t\E[0m(mapid=\E[36;1m1\E[0m)"
	echo -e "[ \E[33;1mI\E[0m ] - \E[32mBuild \E[1mIlshenar\E[0;32m realm\t\t\E[0m(mapid=\E[36;1m2\E[0m)"
	echo -e "[ \E[33;1mJ\E[0m ] - \E[32mBuild \E[1mMalas\E[0;32m realm\t\t\E[0m(mapid=\E[36;1m3\E[0m)"
	echo -e "[ \E[33;1mK\E[0m ] - \E[32mBuild \E[1mTokuno\E[0;32m realm\t\t\E[0m(mapid=\E[36;1m4\E[0m)"
	echo
	echo -e "[ \E[31;1mX\E[0m ] - \E[31mBack\E[0m"
	echo -e "================================================================================"
	echo -n "> "
	read line
	line=`echo $line | tr "[:upper:]" "[:lower:]"`
	if [ "$line" = "a" ]; then
		echo -n "Path to UO directory> "
		read line
		if [ ! -d "$MUL_DIR" ]; then mkdir "$MUL_DIR"
		fi

		cp "$line/multi.*" "$MUL_DIR"
		cp "$line/map*" "$MUL_DIR"
		cp "$line/staidx*" "$MUL_DIR"
		cp "$line/statics*" "$MUL_DIR"
		cp "$line/tiledata.mul" "$MUL_DIR"
	elif [ "$line" = "b" ]; then
		./$UOCONVERT_EXE landtiles; mv landtiles.cfg config
		./$UOCONVERT_EXE multis; mv multis.cfg config
		./$UOCONVERT_EXE tiles; mv tiles.cfg config
	elif [ "$line" = "c" ]; then ./$UOCONVERT_EXE landtiles; mv landtiles.cfg config; Brief_Check;
	elif [ "$line" = "d" ]; then ./$UOCONVERT_EXE multis; mv multis.cfg config; Brief_Check;
	elif [ "$line" = "e" ]; then ./$UOCONVERT_EXE tiles; mv tiles.cfg config; Brief_Check;
	elif [ "$line" = "f" ]; then Build_Britannia; Build_Britannia_Alt; Build_Ilshenar; Build_Malas; Build_Tokuno
	elif [ "$line" = "g" ]; then Build_Britannia
	elif [ "$line" = "h" ]; then Build_Britannia_Alt
	elif [ "$line" = "i" ]; then Build_Ilshenar
	elif [ "$line" = "j" ]; then Build_Malas
	elif [ "$line" = "k" ]; then Build_Tokuno
	elif [ "$line" = "x" ]; then Menu_Main
	fi

	Menu_Data
}

Menu_Scripts()
{
	clear
	echo -e "================================================================================"
	echo -e " \E[31;1mPOLManager v$POLMANAGER_VER\E[0m   \E[0m[ \E[33;1mScript Compilation \E[0m]"
	echo -e "================================================================================"

	if [ "$CHECK_ECL" -ne "0" ]; then
		echo -e "[ \E[33;1mA\E[0m ] - \E[33mCompile all scripts\E[0m"
		echo -e "[ \E[33;1mB\E[0m ] - \E[33mCompile all scripts and output to ecompile.log\E[0m"
		echo -e "[ \E[33;1mC\E[0m ] - \E[33mCompile updated scripts only\E[0m"
	else
		echo -e "[ \E[33;1mA\E[0m ] - \E[32mCompile all scripts\E[0m"
		echo -e "[ \E[33;1mB\E[0m ] - \E[32mCompile all scripts and output to ecompile.log\E[0m"
		echo -e "[ \E[33;1mC\E[0m ] - \E[32mCompile updated scripts only\E[0m"
	fi
	echo
	echo -e "[ \E[33;1mD\E[0m ] - \E[32mCompile a directory\E[0m"
	echo -e "[ \E[33;1mE\E[0m ] - \E[32mCompile a specific script\E[0m"
	echo
	echo -e "[ \E[31;1mX\E[0m ] - \E[31mBack\E[0m"
	echo -e "================================================================================"
	echo -n "> "
	read line
	line=`echo $line | tr "[:upper:]" "[:lower:]"`
	if [ "$line" = "a" ]; then ./$ECOMPILE_EXE -A -b -f; Scripts_Check;
	elif [ "$line" = "b" ]; then  ./$ECOMPILE_EXE -A -b -f >ecompile.log; Scripts_Check;
	elif [ "$line" = "c" ]; then  ./$ECOMPILE_EXE -A -b; Scripts_Check;
	elif [ "$line" = "d" ]; then
		echo -n "Path to directory> "
		read line
		./$ECOMPILE_EXE -b -f -r $line
	elif [ "$line" = "e" ]; then
		echo -n "Path to script> "
		read line
		./$ECOMPILE_EXE $line
	elif [ "$line" = "x" ]; then Menu_Main
	fi

	Menu_Scripts
}

Menu_Cleanup()
{
	clear
	echo -e "================================================================================"
	echo -e " \E[31;1mPOLManager v$POLMANAGER_VER\E[0m   \E[0m[ \E[33;1mCleanup \E[0m]"
	echo -e "================================================================================"
	echo -e "[ \E[33;1mA\E[0m ] - \E[32mOne-Shot Cleaner\E[0m\t(Will remove all of the following)"
	echo
	if [ "$CHECK_NEEDCLEAN" -ne "0" ]; then
		echo -e "[ \E[33;1mB\E[0m ] - \E[33mRemove \E[1m*.ecl\E[0;33m files\E[0m\t(Will need to recompile scripts)"
	else
		echo -e "[ \E[33;1mB\E[0m ] - \E[32mRemove \E[1m*.ecl\E[0;32m files\E[0m\t(Will need to recompile scripts)"
	fi
	echo -e "[ \E[33;1mC\E[0m ] - \E[32mRemove \E[1m*.bak\E[0;32m files\E[0m"
	echo -e "[ \E[33;1mD\E[0m ] - \E[32mRemove \E[1m*.dep\E[0;32m files\E[0m"
	echo -e "[ \E[33;1mE\E[0m ] - \E[32mRemove \E[1m*.log\E[0;32m files\E[0m"
	echo -e "[ \E[33;1mF\E[0m ] - \E[32mRemove \E[1m*.lst\E[0;32m files\E[0m"
	echo -e "[ \E[33;1mG\E[0m ] - \E[32mRemove \E[1m*.dbg\E[0;32m files\E[0m"
	echo
	echo -e "[ \E[33;1mH\E[0m ] - \E[32mRemove other files\E[0m\t(Insert file name or wildcard)"
	echo
	echo -e "[ \E[31;1mX\E[0m ] - \E[31mBack\E[0m"
	echo -e "================================================================================"
	echo -n "> "
	read line
	line=`echo $line | tr "[:upper:]" "[:lower:]"`
	if [ "$line" = "a" ]; then
		find . -name \*.ecl -exec echo {} \; -exec rm {} \;
		find . -name \*.bak -exec echo {} \; -exec rm {} \;
		find . -name \*.dep -exec echo {} \; -exec rm {} \;
		find . -name \*.log -exec echo {} \; -exec rm {} \;
		find . -name \*.lst -exec echo {} \; -exec rm {} \;
		find . -name \*.dbg -exec echo {} \; -exec rm {} \; Scripts_Check;
	elif [ "$line" = "b" ]; then find . -name \*.ecl -exec echo {} \; -exec rm {} \; Scripts_Check;
	elif [ "$line" = "c" ]; then find . -name \*.bak -exec echo {} \; -exec rm {} \;
	elif [ "$line" = "d" ]; then find . -name \*.dep -exec echo {} \; -exec rm {} \;
	elif [ "$line" = "e" ]; then find . -name \*.log -exec echo {} \; -exec rm {} \;
	elif [ "$line" = "f" ]; then find . -name \*.lst -exec echo {} \; -exec rm {} \;
	elif [ "$line" = "g" ]; then find . -name \*.dbg -exec echo {} \; -exec rm {} \;
	elif [ "$line" = "h" ]; then
		echo -n "File Name or Wildcard> "
		read line
		find . -name $line -exec echo {} \; -exec rm {} \;
	elif [ "$line" = "x" ]; then Menu_Main
	fi

	Menu_Cleanup
}

Start_POL()
{
	clear
	./$POL_EXE
	reset
	clear
	Menu_Main
}

Loop_POL()
{
	while [ 1 ]
	do
		clear
		./$POL_EXE
	done
	Menu_Main
}

Menu_POLConfig()
{
	clear
	echo -e "================================================================================"
	echo -e " \E[31;1mPOLManager v$POLMANAGER_VER\E[0m   \E[0m[ \E[33;1mPOL Configuration \E[0m]"
	echo -e "================================================================================"
	echo -e "[ \E[33;1mA\E[0m ] - \E[32mServer name\E[0m"
	echo -e "[ \E[33;1mB\E[0m ] - \E[32mNetwork...\E[0m"
	echo -e "[ \E[33;1mC\E[0m ] - \E[32mCleanup menu\E[0m\t\t(File removal tools)"
	echo
	echo -e "[ \E[33;1mD\E[0m ] - \E[32mStart POL.exe\E[0m\t\t(Returns to menu on exit)"
	echo -e "[ \E[33;1mE\E[0m ] - \E[32mKeep POL.exe running\E[0m\t(Restarts when it exits. Use CTRL+C to stop)"
	echo
	echo -e "[ \E[33;1mF\E[0m ] - \E[32mSystem Check\E[0m"
	echo -e "[ \E[33;1mG\E[0m ] - \E[32mConfigure POL\E[0m"
	echo -e "[ \E[33;1mH\E[0m ] - \E[32mPOLManager Options\E[0m"
	echo
	echo -e "[ \E[31;1mX\E[0m ] - \E[31mBack\E[0m"
	echo -e "================================================================================"
	echo -n "> "
	read line
	line=`echo $line | tr "[:upper:]" "[:lower:]"`
	if [ "$line" = "a" ]; then Menu_Data
	elif [ "$line" = "b" ]; then Menu_Scripts
	elif [ "$line" = "c" ]; then Menu_Cleanup
	elif [ "$line" = "d" ]; then Start_POL
	elif [ "$line" = "e" ]; then Loop_POL
	elif [ "$line" = "g" ]; then Menu_POLConfig
	elif [ "$line" = "x" ]; then Menu_Main
	else Menu_POLConfig
	fi
}

Quit()
{
	echo -e "\nThanks for using \E[31mPOLManager\E[0m!\n"
	exit
}

Menu_Main()
{
	clear
	echo -e "================================================================================"
	echo -e " \E[31;1mPOLManager v$POLMANAGER_VER\E[0m   \E[0m[ \E[33;1mMain Menu \E[0m]"
	echo -e "================================================================================"

	echo -en "[ \E[33;1mA\E[0m ] - "
	if [ "$CHECK_REALMS" -ne "0" ] || [ "$CHECK_CFG" -ne "0" ]; then echo -en "\E[33m"
	else echo -en "\E[32m"
	fi
	echo -e "Data files...\E[0m\t\t(Generate Config files and Realms)"

	echo -en "[ \E[33;1mB\E[0m ] - "
	if [ "$CHECK_ECL" -ne "0" ]; then echo -en "\E[33m"
	else echo -en "\E[32m"
	fi
	echo -e "Scripts...\E[0m\t\t(Scripts compilation and options)"

	echo -en "[ \E[33;1mC\E[0m ] - "
	if [ "$CHECK_NEEDCLEAN" -ne "0" ]; then echo -en "\E[33m"
	else echo -en "\E[32m"
	fi
	echo -e "Cleanup...\E[0m\t\t(Cleanup tasks)"

	echo
	echo -e "[ \E[33;1mD\E[0m ] - \E[32;1mStart POL\E[0m\t\t(Returns to menu on exit)"
	echo -e "[ \E[33;1mE\E[0m ] - \E[32mKeep POL running\E[0m\t(Restarts when it exits. Use CTRL+C to stop)"
	echo
	echo -e "[ \E[33;1mF\E[0m ] - \E[32mSystem Check...\E[0m"
	echo -e "[ \E[33;1mG\E[0m ] - \E[32mConfigure POL...\E[0m"
	echo -e "[ \E[33;1mH\E[0m ] - \E[32mPOLManager Options...\E[0m"
	echo
	echo -e "[ \E[31;1mX\E[0m ] - \E[31mQuit\E[0m"
	echo -e "================================================================================"
	echo -n "> "
	read line
	line=`echo $line | tr "[:upper:]" "[:lower:]"`
	if [ "$line" = "a" ]; then Menu_Data
	elif [ "$line" = "b" ]; then Menu_Scripts
	elif [ "$line" = "c" ]; then Menu_Cleanup
	elif [ "$line" = "d" ]; then Start_POL
	elif [ "$line" = "e" ]; then Loop_POL
	elif [ "$line" = "g" ]; then Menu_POLConfig
	elif [ "$line" = "x" ]; then Quit
	else Menu_Main
	fi
}

Startup_Check
Menu_Main