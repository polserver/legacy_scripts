// History
//   2005/02/23 Shinigami: ServSpecOpt DecayItems to enable/disable item decay
//   2005/04/03 Shinigami: send_feature_enable() call moved from start_client_char()
//						 to send_start() to send before char selection
//   2005/04/04 Shinigami: added can_delete_character( chr, delete_by )
//   2005/06/15 Shinigami: ServSpecOpt UseWinLFH to enable/disable Windows XP/2003 low-fragmentation Heap
//						 added Check_for_WinXP_or_Win2003() and low_fragmentation_Heap()
//   2005/06/20 Shinigami: added llog (needs defined MEMORYLEAK)
//   2005/07/01 Shinigami: removed Check_for_WinXP_or_Win2003() and transformed call of 
//						 Use_low_fragmentation_Heap() into Run-Time Dynamic Linking
//   2005/10/13 Shinigami: added Check_libc_version() and printing a Warning if libc is to old
//   2005/11/25 Shinigami: MSGBF_GENERIC_COMMAND::TYPE_SESPAM will not block Inactivity-Check
//   2005/11/28 MuadDib: Created check_inactivity() bool function to handle checking packets
//					   for ones to be considered "ignored" for inactivity. Returns true if
//					   the packet was one to be ignored.
//   2005/11/28 MuadDib: Implemented check_inactivity() function in appropriate place.
//   2006/03/01 MuadDib: Added connect = true to start_client_char so char creation can use.
//   2006/03/03 MuadDib: Moved all instances of connected = true to start_client_char.
//   2006/06/03 Shinigami: added little bit more logging @ pthread_create
//   2006/06/05 Shinigami: added little bit more logging @ Client disconnects by Core
//   2006/07/05 Shinigami: moved MakeDirectory("log") a little bit up
//   2006/10/07 Shinigami: FreeBSD fix - changed some __linux__ to __unix__
//   2007/03/08 Shinigami: added pthread_exit and _endhreadex to close threads

#include "clib/stl_inc.h"
#ifdef _MSC_VER
#pragma warning( disable: 4786 )
#endif


#include <assert.h>

#ifdef _WIN32
#include <process.h>
#endif

#ifdef __unix__
#include <execinfo.h>
#endif

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "dtrace.h"
#include "clib/dualbuf.h"
#include "clib/endian.h"
#include "clib/esignal.h"
#include "clib/fdump.h"
#include "clib/fileutil.h"
#include "clib/logfile.h"
#include "clib/MD5.h"
#include "clib/mlog.h"
#include "clib/passert.h"
#include "clib/random.h"
#include "clib/stlutil.h"
#include "clib/strexcpt.h"
#include "clib/strutil.h"
#include "clib/threadhelp.h"
#include "clib/tracebuf.h"

#include "plib/pkg.h"
#include "plib/realm.h"

#include "account.h"
#include "allocd.h"
#include "boat.h"
#include "cgdata.h"
#include "charactr.h"
#include "checkpnt.h"
#include "client.h"
#include "cliface.h"
#include "core.h"
#include "crypwrap.h"
#include "decay.h"
#include "gameclck.h"
#include "gflag.h"
#include "guardrgn.h"
#include "itemdesc.h"
#include "lightlvl.h"
#include "loadunld.h"
#include "logfiles.h"
#include "miscrgn.h"
#include "msghandl.h"
#include "msgfiltr.h"
#include "musicrgn.h"
#include "multi.h"
#include "nmsgtype.h"
#include "objecthash.h"
#include "objtype.h"
#include "realms.h"
#include "packethooks.h"
#include "polclock.h"
#include "polcfg.h"
#include "poldbg.h"
#include "polfile.h"
#include "polsig.h"
#include "poltest.h"
#include "plib/polver.h"
#include "polwww.h"
#include "profile.h"
#include "readcfg.h"
#include "savedata.h"
#include "schedule.h"
#include "scrdef.h"
#include "scrsched.h"
#include "scrstore.h"
#include "sockets.h"
#include "sockio.h"
#include "ssopt.h"
#include "ucrypto.h"
#include "uobjcnt.h"
#include "uofile.h"
#include "uoscrobj.h"
#include "ufunc.h"
#include "umsgtype.h"
#include "uvars.h"
#include "uworld.h"

#ifdef __linux__
#include <gnu/libc-version.h>
#endif

extern void cleanup_vars();

using namespace threadhelp;

ofstream start_log;
dualbuf db_cout;
dualbuf db_cerr;

#include "polsem.h"
#define CLIENT_CHECKPOINT(x) client->checkpoint = x
SOCKET listen_socket;
fd_set listen_fd;
struct timeval listen_timeout = { 0, 0 };

fd_set recv_fd;
fd_set err_fd;
fd_set send_fd;
struct timeval select_timeout = { 0, 0 };

typedef struct {
	int msglen; // if 0, no message handler defined.
	void (*func)(Client *client, void *msg);
} MSG_HANDLER;
MSG_HANDLER handler[ 256 ];

MessageHandler::MessageHandler( unsigned char msgtype,
								int msglen,
								void (*func)(Client *client, void *msg) )
{
	passert( msglen != 0 );
	/*
	if (handler[msgtype].msglen)
	{
		// DIE 
		cerr << "WTF!  Message Handler 0x" << std::hex << msgtype << std::dec << " multiply defined!" << endl;
		exit(1);
	}
	*/
	handler[ msgtype ].func = func;
	handler[ msgtype ].msglen = msglen;
}

void send_startup( Client *client )
{
	Character *chr = client->chr;
	MSG1B_STARTUP msg;
	memset( &msg, 0, sizeof msg );
	msg.msgtype = MSGOUT_1B_STARTUP;
	msg.serial = chr->serial_ext;
	msg.graphic = chr->graphic_ext;
	msg.x = ctBEu16(chr->x);
	msg.y = ctBEu16(chr->y);
	msg.z = chr->z;
	msg.dir = chr->facing;
	msg.unk21_7F = 0x7F;
	msg.map_width = ctBEu16(client->chr->realm->width());	
	msg.map_height = ctBEu16(client->chr->realm->height());
	transmit( client, &msg, sizeof msg );
}

// Returns true for "inactivity"
// FIXME: Should be optimized to switch/case function instead
//		if possible.
bool check_inactivity( Client* client )
{
	if ((client->buffer[0] == MSG09_SINGLE_CLICK_ID)) 
	{
		return true;
	}
	if ((client->buffer[0] == MSG73_KEEP_ALIVE_ID) || (((client->buffer[0] == MSGBF_GENERIC_COMMAND_ID) &&
		(client->buffer[3] == 0) && (client->buffer[4] == MSGBF_GENERIC_COMMAND::TYPE_SESPAM))))
	{
		return true;
	}

	return false;
}

void send_betaclient_BF( Client* client )
{
	if (config.client_encryption_version.substr( 0,2 ) == "2.")
	{
		MSGBF_GENERIC_COMMAND msg;
		u16 msglen = offsetof( MSGBF_GENERIC_COMMAND, cursorhue ) + 1;
		msg.msgtype = MSGBF_GENERIC_COMMAND_ID;
		msg.msglen = ctBEu16( msglen );
		msg.subcmd = ctBEu16( MSGBF_GENERIC_COMMAND::TYPE_CURSOR_HUE );
		msg.cursorhue = MSGBF_GENERIC_COMMAND::CURSORHUE_TRAMMEL;
		client->transmit( &msg, msglen );
	}
}

void send_inrange_items( Client* client )
{
	unsigned short wxL, wyL, wxH, wyH;
	zone_convert_clip( client->chr->x - RANGE_VISUAL, client->chr->y - RANGE_VISUAL, client->chr->realm, wxL, wyL );
	zone_convert_clip( client->chr->x + RANGE_VISUAL, client->chr->y + RANGE_VISUAL, client->chr->realm, wxH, wyH );
	for( unsigned short wx = wxL; wx <= wxH; ++wx )
	{
		for( unsigned short wy = wyL; wy <= wyH; ++wy )
		{
			ZoneItems& witem = client->chr->realm->zone[wx][wy].items;
			for( ZoneItems::iterator itr = witem.begin(), end = witem.end(); itr != end; ++itr )
			{
				Item* item = *itr;
				if (inrange( client->chr, item ))
				{
					send_item( client, item );
				}
			}
		}
	}

}

