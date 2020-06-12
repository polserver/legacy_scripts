// History
//  2006/05/23 Shinigami: send_full_statmsg() updated to support Elfs

#include "clib/stl_inc.h"
#include "clib/clib.h"
#include "clib/endian.h"

#include "charactr.h"
#include "client.h"
#include "sockio.h"
#include "ufunc.h"
#include "umsgtype.h"
#include "uoclient.h"
#include "uvars.h"

#include "statmsg.h"

void send_full_statmsg( Client *client, Character *chr )
{
	MSG11_STATS_WINDOW msg;
	msg.msgtype = MSG11_STATS_WINDOW_ID;

	if( (client->UOExpansionFlag & AOS) )
	{
		msg.msglen = ctBEu16(sizeof msg);
		msg.moreinfo = 04;  // Set to AOS level statbar for full info
	}
	else
	{
		unsigned short msglen = offsetof( MSG11_STATS_WINDOW, statcap );
		msg.msglen = ctBEu16(msglen);
		msg.moreinfo = 01;  // Set to oldschool statbar info.
	}
	msg.serial = chr->serial_ext;
	strzcpy( msg.name, chr->name().c_str(), sizeof msg.name );
	
    if (uoclient_general.hits.any)
    {
        long v = chr->vital(uoclient_general.hits.id).current_ones();
        if (v > 0xFFFF)
            v = 0xFFFF;
        msg.hits = ctBEu16( static_cast<u16>(v) );

        v = chr->vital(uoclient_general.hits.id).maximum_ones();
        if (v > 0xFFFF)
            v = 0xFFFF;
        msg.max_hits = ctBEu16( static_cast<u16>(v) );
    }
    else
    {
        msg.hits = 0;
        msg.max_hits = 0;
    }
	
	msg.renameable = 0; // (client->chr->can_rename( chr ) ? 0xFF : 0);

	if (chr->race == RACE_ELF)
		msg.gender = chr->gender | FLAG_RACE;
	else
		msg.gender = chr->gender;
	
    if (uoclient_general.strength.any)
    {
        long v = chr->attribute(uoclient_general.strength.id).effective();
        if (v > 0xFFFF)
            v = 0xFFFF;
       
        msg.str = ctBEu16( static_cast<u16>(v) );
    }
    else
    {
        msg.str = 0;
    }

    if (uoclient_general.dexterity.any)
    {
        long v = chr->attribute(uoclient_general.dexterity.id).effective();
        if (v > 0xFFFF)
            v = 0xFFFF;
       
        msg.dex = ctBEu16( static_cast<u16>(v) );
    }
    else
    {
        msg.dex = 0;
    }
    
    if (uoclient_general.intelligence.any)
    {
        long v = chr->attribute(uoclient_general.intelligence.id).effective();
        if (v > 0xFFFF)
            v = 0xFFFF;
       
        msg.intel = ctBEu16( static_cast<u16>(v) );
    }
    else
    {
        msg.intel = 0;
    }
	
    if (uoclient_general.stamina.any)
    {
        long v = chr->vital(uoclient_general.stamina.id).current_ones();
        if (v > 0xFFFF)
            v = 0xFFFF;
        msg.stamina = ctBEu16( static_cast<u16>(v) );

        v = chr->vital( uoclient_general.stamina.id ).maximum_ones();
        if (v > 0xFFFF)
            v = 0xFFFF;

	    msg.max_stamina = ctBEu16( static_cast<u16>(v) );
    }
    else
    {
        msg.stamina = 0;
	    msg.max_stamina = 0;
    }
	
    if (uoclient_general.mana.any)
    {
        long v = chr->vital(uoclient_general.mana.id).current_ones();
        if (v > 0xFFFF)
            v = 0xFFFF;
        msg.mana = ctBEu16( static_cast<u16>(v) );

        v = chr->vital(uoclient_general.mana.id).maximum_ones();
        if (v > 0xFFFF)
            v = 0xFFFF;
	    msg.max_mana = ctBEu16( static_cast<u16>(v) );
    }
    else
    {
        msg.mana = 0;
	    msg.max_mana = 0;
    }

	
    msg.gold = ctBEu32( chr->gold_carried() );
	msg.AR = ctBEu16( chr->ar() );
	msg.weight = ctBEu16( static_cast<u16>(chr->weight()) );

	// Add AOS stuff here if required.
	if( (client->UOExpansionFlag & AOS) )
	{
		msg.statcap = ctBEu16( uoclient_general.statcap );
		msg.followers = 0;
		msg.followers_max = 0;
		msg.fireresist = 0;
		msg.coldresist = 0;
		msg.poisonresist = 0;
		msg.energyresist = 0;
		msg.luck = 0;
		msg.damage_min = 0;
		msg.damage_max = 0;
		msg.titching = 0;
	}

	transmit(client, &msg, cfBEu16(msg.msglen) );
}

