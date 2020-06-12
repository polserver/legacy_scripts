#include "clib/stl_inc.h"

#include "clib/cfgelem.h"
#include "clib/cfgfile.h"
#include "clib/fileutil.h"
#include "client.h"
//#include "itemdesc.h"


bool is_banned_ip(Client* client)
{
    if (!FileExists( "config/bannedips.cfg" ))
        return false;

    ConfigFile cf( "config/bannedips.cfg" );

    ConfigElem elem;
    unsigned long ipaddr;
    unsigned long ipmask;

    while (cf.read( elem ))
    {
		string iptext = elem.remove_string( "IPMatch" );

            string::size_type delim = iptext.find_first_of( "/" );
            if (delim != string::npos)
            {
                string ipaddr_str = iptext.substr( 0, delim );
                string ipmask_str = iptext.substr( delim+1 );
                ipaddr = inet_addr( ipaddr_str.c_str() );
                ipmask = inet_addr( ipmask_str.c_str() );
            }
            else
            {
                ipaddr = inet_addr( iptext.c_str() );
				string ipmask_str = "255.255.255.255";
                ipmask = inet_addr( ipmask_str.c_str() );
            }
			
        unsigned long addr1part, addr2part;
        struct sockaddr_in* sockin = reinterpret_cast<struct sockaddr_in*>(&client->ipaddr);
        addr1part = ipaddr & ipmask;

#ifdef _WIN32
        addr2part = sockin->sin_addr.S_un.S_addr & ipmask;
#else
        addr2part = sockin->sin_addr.s_addr      & ipmask;
#endif

        if (addr1part == addr2part)
            return true;
    }
	return false;
}
