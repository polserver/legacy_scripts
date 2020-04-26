//randname.inc
//
//assigns a random name from the list according to obtype and gender

use uo;
use os;
use cfgfile;
use util;
use basic;

include "include/randname_util";

function RandomName( me )

	local newname := " ";
	case( me.graphic )
	    OBJ_BIRD:		newname := AssignName( me , BIRD_IDX );
	    OBJ_DAEMON:		newname := AssignName( me , DAEMON_IDX );
	    OBJ_DAEMON_SWORD:	newname := AssignName( me , DAEMON_IDX );
	    OBJ_DRAGON_RED:	newname := AssignName( me , DRAGON_IDX );
	    OBJ_DRAGON_GREY: 	newname := AssignName( me , DRAGON_IDX );
	    OBJ_MALE: 		newname := AssignName( me , MALE_IDX );
	    OBJ_FEMALE:		newname := AssignName( me , FEMALE_IDX );
	    OBJ_LIZARD:		newname := AssignName( me , LIZARD_IDX );
	    OBJ_LIZARD_MACE:	newname := AssignName( me , LIZARD_IDX );
	    OBJ_LIZARD_SPEAR:	newname := AssignName( me , LIZARD_IDX );
	    OBJ_ORC:		newname := AssignName( me , ORC_IDX );
	    OBJ_ORC_CLUB:	newname := AssignName( me , ORC_IDX );
	    OBJ_ORC_CAPT:	newname := AssignName( me , ORC_IDX );
	    OBJ_OGRE:		newname := AssignName( me , OGRE_IDX );
	    OBJ_RATMAN:		newname := AssignName( me , RATMAN_IDX );
	    OBJ_RATMAN_SWORD:	newname := AssignName( me , RATMAN_IDX );
	    OBJ_RATMAN_AXE:	newname := AssignName( me , RATMAN_IDX );
	    default: 		exit;
	endcase

	return(newname);
endfunction


function AssignName( me , index )

    local cfg := ReadConfigFile( "names" );
    local elem := FindConfigElem( cfg , index );
    local number := GetConfigInt( elem , "Count" );

    local dice_str := "1d" + cstr(number);

    local choice := RandomDiceRoll( dice_str );
    local entry := "Name" + choice;

    local str := GetConfigString( elem , entry );
    local title := PreserveTitle( me );
   
    str := str + title;

    return( str );

endfunction


function PreserveTitle( me )

  local myname := me.name;
  local start := find( myname , ">" , 1 );

if( !start )
  return;
endif

  local title := myname[ start+1 , (len( myname ) - 8) ];

if( !title )
  return("");
endif

  return( title );

endfunction




