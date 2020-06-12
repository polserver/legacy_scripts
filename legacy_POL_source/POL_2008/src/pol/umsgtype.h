// History
//   2005/04/03 Shinigami: updated MSGA9_START_FLAGS with SE flags
//   2005/10/16 Shinigami: transformed unk1/2 into x/y inside MSGB0_GUMP_DIALOG
//   2005/11/25 Shinigami: added MSGBF_GENERIC_COMMAND_ID
//   2005/12/05 MuadDib: Added CHAR_HILITE_INVUL for new invul tag handling
//   2006/05/16 Shinigami: updated MSGA9_START_FLAGS with ML flags
//   2007/07/08 Shinigami: added MSGE3_KR_VERIFIER and MSGE4_KR_VERIFIER_RESPONSE - used by UO:KR

#ifndef __UMSGH
#define __UMSGH

#include "clib/rawtypes.h"
#include "skillid.h"
#include "ucfg.h"


/* other messages:
server to client:
   53 05 'another character is actve in the world..'
client to server:
*/

#define MSG00_CHAR_CREATE_ID        0x00
#define MSG01_LOGOUT_ID             0x01
#define MSG02_WALK_ID               0x02
#define MSG03_SPEECH_ID             0x03
#define MSG04_GODMODE_TOGGLE_ID     0x04
#define MSG05_ATTACK_ID             0x05
#define MSG06_DOUBLE_CLICK_ID       0x06
#define MSG07_GET_ITEM_ID           0x07
#define MSG08_DROP_ITEM_ID          0x08
#define MSG09_SINGLE_CLICK_ID       0x09
#define MSG11_STATS_WINDOW_ID       0x11
#define MSG12_EXTENDED_CMD_ID       0x12
#define MSG13_EQUIP_ITEM_ID         0x13
#define MSG15_FOLLOW_CHAR_ID        0x15
#define MSGOUT_1A_ITEM_INFO         0x1A
#define MSGOUT_1B_STARTUP           0x1B
#define MSGOUT_1C_TEXT              0x1C
#define MSGOUT_1D_REMOVE_OBJECT     0x1D
#define MSGOUT_1E_ANIMATE_ID        0x1E
#define MSGOUT_1F_EXPLODE_ID        0x1F
#define MSGOUT_20_GO_XYZ            0x20
#define MSGOUT_21_MOVE_REJECT_ID    0x21
#define MSG22_RESYNC_REQUEST_ID     0x22
#define MSGOUT_22_MOVE_APPROVE_ID   0x22
#define MSG23_MOVE_ITEM_ID          0x23
#define MSGOUT_24_OPEN_GUMP         0x24
#define MSGOUT_25_PUT_IN_CONTAINER  0x25
#define MSGOUT_26_OLDCLIENT_KICK_ID 0x26
#define MSGOUT_27_MOVE_ITEM_FAILURE 0x27
#define MSGOUT_28_DROP_OBJ_FAILURE_ID 0x28
#define MSGOUT_29_DROP_OBJ_SUCCESS_ID 0x29
#define MSGOUT_2A_BLOOD_MODE_ID     0x2A
#define MSGOUT_2B_GOD_MODE_ID       0x2B
#define MSG2C_RESURRECT_MENU_ID     0x2C
#define MSG2D_HEALTH_ID             0x2D
#define MSGOUT_2E_WORN_ITEM         0x2E
#define MSGOUT_2F_SWING_ID          0x2F
#define MSGOUT_30_ATTACK_OK_ID      0x30
#define MSGOUT_31_ATTACK_ENDED_ID   0x31
#define MSGOUT_32_HACK_MOVER_ID     0x32
#define MSGOUT_33_FLOW_CONTROL      0x33
#define MSG34_STATUS_REQUEST_ID     0x34
#define MSG35_RESOURCE_TYPE_ID      0x35
#define MSG36_RESOURCE_TILE_DATA_ID 0x36
#define MSG37_MOVE_OBJECT_ID        0x37
#define MSG38_FOLLOW_MOVE_ID        0x38
#define MSG39_GROUPS_ID             0x39
#define MSGOUT_3A_SKILL_VALUES_ID   0x3A
// MSG3B_BUY_ITEMS
#define MSGOUT_3C_CONTAINER_CONTENTS 0x3C
#define MSG3D_SHIP_ID               0x3D
#define MSG3E_VERSIONS_ID           0x3E
#define MSG3F_UPDATE_OBJ_ID         0x3F
#define MSG40_UPDATE_TERRAIN_ID     0x40
#define MSGOUT_41_UPDATE_TILEDATA_ID 0x41
#define MSGOUT_42_UPDATE_ART_ID     0x42
#define MSGOUT_43_UPDATE_ANIM_ID    0x43
#define MSGOUT_44_UPDATE_HUES_ID    0x44
#define MSG45_VER_OK_ID             0x45
#define MSG46_NEW_ART_ID            0x46
#define MSG47_NEW_TERRAIN_ID        0x47
#define MSG48_NEW_ANIM_ID           0x48
#define MSG49_NEW_HUES_ID           0x49
#define MSG4A_DESTROY_ART_ID        0x4A
#define MSG4B_CHECK_VER_ID          0x4B
#define MSG4C_SCRIPT_NAMES_ID       0x4C
#define MSG4D_SCRIPT_FILE_ID        0x4D
// MSG4E_LIGHT_POINT
#define MSGOUT_4F_LIGHT_LEVEL       0x4F
#define MSG50_BOARD_HEADER_ID       0x50
#define MSG51_BOARD_MSG_ID          0x51
#define MSG52_POST_MSG_ID           0x52
// MSG53_INACTIVITY_WARNING
#define MSGOUT_54_SOUND_EFFECT      0x54
#define MSG55_LOGIN_COMPLETE_ID     0x55
#define MSG56_MAP_PIN_ID            0x56
#define MSG57_UPDATE_REGIONS_ID     0x57
#define MSG58_NEW_REGION_ID         0x58
#define MSG59_NEW_CONTEXT_FX_ID     0x59
#define MSG5A_UPDATE_CONTEXT_FX_ID  0x5A
#define MSG5B_GAME_TIME_ID          0x5B
#define MSG5C_RESTART_VER_ID        0x5C
#define MSG5D_CHAR_SELECT_ID        0x5D
#define MSG5E_SERVER_LIST_ID        0x5E
#define MSG5F_SERVER_ADD_ID         0x5F
#define MSG60_SERVER_REMOVE_ID      0x60
#define MSG61_DESTROY_STATIC_ID     0x61
#define MSG62_MOVE_STATIC_ID        0x62
#define MSG63_AREA_LOAD_ID          0x63
#define MSG64_AREA_LOAD_REQ_ID      0x64
#define MSGOUT_65_WEATHER_ID        0x65
#define MSG66_READ_BOOK_ID          0x66
#define MSG69_UNKNOWN_ID            0x69
#define MSG6C_TARGET_CURSOR_ID      0x6C
#define MSG6D_MUSIC_ID              0x6D
#define MSG6E_DO_ACTION_ID          0x6E
#define MSGOUT_70_SPELL_EFFECT_ID   0x70

#define MSG71_BULLETIN_BOARD_ID     0x71
#define MSGOUT_71_BULLETIN_BOARD_ID 0x71

// This both in and out? 
#define MSG72_MODE_SET_ID           0x72
#define MSGOUT_72_MODE_SET          0x72

