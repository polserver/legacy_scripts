// History
//   2007/04/07 MuadDib: send_object_cache_to_inrange updated from just UObject* to
//						 const UObject* for compatibility across more areas.

#include "clib/stl_inc.h"
#include "clib/endian.h"

#include "charactr.h"
#include "client.h"
//#include "item.h"
#include "itemdesc.h"
#include "msghandl.h"
#include "nmsgtype.h"
#include "ssopt.h"
#include "ufunc.h"
#include "umsgtype.h"
#include "uvars.h"

#include "tooltips.h"


void handle_request_tooltip( Client* client, MSGB6_REQUEST_TOOLTIP* msgin )
{
    u32 serial = cfBEu32( msgin->serial );
    if (IsCharacter( serial ))
    {
    }
    else
    {
        Item* item = find_legal_item( client->chr, serial );
        if (item != NULL)
        {
            const ItemDesc& id = find_itemdesc( item->objtype_ );
            if (!id.tooltip.empty())
            {
                static MSGB7_DISPLAY_TOOLTIP msg;
                msg.msgtype = MSGB7_DISPLAY_TOOLTIP_ID;
                u16 msglen = offsetof( MSGB7_DISPLAY_TOOLTIP, text ) +
                             id.tooltip.length() * 2 + 2;
                msg.msglen = ctBEu16( msglen );
                msg.serial = item->serial_ext;
                for( unsigned i = 0; i < id.tooltip.length(); ++i )
                {
                    msg.text[ i*2 ] = '\0';
                    msg.text[ i*2 + 1 ] = id.tooltip[ i ];
                }
                msg.text[ id.tooltip.length() * 2 ] = '\0';
                msg.text[ id.tooltip.length() * 2 + 1 ] = '\0';
                client->transmit( &msg, msglen );
            }
        }
    }
}
MESSAGE_HANDLER( MSGB6_REQUEST_TOOLTIP, handle_request_tooltip );

//needed if A9 flag is sent with 0x20, single click no longer works. see about text# 1042971 for 0xD6
void send_object_cache(Client* client, const UObject* obj)
{
	if (ssopt.uo_feature_enable & MSGA9_START_FLAGS::FLAG_AOS_FEATURES)
	{
		MSGBF_GENERIC_COMMAND msg;
	    msg.msgtype = MSGBF_GENERIC_COMMAND_ID;
		msg.msglen = ctBEu16(0xD);
	    msg.subcmd = ctBEu16(MSGBF_GENERIC_COMMAND::TYPE_OBJECT_CACHE);
		msg.objectcache.serial = obj->serial_ext;
	    msg.objectcache.listid = ctBEu32(obj->rev());
		client->transmit(&msg, 0xD);
	}
	else
	{
		return;
	}
}

void send_object_cache_to_inrange(const UObject* obj)
{
    for( Clients::iterator itr = clients.begin(), end = clients.end(); itr != end; ++itr )
    {
        Client *client2 = *itr;
        if (!client2->ready)
            continue;
		if (ssopt.uo_feature_enable & MSGA9_START_FLAGS::FLAG_AOS_FEATURES)
		{
	        // FIXME need to check character's additional_legal_items.
		    // looks like inrange should be a Character member function.
			if (inrange(client2->chr, obj))
	        {
				////////aostest
				if(client2->UOExpansionFlag & AOS)
				{
					send_object_cache(client2, obj);
				}
				////////
			}
		}
		else
		{
			continue;
		}
	}
}

void SendAOSTooltip(Client* client, UObject* obj, bool vendor_content)
{
    string desc;
    if(obj->isa(UObject::CLASS_CHARACTER))
    {
        Character* chr = (Character*)obj;
        desc = (chr->title_prefix.empty() ? " \t": chr->title_prefix + "\t") + chr->name() + 
			(chr->title_suffix.empty() ? "\t ": "\t " + chr->title_suffix);
        if (!chr->title_race.empty())
            desc += " (" +  chr->title_race + ")";
    }
    else
		if (vendor_content)
		{
			Item* item = (Item*)obj;
			desc = item->merchant_description();
		}
		else
			desc = obj->description();

    int len = desc.size()*2;
    string out(len,0);
    
    int packetsize = 25+len;

    for(unsigned int i=0,j=0; j<desc.size(); i++,j++)
    {
        out[i] = desc[j];
        i++;
        out[i] = 0;
    }


    unsigned char* buffer = new unsigned char[packetsize];
    MSGD6_AOS_OBJECT_PROPERTY_LIST* msg = reinterpret_cast<MSGD6_AOS_OBJECT_PROPERTY_LIST*>(buffer);
    msg->msgtype = MSGD6_AOS_OBJECT_PROPERTY_LIST_ID;
    msg->msglen = ctBEu16(packetsize);
    msg->unk1 = ctBEu16(1);
    msg->serial = obj->serial_ext;
    msg->unk2 = 0;
    msg->unk3 = 0;
    msg->listid = ctBEu32(obj->rev());

	if(obj->isa(UObject::CLASS_CHARACTER))
	{
		msg->data->localization_num = ctBEu32(1050045); //1 text argument only
	}
	else
		msg->data->localization_num = ctBEu32(1042971); //1 text argument only

	msg->data->textlen = ctBEu16(len);
	memcpy( reinterpret_cast<u8*>(&(msg->data->text)), out.c_str(), len );
    *(reinterpret_cast<u32*>(&buffer[packetsize-4])) = 0; //indicates end of property list

    client->transmit(buffer,packetsize);
}
