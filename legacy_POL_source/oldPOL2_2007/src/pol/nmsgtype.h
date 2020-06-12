// History
//   2005/01/24 Shinigami: added packet 0xd9 (MSGD9_SPY_ON_CLIENT2)
//   2005/03/01 Shinigami: modified MSGBF_SUB18_ENABLE_MAP_DIFFS to use const MAX_NUMER_REALMS
//   2005/04/04 Shinigami: updated MSGB9_CHAT_ENABLE with SE flags
//   2005/08/29 Shinigami: SPY_ON_CLIENT renamed to CLIENT_INFO
//   2005/11/25 MuadDib: added sub 0x24 information to 0xBF packet.
//   2005/12/09 MuadDib: Changed u8 client_lang to char client_lang for support.
//   2006/05/16 Shinigami: updated MSGB9_CHAT_ENABLE with ML flags
//   2006/05/24 Shinigami: added MSGBF_SUB2A_CHARACTER_RACE_CHANGER to support Elfs

#ifndef NMSGTYPE_H
#define NMSGTYPE_H

#include "clib/rawtypes.h"
#include "umsgtype.h"
#include "realms.h"

#ifdef _MSC_VER		/* Visual C++ 4.0 and above */
#pragma pack( push, 1 )
#else
/* Ok, my build of GCC supports this, yay! */
#undef POSTSTRUCT_PACK
#define POSTSTRUCT_PACK
#pragma pack(1)
#endif

/* NOTE for all message types: all u16/u32 variables are BIG-ENDIAN, unless otherwise noted. */
#define MSG53_INACTIVITY_WARNING_ID       0x53
#define MSG66_READ_BOOK_PAGE_ID           0x66
#define MSG6F_SECURE_TRADE_ID             0x6F
#define MSG76_NEW_SUBSERVER_ID            0x76
#define MSG93_OPEN_BOOK_ID                0x93
#define MSGA4_CLIENT_INFO_ID              0xA4
#define MSGA5_OPEN_WEB_BROWSER_ID         0xA5
#define MSGAD_UNICODE_SPEECH_ID           0xAD
#define MSGAE_UNICODE_SPEECH_TO_CLIENT_ID 0xAE
#define MSGB6_REQUEST_TOOLTIP_ID          0xB6
#define MSGB7_DISPLAY_TOOLTIP_ID          0xB7
#define MSGB8_CHARACTER_PROFILE_ID        0xB8
#define MSGB9_CHAT_ENABLE_ID              0xB9
#define MSGOUT_BA_QUEST_ARROW_ID          0xBA
#define MSGBB_ULTIMA_MESSENGER_ID         0xBB
#define MSGOUT_BC_SEASON_INFO_ID          0xBC
#define MSGBD_CLIENT_VERSION_ID           0xBD
#define MSGBF_GENERIC_COMMAND_ID          0xBF
#define MSGC0_GRAPHICAL_EFFECT_ID         0xC0
#define MSGC1_LOCALIZED_MESSAGE_ID        0xC1
#define MSGC2_UNICODE_PROMPT_ID           0xC2
#define MSGC4_SEMIVISIBLE_ID              0xC4
#define MSGC7_3D_PARTICLE_EFFECT_ID       0xC7
#define MSGC8_UPDATE_RANGE_CHANGE_ID      0xC8
#define MSGCC_LOCALIZED_MESSAGE_AFFIX_ID  0xCC
#define MSGD1_LOGOUT_STATUS_ID			  0xD1
#define MSGD6_AOS_OBJECT_PROPERTY_LIST_ID 0xD6
#define MSGD7_AOS_GENERIC_COMMAND_ID      0xD7
#define MSGD8_CUSTOM_HOUSE_DATA_ID        0xD8
#define MSGD9_CLIENT_INFO_ID              0xD9

#define URL_MAX_LEN 200

struct MSG53_INACTIVITY_WARNING
{
    u8 msgtype;
    u8 type_07;

