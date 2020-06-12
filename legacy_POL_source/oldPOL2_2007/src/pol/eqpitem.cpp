#include "clib/stl_inc.h"
#include <assert.h>
#include <stdio.h>

#include "bscript/bobject.h"

#include "clib/endian.h"
#include "clib/logfile.h"

#include "charactr.h"
#include "client.h"
#include "msghandl.h"
#include "ufunc.h"
#include "umsgtype.h"
#include "uofile.h"
#include "uvars.h"

void equip_item( Client *client, MSG13_EQUIP_ITEM *msg )
{
	u32 serial = cfBEu32( msg->serial );
	u8 layer = msg->layer;
	u32 equip_on_serial = cfBEu32( msg->equipped_on );

	//printf( "EQUIP_ITEM: Serial=%08lX, Layer=%d, EquippedOn=%08lX\n",
	//	serial, layer, equipped_on );

	if ((layer > HIGHEST_LAYER) || (layer == 0) || client->chr->dead())
	{
		send_item_move_failure( client, MOVE_ITEM_FAILURE_ILLEGAL_EQUIP );
		return;
	}
	
	Item *item = client->chr->gotten_item;

	if (item == NULL)
	{
		// this really shouldn't happen.
		Log( "Character %08lX tried to equip item %08lx, which did not exist in gotten_items.\n",
			 client->chr->serial, serial );
		send_item_move_failure( client, MOVE_ITEM_FAILURE_ILLEGAL_EQUIP ); // 5
		return;
	}

    if (item->serial != serial)
	{
		// this really shouldn't happen.
		Log( "Character %08lX tried to equip item %08lx, but had gotten item %08lX\n",
			 client->chr->serial, 
			 serial,
			 item->serial);
		send_item_move_failure( client, MOVE_ITEM_FAILURE_ILLEGAL_EQUIP ); // 5
		return;
	}
    ItemRef itemref(item); //dave 1/28/3 prevent item from being destroyed before function ends

	unsigned char tiledata_layer = tilelayer( item->graphic );
	item->layer = tiledata_layer; //dave changed 2/18/3, ignore msg->layer!
	client->chr->gotten_item->inuse(false);
	item->is_gotten(false);
	client->chr->gotten_item = NULL;

    Character* equip_on = NULL;
    if (equip_on_serial == client->chr->serial)
    {
        equip_on = client->chr;
    }
    else
    {
        equip_on = find_character( equip_on_serial );
        if (equip_on == NULL ||
            !client->chr->can_clothe( equip_on ))
        {
		    send_item_move_failure( client, MOVE_ITEM_FAILURE_ILLEGAL_EQUIP );
		    // the client now puts the item back where it was before.

		    // return the item to wherever it was. (?)
		    undo_get_item( client->chr, item );
            return;		
        }
    }

	if (equip_on->layer_is_equipped( tiledata_layer )) //dave changed 2/18/3, ignore msg->layer!
	{
		// it appears the client already checks for this, so this code hasn't been exercised.
		// we'll assume client mouse holds on to object
        // 3D Client doesn't check for this!
		send_item_move_failure( client, MOVE_ITEM_FAILURE_ALREADY_WORN );

        //2lines commented out 11/01/03, don't move to gotten item, just undo get.
		//client->chr->gotten_item = item;
        //client->chr->gotten_item->inuse( true );
        undo_get_item( client->chr, item ); //added 11/01/03 for 3d client
		return;
	}

	/* //dave commented out 2/18/3, ignore msg->layer!
    if (!item->setlayer( layer ))
    {
		send_item_move_failure( client, MOVE_ITEM_FAILURE_ILLEGAL_EQUIP );
		// the client now puts the item back where it was before.

		// return the item to wherever it was. (?)
		undo_get_item( client->chr, item );
        return;		
    }
	*/

    if (!equip_on->strong_enough_to_equip( item ))
    {
		send_item_move_failure( client, MOVE_ITEM_FAILURE_ILLEGAL_EQUIP );
		// the client now puts the item back where it was before.

		// return the item to wherever it was. (?)
		undo_get_item( client->chr, item );
        if (client->chr == equip_on)
        {
            send_sysmessage( client, "You are not strong enough to use that." );
        }
        else
        {
            send_sysmessage( client, "Insufficient strength to equip that." );
        }
        return;		
    }

    if (!equip_on->equippable( item ) ||
        !item->check_equiptest_scripts( equip_on ) ||
        !item->check_equip_script( equip_on, false ))
    {
		send_item_move_failure( client, MOVE_ITEM_FAILURE_ILLEGAL_EQUIP );
		// the client now puts the item back where it was before.
		if(item->orphan()) //dave added 1/28/3, item might be destroyed in RTC script
		{
			return;
		}
		// return the item to wherever it was. (?)
		undo_get_item( client->chr, item );
		return;		
    }

	if(item->orphan()) //dave added 1/28/3, item might be destroyed in RTC script
	{
		return;
	}
    
	equip_on->equip( item );
	send_wornitem_to_inrange( equip_on, item );
}

MESSAGE_HANDLER( MSG13_EQUIP_ITEM, equip_item );