#define MSG73_KEEP_ALIVE_ID         0x73
#define MSGOUT_74_DESCRIBE_CONTENTS 0x74
#define MSG75_RENAME_CHAR_ID        0x75
#define MSGOUT_77_MOVE              0x77
#define MSGOUT_78_OWN_CREATE        0x78
#define MSGOUT_7C_ITEM_SELECT_MENU  0x7C
#define MSG7D_MENU_SELECTION_ID     0x7D
#define MSG80_LOGINSERVER_LOGIN_ID  0x80
#define MSGOUT_82_LOGIN_ERROR       0x82
#define MSG83_DELETE_CHARACTER_ID   0x83
#define MSGOUT_88_PAPERDOLL         0x88
#define MSGOUT_89_SHOW_CORPSE       0x89
#define MSGOUT_8C_LOGIN3            0x8C
#define MSGOUT_90_MAP_ID            0x90
#define MSG91_GAMESERVER_LOGIN_ID   0x91
// FIXME should pick one (ID 95)
#define MSG95_DYE_VAT_ID            0x95
#define MSG95_COLOR_SELECT_ID       0x95
#define MSG98_ALLNAMES_ID			0x98
#define MSGOUT_99_MULTI_PLACEMENT_ID 0x99
#define MSG9A_PROMPT_ID             0x9A
#define MSG9B_GM_PAGE_ID            0x9B
#define MSG9B_HELP_ID                   0x9B
#define MSGA0_SERVER_SELECT_ID      0xA0
#define MSGA1_UPDATE_STATS_ST_HP_ID     0xA1
#define MSGA2_UPDATE_STATS_IN_MN_ID     0xA2
#define MSGA3_UPDATE_STATS_DX_STM_ID    0xA3
#define MSGOUT_A6_TIP_WINDOW_ID     0xA6
#define MSGA7_GET_TIP_ID            0xA7
#define MSGOUT_A8_SERVER_LIST       0xA8
#define MSGOUT_A9_START             0xA9
#define MSGOUT_AA_SET_OPPONENT_ID   0xAA
#define MSGAB_TEXTENTRY_GUMP_ID         0xAB
#define MSGAC_TEXTENTRY_RESULT_ID       0xAC
#define MSGOUT_AF_DISPLAY_DEATH_ACTION  0xAF
#define MSGB0_GUMP_DIALOG_ID            0xB0
#define MSGB1_GUMP_BUTTON_ID            0xB1
#define MSGBF_GENERIC_COMMAND_ID        0xBF
#define MSGE3_KR_VERIFIER_ID            0xE3
#define MSGE4_KR_VERIFIER_RESPONSE_ID   0xE4

// Changed 1/9/2007 MuadDib per OSI packet logs.
const u8 CHAR_FLAG1_INVISIBLE   = 0x80;
const u8 CHAR_FLAG1_WARMODE     = 0x40;
const u8 CHAR_FLAG1_POISONED    = 0x04;
const u8 CHAR_FLAG1_CANALTER    = 0x02;
const u8 CHAR_FLAG1_WARMODE_AOS = 0x01;
const u8 CHAR_FLAG1_NORMAL      = 0x00;

#ifdef _MSC_VER     /* Visual C++ 4.0 and above */
#pragma pack( push, 1 )
#else
/* Ok, my build of GCC supports this, yay! */
#undef POSTSTRUCT_PACK
#define POSTSTRUCT_PACK
#pragma pack(1)
#endif

struct MSGXX_TEXTDEF {
    u8 type;
    u16 color;
    u16 font;
};
asserteql( sizeof(MSGXX_TEXTDEF), 5 );

/* NOTE for all message types: all u16/u32 variables are BIG-ENDIAN, unless otherwise noted. */

struct MSG00_CHAR_CREATE {
    u8 msgtype;      //Byte 0
    u32 serial;      //Bytes 1-4
    u8 unk5; 
    u8 unk6;
    u8 unk7;
    u8 unk8;
    u8 unk9;
    char Name[30];     //Bytes 10-40
    char Password[30]; 
    u8 Sex;          //Byte 70 (0 if Human Male, 1 if Human Female, 2 if Elf Male, 3 if Elf Female)
    u8 Strength;     //Byte 71
    u8 Dexterity;    //Byte 72
    u8 Intelligence; //Byte 73
    u8 SkillNumber1; //Byte 74
    u8 SkillValue1;  //Byte 75
    u8 SkillNumber2; //Byte 76
    u8 SkillValue2;  //Byte 77
    u8 SkillNumber3; //Byte 78
    u8 SkillValue3;  //Byte 79
    u16 SkinColor;   //Byte 80-81
    u16 HairStyle;   //Byte 82-83
    u16 HairColor;   //Byte 84-85
    u16 BeardStyle;  //Byte 86-87
    u16 BeardColor;  //Byte 88-89
    u8 unk90;
    u8 StartIndex;   //Byte 91 (actually a <word> with unk90)
    u8 unk92;
    u8 unk93;        // <word> with unk92
    u8 unk94;
    u8 CharNumber;   //I think (yup! <word> with unk94 -TJ)
    u8 clientip[4];
    u16 shirtcolor;
    u16 pantscolor;
};
asserteql( sizeof(MSG00_CHAR_CREATE), 104 );

struct MSG01_LOGOUT {
	u8 msgtype;
	u32 unk; // 0xffffffff
};
asserteql( sizeof(MSG01_LOGOUT), 5 );

/* Message Type 02, WALK 
        dir: low three bits are direction.
             bit 0x80 is "run" bit.
*/
struct MSG02_WALK {
    u8 msgtype;
    u8 dir; 
    u8 movenum; // Sequence Number (used with fastwalk prevention!) -TJ
    u32 codes;
};
#define MSG02_FACING_MASK 0x07
#define MSG02_DIR_RUNNING_BIT 0x80

/* Message Type 03, Speech Message.  Received from client when a 
   character talks.
    Note text[1].  Text length should be determinable from msglen:
        msg->msglen - offsetof( MSG03_SPEECH, text )
        includes the length of the text as well as the null terminator
            What should the valid range for textlen be?  I don't know.

   SPEECH_MAX_LEN: max length, not including terminator:
    A line of dots is 120 characters.  I haven't found anything
    yielding more characters, but 200 characters should be safe.
*/
#define SPEECH_MAX_LEN 200
struct MSG03_SPEECH {
    u8  msgtype;
    u16 msglen;
    
    MSGXX_TEXTDEF textdef;

    char text[SPEECH_MAX_LEN + 1]; /* one or more characters, null-terminated. */
};

// Message 04: Client request for god-mode on/off
// Server response with MSG2B_GOD_MODE
struct MSG04_GODMODE_TOGGLE {
	u8 msgtype;
	u8 mode; // on/off. 0/1?
};
asserteql( sizeof(MSG04_GODMODE_TOGGLE), 2 );

struct MSG05_ATTACK {
    u8 msgtype;
    u32 serial;
};
assertsize( MSG05_ATTACK, 5 );

struct MSG06_DOUBLE_CLICK {
    u8 msgtype;
    u32 serial; // 0x80000000 bit seems not to count.
};
asserteql( sizeof(MSG06_DOUBLE_CLICK), 5 );


struct MSG07_GET_ITEM {
    u8 msgtype;
    u32 serial;
    u16 amount;
};
asserteql( sizeof(MSG07_GET_ITEM), 7 );

struct MSG08_DROP_ITEM {
    u8 msgtype;
    u32 item_serial;
    u16 x; // only if x & 0xFF00 = 0xFF?
    u16 y;
    s8 z;
    u32 target_serial;
};
asserteql( sizeof(MSG08_DROP_ITEM), 14 );

struct MSG09_SINGLE_CLICK {
    u8 msgtype;
    u32 serial;
};
asserteql( sizeof(MSG09_SINGLE_CLICK), 05 );

struct MSG11_STATS_WINDOW {
    u8 msgtype;
    u16 msglen;
    u32 serial;
    char name[30];
    u16 hits;
    u16 max_hits;
    u8 renameable; // 00, or FF if can rename
    u8 moreinfo; // is the data following present?
    u8 gender; // GENDER_MALE or GENDER_FEMALE (see uconst.h)
    u16 str;
    u16 dex;
    u16 intel;
    
    u16 stamina;
    u16 max_stamina;

    u16 mana;
    u16 max_mana;

    u32 gold;
    u16 AR;
    u16 weight;
//	u8 race; // UOML Introduced. Elven stuff maybe?
	// If moreinfo is 3 or 4
	u16 statcap;
	u8 followers;
	u8 followers_max;
	// If moreinfo is 4
	u16 fireresist;
	u16 coldresist;
	u16 poisonresist;
	u16 energyresist;
	u16 luck;
	u16 damage_min;
	u16 damage_max;
	u32 titching;
};
asserteql( sizeof(MSG11_STATS_WINDOW), 88 );

struct MSG12_EXTENDED_CMD {
    u8 msgtype;
    u16 msglen;
    u8 submsgtype;
    u8 data[ 300 ];
};
asserteql( sizeof(MSG12_EXTENDED_CMD), 304 );

struct MSG13_EQUIP_ITEM {
    u8 msgtype;
    u32 serial;
    u8 layer;
    u32 equipped_on;
};
asserteql( sizeof(MSG13_EQUIP_ITEM), 10 );

struct MSG15_FOLLOW_CHAR {
	u8 msgtype;
	u8 unk1_8[8];
};
asserteql( sizeof(MSG15_FOLLOW_CHAR), 9 );

/* a boat's position
Decrypted Server Data (47 bytes left): 
0000: 1a 00 0e 40 79 4a aa 40  0c 0b cf 03 32 fb         ...@yJ.@ ....2...
*/
struct MSG1A_ITEM_INFO_C {
    u8 msgtype;
    u16 msglen;
    u32 serial;
    u16 graphic;
    u16 x;
    u16 y; 
    u8 z;
};
asserteql( sizeof(MSG1A_ITEM_INFO_C), 0x0E );