	enum {
		WARN_NO_CHARACTER = 1,
		WARN_CHARACTER_EXISTS = 2,
		WARN_CANT_CONNECT_1 = 3,
		WARN_CANT_CONNECT_2 = 4,
		WARN_CHARACTER_IN_WORLD = 5,
		WARN_LOGIN_PROBLEM = 6,
		WARN_CHARACTER_IDLE = 7,
		WARN_CANT_CONNECT_3 = 8
	};
};

struct MSG66_READ_BOOK_PAGE
{
    u8 msgtype;
    u16 msglen;
    u32 book_serial;
    u16 pages;
    u16 page;
    u16 lines;
    char text[1];
};
assertsize( MSG66_READ_BOOK_PAGE, 14 );

struct MSG66_BOOK_CONTENTS_HDR
{
    u8 msgtype;
    u16 msglen;
    u32 book_serial;
    u16 pages;
};
assertsize( MSG66_BOOK_CONTENTS_HDR, 9 );

struct MSG66_BOOK_CONTENTS_PAGE
{
    u16 page;
    u16 lines;
};
assertsize( MSG66_BOOK_CONTENTS_PAGE, 4 );

struct MSG6F_SECURE_TRADE
{
    u8 msgtype;
    u16 msglen;
    u8 action;
    u32 chr_serial;
    u32 cont1_serial;
    u32 cont2_serial;
    u8 havename;
    char name[ 30 ];
    
    enum { ACTION_INIT   = 0,
           ACTION_CANCEL = 1,
           ACTION_STATUS = 2 };
};
asserteql( sizeof(MSG6F_SECURE_TRADE), 47 );

struct MSG76_NEW_SUBSERVER
{
	u8 msgtype; 
	u16 xloc;
	u16 yloc;
	u16 zloc;
	u8 unk0;
	u16 x1;
	u16 y1;
	u16 x2;
	u16 y2;
};
assertsize( MSG76_NEW_SUBSERVER, 16 );

struct MSG93_OPEN_BOOK
{
    u8 msgtype;
    u32 serial;
    u8 writable;
    u8 unk_1;
    u16 npages;
    char title[ 60 ];
    char author[ 30 ];
};
assertsize( MSG93_OPEN_BOOK, 99 );

struct MSGA4_CLIENT_INFO {
    u8 msgtype;
    u8 unknown[0x94]; 
};
asserteql( sizeof(MSGA4_CLIENT_INFO), 0x95 ); 

struct MSGA5_OPEN_WEB_BROWSER {
    u8 msgtype;
    u16 msglen;
	char address[ URL_MAX_LEN + 1 ];
	u8 null_term;
};

struct MSGAD_UNICODE_SPEECH
{
    u8 msgtype;
    u16 msglen;

    MSGXX_TEXTDEF textdef;

    char lang[4]; // "enu" - US english
    u16 wtext[2]; // wide-character, double-null terminated
};
assertsize( MSGAD_UNICODE_SPEECH, 16 );

struct MSGAE_UNICODE_SPEECH_TO_CLIENT
{
    u8 msgtype;
    u16 msglen;
    u32 source_serial;
    u16 source_graphic;
    
    MSGXX_TEXTDEF textdef;

    char lang[4]; // "enu" - US english
    char speaker_name[ 30 ];
    u16 wtext[(SPEECH_MAX_LEN) + 1]; // wide-character, double-null terminated
};
assertsize( MSGAE_UNICODE_SPEECH_TO_CLIENT, 450 );
//asserteql( sizeof(MSGAE_UNICODE_SPEECH_TO_CLIENT), 450 );

struct MSGB6_REQUEST_TOOLTIP {
    u8 msgtype;
    u32 serial;
    u8 langid;
    char lang[3];
};
assertsize( MSGB6_REQUEST_TOOLTIP, 9 );

struct MSGB7_DISPLAY_TOOLTIP {
    u8 msgtype;
    u16 msglen;
    u32 serial;
    enum { MAX_CHARACTERS=256 };
    char text[MAX_CHARACTERS*2+2]; // null-terminated unicode string
};
asserteql( sizeof(MSGB7_DISPLAY_TOOLTIP), 521 );

