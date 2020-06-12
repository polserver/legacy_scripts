// History
//   2005/05/28 Shinigami: now u can call open_trade_window without item and splitted (for use with uo::SecureTradeWin)
//                         place_item_in_secure_trade_container splitted (for use with uo::MoveItemToSecureTradeWin)
//   2005/06/01 Shinigami: return_traded_items - added realm support
//   2005/11/28 MuadDib: Added 2 more send_trade_statuses() for freshing of chr and droped_on
//                       in sending of secure trade windows. This was per packet checks on
//                       OSI, and works in newer clients. Unable to test in 2.0.0

#include "clib/stl_inc.h"

#include <assert.h>
#include <stdio.h>

#include "clib/endian.h"
#include "clib/logfile.h"
#include "clib/passert.h"
#include "clib/random.h"
#include "clib/stlutil.h"
#include "clib/strutil.h"

#include "bscript/berror.h"

#include "plib/realm.h"

#include "boat.h"
#include "charactr.h"
#include "client.h"
#include "dtrace.h"
#include "getitem.h"
#include "layers.h"
#include "los.h"
#include "msghandl.h"
#include "nmsgtype.h"
#include "npc.h"
#include "objtype.h"
#include "polcfg.h"
#include "realms.h"
#include "sfx.h"
#include "sockio.h"
#include "ssopt.h"
#include "storage.h"
#include "ucfg.h"
#include "ufunc.h"
#include "umsgtype.h"
#include "uofile.h"
#include "uoscrobj.h"
#include "uvars.h"
#include "uworld.h"

void send_trade_statuses( Character* chr );

bool place_item_in_container( Client *client, Item *item, UContainer *cont, u16 x, u16 y )
{
	ItemRef itemref(item); //dave 1/28/3 prevent item from being destroyed before function ends
	// check for inserting a container in itself, or 
	// in one of its children.
	if ((cont->serial == item->serial) ||
		is_a_parent( cont, item->serial ))
	{
		// UNTESTED
		send_item_move_failure( client, MOVE_ITEM_FAILURE_UNKNOWN );
	
        return false;
	}

	//Dave split these 2 checks up for a more descriptive error.
	if (!cont->can_add( *item ))
	{
		send_item_move_failure( client, MOVE_ITEM_FAILURE_UNKNOWN );
		send_sysmessage( client, "That item is too heavy for the container or the container is full." );
        return false;
	}
    if (!cont->can_insert_add_item( client->chr, UContainer::MT_PLAYER, item))
	{
		send_item_move_failure( client, MOVE_ITEM_FAILURE_UNKNOWN );
		send_sysmessage( client, "You cannot insert that item into the container." );
        return false;
	}
	if(item->orphan()) //dave added 1/28/3, item might be destroyed in RTC script
	{
		return true; //true so we won't try to undo_get_item on the destroyed item
	}
	client->pause();

	//DAVE commented this out 11/17/02. Let the users use the onInsert scripts for sound effects!
	//play_sound_effect( client->chr, SOUND_EFFECT_ADD_TO_BACKPACK );
	ConstForEach( clients, send_remove_object_if_inrange, item );

    item->set_dirty();
	item->x = x;
	item->y = y;
	cont->add( item );
	cont->restart_decay_timer();
	send_put_in_container_to_inrange( item );
    
    cont->on_insert_add_item( client->chr, UContainer::MT_PLAYER, item );

	client->restart();
    return true;
}

bool do_place_item_in_secure_trade_container( Client* client, Item* item, UContainer* cont, Character* dropon, u16 x, u16 y );
bool place_item_in_secure_trade_container( Client* client, Item* item, u16 x, u16 y )
{
    UContainer* cont = client->chr->trade_container();
    Character* dropon = client->chr->trading_with.get();
    if (dropon == NULL || dropon->client == NULL)
    {
        send_sysmessage( client, "Unable to complete trade" );
        return false;
    }
    if (!cont->can_add( *item ))
	{
        send_sysmessage( client, "That's too heavy to trade." );
		return false;
	}
    
    return do_place_item_in_secure_trade_container( client, item, cont, dropon, x, y );
}