// use message type D when?
struct MSG1A_ITEM_INFO_D {
    u8 msgtype;
    u16 msglen;
    u32 serial; // bit 0x80000000 enables piles
    u16 graphic;
    u16 amount; // or is this color?
    u16 x;          
    u16 y; // bits 0x80 and 0x40 are Dye and Move (dunno which is which)
    u8 z;
};
assertsize( MSG1A_ITEM_INFO_D, 0x10 );

struct MSG1A_ITEM_INFO_A {
    u8 msgtype;
    u16 msglen;
    u32 serial; // bit 0x80000000 enables piles
    u16 graphic;
    u16 amount;
    u16 x;
    u16 y; // bits 0x80 and 0x40 are Dye and Move (dunno which is which)
    u8 z;
    u16 color;
    u8 flags;  
};
asserteql( sizeof(MSG1A_ITEM_INFO_A), 19 );

// use message type B if facing of an item is nonzero
struct MSG1A_ITEM_INFO_B {
    u8 msgtype;
    u16 msglen;
    u32 serial; // bit 0x80000000 enables piles
    u16 graphic;
    u16 amount;
    u16 x;          // set bit 0x8000 for this msg type
    u16 y; // bits 0x80 and 0x40 are Dye and Move (dunno which is which)
    u8 facing;
    u8 z;
    u16 color;
    u8 flags;  
};
asserteql( sizeof(MSG1A_ITEM_INFO_B), 20 );

#define ITEM_FLAG_FORCE_MOVABLE 0x20
#define ITEM_FLAG_HIDDEN        0x80

struct MSG1B_STARTUP {
    u8 msgtype;
    u32 serial;
    u8 unk5, unk6, unk7, unk8;
    u16 graphic;
    u16 x;
    u16 y;
    u8 unk_15;
    u8 z;
    u8 dir;
    u8 unk18;
    u8 unk19;
    u8 unk20;
    u8 unk21_7F;
    u8 unk22;
    
    u16 map_startx;
    u16 map_starty;
    u16 map_width;
	u16 map_height;
    u8 unk31;
    u8 unk32;
    u8 unk33;
    u8 unk34;
    u8 unk35;
    u8 unk36;
};
asserteql( sizeof(MSG1B_STARTUP), 37 );

struct MSG1C_TEXT {
    u8 msgtype;
    u16 msglen;
    u32 source_serial;
    u16 source_graphic;
    
    MSGXX_TEXTDEF textdef;

    char speaker_name[ 30 ];
    char text[ SPEECH_MAX_LEN + 1 ];
};
asserteql( sizeof(MSG1C_TEXT), 44 + SPEECH_MAX_LEN + 1 );

struct MSG1D_REMOVE_OBJECT {
    u8 msgtype;
    u32 serial;
};
asserteql( sizeof(MSG1D_REMOVE_OBJECT), 5 );

struct MSG1E_CONTROL_ANIMATION {
	u8 msgtype;
	u8 unk1_3[3];
};
asserteql( sizeof(MSG1E_CONTROL_ANIMATION), 4 );

struct MSG1F_EXPLODE {
	u8 msgtype;
	u8 unk1_7[7];
};
asserteql( sizeof(MSG1F_EXPLODE), 8 );

struct MSG20_GO_XYZ {
    u8 msgtype;
    u32 serial;
    u16 graphic;
    u8 unk7;
    u16 color;
    u8 flag1;
    u16 x;
    u16 y;
    u8 unk15;
    u8 unk16;
    u8 dir;     // NOTE: bit 0x80 seems to always be set. 
    u8 z;
     
    /*
    enum {
        FLAG_NORMAL = 0x00,
        FLAG_WARMODE = 0x40,
        FLAG_HIDDEN = 0x80
    };
    */
};
asserteql( sizeof(MSG20_GO_XYZ), 19 );

struct MSG21_MOVE_REJECT {
    u8 msgtype;
    u8 sequence;
    u16 x;
    u16 y;
    u8 facing;
    u8 z;
};
asserteql( sizeof(MSG21_MOVE_REJECT), 8 );

struct MSG22_MOVE_APPROVE {
    u8 msgtype;
    u8 movenum;
    u8 unk_41;
};
asserteql( sizeof(MSG22_MOVE_APPROVE), 3 );

struct MSG22_RESYNC_REQUEST {
    u8 msgtype;
    u8 zero1;
    u8 zero2;
};
asserteql( sizeof(MSG22_RESYNC_REQUEST), 3 );

struct MSG23_MOVE_ITEM {
	u8 msgtype;
	u16 itemid;
	u8 zero1;
	u16 zero2;
	u16 amount; // item amount
	u32 src_serial;
	u16 src_x;
	u16 src_y;
	s8 src_z;
	u32 tgt_serial;
	u16 tgt_x;
	u16 tgt_y;
	s8 tgt_z;
};
asserteql( sizeof(MSG23_MOVE_ITEM), 26 );

struct MSG24_OPEN_GUMP {
    u8 msgtype;
    u32 serial;
    u16 gump; // ugh. 0x0009 is the corpse gump...
};
asserteql( sizeof(MSG24_OPEN_GUMP), 7 );
// NOTE: Msg Type 24 followed by message type 3C.
// (for opening containers)

struct MSG25_PUT_IN_CONTAINER {
    u8 msgtype;
    u32 serial;
    u16 graphic;
    u8 unk7; // layer?
    u16 amount; // ??
    u16 x;
    u16 y;
    u32 container_serial;
    u16 color;
};
asserteql( sizeof( MSG25_PUT_IN_CONTAINER ), 20);

// Server->Client
struct MSG26_OLDCLIENT_KICK {
	u8 msgtype;
	u32 gmserial; // who kicked :)
};
asserteql( sizeof(MSG26_OLDCLIENT_KICK), 5 );

struct MSG27_MOVE_ITEM_FAILURE {
    u8 msgtype;
    u8 reason;
};
asserteql( sizeof(MSG27_MOVE_ITEM_FAILURE), 2 );
#define MOVE_ITEM_FAILURE_CANNOT_PICK_THAT_UP       0
#define MOVE_ITEM_FAILURE_TOO_FAR_AWAY              1
#define MOVE_ITEM_FAILURE_OUT_OF_SIGHT              2
#define MOVE_ITEM_FAILURE_BELONGS_TO_OTHER          3
#define MOVE_ITEM_FAILURE_ALREADY_HOLDING_AN_ITEM   4
#define MOVE_ITEM_FAILURE_ALREADY_WORN              5
#define MOVE_ITEM_FAILURE_ILLEGAL_EQUIP             5

// none of the below are right, BTW.
#define MOVE_ITEM_FAILURE_UNKNOWN 5
#define MOVE_ITEM_FAILURE_ALREADY_PICKED_UP 5
#define MOVE_ITEM_FAILURE_ALREADY_CARRYING 5

struct MSG28_DROP_OBJ_FAILURE {
	u8 msgtype;
	u32 unk1;
};
asserteql( sizeof(MSG28_DROP_OBJ_FAILURE), 5 );

struct MSG29_DROP_OBJ_SUCCESS {
	u8 msgtype;
};
asserteql( sizeof(MSG29_DROP_OBJ_SUCCESS), 1 );

struct MSG2A_BLOOD_MODE {
	u8 msgtype;
	u32 unk1;
};
asserteql( sizeof(MSG2A_BLOOD_MODE), 5 );

// Message 2B: Server response to MSG04_GODMODE_TOGGLE
struct MSG2B_GOD_MODE {
	u8 msgtype;
	u8 mode; // on/off. 0/1?
};
asserteql( sizeof(MSG2B_GOD_MODE), 2 );

/* Message 2C:  A little bit strange.
   First, the server sends "2C 00" to the client, to tell it it's dead.
   Next, the client pops up the res menu.
   Next, the user picks.  If the user picks ghost, the client sends "2C 02" - "2C 01" if instares.
   Now, while a ghost, it seems the client transmits "2C 00" in front of MSG72 "war mode"
   messages.  Not sure why.
 */
struct MSG2C_RESURRECT_MENU {
    u8 msgtype;
    u8 choice;
};
asserteql( sizeof(MSG2C_RESURRECT_MENU), 2 );
#define RESURRECT_CHOICE_SELECT         0
#define RESURRECT_CHOICE_INSTARES       1
#define RESURRECT_CHOICE_GHOST          2

struct MSG2D_HEALTH {
	u8 msgtype;
	u8 unk1_10[10];
};
asserteql( sizeof(MSG2D_HEALTH), 11 );

