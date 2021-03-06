/*
        Maintened by *Edwards

        2009-02-10
*/
use uo;
use os;
use cfgfile;

include ":datafile:datafile";
include ":spawnsystem:settings";

function SS_GetEnginesList()

	var cfg_file := SS_GetSettingsCfgFile();
	var cfg_elem := SS_GetSettingsCfgElem( cfg_file, "EngineList" );
	
	return GetConfigStringArray( cfg_elem, "Engine" );
endfunction

function SS_EngineEnabled( engine_name )

	var settings := SS_GetSettingsCfgFile( engine_name );
	settings := settings["Settings"];

	return CInt( settings.Enabled );
endfunction

function SS_StartSpawnEngine( engine_name, realm_name, region_name, group_name )

	var params := array{engine_name, realm_name, region_name, group_name};
	var script := Start_Script( SS_GetEnginePath( engine_name ), params );

	return script;
endfunction

function SS_GetEnginePath( engine_name )

	return ":spawnsystem:engines/"+engine_name+"/spawnCycle";
endfunction