void send_inrange_multis( Client* client )
{
	unsigned short wxL, wyL, wxH, wyH;
	zone_convert_clip( client->chr->x - RANGE_VISUAL, client->chr->y - RANGE_VISUAL, client->chr->realm, wxL, wyL );
	zone_convert_clip( client->chr->x + RANGE_VISUAL, client->chr->y + RANGE_VISUAL, client->chr->realm, wxH, wyH );
	for( unsigned short wx = wxL; wx <= wxH; ++wx )
	{
		for( unsigned short wy = wyL; wy <= wyH; ++wy )
		{
			ZoneMultis& wmulti = client->chr->realm->zone[wx][wy].multis;
			for( ZoneMultis::iterator itr = wmulti.begin(), end = wmulti.end(); itr != end; ++itr )
			{
				UMulti* multi = *itr;
				if (inrange( client->chr, multi ))
				{
					send_multi( client, multi );
				}
			}
		}
	}

}

void textcmd_startlog( Client* client );
void textcmd_stoplog( Client* client );
void start_client_char( Client *client )
{
	client->ready = 1;
	client->chr->connected = true;
	//even if this stuff just gets queued, we still want the client to start
	// getting data now. 

	client->pause();
	
	UMulti* supporting_multi;
	Item* walkon;
	int newz;
	if (client->chr->realm->walkheight( client->chr, client->chr->x, client->chr->y, client->chr->z,
					&newz, &supporting_multi, &walkon ))
	{
		client->chr->z = newz;
		if (supporting_multi != NULL)
		{
			supporting_multi->register_object( client->chr );
		}
		client->chr->position_changed();
	}

	// send_feature_enable( client ); // Shinigami: moved to send_start() to send before char selection
	send_startup( client );

	send_realm_change( client, client->chr->realm );
	send_map_difs( client );
	send_season_info( client );
	// send_feature_enable() should be here from OSI testing? Not putting here yet
	// while testing till I find out why it was removed from this function.  - MuadDib

	client->chr->lastx = client->chr->lasty = client->chr->lastz = 0;

	client->gd->music_region = musicdef->getregion(0,0, client->chr->realm);		//dave 1/13/3 define inital regions for characters or null checking prevents any region changes.
	client->gd->justice_region = justicedef->getregion(0,0, client->chr->realm);	//
	client->gd->weather_region = weatherdef->getregion(0,0, client->chr->realm);	//

	send_goxyz( client, client->chr );
	client->chr->check_region_changes();

	client->chr->send_warmode();
	login_complete(client);// Moved up here before the tellmove to fix problem described below.
	client->chr->tellmove();

	// Put weather & season check here per OSI testing  - MuadDib
	client->chr->check_weather_region_change(true);
	send_season_info( client );

	send_objects_newly_inrange( client );
	
	// Moved higher per OSI testing - MuadDib
	// client->chr->send_warmode();
	client->chr->send_highlight();
	send_owncreate( client, client->chr );

	// Moved higher up per OSI testing. - MuadDib
	// client->chr->check_region_changes();

	send_goxyz( client, client->chr );

	//send_techstuff( client );				//dave commented out 3/25/3 - msg 0x69 causes client 4.0.0e to hang, and is obselete. just need to make sure we send 0x55 (login_complete, below)
	//send_betaclient_BF(client);
	client->restart();

	client->chr->clear_gotten_item();

//  Moved login_complete higher to prevent weather regions from messing up client 
//  spell icon packets(made it look like it was raining spell icons from spellbook if logged 
//  into a weather region with rain.
//	login_complete(client);
}


void call_chr_scripts( Character* chr, const string& root_script_ecl, const string& pkg_script_ecl, bool offline = false )
{
	ScriptDef sd;
	sd.quickconfig( root_script_ecl );

	if (sd.exists())
	{
		call_script( sd, offline ? new EOfflineCharacterRefObjImp(chr) : new ECharacterRefObjImp( chr ) );
	}

	for( Packages::iterator itr = packages.begin(); itr != packages.end(); ++itr )
	{
		Package* pkg = *itr;

		sd.quickconfig( pkg, pkg_script_ecl );
		if (sd.exists())
		{
			call_script( sd, offline ? new EOfflineCharacterRefObjImp(chr) : new ECharacterRefObjImp( chr ) );
		}
	}
}

void run_logon_script( Character* chr )
{
	call_chr_scripts( chr, "scripts/misc/logon.ecl", "logon.ecl" );
}
void run_reconnect_script( Character* chr )
{
	call_chr_scripts( chr, "scripts/misc/reconnect.ecl", "reconnect.ecl" );
}
bool can_delete_character( Character* chr, int delete_by )
{
	ScriptDef sd;
	sd.quickconfig( "scripts/misc/candelete.ecl" );
	
	if (sd.exists())
	{
		return call_script( sd, new EOfflineCharacterRefObjImp(chr), new BLong( delete_by ) );
	}
	else
	{
		return true;
	}
}
void call_ondelete_scripts( Character* chr )
{
	call_chr_scripts( chr, "scripts/misc/ondelete.ecl", "ondelete.ecl", true );
}

// consider moving most of this into a function, so character
// creation can use the same code.
void char_select( Client *client, MSG5D_CHAR_SELECT *msg )
{
	bool reconnecting = false;

	if ((msg->charidx >= MAX_CHARS) ||
		(client->acct->get_character( msg->charidx ) == NULL))
	{
		send_login_error( client, LOGIN_ERROR_MISC );
		client->disconnect = 1;
		return;
	}

	Character *chosen_char = client->acct->get_character( msg->charidx );

	Log( "Account %s selecting character %s\n", 
		 client->acct->name(),
		 chosen_char->name().c_str() );

	if (config.min_cmdlevel_to_login > chosen_char->cmdlevel)
	{
		Log( "Account %s with character %s doesn't fit MinCmdlevelToLogin from pol.cfg. Client disconnected by Core.\n",
			client->acct->name(),
			chosen_char->name().c_str() );

		send_login_error( client, LOGIN_ERROR_MISC );
		client->disconnect = 1;
		return;
	}

	//Dave moved this from login.cpp so client cmdlevel can be checked before denying login
	if ( ( (std::count_if(clients.begin(),clients.end(),clientHasCharacter)) >= config.max_clients) && 
		   (chosen_char->cmdlevel < config.max_clients_bypass_cmdlevel) )
	{
		Log( "To much clients connected. Check MaximumClients and/or MaximumClientsBypassCmdLevel in pol.cfg.\n"
			"Account %s with character %s Client disconnected by Core.\n",
			client->acct->name(),
			chosen_char->name().c_str() );

		send_login_error( client, LOGIN_ERROR_MISC );
		client->disconnect = 1;
		return;
	}

	if (client->acct->active_character != NULL) // this account has a currently active character.
	{
		 // if it's not the one that was picked, refuse to start this one.
		if (client->acct->active_character != chosen_char)
		{
			send_login_error( client, LOGIN_ERROR_OTHER_CHAR_INUSE );
			client->disconnect = 1;
			return;
		}
		
		// we're reattaching to a character that is in-game.  If there is still
		// a client attached, disconnect it.
		if (chosen_char->client)
		{
			chosen_char->client->gd->clear();
			chosen_char->client->disconnect = 1;
			chosen_char->client->ready = 0;
			chosen_char->client->msgtype_filter = &disconnected_filter;


			// disassociate the objects from each other.
			chosen_char->client->acct = NULL;
			chosen_char->client->chr = NULL;

			chosen_char->client = NULL;
		}
		reconnecting = true;
	}
	else
	{
		// logging in a character that's offline.
		SetCharacterWorldPosition( chosen_char );
		chosen_char->logged_in = true;
	}
	
//	chosen_char->connected = true;  -> MuadDib removed. Moved call to start_client_char
	client->acct->active_character = chosen_char;
	client->chr = chosen_char;
	chosen_char->client = client;
	chosen_char->acct.set(client->acct);

	client->msgtype_filter = &game_filter;
	start_client_char( client );

	// 8-26-05  Austin
	// Needed to set lastx and lasty to fix a bug that occurs if a mobile
	// is moved to another realm after logging in and LEFTAREA is sent to the wrong place.
	if ( !chosen_char->lastx && !chosen_char->lasty )
	{
		chosen_char->lastx = chosen_char->x;
		chosen_char->lasty = chosen_char->y;
	}

	if ( !reconnecting )
		run_logon_script( chosen_char );
	else
		run_reconnect_script( chosen_char );

}
MESSAGE_HANDLER( MSG5D_CHAR_SELECT, char_select );