struct MSG2E_WORN_ITEM {
    u8 msgtype;
    u32 serial;
    u16 graphic;
    u8 unk7;
    u8 layer;
    u32 worn_by;
    u16 color;
};
asserteql( sizeof(MSG2E_WORN_ITEM), 15 );

/* MSG2F: came up in battle */
struct MSG2F_SWING {
    u8 msgtype;
    u8 zero1;
    u32 attacker_serial;
    u32 defender_serial;
};
asserteql( sizeof(MSG2F_SWING), 10 );

struct MSG30_ATTACK_OK {
	u8 msgtype;
	u32 unk1;
};
asserteql( sizeof(MSG30_ATTACK_OK), 5 );

struct MSG31_ATTACK_ENDED {
	u8 msgtype;
};
asserteql( sizeof(MSG31_ATTACK_ENDED), 1 );

struct MSG32_HACK_MOVER {
	u8 msgtype;
	u8 unk1;
};
asserteql( sizeof(MSG32_HACK_MOVER), 2 );

struct MSG33_FLOW_CONTROL {
    u8 msgtype;
    u8 flow;
};
#define MSGOPT_33_FLOW_PAUSE 1
#define MSGOPT_33_FLOW_RESTART 0

struct MSG34_STATUS_REQUEST {
    u8 msgtype;
    u32 serial1;
    u8 stattype;
    u32 serial2;
};
asserteql( sizeof(MSG34_STATUS_REQUEST), 10 );
#define STATTYPE_STATWINDOW     4
#define STATTYPE_SKILLWINDOW    5

struct MSG35_RESOURCE_TYPE { // get resource type??
	u8 msgtype;
	u32 unk1_35[35];
};
asserteql( sizeof(MSG35_RESOURCE_TYPE), 141 );

struct MSG36_RESOURCE_TILE_DATA {
	u8 msgtype;
	u16 msglen;
	u8 unk1[1];
};

struct MSG37_MOVE_OBJECT {
	u8 msgtype;
	u8 unk1_7[7];
};
asserteql( sizeof(MSG37_MOVE_OBJECT), 8 );

struct MSG38_FOLLOW_MOVE {
	u8 msgtype;
	u8 unk1_6[6];
};
asserteql( sizeof(MSG38_FOLLOW_MOVE), 7 );

struct MSG39_GROUPS {
	u8 msgtype;
	u8 unk1_8[8];
};
asserteql( sizeof(MSG39_GROUPS), 9 );

/* NOTE on MSG3A: 
    send the header, plus skills[] for each skill, 
    plus two zeros in 'skillid' after all the skills
    sent.
    I'm not sure if it's OK to only send a few skills.
*/
struct MSG3A_SKILL_VALUES
{
    u8 msgtype;
    u16 msglen;
    u8 unk3;
    struct
    {
        u16 skillid;
        u16 value;
        u16 value_unmod;
        u8 lock_mode;
    } skills[100];//SKILLID__CLIENT_HIGHEST - SKILLID__CLIENT_LOWEST + 1 ];
    enum { LOCK_NONE = 0, LOCK_DOWN = 1, LOCK_LOCKED = 2 };
    u16 terminator;
};
//asserteql( sizeof(MSG3A_SKILL_VALUES), 0xBE+2 );


// msg 3C: be sure to not send the whole structure, just as many 'items' as you insert
struct MSG3C_CONTAINER_CONTENTS {
    u8 msgtype;
    u16 msglen; 
    u16 count;
    struct {
        u32 serial;
        u16 graphic;
        u8 unk6_00;
        u16 amount;
        u16 x;
        u16 y;
        u32 container_serial;
        u16 color;
    } items[ MAX_CONTAINER_ITEMS ];
};
asserteql( sizeof(MSG3C_CONTAINER_CONTENTS), 5 + MAX_CONTAINER_ITEMS * 19 );

struct MSG3D_SHIP {
	u8 msgtype;
	u8 unk1;
};
asserteql( sizeof(MSG3D_SHIP), 2 );

struct MSG3E_VERSIONS {
	u8 msgtype;
	u8 unk1_x24[0x24];
};
asserteql( sizeof(MSG3E_VERSIONS), 0x25 );


struct MSG3F_UPDATE_OBJ {
	u8 msgtype;
	u16 msglen;
	u8 data[1];
};


struct MSG40_UPDATE_TERRAIN {
	u8 msgtype;
	u8 unk1_xC8[0xC8];
};
asserteql( sizeof(MSG40_UPDATE_TERRAIN), 0xC9 );

// Server->Client
struct MSG41_UPDATE_TILEDATA {
	u8 msgtype;
	u16 msglen;
	u16 tileid;
	u8 tiledata[37];
	u16 unk; // F777
};

// Server->Client
struct MSG42_UPDATE_ART {
	u8 msgtype;
	u16 msglen;
	u8 data[1];
};

// Server->Client
struct MSG43_UPDATE_ANIM {
	u8 msgtype;
	u8 unk1_x28[0x28];
};
asserteql( sizeof(MSG43_UPDATE_ANIM), 0x29 );

// Server->Client
struct MSG44_UPDATE_HUES {
	u8 msgtype;
	u8 unk1_xC8[0xC8];
};
asserteql( sizeof(MSG44_UPDATE_HUES), 0xC9 );

struct MSG45_VER_OK {
	u8 msgtype;
	u32 unk1;
};
asserteql( sizeof(MSG45_VER_OK), 5 );

struct MSG46_NEW_ART {
	u8 msgtype;
	u16 msglen;
	u32 tileid;
	u8 artdata[1];
};

struct MSG47_NEW_TERRAIN {
	u8 msgtype;
	u16 x;
	u16 y;
	u16 artid;
	u32 unk1;
};
asserteql( sizeof(MSG47_NEW_TERRAIN), 11 );

struct MSG48_NEW_ANIM {
	u8 msgtype;
	u32 tileid;
	u8 frames[64];
	u8 unk1;
	u8 framecount;
	u8 frameinterval;
	u8 startinterval;
};
asserteql( sizeof(MSG48_NEW_ANIM), 73 );

struct MSG49_NEW_HUES {
	u8 msgtype;
	u32 hueid;
	u16 huevalues[32];
	u16 start;
	u16 end;
	char name[20];
};
asserteql( sizeof(MSG49_NEW_HUES), 93 );

struct MSG4A_DESTROY_ART {
	u8 msgtype;
	u32 artid;
};
asserteql( sizeof(MSG4A_DESTROY_ART), 5 );

struct MSG4B_CHECK_VER {
	u8 msgtype;
	u8 unk1_8[8];
};
asserteql( sizeof(MSG4B_CHECK_VER), 9 );

struct MSG4C_SCRIPT_NAMES {
	u8 msgtype;
	u16 msglen;
	u8 data[1];
};

struct MSG4D_SCRIPT_FILE {
	u8 msgtype;
	u16 msglen;
	u8 data[1];
};

struct MSG4E_LIGHT_POINT {
    u8 msgtype;
    u32 mobile_serial;
    u8 lightlevel;
};
asserteql( sizeof(MSG4E_LIGHT_POINT), 6 );

struct MSG4F_LIGHT_LEVEL {
    u8 msgtype;
    u8 lightlevel; // MIN_LIGHTLEVEL - MAX_LIGHTLEVEL
};
asserteql( sizeof(MSG4F_LIGHT_LEVEL), 2 );

struct MSG50_BOARD_HEADER {
	u8 msgtype;
	u16 msglen;
	u8 data[1];
};

struct MSG51_BOARD_MSG {
	u8 msgtype;
	u16 msglen;
	u8 data[1];
};

struct MSG52_POST_MSG {
	u8 msgtype;
	u16 msglen;
	u8 data[1];
};

struct MSG54_SOUND_EFFECT {
    u8 msgtype;
    u8 flags; // 0 = repeating, 1 = single play
    u16 effect; // SOUND_EFFECT_XX, see sfx.h
    u16 volume;
    u16 x;
    u16 y;
    s16 z;
	enum { FLAG_REPEAT = 0, FLAG_SINGLEPLAY = 1 };
};
asserteql( sizeof(MSG54_SOUND_EFFECT), 12 );

struct MSG55_LOGIN_COMPLETE {
	u8 msgtype;
};
asserteql( sizeof(MSG55_LOGIN_COMPLETE), 1 );

struct MSG56_MAP_PIN
{
    u8 msgtype;
    u32 serial;
    u8 type;
    u8 pinidx;
    u16 pinx;
    u16 piny;
    enum {
		TYPE_ADD = 1,
		TYPE_INSERT,
		TYPE_CHANGE,
		TYPE_REMOVE,
		TYPE_REMOVE_ALL,
		TYPE_TOGGLE_EDIT,
		TYPE_TOGGLE_RESPONSE
    };
};
asserteql( sizeof(MSG56_MAP_PIN), 11 );

