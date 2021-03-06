/*
        Maintened by *Edwards

        2009-02-09
*/
use uo;
use os;
use cfgfile;

include ":attributes:attributes";

function AP_GetLevelTitle( skill_level )

	skill_level := CDbl( skill_level );
	if( skill_level < 0.0 )
		return 0;
	endif

	var title_cfg := ReadConfigFile( ":attributes:titles" );
	if( !title_cfg )
		return title_cfg;
	endif

	var title_elem := title_cfg["LevelTitles"];
	if( !title_elem )
		return title_elem;
	endif

	var levels_list := GetConfigStringArray( title_elem, "Level" );
	var cur_title;
	foreach level in ( levels_list )
		level := SplitWords( level );
		var amt := CDbl( level[1] );
		var title := CStr( level[2] );

		if( skill_level >= amt )
			cur_title := title;
		else
			break;
		endif
                SleepMS(5);
	endforeach

	return cur_title;
endfunction

function AP_GetSkillTitle( skill_name )

	var elem_info := AP_GetAttributeCfgElem( skill_name );
	if( !elem_info )
		return elem_info;
	endif

	if( elem_info.Title )
		return elem_info.Title;
	else
		return error{"errortext":="Attribute does not have a 'Title' entry."};
	endif
endfunction

function AP_CheckSkillModulation( skill_amount, item_modulation )
            
        case( item_modulation )
        	1: if( skill_amount > 75 ) // > Apprentice
                           return 0;
                   endif
                   break;
        	2: if( skill_amount > 90 ) // > Journeyman
                           return 0;
                   endif
                   break;
        	3: if( skill_amount > 110 ) // > Expert
                           return 0;
                   endif
                   break;
        	4: if( skill_amount > 125 ) // > Adept
                           return 0;
                   endif
                   break;
        	5: if( skill_amount > 140 ) // > Master
                           return 0;
                   endif
                   break;
        	6: if( skill_amount > 150 ) // > GrandMaster
                           return 1;
                   endif
                   break;
        endcase
        
        return 1;
endfunction