void send_short_statmsg( Client *client, Character *chr )
{
	unsigned short msglen = offsetof( MSG11_STATS_WINDOW, gender );

	MSG11_STATS_WINDOW msg;
	msg.msgtype = MSG11_STATS_WINDOW_ID;
	msg.msglen = ctBEu16(msglen);
	msg.serial = chr->serial_ext;
	strzcpy( msg.name, chr->name().c_str(), sizeof msg.name );

    if (uoclient_general.hits.any)
    {
		long h, mh;

        h = chr->vital(uoclient_general.hits.id).current_ones();
        if (h > 0xFFFF)
            h = 0xFFFF;
        //msg.hits = ctBEu16( static_cast<u16>(h) );

        mh = chr->vital(uoclient_general.hits.id).maximum_ones();
        if (mh > 0xFFFF)
            mh = 0xFFFF;
        //msg.max_hits = ctBEu16( static_cast<u16>(mh) );

		msg.hits = ctBEu16( static_cast<u16>(h * 1000 / mh) );
		msg.max_hits = ctBEu16( 1000 );
    }
    else
    {
        msg.hits = 0;
        msg.max_hits = 0;
    }

    msg.renameable = (client->chr->can_rename( chr ) ? 0xFF : 0);
	msg.moreinfo = 0;

	transmit(client, &msg, msglen );
}

void send_update_hits_to_inrange( Character *chr )
{
	MSGA1_UPDATE_STATS_ST_HP msg;
	msg.msgtype = MSGA1_UPDATE_STATS_ST_HP_ID;
	msg.serial = chr->serial_ext;
	long h, mh;
    if (uoclient_general.hits.any)
    {
        h = chr->vital(uoclient_general.hits.id).current_ones();
        if (h > 0xFFFF)
            h = 0xFFFF;
        msg.hits = ctBEu16( static_cast<u16>(h) );

        mh = chr->vital(uoclient_general.hits.id).maximum_ones();
        if (mh > 0xFFFF)
            mh = 0xFFFF;
        msg.max_hits = ctBEu16( static_cast<u16>(mh) );

		// Send proper data to self (if we exist?)
		if (chr->client && chr->client->ready)
			transmit( chr->client, &msg, sizeof msg );

		// To stop "HP snooping"...
		msg.hits = ctBEu16( static_cast<u16>(h * 1000 / mh) );
		msg.max_hits = ctBEu16( 1000 );
    }
    else
    {
        msg.hits = 0;
        msg.max_hits = 0;
		if (chr->client && chr->client->ready)
			transmit( chr->client, &msg, sizeof msg );
    }
	//msg.str = ctBEu16( chr->strength() );
	//msg.hits = ctBEu16( chr->get_hits() );

	// Exclude self... otherwise their status-window shows 1000 hp!! >_<
	transmit_to_others_inrange( chr, &msg, sizeof msg );

}
