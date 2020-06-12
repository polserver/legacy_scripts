// History

#include "clib/stl_inc.h"

#include <assert.h>
#include <stddef.h>

#include "clib/clib.h"
#include "clib/endian.h"
#include "clib/logfile.h"
#include "clib/passert.h"

#include "charactr.h"
#include "client.h"
#include "sockio.h"
#include "clfunc.h"

void send_sysmessage_cl(Client *client, /*Character *chr_from, ObjArray* oText,*/
						unsigned int cliloc_num, const u16 *arguments,
						unsigned short font, unsigned short color )
{
    MSGC1_LOCALIZED_MESSAGE msg;
	unsigned textlen = 0, msglen;
	while( arguments[textlen] != L'\0' )
		++textlen;
	textlen += 1;
	if (textlen > (sizeof msg.arguments / sizeof(msg.arguments[0])))
		textlen = (sizeof msg.arguments / sizeof(msg.arguments[0]));
	msglen = offsetof( MSGC1_LOCALIZED_MESSAGE, arguments ) + textlen*sizeof(msg.arguments[0]);

	if (msglen <= sizeof msg)
	{
	    msg.msgtype = MSGC1_LOCALIZED_MESSAGE_ID;
		msg.msglen = ctBEu16( msglen );
		msg.serial = 0xFFFFFFFF;
		msg.body = 0xFFFF;
		msg.type = 6;  // Lower Corner System Msg
		msg.hue = ctBEu16( color );
		msg.font = ctBEu16( font );
		msg.msgnumber = ctBEu32( cliloc_num );
		memset( msg.name, '\0', sizeof msg.name );
	    strcpy( msg.name, "System" );
		for(unsigned i=0; i < textlen; i++)
			msg.arguments[i] = ctLEu16(arguments[i]);
		msg.arguments[textlen] = (u16)0L;
		transmit( client, &msg, msglen );
	}
    else     // FIXME need to handle this better
    {
        Log( "send_sysmessage: unicode text is too long\n");
    }

}

void say_above_cl(UObject *obj, unsigned int cliloc_num,
				  const u16 *arguments, unsigned short font,
				  unsigned short color )
{
    MSGC1_LOCALIZED_MESSAGE msg;
	unsigned textlen = 0, msglen;
	while( arguments[textlen] != L'\0' )
		++textlen;
	textlen += 1;
	if (textlen > (sizeof msg.arguments / sizeof(msg.arguments[0])))
		textlen = (sizeof msg.arguments / sizeof(msg.arguments[0]));
	msglen = offsetof( MSGC1_LOCALIZED_MESSAGE, arguments ) + textlen*sizeof(msg.arguments[0]);

	if (msglen <= sizeof msg)
	{
	    msg.msgtype = MSGC1_LOCALIZED_MESSAGE_ID;
		msg.msglen = ctBEu16( msglen );
		msg.serial = obj->serial_ext;
		msg.body = obj->graphic_ext;
		msg.type = 7;  // Overhead Message
		msg.hue = ctBEu16( color );
		msg.font = ctBEu16( font );
		msg.msgnumber = ctBEu32( cliloc_num );
		memset( msg.name, '\0', sizeof msg.name );
	    strcpy( msg.name, "System" );
		for(unsigned i=0; i < textlen; i++)
			msg.arguments[i] = ctLEu16(arguments[i]);
		msg.arguments[textlen] = (u16)0L;

		// todo: only send to those that I'm visible to.
	    transmit_to_inrange( obj, &msg, msglen );
	}
    else     // FIXME need to handle this better
    {
        Log( "send_sysmessage: unicode text is too long\n");
    }

}

void private_say_above_cl(Character *chr, const UObject* obj,
						unsigned int cliloc_num, const u16 *arguments,
						unsigned short font, unsigned short color )
{
	MSGC1_LOCALIZED_MESSAGE msg;
	unsigned textlen = 0, msglen;
	while( arguments[textlen] != L'\0' )
		++textlen;
	textlen += 1;
	if (textlen > (sizeof msg.arguments / sizeof(msg.arguments[0])))
		textlen = (sizeof msg.arguments / sizeof(msg.arguments[0]));
	msglen = offsetof( MSGC1_LOCALIZED_MESSAGE, arguments ) + textlen*sizeof(msg.arguments[0]);

	if (msglen <= sizeof msg)
	{
	    msg.msgtype = MSGC1_LOCALIZED_MESSAGE_ID;
		msg.msglen = ctBEu16( msglen );
		msg.serial = obj->serial_ext;
		msg.body = obj->graphic_ext;
		msg.type = 7;  // Overhead Message
		msg.hue = ctBEu16( color );
		msg.font = ctBEu16( font );
		msg.msgnumber = ctBEu32( cliloc_num );
		memset( msg.name, '\0', sizeof msg.name );
	    strcpy( msg.name, "System" );
		for(unsigned i=0; i < textlen; i++)
			msg.arguments[i] = ctLEu16(arguments[i]);
		msg.arguments[textlen] = (u16)0L;
		chr->client->transmit( &msg, msglen );
	}
    else     // FIXME need to handle this better
    {
        Log( "send_sysmessage: unicode text is too long\n");
    }
}
