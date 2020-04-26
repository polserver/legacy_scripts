use os;
use util;
use uo;

include "include/client";
include "include/fields";

Const SOUND_EFFECT_FIREFIELD:= 0x1DE;
Const UOBJ_FIREFIELD_NS := 0x398c;
Const UOBJ_FIREFIELD_EW := 0x3996;
Const SOUND_EFFECT_POISON := 0x247;
Const EFFECT_RED_SPARKLE := 0x374a;





function firefield( caster )

local thetarget;
local x;
local y;
local z;
local i;
local fires := { };
local duration;

local listeners := { };
local sufferers := { };

thetarget := TargetCoordinates(caster);

if ((!thetarget))
        SendSysmessage(caster, "Targetting Cancelled.");
endif

x := thetarget.x;
y := thetarget.y;
z := thetarget.z;
	
if (!CheckLosAt( caster, x, y, z ))
        SendSysmessage(caster, "You can't see that!");
        return;
endif

local dir := get_direction (caster, x, y, z);
	
case dir
	"north" : fires:= CreateNorthSouthField(caster, UOBJ_FIREFIELD_NS, x, y, z, 5);
	"south"  :fires:= CreateNorthSouthField(caster, UOBJ_FIREFIELD_NS, x, y, z, 5);
	"east"  : fires:= CreateEastWestField(caster, UOBJ_FIREFIELD_EW, x, y, z, 5);
        "west"  : fires:= CreateEastWestField(caster, UOBJ_FIREFIELD_EW, x, y, z, 5);
        default:  local firetype := randomint(2) + 1;
        	  case firetype
        	  	1: fires:= CreateNorthSouthField(caster, UOBJ_FIREFIELD_NS, x, y, z, 5);
        	  	2: fires:= CreateEastWestField(caster, UOBJ_FIREFIELD_EW, x, y, z, 5);
        	  endcase
endcase

detach();

foreach fire in fires

        PlaySoundEffect(fire, SOUND_EFFECT_FIREFIELD);
endforeach

	        
local counter := 0;

duration := Cint(GetSkill( caster, SKILLID_MAGERY ) );
for (counter := 1; counter <= duration; counter := counter + 1)



foreach fire in fires
	if (fire)
        PlaySoundEffect(fire, SOUND_EFFECT_FIREFIELD);
    endif
endforeach
sleep(1); 
	
endfor

//now, cleanup
foreach fire in fires
    DestroyItem( fire );
endforeach

endfunction
