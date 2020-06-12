#ifndef UOCLIENT_H
#define UOCLIENT_H

class UoClientGeneral
{
public:
    void check( std::string& var, const char* tag, const char* deflt );

public:

    class Mapping
    {
    public:
        bool any;
        std::string name;
        unsigned id;
    };

    Mapping strength;
    Mapping intelligence;
    Mapping dexterity;

    Mapping hits;
    Mapping stamina;
    Mapping mana;
	unsigned int maxskills; //dave changed 3/15/03, support configurable max skillid
	unsigned int statcap; //muaddib changed 06/14/05, support configurable stat cap
};

extern UoClientGeneral uoclient_general;

class UoClientProtocol
{
public:
    bool EnableFlowControlPackets;
};
extern UoClientProtocol uoclient_protocol;

class ConfigElem;
class UoClientListener
{
public:
    UoClientListener( ConfigElem& elem );

    unsigned port;
    string encryption;
};

typedef vector< UoClientListener > UoClientListeners;

extern UoClientListeners uoclient_listeners;

#endif
