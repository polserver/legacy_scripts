/////////////////////////////////////////////////////////////////////////////
//
//  spellutil.inc - spell casting utilities
//
//  Spells defined here are appropriate for calling from NPC scripts, as
//  well as character cast scripts.  They assume line-of-sight has been 
//  checked.
//  All spells take parameters (caster, cast_on);
//
//  Spells defined:
//      cast_ebolt()
//
//  Helper functions defined:
//      reflectable_moving_direct_damage_spell()
//
//
//  Last Modified: 1999/02/05 Syzygy
//
/////////////////////////////////////////////////////////////////////////////

use os;
use uo;

function cast_ebolt( caster, cast_on )
    local die_string := "6d6+1";
    local dmg := RandomDiceRoll( die_string );
    dmg := dmg + (GetSkill( caster, SKILLID_MAGERY ) / 10);
	
    reflectable_moving_direct_damage_spell(
            caster,
            cast_on,
            moving_effect := FX_ENERGY_BOLT,
            speed := 0xa,
            loop := 0x1e,
            explode := 1,
            sound_effect := SFX_SPELL_ENERGY_BOLT,
            dmg := dmg );
endfunction

function reflectable_moving_direct_damage_spell(   
                                    caster,
                                    cast_on,
                                    moving_effect,
                                    speed,
                                    loop,
                                    explode,
                                    sound_effect,
                                    dmg )
    local thetarget;

    PlayMovingEffect(caster, cast_on, moving_effect, speed, loop, 0);
    PlaySoundEffect( cast_on, sound_effect );

    if (GetObjProperty(cast_on, "mr") = "1")  //target has MR on
        EraseObjProperty(cast_on, "mr"); 
    
        sleepms( 1200 );
        PlayMovingEffect(cast_on, caster, moving_effect, speed, loop, explode);
        PlaySoundEffect( caster, sound_effect );

            //
            // Do we really want to do this reflect-reflect deal?
            //

        if (GetObjProperty( caster, "mr") = "1") //caster has MR too
            EraseObjProperty(caster, "mr");
            PrintTextAbovePrivate(caster, "Your magic reflect spell saved you!", caster);
            return;
        else
            PrintTextAbovePrivate(caster, "The spell is reflected back to you!", caster);
            thetarget := caster;
        endif      //caster w/MR
    else  //he doesn't have MR on
        thetarget := cast_on;
    endif

        //
        // TODO: Check Resisting Spells.
        //

    ApplyDamage(thetarget, dmg);
endfunction