BObjectImp* place_item_in_secure_trade_container( Client* client, Item* item )
{
    UContainer* cont = client->chr->trade_container();
    Character* dropon = client->chr->trading_with.get();
    if (dropon == NULL || dropon->client == NULL)
    {
        return new BError( "Unable to complete trade" );
    }
    if (!cont->can_add( *item ))
	{
        return new BError( "That's too heavy to trade." );
	}
    
    if (do_place_item_in_secure_trade_container( client, item, cont, dropon, 5 + random_int( 45 ), 5 + random_int( 45 ) ))
        return new BLong(1);
    else
        return new BError( "Something went wrong with trade window." );

}

bool do_place_item_in_secure_trade_container( Client* client, Item* item, UContainer* cont, Character* dropon, u16 x, u16 y )
{
	client->pause();

    client->chr->trade_accepted = false;
    dropon->trade_accepted = false;
    send_trade_statuses( client->chr );

	//DAVE commented this out 11/17/02. Let the users use the onInsert scripts for sound effects!
	//play_sound_effect( client->chr, SOUND_EFFECT_ADD_TO_BACKPACK );
	ConstForEach( clients, send_remove_object_if_inrange, item );

	item->x = x;
	item->y = y;
    item->z = 9;

	cont->add( item );
	
	send_put_in_container( client, item );
	send_put_in_container( dropon->client, item );

	// MuadDib ADDED to try to fix secure trade errors.
	send_trade_statuses( client->chr );
	send_trade_statuses( dropon->client->chr );

	client->restart();
    return true;
}

bool add_item_to_stack( Client *client, Item *item, Item *target_item )
{
	// TJ 3/18/3: Only called (so far) from place_item(), so it's only ever from
	// a player; this means that there's no need to worry about passing
	// UContainer::MT_CORE_* constants to the can_insert function (yet). :)

	ItemRef itemref(item); //dave 1/28/3 prevent item from being destroyed before function ends
	// placing a stack on another stackable?
	if ( (!target_item->can_add_to_self( *item )) ||
        (target_item->container && !target_item->container->can_insert_increase_stack( client->chr, UContainer::MT_PLAYER, target_item, item->getamount(), item )))
	{
		send_sysmessage(client,"Could not add item to stack.");
		send_item_move_failure( client, MOVE_ITEM_FAILURE_UNKNOWN );
		
        return false;
	}

	/* At this point, we know:
		 the object types match
		 the combined stack isn't 'too large'
	   We don't know: (FIXME)
	     if the item type is stackable, period (CLIENT_HOLE)
	     if a container that the target_item is in will overfill from this
	*/

	ConstForEach( clients, send_remove_object_if_inrange, item );
	
    u16 amtadded = item->getamount();

	target_item->add_to_self( item );
	update_item_to_inrange( target_item );

    if (target_item->container)
	{
        target_item->container->on_insert_increase_stack( client->chr, UContainer::MT_PLAYER, target_item, amtadded );
		target_item->container->restart_decay_timer();
	}
    return true;
}

bool place_item( Client *client, Item *item, u32 target_serial, u16 x, u16 y )
{

	// now, find the item or container into which to place the object.
	Item *target_item = find_legal_item( client->chr, target_serial );
	
    if (target_item == NULL && client->chr->is_trading())
    {
        UContainer* cont = client->chr->trade_container();
        if (target_serial == cont->serial) // placing directly in secure container
        {
            // cout << "trade window placement: " << x << "," << y << endl;
            return place_item_in_secure_trade_container( client, item, x, y );
        }
    }
    
    if (target_item == NULL)
	{
		// UNTESTED
		send_item_move_failure( client, MOVE_ITEM_FAILURE_UNKNOWN );
		
        return false;
	}

    if (pol_distance( client->chr, target_item ) > 2)
    {
		send_item_move_failure( client, MOVE_ITEM_FAILURE_TOO_FAR_AWAY );
        return false;
    }
    if (!client->chr->realm->has_los( *client->chr, *target_item->toplevel_owner() ))
    {
        send_item_move_failure( client, MOVE_ITEM_FAILURE_OUT_OF_SIGHT );
        return false;
    }


    
	if (target_item->isa(UObject::CLASS_ITEM))
	{
        return add_item_to_stack( client, item, target_item );
	}
	else if (target_item->isa(UObject::CLASS_CONTAINER))
	{
		return place_item_in_container( client, item, static_cast<UContainer *>(target_item), x, y );
	}
	else
	{
		// UNTESTED CLIENT_HOLE?
		send_item_move_failure( client, MOVE_ITEM_FAILURE_UNKNOWN );
		
        return false;
	}
}

