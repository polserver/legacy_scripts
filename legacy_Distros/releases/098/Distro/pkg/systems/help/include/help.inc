/* $Id: help.inc 616 2006-07-06 16:35:37Z austinheilman $
 *
 */

include ":datafile:datafile";

function GetHelpDataFile()
	return DFOpenDataFile(":help:helpsys", DF_CREATE);
endfunction

function LoadHelpInfo(elem_name, prop_name)
	var data_file := GetHelpDataFile();
	var data_elem := DFFindElement(data_file, elem_name, DF_CREATE);
	return data_elem.GetProp(prop_name);
endfunction

function SaveHelpInfo(elem_name, prop_name, byref data)
	var data_file := GetHelpDataFile();
	var data_elem := DFFindElement(data_file, elem_name, DF_CREATE);
	return data_elem.SetProp(prop_name, data);
endfunction

function GetHelpFilePath(elem_name)
	var help_cfg := ReadConfigFile(":help:helptopics");
	var help_elem := help_cfg[elem_name];

	var file_name := help_elem.File;
	if ( !file_name )
		return error{"errortext":="No file path was found."};
	endif

	return ":help:helpFiles/"+file_name+".txt";
endfunction
