/* This function perform the recruit process, that's pretty simple, after targeting we check for target validity, is not on a guild and are not on the queue. Then append a small structure with serial and name to guild_struct */

function Recruit()

local targ := Target(who, TGTOPT_CHECK_LOS+TGTOPT_NEUTRAL);
if(!targ.acctname)
	SendSysMessage(who, "You can only target players");

	return;
endif

if(GetObjProperty(targ, "guild_id"))
	SendSysMessage(who, "You can only recruit people that are not in any guild");
	return;
endif

if(len(guild_struct[9]) >= MAX_GUILD_RECRUIT_QUEUE)
	SendSysMessage(who, "Too many recruited players");
	return;
endif
local i;
for(i:=1;i<=len(guild_struct[9]);i:=i+1)
	if(guild_struct[9][i][1] = targ.serial) //recruited
		SendSysMessage(who, "Player already recruited");
		return;
	endif
endfor

set_critical(1);
var temp := guild_struct[9];
temp.append( {targ.serial,GetTrueName(targ.name)} );
guild_struct[9] := temp;
SaveGuild();
set_critical(0);
SendSysMessage(who, targ.name + " Have been recruited");
SendSysMessage(targ, "You have been recruited to " + guild_struct[2]);
endfunction


/*This one change the fealty setting and if no guildmaster is set do the math*/
function SetFealty()

local member := BuildDynaGumpWithChoose(2,1,1);
if(!member)
	return;
endif

set_critical(1);
LoadDefaultGuild();
LoadDefaultPlayer();
guild_struct[6][player_id][4] := member;

if(guild_struct[1][1] =0) //needs a new guildmaster
	local i,result:={};
	for(i:=1;i<=len(guild_struct[6]);i:=i+1)
		if(!result[guild_struct[6][i][4]])
			result[guild_struct[6][i][4]] :=1;
		else
			result[guild_struct[6][i][4]] := result[guild_struct[6][i][4]]+1;
		endif
	endfor

	local winner:=0,votes:=0,tie:=0;
	for(i:=1;i<=result.size();i:=i+1)
		if(result[i] > votes)
			votes := result[i];
			winner :=i;
			tie :=0;
		elseif(result[i] = votes)
			tie :=1;
		endif
	endfor

	if(tie)
		guild_struct[1][1] :=0;
		guild_struct[1][2] := "None";
	else
		guild_struct[1][1] := guild_struct[6][winner][1];
		guild_struct[1][2] := guild_struct[6][winner][2];
		var update := GetObjProperty(item,"update");
		if(!update)
			update := {};
		endif
		update.append(guild_struct[6][winner][1]);
		SetObjProperty(item,"update",update);
	endif
endif

SaveGuild();
set_critical(0);

endfunction


/* this adds and removes guild abrev and title from ppl names */
//0: toggle
//1: refresh
//2: remove
//3: add

function Toggle_Showing(me,me_id,type)



if(!len(guild_struct[3]))
	SendSysMessage(me,"No abbreviation defined");
	return;
endif

case(type)
	0:if(GetObjProperty(me,"abv")) //disabling
			me.title_guild := "";
		EraseObjProperty(me,"abv");
	  else //enabling
		me.title_guild:=me.title_guild+" ["+guild_struct[3]+"]";
		if(len(guild_struct[6][me_id][3]))
			me.title_guild:=me.title_guild+", "+guild_struct[6][me_id][3];
		endif
		case(guild_struct[4])
			0: break;//common guild
			1: //order
				me.title_guild:= me.title_guild+" [Order]";
			2://chaos
				me.title_guild:= me.title_guild+" [Chaos]";
		endcase
		
		SetObjProperty(me,"abv",1);
	  endif

	1:if(GetObjProperty(me,"abv"))
			me.title_guild := "";
		me.title_guild:=me.title_guild+" ["+guild_struct[3]+"]";
		if(len(guild_struct[6][me_id][3]))
			me.title_guild:=me.title_guild+", "+guild_struct[6][me_id][3];
		endif
		case(guild_struct[4])
			0: break;//common guild
			1: //order
				me.title_guild:= me.title_guild+" [Order]";
			2://chaos
				me.title_guild:= me.title_guild+" [Chaos]";
		endcase
	   endif

	2:me.title_guild := "";
	  EraseObjProperty(me,"abv");

	3: me.title_guild := "";
	  me.title_guild:=me.title_guild+" ["+guild_struct[3]+"]";
	  if(len(guild_struct[6][me_id][3]))
		me.title_guild:=me.title_guild+", "+guild_struct[6][me_id][3];
	  endif
	  case(guild_struct[4])
		0: break;//common guild
		1: //order
			me.title_guild:= me.title_guild+" [Order]";
		2://chaos
			me.title_guild:= me.title_guild+" [Chaos]";
	  endcase
	  SetObjProperty(me,"abv",1);
