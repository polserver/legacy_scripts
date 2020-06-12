// History
//   2005/12/08 Shinigami: New Version added to force Recompiling...
//   2006/09/16 Shinigami: New Version... Internal handling of UO.EM Function execution changed.

#ifndef __BSCRIPT_FILEFMT_H
#define __BSCRIPT_FILEFMT_H

#include "clib/rawtypes.h"

#ifdef _MSC_VER
#pragma pack(push,1)
#else
#pragma pack(1)
#endif

#define BSCRIPT_FILE_MAGIC0 'C'
#define BSCRIPT_FILE_MAGIC1 'E'
#define ESCRIPT_FILE_VER_0000_obs   0x0000
#define ESCRIPT_FILE_VER_0001_obs   0x0001
#define ESCRIPT_FILE_VER_0002_obs   0x0002

// Version 3: Changed the semantics of substrings and assignments.
// Now, some cooperation from the compiler is required.
#define ESCRIPT_FILE_VER_0003       0x0003

// Version 4: Added get/set/setconsume member by ID number. A list of
// known members is in objmember.h. This hoepfully will speed up the script
// executor by bypassing all those string compares. Note an unrecognized member
// uses the old string compare code, so structs and stuff still works. 
// todo: obj methods.
#define ESCRIPT_FILE_VER_0004       0x0004

// Version 5: A lot of changes were made in Syntax (Shift Operators "<<" and ">>"),
// but additional changes too. It's to hard to list'em all here ;o)
#define ESCRIPT_FILE_VER_0005       0x0005

// VERSION 6: Added += -= *= /= and %= operators.
#define ESCRIPT_FILE_VER_0006		0x0006

// VERSION 7: Internal handling of UO.EM Function execution changed.
#define ESCRIPT_FILE_VER_0007		0x0007

// VERSION 8: INSERT_INTO token for arrays (and later other containers) for += fixes.
#define ESCRIPT_FILE_VER_0008		0x0008

// Note to Eric: Update the following #define when you move to a new
// version so that the banner text for ecompile and runecl
// automatically reflect the changes :-)  (TJ)
#define ESCRIPT_FILE_VER_CURRENT	(ESCRIPT_FILE_VER_0008)

struct BSCRIPT_FILE_HDR
{
	char magic2[2];
	unsigned short version;
    unsigned short globals;
} POSTSTRUCT_PACK;
asserteql( sizeof(BSCRIPT_FILE_HDR), 6 );

struct BSCRIPT_SECTION_HDR
{
	unsigned short type;
	unsigned long length;
} POSTSTRUCT_PACK;
asserteql( sizeof(BSCRIPT_SECTION_HDR), 6 );

enum BSCRIPT_SECTION {
    BSCRIPT_SECTION_MODULE  =   1,
    BSCRIPT_SECTION_CODE	=   2,
    BSCRIPT_SECTION_SYMBOLS =   3,
    BSCRIPT_SECTION_PROGDEF =   4,
    BSCRIPT_SECTION_GLOBALVARNAMES  = 5,
    BSCRIPT_SECTION_EXPORTED_FUNCTIONS = 6
};


struct BSCRIPT_MODULE_HDR
{
	char modulename[ 14 ];
	unsigned long nfuncs;
} POSTSTRUCT_PACK;
asserteql( sizeof(BSCRIPT_MODULE_HDR), 18 );

struct BSCRIPT_MODULE_FUNCTION
{
	char funcname[ 33 ];
	unsigned char nargs;
} POSTSTRUCT_PACK;
asserteql( sizeof(BSCRIPT_MODULE_FUNCTION), 34 );

struct BSCRIPT_PROGDEF_HDR
{
    unsigned expectedArgs;
    unsigned char rfu[12];
} POSTSTRUCT_PACK;
asserteql( sizeof(BSCRIPT_PROGDEF_HDR), 16 );

struct BSCRIPT_GLOBALVARNAMES_HDR
{
    unsigned nGlobalVars;
} POSTSTRUCT_PACK;
asserteql( sizeof(BSCRIPT_GLOBALVARNAMES_HDR), 4 );
struct BSCRIPT_GLOBALVARNAME_HDR
{
    unsigned namelen;
} POSTSTRUCT_PACK;
asserteql( sizeof(BSCRIPT_GLOBALVARNAME_HDR), 4 );

struct BSCRIPT_DBG_INSTRUCTION
{
    unsigned filenum;
    unsigned linenum;
    unsigned blocknum;
    unsigned statementbegin;
    unsigned rfu1;
    unsigned rfu2;
} POSTSTRUCT_PACK;
asserteql( sizeof(BSCRIPT_DBG_INSTRUCTION), 24 );

struct BSCRIPT_EXPORTED_FUNCTION
{
    char funcname[33];
    unsigned nargs;
    unsigned PC;
};
asserteql( sizeof(BSCRIPT_EXPORTED_FUNCTION),41 );

#ifdef _MSC_VER
#pragma pack(pop)
#else
#pragma pack()
#endif

#endif
