#ifndef PACKETHOOKS_H
#define PACKETHOOKS_H
#include "clib/stl_inc.h"

#include "syshook.h"
#include "client.h"
#include "packetscrobj.h"

class PacketHookData 
{
public:
    PacketHookData() :
        length(0),
        function(NULL),
        outgoing_function(NULL),
        default_handler(NULL),
        sub_command_offset(0),
        sub_command_length(0)
    {
    };
    ~PacketHookData(){};

    int length; // if 0, variable length
    ExportedFunction* function;
    ExportedFunction* outgoing_function;
    void(*default_handler)(Client *client, void *msg);
    unsigned short sub_command_offset;
    unsigned short sub_command_length;
    map<u32,PacketHookData*>SubCommands;
};

extern std::vector<PacketHookData> packet_hook_data;
void load_packet_hooks();
void ExportedPacketHookHandler(Client* client, void* data);
void CallOutgoingPacketExportedFunction(Client* client, const void*& data, int& inlength, ref_ptr<BPacket>& outpacket, bool& handled);

#endif
