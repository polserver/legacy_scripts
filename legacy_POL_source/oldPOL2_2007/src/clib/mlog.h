// History
//   2005/06/20 Shinigami: added llog (needs defined MEMORYLEAK)

#include "stl_inc.h"
/*
#include "mstream.h"

extern multiway_ostream mlog;
*/

extern ofstream mlog;
#ifdef MEMORYLEAK
extern ofstream llog;
#endif