struct MSGB8_CHARACTER_PROFILE {
    u8 msgtype;
    u16 msglen;
    u8 mode;
    u32 serial;
    //incomplete, packet changes if request, update, or just sending text
};

struct MSGB9_CHAT_ENABLE {
    u8 msgtype;
    u16 enable;
    /*
    if (MSB not set)
      Bit 0 T2A upgrade, enables chatbutton, 
      Bit 1 enables LBR update.  (of course LBR installation is required)
             (plays MP3 instead of midis, 2D LBR client shows new LBR monsters,…)
    if (MSB set)
      Bit 2 T2A upgrade, enables chatbutton, 
      Bit 3 enables LBR update.  
      Bit 4 enables Age of Shadows update (AoS monsters/map (AOS installation required for that), 
              AOS skills/necro/paladin/fight book stuff  – works for ALL clients 4.0 and above)
      Bit 5 enables use of up to 6 Characters (not only 5)
      Bit 6 enables Samurai Empire update (SE map (SE installation required for that))
	  Bit 7 enables Mondain's Legacy update (Elves (ML/Gold installation required for that))
    */
};
asserteql( sizeof(MSGB9_CHAT_ENABLE), 3 );

struct MSGOUT_BA_QUEST_ARROW {
	u8 msgtype;
	u8 active; enum { ARROW_OFF = 0, ARROW_ON = 1 };
	u16 x_tgt;
	u16 y_tgt;
};
asserteql( sizeof(MSGOUT_BA_QUEST_ARROW), 6);

struct MSGBB_ULTIMA_MESSENGER {
    u8 msgtype;
    u32 serial1;
    u32 serial2;
};
asserteql( sizeof(MSGBB_ULTIMA_MESSENGER), 9 );

//Server->Client (SEASON INFO) len=3
//0000:bc 00 01                                            ...
struct MSGOUT_BC_SEASON_INFO {
    u8 msgtype;
	u8 season; enum { SEASON_SPRING = 0, SEASON_SUMMER, SEASON_FALL, SEASON_WINTER, SEASON_DESOLATION };
	u8 change; enum { NO_CHANGE = 0, IS_CHANGE = 1 };
};
asserteql( sizeof(MSGOUT_BC_SEASON_INFO), 3 );

struct MSGBD_CLIENT_VERSION {
    u8 msgtype;
    u16 msglen;
    char version[1];
};
assertsize( MSGBD_CLIENT_VERSION, 4 );

struct MSGBF_SUB4_CLOSE_GUMP
{
    u32 gumpid;
    u32 buttonid; //response for 0xB1
};

struct MSGBF_SUB5_SCREEN_SIZE
{
    u16 unk1; //always 0
    u16 x;
    u16 y;
    u16 unk2;
};

struct MSGBF_SUB6_PARTY_SYSTEM
{
    u8 subsubcmd;
    u8 data[1]; //fixme: union all the subsubcmd fields
};


struct MSGBF_SUB10_OBJECT_CACHE
{
    u32 serial;
    u32 listid;
};

//TextID is broken into two decimal parts:
//stringID / 1000: intloc fileID
//stringID % 1000: text index
//So, say you want the 123rd text entry of intloc06, the stringID would be 6123
struct MSGBF_SUB14_DISPLAY_POPUP_MENU_ENTRY
{
    u16 entry_tag;
    u16 text_id;
    u16 flags;
    //if flags & 0x20:
    u16 color;// rgb 1555 color (ex, 0 = transparent, 0x8000 = solid black, 0x1F = blue, 0x3E0 = green, 0x7C00 = red)

    enum
    {
        POPUP_MENU_LOCKED = 0x1,
        POPUP_MENU_ARROW = 0x2,
        POPUP_MENU_COLOR = 0x20
    };
};

