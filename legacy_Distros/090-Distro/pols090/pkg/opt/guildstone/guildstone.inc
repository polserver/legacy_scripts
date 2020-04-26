const MAX_GUILD_NAME_SIZE := 30;
const MAX_LINE_LEN := 42;
const MAX_GM_TITLE_SIZE := 30;
const MAX_GUILD_ABREV_SIZE := 4;
const GUILD_MENU_SIZE := 10;
const MAX_GUILD_RECRUIT_QUEUE := 20;
const MAX_GUILD_TITLE_SIZE := 20;
const UOBJ_GUILDSTONE := 0xa390; //don't know :-(
const UOBJ_GUILDSTONE_GRAPHIC := 0x0EDD;
const UOBJ_PACKED_GUIDSTONE_GRAPHIC := 0x0EDD; //nether

/*
guild_list
[1]guild_serial
[2]guild_name //
[3]guild_abrev //these are for fast-find 


guild'guildserial'
[1] guild_master
[2] name
[3] abrev
[4] type
[5] charter
[6] member_list
[7] war_list
[8] aliance_list
[9] recruit_list

[1][1] gm_serial
[1][2] gm_name
[1][3] gm_title

[6][i][1] member_serial
[6][i][2] member_name
[6][i][3] member_title
[6][i][4] member_fealty_id

[7][i][1] enemy_serial
[7][i][2] us_declared
[7][i][3] they_declared
[7][i][4] status

[8][i][1] alied_serial
[8][i][2] us_asked
[8][i][3] they_asked
[8][i][4] status

[9][i][1] recruit_serial
[9][i][2] recruit_name

*/


global guild_list := {};
global guild_struct := {};
global player_struct := {};
global player_id;
global guild_id;


function LoadGuilds()
guild_list := GetGlobalProperty("guild_list");
if(!guild_list)
	guild_list := {};
endif

//guild_list.append({"oo","oo","oo"});
endfunction

function SaveGuildList()
return SetGlobalProperty("guild_list", guild_list);
endfunction

function LoadGuildByID(id)
return GetGlobalProperty("guild"+guild_list[id][1]);
endfunction


function LoadGuildBySerial(serial)
return GetGlobalProperty("guild"+serial);
endfunction

function SaveGuild()
SetGlobalProperty("guild"+item.serial,guild_struct);
endfunction

function LoadDefaultGuild()
guild_struct :=GetGlobalProperty("guild"+item.serial);
if(!guild_struct)
	guild_struct := {};
endif
endfunction

function LoadDefaultPlayer()
player_struct := guild_struct[6][player_id];
if(!player_struct)
	player_struct :={};
endif
endfunction

function FindGuildID(serial)
local i;
for(i:=1;i<=len(guild_list[1]);i:=i+1);
	if(guild_list[i][1] = serial)
		return i;
	endif
endfor
return 0;
endfunction

function FindMemberID(serial)
local i;
for(i:=1;i<=len(guild_struct[6]);i:=i+1);
	if(guild_struct[6][i][1] = serial)
		return i;
	endif
endfor
return 0;
endfunction


function BuildDynaGumpWithChoose(type,listype,button)
basedata := ldata;
baselayout := llayout;
if(button)
 	baselayout.append("button 400 405 5200 5201 1 0 1000");
endif

case(type)
	1: basedata[2] := "Set Fealty";
	2: basedata[2] := guild_struct[2] + "  Roster";
	3: basedata[2] := guild_struct[2] + "  Recruit List";
	4: basedata[2] := "Declare war";
	5: basedata[2] := "Call peace";
	6: basedata[2] := "Accept Candidate";
	7: basedata[2] := "Dismiss a member";
	8: basedata[2] := "Grant title to a member";
	9: basedata[2] := "Refuse Candidate";
	10:basedata[2] := "Guilds that we have declared war";
	11:basedata[2] := "Guilds tha have declares war on us";
endcase



local maxpos;
case(listype)
	1:maxpos := len(guild_struct[6]); //members
	2:maxpos := len(guild_struct[9]); //recruits
	3:maxpos := len(guild_list); //guilds
	4:maxpos := len(guild_struct[7]); //warlist
endcase

local i;

local poscount :=1;
local pagecount :=1;
local lastpage := maxpos/GUILD_MENU_SIZE;
if( (lastpage*GUILD_MENU_SIZE) < maxpos )
	lastpage :=  lastpage+1;
