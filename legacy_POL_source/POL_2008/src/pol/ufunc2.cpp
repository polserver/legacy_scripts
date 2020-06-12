#include "clib/stl_inc.h"
#include <string.h>

#include "clib/endian.h"
#include "clib/clib.h"

#include "client.h"
#include "sockio.h"
#include "menu.h"
#include "tooltips.h"
#include "ufunc.h"
#include "umsgtype.h"
#include "uworld.h"

static unsigned char bfr[ 2000 ];

bool send_menu( Client *client, Menu *menu )
{
	// build up the message so it gets sent as a unit.
	if (menu->menuitems_.size() > 255)
        return false;

    unsigned short msglen = 0;
	MSG7C_ITEM_SELECT_MENU_HEAD *head = (MSG7C_ITEM_SELECT_MENU_HEAD *) bfr;
	head->msgtype = MSGOUT_7C_ITEM_SELECT_MENU;
	head->msglen = 0;
	head->used_item_serial = 0;
	head->menu_id = ctBEu16( menu->menu_id );
	
	MSG7C_ITEM_SELECT_MENU_TITLE *title= (MSG7C_ITEM_SELECT_MENU_TITLE *) (head+1);
	strzcpy( title->title, menu->title, sizeof title->title );
	title->titlelen = strlen( title->title ); // NOTE null-term not included!

	MSG7C_ITEM_SELECT_MENU_ITEM_COUNT *countpart = (MSG7C_ITEM_SELECT_MENU_ITEM_COUNT *)
												&title->title[ title->titlelen ];

    countpart->item_count = (u8) menu->menuitems_.size();


	MSG7C_ITEM_SELECT_MENU_ELEM *elem = (MSG7C_ITEM_SELECT_MENU_ELEM *) (countpart+1);
    msglen = (unsigned short) (((unsigned char *) elem) - bfr);
	for( unsigned idx = 0; idx < menu->menuitems_.size(); idx++ )
	{
        if (msglen + sizeof(*elem) > sizeof bfr )
            return false;
		MenuItem* mi = &menu->menuitems_[ idx ];

		elem->graphic = ctBEu16( mi->graphic_ );
		elem->color = ctBEu16( mi->color_ );
		strzcpy( elem->desc, mi->title, sizeof elem->desc );
		elem->desclen = strlen( elem->desc );
		elem = (MSG7C_ITEM_SELECT_MENU_ELEM *) &elem->desc[ elem->desclen ]; // NOTE null-term not included!
        msglen = (unsigned short) (((unsigned char *) elem) - bfr);
	}
	head->msglen = ctBEu16( msglen );
	
	transmit( client, bfr, msglen );
    return true;
}

#include "npc.h"
#include "uvars.h"
//DAVE added texttype, ranges
void for_nearby_npcs( void (*f)(NPC& npc, Character *chr, const char *text, int textlen, u8 texttype),
                      Character *p_chr, const char *p_text, int p_textlen, u8 texttype )
{
    int range;
    switch( texttype )
    {
    case TEXTTYPE_WHISPER:  range = RANGE_WHISPER;  break;
    case TEXTTYPE_YELL:     range = RANGE_YELL;     break;
    default:                range = RANGE_SAY;      break;
    }

    unsigned short wxL, wyL, wxH, wyH;
    zone_convert_clip( p_chr->x - range, p_chr->y - range, p_chr->realm, wxL, wyL );
    zone_convert_clip( p_chr->x + range, p_chr->y + range, p_chr->realm, wxH, wyH );
    for( unsigned short wx = wxL; wx <= wxH; ++wx )
    {
        for( unsigned short wy = wyL; wy <= wyH; ++wy )
        {
            ZoneCharacters& wchr = p_chr->realm->zone[wx][wy].characters;

            for( ZoneCharacters::iterator itr = wchr.begin(), end = wchr.end(); itr != end; ++itr )
            {
                Character* chr = *itr;

                if (chr->isa(UObject::CLASS_NPC))
                {
                    NPC* npc = static_cast<NPC*>(chr);

                    if (inrangex( npc, p_chr, range ))
                        (*f)( *npc, p_chr, p_text, p_textlen, texttype );
                }
            }
        }
    }
}