struct MSGBF_SUB14_DISPLAY_POPUP_MENU
{
    u16 unk; //always 00 01
    u32 serial;
    u8 numentries;
    MSGBF_SUB14_DISPLAY_POPUP_MENU_ENTRY entries[1];
};

struct MSGBF_SUB15_POPUP_ENTRY_SELECT
{
    u32 serial;
    u16 entry_tag;
};

//Shows codex of wisdom’s text #msg.  (msg is linearised (including sub indices) index number starting with 1)
struct MSGBF_SUB17_CODEX_OF_WISDOM
{
    u8 unk; //always 1
    u32 msgnum;
    u8 mode; //(0: “?” flashing, 1: directly opening) 
};

struct MSGBF_SUB18_ENABLE_MAP_DIFFS_ELEM
{
    u32 num_static_patches;
    u32 num_map_patches;
};
struct MSGBF_SUB18_ENABLE_MAP_DIFFS
{
    u32 num_maps;
    MSGBF_SUB18_ENABLE_MAP_DIFFS_ELEM elems[MAX_NUMER_REALMS];
};

struct MSGBF_SUB19_EXTENDED_STATS_OUT
{
    u8 type; //always 2?
    u32 serial;
    u8 unk; //always 0?
    u8 lockbits; //Bits: XXSS DDII (s=strength, d=dex, i=int), 0 = up, 1 = down, 2 = locked 
    enum
    {
        STR_MASK = 0x30,
        DEX_MASK = 0x0C,
        INT_MASK = 0x03
    };
    enum { MODE_UP, MODE_DOWN, MODE_LOCKED };
};

struct MSGBF_SUB1A_EXTENDED_STATS_IN
{
    u8 stat;
    u8 mode;
    enum { STAT_STR, STAT_DEX, STAT_INT };
    enum { MODE_UP, MODE_DOWN, MODE_LOCKED };
};

struct MSGBF_SUB1B_NEW_SPELLBOOK
{
    u16 unk; //always 1
    u32 book_serial;
    u16 graphic;
    u16 scroll_offset;
    u8 content[8];// first bit of first byte = spell #1, second bit of first byte = spell #2, first bit of second byte = spell #8, etc 

    enum { SCROLL_OFFSET_REGULAR = 1, 
        SCROLL_OFFSET_NECRO = 101,
        SCROLL_OFFSET_PALADIN = 201 };
};

struct MSGBF_SUB1C_SPELL_SELECT
{
    u16 unk2; //00 02
    u16 selected_spell; //absolute spell number. 1-64, 101-etc
};

struct MSGBF_SUB1D_CUSTOM_HOUSE_SHORT
{
    u32 house_serial;
    u32 revision;
};

struct MSGBF_SUB1E_REQ_FULL_CUSTOM_HOUSE
{
    u32 house_serial;
};

struct MSGBF_SUB20_ACTIVATE_CUSTOM_HOUSE_TOOL
{
    u32 house_serial;
    u8 unk1; //0x4 for begin 0x5 for end
    u16 unk2; //0
    u32 unk3; //all FF
    u8 unk4; // FF
};

//displays damage amount over head
struct MSGBF_SUB22_DAMAGE
{
    u16 unk; //always 1
    u32 serial;
    u8 damage_amt;
};

//no clue what this spam does.
struct MSGBF_SUB24_SESPAM
{
    u8 spam_byte;
};

struct MSGBF_SUB2A_CALL_CHARACTER_RACE_CHANGER
{
	u8 gender; // 0 = Male, 1 = Female
	u8 race;   // 1 = Human, 2 = Elf
};

struct MSGBF_SUB2A_RESULT_CHARACTER_RACE_CHANGER
{
	u16 BodyHue;
	u16 HairId;
	u16 HairHue;
	u16 BeardId;
	u16 BeardHue;
};

// to change Hair, Beard and Color (maybe Race, but not in this Release)
struct MSGBF_SUB2A_CHARACTER_RACE_CHANGER
{
	union {
		MSGBF_SUB2A_CALL_CHARACTER_RACE_CHANGER call;
		MSGBF_SUB2A_RESULT_CHARACTER_RACE_CHANGER result;
	};
};

