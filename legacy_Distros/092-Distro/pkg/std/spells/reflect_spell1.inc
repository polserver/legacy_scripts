/* 
 *  Reflect Spell Include - meant to be included by direct-damage scripts.
 *
 *  This approach allows the compiler to optimize away many variables, and even code.
 *
 *  The following constants should be set before including:
 *      SPELL_EFFECT_TYPE_MOVING (0/1) - if true, a moving effect.  If false, object-centered.
 *      SPELL_EFFECT_ID                - effect ID
 *      SPELL_EFFECT_SPEED             - \
 *      SPELL_EFFECT_LOOP              -  - parameters for playing effect
 *      SPELL_EFFECT_EXPLODE           - /
 *      SPELL_SCRIPTNAME               - name of script to be fired, if any
 *
 *
 *  No attempt has been made to convert CInt(GetObjProperty()) stuff, or to 
 *  restructure the logic in any way.
 *
 * 
 *
 */

use os;
use uo;

function DirectDamageSpellEffect( caster, cast_on, rawdamage, script_parms := 0 )
    local reflected;        // was the spell reflected?
    local thetarget;        // either caster or cast_on, depending on reflection results

    if (GetObjProperty(cast_on, "mr") =  "1")  //target has MR on
        reflected := 1;
        SetObjProperty(cast_on, "mr", ""); 
        if (SPELL_EFFECT_TYPE_MOVING)                // moving effect
            PlayMovingEffect(caster, 
                             cast_on, 
                             SPELL_EFFECT_ID, 
                             SPELL_EFFECT_SPEED, 
                             SPELL_EFFECT_LOOP, 
                             explode := 0); // Why is this hardcoded to false?
            sleep(2);
            if (GetObjProperty( caster, "mr") = "1") //caster has MR too
                EraseObjProperty(caster, "mr");
                SendSysmessage (caster, "Your magic reflect spell saved you!");
                return;
            else
                SendSysmessage (caster, "The spell is reflected back to you!");
                thetarget := caster;
                PlayMovingEffect(cast_on, 
                                 caster, 
                                 SPELL_EFFECT_ID, 
                                 SPELL_EFFECT_SPEED, 
                                 SPELL_EFFECT_LOOP, 
                                 SPELL_EFFECT_EXPLODE);
            endif     
        else                                    // object_centered effect
            if (GetObjProperty( caster, "mr") = "1")
                EraseObjProperty(caster, "mr");
                SendSysmessage (caster, "Your magic reflect spell saved you!");
                return;
            endif
            PlayObjectCenteredEffect(caster, 
                                     SPELL_EFFECT_ID,
                                     SPELL_EFFECT_SPEED,
                                     SPELL_EFFECT_LOOP);
            print ( "played effect");
            thetarget := caster;
        endif //moving/object-centered effect
    else  //he doesn't have MR on
        if (SPELL_EFFECT_TYPE_MOVING)
            PlayMovingEffect(caster, 
                             cast_on, 
                             SPELL_EFFECT_ID, 
                             SPELL_EFFECT_SPEED,
                             SPELL_EFFECT_LOOP, 
                             SPELL_EFFECT_EXPLODE);
        else
            PlayObjectCenteredEffect(cast_on, 
                                     SPELL_EFFECT_ID, 
                                     SPELL_EFFECT_SPEED, 
                                     SPELL_EFFECT_EXPLODE);
        endif        
        thetarget := cast_on;
    endif

    /*
    if (thedamage > 0 )
            ApplyDamage(thetarget, thedamage);
    endif
    */

    if (rawdamage > 0)
        ApplyRawDamage(thetarget, rawdamage);
    endif

    if (SPELL_SCRIPTNAME != "" )
        if (script_parms)
            if (reflected)
                local thetemp;
                script_parms[1] := thetemp;
                script_parms[1] := script_parms[2] ;
                script_parms[2] := thetemp;

                print ("script_parms[2] = " + script_parms[2]);
                print ("script_parms[1] = " + script_parms[1]);
                print ("script_parms[3] = " + script_parms[3]);
            endif
            start_script( SPELL_SCRIPTNAME, script_parms ); 
        else
            start_script( SPELL_SCRIPTNAME );
        endif
    endif
endfunction
