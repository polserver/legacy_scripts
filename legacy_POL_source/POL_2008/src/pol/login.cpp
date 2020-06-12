// History
//   2005/01/24 Shinigami: added message handler for packet 0xd9 (Spy on Client 2)
//   2005/01/27 Shinigami: using little/big endian functions provided by endian.h
//   2005/04/03 Shinigami: send_feature_enable() call moved from start_client_char()
//                         to send_start() to send before char selection
//   2005/04/03 Shinigami: FLAG_UPTO_SIX_CHARACTERS disabled in uo_feature_enable
//   2005/04/04 Shinigami: added candelete script
//   2005/08/29 Shinigami: get-/setspyonclient2 renamed to get-/setclientinfo
//   2007/07/09 Shinigami: modified MSG8C_LOGIN3.new_key to not conflict with UO:KR detection
//                         added message handler for packet 0xe4 (UO:KR Verifier Response)
//   2008/02/09 Shinigami: removed hardcoded MAX_CHARS from send_start()

#include "clib/stl_inc.h"
#ifdef _MSC_VER
#pragma warning( disable: 4786 )
#endif

#include "clib/stlutil.h"
#include "clib/MD5.h"

#include <string.h>


#include "clib/clib.h"
#include "clib/endian.h"
#include "clib/logfile.h"
#include "clib/fdump.h"

#include "account.h"
#include "charactr.h"
#include "client.h"
#include "core.h"
#include "msghandl.h"
#include "nmsgtype.h"
#include "polcfg.h"
#include "startloc.h"
#include "umsgtype.h"
#include "uvars.h"
#include "servdesc.h"
#include "sockio.h"
#include "ssopt.h"
#include "ufunc.h"

bool is_banned_ip(Client* client);

void send_login_error( Client *client, unsigned char reason )
{
	MSG82_LOGIN_ERROR msg;
	msg.msgtype = MSGOUT_82_LOGIN_ERROR;
	msg.error = reason;
	client->transmit( &msg, sizeof msg );
}

bool acct_check(Client* client, int i)
{
	if (servers[i]->acct_match.empty())
		return true;

	for ( unsigned j = 0; j < servers[i]->acct_match.size(); ++j )
	{
		if (stricmp(servers[i]->acct_match[j].c_str(), client->acct->name()) == 0)
			return true;
	}

	return false;
}

bool server_applies( Client* client, int i )
{
    if (servers[i]->ip_match.empty())
        return acct_check(client, i);

    for( unsigned j = 0; j < servers[i]->ip_match.size(); ++j )
    {
        unsigned long addr1part, addr2part;
        struct sockaddr_in* sockin = reinterpret_cast<struct sockaddr_in*>(&client->ipaddr);

        addr1part = servers[i]->ip_match[j]      & servers[i]->ip_match_mask[j];
#ifdef _WIN32
        addr2part = sockin->sin_addr.S_un.S_addr & servers[i]->ip_match_mask[j];
#else
        addr2part = sockin->sin_addr.s_addr      & servers[i]->ip_match_mask[j];
#endif
        if (addr1part == addr2part)
            return true;
    }
    return false;
}

