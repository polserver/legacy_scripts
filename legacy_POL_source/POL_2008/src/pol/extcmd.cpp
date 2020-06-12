#include "clib/stl_inc.h"
#include <stddef.h>

#include "clib/fdump.h"
#include "clib/endian.h"

#include "msghandl.h"
#include "uvars.h"
#include "umsgtype.h"
#include "extcmd.h"

struct ExtMsgHandler {
	void (*func)(Client *client, MSG12_EXTENDED_CMD *msg);
};

static ExtMsgHandler handler_table[ 256 ];

ExtendedMessageHandler::ExtendedMessageHandler( UEXTMSGID submsgtype,
												void (*func)(Client *client, MSG12_EXTENDED_CMD *msg) )
{
	if (handler_table[ submsgtype ].func)
	{
		fprintf( stderr, "Extended Message Handler %d (0x%x) multiply defined.\n", 
					submsgtype, submsgtype );
		throw runtime_error( "Extended Message Handler multiply defined." );
	}
	handler_table[ submsgtype ].func = func;
}

void handle_extended_cmd( Client *client, MSG12_EXTENDED_CMD *msg )
{
	u16 msglen = cfBEu16( msg->msglen );

	if (msglen > sizeof *msg)			// sanity-check the message.
		return;
	if (msg->data[ msglen - offsetof(MSG12_EXTENDED_CMD,data) - 1] != '\0')				// the string must be null-terminated.
		return;

	// printf( "Extended Message: Type 0x%x\n", msg->submsgtype );
	// fdump( stdout, msg, cfBEu16( msg->msglen ) );
	if (handler_table[ msg->submsgtype ].func)
		handler_table[ msg->submsgtype ].func(client,msg);
}

MESSAGE_HANDLER_VARLEN( MSG12_EXTENDED_CMD, handle_extended_cmd );