bool drop_item_on_ground( Client *client, Item *item, u16 x, u16 y, s8 z )
{
	Character* chr = client->chr;

    UMulti* multi;
    int newz;
    if (!inrangex( chr, x, y, 2))
    {
        Log("Client (Character %s) tried to drop an item out of range.\n",client->chr->name().c_str());
		send_item_move_failure( client, MOVE_ITEM_FAILURE_TOO_FAR_AWAY );
        return false;
    }

    if (!chr->realm->dropheight( x, y, z, client->chr->z, &newz, &multi ))
    {
        Log("Client (Character %s) tried to drop an item at (%d,%d,%d), which is a blocked location.\n",
                client->chr->name().c_str(),
                int(x),int(y),int(z));
        return false;
    }

    if (newz != z)
    {
        cerr << "Client tried to drop item to "
              << x << "," << y << "," << int(z) 
              << " but server is putting it at Z=" << int(newz) << endl;
    }
    LosObj att(*client->chr);
    LosObj tgt(x,y,newz);
    if (!chr->realm->has_los( att, tgt ))
    {
        send_item_move_failure( client, MOVE_ITEM_FAILURE_OUT_OF_SIGHT );
        return false;
    }

    item->set_dirty();
	item->restart_decay_timer();  // MuadDib Moved higher, to help with instant decay.
	item->x = x;
	item->y = y;
	item->z = newz;
	if (item->realm != chr->realm)
	{
		if (item->isa( UObject::CLASS_CONTAINER ))
		{
			UContainer* cont = static_cast<UContainer*>(item);
            cont->for_each_item(setrealm, (void*)chr->realm);
		}
        setrealm(item, (void*)chr->realm);
	}
	item->container = NULL;
    add_item_to_world( item );
    if (multi != NULL)
    {
        multi->register_object( item );
    }

	send_item_to_inrange( item );
    return true;
}

UContainer* find_giveitem_container( Item* item_to_add )
{   
    StorageArea* area = storage.create_area( "GivenItems" );
    passert( area != NULL );

    for( unsigned short i = 0; i < 500; ++i )
    {
        string name = "Cont" + decint(i);
        Item* item = NULL;
        item = area->find_root_item( name );
        if (item == NULL)
        {
            item = Item::create( UOBJ_BACKPACK );
            item->setname( name );
			item->realm = find_realm(string("britannia"));;
            area->insert_root_item( item );
        }   
        passert( item->isa( UObject::CLASS_CONTAINER ));
        UContainer* cont = static_cast<UContainer*>(item);
        if (cont->can_add(*item_to_add))
            return cont;
    }
    return NULL;
}

void send_trade_container( Client* client,
                           Character* whos,
                           UContainer* cont )
{
    MSG25_PUT_IN_CONTAINER msg;
    msg.msgtype = MSGOUT_25_PUT_IN_CONTAINER;
    msg.serial = cont->serial_ext;
    msg.graphic = cont->graphic_ext;
    msg.unk7 = 0;
    msg.amount = ctBEu16(1);
    msg.x = 0;
    msg.y = 0;
    msg.container_serial = whos->serial_ext;
    msg.color = 0;
    client->transmit( &msg, sizeof msg );
}