void loginserver_login( Client *client, MSG80_LOGINSERVER_LOGIN *msg )
{
	unsigned idx;

    if (is_banned_ip( client ))
    {
        send_login_error( client, LOGIN_ERROR_ACCOUNT_BLOCKED );
        client->disconnect = 1;
        return;
    }

	Account *acct = find_account( msg->name );
	if (!acct)
	{
		send_login_error( client, LOGIN_ERROR_NO_ACCOUNT );
		client->disconnect = 1;
		return;
	}

	bool correct_password = false;

	//dave changed 6/5/3, always authenticate with hashed user+pass
	string msgpass = msg->password;
	string acctname = acct->name();
	string temp;
	MD5_Encrypt(acctname + msgpass,temp); //MD5
	correct_password = MD5_Compare(acct->passwordhash(), temp);

	if (!correct_password)
	{
		send_login_error( client, LOGIN_ERROR_WRONG_PASSWORD );
		client->disconnect = 1;
        Log( "Incorrect password for account %s from %s\n",
             acct->name(),
             AddressToString( &client->ipaddr ) );
		return;
	}
	else
	{
		//write out cleartext and passwordonly hash if necessary
		if(config.retain_cleartext_passwords)
		{
			if(acct->password().empty())
				acct->set_password(msgpass);
		}
		if(acct->password_only_hash().empty())
		{
			string temp;
			MD5_Encrypt(msgpass,temp);
			acct->set_password_only_hash(temp); //MD5
		}
	}

    if (!acct->enabled() || acct->banned())
    {
        send_login_error( client, LOGIN_ERROR_ACCOUNT_BLOCKED );
        client->disconnect = 1;
        return;
    }

    Log( "Account %s logged in from %s\n",
         acct->name(),
         AddressToString( &client->ipaddr ));

	/* At this point, we don't care if the account is in use, really.
		So, we don't associate it.  More checking will be done when
		a character is selected.
	*/
	client->acct = acct;
	// NOTE: client->acct->client is intentionally not set to client here.

	// Transmit server list to client
    static char buffer[ 2000 ];
	MSGA8_SERVER_LIST_HEAD *phead = reinterpret_cast<MSGA8_SERVER_LIST_HEAD*>(buffer);
	MSGA8_SERVER_LIST_ELEM *pelem = reinterpret_cast<MSGA8_SERVER_LIST_ELEM*>(phead+1);
    unsigned short msglen = sizeof *phead;
    unsigned short servcount = 0;

	phead->msgtype = MSGOUT_A8_SERVER_LIST;
	phead->msglen = 0;
	phead->unk3_FF = 0xFF;
	phead->servcount = 0;

    for( idx = 0; idx < servers.size(); idx++ )
	{
        ServerDescription* server = servers[idx];

        if (!server->hostname.empty())
        {
            struct hostent* he = gethostbyname( server->hostname.c_str() ); // FIXME: here is a potential server lockup
            if (he && he->h_addr_list[0])
            {
                char* addr = he->h_addr_list[0];
                server->ip[0] = addr[3];
                server->ip[1] = addr[2];
                server->ip[2] = addr[1];
                server->ip[3] = addr[0];
/*
                struct sockaddr_in saddr;
                memcpy( &saddr.sin_addr, he->h_addr_list[0], he->h_length);
                server->ip[0] = saddr.sin_addr.S_un.S_un_b.s_b1;
                server->ip[1] = saddr.sin_addr.S_un.S_un_b.s_b2;
                server->ip[2] = saddr.sin_addr.S_un.S_un_b.s_b3;
                server->ip[3] = saddr.sin_addr.S_un.S_un_b.s_b4;
*/
            }
            else
            {
                Log( "gethostbyname(\"%s\") failed for server %s\n",
                      server->hostname.c_str(),
                      server->name.c_str() );
                continue;
            }
        }

        if (server_applies( client, idx ))
        {
            memset( pelem, 0, sizeof *pelem );
            ++servcount;
            pelem->servernum = ctBEu16( idx+1 );
		    strzcpy( pelem->servername, server->name.c_str(), sizeof pelem->servername );
		    pelem->servernum2 = ctBEu16( idx+1 );
            pelem->ip[0] = server->ip[3];
		    pelem->ip[1] = server->ip[2];
		    pelem->ip[2] = server->ip[1];
		    pelem->ip[3] = server->ip[0];
            ++pelem;
            msglen += sizeof *pelem;
        }
	}
	phead->msglen = ctBEu16( msglen );
	phead->servcount = ctBEu16( servcount );
	client->transmit( buffer, msglen );

    if (servcount == 0)
    {
        Log( "No applicable servers for client connecting from %s\n", AddressToString( &client->ipaddr ) );
    }
}
MESSAGE_HANDLER( MSG80_LOGINSERVER_LOGIN, loginserver_login );

//* SPY: seems to be sent before "server select" message
void handle_A4( Client *client, MSGA4_CLIENT_INFO *msg )
{
	//printf( "Message Type 0xA4 (unhandled)\n" );
	//if (logfile) fdump( logfile, msg, sizeof msg );
}
MESSAGE_HANDLER( MSGA4_CLIENT_INFO, handle_A4 );

// ClientInfo - delivers a lot of usefull infomation about client PC
void handle_D9( Client *client, MSGD9_CLIENT_INFO *msg )
{
  // Transform Little-Endian <-> Big-Endian
  msg->instance =       cfBEu32(msg->instance);       // Unique Instance ID of UO
  msg->os_major =       cfBEu32(msg->os_major);       // OS Major
  msg->os_minor =       cfBEu32(msg->os_minor);       // OS Minor
  msg->os_revision =    cfBEu32(msg->os_revision);    // OS Revision
  msg->cpu_family =     cfBEu32(msg->cpu_family);     // CPU Family
  msg->cpu_model =      cfBEu32(msg->cpu_model);      // CPU Model
  msg->cpu_clockspeed = cfBEu32(msg->cpu_clockspeed); // CPU Clock Speed [Mhz]
  msg->memory =         cfBEu32(msg->memory);         // Memory [MB]
  msg->screen_width =   cfBEu32(msg->screen_width);   // Screen Width
  msg->screen_height =  cfBEu32(msg->screen_height);  // Screen Height
  msg->screen_depth =   cfBEu32(msg->screen_depth);   // Screen Depth [Bit]
  msg->directx_major =  cfBEu16(msg->directx_major);  // DirectX Major
  msg->directx_minor =  cfBEu16(msg->directx_minor);  // DirectX Minor

  for ( unsigned i = 0; i < sizeof(msg->video_description) / sizeof(msg->video_description[0]); ++i )
    msg->video_description[i] = cfBEu16(msg->video_description[i]); // Video Card Description [wide-character]

  msg->video_vendor =   cfBEu32(msg->video_vendor);   // Video Card Vendor ID
  msg->video_device =   cfBEu32(msg->video_device);   // Video Card Device ID
  msg->video_memory =   cfBEu32(msg->video_memory);   // Video Card Memory [MB]

  for ( unsigned i = 0; i < sizeof(msg->langcode) / sizeof(msg->langcode[0]); ++i )
    msg->langcode[i] = cfBEu16(msg->langcode[i]); // Language Code [wide-character]

  client->setclientinfo(msg);
}
MESSAGE_HANDLER( MSGD9_CLIENT_INFO, handle_D9 );

