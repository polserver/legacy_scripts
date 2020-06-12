// History
//   2005/03/01 Shinigami: added MAX_NUMER_REALMS for use in MSGBF_SUB18_ENABLE_MAP_DIFFS

#ifndef REALMS_H
#define REALMS_H

class Realm;

extern Realm* main_realm;
extern vector<Realm*>* Realms;
Realm* find_realm(const string name);
#define ENABLE_OLD_MAPCODE 0

// Support up to 5 Maps: Britannia, Britannia_alt, Ilshenar, Malas, Tokuno
#define MAX_NUMER_REALMS 5

#endif
