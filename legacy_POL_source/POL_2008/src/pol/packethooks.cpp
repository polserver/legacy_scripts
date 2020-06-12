// History
//   2006/05/23 Shinigami: added missing Check to ExportedPacketHookHandler() for
//                         missing default_handler in Packets with SubCommands
//   2007/08/19 Shinigami: fixed Memory Leak in PacketHook functions

#include "clib/stl_inc.h"

#include "clib/cfgelem.h"
#include "clib/cfgfile.h"
#include "clib/endian.h"
#include "clib/logfile.h"
#include "clib/fileutil.h"
#include "clib/strutil.h"

#include "bscript/bobject.h"
#include "bscript/impstr.h"

#include "plib/pkg.h"

#include "account.h"
#include "charactr.h"
#include "client.h"
#include "msghandl.h"
#include "packethooks.h"
#include "packetscrobj.h"

//stores information about each packet and its script & default handler
std::vector<PacketHookData> packet_hook_data(256);

//MSG_HANDLER and handler array defined in pol.cpp
typedef struct {
    int msglen; // if 0, no message handler defined.
    void (*func)(Client *client, void *msg);
} MSG_HANDLER;
extern MSG_HANDLER handler[ 256 ];

u32 GetSubCmd(const unsigned char* message, PacketHookData* phd)
{
    if(phd->sub_command_length == 1)
        return *(reinterpret_cast<const u8*>(&message[phd->sub_command_offset]));
    else if(phd->sub_command_length == 2)
        return cfBEu16(*(reinterpret_cast<const u16*>(&message[phd->sub_command_offset])));
    else if(phd->sub_command_length == 4)
        return cfBEu32(*(reinterpret_cast<const u32*>(&message[phd->sub_command_offset])));
    else
        return cfBEu32(*(reinterpret_cast<const u32*>(&message[phd->sub_command_offset])));

}

//MSG_HANDLER function used for each hooked packet type.
void ExportedPacketHookHandler(Client* client, void* data)
{
    //find the script handler data
    unsigned char* message = static_cast<unsigned char*>(data);
    PacketHookData* phd = &(packet_hook_data.at(message[0]));
    if(phd->function == NULL && phd->SubCommands.empty())
    {
        Log("Expected packet hook function for msg %x but was null!\n",*message);
        return;
    }

    if(!phd->SubCommands.empty())
    {
        u32 subcmd = GetSubCmd(message, phd);//cfBEu16(*(reinterpret_cast<u16*>(&message[phd->sub_command_offset])));
        map<u32,PacketHookData*>::iterator itr;
        itr = phd->SubCommands.find(subcmd);
        if(itr != phd->SubCommands.end())
        {
            if(itr->second->function != NULL)
                phd = itr->second;
        }
    }
    if(phd->function == NULL) //this will happen if the main packet entry does not define a receive function,
                              //but has subcommands, and we've received an unhooked subcmd.
    {
		if(phd->default_handler != NULL)
			phd->default_handler(client, data);
        return;
    }

    //This packet has fixed length
    if(phd->length != 0)
    {
        ref_ptr<BPacket> pkt( new BPacket(message, phd->length, false) );
        //if function returns 0, we need to call the default handler

		BObjectImp* calling_ref;
		if (client->chr)
		{
			calling_ref = client->chr->make_ref();
		}
		else
		{
			// Create the struct only if really needed...
			BStruct* client_ret = new BStruct;
			client_ret->addMember( "ip",        new String( client->ipaddrAsString().c_str() ) );
//			client_ret->addMember( "account",   new String( (client->acct != NULL)? client->acct->name():"No Account Selected" ) );

			calling_ref = client_ret;
		}

		if( phd->function->call(calling_ref , pkt.get()) == 0 )
        {
            if(phd->default_handler != NULL)
                phd->default_handler(client, static_cast<void*>(&pkt->buffer[0]));
        }
    }
    else //packet is variable length
    {
        //discover packet length, and create new packet
        unsigned short len = cfBEu16(*( reinterpret_cast<unsigned short*>(&message[1]) ));
        ref_ptr<BPacket> pkt( new BPacket(message, len, true) );
        //if function returns 0, we need to call the default handler

		BObjectImp* calling_ref;
		if (client->chr)
		{
			calling_ref = client->chr->make_ref();
		}
		else
		{
			// Create the struct only if really needed...
			BStruct* client_ret = new BStruct;
			client_ret->addMember( "ip",        new String( client->ipaddrAsString().c_str() ) );
//			client_ret->addMember( "account",   new String( (client->acct != NULL)? client->acct->name():"No Account Selected" ) );

			calling_ref = client_ret;
		}

		if( phd->function->call(calling_ref ,pkt.get()) == 0 )
        {
            if(phd->default_handler != NULL)
            {
                //the buffer size may have changed in the script, make sure the packet gets the right size
                //u16* sizeptr = (u16*)(&pkt->buffer[1]);
                //*sizeptr = ctBEu16(pkt->buffer.size());
                phd->default_handler(client, static_cast<void*>(&pkt->buffer[0]));
            }
        }
    }
}

