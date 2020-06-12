// History
//   2006/05/07 Shinigami: SendAOSTooltip - will now send merchant_description() if needed

#ifndef __UFUNC_H
#define __UFUNC_H

// this is for NULLs found..
#include <stddef.h>

#include "clib/rawtypes.h"

#include "uconst.h"

const u32 CHARACTERSERIAL_START = 0x00000001Lu;
const u32 CHARACTERSERIAL_END = 0x3FFFFFFFLu;
const u32 ITEMSERIAL_START = 0x40000000Lu;
const u32 ITEMSERIAL_END = 0x7FffFFffLu;

extern u32 itemserialnumber;
extern u32 nonsaveditemserialnumber;
extern u32 charserialnumber;

class UObject;
class Client;
class Character;
class Item;
struct USTRUCT_TILE;
class Menu;
class UContainer;
class NPC;
class UMulti;
class Realm;

void SetCurrentItemSerialNumber(u32 serial);
void SetCurrentCharSerialNumber(u32 serial);
u32 GetNextSerialNumber();
u32 GetCurrentItemSerialNumber();
u32 GetCurrentCharSerialNumber();
u32 UseCharSerialNumber( u32 serial );

u32 GetNewItemSerialNumber();
u32 UseItemSerialNumber( u32 serial ); // announce this is in use, don't reallocate

/* Reason is a LOGIN_ERROR_xxx */
void send_login_error( Client *client, unsigned char reason );

bool inrangex( const Character* c1, const Character* c2, int maxdist );
bool inrangex( const UObject *c1, unsigned short x, unsigned short y, int maxdist );
bool inrange( const Character* c1, const Character* c2 );
//int inrange( const Character* c1, const Item* item );
bool inrange( const Character* c1, const UObject* obj );
bool inrange( unsigned short x1, unsigned short y1,
			  unsigned short x2, unsigned short y2 );
bool multi_inrange( const Character *c1, const UMulti *obj );
bool multi_inrange( unsigned short x1, unsigned short y1,
             unsigned short x2, unsigned short y2 );

unsigned short pol_distance( const Character* c1, const UObject* obj );
unsigned short pol_distance( unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2 );

bool in_say_range( const Character *c1, const Character *c2 );
bool in_yell_range( const Character *c1, const Character *c2 );
bool in_whisper_range( const Character *c1, const Character *c2 );

void send_owncreate( Client *client, const Character *chr );
void send_item( Client *client, const Item *item );
void send_wornitem( Client *client, const Character *chr, const Item *item );
// obsoleted: void send_all_worn_items( Client *client, const Character *chr );
void send_move( Client *client, const Character *chr );
void send_move_if_inrange( Client *client, const Character *chr );
void send_objdesc( Client *client, Item *item );

void send_item_to_inrange( const Item *item );
void update_item_to_inrange( const Item* item );
void send_put_in_container( Client* client, const Item* item );
void send_put_in_container_to_inrange( const Item *item );
void send_wornitem_to_inrange( const Character *chr, const Item *item );

void send_midi( Client* client, unsigned short midi );
//Item *find_item( RArray<Item>& items, u32 serial, unsigned *posn = NULL );
Character *find_character( u32 serial );

UObject* find_toplevel_object( u32 serial );
void setrealm(Item* item, void* arg);

void send_remove_character( Client *client, const Character *chr );
void send_remove_object_if_inrange( Client *client, const Item *obj ); 
void send_remove_object( Client *client, const Item *obj);

void send_remove_character_to_nearby( const Character* chr );
void send_remove_character_to_nearby_cantsee( const Character* chr );
void send_char_data_to_nearby_ghosts( Character* chr );


void send_goxyz( Client *client, const Character *chr );
void send_light( Client *client, int lightlevel );
void send_weather( Client *client, 
                   unsigned char type, 
                   unsigned char severity,
                   unsigned char aux );
void send_mode( Client *client );
void send_techstuff( Client *client );
void send_item_move_failure( Client *client, u8 reason ); // ITEM_MOVE_FAILURE_xxx
u16 convert_objtype_to_spellnum( u16 objtype, int school );

void send_stamina_level( Client *client );
void send_mana_level( Client *client );

//void send_pause( Client *client );
//void send_restart( Client *client );
UContainer *find_legal_container( const Character *chr, u32 serial );
bool is_a_parent( const Item *item, u32 serial );
void play_sound_effect( const UObject *center, u16 effect );
void play_sound_effect_private( const UObject *center, u16 effect, Character* forchr );
void play_lightning_bolt_effect( const UObject* center );
void play_moving_effect( const UObject *src, const UObject *dst,
						 u16 effect,
						 u8 speed,
						 u8 loop,
						 u8 explode,
						 Realm* realm );
void play_moving_effect2( u16 xs, u16 ys, s8 zs,
                          u16 xd, u16 yd, s8 zd,
                          u16 effect,
                          u8 speed,
                          u8 loop,
                          u8 explode,
						  Realm* realm );