void send_client_char_data( Character *chr, Client *client );
void handle_resync_request( Client* client, MSG22_RESYNC_REQUEST* msg )
{
	send_goxyz( client, client->chr );

	client->send_pause(); //dave removed force=true 5/10/3, let uoclient.cfg option determine xflow packets (else this hangs 4.0.0e clients)

	ForEachMobileInVisualRange( client->chr, send_client_char_data, client );

	send_inrange_items( client );
	send_inrange_multis( client );

	client->send_restart();//dave removed force=true 5/10/3
}
MESSAGE_HANDLER( MSG22_RESYNC_REQUEST, handle_resync_request );

void handle_keep_alive( Client *client, MSG73_KEEP_ALIVE *msg )
{
	// cout << "keep-alive" << endl;
	transmit(client, msg, sizeof *msg );
}
MESSAGE_HANDLER( MSG73_KEEP_ALIVE, handle_keep_alive );

void handle_unknown_packet( Client* client )
{
	if (config.display_unknown_packets)
	{
		printf( "Unknown packet type 0x%2.02x: %d bytes (IP:%s, Account:%s)\n", 
					client->buffer[0], client->bytes_received,
					client->ipaddrAsString().c_str(), (client->acct != NULL)? client->acct->name():"None" );

		if (client->bytes_received <= 64)
			fdump( stdout, client->buffer, client->bytes_received );

		if (logfile)
		{
			fprintf( logfile, "Unknown packet type 0x%2.02x: %d bytes (IP:%s, Account:%s)\n", 
					client->buffer[0], client->bytes_received,
					client->ipaddrAsString().c_str(), (client->acct != NULL)? client->acct->name():"None"  );
			fdump( logfile, client->buffer, client->bytes_received );
		}
	}
}

void just_ignore_message( Client *client )
{
	printf( "Ignoring message type 0x%2.02x: %d bytes (IP:%s, Account:%s)\n", 
				client->buffer[0], client->bytes_received,
				client->ipaddrAsString().c_str(), (client->acct != NULL)? client->acct->name():"None"  );

	if (client->bytes_received <= 64)
		fdump( stdout, client->buffer, client->bytes_received );

	if (logfile)
	{
		fprintf( logfile, "Ignored message type 0x%2.02x: %d bytes (IP:%s, Account:%s)\n", 
				client->buffer[0], client->bytes_received,
				client->ipaddrAsString().c_str(), (client->acct != NULL)? client->acct->name():"None" );
		fdump( logfile, client->buffer, client->bytes_received );
	}
}

void ignore_69( Client *client, MSG69_UNKNOWN *msg )
{
	// just_ignore_message( client );
}
MESSAGE_HANDLER_VARLEN( MSG69_UNKNOWN, ignore_69 );

void restart_all_clients()
{
	for( Clients::iterator itr = clients.begin(), end = clients.end();
		 itr != end;
		 ++itr )
	{
		Client* client = (*itr);
		if (client->pause_count)
		{
			client->restart2();
		}
	}
}


// bool - return true when a message was processed.
bool process_data( Client *client )
{
	// NOTE: This is coded such that for normal messages, which are completely available,
	// this function will get the type, then the length, then the data, without having
	// to wait for a second or third call.
	// Also, the abnormal state, RECV_STATE_CRYPTSEED_WAIT, is handled at the end, so in 
	// normal processing its code doesn't see the code path.
	passert( client->bufcheck1_AA == 0xAA );
	passert( client->bufcheck2_55 == 0x55 );
	if (client->recv_state == Client::RECV_STATE_MSGTYPE_WAIT )
	{
		client->bytes_received = 0;
		
		client->recv_remaining( 1 );
		CLIENT_CHECKPOINT(22);
		if (client->bytes_received < 1) // this really should never happen.
		{
			client->disconnect = 1;
			return false;
		}

		unsigned char msgtype = client->buffer[0];
		client->last_msgtype = msgtype; //CNXBUG
		if (config.verbose)
			printf( "Incoming msg type: %x\n", msgtype );

		if (!handler[ msgtype ].msglen)
		{
			printf( "Undefined message type %2.02x\n", (unsigned char) msgtype );
			client->recv_remaining( sizeof client->buffer/2 );
			printf( "Unexpected message type %2.02x, %d bytes (IP:%s, Account:%s)\n", 
				(unsigned char) msgtype, client->bytes_received,
				client->ipaddrAsString().c_str(), (client->acct != NULL)? client->acct->name():"None" );
			
			if (logfile)
			{
				fprintf( logfile, 
						 "Client#%lu: Unexpected message type %2.02x, %d bytes (IP:%s, Account:%s)\n", 
						 client->instance_,
						 (unsigned char) msgtype, 
						 client->bytes_received,
						 client->ipaddrAsString().c_str(), (client->acct != NULL)? client->acct->name():"None" );
				fdump( logfile, client->buffer, client->bytes_received );
			}
			
			if (client->bytes_received <= 64)
				fdump( stdout, client->buffer, client->bytes_received );
			
			// remain in RECV_STATE_MSGTYPE_WAIT

			return false;
		}
		if (handler[msgtype].msglen == MSGLEN_2BYTELEN_DATA)
		{
			client->recv_state = Client::RECV_STATE_MSGLEN_WAIT;
		}
		else // (handler[msgtype].msglen > 0) must be true
		{
			client->recv_state = Client::RECV_STATE_MSGDATA_WAIT;
			client->message_length = handler[msgtype].msglen;
		}

	}

	if (client->recv_state == Client::RECV_STATE_MSGLEN_WAIT)
	{
		client->recv_remaining( 3 );
		CLIENT_CHECKPOINT(23);
		if (client->bytes_received == 3) // the length bytes were available.
		{
			// MSG is [MSGTYPE] [LENHI] [LENLO] [DATA ... ]
			client->message_length = (client->buffer[1] << 8)+client->buffer[2]; 
			if (client->message_length > sizeof client->buffer)
			{
				Log2( "Client#%lu: Too-long message length of %u\n", 
					 client->instance_,
					 client->message_length );
				client->message_length = sizeof client->buffer;
			}
			else if (client->message_length < 3)
			{
				Log2( "Client#%lu: Too-short message length of %u\n",
					 client->instance_,
					 client->message_length );
				client->message_length = 3;
			}
			client->recv_state = Client::RECV_STATE_MSGDATA_WAIT;
		}
		// else keep waiting. 
	}

	if (client->recv_state == Client::RECV_STATE_MSGDATA_WAIT)
	{
		CLIENT_CHECKPOINT(24);
		client->recv_remaining( client->message_length );
		CLIENT_CHECKPOINT(25);
		if (client->bytes_received == client->message_length) // we have the whole message
		{
			unsigned char msgtype = client->buffer[0];
			if (client->fpLog != NULL)
			{
				PolLock lck;
				fprintf( client->fpLog, "Client -> Server: 0x%X, %d bytes\n", msgtype, client->message_length );
				fdump( client->fpLog, &client->buffer, client->message_length );
				fprintf( client->fpLog, "\n" );
			}   

			if (config.verbose)
				printf( "Message Received: Type 0x%X, Length %d bytes\n", msgtype, client->message_length );
			//if (logfile)
			//  fdump( logfile, client->buffer, client->message_length );

			PolLock lck;
			// it can happen that a client gets disconnected while waiting for the lock.
			if (!client->disconnect)
			{
				if (client->msgtype_filter->msgtype_allowed[ msgtype])
				{
					try {
						dtrace(10) << "Client#" << client->instance_ << ": message " << hexint( static_cast<unsigned short>(msgtype)) << endl;
						CLIENT_CHECKPOINT(26);
						(*handler[msgtype].func)(client, client->buffer);
						CLIENT_CHECKPOINT(27);
						restart_all_clients();
					}
					catch( std::exception& ex )
					{
						Log2( "Client#%lu: Exception in message handler 0x%02.02x: %s\n",
							 client->instance_,
							 msgtype,
							 ex.what());
						if (logfile)
							fdump( logfile, client->buffer, client->bytes_received );
						restart_all_clients();
						throw;
					}
				}
				else
				{ // inet_addr in_addr
					Log2( "Client#%lu (%s, Acct %s) sent non-allowed message type %x.\n",
							client->instance_,
							AddressToString( &client->ipaddr ), 
							client->acct ? client->acct->name() : "unknown",
							msgtype );
				}
			}
			client->recv_state = Client::RECV_STATE_MSGTYPE_WAIT;
			CLIENT_CHECKPOINT(28);
			return true;
		}
		// else keep waiting 
	}
	else if (client->recv_state == Client::RECV_STATE_CRYPTSEED_WAIT)
	{	   // The abnormal case.  
			// The first four bytes after connection are the 
			// crypto seed

		client->recv_remaining_nocrypt( 4 );
		
		if (client->bytes_received == 4)
		{
			/* The first four bytes transmitted are the encryption seed */
			client->setcryptseed( client->buffer );

			client->clicrypt.setseed( client->buffer );
			client->cryptengine->Init( client->buffer );

			client->recv_state = Client::RECV_STATE_MSGTYPE_WAIT;
		}
		// Else keep waiting for IP address. 
	}
	return false;
}


