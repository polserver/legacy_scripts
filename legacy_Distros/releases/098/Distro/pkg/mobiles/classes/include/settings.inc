//$Id: settings.inc 544 2006-07-04 03:13:44Z austinheilman $

/*===============================================================
* Current Version
* SETTINGS.INC - v1.0
* Updated 7/2/2006 5:25PM
*
* -- Revision v1.0 --
* Austin:
*  Created include file
===============================================================*/

use uo;
use os;
use cfgfile;

/*
 * CLASS_GetSettingsCfgFile(engine_name)
 *
 * Purpose
 * Reads in :damage:configs/settings.cfg
 *
 * Parameters
 *
 * Return value
 * A config file reference.
 *
 */
function CLASS_GetSettingsCfgFile()
	var cfg := ReadConfigFile(":classes:settings");

	if ( cfg.errortext )
		SysLog("Error::CLASS_GetSettingsCfgFile() - Unable to open [:classes:settings.cfg] ->"+cfg.errortext);
	endif

	return cfg;
endfunction

/*
 * CLASS_GetSettingsCfgElem(elem_name, cfg_file)
 *
 * Purpose
 * Retrieves an elem from a config file.
 *
 * Parameters
 * elem_name:	A string matching the elem name to be retrieved.
 * cfg_file:	Optional parameter - reference to a config already read in by CLASS_GetSettingsCfgFile()
 *
 * Return value
 * A config file elem reference.
 *
 */
function CLASS_GetSettingsCfgElem(elem_name, byref cfg_file:=0)
	if ( !cfg_file )
		cfg_file := CLASS_GetSettingsCfgFile();
	endif

	var elem := cfg_file[elem_name];

	if ( elem.errortext )
		SysLog("Error::CLASS_GetSettingsCfgElem() - Unable to find elem ["+elem_name+"] ->"+elem.errortext);
	endif

	return elem;
endfunction
