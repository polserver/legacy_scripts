#include "clib/stl_inc.h"

#include <stddef.h>

#include "clib/endian.h"

#include "msghandl.h"
#include "umsgtype.h"
#include "tiplist.h"
#include "sockio.h"

TipFilenames tipfilenames;

static MSGA6_TIP_WINDOW tipmsg;

bool send_tip( Client* client, const char* tipname, unsigned short tipnum )
{
    ifstream ifs( tipname );
    ifs.read( tipmsg.text, sizeof tipmsg.text );
    streamsize textlen = ifs.gcount();
    if ( textlen > 0 && unsigned(textlen) <= sizeof tipmsg.text)
    {
        int msglen = offsetof(MSGA6_TIP_WINDOW, text) + textlen;

        tipmsg.msgtype = MSGOUT_A6_TIP_WINDOW_ID;
        tipmsg.msglen = ctBEu16(msglen);
        tipmsg.type = MSGA6_TIP_WINDOW::TYPE_TIP;
        tipmsg.unk4 = 0;
        tipmsg.unk5 = 0;
        tipmsg.tipnum = ctBEu16(tipnum);
        tipmsg.textlen = ctBEu16(textlen);
        // tipmsg.text read in above

        transmit( client, &tipmsg, msglen );
        return true;
    }
    else
    {
        return false;
    }
}

void send_tip( Client* client, const std::string& tiptext )
{
    unsigned textlen = tiptext.size();
    if (textlen >= sizeof tipmsg.text)
        textlen = sizeof tipmsg.text - 1;
    
    int msglen = offsetof(MSGA6_TIP_WINDOW, text) + textlen;

    tipmsg.msgtype = MSGOUT_A6_TIP_WINDOW_ID;
    tipmsg.msglen = ctBEu16(msglen);
    tipmsg.type = MSGA6_TIP_WINDOW::TYPE_TIP;
    tipmsg.unk4 = 0;
    tipmsg.unk5 = 0;
    tipmsg.tipnum = 0;
    tipmsg.textlen = ctBEu16(textlen);
    memcpy( tipmsg.text, tiptext.c_str(), textlen );
    tipmsg.text[textlen] = '\0';

    transmit( client, &tipmsg, msglen );
}

/*
bool send_tip( Client* client, const char* tipname )
{
    TipListByName::iterator itr = tiplist_byname.find( tipname );
    if (itr != tiplist_byname.end())
    {
        return send_tip( client, tipname, (*itr).second );
    }
    else
    {
        return send_tip( client, tipname, 0 );
    }
}
*/

void handle_get_tip( Client* client, MSGA7_GET_TIP* msg )
{
	u16 tipnum = cfBEu16( msg->lasttip );
    if (tipfilenames.size())
    {
        if (msg->prevnext)
        {
            ++tipnum;
            if (tipnum >= tipfilenames.size())
                tipnum = 0;
        }
        else
        {
            --tipnum;
            if (tipnum >= tipfilenames.size())
                tipnum = tipfilenames.size() - 1;
        }

        send_tip( client, tipfilenames[tipnum].c_str(), tipnum );
    }
}

MESSAGE_HANDLER( MSGA7_GET_TIP, handle_get_tip );
