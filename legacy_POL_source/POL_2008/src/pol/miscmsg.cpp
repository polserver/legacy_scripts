// History
//   2005/11/25 MuadDib: Added MSGBF_GENERIC_COMMAND::TYPE_SESPAM: to do away with spam.
//   2005/11/23 MuadDib: Altered handle_mode_set for 0x72 Packet. Nows reads
//                       from combat.cfg for warmode_wait object. Sends the
//                       0x77 Packet and returns 0 when out of timer.
//   2005/12/09 MuadDib: Added TYPE_CLIENT_LANGUAGE for setting member uclang.
//   2006/05/24 Shinigami: Added MSGBF_GENERIC_COMMAND::TYPE_CHARACTER_RACE_CHANGER to support Elfs
//   2006/05/30 Shinigami: Changed params of character_race_changer_handler()

#include "clib/stl_inc.h"

/* MISCMSG.CPP: Miscellaneous message handlers.  Handlers shouldn't stay here long, 
   only until they find a better home - but this is better than putting them in POL.CPP. */
#include <stddef.h>

#include "clib/clib.h"
#include "clib/endian.h"
#include "clib/fdump.h"
#include "clib/logfile.h"
#include "clib/strutil.h"

#include "account.h"
#include "charactr.h"
#include "client.h"
#include "cmbtcfg.h"
#include "customhouses.h"
#include "fnsearch.h"
#include "msghandl.h"
#include "multi.h"
#include "nmsgtype.h"
#include "spells.h"
#include "tooltips.h"
#include "uvars.h"
#include "umsgtype.h"
#include "ufunc.h"
#include "sockio.h"

void handle_unknown_packet( Client* client );
void character_race_changer_handler( Client* client, MSGBF_GENERIC_COMMAND* msg );

void handle_bulletin_boards( Client* client, MSG71_BULLETIN_BOARD* msg )
{
	handle_unknown_packet( client );
}
MESSAGE_HANDLER_VARLEN( MSG71_BULLETIN_BOARD, handle_bulletin_boards );

void handle_mode_set( Client *client, MSG72_MODE_SET *msg )
{
	if ( client->chr->warmode_wait > read_gameclock() )
	{
        send_move( client, client->chr );
		return;
	}
	else
	{
		client->chr->warmode_wait = read_gameclock() + combat_config.warmode_delay;
	}

	bool msg_warmode = msg->warmode ? true : false;

	// FIXME? Should reply with 0x77 packet!? (so says various docs!) [TJ]
	transmit( client, msg, sizeof *msg );

    client->chr->set_warmode( msg_warmode );
}
MESSAGE_HANDLER( MSG72_MODE_SET, handle_mode_set );

void handle_rename_char( Client* client, MSG75_RENAME_CHAR* msg )
{
    Character* chr = find_character( cfBEu32( msg->serial ));
    if (chr != NULL)
    {
        if (client->chr->can_rename( chr ))
        {
            msg->name[ sizeof msg->name-1 ] = '\0';
            // check for legal characters
			for( char* p = msg->name; *p; p++ )
			{
				// only allow: a-z, A-Z & spaces
				if ( *p != ' ' && !isalpha(*p) )
				{
					char buffer[512];
					sprintf(buffer, "Client #%lu (account %s) attempted an invalid rename (packet 0x%2.02x):",
									client->instance_,
									(client->acct != NULL) ? client->acct->name() : "unknown",
									msg->msgtype);
					cout << buffer << endl;
					fdump( stdout, msg->name, static_cast<int>(strlen(msg->name)) );

					if (logfile)
					{
						Log("%s\n", buffer);
						fdump( logfile, msg->name, static_cast<int>(strlen(msg->name)) );
					}

					*p = '\0';
					send_sysmessage( client, "Invalid name!" );
					return; //dave 12/26 if invalid name, do not apply to chr!
				}
			}
            chr->setname( msg->name );
        }
        else
        {
            send_sysmessage( client, "I can't rename that." );
        }
    }
    else
    {
        send_sysmessage( client, "I can't find that." );
    }
}
MESSAGE_HANDLER( MSG75_RENAME_CHAR, handle_rename_char );

void handle_msg_B8( Client* client, MSGB8_CHARACTER_PROFILE* msg )
{
    handle_unknown_packet( client );
}
MESSAGE_HANDLER_VARLEN( MSGB8_CHARACTER_PROFILE, handle_msg_B8 );

void handle_msg_BB( Client* client, MSGBB_ULTIMA_MESSENGER* msg )
{
    handle_unknown_packet( client );
}
MESSAGE_HANDLER( MSGBB_ULTIMA_MESSENGER, handle_msg_BB );

