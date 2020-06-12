#include "clib/stl_inc.h"

#include "clib/cfgfile.h"
#include "clib/cfgelem.h"
#include "clib/dirlist.h"
#include "clib/xmain.h"

#include "compilercfg.h"

void CompilerConfig::Read( const string& path )
{
    ConfigFile cf( path.c_str() );
    ConfigElem elem;
    cf.readraw( elem );

    PackageRoot.clear();
    IncludeDirectory.clear();

    string tmp;
    while (elem.remove_prop( "PackageRoot", &tmp ))
    {
        PackageRoot.push_back( normalized_dir_form(tmp) );
    }
    if (elem.remove_prop( "IncludeDirectory", &tmp ))
    {
        IncludeDirectory = normalized_dir_form(tmp);
    }
    ModuleDirectory = normalized_dir_form(elem.remove_string( "ModuleDirectory" ));
    PolScriptRoot = normalized_dir_form(elem.remove_string( "PolScriptRoot" ));
	GenerateListing = elem.remove_bool( "GenerateListing", false );
	GenerateDebugInfo = elem.remove_bool( "GenerateDebugInfo", false );
	GenerateDebugTextInfo = elem.remove_bool( "GenerateDebugTextInfo", false );
    DisplayWarnings = elem.remove_bool( "DisplayWarnings", false );
    CompileAspPages = elem.remove_bool( "CompileAspPages", false );
    AutoCompileByDefault = elem.remove_bool( "AutoCompileByDefault", false );
    UpdateOnlyOnAutoCompile = elem.remove_bool( "UpdateOnlyOnAutoCompile", false );
    OnlyCompileUpdatedScripts = elem.remove_bool( "OnlyCompileUpdatedScripts", false );
    DisplaySummary = elem.remove_bool( "DisplaySummary", false );
    OptimizeObjectMembers = elem.remove_bool( "OptimizeObjectMembers", true);
    // GenerateDependencyInfo defaults to OnlyCompileUpdatedScripts setting
    GenerateDependencyInfo = elem.remove_bool( "GenerateDependencyInfo", OnlyCompileUpdatedScripts );

    DisplayUpToDateScripts = elem.remove_bool( "DisplayUpToDateScripts", true );
}

void CompilerConfig::SetDefaults()
{
    const char* tmp;
    
    tmp = getenv( "ECOMPILE_PATH_EM" );
    ModuleDirectory = tmp ? normalized_dir_form(tmp) : xmain_exedir;
    
    tmp = getenv( "ECOMPILE_PATH_INC" );
    IncludeDirectory = tmp ? normalized_dir_form(tmp) : xmain_exedir;

    PolScriptRoot = IncludeDirectory;

    DisplayUpToDateScripts = true;
}

CompilerConfig compilercfg;