void play_object_centered_effect( const UObject* center,
                                  u16 effect,
                                  u8 speed,
                                  u8 loop );

void play_stationary_effect( u16 x, u16 y, u8 z, u16 effect, u8 speed, u8 loop, u8 explode, Realm* realm );

// find_legal_item: search worn items, including backpack recursively, and
// items on the ground, recursively, for an item of a given serial.
Item *find_legal_item( const Character *chr, u32 serial, bool* additlegal = NULL, bool* isRemoteContainer = NULL );

const unsigned short DEFAULT_TEXT_FONT  = 3;
const unsigned short DEFAULT_TEXT_COLOR = 0x3B2;

void send_sysmessage(Client *client,                // System message (lower left corner)
                     const char *text,
                     unsigned short font = DEFAULT_TEXT_FONT,
                     unsigned short color = DEFAULT_TEXT_COLOR );
void send_sysmessage(Client *client,				// Unicode System message (lower left)
					 const u16 *wtext, const char lang[4],
					 unsigned short font = DEFAULT_TEXT_FONT,
					 unsigned short color = DEFAULT_TEXT_COLOR );
void broadcast( const char *text,
                unsigned short font = DEFAULT_TEXT_FONT,
                unsigned short color = DEFAULT_TEXT_COLOR ); // send_sysmessage to all
void broadcast( const u16 *wtext, const char lang[4],
                unsigned short font = DEFAULT_TEXT_FONT,
                unsigned short color = DEFAULT_TEXT_COLOR ); // Unicode send_sysmessage to all
bool say_above( const UObject* obj, 
                const char* text,
                unsigned short font = DEFAULT_TEXT_FONT,
                unsigned short color = DEFAULT_TEXT_COLOR );
bool say_above( const UObject* obj, 
                const u16 *wtext, const char lang[4],
                unsigned short font = DEFAULT_TEXT_FONT,
                unsigned short color = DEFAULT_TEXT_COLOR );

bool private_say_above( Character* chr, 
                        const UObject* obj, 
                        const char *text,
                        unsigned short font = DEFAULT_TEXT_FONT,
                        unsigned short color = DEFAULT_TEXT_COLOR );
bool private_say_above( Character* chr, 
                        const UObject* obj, 
                        const u16 *wtext, const char lang[4],
                        unsigned short font = DEFAULT_TEXT_FONT,
                        unsigned short color = DEFAULT_TEXT_COLOR );

Item *copy_item( const Item *src_item );
void update_all_weatherregions();

bool send_menu( Client *client, Menu *menu );
void send_death_message( Character *chr_died, Item *corpse );
void send_container_contents( Client *client, const UContainer& cont, bool show_invis = false );

/* send_char_data: called once for each client when a new character enters
   the world. */
void send_char_data( Client *client, Character *chr );

void for_nearby_npcs( void (*f)(NPC& npc, Character *chr, const char *text, int textlen, u8 texttype),
                      Character *p_chr, const char *p_text, int p_textlen, u8 texttype );
void for_nearby_npcs( void (*f)(NPC& npc, Character *chr, const char *text, int textlen, u8 texttype, const u16 *wtext, const char lang[4], int wtextlen),
                      Character *p_chr, const char *p_text, int p_textlen, u8 texttype, const u16 *p_wtext, const char p_lang[4], int p_wtextlen );

void transmit_to_inrange( const UObject* center, const void* msg, unsigned msglen );
void transmit_to_others_inrange( Character* center, const void* msg, unsigned msglen );

void destroy_item( Item* item );
void world_delete( UObject* uobj );

enum UFACING;
void move_item( Item* item, UFACING facing );
void move_item( Item* item, unsigned short newx, unsigned short newy, signed char newz, Realm* oldrealm );
void move_boat_item( Item* item, unsigned short newx, unsigned short newy, signed char newz, Realm* oldrealm );

void send_char_if_newly_inrange( Character *chr, Client *client	);
void send_item_if_newly_inrange( Item *item, Client *client );
void send_objects_newly_inrange( Client* client );

void send_create_mobile_if_nearby_cansee( Client* client, const Character* chr );
void send_create_mobile_to_nearby_cansee( const Character* chr );

void send_move_mobile_if_nearby_cansee( Client* client, const Character* chr );
void send_move_mobile_to_nearby_cansee( const Character* chr );

void send_remove_character_to_nearby_cansee( const Character* chr );

void send_open_gump( Client *client, const UContainer& cont );

class UMulti;
void send_multis_newly_inrange( UMulti *multi, Client *client ); 

Character* chr_from_wornitems( UContainer* wornitems );

void register_with_supporting_multi( Item* item );

Character* UpdateCharacterWeight(Item* item); //Dave
bool clientHasCharacter(Client* c); //Dave
void login_complete(Client* c); //Dave
void send_feature_enable(Client* client);
void send_realm_change( Client* client, Realm* realm );
void send_map_difs( Client* client );
void send_season_info( Client* client );
void send_new_subserver( Client* client );
#endif