void select_server(Client *client, MSGA0_SERVER_SELECT *msg ) // Relay player to a certain IP
{
	unsigned servernum = cfBEu16(msg->servernum) - 1;
    // printf( "Select server: %d\n", servernum );

	if (servernum >= servers.size())
	{
		client->disconnect = 1;
		return;
	}

	ServerDescription *svr = servers[ servernum ];

	MSG8C_LOGIN3 rsp;
	rsp.msgtype = MSGOUT_8C_LOGIN3;
	rsp.ip[0] = svr->ip[3];
	rsp.ip[1] = svr->ip[2];
	rsp.ip[2] = svr->ip[1];
	rsp.ip[3] = svr->ip[0];
	if (client->listen_port != 0)
        rsp.port = ctBEu16( client->listen_port );
    else
        rsp.port = ctBEu16( svr->port );
	rsp.unk7_00 = 0xFE; // This was set to 0xffffffff in the past but this will conflict with UO:KR detection
	rsp.unk8_03 = 0xFE;
	rsp.unk9_C3 = 0xFE;
	rsp.unk10_4B =0xFE;

	client->transmit( &rsp, sizeof rsp );

    unsigned long nseed = 0xFFffFFffLu; // CRYPT_AUTOMASK;
    client->clicrypt.setseed( client->cryptseed );
    client->cryptengine->Init( &nseed /*client->cryptseed*/ );
}

MESSAGE_HANDLER( MSGA0_SERVER_SELECT, select_server );

void send_start( Client *client )
{
  send_feature_enable( client ); // Shinigami: moved from start_client_char() to send before char selection

	unsigned i;
    unsigned msglen;
    MSGA9_START_HEAD head;
	MSGA9_CHAR_LIST_ELEM char_elem;
	MSGA9_START_COUNT startcount;
	MSGA9_START_ELEM startelem;
	MSGA9_START_FLAGS startflags;

	msglen = sizeof head +
             sizeof char_elem * config.character_slots +
             sizeof startcount +
             sizeof startelem * startlocations.size() +
			 sizeof startflags ;

    char* msg = new char[msglen];
    unsigned int next_offset = 0;

    head.msgtype = MSGOUT_A9_START;
    head.msglen = ctBEu16(msglen);
	head.numchars = client->acct->numchars();
	//client->transmit( &head, sizeof head );
    memcpy(msg+next_offset,&head,sizeof head);
    next_offset += sizeof head;

	for( i = 0; i < config.character_slots; i++ )
	{
        memset( &char_elem, 0, sizeof char_elem );
		Character* chr = client->acct->get_character( i );
        if (chr)
		{
			strzcpy( char_elem.name,
                     chr->name().c_str(),
                     sizeof char_elem.name );
		}
		//client->transmitmore( &char_elem, sizeof char_elem );
        memcpy(msg+next_offset,&char_elem,sizeof char_elem);
        next_offset += sizeof char_elem;
	}

	startcount.startcount = (u8) startlocations.size();
	//client->transmitmore( &startcount, sizeof startcount );
    memcpy(msg+next_offset,&startcount,sizeof startcount);
    next_offset += sizeof startcount;

	for( i = 0; i < startlocations.size(); i++ )
	{
        memset( &startelem, 0, sizeof startelem );
		startelem.startnum = i;
		strzcpy( startelem.city, startlocations[i]->city.c_str(), sizeof startelem.city );
		strzcpy( startelem.desc, startlocations[i]->desc.c_str(), sizeof startelem.desc );
		//client->transmitmore( &startelem, sizeof startelem );
        memcpy(msg+next_offset,&startelem,sizeof startelem);
        next_offset += sizeof startelem;
	}
  startflags.flags = ctBEu32(ssopt.uo_feature_enable & ~MSGA9_START_FLAGS::FLAG_UPTO_SIX_CHARACTERS);
    memcpy(msg+next_offset,&startflags,sizeof startflags);
	//client->transmitmore( &startflags, sizeof startflags );
    client->transmit( msg, msglen );
    delete[] msg;
}