struct MSG57_UPDATE_REGIONS {
	u8 msgtype;
	u8 unk1_x6D[0x6D];
};
asserteql( sizeof(MSG57_UPDATE_REGIONS), 0x6E );

struct MSG58_NEW_REGION {
	u8 msgtype;
	char areaname[40];
	u32 zero1;
	u16 x;
	u16 y;
	u16 width;
	u16 height;
	s16 z1;
	s16 z2;
	char description[40];
	u16 soundfx;
	u16 music;
	u16 soundfx_night;
	u8 dungeon;
	u16 light;
};
asserteql( sizeof(MSG58_NEW_REGION), 106 );

struct MSG59_NEW_CONTEXT_FX {
	u8 msgtype;
	u16 msglen;
	u8 data[1];
};

struct MSG5A_UPDATE_CONTEXT_FX {
	u8 msgtype;
	u16 msglen;
	u8 data[1];
};

struct MSG5B_GAME_TIME {
	u8 msgtype;
	u8 hours;
	u8 minutes;
	u8 seconds;
};
asserteql( sizeof(MSG5B_GAME_TIME), 4 );

struct MSG5C_RESTART_VER {
	u8 msgtype;
	u8 unk1;
};
asserteql( sizeof(MSG5C_RESTART_VER), 2 );

struct MSG5D_CHAR_SELECT {
    u8 msgtype;
    u8 unk_x01_x43[0x43]; // rel: 0x04..0x21 is char name, 0x22..end is password
    u8 charidx; /* 0x44 */
    u8 unk45;
    u8 unk46;
    u8 unk47;
    u8 unk48;
};
asserteql( sizeof(MSG5D_CHAR_SELECT), 0x49 );

struct MSG5E_SERVER_LIST {
	u8 msgtype;
	u16 msglen;
	u8 data[1];
};

struct MSG5F_SERVER_ADD {
	u8 msgtype;
	u8 unk1_x30[0x30];
};
asserteql( sizeof(MSG5F_SERVER_ADD), 0x31 );

struct MSG60_SERVER_REMOVE {
	u8 msgtype;
	u8 unk1_4[4];
};
asserteql( sizeof(MSG60_SERVER_REMOVE), 5 );

struct MSG61_DESTROY_STATIC{
	u8 msgtype;
	u16 x;
	u16 y;
	u8 unk1;
	s8 z;
	u16 id;
};
asserteql( sizeof(MSG61_DESTROY_STATIC), 9 );

struct MSG62_MOVE_STATIC {
	u8 msgtype;
	u8 unk1_14[14];
};
asserteql( sizeof(MSG62_MOVE_STATIC), 15 );

struct MSG63_AREA_LOAD {
	u8 msgtype;
	u8 unk1_12[12];
};
asserteql( sizeof(MSG63_AREA_LOAD), 13 );

struct MSG64_AREA_LOAD_REQ {
	u8 msgtype;
};
asserteql( sizeof(MSG64_AREA_LOAD_REQ), 1 );

struct MSG65_WEATHER {
    u8 msgtype;
    u8 type;
    u8 severity;
    u8 aux;
    enum { TYPE_NONE = 0, TYPE_RAIN = 1, TYPE_SNOW = 2 };
	enum { //TODO:FIX//
		WTYPE_START_TO_RAIN = 0x00,
		WTYPE_FIERCE_STORM = 0x01,
		WTYPE_BEGIN_TO_SNOW = 0x02,
		WTYPE_STORM_BREWING = 0x03,
		WTYPE_STOP_WEATHER = 0xFF
	};
};
asserteql( sizeof(MSG65_WEATHER), 4 );

struct MSG69_UNKNOWN {
    u8 msgtype;
    u16 msglen;
};
/* variable length - don't know what this message is for. */

struct MSG6C_TARGET_CURSOR {
    u8 msgtype;
    u8 unk1;        // TARGET_CURSOR_CREATE / _DELETE ??
    u32 target_cursor_serial;
    u8 cursor_type;
    u32 selected_serial;
    u16 x;
    u16 y;
    u8 unk15;
    s8 z;
    u16 graphic;
    enum { UNK1_00, UNK1_01 };
    enum CURSOR_TYPE { CURSOR_TYPE_OBJECT = 0,
           CURSOR_TYPE_COORDINATE = 1,
    
           CURSOR_TYPE_NEUTRAL = 0,
           CURSOR_TYPE_HARMFUL = 1,
           CURSOR_TYPE_HELPFUL = 2 };
};
asserteql( sizeof(MSG6C_TARGET_CURSOR), 19 );
/*#define TARGET_CURSOR_CREATE 0x01
#define TARGET_CURSOR_DELETE 0x02*/

struct MSG6D_MUSIC
{
    u8 msgtype;
    u16 midi;
};
asserteql( sizeof(MSG6D_MUSIC), 3 );


struct MSG6E_DO_ACTION {
    u8 msgtype;
    u32 serial;
    u16 action;
    u8 unk7;
    u8 unk8_05;
    u8 unk9;
    u8 unk10_01;
    u8 unk11;
    u8 unk12;
    u8 unk13_01;
};
asserteql( sizeof(MSG6E_DO_ACTION), 14 );

/* 
Okay, this just sucks.  If, in a client, you click on someone with
the targetting cursor, and at that same moment (or even slightly thereafter!)
that person moves, the client seems to regularly get the "move" before 
registering your selection.  The end result is that you target whatever is
NOW under your cursor, rather than what WAS under it. 
On the plus side, it seems you can target an effect from an x/y/z coord
to another x/y/z coordinate.
*/

struct MSG70_SPELL_EFFECT {
    u8 msgtype;
    u8 effect_type;     // EFFECT_TYPE_xx
    u32 source_serial;
    u32 target_serial;
    u16 effect;
    u16 srcx;
    u16 srcy;
    u8 srcz;
    u16 dstx;
    u16 dsty;
    u8 dstz;
    u8 speed; /* was: 07 */
    u8 loop;    /* 1=shortest, 0=very long */
    u8 unk24;
    u8 unk25;
    u8 unk26;
    u8 explode;/* not sure what to put here */
};
asserteql( sizeof(MSG70_SPELL_EFFECT), 28 );
#define EFFECT_TYPE_MOVING          0x00
#define EFFECT_TYPE_LIGHTNING_BOLT  0x01 
#define EFFECT_TYPE_STATIONARY_XYZ  0x02
#define EFFECT_TYPE_FOLLOW_OBJ      0x03 

#define SPELL_EFFECT_FIREBALL   0x36D4
#define SPELL_EFFECT_EBOLT      0x379F
/* (speed, loop, explode) triplets:
    Magic Arrow: (5,0,1)
    Fireball: (5,0,1)
    Ebolt: (5,0,1)
*/

