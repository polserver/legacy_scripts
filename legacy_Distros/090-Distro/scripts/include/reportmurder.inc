//
// Sends gump for player to report players for the murder
// and adjusts their murder counts
//
// TODO: ping pong counter
//

use uo;

const LONG_COUNT_DECAY_TIME := 144000; //40 hours
const SHORT_COUNT_DECAY_TIME := 28800; //8 hours

var layout := {

"page 0",
"nodispose",
"noclose",
"gumppic 0 0 206",
"gumppic 43 0 201",
"gumppic 460 0 207",

"gumppic 0 43 202",
"gumppic 43 43 200",
"gumppic 170 43 200",
"gumppic 295 43 200",
"gumppic 335 43 200",
"gumppic 43 170 200",
"gumppic 170 170 200",
"gumppic 295 170 200",
"gumppic 335 170 200",
"gumppic 43 220 200",
"gumppic 170 220 200",
"gumppic 295 220 200",
"gumppic 335 220 200",
"gumppic 460 43 203",

"gumppic 0 340 204",
"gumppic 43 340 233",
"gumppic 460 340 205",

"button 380 345 249 248 1 0 0",

"text 50 20 40 0",
"text 50 50 40 1",
"text 50 70 40 2",

//"checkbox 100 100 210 211 0 10",
//"checkbox 100 200 210 211 1 11",
};

var data := {
"Now is the time for retribution!",
"Check zero or more players to report for your murder",
"and press OKAY.",
};

var reportables;

function SendReportGump(who)

	reportables := who.reportables;
	
	FillInArrays();
	
	var ret := SendDialogGump(who,layout,data);
	
	foreach id in (ret.keys)
		if(id >= 10) //it's from a checkbox
			id := id-9;
			var mob := who.reportables[id];
			SendSysMessage( mob.killer, "You have been reported for murder.");
			AdjustMurderCounts(mob.killer);
		endif
	endforeach
	
	foreach reportable in (who.reportables)
		who.RemoveReportable(reportable.serial,reportable.gameclock);
	endforeach
endfunction

function FillInArrays()

	var prev_y := 80;
	var prev_x := 40;
	var buttonret := 10;
	
	layout.append("page 1");
	
	foreach reportable in reportables
		if(buttonret=21)
			prev_x := 300;
			prev_y := 80;
		endif
		layout.append("checkbox " + cstr(prev_x) + " " + cstr(prev_y+20) + " 210 211 0 " + buttonret);
		
		data.append(reportable.killer.name);
		layout.append("text " + cstr(prev_x+20) + " " + cstr(prev_y+20) + " 40 " + cstr(len(data)-1));
		
		prev_y := prev_y+20;
		buttonret := buttonret+1;
	endforeach
endfunction

function AdjustMurderCounts(character)

	var onlinetime := GetObjProperty(character,"onlinetimer");
	
	//inc long term count
	var longcount := GetObjProperty(character,"longmurders");
	if(!longcount)
		longcount := 0;
	endif
	longcount := longcount + 1;
	SetObjProperty(character,"longmurders",longcount);
	if(longcount>4)
		character.SetMurderer(1);
		SendSysmessage(character,"You are now known as a murderous brigand!");
	endif
	
	//reset decaylongcountat
	SetObjProperty(character,"decaylongcountat",onlinetime+LONG_COUNT_DECAY_TIME);
	
	//inc short term count
	var shortcount := GetObjProperty(character,"shortmurders");
	if(!shortcount)
		shortcount := 0;
	endif
	shortcount := shortcount + 1;
	SetObjProperty(character,"shortmurders",shortcount);
	
	//reset decayshortcountat
	SetObjProperty(character,"decayshortcountat",onlinetime+SHORT_COUNT_DECAY_TIME);
	
endfunction
	