endif
local ypos;

for(i:=1;i<=maxpos;i:=i+1)
	if(poscount =1) //needs page header
		baselayout.append("page "+pagecount);
		if(pagecount >1) //needs upper button
			baselayout.append("button 490 60 5208 5210 0 "+(pagecount-1));
		endif
		if(pagecount <lastpage) //needs lower button
			baselayout.append("button 490 410 5209 5210 0 "+(pagecount+1));
		endif
		pagecount := pagecount+1;
	endif

	//time to add the text entry, the data and, if asked, the button
	//first the text
	ypos := (poscount*20) + 70;
	if(type != 5) //everything
		baselayout.append("text 165 " + ypos + " 0 " + (i+1)); //access i+1 data field
		if(button) //needs a radio button
			baselayout.append("radio 140 "+ypos+" 5002 5003 0 "+i);
		endif
		case(listype)
			1:if(len(guild_struct[6][i][3]));
				basedata.append(guild_struct[6][i][2] +", " + guild_struct[6][i][3]);
			  else
				basedata.append(guild_struct[6][i][2]);
			  endif
			2:basedata.append(guild_struct[9][i][2]);
			3:if(type = 4)//war list
				if(guild_struct[7][i][2]) //already declared
					if(poscount > 1)
						poscount := poscount-1;
					endif
					basedata.append("");
				else
					basedata.append(guild_list[i][2]);
				endif
			  else
				basedata.append(guild_list[i][2]);
			  endif
			4:case(type)
				5: //fall intencionally, since the lists are the same
				10:if(guild_struct[7][i][2]) //us declared
					basedata.append(guild_list[FindGuildID(guild_struct[7][i][1])][2]);
				   else
					if(poscount > 1)
						poscount := poscount-1;
					endif
					basedata.append("");
				   endif
				11:if(guild_struct[7][i][3]) //theys declared
					basedata.append(guild_list[FindGuildID(guild_struct[7][i][1])][2]);
				   else
					if(poscount > 1)
						poscount := poscount-1;
					endif
					basedata.append("");
				   endif
			  endcase

		endcase
	else //peace menu
		if(guild_struct[7][i][2]) //can't ask 4 peace if it's off...
			baselayout.append("text 165 " + ypos + " 0 " + (i+1)); //access i+1 data field
			if(button) //needs a radio button
				baselayout.append("radio 140 "+ypos+" 5002 5003 0 "+i);
			endif	
		else
			if(poscount > 1)
				poscount := poscount-1;
			endif
		endif
		basedata.append(guild_list[FindGuildID(guild_struct[7][i][1])][2]);
	endif

	if(poscount >= GUILD_MENU_SIZE)
		poscount := 1;
	else
		poscount := poscount +1;
	endif
endfor


local box3 := SendDialogGump(who, baselayout, basedata);

if(box3[0] =2000) //done button
	foreach k in (box3.keys)
		if(k>0 and k<1000)
			return k;
		endif
	endforeach
endif

return 0;
endfunction



function SortGuildID(id)
local i;
set_critical(1);
LoadDefaultGuild();
for(i:=1;i<=len(guild_struct[6]);i:=i+1)
	if(guild_struct[6][i][4] > id)
		guild_struct[6][i][4] := guild_struct[6][i][4]-1;
	elseif(guild_struct[6][i][4] =id)
		guild_struct[6][i][4] :=i;
	endif
endfor
SaveGuild();
set_critical(0);
endfunction



function CheckGuildList(str,index)
LoadGuilds();

local i;
for(i:=1;i<=guild_list.size();i:=i+1)
	if(lower(guild_list[i][index]) = lower(str))
		return 1;
	endif
endfor
return 0;

endfunction

function NameValidation(theString)
local validstr := {"0","1","2","3","4","5","6","7","8","9","a","b","c","d","e",
		   "f","g","h","i","j","k","l","m","n","o","p","k","r","s","t","u","v","w","x","y","z"," ",};
theString := lower(theString);
local i;
for( i :=1; i<=len(theString);i:=i+1)
	if(!(theString[i] in validstr))
		return 0;
	endif
endfor
return 1;

endfunction


function GetTrueName(name)


if(name["The Murderer"])
	local ret := name[14,(len(name)-13)];
	return ret;
endif

return name;

endfunction