endcase
endfunction


/*remove a player from the guild and do all upkeep stuff needed...*/
function Resing()

local YesNoData := {
	"Do you really wish to resign?",
	"No, I am faithful.",
	"To Hell with the guild!",
};

local query := SendDialogGump(who,ChooseLayout,YesNoData);

case(query[0])
	0:
	1:return; //canseled or closed the menu
endcase

set_critical(1);

if (who.cmdlevel<>0)
if(len(guild_struct[6])=1)
	DestroyGuild();
endif
	set_critical(0);
	return;
endif

LoadDefaultGuild();
player_id := FindMemberID(who.serial);
if(!player_id)
	SendSysMessage(who, "Shit Happening");
	set_critical(0);
	return;
endif
LoadDefaultPlayer();

Toggle_Showing(who,player_id,2);

LoadDefaultPlayer();
EraseObjProperty(who,"guild_id");
if(len(guild_struct[6]) =1)
	DestroyGuild();
	set_critical(0);
	return;
endif

if(who.serial = guild_struct[1][1]) //is the guildmaster
	local i,result:={};
	for(i:=1;i<=len(guild_struct[6]);i:=i+1)
		if(guild_struct[6][i][4] = player_id) //fealty to the out gm
			guild_struct[6][i][4] := i; //back to default
		endif

		if(!result[guild_struct[6][i][4]])
			result[guild_struct[6][i][4]] :=1;
		else
			result[guild_struct[6][i][4]] := result[guild_struct[6][i][4]]+1;
		endif
	endfor

	local winner:=0,votes:=0,tie:=0;
	for(i:=1;i<=result.size();i:=i+1)
		if(result[i] > votes)
			votes := result[i];
			winner :=i;
			tie :=0;
		elseif(result[i] = votes)
			tie :=1;
		endif
	endfor

	if(tie)
		guild_struct[1][1] :=0;
		guild_struct[1][2] := "None";
	else
		guild_struct[1][1] := guild_struct[6][winner][1];
		guild_struct[1][2] := guild_struct[6][winner][2];
		var update := GetObjProperty(item,"update");
		if(!update)
			update := {};
		endif
		update.append(guild_struct[6][winner][1]);
		SetObjProperty(item,"update",update);
	endif
	guild_struct[1][3] := "Guild Master";
endif

var temp := guild_struct[6];
temp.erase(player_id);
guild_struct[6]:= temp;
SaveGuild();
SortGuildID(player_id);
set_critical(0);

endfunction




//////gm menu functions

/*this one is intented to change the guild name, the hard part is the validity check*/

function SetGuildName()

//check timeframe
local changetime := GetObjProperty(item,"namechangetime");
if(!changetime)
	changetime := ReadGameClock()+(7*24*3600);
	SetObjProperty(item,"namechangetime",changetime);
	SendSysMessage(who, "You can only change the guild name once a week");
	return;
endif

if(ReadGameClock() < changetime)
	SendSysMessage(who, "You can only change the guild name once a week");
	return;
endif

//query for the new name
local tname := SendTextEntryGump(who,"What's the new guild name",0,1,MAX_GUILD_NAME_SIZE,"Write the name");