struct MSGBF_GENERIC_COMMAND {
    u8 msgtype;
    u16 msglen;
    u16 subcmd;
    union {
        u32 keys[6];	// BF.1: Cycling key-stack
        u32 addkey;		// BF.2: Adding key to top of stack
        MSGBF_SUB4_CLOSE_GUMP closegump; //BF.4
        MSGBF_SUB5_SCREEN_SIZE screensize; //BF.5
        MSGBF_SUB6_PARTY_SYSTEM partydata; //BF.6
		u8 cursorhue;	// BF.8: Cursor Hue (0=Felucca,1=Trammel)
        char client_lang[3]; // BF.B Client language (client message, sent once at login)
        u32 serial_closed_status_gump; //BF.C closed status gump
        MSGBF_SUB10_OBJECT_CACHE objectcache; //BF.10
        u32 serial_request_popup_menu; //BF.13
        MSGBF_SUB14_DISPLAY_POPUP_MENU displaypopup; //BF.14
        MSGBF_SUB15_POPUP_ENTRY_SELECT popupselect; //BF.15
        MSGBF_SUB17_CODEX_OF_WISDOM codex; //BF.17
        MSGBF_SUB18_ENABLE_MAP_DIFFS mapdiffs; //BF.18
        MSGBF_SUB19_EXTENDED_STATS_OUT extstatsout; //BF.19
        MSGBF_SUB1A_EXTENDED_STATS_IN extstatin; //BF.1A
        MSGBF_SUB1B_NEW_SPELLBOOK newspellbook; //BF.1B
        MSGBF_SUB1C_SPELL_SELECT spellselect; //BF.1C
        MSGBF_SUB1D_CUSTOM_HOUSE_SHORT customhouseshort;
        MSGBF_SUB1E_REQ_FULL_CUSTOM_HOUSE reqfullcustomhouse;
        MSGBF_SUB20_ACTIVATE_CUSTOM_HOUSE_TOOL activatehousetool; //BF.20
        MSGBF_SUB22_DAMAGE damage; //BF.22
		u8 se_spam; //BF.24
		MSGBF_SUB2A_CHARACTER_RACE_CHANGER characterracechanger; //BF.2A
	};

	enum {
		TYPE_CYCLE_STACK                = 0x01,
		TYPE_ADD_KEY                    = 0x02,
        TYPE_CLOSE_GUMP                 = 0x04,
        TYPE_SCREEN_SIZE                = 0x05,
        TYPE_PARTY_SYSTEM               = 0x06,
		TYPE_CURSOR_HUE                 = 0x08,
        TYPE_WRESTLING_STUN             = 0x0A,
        TYPE_CLIENT_LANGUAGE            = 0x0B,
        TYPE_CLOSED_STATUS_GUMP         = 0x0C,
        TYPE_UNKNOWN1                   = 0x0F,
        TYPE_OBJECT_CACHE               = 0x10,
        TYPE_DISPLAY_POPUP_MENU         = 0x14,
        TYPE_POPUP_ENTRY_SELECT         = 0x15,
        TYPE_CODEX_OF_WISDOM            = 0x17,
        TYPE_ENABLE_MAP_DIFFS           = 0x18,
        TYPE_EXTENDED_STATS_OUT         = 0x19,
        TYPE_EXTENDED_STATS_IN          = 0x1A,
        TYPE_NEW_SPELLBOOK              = 0x1B,
        TYPE_SPELL_SELECT               = 0x1C,
        TYPE_CUSTOM_HOUSE_SHORT         = 0x1D,
        TYPE_REQ_FULL_CUSTOM_HOUSE      = 0x1E,
        TYPE_ABILITY_ICON_CONFIRM       = 0x21,
        TYPE_ACTIVATE_CUSTOM_HOUSE_TOOL = 0x20,
        TYPE_DAMAGE                     = 0x22,
		TYPE_SESPAM                     = 0x24,
		TYPE_CHARACTER_RACE_CHANGER     = 0x2A
	};
	enum {
		CURSORHUE_FELUCCA = 0,
		CURSORHUE_TRAMMEL = 1
	};
    enum {
        PARTYCMD_ADD = 1,
        PARTYCMD_REMOVE = 2,
        PARTYCMD_MEMBER_MSG = 3,
        PARTYCMD_PARTY_MSG = 4,
        PARTYCMD_LOOT_PERMISSION = 6,
        PARTYCMD_ACCEPT_INVITE = 8,
        PARTYCMD_DECLINE_INVITE = 9
    };
};

