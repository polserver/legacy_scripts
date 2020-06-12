// History
//   2005/11/26 Shinigami: changed "strcmp" into "stricmp" to suppress Script Errors

#include "clib/stl_inc.h"

#include "bscript/bobject.h"
#include "bscript/berror.h"
#include "bscript/bstruct.h"
#include "bscript/impstr.h"

#include "clib/cfgelem.h"
#include "clib/esignal.h"
#include "clib/sckutil.h"
#include "clib/socketsvc.h"
#include "clib/stlutil.h"
#include "clib/strutil.h"
#include "clib/threadhelp.h"
#include "clib/weakptr.h"

#include "plib/pkg.h"

#include "core.h"
#include "polsem.h"
#include "scrdef.h"
#include "scrsched.h"
#include "uoemod.h"
#include "uoexec.h"

class AuxClientThread;

class AuxConnection : public BObjectImp
{
public:
    AuxConnection( AuxClientThread* auxclientthread ) : 
        BObjectImp( OTUnknown ),
        _auxclientthread( auxclientthread )
    {}

    virtual BObjectImp* copy() const;
    virtual bool isTrue() const;
    virtual std::string getStringRep() const;
    virtual unsigned long sizeEstimate() const;

    virtual BObjectImp* call_method( const char* methodname, Executor& ex );
	virtual BObjectRef get_member( const char *membername );

    void disconnect();

private:
    AuxClientThread* _auxclientthread;
};

class AuxService
{
public:
    AuxService( const Package* pkg, ConfigElem& elem );
    void run();

    const ScriptDef& scriptdef() const { return _scriptdef; }
private:
    const Package* _pkg;
    ScriptDef _scriptdef;
    unsigned short _port;
};

class AuxClientThread : public SocketClientThread
{
public:
    AuxClientThread( AuxService* auxsvc, SocketListener& listener );

    virtual void run();
    void transmit( const BObjectImp* imp );
    BObjectImp* get_ip();
    
private:
    void init();

    AuxService* _auxservice;
    ref_ptr<AuxConnection> _auxconnection;
    weak_ptr<UOExecutor> _uoexec;
};

BObjectImp* AuxConnection::copy() const
{
    return const_cast<AuxConnection*>(this);
}

std::string AuxConnection::getStringRep() const
{
    return "<AuxConnection>";
}

unsigned long AuxConnection::sizeEstimate() const
{
    return sizeof(AuxConnection);
}

bool AuxConnection::isTrue() const
{
    return (_auxclientthread != NULL);
}

BObjectRef AuxConnection::get_member( const char *membername )
{
    if (stricmp( membername, "ip" ) == 0)
    {
        return BObjectRef(_auxclientthread->get_ip());
    }
    return BObjectRef(UninitObject::create());
}

BObjectImp* AuxConnection::call_method( const char* methodname, Executor& ex )
{
    if (stricmp( methodname, "transmit" ) == 0)
    {
        if (ex.numParams()==1)
        {
            if (_auxclientthread != NULL)
            {
                BObjectImp* value = ex.getParamImp(0);
                // FIXME this can block!
                _auxclientthread->transmit( value );
            }
            else
            {
                return new BError( "Client has disconnected" );
            }
        }
        else
        {
            return new BError( "1 parameter expected" );
        }
    }
    return NULL;
}

void AuxConnection::disconnect()
{
    _auxclientthread = NULL;
}

AuxClientThread::AuxClientThread( AuxService* auxsvc, SocketListener& listener ) : 
    SocketClientThread(listener), 
    _auxservice(auxsvc),
    _uoexec(0)
{
}

void AuxClientThread::init()
{
    PolLock lock;
    _auxconnection.set( new AuxConnection( this ) );

    UOExecutorModule* uoemod = start_script( _auxservice->scriptdef(), _auxconnection.get() ); 
    _uoexec = uoemod->uoexec.weakptr;
}

void AuxClientThread::run()
{
    init();

    string tmp;
    while (readline( _sck, tmp ))
    {
        PolLock lock;

        if (_uoexec.exists())
        {
            istringstream is(tmp);
            BObjectImp* value = BObjectImp::unpack( is );

            BStruct* event = new BStruct;
            event->addMember( "type", new String( "recv" ) );
            event->addMember( "value", value );
            _uoexec->os_module->signal_event( event );
        }
        else
        {   // the controlling script dropped its last reference to the connection,
            // by exiting or otherwise.
            break;
        }
    }
    
    PolLock lock;
    _auxconnection->disconnect();
    // the auxconnection is probably referenced by another ref_ptr, 
    // so its deletion must be protected by the lock.  
    // Clear our reference:
    _auxconnection.clear();
}

void AuxClientThread::transmit( const BObjectImp* value )
{
    string tmp = value->pack();
    writeline( _sck, tmp );
}

BObjectImp* AuxClientThread::get_ip()
{
    return new String(_sck.getpeername());
}

AuxService::AuxService( const Package* pkg, ConfigElem& elem ) :
    _pkg(pkg),
    _scriptdef( elem.remove_string( "SCRIPT" ), _pkg ),
    _port( elem.remove_ushort( "PORT" ) )
{
}

void AuxService::run()
{
    atomic_cout( "Starting Aux Listener (" + _scriptdef.relativename() + ", port " + decint(_port) + ")" );

    SocketListener listener( _port );
    while (!exit_signalled)
    {
        if (listener.GetConnection( 5 ))
        {
            // Shinigami: Just 4 Debugging. We got Crashes here...
            #ifdef PERGON
			cerr << "Aux Listener (" << _scriptdef.relativename() << ", port " << decint(_port) << ") - create Thread" << endl;
            #endif
			SocketClientThread* clientthread = new AuxClientThread( this, listener );
            clientthread->start(); 
            // note SocketClientThread::start deletes the SocketClientThread upon thread exit.
        }
    }
}


typedef vector< AuxService* > AuxServices;
AuxServices auxservices;

void aux_service_thread_stub( void* arg )
{
    AuxService* as = reinterpret_cast<AuxService*>(arg);
    as->run();
}

void start_aux_services()
{
    for( unsigned i = 0; i < auxservices.size(); ++i )
    {
        threadhelp::start_thread( aux_service_thread_stub, "AuxService", auxservices[i] );
    }
}

void load_auxservice_entry( const Package* pkg, ConfigElem& elem )
{
    auxservices.push_back( new AuxService(pkg,elem) );
}

void load_aux_services()
{
    load_packaged_cfgs( "auxsvc.cfg", "AuxService", load_auxservice_entry );
}

void unload_aux_services()
{
    delete_all( auxservices );
}
