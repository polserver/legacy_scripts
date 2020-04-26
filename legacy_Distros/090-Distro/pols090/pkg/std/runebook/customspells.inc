/////////////////////////////////////////////////////
// customspells.inc -- Simplified gate and recall code
//                     for use with runebook.src
//
// Author(s): Racalac
// Revisions: 1.0, 11/13/99: Gate, Recall
//                           Mana, Reagent consumption
//                           Normal Spell Restrictions
//
// Notes: (rev1.0) - None
//
// Dependencies: (rev1.0) - client.inc
//                          objtype.inc
//                          spellrestrictions.inc
//
/////////////////////////////////////////////////////

use uo;
use os;
use cfgfile;

include "include/client";
include "include/objtype";
include "include/spellrestrictions";

var circlescfg := ReadConfigFile("::circles");

function custom_recall( caster , parms )

        var x := parms[2];
        var y := parms[3];
        var z := parms[4];

        SpeakPowerWords( caster, 32 );
    var circelem := FindConfigElem(circlescfg, "4");
    var delay := GetConfigInt(circelem,"Delay");
    
    PerformAction( caster, ANIM_CAST_DIR );
    sleepms(delay);
        
	local circle := 4;
	if (!can_cast(caster, circle))
           return 0;
	endif

        if(!CheckSkill( caster , 25, GetSpellDifficulty(32), 50 ) )
           return 0;
        endif

        if(!ConsumeReagents( caster, 32 ))
           return 0;
        endif

        if(!ConsumeMana( caster , 32 ))
           return 0;
        endif

	local tox := x;
	local toy := y;
	local toz := z;

	PlaySoundEffect( caster, SFX_210 );
	sleepms(250);
	MoveCharacterToLocation( caster, tox, toy, toz );
	PlaySoundEffect( caster, SFX_SPELL_RECALL );

        return 1;
endfunction


function custom_gate( caster, parms )

	local circle := 7;
        var x := parms[2];
        var y := parms[3];
        var z := parms[4];

        SpeakPowerWords( caster, 52 );
        
    var circelem := FindConfigElem(circlescfg, "7");
    var delay := GetConfigInt(circelem,"Delay");
    
    PerformAction( caster, ANIM_CAST_DIR );
    sleepms(delay);
        
	if (!can_cast(caster, circle))
           return 0;
	endif

        if(!CheckSkill( caster , 25, GetSpellDifficulty(52), 50 ) )
           return 0;
        endif

        if(!ConsumeReagents( caster, 52 ))
           return 0;
        endif

        if(!ConsumeMana( caster , 52 ))
           return 0;
        endif

	local tox := x;
	local toy := y;
	local toz := z;

	set_critical( 1 );
	local gate1 := CreateItemAtLocation( caster.x, caster.y, caster.z, UOBJ_BLUE_MOONGATE, 1 );
	local gate2 := CreateItemAtLocation( tox, toy, toz, UOBJ_BLUE_MOONGATE, 1 );
	if (!gate1 or !gate2)
           return 0;
	endif
	SetObjProperty( gate1, "GateDestX", gate2.x );
	SetObjProperty( gate1, "GateDestY", gate2.y );
	SetObjProperty( gate1, "GateDestZ", gate2.z );
	SetObjProperty( gate2, "GateDestX", gate1.x );
	SetObjProperty( gate2, "GateDestY", gate1.y );
	SetObjProperty( gate2, "GateDestZ", gate1.z );

	Detach();

	set_critical( 0 );

	PlaySoundEffect( caster, SFX_SPELL_GATE_TRAVEL );

	sleep(30);
    
	set_critical(1);
	DestroyItem( gate1 );
	DestroyItem( gate2 );
	set_critical(0);

        return(1);
endfunction