struct MSGC0_GRAPHICAL_EFFECT
{
    u8 msgtype;
    u8 type;
    u32 source_serial;
    u32 target_serial;
    u16 itemid;
    u16 x_source;
    u16 y_source;
    u8 z_source;
    u16 x_target;
    u16 y_target;
    u8 z_target;
    u8 speed;
    u8 duration;
    u16 unk; //On OSI, flamestrikes are 0x0100 
    u8 fixed_direction;
    u8 explodes;
    u32 hue;
    u32 render_mode;
};

//Argument example:
//take number 1042762:
//"Only ~1_AMOUNT~ gold could be deposited. A check for ~2_CHECK_AMOUNT~ gold was returned to you."
//the arguments string may have "100 thousand\t25 hundred", which in turn would modify the string:
//"Only 100 thousand gold could be deposited. A check for 25 hundred gold was returned to you."
struct MSGC1_LOCALIZED_MESSAGE {
    u8 msgtype;
    u16 msglen;
    u32 serial; //0xFFffFFff for system message
    u16 body; //0xFFff for system message
    u8 type; //6 lower left, 7 on player
    u16 hue;
    u16 font;
    u32 msgnumber;
    char name[30];
    u16 arguments[(SPEECH_MAX_LEN) + 1]; // _little-endian_ unicode string, tabs ('\t') seperate the arguments
};
assertsize( MSGC1_LOCALIZED_MESSAGE, 450 );

struct MSGC2_UNICODE_PROMPT
{
    u8 msgtype;
    u16 msglen;
    u32 serial;
    u32 msg_id;
	// When a server-sent message, these next 10+ bytes must all be 0x00.
	u32 unk; // unknown -- always 0 0 0 1 (from client)
    char lang[3]; // "enu" - US english
    u16 wtext[2]; // wide-character, double-null terminated

	enum {
		SERVER_MSGLEN = 0x15
	};
};
asserteql( sizeof(MSGC2_UNICODE_PROMPT), 22 );

//not used:
struct MSGC4_SEMIVISIBLE {
	u8 msgtype;
	u32 serial;
    u8 intensity;
};

struct MSGC7_3D_PARTICLE_EFFECT
{
    MSGC0_GRAPHICAL_EFFECT header;
    u16 effect_num; //see particleffect subdir
    u16 explode_effect_num; //0 if no explosion
    u16 additional_effect_num; //for moving effects, 0 otherwise
    u32 itemid; //if target is item (type 2), 0 otherwise 
    u8 layer; //(of the character, e.g left hand, right hand, … 0-4, 0xff: moving effect or target is no char) 
    u16 unk_effect; //for moving effect, 0 otherwise
};

// 0xC8
// This packet says how far away the client wants to see.. that
// allows slower modems to not get items and MOBs that are over
// a certain distance away.
struct MSGC8_UPDATE_RANGE_CHANGE {
	u8 msgtype;
	u8 range;
};
assertsize( MSGC8_UPDATE_RANGE_CHANGE, 2 );

