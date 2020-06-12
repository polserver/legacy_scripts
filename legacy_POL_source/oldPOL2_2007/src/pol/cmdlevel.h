/*
#define CMDLEVEL_PLAYER 0
#define CMDLEVEL_COUN   1
#define CMDLEVEL_SEER   2
#define CMDLEVEL_GM     3
#define CMDLEVEL_ADMIN  4
#define CMDLEVEL_TEST   5

#define CMDLEVEL__HIGHEST 5

#define MAX_CMDLEVELS   6
*/

/*
extern char* cmdlevels[ MAX_CMDLEVELS ];
extern char* cmdleveldirs[ MAX_CMDLEVELS ];
*/

#include <string>
#include <vector>

#include "bscript/bobject.h"

class ConfigElem;
class Package;

class CmdLevel
{
public:
    CmdLevel( ConfigElem& elem, int cmdlevelnum );

    bool matches( const std::string& name ) const;
    void add_searchdir( Package* pkg, const std::string& dir );
    void add_searchdir_front( Package* pkg, const std::string& dir );

    std::string name;
    int cmdlevel;
    
    struct SearchDir
    {
        Package* pkg;
        std::string dir;
    };

    typedef std::vector< SearchDir > SearchList;
    SearchList searchlist;

    typedef std::vector< std::string > Aliases;
    Aliases aliases;
};

typedef std::vector< CmdLevel > CmdLevels;
extern CmdLevels cmdlevels2;

CmdLevel* find_cmdlevel( const char* name );
CmdLevel* FindCmdLevelByAlias( const std::string& str );

ObjArray* GetCommandsInPackage(Package* m_pkg, long cmdlvl_num);