void kill_disconnected_clients( void )
{
	Clients::iterator itr = clients.begin();
	while (itr != clients.end())
	{
		Client* client = *itr;
		if (client->disconnect)
		{
			OSTRINGSTREAM os;
			os << "Disconnecting Client " << client << "(";
			if (client->acct)
				os << client->acct->name();
			else
				os << "[no account]";
			os << "/";
			if (client->chr)
				os << client->chr->name();
			else
				os << "[no character]";
			os << ")" << endl;
			
			cerr << OSTRINGSTREAM_STR(os) << endl;
			if (config.loglevel >= 4)
				Log( "%s", OSTRINGSTREAM_STR(os).c_str() );

			Client::Delete( client );
			client = NULL;
			itr = clients.erase( itr );
		}
		else
		{
			++itr;
		}
	}
}

polclock_t checkin_clock_times_out_at;

void polclock_checkin()
{
	checkin_clock_times_out_at = polclock() + 30 * POLCLOCKS_PER_SEC;
//	cerr << "polclock checkin timeout at " << checkin_clock_times_out_at << endl;
}

void client_io_thread( Client* client )
{
	fd_set recv_fd;
	fd_set err_fd;
	fd_set send_fd;
	struct timeval select_timeout = { 0, 0 };
	int checkpoint = 0;
	int nidle = 0;
	polclock_t last_activity;
	wallclock_t last_packet_at  = wallclock();
	if (config.loglevel >= 11)
	{
		Log( "Client#%lu i/o thread starting\n", client->instance_ );
	}
client->checkpoint = 60; //CNXBUG
	{ 
		PolLock lck;  
client->checkpoint = 61; //CNXBUG
		last_activity = polclock(); 
	}

	if (config.loglevel >= 11)
	{
		Log( "Client#%lu i/o thread past initial lock\n", client->instance_ );
	}

	CLIENT_CHECKPOINT(0);
	// printf( "[Client i/o thread]" );
	try
	{
		while (!exit_signalled && !client->disconnect)
		{
			CLIENT_CHECKPOINT(1);
			int nfds = 0;
			FD_ZERO( &recv_fd );
			FD_ZERO( &err_fd );
			FD_ZERO( &send_fd );
			checkpoint = 1;
			FD_SET( client->csocket, &recv_fd );
			FD_SET( client->csocket, &err_fd );
			if (client->have_queued_data())
				FD_SET( client->csocket, &send_fd );
			checkpoint = 2;
			if ((int)(client->csocket+1) > nfds) 
				nfds = client->csocket+1;

			int res;
			do
			{
				select_timeout.tv_sec = 2;
				select_timeout.tv_usec = 0;
				CLIENT_CHECKPOINT(2);
				res = select( nfds, &recv_fd, &send_fd, &err_fd, &select_timeout );
				CLIENT_CHECKPOINT(3);
			} while (res < 0 && !exit_signalled && socket_errno == SOCKET_ERRNO(EINTR));

			checkpoint = 3;
	
			if (res < 0)
			{
				int sckerr = socket_errno;
				Log( "Client#%lu: select res=%d, sckerr=%d\n", client->instance_, res, sckerr );
				break;
			}
			else if (res == 0)
			{
				if ((!client->chr || !client->chr->cmdlevel) &&
					config.inactivity_warning_timeout && config.inactivity_disconnect_timeout)
				{
					++nidle;
					if (nidle == 30*config.inactivity_warning_timeout)
					{
						CLIENT_CHECKPOINT(4);
						PolLock lck;
						MSG53_INACTIVITY_WARNING msg;
						msg.msgtype = MSG53_INACTIVITY_WARNING_ID;
						msg.type_07 = 0x07;
						CLIENT_CHECKPOINT(5);
						client->transmit( &msg, sizeof msg );
						CLIENT_CHECKPOINT(18);
						if (client->pause_count)
							client->restart2();
					}
					else if (nidle == 30*config.inactivity_disconnect_timeout)
					{
						client->disconnect = true;
					}
				}
			}
			
	CLIENT_CHECKPOINT(19);
			if (client->disconnect)
				break;

			if (FD_ISSET( client->csocket, &err_fd ))
			{
				client->disconnect = 1;
				break;
			}
			
			if (FD_ISSET( client->csocket, &recv_fd ))
			{
				checkpoint = 4;
				CLIENT_CHECKPOINT(6);
				if (process_data( client ))
				{
					CLIENT_CHECKPOINT(17);
					PolLock lck;
					//reset packet timer
					last_packet_at = wallclock();					
					if (!check_inactivity(client)) 
					{
						nidle = 0;
						last_activity = polclock();
					}

					checkpoint = 5;
					CLIENT_CHECKPOINT(7);
					//printf( "Client i/o: Pulse!\n" );
					send_pulse();
					if (TaskScheduler::is_dirty())
						wake_tasks_thread();
				}

			}
			checkpoint = 6;

			wallclock_t wallclock_now = wallclock();
			if (wallclock_diff_ms( last_packet_at, wallclock_now ) >= 120000) //2 mins
			{
			   client->disconnect = true;
			   break;
			}

			if (client->have_queued_data() && FD_ISSET( client->csocket, &send_fd ))
			{
				PolLock lck;
				CLIENT_CHECKPOINT(8);
				client->send_queued_data();
			}
			checkpoint = 7;
			CLIENT_CHECKPOINT(21);
		}
	}
	catch( string& str )
	{
		Log( "Client#%lu: Exception in i/o thread: %s! (checkpoint=%d)\n", 
			  client->instance_, str.c_str(), checkpoint );
	}
	catch( const char* msg )
	{
		Log( "Client#%lu: Exception in i/o thread: %s! (checkpoint=%d)\n", 
			  client->instance_, msg, checkpoint );
	}
	catch( exception& ex )
	{
		Log( "Client#%lu: Exception in i/o thread: %s! (checkpoint=%d)\n", 
			  client->instance_, ex.what(), checkpoint );
	}
CLIENT_CHECKPOINT(20);
	// kill_disconnected_client( client );
	// printf( "Client thread cleaning up\n" );
	Log( "Client#%lu (%s): disconnected (account %s)\n", 
		 client->instance_,
		 AddressToString( &client->ipaddr ),
		 (client->acct != NULL) ? client->acct->name() : "unknown" );


	try 
	{
		if (1)
		{ 
			CLIENT_CHECKPOINT(9);
			PolLock lck;
			clients.erase( find_in( clients, client ) );
			client->Disconnect();
			cout << "Client disconnected from " << AddressToString( &client->ipaddr )
				 << " (" << clients.size() << " connections)"
				 << endl;
			CoreSetSysTrayToolTip( tostring(clients.size()) + " clients connected", ToolTipPrioritySystem );
		}

		long seconds_wait = 0;

		checkpoint = 8;
		CLIENT_CHECKPOINT(10);
		if(client->chr)
		{
			if (1)
			{
				CLIENT_CHECKPOINT(11);
				PolLock lck;

				client->chr->disconnect_cleanup();
				client->gd->clear();
				client->chr->connected = false;
				ScriptDef sd;
				sd.quickconfig( "scripts/misc/logofftest.ecl" );
				if (sd.exists())
				{
					CLIENT_CHECKPOINT(12);
					BObject bobj( run_script_to_completion( sd, new ECharacterRefObjImp( client->chr ) ) );
					if (bobj.isa( BObjectImp::OTLong ))
					{
						const BLong* blong = static_cast<const BLong*>(bobj.impptr());
						seconds_wait = blong->value();
					}
				}
			}
	
			polclock_t when_logoff = last_activity + seconds_wait * POLCLOCKS_PER_SEC;

			checkpoint = 9;
CLIENT_CHECKPOINT(13);
			while (!exit_signalled)
			{
				CLIENT_CHECKPOINT(14);
				{
					PolLock lck;
					if (polclock() >= when_logoff)
						break;
				}
				pol_sleep_ms( 2000 );
			}

			checkpoint = 10;
CLIENT_CHECKPOINT(15);
			if (1)
			{
				PolLock lck;
				if (client->chr)
				{
					Character* chr = client->chr;
					CLIENT_CHECKPOINT(16);
					call_chr_scripts( chr, "scripts/misc/logoff.ecl", "logoff.ecl" );
					ForEachMobileInRange( chr->x, chr->y, chr->realm, 32, NpcPropagateLeftArea, chr);
				}
			}
		} //if(client->chr) block
	}
	catch( std::exception& ex )
	{
		Log( "Client#%lu: Exception in i/o thread! (checkpoint=%d, what=%s)\n", client->instance_, checkpoint, ex.what() );
	}

	if (1)
	{
		PolLock lck;
CLIENT_CHECKPOINT(17);
		Client::Delete( client );
		client = NULL;
	}
}

