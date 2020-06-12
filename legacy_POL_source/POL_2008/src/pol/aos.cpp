#include "clib/stl_inc.h"
#include "clib/endian.h"

#include "client.h"
#include "customhouses.h"
#include "msghandl.h"
#include "nmsgtype.h"
#include "ssopt.h"


void handle_unknown_packet( Client* client );

void handle_AOS_Command( Client* client, MSGD7_AOS_GENERIC_COMMAND* msg )
{
    switch( cfBEu16(msg->subcmd) )
    {
        //fixme: fallthrough until each subcmd is implimented
    case MSGD7_AOS_GENERIC_COMMAND::CUSTOM_HOUSE_ADD:
        CustomHousesAdd(msg); break;
    case MSGD7_AOS_GENERIC_COMMAND::CUSTOM_HOUSE_ERASE:
        CustomHousesErase(msg); break;
    case MSGD7_AOS_GENERIC_COMMAND::CUSTOM_HOUSE_QUIT:
        CustomHousesQuit(msg); break;
    case MSGD7_AOS_GENERIC_COMMAND::CUSTOM_HOUSE_CLEAR:
        CustomHousesClear(msg); break;
    case MSGD7_AOS_GENERIC_COMMAND::CUSTOM_HOUSE_COMMIT:
        CustomHousesCommit(msg); break;
    case MSGD7_AOS_GENERIC_COMMAND::CUSTOM_HOUSE_SELECT_FLOOR:
        CustomHousesSelectFloor(msg); break;
    case MSGD7_AOS_GENERIC_COMMAND::CUSTOM_HOUSE_BACKUP:
        CustomHousesBackup(msg); break;
    case MSGD7_AOS_GENERIC_COMMAND::CUSTOM_HOUSE_RESTORE:
        CustomHousesRestore(msg); break;
    case MSGD7_AOS_GENERIC_COMMAND::CUSTOM_HOUSE_SYNCH:
        CustomHousesSynch(msg); break;
    case MSGD7_AOS_GENERIC_COMMAND::CUSTOM_HOUSE_REVERT:
        CustomHousesRevert(msg); break;
    case MSGD7_AOS_GENERIC_COMMAND::CUSTOM_HOUSE_ADD_MULTI:
        CustomHousesAddMulti(msg); break;
    
    case MSGD7_AOS_GENERIC_COMMAND::FIGHT_BOOK: //implement me!
    default: handle_unknown_packet( client );
    }
}
MESSAGE_HANDLER_VARLEN( MSGD7_AOS_GENERIC_COMMAND, handle_AOS_Command );
