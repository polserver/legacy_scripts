use cfgfile;

function get_default_points(skillid)
        local cfg := readconfigfile("skills");

        local default_points := cfg[skillid].default_points;
        
        if (!default_points)
                return 0;
        else
                return default_points;

        endif

endfunction
