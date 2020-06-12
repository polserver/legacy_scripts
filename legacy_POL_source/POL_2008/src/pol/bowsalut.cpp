// History
//   2005/11/26 Shinigami: changed "strcmp" into "stricmp" to suppress Script Errors

#include "clib/stl_inc.h"
//#include <iostream>

#include "clib/cfgelem.h"
#include "clib/cfgfile.h"
#include "clib/endian.h"
#include "clib/fileutil.h"
#include "clib/strutil.h"

#include "charactr.h"
#include "client.h"
#include "extcmd.h"
#include "uobject.h"
#include "uvars.h"
#include "umsgtype.h"
#include "ufunc.h"
#include "sockio.h"

#include "action.h"

UACTION mount_action_xlate[ ACTION__HIGHEST+1 ];

UACTION str_to_action( ConfigElem& elem, const string& str )
{
    unsigned short tmp = static_cast<unsigned short>(strtoul( str.c_str(), NULL, 0 ));

    if (UACTION_IS_VALID( tmp ))
    {
        return static_cast<UACTION>(tmp);
    }
    else
    {
        elem.throw_error( "Animation value of " + str + " is out of range" );
        return ACTION__LOWEST;
    }
}

void load_anim_xlate_cfg( bool reload )
{
    memset( mount_action_xlate, 0, sizeof mount_action_xlate );

    if (FileExists( "config/animxlate.cfg" ))
    {
        ConfigFile cf( "config/animxlate.cfg", "OnMount" );
        ConfigElem elem;

        if (cf.read( elem ))
        {
            string from_str, to_str;
            while (elem.remove_first_prop( &from_str, &to_str ))
            {
                UACTION from = str_to_action( elem, from_str );
                UACTION to = str_to_action( elem, to_str );
                mount_action_xlate[ from ] = to;
            }
        }
    }
}

void send_action_to_inrange( const Character* obj, UACTION action )
{
    // std::cout << "Action: " << hexint( (unsigned short)action ) << std::endl;
    if (obj->on_mount())
    {
        if (action < ACTION_RIDINGHORSE1 || action > ACTION_RIDINGHORSE7)
        {
            UACTION new_action = mount_action_xlate[ action ];
            if (new_action == 0)
                return;
            //std::cout << "Action " << hexint( (unsigned short) action ) 
            //          << " translates to action " << hexint((unsigned short)new_action) << std::endl;
            action = new_action;
        }
    }

	MSG6E_DO_ACTION msg;
	msg.msgtype = MSG6E_DO_ACTION_ID;
	msg.serial = obj->serial_ext;
	msg.action = ctBEu16( (u16)action );
	msg.unk7 = 0;
	msg.unk8_05 = 0x05;
	msg.unk9 = 0;
	msg.unk10_01 = 0x01;
	msg.unk11 = 0;
	msg.unk12 = 0;
	msg.unk13_01 = 0x01;

	transmit_to_inrange( obj, &msg, sizeof msg );
}

void handle_action( Client *client, MSG12_EXTENDED_CMD *cmd )
{
	// printf( "Action: %s\n", cmd->data );
	if (stricmp( (const char *) cmd->data, "bow" ) == 0)
		send_action_to_inrange( client->chr, ACTION_BOW );
	else if (stricmp( (const char *) cmd->data, "salute" ) == 0)
		send_action_to_inrange( client->chr, ACTION_SALUTE );
}

ExtendedMessageHandler action_handler( EXTMSGID_ACTION, handle_action );