if(!tname)
	return;
endif

//double check the len
if(len(tname) > MAX_GUILD_NAME_SIZE)
	SendSysMessage(who,"Guild name can't have more than "
			+MAX_GUILD_NAME_SIZE+" caracters");
	return;
endif

//check if is unique
if(CheckGuildList(tname,2)) //have clone
	SendSysMessage(who, "This name is already in use");
	return;
endif

if(!NameValidation(tname))
	SendSysMessage(who, "You are using invalid characters");
	return;
endif

//set everything
set_critical(1);
LoadGuilds();
LoadDefaultGuild();
guild_id := FindGuildID(item.serial);
guild_list[guild_id][2] := tname;
guild_struct[2] :=  tname;
SaveGuildList();
SaveGuild();
changetime := ReadGameClock() +(7*24*3600);
SetObjProperty(item,"namechangetime",changetime);
set_critical(0);
item.name := "GuildStone for " + tname;

endfunction


/*Change the Guild Abreviation*/
function SetAbrevName()

//check timeframe
local changetime := GetObjProperty(item,"abrevchangetime");
if(!changetime)
	changetime := 1;
endif

if(ReadGameClock() < changetime)
	SendSysMessage(who, "You can only change the guild abbreviation once a week");
	return;
endif

//query for the new name
local tname := SendTextEntryGump(who,"What's the new guild's abbreviation",0,1,MAX_GUILD_ABREV_SIZE,"Write the abrev");
if(!tname)
	return;
endif

//double check the len
if(len(tname) > MAX_GUILD_ABREV_SIZE)
	SendSysMessage(who,"Abbreviation can't have more than "
			+MAX_GUILD_ABREV_SIZE+" caracters");
	return;
endif

//check if is unique
if(CheckGuildList(tname,3)) //have clone
	SendSysMessage(who, "This abbreviation is already in use");
	return;
endif

if(!NameValidation(tname))
	SendSysMessage(who, "You are using invalid characters");
	return;
endif

//set everything
set_critical(1);
LoadGuilds();
LoadDefaultGuild();
guild_id := FindGuildID(item.serial);
guild_list[guild_id][3] := tname;
guild_struct[3] :=  tname;
SaveGuildList();
SaveGuild();
changetime := ReadGameClock() +(7*24*3600);
SetObjProperty(item,"abrevchangetime",changetime);
set_critical(0);

local update:= {},tmp;
tmp := guild_struct[6];
foreach dd in tmp
	update.append(dd[1]);
endforeach
SetObjProperty(item,"update", update);

endfunction



/*This one is intented to remove a guild member, due core lack of support the member must be  online, sort the fealties after*/
function Dismiss()
	
local dude := BuildDynaGumpWithChoose(7,1,1);
if(!dude)
	return;
endif

if(dude = player_id)//no need for look-up only the gm can do it :-)
	SendSysMessage(who,"You cannot dismiss yourself");
	return;
endif

local kicked := SystemFindObjectBySerial(guild_struct[6][dude][1]);
if(!kicked)
	SendSysMessage(who, "The dismissed member must be online\nOffline Dissmis coming soon");
	return;
endif

local YesNoData := {
	"Do you want to dismiss ",
	"No, the member stays!",//1
	"Yes, out with this fellow!",//2
};

data[1] := data[1]+ guild_struct[6][dude][2]+"?";
local query := SendDialogGump(who,ChooseLayout,YesNoData);

case(query[0])
	2: break; //accepted
	1: //canseled
	0: return; //canseled
endcase


if(GetObjProperty(kicked,"abv")) //have title on...
	kicked.name := guild_struct[6][dude][2];
	EraseObjProperty(kicked,"abv");
endif
EraseObjProperty(kicked,"guild_id");
set_critical(1);
LoadDefaultGuild();

var temp := guild_struct[6];
temp.erase(dude);
guild_struct[6] := temp;
SaveGuild();
SortGuildID(dude);
set_critical(0);

endfunction