void client_io_thread_stub2( void *arg )
{
	Client* client = static_cast<Client*>(arg);
client->checkpoint = 59; //CNXBUG
	client_io_thread( client );
}

#ifdef _WIN32
unsigned __stdcall client_io_thread_stub( void *arg )
{
	InstallStructuredExceptionHandler();
	Client* client = reinterpret_cast<Client*>(arg);
//client->checkpoint = 55; //CNXBUG
	threadmap.Register( threadhelp::thread_pid(),string("Client#")+decint(client->instance_));
//client->checkpoint = 56; //CNXBUG		
	SetThreadPriority( GetCurrentThread(), THREAD_PRIORITY_HIGHEST );
//client->checkpoint = 57; //CNXBUG
	threadhelp::run_thread( client_io_thread_stub2, arg );
//client->checkpoint = 58; //CNXBUG	
	_endthreadex(0);
	return 0;
}

#else
#include <pthread.h>
void * client_io_thread_stub( void *arg )
{
	InstallStructuredExceptionHandler();

	Client* client = reinterpret_cast<Client*>(arg);
	threadmap.Register( threadhelp::thread_pid(),string("Client#")+decint(client->instance_));
	
	run_thread( client_io_thread_stub2, arg );
	
	pthread_exit(NULL);
}
#endif

void listen_thread( void )
{
	fd_set listen_fd;
	struct timeval listen_timeout = { 0, 0 };

#ifdef _WIN32
	SetThreadPriority( GetCurrentThread(), THREAD_PRIORITY_HIGHEST );
#else
	pthread_attr_t attr;
	pthread_attr_init( &attr );
	pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_DETACHED );
#endif

	while (!exit_signalled)
	{
		int nfds = 0;
		FD_ZERO( &listen_fd );

		FD_SET( listen_socket, &listen_fd );
#ifndef _WIN32
		nfds = listen_socket + 1;
#endif

		int res;
		// printf( "Listen!\n" );
		do
		{
			listen_timeout.tv_sec = 5;
			listen_timeout.tv_usec = 0;
			res = select( nfds, &listen_fd, NULL, NULL, &listen_timeout );
		} while (res < 0 && !exit_signalled && socket_errno == SOCKET_ERRNO(EINTR));
		
		if (res <= 0)
			continue;
		
		if ( FD_ISSET( listen_socket, &listen_fd ))
		{
			// cout << "Accepting connection.." << endl;
			struct sockaddr client_addr, host_addr; // inet_addr
			socklen_t addrlen = sizeof client_addr;
			socklen_t host_addrlen = sizeof host_addr;
			SOCKET client_socket = accept( listen_socket, &client_addr, &addrlen );
			if (client_socket < 0)
				return;
			apply_socket_options( client_socket );

			PolLock lck;
			Client *client = new Client( uo_client_interface, config.client_encryption_version );
client->checkpoint = 50; //CNXBUG
			client->csocket = client_socket;
			memcpy( &client->ipaddr, &client_addr, sizeof client->ipaddr );
			// Added null setting for pre-char selection checks using NULL validation
			client->acct = NULL;
client->checkpoint = 51; //CNXBUG
			clients.push_back( client );
client->checkpoint = 52; //CNXBUG
			CoreSetSysTrayToolTip( tostring(clients.size()) + " clients connected", ToolTipPrioritySystem );
			cout << "Client connected from " << AddressToString( &client_addr )
				 << " (" << clients.size() << " connections)"
				 << endl;
			string ifdesc = "";
client->checkpoint = 53; //CNXBUG
			if (getsockname( client_socket, &host_addr, &host_addrlen ) == 0)
			{
				// cout << "host address: " << AddressToString( &host_addr ) << endl;
				ifdesc = " on interface ";
				ifdesc += AddressToString( &host_addr );
			}
			Log( "Client#%lu connected from %s (%d connections)%s\n", 
					client->instance_,
					AddressToString( &client_addr ),
					clients.size(),
					ifdesc.c_str() );
client->checkpoint = 54; //CNXBUG
			threadhelp::inc_child_thread_count( false );
#ifdef _WIN32
			unsigned threadid;
			HANDLE h = (HANDLE) _beginthreadex( NULL, 0, client_io_thread_stub, client, 0, &threadid );
client->checkpoint = 55; //CNXBUG
			CloseHandle( h );
			client->thread_pid = (int)threadid;
#else
			pthread_t th;
			int res = pthread_create( &th, &attr, client_io_thread_stub, client ); 
			if (res)
			{
				Log( "Failed to create worker thread for Client#%lu\n (res = %d)", client->instance_, res );
				PolLock lck;
				clients.erase( find_in( clients, client ) );
				Client::Delete( client );	
			}
#endif
		}
	}
}

#define clock_t_to_ms(x) (x)

void tasks_thread( void )
{
	polclock_t sleeptime;
	bool activity;
	try
	{
		while (!exit_signalled)
		{
			THREAD_CHECKPOINT( tasks, 1 );
			{
				PolLock lck;
				polclock_checkin();
				THREAD_CHECKPOINT( tasks, 2 );
				INC_PROFILEVAR( scheduler_passes );
				check_scheduled_tasks( &sleeptime, &activity );
				THREAD_CHECKPOINT( tasks, 3 );
				restart_all_clients();
				THREAD_CHECKPOINT( tasks, 5 );
			}

			THREAD_CHECKPOINT( tasks, 6 );
			if (activity)
				send_pulse();
			else
				INC_PROFILEVAR( noactivity_scheduler_passes );
			THREAD_CHECKPOINT( tasks, 7 );

			passert( sleeptime > 0 );

			TRACEBUF_ADDELEM( "tasks wait_for_pulse now", polclock() );
			TRACEBUF_ADDELEM( "tasks wait_for_pulse sleeptime", sleeptime );

			THREAD_CHECKPOINT( tasks, 8 );
			tasks_thread_sleep( polticks_t_to_ms( sleeptime ) );
			THREAD_CHECKPOINT( tasks, 9 );
		}
	}
	catch( const char* msg )
	{
		Log( "Tasks Thread exits due to exception: %s\n", msg );
		throw;
	}
	catch( string& str )
	{
		Log( "Tasks Thread exits due to exception: %s\n", str.c_str() );
		throw;
	}
	catch( exception& ex )
	{
		Log( "Tasks Thread exits due to exception: %s\n", ex.what() );
		throw;
	}
}

void scripts_thread( void )
{
	polclock_t sleeptime;
	bool activity;
	while (!exit_signalled)
	{
		THREAD_CHECKPOINT( scripts, 0 );
		{
			PolLock lck;
			polclock_checkin();
			TRACEBUF_ADDELEM( "scripts thread now", polclock() );
			++script_passes;
			THREAD_CHECKPOINT( scripts, 1 );

			step_scripts( &sleeptime, &activity );

			THREAD_CHECKPOINT( scripts, 50 );

			restart_all_clients();

			THREAD_CHECKPOINT( scripts, 52 );

			if (TaskScheduler::is_dirty())
			{
				THREAD_CHECKPOINT( scripts, 53 );

				wake_tasks_thread();
			}
		}
		
		if (activity)
		{
			++script_passes_activity;
		}
		else
		{
			++script_passes_noactivity;
		}
		
		if (sleeptime)
		{
			THREAD_CHECKPOINT( scripts, 54 );

			wait_for_pulse( polticks_t_to_ms( sleeptime ) );

			THREAD_CHECKPOINT( scripts, 55 );
		}
	}
}

