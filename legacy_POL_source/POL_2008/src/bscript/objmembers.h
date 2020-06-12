// History
//   2005/01/24 Shinigami: added ObjMember character.spyonclient2 to get data from packet 0xd9 (Spy on Client 2)
//   2005/03/09 Shinigami: added Prop Character::Delay_Mod [ms] for WeaponDelay
//   2005/04/04 Shinigami: added Prop Character::CreatedAt [PolClock]
//   2005/08/29 Shinigami: character.spyonclient2 renamed to character.clientinfo
//   2005/10/02 Shinigami: added Prop Script.attached_to and Script.controller
//   2005/12/09 MuadDib: added member uclang for storing Unicode Language sent during logon.
//   2006/05/16 Shinigami: added Prop Character.Race [RACE_* Constants] to support Elfs
//   2007/07/09 Shinigami: added Prop Character.isUOKR [bool] - UO:KR client used?

#ifndef OBJMEMBERS_H
#define OBJMEMBERS_H

enum MemberID
{
    MBR_X, //uobject        //0
    MBR_Y,                  //1
    MBR_Z,
    MBR_NAME,
    MBR_OBJTYPE,
    MBR_GRAPHIC,        //5
    MBR_SERIAL,
    MBR_COLOR,
    MBR_HEIGHT,
    MBR_FACING,
    MBR_DIRTY,        //10
    MBR_WEIGHT,
    MBR_MULTI,
    MBR_AMOUNT, //item
    MBR_LAYER,
    MBR_CONTAINER,        //15
    MBR_USESCRIPT,
    MBR_EQUIPSCRIPT,
    MBR_UNEQUIPSCRIPT,
    MBR_DESC,
    MBR_MOVABLE,        //20
    MBR_INVISIBLE,
    MBR_DECAYAT,
    MBR_SELLPRICE,
    MBR_BUYPRICE,
    MBR_NEWBIE,        //25
    MBR_ITEM_COUNT,
    MBR_WARMODE, //character
    MBR_GENDER,
    MBR_TRUEOBJTYPE,
    MBR_TRUECOLOR,        //30
    MBR_AR_MOD,
    MBR_HIDDEN,
    MBR_CONCEALED,
    MBR_FROZEN,
    MBR_PARALYZED,        //35
    MBR_POISONED,
    MBR_STEALTHSTEPS,
    MBR_SQUELCHED,
    MBR_DEAD,
    MBR_AR,        //40
    MBR_BACKPACK,
    MBR_WEAPON,
	MBR_SHIELD,
	MBR_ACCTNAME,
    MBR_ACCT,
    MBR_CMDLEVEL,        //45
    MBR_CMDLEVELSTR,
    MBR_CRIMINAL,
    MBR_IP,
    MBR_GOLD,
    MBR_TITLE_PREFIX,        //50
    MBR_TITLE_SUFFIX,
    MBR_TITLE_GUILD,
    MBR_TITLE_RACE,
    MBR_GUILDID,
    MBR_GUILD,        //55
    MBR_MURDERER,
    MBR_ATTACHED,
    MBR_CLIENTVERSION,
    MBR_REPORTABLES,
    MBR_SCRIPT,  //npc        //60
    MBR_NPCTEMPLATE,
    MBR_MASTER,
    MBR_PROCESS,
    MBR_EVENTMASK,
    MBR_SPEECH_COLOR,        //65
    MBR_SPEECH_FONT,
    MBR_USE_ADJUSTMENTS,
    MBR_RUN_SPEED,
    MBR_LOCKED, //lockable
    MBR_CORPSETYPE, //corpse        //70
    MBR_TILLERMAN, //boat
    MBR_PORTPLANK,
    MBR_STARBOARDPLANK,
    MBR_HOLD,
	MBR_HAS_OFFLINE_MOBILES,		//75
    MBR_COMPONENTS, //house
    MBR_ITEMS, //multi
    MBR_MOBILES,
    MBR_XEAST, //map
    MBR_XWEST,        //80
    MBR_YNORTH,
    MBR_YSOUTH,
    MBR_GUMPWIDTH,
    MBR_GUMPHEIGHT,
    MBR_ISOPEN, //door        //85
    MBR_QUALITY, //equipment
    MBR_HP,
    MBR_MAXHP_MOD,
    MBR_MAXHP,
    MBR_DMG_MOD, //weapon        //90
    MBR_ATTRIBUTE,
	MBR_INTRINSIC,
    MBR_HITSCRIPT,
    MBR_AR_BASE,
    MBR_ONHIT_SCRIPT,
    MBR_ENABLED, //account        //95
    MBR_BANNED,
    MBR_PASSWORDHASH,
    MBR_USERNAMEPASSWORDHASH,
    MBR_MEMBERS, //guild
    MBR_ALLYGUILDS,        //100
    MBR_ENEMYGUILDS,
    MBR_PID, //script
    MBR_STATE,
    MBR_INSTR_CYCLES,
    MBR_SLEEP_CYCLES,        //105
    MBR_CONSEC_CYCLES,
    MBR_PC,
    MBR_CALL_DEPTH,
    MBR_NUM_GLOBALS,
    MBR_VAR_SIZE,        //110
	MBR_REALM,
	MBR_UO_EXPANSION,
	MBR_CUSTOM,
    MBR_GLOBALS,
	MBR_FOOTPRINT,		//115
    MBR_CLIENTINFO,
    MBR_DELAY_MOD,
    MBR_CREATEDAT,
	MBR_OPPONENT,
	MBR_CONNECTED,		//120
	MBR_ATTACHED_TO,
	MBR_CONTROLLER,
	MBR_OWNERSERIAL,
	MBR_DEFAULTCMDLEVEL,
	MBR_UCLANG,			//125
	MBR_RACE,
	MBR_TRADING_WITH,
	MBR_TRADE_CONTAINER,
	MBR_ALIGNMENT,
	MBR_ISUOKR,          //130
	MBR_CURSOR,
	MBR_GUMP,
	MBR_PROMPT
};


typedef struct {
    MemberID id;
    char code[30];
    bool read_only;
} ObjMember;

extern ObjMember object_members[];
extern int n_objmembers;

ObjMember* getKnownObjMember(const char* token);
ObjMember* getObjMember(int id);

#endif