/*Handles basic warfare, there's not utility right now*/
function Declarewar()
guild_id := FindGuildID(item.serial);
local oposing_guild := BuildDynaGumpWithChoose(4,3,1);
if(!oposing_guild)
	return;
endif

if(oposing_guild = guild_id)
	SendSysMessage(who, "You cannot declare war against your guild");
	return;
endif
local i, listpoz :=0,enemypoz :=0;
local enemy_guild := {};
local opgs := guild_list[oposing_guild][1];
set_critical(1);

LoadDefaultGuild();
enemy_guild:= LoadGuildBySerial(opgs);

//find if thes guilds have some tread between'em
for(i:=1;i<=len(guild_struct[7]);i:=i+1)
	if(guild_struct[7][i][1] = opgs)
		listpoz := i;
		break;
	endif
endfor
var temp;
if(!listpoz) //new declare
	temp := guild_struct[7];
	temp.append({opgs,1,0,0});
	guild_struct[7] := temp;
	temp := enemy_guild[7];
	temp.append({item.serial,0,1,0});
	enemy_guild[7] := temp;

else //already declared by one side
	for(i:=1;i<=len(enemy_guild[7]);i:=i+1)
		if(enemy_guild[7][i][1] = item.serial)
			enemypoz := i;
			break;
		endif
	endfor

	if(guild_struct[7][listpoz][2])
		SendSysMessage(who,"War already declared");
		return;
	endif

	if(guild_struct[7][listpoz][3]) //they have declared
		guild_struct[7][listpoz][4] :=1;
		enemy_guild[7][enemypoz][4] :=1;
		SendSysMessage(who,"War now is declared");
	else
		SendSysMessage(who,"We have declared war on'em");
	endif

	guild_struct[7][listpoz][2] :=1;
	enemy_guild[7][enemypoz][3] :=1;

endif

SaveGuild();
SetGlobalProperty("guild"+opgs,enemy_guild);

set_critical(0);

endfunction


/* Intented to call peace uppon conflicts*/
function DeclarePeace()
var temp;
local oposing_guild := BuildDynaGumpWithChoose(5,4,1);
if(!oposing_guild)
	return;
endif

local i, listpoz :=0,enemypoz :=0;
local enemy_guild:= {};
local opgs := guild_list[oposing_guild][1];

set_critical(1);
LoadDefaultGuild();
enemy_guild:= LoadGuildBySerial(opgs);

//find if thes guilds have some tread between'em
for(i:=1;i<=len(guild_struct[7]);i:=i+1)
	if(guild_struct[7][i][1] = opgs)
		listpoz := i;
		break;
	endif
endfor


if(!listpoz) //bug
	SendSysMessage(who, "Unknown guild");
	set_critical(0);
	return;
endif

//find enemypoz
for(i:=1;i<=len(enemy_guild[7]);i:=i+1)
	if(enemy_guild[7][i][1] = item.serial)
		enemypoz := i;
		break;
	endif
endfor

if(!enemypoz)//bug check
	SendSysMessage(who,"Stone problem, canseling war");
	temp := guild_struct[7];
	temp.erase(listpoz);
	guild_struct[7] := temp;
	SaveGuild();
	set_critical(0);
	return;
endif

//do peace stuff
if(guild_struct[7][listpoz][4])//war goin on
	if(!guild_struct[7][listpoz][3]) //they made peace too, soo wipe list
		temp := guild_struct[7];
		temp.erase(listpoz);
		guild_struct[7] := temp;

		temp :=enemy_guild[7];
		temp.erase(enemypoz);
		enemy_guild[7] := temp;
		SendSysMessage(who,"Peace made, war off");
	else
		guild_struct[7][listpoz][2] :=0;
		enemy_guild[7][enemypoz][3] :=0;
	endif

else//war off soo other side have not declared, since right now current guild have declared
	temp := guild_struct[7];
	temp.erase(listpoz);
	guild_struct[7] := temp;
	temp := enemy_guild[7];
	temp.erase(enemypoz);
	enemy_guild[7] := temp;
endif

