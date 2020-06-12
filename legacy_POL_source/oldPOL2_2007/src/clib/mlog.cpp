// History
//   2005/06/20 Shinigami: added llog (needs defined MEMORYLEAK)

#include "stl_inc.h"

#include <fstream>

#include "mlog.h"

ofstream mlog;
#ifdef MEMORYLEAK
ofstream llog;
#endif
// multiway_ostream mlog;
