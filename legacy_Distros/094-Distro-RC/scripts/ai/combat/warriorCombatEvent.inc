include "include/attributes";
function process_combat_event(opponent)
    if ((GetHp(me) * 100/GetMaxHp(me)) < flee_point)
      EraseObjProperty(me,"#flees");
      flee(opponent);
    elseif (GetHp(me) < GetMaxHp(me)/2)
      ApplyHealing();
    endif
endfunction

function process_flee_event(opponent)
  return 0;
endfunction

function in_combat_event_loop(opponent, loops)
  if ( loops > 50 )
    flee(opponent);
    return;
  endif
endfunction

function post_combat()
  DisableCombatEvents();
  EnableMainEvents();
  SetWarMode( 0 );
  SetOpponent( 0 );
  sleep(1);
  look_around();
endfunction

function prepare_for_fight(opponent)
  DisableMainEvents();
  EnableCombatEvents();
endfunction

function ApplyHealing()
  if ( GetObjProperty(me,"#lasthealed") > ReadGameClock())
    SetHp(me, GetHp(me) + (RandomInt(GetMaxHp(me) - GetHp(me))+1));
    SetObjProperty(me,"#lasthealed", ReadGameClock()+20);
  endif
endfunction