// 23/4/00 original scripts by Mad & Rac, adjusted considerably by Rax

Function DoMoons(wstuff)

local FPhase := wstuff[1];
local TPhase := wstuff[2];
local season := wstuff[8];

FPhase := FPhase + 1;
if (FPhase = 8)
	FPhase := 0;
	TPhase := Tphase +1;
endif
wstuff[1] := FPhase;
if (TPhase = 8)
	TPhase := 0;
	season := season + 1;
	if (season = 4)
		season := 0;
	endif
	wstuff[2] := TPhase;
	wstuff[8] := season;
	case(season)
		0: broadcast("The warmth of summer envelopes the land");
		1: broadcast("Leaves begin to fall as Autumn appears");
		2: broadcast("Winters cold hand touches the land");
		3: broadcast("Winters touch thaws as Spring arrives");
	endcase

SetGlobalProperty("Weather", wstuff);

endif

endfunction

function ChangeMoonLightSet(wstuff)

local TLight := FindLight(wstuff[2]);
local FLight := FindLight(wstuff[1]);
local mlight := (TLight + Flight);
local moonlight := (mlight / 2);
wstuff[5] := (27 - moonlight);

endfunction

function DuskTransition(wstuff)

local dlight := wstuff[4];
local mlight := wstuff[5];
local inter_level_time := (Cint(transtime / (mlight - dlight)));
local lightamt := dlight;

while( lightamt <= mlight )
	wstuff := GetGlobalProperty("Weather");
	lightamt := lightamt + 1;
	SetRegionLightLevel("Background", lightamt);
	wstuff[6] := lightamt;
	SetGlobalProperty("Weather", wstuff);
        sleep(inter_level_time);
endwhile

endfunction

function DawnTransition(wstuff)

local dlight := wstuff[4];
local mlight := wstuff[5];
local inter_level_time := (Cint(transtime / (mlight - dlight)));
local lightamt := mlight;

while( lightamt >= dlight )
	wstuff := GetGlobalProperty("Weather");
	lightamt := lightamt - 1;
	SetRegionLightLevel("Background", lightamt);
	wstuff[6] := lightamt;
	SetGlobalProperty("Weather", wstuff);
        sleep(inter_level_time);
endwhile

endfunction

function FindLight(phase)

case(phase)
	0: return 0;
	1: return 1;
	2: return 2;
	3: return 3;
	4: return 4;
	5: return 3;
	6: return 2;
	7: return 1;
endcase

endfunction