SaveGuild();
SetGlobalProperty("guild"+opgs,enemy_guild);
set_critical(0);

endfunction


/*Self explanable, first checks for validity, them append stuff to guild_struct*/

function  AcceptCandidate()

local num := BuildDynaGumpWithChoose(6,2,1);
if(!num)
	return;
endif
local newbie := SystemFindObjectBySerial(guild_struct[9][num][1]);

if(!newbie)
	SendSysMessage(who, "The player must be online to be accepted");
	return;
endif


set_critical(1);
LoadDefaultGuild();
var temp;
if(GetObjProperty(newbie," guild_id"))
	SendSysMessage(who, "Recruit already in a guild");
	temp :=	guild_struct[9];
	temp.erase(num);
	guild_struct[9] := temp;
	SaveGuild();
	set_critical(0);
	return;
endif

local newbie_id := len(guild_struct[6])+1;
temp := guild_struct[6];
temp.append({newbie.serial, GetTrueName(newbie.name),"",newbie_id});
guild_struct[6] := temp;
temp :=guild_struct[9];
temp.erase(num);
guild_struct[9] := temp;


SetObjProperty(newbie,"guild_id", item.serial);
SaveGuild();
set_critical(0);

endfunction


/*
this function throw out of someone from the candidate list
*/
function RefuseCandidate()

local num := BuildDynaGumpWithChoose(9,2,1);
if(!num)
	return;
endif

set_critical(1);
LoadDefaultGuild();
var temp := guild_struct[9];
temp.erase(num);
guild_struct[9] := temp;
SaveGuild();
set_critical(0);

endfunction


/*GM have special title*/
function SetGMTitle()

local tname := SendTextEntryGump(who,"What's the new guildmaster title name",0,1,MAX_GM_TITLE_SIZE,"Write the name");
if(!tname)
	return;
endif

if(len(tname) > MAX_GM_TITLE_SIZE)
	SendSysMessage(who, "Your title can't have more than "+MAX_GM_TITLE_SIZE+" caracters");
	return;
endif
if(!NameValidation(tname))
	SendSysMessage(who, "You are using invalid characters");
	return;
endif

set_critical(1);
LoadDefaultGuild();

guild_struct[6][player_id][3] := tname;
guild_struct[1][3] := tname;
SaveGuild();
LoadDefaultPlayer();
set_critical(0);
Toggle_Showing(who,player_id,1);

endfunction


/*Grant a title to another member, again both must be online*/
function GrantTitle()

local dude := BuildDynaGumpWithChoose(8,1,1);
if(!dude)
	return;
endif

if(dude = player_id) //guildmaster can't have title
	SendSysMessage(who, "You cannot give the leader a title");
	return;
endif

local tname := SendTextEntryGump(who,"What's the new member title?",0,1,MAX_GUILD_TITLE_SIZE,"Write the title");
if(!tname)
	return;
endif

if(!NameValidation(tname))
	SendSysMessage(who, "You are using invalid characters");
	return;
endif

set_critical(1);
LoadDefaultGuild();
guild_struct[6][dude][3] := tname;
SaveGuild();
set_critical(0);

local granted := SystemFindObjectBySerial(guild_struct[6][dude][1]);
if(granted)
	Toggle_Showing(granted,dude,1);
else
	var update := GetObjProperty(item,"update");
	if(!update)
		update := {};
	endif
	update.append(guild_struct[6][dude][1]);
	SetObjProperty(item,"update",update);
endif

endfunction




function Teleport()

local changetime := GetObjProperty(item,"telechangetime");
if(ReadGameClock() < changetime)
	changetime := ReadGameClock()+(7*24*3600);
	SetObjProperty(item,"telechangetime", changetime);
else
	SendSysMessage(who, "You can only teleport the stone once a week");
	return;
endif

item.movable :=0;
item.newbie :=1;
MoveItemToContainer(item, who.backpack);
item.graphic := UOBJ_PACKED_GUIDSTONE_GRAPHIC;
item.movable :=0;
item.usescript :="packedstone";

endfunction