struct MSGCC_LOCALIZED_MESSAGE_AFFIX 
{
    u8 msgtype;
    u16 msglen;
    u32 serial; //0xFFffFFff for system message
    u16 body; //0xFFff for system message
    u8 type; //6-lowerleft, 7-on player
    u16 hue;
    u16 font;
    u32 messagenum;
    u8 flags; //0x2 unk, 0x4 message doesn't move,
              //(flags & 0x1) == 0 signals affix is appended to the localization string, (flags & 0x1) == 1 signals to prepend.
    u8 name[32];
    u8 affix[1]; //nullterminated, uh not just 1 byte
    u16 arguments[1]; // _big-endian_ unicode string, tabs ('\t') seperate arguments, see 0xC1 for argument example
};

// Client sends on logout if (flag & 0x02) in packet 0xA9, 
// when server responds with same packet, client finishes logging out
struct MSGD1_LOGOUT_STATUS
{
    u8 msgtype;
    u8 unk_1;
	};
assertsize( MSGD1_LOGOUT_STATUS, 2 );

struct AOS_OBJECT_PROPERTY_LIST_ELEM
{
    u32 localization_num;
    u16 textlen;
    u16 text; //little endian Unicode text, not 0 terminated 
};

// This is used to send Prefix, Name, Suffix, Race. Then
// Adds Guild information to the tooltip, with the
// Frozen/Paralyzed/Invul at the very end.
struct AOS_MOBILE_PROPERTY_LIST_ELEM
{
    u32 name_num;
    u16 namelen;
    u16 name; //little endian Unicode text, not 0 terminated 
    u32 guild_num;
    u16 guildlen;
    u16 guild; //little endian Unicode text, not 0 terminated 
	u32 mobstate_num;
	u16 mobstatelen;
	u16 mobstate; //little endian Unicode text, not 0 terminated 
};

struct MSGD6_AOS_MOBILE_PROPERTY_LIST
{
    u8 msgtype;
    u16 msglen;
    u16 unk1; //always 1
    u32 serial;
    u8 unk2; //always 0
    u8 unk3; //always 0
    u32 listid; 
    AOS_MOBILE_PROPERTY_LIST_ELEM data[1];
};

struct MSGD6_AOS_OBJECT_PROPERTY_LIST
{
    u8 msgtype;
    u16 msglen;
    u16 unk1; //always 1
    u32 serial;
    u8 unk2; //always 0
    u8 unk3; //always 0
    u32 listid; 
    AOS_OBJECT_PROPERTY_LIST_ELEM data[1];
};

//fixme: which 0 is z?
struct CH_ERASE
{
    u8 unk1; // 0x00
    u8 unk2; // 0x00
    u8 unk3; // 0x00
    u16 tileID;
    u8 unk4; // 0x00
    u32 xoffset; // from multi object center
    u8 unk5; // 0x00
    u32 yoffset; // from multi object center
    u8 unk6; // 0x00
    u32 z; // 0x00
};

struct CH_ADD
{
    u8 unk1; // 0x00
    u8 unk2; // 0x00
    u8 unk3; // 0x00
    u16 tileID;
    u8 unk4; // 0x00
    u32 xoffset;// from multi object center
    u8 unk5; // 0x00
    u32 yoffset;// from multi object center
};

struct CH_ADD_MULTI
{
    u8 unk1; // 0x00
    u8 unk2; // 0x00
    u8 unk3; // 0x00
    u16 multiID;
    u8 unk4; // 0x00
    u32 xoffset;// from multi object center
    u8 unk5; // 0x00
    u32 yoffset;// from multi object center
};

struct CH_SELECT_FLOOR
{
    u32 unk1; // (0x00)
    u8 floornumber;
};

struct FIGHT_BOOK_ABILITY
{
    u32 unk1; // (0x00)
    u8 ability; // 1 armor ignore, 2 bleed attack … 13 whirlwind attack 
};

//must send msg A9 with flags | 0x20
struct MSGD7_AOS_GENERIC_COMMAND
{
    u8 msgtype;
    u16 msglen;
    u32 serial;
    u16 subcmd;