/* A Fireball, at a cow 
CLIENT: Cast Spell "Fireball" (18)
    Unexpected message type 0x12
    0000: 12 00 07 56 31 38 00                               ...V18.. ........
SERVER: Do Action
    Decrypted Server Data (0 bytes left): 
    0000: 6e 00 1b 90 50 00 10 00  07 00 01 00 00 00         n...P... ........
SERVER: Select Target
    Decrypted Server Data (2 bytes left): 
    0000: 6c 00 40 43 98 ca 01 00  00 00 00 00 00 00 00 00   l.@C.... ........
    0010: 00 00 00                                           ........ ........
CLIENT: Select Target
    Unexpected message type 0x6c
    0000: 6c 00 40 43 98 ca 01 00  7e b2 36 04 09 02 d3 00   l.@C.... ~.6.....
    0010: 01 00 e7                                           ........ ........
SERVER: stuff
    Decrypted Server Data (91 bytes left): 
    0000: 4e 00 1b 90 50 00                                  N...P... ........
    Decrypted Server Data (32 bytes left): - GOXYZ
    0000: 20 00 1b 90 50 01 90 00  83 ea 00 04 07 02 dd 00    ...P... ........
    0010: 00 00 01                                           ........ ........
    Decrypted Server Data (23 bytes left): - SOUND EFFECT 
    0000: 54 01 01 5e 00 00 04 07  02 dd 00 01               T..^.... ........
    Decrypted Server Data (0 bytes left): 
    0000: 70 00 00 1b 90 50 00 7e  b2 36 36 d4 04 07 02 dd   p....P.~ .66.....
    0010: 01 04 09 02 d3 01 07 00  00 00 00 01               ........ ........
    Decrypted Server Data (608 bytes left): - DO ACTION
    0000: 6e 00 8e b1 cb 00 12 00  07 00 01 00 00 01         n....... ........
    Decrypted Server Data (599 bytes left): - SOUND EFFECT
    0000: 54 01 02 3a 00 00 03 6c  03 1b 00 00               T..:...l ........
    
      An energy bolt, also at that same cow
CLIENT: Cast Spell "Corp Por"
    Unexpected message type 0x12
    0000: 12 00 07 56 34 32 00                               ...V42.. ........
SERVER: effects
    Decrypted Server Data (0 bytes left): 
    0000: 6e 00 1b 90 50 00 10 00  07 00 01 00 00 00         n...P... ........
SERVER: select target
    0000: 6c 00 40 2c f1 14 01 00  00 00 00 00 00 00 00 00   l.@,.... ........
    0010: 00 00 00                                           ........ ........
CLIENT: select target
    Unexpected message type 0x6c
    0000: 6c 00 40 2c f1 14 01 00  7e b2 36 04 07 02 d5 00   l.@,.... ~.6.....
    0010: 01 00 e7                                           ........ ........
SERVER: stuff
    Decrypted Server Data (100 bytes left): - GOXYZ
    0000: 20 00 1b 90 50 01 90 00  83 ea 00 04 07 02 dd 00    ...P... ........
    0010: 00 00 01                                           ........ ........

    Decrypted Server Data (98 bytes left): 
    0000: 4f 00                                              O....... ........

    Decrypted Server Data (92 bytes left): 
    0000: 4e 00 1b 90 50 00                                  N...P... ........

    Decrypted Server Data (32 bytes left): 
    0000: 20 00 1b 90 50 01 90 00  83 ea 00 04 07 02 dd 00    ...P... ........
    0010: 00 00 01                                           ........ ........

    Decrypted Server Data (9 bytes left): 
    0000: 70 00 00 1b 90 50 00 7e  b2 36 37 9f 04 07 02 dd   p....P.~ .67.....
    0010: 01 04 07 02 d5 01 07 00  00 00 00 01               ........ ........

    Decrypted Server Data (0 bytes left): 
    0000: 54 01 02 0a 00 00 04 07  02 dd 00 01               T....... ........
*/

struct MSG71_BULLETIN_BOARD {
	// One of these...
	struct HEADER {
		u8 msgtype;
		u16 msglen;
		u8 subcmd;
	};

	enum SUBCMDs {
		OUT_0_DISPLAY_BOARD = 0,
		OUT_1_MESSAGE_SUMMARY = 1,
		OUT_2_MESSAGE = 2,
		IN_3_REQ_MESSAGE = 3,
		IN_4_REQ_MESSAGE_SUMMARY = 4,
		IN_5_POST_MESSAGE = 5,
		IN_6_REMOVE_MESSAGE = 6
	};

	// Followed by a sub-cmd block...
	struct SUBCMD_OUT_0_DISPLAY_BOARD {
		u32 board_id;
		char name[22];
		u32 id;		// always 0x402000FF ?
		u32 unk_00; // always 0x00
	};

	struct SUBCMD_OUT_1_MESSAGE_SUMMARY {
		u32 board_id;
		u32 msg_id;
		u32 parent_id; // 0 if top-level
		struct POSTER {
			u8 post_len;
			char poster[1]; // post_len bytes long (null term)
		};
		struct SUBJECT {
			u8 subj_len;
			char subject[1]; // subj_len bytes long (null term)
		};
		struct TIME {
			u8 time_len;
			char time[1]; // time_len bytes long (null term)
		};
	};

	struct SUBCMD_OUT_2_MESSAGE {
		u32 board_id;
		u32 msg_id;
		struct POSTER {
			u8 post_len;
			char poster[1]; // post_len bytes long (NULL term)
		};
		struct SUBJECT {
			u8 subj_len;
			char subject[1]; // subj_len bytes long (null term)
		};
		struct TIME {
			u8 time_len;
			char time[1]; // time_len bytes long (null term)
		};
		struct LINE_HEADER {
			// 01 91 84 0A 06 1E FD 01 0B 15 2E 01 0B 17 0B
			// 01 BB 20 46 04 66 13 F8 00 00 0E 75 00 00
			u8 constant_hdr[29]; // use MSG71_SUBCMD_2_CONSTANT to initialise
			u8 numlines;
		};
		struct LINES {
			u8 line_len;
			char body[1]; // line_len bytes long (null term)
		};
	};

	struct SUBCMD_IN_3_REQ_MESSAGE {
		u32 board_id;
		u32 msg_id;
	};

	struct SUBCMD_IN_4_REQ_MESSAGE_SUMMARY {
		u32 board_id;
		u32 msg_id;
	};

	struct SUBCMD_IN_5_POST_MESSAGE {
		u32 board_id;
		u32 reply_id; // 0 for top-level or if not a reply
		struct SUBJECT {
			u8 subj_len;
			char subject[1]; // subj_len bytes long (null term)
		};
		struct LINE_HEADER {
			u8 numlines;
		};
		struct LINES {
			u8 line_len;
			char body[1]; // line_len bytes long (null term)
		};
	};

	struct SUBCMD_IN_6_REMOVE_MESSAGE {
		u32 board_id;
		u32 msg_id;
	};
};
asserteql( sizeof(MSG71_BULLETIN_BOARD::HEADER), 4 );
asserteql( sizeof(MSG71_BULLETIN_BOARD::SUBCMD_OUT_0_DISPLAY_BOARD), 34 );
asserteql( sizeof(MSG71_BULLETIN_BOARD::SUBCMD_OUT_1_MESSAGE_SUMMARY::POSTER), 2);
asserteql( sizeof(MSG71_BULLETIN_BOARD::SUBCMD_OUT_1_MESSAGE_SUMMARY::SUBJECT), 2);
asserteql( sizeof(MSG71_BULLETIN_BOARD::SUBCMD_OUT_1_MESSAGE_SUMMARY::TIME), 2);
asserteql( sizeof(MSG71_BULLETIN_BOARD::SUBCMD_OUT_2_MESSAGE::POSTER), 2);
asserteql( sizeof(MSG71_BULLETIN_BOARD::SUBCMD_OUT_2_MESSAGE::SUBJECT), 2);
asserteql( sizeof(MSG71_BULLETIN_BOARD::SUBCMD_OUT_2_MESSAGE::TIME), 2);
asserteql( sizeof(MSG71_BULLETIN_BOARD::SUBCMD_OUT_2_MESSAGE::LINE_HEADER), 30);
asserteql( sizeof(MSG71_BULLETIN_BOARD::SUBCMD_OUT_2_MESSAGE::LINES), 2);
asserteql( sizeof(MSG71_BULLETIN_BOARD::SUBCMD_IN_3_REQ_MESSAGE), 8);
asserteql( sizeof(MSG71_BULLETIN_BOARD::SUBCMD_IN_4_REQ_MESSAGE_SUMMARY), 8);
asserteql( sizeof(MSG71_BULLETIN_BOARD::SUBCMD_IN_5_POST_MESSAGE::SUBJECT), 2);
asserteql( sizeof(MSG71_BULLETIN_BOARD::SUBCMD_IN_5_POST_MESSAGE::LINE_HEADER), 1);
asserteql( sizeof(MSG71_BULLETIN_BOARD::SUBCMD_IN_5_POST_MESSAGE::LINES), 2);
asserteql( sizeof(MSG71_BULLETIN_BOARD::SUBCMD_IN_6_REMOVE_MESSAGE), 8);

#define MSG71_SUBCMD_2_CONSTANT {0x01,0x91,0x84,0x0A,0x06,0x1E,0xFD,0x01,0x0B,0x15,0x2E,0x01,0x0B,0x17,0x0B,0x01,0xBB,0x20,0x46,0x04,0x66,0x13,0xF8,0x00,0x00,0x0E,0x75,0x00,0x00}


struct MSG72_MODE_SET {
    u8 msgtype;
    u8 warmode; /* 00 = not, 01 = war mode */
    u8 unk2;
    u8 unk3_32;
    u8 unk4;
};
asserteql( sizeof(MSG72_MODE_SET), 5 );

struct MSG73_KEEP_ALIVE {
    u8 msgtype;
    u8 unk1;
};
asserteql( sizeof(MSG73_KEEP_ALIVE), 2 );


/*
0000: 75 00 00 00 0e 45 6c 6d  65 72 00 00 00 00 00 00   u....Elm er......
0010: 00 01 00 00 00 00 00 00  00 98 3d 4b 00 00 00 00   ........ ..=K....
0020: 00 00 00                                           ........ ........
*/
struct MSG75_RENAME_CHAR
{
    u8 msgtype;
    u32 serial;
    char name[30]; // always null-terminated, or at least the client is supposed to.
};
asserteql( sizeof(MSG75_RENAME_CHAR), 35 );

