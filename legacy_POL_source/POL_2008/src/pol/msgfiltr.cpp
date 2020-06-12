// History
//   2005/01/24 Shinigami: added packet 0xd9 (Spy on Client 2) to exceptions
//   2005/08/29 Shinigami: SPY_ON_CLIENT renamed to CLIENT_INFO
//   2007/07/08 Shinigami: added MSGE4_KR_VERIFIER_RESPONSE_ID to login_exceptions (used by UO:KR)

#include "clib/stl_inc.h"
#include <string.h>
#include "nmsgtype.h"
#include "umsgtype.h"

#include "msgfiltr.h"

MessageTypeFilter::MessageTypeFilter( MessageDefault def, int* exceptions )
{
    unsigned char normal;
    unsigned char abnormal;
    if (def == NormallyDisabled)
        normal = 0;
    else
        normal = 1;
    abnormal = !normal;

    memset( msgtype_allowed, normal, sizeof msgtype_allowed );
    
    while (*exceptions != -1)
    {
        msgtype_allowed[ *exceptions & 0xFF ] = abnormal;
        ++exceptions;
    }
}

int login_exceptions[] =
{
    MSG00_CHAR_CREATE_ID,
    MSG5D_CHAR_SELECT_ID,
    MSG73_KEEP_ALIVE_ID,
	MSG80_LOGINSERVER_LOGIN_ID,
    MSG83_DELETE_CHARACTER_ID,
    MSG91_GAMESERVER_LOGIN_ID,
    MSGA0_SERVER_SELECT_ID,
    MSGA4_CLIENT_INFO_ID,
    MSGD9_CLIENT_INFO_ID,
	MSGE4_KR_VERIFIER_RESPONSE_ID, // Used by UO:KR
    -1
};
MessageTypeFilter login_filter( MessageTypeFilter::NormallyDisabled, login_exceptions );

int game_exceptions[] =
{
    MSG00_CHAR_CREATE_ID,
    MSG5D_CHAR_SELECT_ID,
	MSG80_LOGINSERVER_LOGIN_ID,
    MSG83_DELETE_CHARACTER_ID,
    MSG91_GAMESERVER_LOGIN_ID,
    MSGA0_SERVER_SELECT_ID,
    MSGA4_CLIENT_INFO_ID,
    MSGD9_CLIENT_INFO_ID,
    -1
};
MessageTypeFilter game_filter( MessageTypeFilter::NormallyEnabled, game_exceptions );

int no_exceptions[] =
{
    -1
};
MessageTypeFilter disconnected_filter( MessageTypeFilter::NormallyDisabled, no_exceptions );