    enum
    {
        CUSTOM_HOUSE_BACKUP = 0x02,
        CUSTOM_HOUSE_RESTORE = 0x03,
        CUSTOM_HOUSE_COMMIT = 0x04,
        CUSTOM_HOUSE_ERASE = 0x05,
        CUSTOM_HOUSE_ADD = 0x06,
        CUSTOM_HOUSE_QUIT = 0x0C,
        CUSTOM_HOUSE_ADD_MULTI = 0x0D,
        CUSTOM_HOUSE_SYNCH = 0x0E,
        CUSTOM_HOUSE_CLEAR = 0x10,
        CUSTOM_HOUSE_SELECT_FLOOR = 0x12,
        FIGHT_BOOK = 0x19,
        CUSTOM_HOUSE_REVERT = 0x1A,
		GUILD_BUTTON = 0x28,
		QUEST_BUTTON = 0x32
    };
    union
    {
        CH_ERASE ch_erase;
        CH_ADD ch_add;
        CH_ADD_MULTI ch_add_multi;
        CH_SELECT_FLOOR ch_select_floor;
        FIGHT_BOOK_ABILITY fight_book_ability;
    };
    u8 unk; //always 7
};

struct CUSTOM_HOUSE_PLANE
{
    u32 header; // bitpacked: TTTTZZZZ UUUUUUUU LLLLLLLL UUUULLLL
                // T : numeric type value from 0-2, determines how to process 'data'
                // Z : encoded Z offset value used in type 1 and 2, actual Z value translated from table in customhouses.cpp
                // U : uncompressed length of 'data'
                // L : compressed length of 'data'
    u8 data[1];
};

struct CUSTOM_HOUSE_PLANE_BUFFER
{
    u8 planecount;
    CUSTOM_HOUSE_PLANE planes[1];
};

struct MSGD8_CUSTOM_HOUSE_DATA
{
    u8 msgtype;
    u16 msglen; //flipped
    u8 compressiontype; //0x03
    u8 unk; //usually 0
    u32 serial;
    u32 revision; //incremented whenever content changes
    u16 numtiles;
    u16 planebuffer_len; //length of remaining packet data
    CUSTOM_HOUSE_PLANE_BUFFER buffer[1];
};

struct CUSTOM_HOUSE_DATA_TYPE0
{
    u16 itemid;
    s8 x;
    s8 y;
    s8 z;
};
struct CUSTOM_HOUSE_DATA_TYPE1
{
    u16 itemid;
    s8 x;
    s8 y;
};
struct CUSTOM_HOUSE_DATA_TYPE2
{
    u16 itemid;
};

struct MSGD9_CLIENT_INFO {
    u8 msgtype;
    u8 unknown1;               // Unknown - allways 0x02
    u32 instance;              // Unique Instance ID of UO
    u32 os_major;              // OS Major
    u32 os_minor;              // OS Minor
    u32 os_revision;           // OS Revision
    u8 cpu_manufacturer;       // CPU Manufacturer
    u32 cpu_family;            // CPU Family
    u32 cpu_model;             // CPU Model
    u32 cpu_clockspeed;        // CPU Clock Speed [Mhz]
    u8 cpu_quantity;           // CPU Quantity
    u32 memory;                // Memory [MB]
    u32 screen_width;          // Screen Width
    u32 screen_height;         // Screen Height
    u32 screen_depth;          // Screen Depth [Bit]
    u16 directx_major;         // DirectX Major
    u16 directx_minor;         // DirectX Minor
    u16 video_description[64]; // Video Card Description [wide-character]
    u32 video_vendor;          // Video Card Vendor ID
    u32 video_device;          // Video Card Device ID
    u32 video_memory;          // Video Card Memory [MB]
    u8 distribution;           // Distribution
    u8 clients_running;        // Clients Running
    u8 clients_installed;      // Clients Installed
    u8 partial_installed;      // Partial Insstalled
    u16 langcode[4];           // Language Code [wide-character]
    u8 unknown2[64];           // Unknown
};
asserteql( sizeof(MSGD9_CLIENT_INFO), 0x10C );

#ifdef _MSC_VER		/* Visual C++ 4.0 + */
#pragma pack( pop )
#else
#pragma pack()
#endif

#endif