bool do_open_trade_window( Client* client, Item* item, Character* dropon );
bool open_trade_window( Client* client, Item* item, Character* dropon )
{
    if (!config.enable_secure_trading)
    {
        send_sysmessage( client, "Secure trading is unavailable." );
        return false;
    }

    if (!ssopt.allow_secure_trading_in_warmode)
    {
        if (dropon->warmode)
        {
            send_sysmessage( client, "You cannot trade with someone in war mode." );
            return false;
        }
        if (client->chr->warmode)
        {
            send_sysmessage( client, "You cannot trade while in war mode." );
            return false;
        }
    }
    if (dropon->is_trading())
    {
        send_sysmessage( client, "That person is already involved in a trade." );
        return false;
    }
    if (client->chr->is_trading())
    {
        send_sysmessage( client, "You are already involved in a trade." );
        return false;
    }
    if (!dropon->client)
    {
        send_sysmessage( client, "That person is already involved in a trade." );
        return false;
    }
	//DAVE added this check 11/19
	if( client->chr->dead() || dropon->dead() )
	{
        send_sysmessage( client, "Ghosts cannot trade items." );
        return false;		
	}
    
    return do_open_trade_window( client, item, dropon );
}

BObjectImp* open_trade_window( Client* client, Character* dropon )
{
    if (!config.enable_secure_trading)
    {
        return new BError( "Secure trading is unavailable." );
    }

    if (!ssopt.allow_secure_trading_in_warmode)
    {
        if (dropon->warmode)
        {
            return new BError( "You cannot trade with someone in war mode." );
        }
        if (client->chr->warmode)
        {
            return new BError( "You cannot trade while in war mode." );
        }
    }
    if (dropon->is_trading())
    {
        return new BError( "That person is already involved in a trade." );
    }
    if (client->chr->is_trading())
    {
        return new BError( "You are already involved in a trade." );
    }
    if (!dropon->client)
    {
        return new BError( "That person is already involved in a trade." );
    }
	//DAVE added this check 11/19
	if( client->chr->dead() || dropon->dead() )
	{
        return new BError( "Ghosts cannot trade items." );
	}
    
    if (do_open_trade_window( client, NULL, dropon ))
        return new BLong(1);
    else
        return new BError( "Something goes wrong." );
}

bool do_open_trade_window( Client* client, Item* item, Character* dropon )
{
    dropon->create_trade_container();
    client->chr->create_trade_container();

    dropon->trading_with.set( client->chr );
    dropon->trade_accepted = false;
    client->chr->trading_with.set( dropon );
    client->chr->trade_accepted = false;

    send_trade_container( client,         dropon,      dropon->trade_container() );
    send_trade_container( dropon->client, dropon,      dropon->trade_container() );
    send_trade_container( client,         client->chr, client->chr->trade_container() );
    send_trade_container( dropon->client, client->chr, client->chr->trade_container() );


    MSG6F_SECURE_TRADE msg;
    msg.msgtype = MSG6F_SECURE_TRADE_ID;
    msg.msglen = ctBEu16(sizeof msg);
    msg.action = MSG6F_SECURE_TRADE::ACTION_INIT;
    msg.chr_serial = dropon->serial_ext;
    msg.cont1_serial = client->chr->trade_container()->serial_ext;
    msg.cont2_serial = dropon->trade_container()->serial_ext;
    msg.havename = 1;
    strzcpy( msg.name, dropon->name().c_str(), sizeof msg.name );
    client->transmit( &msg, sizeof msg );

    msg.chr_serial = client->chr->serial_ext;
    msg.cont1_serial = dropon->trade_container()->serial_ext;
    msg.cont2_serial = client->chr->trade_container()->serial_ext;
    msg.havename = 1;
    strzcpy( msg.name, client->chr->name().c_str(), sizeof msg.name );
    dropon->client->transmit( &msg, sizeof msg );
    
    if (item != NULL)
        return place_item_in_secure_trade_container( client, item, 20, 20 );
    else
        return true;
}

