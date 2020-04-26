use os;

function do_temp_mod(who, mod, amt, duration)

local parms := {};
parms[2] := who;
parms[1] := mod;
parms[3] := amt;
parms[4] := duration;

start_script("::/misc/demod", parms);

endfunction



function can_stat_mod(who, stat)

local mod;

case (stat)
        "str": mod := "#ss";
        "dex" : mod := "#dd";
        "int" : mod := "#ii";
        "ar"  : mod := "#aa";
endcase

if (getobjproperty(who, mod))
        return 0;
else
        setobjproperty(who, mod, 1);
        return 1;
endif

endfunction

function allow_stat_mod(who, stat, duration)
detach();
local mod;

case (stat)
        "str": mod := "#ss";
        "dex" : mod := "#dd";
        "int" : mod := "#ii";
        "ar"  : mod := "#aa";
endcase

while(1)

        sleep(duration);
        if (who)
                eraseobjproperty(who, mod);
                return;
        endif
endwhile

endfunction