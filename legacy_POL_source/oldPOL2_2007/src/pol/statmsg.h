#ifndef __STATMSG_H
#define __STATMSG_H

class Client;
class Character;

void send_full_statmsg( Client *client, Character *chr );
void send_short_statmsg( Client *client, Character *chr );
void send_update_hits_to_inrange( Character *chr );

#endif