bool drop_item_on_mobile( Client* client, Item* item, u32 target_serial )
{
    Character* dropon = find_character( target_serial );
    
    if (dropon == NULL) 
    {
        send_item_move_failure( client, MOVE_ITEM_FAILURE_UNKNOWN );
        return false;
    }

	//dave added this 12/20 to protect against an injection exploit
    if (pol_distance( client->chr, dropon ) > 2)
    {
		send_item_move_failure( client, MOVE_ITEM_FAILURE_TOO_FAR_AWAY );
        return false;
    }
    if (!client->chr->realm->has_los( *client->chr,*dropon ))
    {
        send_item_move_failure( client, MOVE_ITEM_FAILURE_OUT_OF_SIGHT );
        return false;
    }
    
	if (!dropon->isa( UObject::CLASS_NPC)) 
    {
        bool res = open_trade_window( client, item, dropon );
        if (!res)
	        send_item_move_failure( client, MOVE_ITEM_FAILURE_UNKNOWN );
        return res;
    }

    NPC* npc = static_cast<NPC*>(dropon);
    if (!npc->can_accept_event( EVID_ITEM_GIVEN )) 
    {
        send_item_move_failure( client, MOVE_ITEM_FAILURE_UNKNOWN );
        return false;
    }

    UContainer* cont = find_giveitem_container(item);
    if (cont == NULL)
    {
        send_item_move_failure( client, MOVE_ITEM_FAILURE_UNKNOWN );
        return false;
    }

    client->pause();

	ConstForEach( clients, send_remove_object_if_inrange, item );

    u16 rx, ry;
    cont->get_random_location( &rx, &ry );

    item->set_dirty();
    item->container = cont;
	item->x = rx;
	item->y = ry;
	cont->add_at_random_location( item );

    npc->send_event( new ItemGivenEvent( client->chr, item, npc ) );
	
	client->restart();

    return true;

}
// target_serial should indicate a character, or a container, but not a pile.
bool drop_item_on_object( Client *client, Item *item, u32 target_serial )
{
	ItemRef itemref(item); //dave 1/28/3 prevent item from being destroyed before function ends
	UContainer *cont = NULL;
	if (IsCharacter( target_serial )) // dropped on a character.
	{
		if (target_serial == client->chr->serial) // the most common, dropping on yourself
		{
			cont = client->chr->backpack();
			// then continue below, as if dropped on my backpack.
		}
		else
		{
            return drop_item_on_mobile( client, item, target_serial );
		}
	}
	
	if (cont == NULL)
	{
		cont = find_legal_container( client->chr, target_serial );
	}

	if (cont == NULL)
	{
		// UNTESTED
		send_item_move_failure( client, MOVE_ITEM_FAILURE_UNKNOWN );

        return false;
	}
    if (pol_distance( client->chr, cont ) > 2)
    {
		send_item_move_failure( client, MOVE_ITEM_FAILURE_TOO_FAR_AWAY );
        return false;
    }
    if (!client->chr->realm->has_los( *client->chr, *cont->toplevel_owner() ))
    {
        send_item_move_failure( client, MOVE_ITEM_FAILURE_OUT_OF_SIGHT );
        return false;
    }

    // try to add to an existing stack
    if (item->stackable())
    {
        for( UContainer::const_iterator itr = cont->begin(); itr != cont->end(); ++itr )
        {
            Item* exitem = GET_ITEM_PTR( itr );
            if ( exitem->can_add_to_self( *item ) )
            {

				//DAVE add this 11/28 - dropping a stack onto backpack with existing stack in pack:
				//	check container limits and call canInsert script for the container.
				if (cont->can_add( *item ))
				{
                    if(cont->can_insert_increase_stack( client->chr, UContainer::MT_PLAYER, exitem, item->getamount(), item ))
					{
						if(item->orphan()) //dave added 1/28/3, item might be destroyed in RTC script
						{
							return true; //true so we won't try to undo_get_item on the destroyed item;
						}
                        u16 amtadded = item->getamount();
						exitem->add_to_self( item );
						update_item_to_inrange( exitem );

                        cont->on_insert_increase_stack( client->chr, UContainer::MT_PLAYER, exitem, amtadded );

                        return true; //true so we won't try to undo_get_item on the destroyed item
					}
				}
				return false;
            }
        }
    }

    u16 rx, ry;
    cont->get_random_location( &rx, &ry );

	return place_item_in_container( client, item, cont, rx, ry ); 
}

