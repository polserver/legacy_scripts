// History
//   2005/08/19 Shinigami: Ready for VS.Net 2003
//   2006/06/09 Shinigami: Pol096 Final

#include "clib/stl_inc.h"
#include "clib/progver.h"
#include "polver.h"

const char polverstr[] = "POL097-2007-04-15 RC3 Coregina";
unsigned long polver = 97;

#ifdef PERGON
#define PERGON_TAG " - Pergon"
#else
#define PERGON_TAG ""
#endif

#ifdef MEMORYLEAK
#define MEMORYLEAK_TAG " - MemoryLeak"
#else
#define MEMORYLEAK_TAG ""
#endif

#ifdef POL_BUILDTAG
  const char polbuildtag[] = POL_BUILDTAG""PERGON_TAG""MEMORYLEAK_TAG;
#elif defined(_MSC_VER) && (_MSC_VER <= 1200)
  const char polbuildtag[] = "VC6"PERGON_TAG""MEMORYLEAK_TAG;
#elif defined(_MSC_VER) && (_MSC_VER <= 1300)
  const char polbuildtag[] = "VS.NET"PERGON_TAG""MEMORYLEAK_TAG;
#elif defined(_MSC_VER) && (_MSC_VER <= 1310)
  const char polbuildtag[] = "VS.NET 2003"PERGON_TAG""MEMORYLEAK_TAG;
#endif
