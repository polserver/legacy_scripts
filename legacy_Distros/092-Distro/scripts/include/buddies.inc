

function ask_for_help(opponent)

local range := cint(me.hp/10);


local ev := array;
ev.+ type;
ev.+source;
ev.+buddy;


ev.type := EVID_BUDDYATTACKED;
ev.source := opponent;
ev.buddy := me;

foreach critter in listmobilesnearlocation(me.x, me.y, me.z, range)
        if (!critter.npctemplate)
                SendEvent(critter, ev);
        endif
endforeach

endfunction



               