/* DROP_ITEM messages come in a couple varieties:
	
	1)	Dropping an item on another object, or a person:
			item_serial: serial number of item to drop
			x: 0xFFFF
			y: 0xFFFF
			z: 0
			target_serial: serial number of item or character to drop on.

	2)  Dropping an item on the ground:
			item_serial: serial number of item to drop
			x,y,z: position
			target_serial: 0xFFFFFFFF

    3)	Placing an item in a container, or in an existing pile:
			item_serial: serial number of item to drop
			x: x-position
			y: y-position
			z: 0
			target_serial: serial number of item or character or pile to drop on.
*/

void drop_item( Client *client, MSG08_DROP_ITEM *msg )
{
	u32 item_serial = cfBEu32( msg->item_serial );
	u16 x = cfBEu16( msg->x );
	u16 y = cfBEu16( msg->y );
	s8 z = msg->z;
	u32 target_serial = cfBEu32( msg->target_serial );

	//printf( "DROP_ITEM: serial=%08lX, pos=[%d,%d,%d], target=%08lX\n",
	//		item_serial, x, y, z, target_serial );


	Item *item = client->chr->gotten_item;
	if (item == NULL)
	{
		Log( "Character %08lX tried to drop item %08lX, but had not gotten an item.\n",
			     client->chr->serial, 
				 item_serial);
		return;
	}
	if (item->serial != item_serial)
	{
		Log( "Character %08lX tried to drop item %08lX, but instead had gotten item %08lX.\n",
			     client->chr->serial, 
				 item_serial,
				 item->serial);
		return;
	}
    client->chr->gotten_item->inuse( false );
	client->chr->gotten_item = NULL;

    bool res;
	if (target_serial == 0xFFffFFffLu)
	{
		res = drop_item_on_ground( client, item, x, y, z );
	}
	else if (x == 0xFFFF) // but target_serial is an actual serial
	{
		res = drop_item_on_object( client, item, target_serial );
	}
	else
	{
		res = place_item( client, item, target_serial, x, y );
	}

    if (!res)
    {
		undo_get_item( client->chr, item );
    }
	item->inuse(false);
	item->is_gotten(false);

    client->chr->refresh_ar();
}

MESSAGE_HANDLER( MSG08_DROP_ITEM, drop_item );

void return_traded_items( Character* chr )
{
    UContainer* cont = chr->trade_container();
    UContainer* bp = chr->backpack();
    if (cont == NULL || bp == NULL)
        return;

    UContainer::Contents tmp;
    cont->extract( tmp );
    while (!tmp.empty())
    {
        Item* item = ITEM_ELEM_PTR( tmp.back() );
        tmp.pop_back();
        item->container = NULL;
        item->layer = 0;

		ItemRef itemref(item); //dave 1/28/3 prevent item from being destroyed before function ends
		//DAVE 11/27- check caninsert on STW cancel, drop to ground if fails.
		if (bp->can_add( *item ) && bp->can_insert_add_item( chr, UContainer::MT_CORE_MOVED, item ) )
        {
			if(item->orphan()) //dave added 1/28/3, item might be destroyed in RTC script
			{
				continue;
			}
            bp->add_at_random_location( item );
			if (chr->client)
				send_put_in_container( chr->client, item );
			bp->on_insert_add_item( chr, UContainer::MT_CORE_MOVED, item );
        }
        else
        {
            item->set_dirty();
            item->x = chr->x;
            item->y = chr->y;
            item->realm = chr->realm;
            add_item_to_world( item );
            register_with_supporting_multi( item );
            move_item( item, chr->x, chr->y, chr->z, NULL );
        }
    }

}


