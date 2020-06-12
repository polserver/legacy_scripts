#include "clib/stl_inc.h"
#include "clib/endian.h"
#include "clib/fdump.h"
#include "clib/clib.h"

#include "attribute.h"
#include "charactr.h"
#include "client.h"
#include "msghandl.h"
#include "polcfg.h"
#include "scrdef.h"
#include "scrstore.h"
#include "statmsg.h"
#include "uoskills.h"
#include "uvars.h"
#include "umsgtype.h"
#include "ufunc.h"


void statrequest( Client *client, u32 serial)
{
    if (serial == client->chr->serial)
    {
        send_full_statmsg( client, client->chr );
    }
    else
    {
        Character *chr = find_character( serial );
        if (chr != NULL)
        {
            if (inrange( client->chr, chr ))
             send_short_statmsg( client, chr );
        }
    }
}

void send_skillmsg( Client *client, const Character *chr )
{
    unsigned short msglen;

    static MSG3A_SKILL_VALUES msg;
	//dave changed 3/15/03, support configurable max skillid
    msglen = 6 + (7*(uoclient_general.maxskills+1)); //0-based, only send skill structs up to configured maxskillid, reserve space for header, term, etc.

    msg.msgtype = MSGOUT_3A_SKILL_VALUES_ID;
    msg.msglen = ctBEu16( msglen );
    msg.unk3 = 0;
    unsigned int i;
    for( i = 0; i <= uoclient_general.maxskills; i++ ) //dave changed 3/15/03, support configurable max skillid
    {
        const UOSkill& uoskill = GetUOSkill(i);

        msg.skills[i].skillid = ctBEu16(i+1); // for some reason, we send this 1-based
        if (uoskill.pAttr)
        {
            long value;
            
            value = chr->attribute(uoskill.pAttr->attrid).effective_tenths();
            if (value > 0xFFFF) 
                value = 0xFFFF;
            msg.skills[i].value = ctBEu16( static_cast<u16>(value) );

            value = chr->attribute(uoskill.pAttr->attrid).base();
            if (value > 0xFFFF)
                value = 0xFFFF;
            msg.skills[i].value_unmod = ctBEu16( static_cast<u16>(value) );
            msg.skills[i].lock_mode = MSG3A_SKILL_VALUES::LOCK_NONE;
        }
        else
        {
            msg.skills[i].value = 0;
            msg.skills[i].value_unmod = 0;
            msg.skills[i].lock_mode = MSG3A_SKILL_VALUES::LOCK_DOWN;
        }
    }
    msg.terminator = 0;
	u16* term1 = (u16*)((u8*)(&msg)+(msglen-1)); //this points to the last 2 bytes in the msg no matter the length, we want to set those to 0.
	*term1 = 0;
    client->transmit( &msg, msglen );
}

void skillrequest( Client *client, u32 serial )
{
    if (serial == client->chr->serial)
    {
        ScriptDef sd;
        sd.quickconfig( "scripts/misc/skillwin.ecl" );
        if (sd.exists())
        {
            ref_ptr<EScriptProgram> prog;
            prog = find_script2( sd, 
                                 false, // complain if not found
                                 config.cache_interactive_scripts );
            if (prog.get() != NULL &&
                client->chr->start_script( prog.get(), false ))
            {
                return;
            }
        }
        send_skillmsg( client, client->chr );
    }
}

void srequest( Client *client, MSG34_STATUS_REQUEST *msg )
{
    //printf( "Status Request:\n" );
    //fdump( stdout, msg, sizeof *msg );
    u32 serial = cfBEu32( msg->serial2 );
    
    if (msg->stattype == STATTYPE_STATWINDOW)
        statrequest( client, serial );
    else if (msg->stattype == STATTYPE_SKILLWINDOW)
        skillrequest( client, serial );
}

MESSAGE_HANDLER( MSG34_STATUS_REQUEST, srequest );