void combined_thread( void )
{
	polclock_t sleeptime;
	bool activity;
	polclock_t script_clocksleft, scheduler_clocksleft;
	polclock_t sleep_clocks;
	polclock_t now;
	while (!exit_signalled)
	{
		++script_passes;
		do {
			PolLock lck;
			step_scripts( &sleeptime, &activity );
			check_scheduled_tasks( &sleeptime, &activity );
			restart_all_clients();
			now = polclock();
			script_clocksleft = calc_script_clocksleft( now );
			scheduler_clocksleft = calc_scheduler_clocksleft( now );
			if (script_clocksleft < scheduler_clocksleft)
				sleep_clocks = script_clocksleft;
			else
				sleep_clocks = scheduler_clocksleft;
		} while (sleep_clocks <= 0);

		wait_for_pulse( clock_t_to_ms( sleep_clocks ) );
	}
}

void decay_thread( void* );

template<class T>
inline void Delete( T* p )
{
	delete p;
}

template<class T>
class delete_ob
{
public:
	void operator()(T* p)
	{
		delete p;
	}
};

void reap_thread( void )
{
	while (!exit_signalled)
	{
		{
			PolLock lck;
			polclock_checkin();
			objecthash.Reap();
			
			for_each( dynamic_item_descriptors.begin(), dynamic_item_descriptors.end(), delete_ob<ItemDesc>() );
			dynamic_item_descriptors.clear();
		}

		threadhelp::thread_sleep_ms( 2000 );
	}
}
#ifndef _WIN32
#include <signal.h>
#include <unistd.h>
#endif

void threadstatus_thread( void )
{
	int timeouts_remaining = 1;
	bool sent_wakeups = false;
	// we want this thread to be the last out, so that it can report stuff at shutdown.
	while (!exit_signalled || threadhelp::child_threads>1)
	{
		if (!polclock_paused_at)
		{
			polclock_t now = polclock();
			if (now >= checkin_clock_times_out_at)
			{
				cerr << "No clock movement in 30 seconds.  Dumping thread status." << endl;
				report_status_signalled = true;
				checkin_clock_times_out_at = now + 30 * POLCLOCKS_PER_SEC;
			}
		}

		if(report_status_signalled)
		{
			ofstream pol_log;
			pol_log.open( "log/pol.log", ios::out|ios::app );
			db_cerr.setbufs( cerr.rdbuf(), pol_log.rdbuf() );
			db_cerr.install( &cerr );
			cerr << "*Thread Info*" << endl;
			cerr << "Semaphore PID: " << locker << endl;
#ifdef __unix__
			if (locker) 
			{
				cerr << "  (\"kill -SIGUSR2 " << locker << "\" to output backtrace)" << endl;
			}
			void* bt[ 200 ];
			int n = backtrace( bt, 200 );
			backtrace_symbols_fd( bt, n, STDERR_FILENO );
#endif
			cerr << "Scripts Thread Checkpoint: " << scripts_thread_checkpoint << endl;
			cerr << "Last Script: " << scripts_thread_script << " PC: " << scripts_thread_scriptPC << endl;
			cerr << "Escript Instruction Cycles: " << escript_instr_cycles << endl;
			cerr << "Tasks Thread Checkpoint: " << tasks_thread_checkpoint << endl;
			cerr << "Active Client Thread Checkpoint: " << active_client_thread_checkpoint << endl;
			if (check_attack_after_move_function_checkpoint)
				cerr << "check_attack_after_move() Checkpoint: " << check_attack_after_move_function_checkpoint << endl;
			cerr << "Current Threads:" << endl;
			ThreadMap::Contents contents;
			threadmap.CopyContents( contents );
			for(ThreadMap::Contents::const_iterator citr = contents.begin(); citr != contents.end(); ++citr)
			{
				cerr << (*citr).first << " - " << (*citr).second << endl;
			}
			report_status_signalled = false;
			db_cerr.uninstall();
		}
		if (exit_signalled)
		{
			if (!sent_wakeups)
			{
				send_pulse();
				wake_tasks_thread();
				sent_wakeups = true;
			}
		 
			--timeouts_remaining;
			//cout << "timeouts remaining: " << timeouts_remaining << endl;
			if (timeouts_remaining == 0)
			{
				cout << "Waiting for " << threadhelp::child_threads << " child threads to exit" << endl;
				timeouts_remaining = 5;
			}
		}
		pol_sleep_ms( 1000 );
	}
	//cerr << "threadstatus thread exits." << endl;
	signal_catch_thread();
}

void catch_signals_thread( void );
void check_console_commands();
void reload_configuration();

void console_thread( void )
{
	while (!exit_signalled)
	{
		pol_sleep_ms( 1000 );

		check_console_commands();
#ifndef _WIN32
		if (reload_configuration_signalled)
		{
			PolLock lck;
			cout << "Reloading configuration...";
			reload_configuration_signalled = false;
		reload_configuration();
		cout << "Done." << endl;
	}
#endif
	}
}

void start_threads()
{
	threadmap.Register( thread_pid(), "Main" );
	
	if (config.listen_port)
		threadhelp::start_thread( listen_thread, "Listen" );

	if (config.web_server)
		start_http_server();

	if (config.multithread == 1)
	{
		checkpoint( "start tasks thread" );
		threadhelp::start_thread( tasks_thread, "Tasks" );
		checkpoint( "start scripts thread" );
		threadhelp::start_thread( scripts_thread, "Scripts" );
	}
	else
	{
		checkpoint( "start combined scripts/tasks thread" );
		threadhelp::start_thread( combined_thread, "Combined" );
	}

	if (ssopt.decay_items)
	{
	  checkpoint( "start decay thread" );
	  vector<Realm*>::iterator itr;
	  for(itr = Realms->begin(); itr != Realms->end(); ++itr)
	  {
		ostringstream thname;
		thname << "Decay_" << (*itr)->name();
		threadhelp::start_thread( decay_thread, thname.str().c_str(), (void*)(*itr) );
	  }
	}
	else
	{
	  checkpoint( "don't start decay thread" );
	}

	checkpoint( "start reap thread" );
	threadhelp::start_thread( reap_thread, "Reap" );

	checkpoint( "start dbglisten thread" );
	threadhelp::start_thread( debug_listen_thread, "DbgListn" );

	checkpoint( "start threadstatus thread" );
	start_thread( threadstatus_thread, "ThreadStatus" );

#ifndef _WIN32
	//checkpoint( "start catch_signals thread" );
	//start_thread( catch_signals_thread, "CatchSignals" );
#endif
}

void check_incoming_data(void)
{
	unsigned cli;
	int nfds = 0;
	FD_ZERO( &recv_fd );
	FD_ZERO( &err_fd );
	FD_ZERO( &send_fd );

	FD_SET( listen_socket, &recv_fd );
#ifndef _WIN32
	nfds = listen_socket + 1;
#endif

	for( cli = 0; cli < clients.size(); cli++ )
	{
		Client *client = clients[cli];
		
		FD_SET( client->csocket, &recv_fd );
		FD_SET( client->csocket, &err_fd );
		if (client->have_queued_data())
			FD_SET( client->csocket, &send_fd );

		if ((int)(client->csocket+1) > nfds) 
			nfds = client->csocket+1;
	}

	int res;
	do
	{
		select_timeout.tv_sec = 0;
		select_timeout.tv_usec = config.select_timeout_usecs;
		res = select( nfds, &recv_fd, &send_fd, &err_fd, &select_timeout );
	} while (res < 0 && !exit_signalled && socket_errno == SOCKET_ERRNO(EINTR));

	
	if (res <= 0)
		return;
	
	for( cli = 0; cli < clients.size(); cli++ )
	{
		Client *client = clients[cli];

		if (client->disconnect)
			continue;

		if (FD_ISSET( client->csocket, &err_fd ))
		{
			client->disconnect = 1;
		}
		
		if (FD_ISSET( client->csocket, &recv_fd ))
		{
			process_data( client );
		}

		if (client->have_queued_data() && FD_ISSET( client->csocket, &send_fd ))
		{
			client->send_queued_data();
		}
	}

	kill_disconnected_clients();

	if ( FD_ISSET( listen_socket, &recv_fd ))
	{
		// cout << "Accepting connection.." << endl;
		struct sockaddr client_addr; // inet_addr
		socklen_t addrlen = sizeof client_addr;
		SOCKET client_socket = accept( listen_socket, &client_addr, &addrlen );
		if (client_socket < 0)
			return;

		apply_socket_options( client_socket );

		printf( "Client connected from %s\n",
				  AddressToString( &client_addr ) );
		if (config.loglevel >= 2)
			Log( "Client connected from %s\n", AddressToString( &client_addr ) );

		Client *client = new Client( uo_client_interface, config.client_encryption_version );
		client->csocket = client_socket;
		memcpy( &client->ipaddr, &client_addr, sizeof client->ipaddr );
		// Added null setting for pre-char selection checks using NULL validation
		client->acct = NULL;

		clients.push_back( client );
		printf( "Client connected (Total: %d)\n", clients.size() );
	}
}
#if REFPTR_DEBUG
unsigned int ref_counted::_ctor_calls;
#endif

