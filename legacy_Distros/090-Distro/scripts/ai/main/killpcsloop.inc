include "include/speech";
function main_AI_loop()

    local ev;
    local wanders := 120;
    local next_loot := readgameclock() + 20;

    EnableMainEvents();

    look_around();

    while (1)

	if (ReadGameClock() > next_loot)
		grabloot();
		next_loot := ReadGameClock() + 10;
	else
		wander();
	endif

	wanders := wanders +1;

	if (wanders > 120)
		wanders :=0;
		ev := sleepmode();
	else
		ev := os::wait_for_event( 2 );
	endif

	if (ev)
	  repeat
            case (ev.type)
                EVID_SPEECH:
                	RespondToSpeech(me,ev.text);
                EVID_ENTEREDAREA:
                    if ( (!ev.source.npctemplate) or (ev.source.script = "tamed") )
		    	get_help( ev.source );
			Fight( ev.source );
                    endif
		EVID_ALL_ATTACK_CMD:
		    Fight( ev.source );
                EVID_ENGAGED:
                EVID_DAMAGED:
		    get_help( ev.source );
		    fight(ev.source);
            endcase
	  until (!(ev := os::wait_for_event(2)));
	endif

   endwhile      

endfunction       

function look_around()

	foreach npc in ListMobilesInLineOfSight( me, HALT_THRESHOLD )
          if ( (!npc.npctemplate) or (npc.script = "tamed") )
                Fight( npc );
		return;
          endif
	endforeach

endfunction

function EnableMainEvents()

    DisableEvents( EVID_SPEECH + EVID_LEFTAREA + EVID_DISENGAGED + EVID_OPPONENT_MOVED );
    EnableEvents( EVID_ENGAGED + EVID_DAMAGED + EVID_ENTEREDAREA, HALT_THRESHOLD );

endfunction

function DisableMainEvents()

    DisableEvents( EVID_ENGAGED + EVID_DAMAGED + EVID_ENTEREDAREA);

endfunction

function get_help( opponent )

	local inev;
	local ev Array;
	ev.+type := EVID_ALL_ATTACK_CMD;
	ev.+source := opponent;

	set_critical(1);
	foreach critter in ListMobilesInLineOfSight( me, HALT_THRESHOLD )
		if ( (critter.npctemplate) and (critter.script != "tamed") and (!critter.warmode) )
			SendEvent(critter, ev);
		endif
		inev := wait_for_event(0);
		if (inev = EVID_ALL_ATTACK_CMD)
			set_critical(0);
			return;
		endif
	endforeach
	set_critical(0);

endfunction