void CallOutgoingPacketExportedFunction(Client* client, const void*& data, int& inlength, ref_ptr<BPacket>& outpacket, bool& handled)
{
    //find the script handler data
    bool subcmd_handler_exists = false;
    const unsigned char* message = static_cast<const unsigned char*>(data);
    PacketHookData* phd = &(packet_hook_data.at(message[0]));
    if(!phd->SubCommands.empty())
    {
        u32 subcmd = GetSubCmd(message, phd);//cfBEu16(*(reinterpret_cast<const u16*>(&message[phd->sub_command_offset])));
        map<u32,PacketHookData*>::iterator itr;
        itr = phd->SubCommands.find(subcmd);
        if(itr != phd->SubCommands.end())
        {
            if(itr->second->outgoing_function != NULL)
            {
                phd = itr->second;
                subcmd_handler_exists = true;
            }
        }
	}
    if(phd->outgoing_function == NULL && !subcmd_handler_exists)
    {
        handled = false;
        return;
    }

    //This packet has fixed length
    if(phd->length != 0)
    {
        outpacket.set(new BPacket(message, phd->length, false) );
        //if function returns 0, we need to call the default handler

		BObjectImp* calling_ref;
		if (client->chr)
		{
			calling_ref = client->chr->make_ref();
		}
		else
		{
			// Create the struct only if really needed...
			BStruct* client_ret = new BStruct;
			client_ret->addMember( "ip",        new String( client->ipaddrAsString().c_str() ) );
//			client_ret->addMember( "account",   new String( (client->acct != NULL)? client->acct->name():"No Account Selected" ) );

			calling_ref = client_ret;
		}

		if( phd->outgoing_function->call(calling_ref , outpacket.get()) == 0 )
        {
            data = static_cast<void*>(&outpacket->buffer[0]);
            //a fixed-length packet
            inlength = phd->length;
            handled = false;
        }
        else
            handled = true;
    }
    else //packet is variable length
    {
        //discover packet length, and create new packet
        unsigned short len = cfBEu16(*( reinterpret_cast<const unsigned short*>(&message[1]) ));
        outpacket.set(new BPacket(message, len, true) );
        //if function returns 0, we need to call the default handler

		BObjectImp* calling_ref;
		if (client->chr)
		{
			calling_ref = client->chr->make_ref();
		}
		else
		{
			// Create the struct only if really needed...
			BStruct* client_ret = new BStruct;
			client_ret->addMember( "ip",        new String( client->ipaddrAsString().c_str() ) );
//			client_ret->addMember( "account",   new String( (client->acct != NULL)? client->acct->name():"No Account Selected" ) );

			calling_ref = client_ret;
		}

		if( phd->outgoing_function->call(calling_ref ,outpacket.get()) == 0 )
        {
            //the buffer size may have changed in the script, make sure the packet gets the right size

			u16* sizeptr = reinterpret_cast<u16*>(&outpacket->buffer[1]); //var-length packets always have length at 2nd and 3rd byte
            //*sizeptr = ctBEu16(outpacket->buffer.size());

            data = static_cast<void*>(&outpacket->buffer[0]);
            //pass the new size back to client::transmit
            inlength = cfBEu16(*sizeptr);
            handled = false;
        }
        else
			handled = true;
	}
}

