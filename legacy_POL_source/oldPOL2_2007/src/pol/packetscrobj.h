#ifndef PACKETSCROBJ_H
#define PACKETSCROBJ_H

#include "bscript/bobject.h"
#include "clib/rawtypes.h"

class BPacket : public BObjectImp
{
public:
    BPacket();
    BPacket(const BPacket& copyfrom);
    BPacket(u8 type, signed short length);
    BPacket(const unsigned char* data, unsigned short length, bool variable_len);
    ~BPacket();

    vector<unsigned char> buffer;
    virtual BObjectRef get_member( const char* membername );
    virtual BObjectRef get_member_id( const int id ); //id test
    virtual BObjectImp* call_method( const char* methodname, Executor& ex );
    virtual BObjectImp* call_method_id( const int id, Executor& ex );
    virtual BObjectImp* copy() const;
    virtual std::string getStringRep() const;
    BObjectImp* SetSize(u16 newsize);
    virtual unsigned long sizeEstimate() const { return buffer.size(); }
    virtual const char* typeOf() const { return "Packet"; }

    bool is_variable_length;
};

#endif