void clear_script_storage()
{
	scrstore.clear();
}

void show_item( Item* item )
{
	cout << "Remaining item: " << item->serial << ": " << item->name() << endl;
} 
#include "bscript/escriptv.h"
void display_executor_instances();
void display_unreaped_orphan_instances();
void display_bobjectimp_instances();
void display_reftypes();

void display_leftover_objects()
{
	display_executor_instances();
	display_unreaped_orphan_instances();
#if BOBJECTIMP_DEBUG
	display_bobjectimp_instances();
#endif
	display_reftypes();
ofstream ofs( "leftovers.txt" );
objecthash.PrintContents(ofs);
	if (uobject_count != 0)
		cout << "Remaining UObjects: " << uobject_count << endl;
	if (ucharacter_count != 0)
		cout << "Remaining Mobiles: " << ucharacter_count << endl;
	if (npc_count != 0)
		cout << "Remaining NPCs: " << npc_count << endl;
	if (uitem_count != 0)
		cout << "Remaining Items: " << uitem_count << endl;
	if (umulti_count != 0)
		cout << "Remaining Multis: " << umulti_count << endl;
	if (unreaped_orphans != 0)
		cout << "Unreaped orphans: " << unreaped_orphans << endl;
	if (uobj_count_echrref != 0)
		cout << "Remaining EChrRef objects: " << uobj_count_echrref << endl;
	if (executor_count)
		cout << "Remaining Executors: " << executor_count << endl;
	if (eobject_imp_count)
		cout << "Remaining script objectimps: " << eobject_imp_count << endl;
	if (!existing_items.empty())
	{
		ForEach( existing_items, show_item );
	}
}

void run_package_startscript( Package* pkg )
{
	string scriptname = pkg->dir() + "start.ecl";
	
	if (FileExists( scriptname.c_str() ))
	{
		ScriptDef script( "start", pkg, "" );
		BObject obj( run_script_to_completion( script ) );
	}
}

void run_start_scripts()
{
	cout << "Running startup script." << endl;
	run_script_to_completion( "start" );
	ForEach( packages, run_package_startscript );
	cout << "Startup script complete." << endl;
}	   


#include "tasks.h"

#include "stubdata.h"
#include "uimport.h"
//#include <direct.h>

void load_intrinsic_weapons();
void cancel_all_trades();
void load_system_hooks();
void load_realms();
void load_aux_services();
void start_aux_services();
void clear_listen_points();
void InitializeSystemTrayHandling();
void ShutdownSystemTrayHandling();
void start_uo_client_listeners( void );
void unload_other_objects();
void unload_itemdesc_scripts();
void unload_system_hooks();
void start_tasks();
void UnloadAllConfigFiles();
void allocate_intrinsic_weapon_serials();

#ifdef _WIN32
#include "clib/mdump.h"
#endif

#ifdef _WIN32
typedef BOOL (WINAPI *DynHeapSetInformation) (
	PVOID HeapHandle,
	HEAP_INFORMATION_CLASS HeapInformationClass,
	PVOID HeapInformation,
	SIZE_T HeapInformationLength
);

const char* Use_low_fragmentation_Heap()
{
	if (ssopt.use_win_lfh)
	{
		HINSTANCE hKernel32;
		DynHeapSetInformation ProcAdd;
		
		hKernel32 = LoadLibrary("Kernel32");
		if (hKernel32 != NULL)
		{
			ProcAdd = (DynHeapSetInformation) GetProcAddress(hKernel32, "HeapSetInformation");
			if (ProcAdd != NULL)
			{
				ULONG HeapFragValue = 2;
				
				if ((ProcAdd) (GetProcessHeap(), HeapCompatibilityInformation, &HeapFragValue, sizeof(HeapFragValue)))
				{
					FreeLibrary(hKernel32);
					return "low-fragmentation Heap ...activated";
				}
				else
				{
					FreeLibrary(hKernel32);
					return "low-fragmentation Heap ...not activated";
				}
			}
			else
			{
				FreeLibrary(hKernel32);
				return "low-fragmentation Heap ...not available on your Windows";
			}
		}
		else
			return "low-fragmentation Heap ...not available on your Windows";
	}
	else
		return "low-fragmentation Heap ...disabled via ServSpecOpt";
}
#endif

#ifdef __linux__
void Check_libc_version()
{
	const char* libc_version = gnu_get_libc_version();

	long main_version;
	long sub_version;
	long build;
	char delimiter;
	ISTRINGSTREAM is( libc_version );

	if (is >> main_version >> delimiter >> sub_version >> delimiter >> build)
	{
		if (main_version*100000000 + sub_version*10000 + build >= 2*100000000 + 3*10000 + 2)
			cout << "Found libc " << libc_version << "... ok" << endl;
		else
			cout << "Found libc " << libc_version << "... Please update to 2.3.2 or above." << endl;
	}
	else
		cout << "Error in analyzing libc version string [" << libc_version << "]. Please contact Core-Team." << endl;
}
#endif

