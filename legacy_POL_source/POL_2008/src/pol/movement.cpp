#include "clib/stl_inc.h"

#include "clib/endian.h"
#include "clib/stlutil.h"

#include "charactr.h"
#include "client.h"
#include "customhouses.h"
#include "house.h"
#include "msghandl.h"
#include "umsgtype.h"
#include "uvars.h"
#include "multi.h"
//#include "sockio.h"
#include "ufunc.h"
#include "uworld.h"


char walkok[4]="\x22\x00\x41";


void send_char_if_newly_inrange( Character *chr, Client *client	)
{
	if (  inrange( chr, client->chr ) &&
		  !inrange( chr->x, chr->y,
		            client->chr->lastx, client->chr->lasty ) &&
          client->chr->is_visible_to_me( chr ) &&
          client->chr != chr)
	{
		send_owncreate( client, chr );
	}
}

void send_item_if_newly_inrange( Item *item, Client *client )
{
	if (  inrange( client->chr, item ) &&				// if in range now,
		  !inrange( item->x, item->y,		// but weren't in range before this move,
		            client->chr->lastx, client->chr->lasty ))
	{
		send_item( client, item );
	}
}

void send_multi_if_newly_inrange( UMulti *multi, Client *client )
{
	if (  multi_inrange( client->chr, multi ) &&				// if in range now,
		  !multi_inrange( multi->x, multi->y,		// but weren't in range before this move,
		            client->chr->lastx, client->chr->lasty ))
	{
		send_multi( client, multi );
        UHouse* house = multi->as_house();
        if( (client->UOExpansionFlag & AOS) && house != NULL && house->IsCustom())
            CustomHousesSendShort(house, client);
	}
}

void send_objects_newly_inrange( Client* client )
{
    Character* chr = client->chr;
    unsigned short wxL, wyL, wxH, wyH;

//	Moved this higher, to see if would affect item height = 0 stuff dissapearing under floors
	zone_convert_clip( chr->x - RANGE_VISUAL_LARGE_BUILDINGS, chr->y - RANGE_VISUAL_LARGE_BUILDINGS, chr->realm, wxL, wyL );
    zone_convert_clip( chr->x + RANGE_VISUAL_LARGE_BUILDINGS, chr->y + RANGE_VISUAL_LARGE_BUILDINGS, chr->realm, wxH, wyH );
    for( unsigned short wx = wxL; wx <= wxH; ++wx )
    {
        for( unsigned short wy = wyL; wy <= wyH; ++wy )
        {
            ZoneMultis& wmulti = chr->realm->zone[wx][wy].multis;
            for( ZoneMultis::iterator itr = wmulti.begin(), end = wmulti.end(); itr != end; ++itr )
            {
                UMulti* multi = *itr;
                send_multi_if_newly_inrange( multi, client );
            }
        }
    }

    zone_convert_clip( chr->x - RANGE_VISUAL, chr->y - RANGE_VISUAL, chr->realm, wxL, wyL );
    zone_convert_clip( chr->x + RANGE_VISUAL, chr->y + RANGE_VISUAL, chr->realm, wxH, wyH );
    for( unsigned short wx = wxL; wx <= wxH; ++wx )
    {
        for( unsigned short wy = wyL; wy <= wyH; ++wy )
        {
            ZoneCharacters& wchr = chr->realm->zone[wx][wy].characters;
            for( ZoneCharacters::iterator itr = wchr.begin(), end = wchr.end(); itr != end; ++itr )
            {
                Character* chr = *itr;
                send_char_if_newly_inrange( chr, client );
            }


            ZoneItems& witem = chr->realm->zone[wx][wy].items;
            for( ZoneItems::iterator itr = witem.begin(), end = witem.end(); itr != end; ++itr )
            {
                Item* item = *itr;
                send_item_if_newly_inrange( item, client );
            }
        }
    }

//	Moved this higher, to see if would affect item height = 0 stuff dissapearing under floors
/*    zone_convert_clip( chr->x - RANGE_VISUAL_LARGE_BUILDINGS, chr->y - RANGE_VISUAL_LARGE_BUILDINGS, chr->realm, wxL, wyL );
    zone_convert_clip( chr->x + RANGE_VISUAL_LARGE_BUILDINGS, chr->y + RANGE_VISUAL_LARGE_BUILDINGS, chr->realm, wxH, wyH );
    for( unsigned short wx = wxL; wx <= wxH; ++wx )
    {
        for( unsigned short wy = wyL; wy <= wyH; ++wy )
        {
            ZoneMultis& wmulti = chr->realm->zone[wx][wy].multis;
            for( ZoneMultis::iterator itr = wmulti.begin(), end = wmulti.end(); itr != end; ++itr )
            {
                UMulti* multi = *itr;
                send_multi_if_newly_inrange( multi, client );
            }
        }
    }
*/
}


void handle_walk( Client *client, MSG02_WALK *msg02 )
{
	//printf( "Walk: Dir=0x%02.02x, MoveNum=0x%02.02x\n", 
	//				msg02->dir,
	//				msg02->movenum );

	Character *chr = client->chr;

	if (chr->move( msg02->dir ))
    {
	    client->pause();
	    MSG22_MOVE_APPROVE msg;
	    msg.msgtype = MSGOUT_22_MOVE_APPROVE_ID;
	    msg.movenum = msg02->movenum;
		msg.unk_41 = client->chr->hilite_color_idx( client->chr ); // MuadDib - 03/02/2006
	    client->transmit( &msg, sizeof msg );


	    // tell other clients about me if they can see me
	    chr->tellmove();

        send_objects_newly_inrange( client );

	    client->restart();
    }
    else
    {
        MSG21_MOVE_REJECT msg;
        msg.msgtype = MSGOUT_21_MOVE_REJECT_ID;
        msg.sequence = msg02->movenum;
        msg.x = ctBEu16( chr->x );
        msg.y = ctBEu16( chr->y );
        msg.facing = chr->facing;
        msg.z = chr->z;
        client->transmit( &msg, sizeof msg );
    }
}

MESSAGE_HANDLER( MSG02_WALK, handle_walk );