void load_packet_entries( const Package* pkg, ConfigElem& elem )
{
    if( stricmp(elem.type(),"Packet") != 0)
        return;
    string lengthstr;
    long length = 0;
    ExportedFunction* exfunc = (ExportedFunction*) NULL;
    ExportedFunction* exoutfunc = (ExportedFunction*) NULL;
    if(elem.has_prop("ReceiveFunction"))
        exfunc = FindExportedFunction( elem, pkg, elem.remove_string("ReceiveFunction"), 2, true );
    if(elem.has_prop("SendFunction"))
        exoutfunc = FindExportedFunction( elem, pkg, elem.remove_string("SendFunction"), 2, true );

    char *endptr = NULL;
    unsigned long idlong = strtoul( elem.rest(), &endptr, 0 );
    if ((endptr != NULL) &&
        (*endptr != '\0') &&
        !isspace(*endptr))
    {
        elem.throw_error("Packet ID not defined or poorly formed");
    }
    if(idlong > 0xFF)
        elem.throw_error("Packet ID must be between 0x0 and 0xFF");

    unsigned char id = static_cast<unsigned char>(idlong);

    unsigned short subcmdoff;
    if( !elem.remove_prop( "SubCommandOffset", &subcmdoff) )
        subcmdoff = 0;
    unsigned short subcmdlen;
    if( !elem.remove_prop( "SubCommandLength", &subcmdlen) )
        subcmdlen = 0;

    if( elem.remove_prop( "Length", &lengthstr ) )
    {
        if( lengthstr == "variable" )
            length = 0;
        else
        {   
            unsigned short temp;
            endptr = NULL;
            temp = (unsigned short) strtoul( lengthstr.c_str(), &endptr, 0 );
            if ((endptr != NULL) &&
                (*endptr != '\0') &&
                !isspace(*endptr))
            {
                elem.throw_error("Length must be an integer or 'variable'");
            }
            else
                length = temp;
        }

    }
    else
        elem.throw_error("Length property missing.");

    ExportedFunction* existing_in_func = packet_hook_data.at(id).function;
    ExportedFunction* existing_out_func = packet_hook_data.at(id).outgoing_function;
    if(existing_in_func != NULL)
        Log("Packet hook receive function multiply defined for packet %x!\n",id);
    if(existing_out_func != NULL)
        Log("Packet hook send function multiply defined for packet %x!\n",id);

    //assign to packet hook data
    packet_hook_data.at(id).function = exfunc; 
    packet_hook_data.at(id).outgoing_function = exoutfunc;
    packet_hook_data.at(id).length = length;
    packet_hook_data.at(id).sub_command_offset = subcmdoff;
    packet_hook_data.at(id).sub_command_length = subcmdlen;
    if (handler[id].msglen)
        packet_hook_data.at(id).default_handler = handler[id].func;

    //if the incoming handler doesn't exist (i.e. is only serv->client packet), don't install handler
    //if(exfunc != NULL)
    //{
        if(length == 0)
            MessageHandler( id, MSGLEN_2BYTELEN_DATA, ExportedPacketHookHandler );
        else
            MessageHandler( id, length, ExportedPacketHookHandler );
    //}
}

void load_subpacket_entries( const Package* pkg, ConfigElem& elem )
{
    if( stricmp(elem.type(),"SubPacket") != 0)
        return;
    ExportedFunction* exfunc = (ExportedFunction*) NULL;
    ExportedFunction* exoutfunc = (ExportedFunction*) NULL;
    if(elem.has_prop("ReceiveFunction"))
        exfunc = FindExportedFunction( elem, pkg, elem.remove_string("ReceiveFunction"), 2, true );
    if(elem.has_prop("SendFunction"))
        exoutfunc = FindExportedFunction( elem, pkg, elem.remove_string("SendFunction"), 2, true );

    char *endptr = NULL;
    unsigned long idlong = strtoul( elem.rest(), &endptr, 0 );
    if ((endptr != NULL) &&
        (*endptr != '\0') &&
        !isspace(*endptr))
    {
        elem.throw_error("Packet ID not defined or poorly formed");
    }
    if(idlong > 0xFF)
        elem.throw_error("Packet ID must be between 0x0 and 0xFF");

    unsigned char id = static_cast<unsigned char>(idlong);
    
    unsigned short subid = elem.remove_ushort( "SubCommandID" );

    PacketHookData* parent = &packet_hook_data.at(id);
    //validate that the parent packet has a definition and a SubCommandOffset
    if(!parent->sub_command_offset)
        elem.throw_error( string("Parent packet " + hexint(id) + " does not define SubCommandOffset!") );
    if(!parent->sub_command_length)
        elem.throw_error( string("Parent packet " + hexint(id) + " does not define SubCommandLength") );
    if(parent->SubCommands.find(subid) != parent->SubCommands.end())
        elem.throw_error( string("SubCommand " + hexint(subid) + " for packet " + hexint(id) + " multiply defined!") );

    PacketHookData* SubData = new PacketHookData();
    SubData->function = exfunc;
    SubData->outgoing_function = exoutfunc;
    SubData->length = parent->length;
    SubData->default_handler = parent->default_handler;
    
    parent->SubCommands.insert( make_pair(subid,SubData) );
}

//loads "uopacket.cfg" entries from packages
void load_packet_hooks()
{
    load_packaged_cfgs( "uopacket.cfg", "packet subpacket", load_packet_entries );
    load_packaged_cfgs( "uopacket.cfg", "packet subpacket", load_subpacket_entries );
}

//unload packet hooks
