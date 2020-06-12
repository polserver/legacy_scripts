// History
//   2006/05/24 Shinigami: extended progverstr and buildtagstr upto 64 chars to hold Code names

#include "stl_inc.h"
#include "progver.h"

// Update next line to make the "compile time/date" correct...
//
// Last update: 2004-04-18 by Racalac
//
const char compiledate[] = __DATE__;
const char compiletime[] = __TIME__;

char progverstr[64];
char buildtagstr[64];
unsigned long progver;
