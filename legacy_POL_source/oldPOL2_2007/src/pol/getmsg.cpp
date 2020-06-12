#include "clib/stl_inc.h"
#include "bscript/berror.h"
#include "bscript/impstr.h"

#include "clib/endian.h"
#include "clib/rawtypes.h"

#include "client.h"
#include "sockio.h"
#include "msghandl.h"

#include "cgdata.h"
#include "charactr.h"
#include "item.h"
#include "osemod.h"
#include "ufunc.h"
#include "uoexec.h"
#include "uoemod.h"
#include "umsgtype.h"

void send_prompt( Client* client, u32 serial )
{
    static MSG9A_PROMPT msg;
    msg.msgtype = MSG9A_PROMPT_ID;
    u16 msglen = sizeof(MSG9A_PROMPT);
    msg.msglen = ctBEu16( msglen );
    msg.serial = serial;
    msg.unk = ctBEu32( 0x15 );
    msg.unk2 = 0;
    msg.text[0] = 0;
    transmit( client, &msg, msglen );
}

void handle_prompt( Client* client, MSG9A_PROMPT* msg )
{
    UOExecutorModule* uoemod = client->gd->prompt_uoemod;
    if (uoemod == NULL)
        return;
    int textlen = cfBEu16( msg->msglen ) - offsetof(MSG9A_PROMPT,text);
    if (msg->unk2)
    {
        if (textlen <= 120 && msg->text[textlen-1] == '\0')
        {
            bool ok = true;
            --textlen; // don't include null terminator (already checked)
            for( int i = 0; i < textlen; ++i )
            {
                if (!isprint(msg->text[i]))
                {
                    ok = false;
                    break;
                }
            }
            if (ok)
            {
                String* str = new String( msg->text, textlen );
                uoemod->uoexec.ValueStack.top().set( new BObject( str ) );
            }
        }
    }
    uoemod->uoexec.os_module->revive();
    uoemod->prompt_chr = NULL;
    client->gd->prompt_uoemod = NULL;
}
MESSAGE_HANDLER_VARLEN( MSG9A_PROMPT, handle_prompt );

BObjectImp* UOExecutorModule::mf_PromptInput()
{
    Character* chr;
    Item* item;
    const String* prompt;
    if (!getCharacterParam( exec, 0, chr ) ||
        !getItemParam( exec, 1, item ) ||
        !exec.getStringParam( 2, prompt ))
    {
        return new BError( "Invalid parameter" );
    }

    if (chr->client == NULL)
    {
        return new BError( "No client attached" );
    }

    if (chr->has_active_prompt() != false)
    {
        return new BError( "Another script has an active prompt" );
    }

    send_sysmessage( chr->client, prompt->data() );

    chr->client->gd->prompt_uoemod = this;
    prompt_chr = chr;

    send_prompt( chr->client, ctBEu32( item->serial ) );
    uoexec.os_module->suspend();
    return new BLong(0);
}