struct MSG77_MOVE {
    u8 msgtype;
    u32 serial;
    u16 graphic;
    u16 x;
    u16 y;
    u8 z;
    u8 dir; 
    u16 skin;
    u8 flag1; /* bit 0x80 seems to be invis (and self hidden), bit 0x40 is war mode */
    u8 hilite; /* only 1 to 7 seem to be valid. any other value in the entire byte causes white highlighting */
};
asserteql( sizeof(MSG77_MOVE), 17 );
#define DIR_FLAG_RUN 0x80

/* Notes on 'dir': bit 0x80 is 'run' bit.  Mask 0x78 must be left zero or the directions
will display wrong.  Only mask 0x7 is used, for facing. 
*/


#define CHAR_HILITE_INNOCENT    0x01
#define CHAR_HILITE_FRIEND      0x02
#define CHAR_HILITE_ATTACKABLE  0x03
#define CHAR_HILITE_CRIMINAL    0x04
#define CHAR_HILITE_ENEMY       0x05
#define CHAR_HILITE_MURDERER    0x06
#define CHAR_HILITE_INVUL       0x07

#include "layers.h"

struct MSG78_OWN_CREATE {
    u8 msgtype;
    u16 msglen;     
    u32 serial;
    u16 graphic;
    u16 x;
    u16 y;
    u8  z;
    u8  facing;     /* WALK_DIR_x */
    u16 skin;       /* skin color? */
    u8 flag1;
    u8 hilite;
    u8 wornitems[ 9 * HIGHEST_LAYER + 4 ]; 
}; 

struct MSGPART_78_OBJINFO_NOCOLOR {
    u32 serial;
    u16 graphic;
    u8 layer;
};
asserteql( sizeof( MSGPART_78_OBJINFO_NOCOLOR ), 7 );

struct MSGPART_78_OBJINFO_WITHCOLOR {
    u32 serial;
    u16 graphic;
    u8 layer;
    u16 color;
};
asserteql( sizeof( MSGPART_78_OBJINFO_WITHCOLOR ), 9 );
//asserteql( sizeof(MSG78_OWN_CREATE), 0x17 );
/*
  FIXME: msg type 78 is incompletely understood.  I believe item/clothing data can be sent in it.
  NOTE on wornitems: each layer is sent.  The format seems to be:
  SERIAL OBJTYPE LAYER {Optional Color}
    4      2        1       2
  when there are no more, inject 4 zeros into wornitems.
  I think the high bit of OBJTYPE indicates whether or not color is present.
  For our purposes, if the item has a nonzero color, we will send it.
  I'm not sure if the tiledata has a bit to indicate whether color goes along or not.

*/

// message 7C:
struct MSG7C_ITEM_SELECT_MENU_HEAD {
    u8 msgtype;
    u16 msglen;
    u32 used_item_serial;
    u16 menu_id;
};

struct MSG7C_ITEM_SELECT_MENU_TITLE {
    u8 titlelen;
    char title[ 80 ]; /* don't know what this should be */
};

// between the title and the elements is a one-byte # of elements 
struct MSG7C_ITEM_SELECT_MENU_ITEM_COUNT {
    u8 item_count;
};

struct MSG7C_ITEM_SELECT_MENU_ELEM {
    u16 graphic;
    u16 color; // ?? they were all grey.
    u8 desclen;
    char desc[ 80 ]; // also not sure what the max is
};

/* An example 7C message:
0000: 7c 00 3b 40 59 df cd 00  11 12 43 68 6f 6f 73 65   |.;@Y... ..Choose

0010: 20 61 20 63 61 74 65 67  6f 72 79 2e 
                                           03 15 17 00    a categ ory.....

0020: 00 06 73 68 69 72 74 73  
                               15 39 00 00 05 70 61 6e   ..shirts .9...pan

0030: 74 73 
            15 3d 00 00 04 6d  69 73 63                  ts.=...m isc.....


*/

struct MSG7D_MENU_SELECTION {
    u8 msgtype;
    u32 used_item_serial;
    u16 menu_id;
    u16 choice;
    u16 graphic;
    u16 color; // ?? maybe, has always been 0
};

struct MSG80_LOGINSERVER_LOGIN {
    u8 msgtype;
    char name[ 30 ];
    char password[ 30 ];
    u8 unk;
};
asserteql( sizeof(MSG80_LOGINSERVER_LOGIN), 62 );

struct MSG82_LOGIN_ERROR {
    u8 msgtype;
    u8 error;
};
asserteql( sizeof(MSG82_LOGIN_ERROR), 2 );

#define LOGIN_ERROR_NO_ACCOUNT       0x00
#define LOGIN_ERROR_OTHER_CHAR_INUSE 0x01
#define LOGIN_ERROR_ACCOUNT_BLOCKED  0x02
#define LOGIN_ERROR_WRONG_PASSWORD   0x03
// 04 "some problem communicating with Origin.  Please restart Ultima Online and try again."
// 05 same
#define LOGIN_ERROR_MISC             0x06


struct MSG83_DELETE_CHARACTER {
    u8 msgtype;
    u8 password[30];
    u32 charidx;
    u32 clientip;
};
asserteql( sizeof(MSG83_DELETE_CHARACTER), 39 );

struct MSG88_PAPERDOLL {
    u8 msgtype;
    u32 serial;
    u8 text[ 60 ];
    u8 flag1;     // 01=warmode, 00=not warmode
};
asserteql( sizeof(MSG88_PAPERDOLL), 66 );

// MSG89_SHOW_CORPSE: after all layers inserted, set the 'layer' value
// in the next array elem to 0.  length should include this byte.
// Example, for an empty corpse: 89 00 08 40 75 13 f9 00  
struct MSG89_SHOW_CORPSE {
    u8 msgtype;
    u16 msglen;
    u32 serial;
    struct {
        u8 layer;
        u32 serial;
    } layers[ NUM_LAYERS +1 ];
};

struct MSG8C_LOGIN3 {
    u8 msgtype;
    u8 ip[4];
    u16 port;
    u8 unk7_00;
    u8 unk8_03;
    u8 unk9_C3;
    u8 unk10_4B;
};
asserteql( sizeof(MSG8C_LOGIN3), 11 );

struct MSG90_MAP
{
    u8 msgtype;
    u32 serial;
    u8 unk5_13;
    u8 unk6_9d;
    u16 x1;
    u16 y1;
    u16 x2;
    u16 y2;
    u16 xsize;
    u16 ysize;
};
asserteql( sizeof(MSG90_MAP), 19 );

struct MSG91_GAMESERVER_LOGIN {
    u8 msgtype;
    u8 unk1, unk2, unk3, unk4;
    char name[30];
    char password[30];
};
asserteql( sizeof(MSG91_GAMESERVER_LOGIN), 65 );



struct MSG95_DYE_VAT 
{
    u8 msgtype;
    u32 serial;
    u16 unk;
    u16 graphic_or_color; 
} POSTSTRUCT_PACK;
asserteql( sizeof(MSG95_DYE_VAT), 9 );

struct MSG98_ALLNAMES
{
    u8 msgtype;
    u16 msglen;
    u32 serial;
};
asserteql( sizeof(MSG98_ALLNAMES), 7 );

struct MSGOUT_98_ALLNAMES
{
    u8 msgtype;
    u16 msglen;
    u32 serial;
    char name[30];
};
asserteql( sizeof(MSGOUT_98_ALLNAMES), 37 );

struct MSG99_MULTI_PLACEMENT {
    u8 msgtype;
    u8 unk1_01;
    u32 deed;
    u8 unk_x06_x11[12];
    u16 graphic;
    u16 xoffset;
	u16 yoffset;
	u16 maybe_zoffset;
};
asserteql( sizeof(MSG99_MULTI_PLACEMENT), 26 );

struct MSG9A_PROMPT
{
    u8 msgtype;
    u16 msglen;
    u32 serial;
    u32 unk;
    u32 unk2;
    char text[1];
};
asserteql( sizeof(MSG9A_PROMPT), 16 );


struct MSG9B_HELP
{
    u8 msgtype;
    u8 unk[ 0x101 ];
};
asserteql( sizeof(MSG9B_HELP), 258 );


struct MSGA0_SERVER_SELECT {
    u8 msgtype;
    u16 servernum;
};
asserteql( sizeof(MSGA0_SERVER_SELECT), 3 );

struct MSGA1_UPDATE_STATS_ST_HP 
{
    u8 msgtype;
    u32 serial;
    u16 max_hits; // str
    u16 hits;
};
asserteql( sizeof(MSGA1_UPDATE_STATS_ST_HP), 9 );