void handle_client_version( Client* client, MSGBD_CLIENT_VERSION* msg )
{
    u16 len = cfBEu16(msg->msglen)-3;
    if (len < 100)
    {
	    string ver2(msg->version, len); //dave 2/11/3 version is variable length :P
        client->setversion( ver2 );
		send_season_info( client ); // Scott 10/11/2007 added for login fixes and handling 1.x clients.
									// Season info needs to check client version to keep from crashing 1.x
									// version not set until shortly after login complete.
		//send_feature_enable(client); //dave commented out 8/21/03, unexpected problems with people sending B9 via script with this too.
////////aostest
if( (client->UOExpansionFlag & AOS) )
{
    send_object_cache(client, dynamic_cast<const UObject*>(client->chr));
}
////////
    }
    else
    {
        Log2( "Suspect string length in MSGBD_CLIENT_VERSION packet: %u\n", (unsigned) len );
    }
}
MESSAGE_HANDLER_VARLEN( MSGBD_CLIENT_VERSION, handle_client_version );

void handle_msg_BF( Client* client, MSGBF_GENERIC_COMMAND* msg )
{
    UObject* obj = NULL;
    UMulti* multi = NULL;
    UHouse* house = NULL;
    switch(cfBEu16(msg->subcmd))
    {
		case MSGBF_GENERIC_COMMAND::TYPE_CLIENT_LANGUAGE:
			client->chr->uclang = strlower(msg->client_lang);
			break;
        case MSGBF_GENERIC_COMMAND::TYPE_REQ_FULL_CUSTOM_HOUSE:
            if( (client->UOExpansionFlag & AOS) == 0 )
                return;
            multi = system_find_multi(cfBEu32(msg->reqfullcustomhouse.house_serial));
            if(multi != NULL)
            {
                house = multi->as_house();
                if(house != NULL)
                {
					if(client->UOExpansionFlag & AOS)
					{
						send_object_cache(client, (UObject*)(house));
					}
                    //consider sending working design to certain players, to assist building, or GM help
                    CustomHousesSendFull( house, client, HOUSE_DESIGN_CURRENT );
                }
            }
            break;
        case MSGBF_GENERIC_COMMAND::TYPE_OBJECT_CACHE:
            if( (client->UOExpansionFlag & AOS) == 0)
                return;
            obj = system_find_object(cfBEu32(msg->objectcache.serial));
            if(obj != NULL)
            {
                SendAOSTooltip(client,obj);
            }
            break;
		case MSGBF_GENERIC_COMMAND::TYPE_SESPAM:
			return;
			break;
		case MSGBF_GENERIC_COMMAND::TYPE_SPELL_SELECT:
			do_cast(client, cfBEu16(msg->spellselect.selected_spell) );
			break;
		case MSGBF_GENERIC_COMMAND::TYPE_CHARACTER_RACE_CHANGER:
			character_race_changer_handler( client, msg );
			break;
		default:
            handle_unknown_packet( client );
    }
}
MESSAGE_HANDLER_VARLEN( MSGBF_GENERIC_COMMAND, handle_msg_BF );

void handle_unknown_C4( Client* client, MSGC4_SEMIVISIBLE* msg )
{
	handle_unknown_packet( client );
}
MESSAGE_HANDLER( MSGC4_SEMIVISIBLE, handle_unknown_C4 );

void handle_update_range_change( Client* client, MSGC8_UPDATE_RANGE_CHANGE* msg )
{
	handle_unknown_packet( client );
}
MESSAGE_HANDLER( MSGC8_UPDATE_RANGE_CHANGE, handle_update_range_change );

void handle_allnames( Client *client, MSG98_ALLNAMES *msg )
{

	u32 serial = cfBEu32( msg->serial );
	Character *the_mob = find_character( serial );
	if (the_mob != NULL)
	{
		if (!client->chr->is_visible_to_me(the_mob)) {
			return;
		}
		if (pol_distance(client->chr->x, client->chr->y, the_mob->x, the_mob->y) > 20) {
			return;
		}

		MSGOUT_98_ALLNAMES msgout;
		msgout.msgtype = 0x98;
		msgout.msglen = ctBEu16(0x25);  // 0x25 = 37. Static Length.
		msgout.serial = the_mob->serial_ext;
		strzcpy( msgout.name, the_mob->name().c_str(), sizeof msgout.name );
		transmit( client, &msgout, sizeof msgout );

	}
	else
	{
		return;
	}
}
MESSAGE_HANDLER( MSG98_ALLNAMES, handle_allnames );

