function process_combat_event(ev)

	spellattack(ev.source);
        if (ev.type = EVID_DAMAGED)
		if ((me.hp * 100/me.maxhp) < flee_point)
			flee(ev.source);
		endif
        endif

endfunction

function spellattack(opponent);

        cast_meteor(me);

endfunction

function in_combat_event_loop(opponent, loops)

	spellattack(opponent);

	if ( loops > 100 )
		flee(opponent);
		return;
	endif
         
endfunction

function prepare_for_fight(opponent)

	DisableMainEvents();
	EnableCombatEvents();

endfunction

function post_combat()

	DisableCombatEvents();
	EnableMainEvents();

	SetWarMode( 0 );
	SetOpponent( 0 );

	sleep(1);
	look_around();

endfunction


function cast_meteor( caster )

	if (!GetObjProperty(me,"#lastbreath"))
		SetObjProperty(me,"#lastbreath",ReadGameClock() + 10);
	endif

	if (GetObjProperty(me,"#lastbreath") > ReadGameClock() )
		return;
	endif

	SetObjProperty(me,"#lastbreath",ReadGameClock() + 10);

	if (RandomInt(3) != 1)
		return;
	endif

    local parms array;

    local targets := ListMobilesInLineofSight(me, 4);
    PerformAction(me,0x0c);

    foreach mobile in targets
      if ( (!mobile.npctemplate or mobile.script = "tamed") and (!mobile.cmdlevel))
	   Local dmg := randomint((cint( cint(me.maxhp/2.5) * me.hp)/me.maxhp))+5;
	   PlayMovingEffect(caster, mobile ,MOVING_EFFECT_FIREBALL, 10,1,1);
	   applydamage(mobile, dmg);           
      endif
    endforeach

endfunction