void login2(Client *client, MSG91_GAMESERVER_LOGIN *msg) // Gameserver login and character listing
{
	client->encrypt_server_stream = 1;

    if (is_banned_ip( client ))
    {
        send_login_error( client, LOGIN_ERROR_ACCOUNT_BLOCKED );
        client->disconnect = 1;
        return;
    }

	/* Hmm, might have to re-search for account.
	   For now, we already have the account in client->acct.
	   Might work different if real loginservers were used. */
	Account *acct = find_account( msg->name );
	if (acct == NULL)
	{
		send_login_error( client, LOGIN_ERROR_NO_ACCOUNT );
		client->disconnect = 1;
		return;
	}

	// First check the password - if wrong, you can't find out anything else.
	bool correct_password = false;

	//dave changed 6/5/3, always authenticate with hashed user+pass
	string msgpass = msg->password;
	string acctname = acct->name();
	string temp;
	MD5_Encrypt(acctname + msgpass,temp); //MD5
	correct_password = MD5_Compare(acct->passwordhash(), temp);

	if (!correct_password)
	{
		send_login_error( client, LOGIN_ERROR_WRONG_PASSWORD );
		client->disconnect = 1;
        Log( "Incorrect password for account %s from %s\n",
             acct->name(),
             AddressToString( &client->ipaddr ) );
		return;
	}
	else
	{
		//write out cleartext and passwordonly hash if necessary
		if(config.retain_cleartext_passwords)
		{
			if(acct->password().empty())
				acct->set_password(msgpass);
		}
		if(acct->password_only_hash().empty())
		{
			string temp;
			MD5_Encrypt(msgpass,temp);
			acct->set_password_only_hash(temp); //MD5
		}
	}

    if (!acct->enabled() || acct->banned())
    {
        send_login_error( client, LOGIN_ERROR_ACCOUNT_BLOCKED );
        client->disconnect = 1;
        return;
    }

	//
	//Dave moved the max_clients check to pol.cpp so character cmdlevel could be checked.
	//

    Log( "Account %s logged in from %s\n",
         acct->name(),
         AddressToString( &client->ipaddr ));

	// ENHANCEMENT: could authenticate with real loginservers.

	client->acct = acct;
	/* NOTE: acct->client is not set here.  It is possible that another client
	   is still connected, or a connection is stuck open, or similar.  When
	   a character is selected, if another client is connected, measures will
	   be taken. */

	// Tell the client about the starting locations and his characters (up to 5).

	send_start( client );
}

MESSAGE_HANDLER( MSG91_GAMESERVER_LOGIN, login2 );

void delete_character( Account* acct, Character* chr, int charidx )
{
    if (!chr->logged_in)
    {
        Log( "Account %s deleting character 0x%lu\n", acct->name(), chr->serial );

        chr->acct.clear();
        acct->clear_character( charidx );

        chr->destroy();
    }
}

bool can_delete_character( Character* chr, int delete_by );
void call_ondelete_scripts( Character* chr );

void handle_delete_character( Client* client, MSG83_DELETE_CHARACTER* msg )
{
    u32 charidx = cfBEu32( msg->charidx );

	if ((charidx < 0) || (charidx >= config.character_slots) ||
		(client->acct == NULL) ||
        (client->acct->get_character( charidx ) == NULL))
	{
		send_login_error( client, LOGIN_ERROR_MISC );
		client->disconnect = 1;
		return;
	}

    Account* acct = client->acct;
    Character* chr = acct->get_character( charidx );
	if (chr->client != NULL ||
        acct->active_character != NULL) // this account has a currently active character.
	{
		send_login_error( client, LOGIN_ERROR_OTHER_CHAR_INUSE );
		client->disconnect = 1;
		return;
    }
    
    if (can_delete_character( chr, DELETE_BY_PLAYER ))
    {
        call_ondelete_scripts( chr );
        delete_character( acct, chr, charidx );
    }
    
    send_start( client );
}
MESSAGE_HANDLER( MSG83_DELETE_CHARACTER, handle_delete_character );

void KR_Verifier_Response( Client *client, MSGE4_KR_VERIFIER_RESPONSE *msg )
{
	//
}
MESSAGE_HANDLER_VARLEN( MSGE4_KR_VERIFIER_RESPONSE, KR_Verifier_Response );