struct MSGA2_UPDATE_STATS_IN_MN {
    u8 msgtype;
    u32 serial;
    u16 max_mana; // intel;
    u16 mana;
};
asserteql( sizeof(MSGA2_UPDATE_STATS_IN_MN), 9 );

struct MSGA3_UPDATE_STATS_DX_STM {
    u8 msgtype;
    u32 serial;
    u16 max_stamina; // dex;
    u16 stamina;
};
asserteql( sizeof(MSGA3_UPDATE_STATS_DX_STM), 9 );

struct MSGA6_TIP_WINDOW {
    u8 msgtype;
    u16 msglen;
    u8 type;
    u8 unk4;
    u8 unk5;
    u16 tipnum;
    u16 textlen;
    char text[10000];
    enum { TYPE_TIP = 0,
           TYPE_UPDATE = 1 };
};
asserteql( sizeof(MSGA6_TIP_WINDOW), 10010);

struct MSGA7_GET_TIP {
    u8 msgtype;
    u16 lasttip;
    u8 prevnext; // 0=prev 1=next
};
asserteql( sizeof(MSGA7_GET_TIP), 4 );



/* Message A8, the Server List message: a HEAD followed by 0 or more ELEMS. */
struct MSGA8_SERVER_LIST_HEAD {
    u8 msgtype;
    u16 msglen;
    u8 unk3_FF;
    u16 servcount;
};
asserteql( sizeof(MSGA8_SERVER_LIST_HEAD), 6 );

struct MSGA8_SERVER_LIST_ELEM {
    u16 servernum;
    char servername[30]; // think the last two bytes are the sorting amounts.
    u16 servernum2;
    u8 percentfull;
    s8 timezone;
    char ip[4];
};
asserteql( sizeof(MSGA8_SERVER_LIST_ELEM), 40 );
/****************************************/

/* Message A9, the Startup message:
        1 START_HEAD
        5 CHAR_LIST_ELEMs
        1 START_COUNT
        START_COUNT START_ELEMs
        */
struct MSGA9_START_HEAD {
    u8 msgtype;
    u16 msglen;
    u8 numchars;
};
asserteql( sizeof(MSGA9_START_HEAD), 4 );

// I don't think all of this is the name.  At on point, some garbage was
// being left in the structure, and a password was prompted for - seems
// one of these bytes is a password flag.
struct MSGA9_CHAR_LIST_ELEM {
    char name[60];
};
asserteql( sizeof(MSGA9_CHAR_LIST_ELEM), 60 );

struct MSGA9_START_COUNT {
    u8 startcount;
};
asserteql( sizeof(MSGA9_START_COUNT), 1 );

struct MSGA9_START_ELEM {
    u8 startnum;
    char city[31];
    char desc[31];
};
asserteql( sizeof(MSGA9_START_ELEM), 63 );

struct MSGA9_START_FLAGS {
	u32 flags;
	enum {
		FLAG_UNK_0x0001             = 0x0001, // ?
		FLAG_SEND_CONFIG_REQ_LOGOUT = 0x0002, // Sends Configuration to Server
		FLAG_SINGLE_CHARACTER_SIEGE = 0x0004, // ?
		FLAG_ENABLE_NPC_POPUP_MENUS = 0x0008, // Enable NPC PopUp Menus
		FLAG_SINGLE_CHARACTER       = 0x0010, // Use 1 Character only
		FLAG_AOS_FEATURES           = 0x0020, // Age of Shadows
		FLAG_UPTO_SIX_CHARACTERS    = 0x0040, // Use up to 6 Characters (not only 5)
		FLAG_SE_FEATURES            = 0x0080, // Samurai Empire
		FLAG_ML_FEATURES            = 0x0100  // Mondain's Legacy
	};
};
/****************************************/

/* AA message - Sets Opponent - the always-highlighted being.
    Setting to 0 clears.
    Sent to you when you die (serial=0), or when others target you if you
      have noone targetted.
*/
struct MSGAA_SET_OPPONENT {
    u8 msgtype;
    u32 serial;
};
asserteql( sizeof(MSGAA_SET_OPPONENT), 5 );

struct MSGAB_TEXTENTRY_GUMP
{
    struct HEADER {
        u8 msgtype;
        u16 msglen;
        u32 serial;
        u8 type;
        u8 index;
    };
    struct TEXTLINE1 {
        u16 numbytes;
        char text[256]; // null-terminated
    };
    struct TEXTLINE2 {
        u8 cancel; enum { CANCEL_DISABLE, CANCEL_ENABLE };
        u8 style; enum { STYLE_DISABLE, STYLE_NORMAL, STYLE_NUMERICAL };
        u32 mask; 
        u16 numbytes;
        char text[256]; // null-terminated
    };

};
asserteql( sizeof(MSGAB_TEXTENTRY_GUMP::HEADER), 9 );
asserteql( sizeof(MSGAB_TEXTENTRY_GUMP::TEXTLINE1), 258 );
asserteql( sizeof(MSGAB_TEXTENTRY_GUMP::TEXTLINE2), 264 );

struct MSGAC_TEXTENTRY_RESULT
{
    u8 msgtype;
    u16 msglen;
    u32 serial;
    u8 type;
    u8 index;
    u8 retcode; enum { RETCODE_CANCELED, RETCODE_OKAY };
    u16 datalen;
    char data[1];
};
asserteql( sizeof(MSGAC_TEXTENTRY_RESULT), 13 );

struct MSGAF_DISPLAY_DEATH_ACTION {
    u8 msgtype;
    u32 player_id;
    u32 corpse_id;
    u32 unk4_zero; // 0x00000000
};
asserteql( sizeof(MSGAF_DISPLAY_DEATH_ACTION), 13 );



struct MSGB0_GUMP_DIALOG
{
// one of these:
    struct HEADER {
        u8 msgtype;
        u16 msglen;
        u32 serial;
        u32 dialogid;
        u32 x;
        u32 y;
    };
// followed by one of these (variable length):
    struct LAYOUT {
        u16 len;
        char text[ 1 ]; // null-terminated
    };
// followed by one of these:
    struct DATA_HEADER {
        u16 numlines;
    };
// followed by zero or more (numlines, above) of these:
    struct DATA {
        u16 len;
        char text[ 1 ];
    };
};
asserteql( sizeof(MSGB0_GUMP_DIALOG::HEADER), 19 );
asserteql( sizeof(MSGB0_GUMP_DIALOG::LAYOUT), 3 );
asserteql( sizeof(MSGB0_GUMP_DIALOG::DATA_HEADER), 2 );
asserteql( sizeof(MSGB0_GUMP_DIALOG::DATA), 3 );

struct MSGB1_GUMP_BUTTON
{
    struct HEADER {
        u8 msgtype;
        u16 msglen;
        u32 serial;
        u32 dialogid;
        u32 gumpid;
    };
    struct INTS_HEADER
    {
        u32 count;
    };
    struct INT_ENTRY // FIXME bad name
    {
        u32 value;
    };
    struct STRINGS_HEADER
    {
        u32 count;
    };
    struct STRING_ENTRY
    {
        u16 tag;
        u16 length;
        u8 data[1]; // !@#!# unicode
    };
};
asserteql( sizeof(MSGB1_GUMP_BUTTON::HEADER), 15 );
asserteql( sizeof(MSGB1_GUMP_BUTTON::INTS_HEADER), 4 );
asserteql( sizeof(MSGB1_GUMP_BUTTON::INT_ENTRY), 4 );
asserteql( sizeof(MSGB1_GUMP_BUTTON::STRINGS_HEADER), 4 );
asserteql( sizeof(MSGB1_GUMP_BUTTON::STRING_ENTRY), 5 );

struct MSGE3_KR_VERIFIER {
    u8  msgtype;
    u16 msglen;
    u32 unknown_A_length; //  3 Bytes
	u8  unknown_A[3];
    u32 unknown_B_length; // 19 Bytes
	u8  unknown_B[19];
    u32 unknown_C_length; // 16 Bytes
	u8  unknown_C[16];
	u32 unknown_D;
    u32 unknown_E_length; // 16 Bytes
	u8  unknown_E[16];
};
asserteql( sizeof(MSGE3_KR_VERIFIER), 77 );

#define RESPONSE_MAX_LEN 200
struct MSGE4_KR_VERIFIER_RESPONSE {
    u8  msgtype;
    u16 msglen;
    u32 unknown_length; // 16 Bytes
	u8  unknown[RESPONSE_MAX_LEN];
};

#ifdef _MSC_VER     /* Visual C++ 4.0 + */
#pragma pack( pop )
#else
#pragma pack()
#endif

#endif