int xmain_inner( int argc, char *argv[] )
{
#ifdef _WIN32
	MiniDumper::Initialize();
	// Aug. 15, 2006 Austin
	// Added atexit() call to remove the tray icon.
	atexit(ShutdownSystemTrayHandling);
#endif

	int res;

	// for profiling:
	   // chdir( "d:\\pol" );
	// PrintAllocationData(); 

	MakeDirectory( "log" );

	mlog.open( "log/debug.log", ios::out|ios::app );
#ifdef MEMORYLEAK
	llog.open( "log/leak.log", ios::out|ios::app );
#endif

	start_log.open( "log/start.log", ios::out|ios::trunc );

	db_cout.setbufs( cout.rdbuf(), start_log.rdbuf() );
	db_cout.install( &cout );

	db_cerr.setbufs( cerr.rdbuf(), start_log.rdbuf() );
	db_cerr.install( &cerr );

	cout << progverstr << " (" << polbuildtag << ")" << endl;
	cout << "compiled on " << compiledate << " " << compiletime << endl;
	cout << "Copyright (C) 1993-2007 Eric N. Swanson" << endl;
	cout << endl;

	checkpoint( "opening logfiles" );
	open_logfiles();

	OpenLogFileName( "log/pol", true );
	Log( "%s (%s) compiled on %s %s running.\n", progverstr, polbuildtag, compiledate, compiletime );

	//cerr << "xmain2 cerr interceptor installed." << endl;
/*
	ofstream pol_lg2( "pol.lg2", ios::out|ios::app );
	pol_lg2 << "Log file opened at <FIXME:time here>" << endl;

	mlog.add( cout );
	mlog.add( pol_lg2 );
*/

#ifndef NDEBUG
	printf( "Sizes: \n" );
	printf( "   UObject:	%d\n", sizeof(UObject) );
	printf( "   Item:	   %d\n", sizeof(Item) );
	printf( "   UContainer: %d\n", sizeof(UContainer) );
	printf( "   Character:  %d\n", sizeof(Character) );
	printf( "   Client:	 %d\n", sizeof(Client) );
#ifdef __unix__
	printf( "   Max Threads:%d\n", (int)PTHREAD_THREADS_MAX );
#endif
#endif


	checkpoint( "installing signal handlers" );
	install_signal_handlers();

	checkpoint( "starting POL clocks" );
	start_pol_clocks();
	pause_pol_clocks();

	cout << "Reading Configuration." << endl;

	gflag_in_system_startup = true;

	checkpoint( "reading pol.cfg" );
	read_pol_config( true );
	
	checkpoint( "reading servspecopt.cfg" );
	read_servspecopt();

#ifdef _WIN32
	checkpoint( Use_low_fragmentation_Heap() );
#endif

#ifdef __linux__
	checkpoint( "checking libc version" );
	Check_libc_version();
#endif

	checkpoint( "init default itemdesc defaults" );
	empty_itemdesc.doubleclick_range = ssopt.default_doubleclick_range;
	empty_itemdesc.decay_time = ssopt.default_decay_time;

	checkpoint( "initializing random number generator" );
	set_seed( 1432776498L );

	//cout << "Opening UO client data files." << endl;
	//open_uo_data_files();
	
	//cout << "Reading UO data: ";
	//read_uo_data();
	//cout << "Done!" << endl;

	checkpoint( "loading POL map file" );
	load_realms();
	
//	if (FileExists( config.keyfile.c_str() ))
//	{
//		checkpoint( "reading crypto constants" );
//		read_crypto_constants( config.keyfile.c_str() );
//	}
//	else
//	{
//		cout << "Key file " << config.keyfile << " does not exist.  Aborting." << endl;
//	  throw runtime_error( "File not found" );
//  }

	// PrintAllocationData();

	checkpoint( "initializing IPC structures" );
	init_ipc_vars();
	threadhelp::init_threadhelp();

#ifdef _WIN32
	InitializeSystemTrayHandling();
#endif

	checkpoint( "initializing sockets library" );
	res = init_sockets_library();
	if (res < 0)
	{
		cout << "Unable to initialize sockets library." << endl;
		return 1;
	}
/*
	if (argc == 1)
	{
		if (config.listen_port)
		{
			checkpoint( "opening listen socket" );
			listen_socket = open_listen_socket( config.listen_port );
			if (listen_socket < 0)
			{
				cout << "Unable to listen on socket " << config.listen_port << endl;
				return 1;
			}
		}
	}
*/
	checkpoint( "loading configuration" );
	load_data();

	checkpoint( "loading system hooks" );
	load_system_hooks();
	
	checkpoint( "loading packet hooks" );
	load_packet_hooks();

	checkpoint( "loading auxservice configuration" );
	load_aux_services();

	checkpoint( "reading menus" );
	read_menus();

	checkpoint( "loading intrinsic weapons" );
	load_intrinsic_weapons();
	checkpoint( "reading gameservers" );
	read_gameservers();
	checkpoint( "reading starting locations" );
	read_starting_locations();

	if (argc > 1)
	{
		cout << "Running POL test suite." << endl;
		run_pol_tests();
		return 0;
	}
	
	// PrintAllocationData();
	cout << "Reading data files:\n";
	wallclock_t rd_start = wallclock();
		checkpoint( "reading account data" );
		read_account_data();

		checkpoint( "reading data" );
		read_data();
	wallclock_t rd_end = wallclock();
	long ms = wallclock_diff_ms( rd_start, rd_end );
	cout << "Done! " << ms << " milliseconds."<< endl;

	allocate_intrinsic_weapon_serials();
	gflag_in_system_startup = false;

	// PrintAllocationData();

	//onetime_create_stubdata();

	checkpoint( "running start scripts" );
	run_start_scripts();

	checkpoint( "starting client listeners" );
	start_uo_client_listeners( );

	if ( config.listen_port )
	{
		checkpoint( "opening listen socket" );
		listen_socket = open_listen_socket( config.listen_port );
		if (listen_socket < 0)
		{
			cout << "Unable to listen on socket " << config.listen_port << endl;
			return 1;
		}
	}

	if( 1 )
	{
		PolLock lock;
		cout << "Initialization complete.  POL is active.  Ctrl-C to stop." << endl << endl;
	}
	db_cout.uninstall();
	db_cerr.uninstall();
	start_log.close();
	if( 1 )
	{
		PolLock lock;
		Log( "Game is active.\n" );
	}
	CoreSetSysTrayToolTip( "Running", ToolTipPrioritySystem );

//goto skip;

	restart_pol_clocks();
	polclock_checkin();

	// this is done right after reading globals from pol.txt:
	//checkpoint( "starting game clock" );
	//start_gameclock();

	checkpoint( "starting periodic tasks" );
	start_tasks();
		
	if ( config.multithread )
	{
		checkpoint( "starting threads" );
		start_threads();
		start_aux_services();

#ifdef _WIN32
		console_thread();
		checkpoint( "exit signal detected" );
		CoreSetSysTrayToolTip( "Shutting down", ToolTipPriorityShutdown );
#else
		// On Linux, signals are directed to a particular thread, if we use pthread_sigmask like we're supposed to.
		// therefore, we have to do this signal checking in this thread.
		threadhelp::start_thread( console_thread, "Console" );

		catch_signals_thread();
#endif
		checkpoint( "waiting for child threads to exit" );
		// NOTE that it's possible that the thread_status thread not have exited yet..
		// it signals the catch_signals_thread (this one) just before it exits. 
		// and on windows, we get here right after the console thread exits. 
		while (threadhelp::child_threads)
		{
			pol_sleep_ms( 1000 );
		}
		checkpoint( "child threads have shut down" );
	}
	else
	{
		polclock_t sleeptime;
		bool activity;
		while ( !exit_signalled )
		{
										last_checkpoint = "receiving TCP/IP data";
			check_incoming_data();
										last_checkpoint = "running scheduled tasks";
			check_scheduled_tasks( &sleeptime, &activity);
										last_checkpoint = "stepping scripts";
			step_scripts(&sleeptime, &activity);
										last_checkpoint = "performing decay";
			if (ssopt.decay_items)
				decay_items();
										last_checkpoint = "reaping objects";
				objecthash.Reap();
										last_checkpoint = "restarting clients";

				restart_all_clients();

				++rotations;
		}
	}
	cancel_all_trades();
	stop_gameclock();
	Log( "Shutting down...\n" );

	checkpoint( "writing data" );
	if (should_write_data())
	{
		CoreSetSysTrayToolTip( "Writing data files", ToolTipPriorityShutdown );
		cout << "Writing data files...";

		PolLock lck;
		unsigned long dirty, clean, elapsed_ms;
		int savetype;

		if (passert_shutdown_due_to_assertion)
			savetype = config.assertion_shutdown_save_type;
		else
			savetype = config.shutdown_save_type;
		
		// TODO: full save if incremental_saves_disabled ?
		// otherwise could have really, really bad timewarps
		if (savetype == SAVE_FULL)
			write_data( dirty, clean, elapsed_ms );
		else
			save_incremental( dirty, clean, elapsed_ms );
			cout << "Done!  " << elapsed_ms << " ms." << endl;
	}
	else
	{
		if ( passert_shutdown_due_to_assertion && passert_nosave )
			Log2( "Not writing data due to assertion failure.\n" );
		else if ( config.inhibit_saves )
			Log2( "Not writing data due to pol.cfg InhibitSaves=1 setting.\n" );
	}

	deinit_ipc_vars();
	// PrintAllocationData();
//skip:

	if ( config.log_script_cycles )
		log_all_script_cycle_counts( false );

	// LogTraceBuffer();

									checkpoint("cleaning up vars");
	cleanup_vars();
									checkpoint("cleaning up scripts");
	cleanup_scripts();

	// scripts remove their listening points when they exit..
	// so there should be no listening points to clean up.
									checkpoint("cleaning listen points");
	clear_listen_points();
	
	unload_other_objects();
	unload_itemdesc_scripts();
	unload_system_hooks();
	UnloadAllConfigFiles();

	UninitObject::ReleaseSharedInstance();
	
	objecthash.Clear();
	display_leftover_objects();
									checkpoint("unloading data");
	unload_data();

	MD5_Cleanup();

									checkpoint("misc cleanup");
	CloseLogFile();

	clear_script_storage();

#ifdef _WIN32
	closesocket( listen_socket );
#else
	close( listen_socket ); // shutdown( listen_socket, 2 ); ??
#endif
	res = deinit_sockets_library();

									 checkpoint("end of xmain2");
	//mlog.clear();
	//pol_lg2 << "Log file closed at <FIXME:time here>" << endl;
	
	close_logfiles();
	
	return 0;
}

int xmain_outer( int argc, char *argv[] )
{
	try {
		return xmain_inner(argc,argv);
	}
	catch(std::exception&)
	{
		if (last_checkpoint != NULL)
		{
			cout << "Server Shutdown: " << last_checkpoint << endl;
			//pol_sleep_ms( 10000 );
		}
		objecthash.Clear();

		throw;
	}
}

#ifndef _WIN32
int xmain( int argc, char *argv[] )
{
	strcpy( progverstr, polverstr );
	strcpy( buildtagstr, polbuildtag );
	progver = polver;

	return xmain_outer( argc, argv );
}
#endif