void cancel_trade( Character* chr1 )
{
    Character* chr2 = chr1->trading_with.get();

    return_traded_items( chr1 );
    chr1->trading_with.clear();

    if (chr1->client)
    {
        MSG6F_SECURE_TRADE msg;
        msg.msgtype = MSG6F_SECURE_TRADE_ID;
        msg.msglen = ctBEu16( offsetof(MSG6F_SECURE_TRADE,name) );
        msg.action = MSG6F_SECURE_TRADE::ACTION_CANCEL;
        msg.chr_serial = chr1->trade_container()->serial_ext;
        msg.cont1_serial = 0;
        msg.cont2_serial = 0;
        msg.havename = 0;
        transmit( chr1->client, &msg, offsetof(MSG6F_SECURE_TRADE,name) );
    }
    chr1->refresh_ar();

    if (chr2)
    {
        return_traded_items( chr2 );
        chr2->trading_with.clear();
        if (chr2->client)
        {
            MSG6F_SECURE_TRADE msg;
            msg.msgtype = MSG6F_SECURE_TRADE_ID;
            msg.msglen = ctBEu16( offsetof(MSG6F_SECURE_TRADE,name) );
            msg.action = MSG6F_SECURE_TRADE::ACTION_CANCEL;
            msg.chr_serial = chr2->trade_container()->serial_ext;
            msg.cont1_serial = 0;
            msg.cont2_serial = 0;
            msg.havename = 0;
            transmit( chr2->client, &msg, offsetof(MSG6F_SECURE_TRADE,name) );
        }
        chr2->refresh_ar();
    }
}

void send_trade_statuses( Character* chr )
{
    unsigned long stat1 = chr->trade_accepted?1:0;
    unsigned long stat2 = chr->trading_with->trade_accepted?1:0;

    MSG6F_SECURE_TRADE msg;
    msg.msgtype = MSG6F_SECURE_TRADE_ID;
    msg.msglen = ctBEu16( offsetof(MSG6F_SECURE_TRADE,name) );
    msg.action = MSG6F_SECURE_TRADE::ACTION_STATUS;

    msg.chr_serial = chr->trade_container()->serial_ext;
    msg.cont1_serial = ctBEu32( stat1 );
    msg.cont2_serial = ctBEu32( stat2 );
    msg.havename = 0;
    transmit( chr->client, &msg, offsetof(MSG6F_SECURE_TRADE,name) );

    msg.chr_serial = chr->trading_with->trade_container()->serial_ext;
    msg.cont1_serial = ctBEu32( stat2 );
    msg.cont2_serial = ctBEu32( stat1 );
    transmit( chr->trading_with->client, &msg, offsetof(MSG6F_SECURE_TRADE,name) );
}

void change_trade_status( Character* chr, bool set )
{
    chr->trade_accepted = set;
    send_trade_statuses( chr );
    if (chr->trade_accepted && 
        chr->trading_with->trade_accepted)
        
    {
        UContainer* cont0 = chr->trade_container();
        UContainer* cont1 = chr->trading_with->trade_container();
        if (cont0->can_swap( *cont1 ))
        {
            cont0->swap( *cont1 );
            cancel_trade( chr );
        }
        else
        {
            Log( "Can't swap trade containers: ic0=%ld,w0=%ld, ic1=%ld,w1=%ld\n",
                    cont0->item_count(),
                    cont0->weight(),
                    cont1->item_count(),
                    cont1->weight());
        }
    }
}

void handle_secure_trade_msg( Client* client, MSG6F_SECURE_TRADE* msg )
{
    if (!client->chr->is_trading())
        return;
    switch( msg->action )
    {
        case MSG6F_SECURE_TRADE::ACTION_CANCEL:
            dtrace(5) << "Cancel secure trade" << endl;
            cancel_trade( client->chr );
            break;

        case MSG6F_SECURE_TRADE::ACTION_STATUS:
            bool set;
            set = msg->cont1_serial?true:false;
            if (set)
                dtrace(5) << "Set secure trade indicator" << endl;
            else
                dtrace(5) << "Clear secure trade indicator" << endl;
            change_trade_status( client->chr, set );
            break;
        
        default:
            dtrace(5) << "Unknown secure trade action: " << (int)msg->action << endl;
            break;
    }
}
MESSAGE_HANDLER_VARLEN( MSG6F_SECURE_TRADE, handle_secure_trade_msg );

void cancel_all_trades()
{
    for( Clients::iterator itr = clients.begin(); itr != clients.end(); ++itr )
    {
        Client* client = (*itr);
        if (client->ready && client->chr)
        {
            Character* chr = client->chr;
            if (chr->is_trading())
                cancel_trade( chr );
        }
    }
}
