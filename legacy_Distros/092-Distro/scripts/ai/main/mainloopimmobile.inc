function main_AI_loop()

    local next_wander := ReadGameClock() + 10;
    local ev;
    local wanders := 60;

    look_around();

    EnableMainEvents();
 
    while (1)

	if (readgameclock() > next_wander)
		wanders := wanders +1;
		next_wander := readgameclock()+10;
		if (wanders > 60)
			wanders :=0;
			ev := sleepmode();
	    	endif
	else
		ev := os::wait_for_event( 10 );
	endif

        repeat

            case (ev.type)
                EVID_ENGAGED:
                EVID_DAMAGED:
                    Fight( ev.source );
            endcase

        until (! (ev := os::wait_for_event(10)) );

   endwhile      

endfunction        

function look_around()

	return;

endfunction

function EnableMainEvents()

    DisableEvents( EVID_SPEECH + EVID_LEFTAREA + EVID_DISENGAGED + EVID_OPPONENT_MOVED + EVID_ENTEREDAREA );
    EnableEvents( EVID_ENGAGED + EVID_DAMAGED, HALT_THRESHOLD );

endfunction

function DisableMainEvents()

    DisableEvents( EVID_ENGAGED + EVID_DAMAGED + EVID_ENTEREDAREA);

endfunction