// UNICODE version
void for_nearby_npcs( void (*f)(NPC& npc, Character *chr, const char *text, int textlen, u8 texttype, const u16 *wtext, const char lang[4], int wtextlen),
                      Character *p_chr, const char *p_text, int p_textlen, u8 texttype, const u16 *p_wtext, const char p_lang[4], int p_wtextlen )
{
    int range;
    switch( texttype )
    {
    case TEXTTYPE_WHISPER:  range = RANGE_WHISPER;  break;
    case TEXTTYPE_YELL:     range = RANGE_YELL;     break;
    default:                range = RANGE_SAY;      break;
    }

    unsigned short wxL, wyL, wxH, wyH;
    zone_convert_clip( p_chr->x - range, p_chr->y - range, p_chr->realm, wxL, wyL );
    zone_convert_clip( p_chr->x + range, p_chr->y + range, p_chr->realm, wxH, wyH );
    for( unsigned short wx = wxL; wx <= wxH; ++wx )
    {
        for( unsigned short wy = wyL; wy <= wyH; ++wy )
        {
            ZoneCharacters& wchr = p_chr->realm->zone[wx][wy].characters;

            for( ZoneCharacters::iterator itr = wchr.begin(), end = wchr.end(); itr != end; ++itr )
            {
                Character* chr = *itr;
                if (chr->isa(UObject::CLASS_NPC))
                {
                    NPC* npc = static_cast<NPC*>(chr);

			        if (inrangex( p_chr, npc, range ))
                        (*f)( *npc, p_chr, p_text, p_textlen, texttype, p_wtext, p_lang, p_wtextlen );
                }
            }
        }
    }
}

void send_open_gump( Client *client, const UContainer& cont )
{
	MSG24_OPEN_GUMP opengump;
	opengump.msgtype = MSGOUT_24_OPEN_GUMP;
	opengump.serial = cont.serial_ext;
	opengump.gump = ctBEu16( cont.gump() );
	transmit( client, &opengump, sizeof opengump );
}

//dave changed 11/9/3, don't send invis items to those who can't see invis
void send_container_contents( Client *client, const UContainer& cont, bool show_invis )
{
	static MSG3C_CONTAINER_CONTENTS msg;

	msg.msgtype = MSGOUT_3C_CONTAINER_CONTENTS;
	
    int count = 0;
    UContainer::const_iterator itr = cont.begin();
	for( ; itr != cont.end(); ++itr )
	{
		const Item* item = GET_ITEM_PTR( itr );
        if ( show_invis || (!item->invisible() || client->chr->can_seeinvisitems()) )
        {
		    msg.items[count].serial = item->serial_ext;
		    msg.items[count].graphic = item->graphic_ext;
		    msg.items[count].unk6_00 = 0x00;

            msg.items[count].amount = ctBEu16( item->get_senditem_amount() );

            msg.items[count].x = ctBEu16( item->x );
		    msg.items[count].y = ctBEu16( item->y );
		    msg.items[count].container_serial = cont.serial_ext;
		    msg.items[count].color = item->color_ext;
////////aostest
if(client->UOExpansionFlag & AOS)
{
    send_object_cache(client, dynamic_cast<const UObject*>(item));
}
////////
            ++count;
        }
		else
		{
			send_remove_object(client,item);
		}
	}

	unsigned short msglen = offsetof( MSG3C_CONTAINER_CONTENTS, items ) + 
		                    count * sizeof msg.items[0];
	msg.msglen = ctBEu16( msglen );
	msg.count = ctBEu16( count );

	transmit( client, &msg, msglen );
}
