use uo;
use cfgfile;

function GetTime()

    local cfg := ReadConfigFile( "::days" );
    local elem := FindConfigElem( cfg , "WholeDay" );
    local length := GetConfigInt( elem , "Length" );
    local day_length := (60 * length); 

//Print("DayLength: " + day_length);

    local x := readgameclock();

//Print("Read Game Clock: " + x);

    local bigpart := x / day_length;

//Print("Bigpart: " + bigpart);

    local smallpart := bigpart * day_length;
    x := (x - smallpart);

//Print("Time of Day: " + x);
//Print("The Day is " + day_length/60 + " minutes long.");

return ( x );

endfunction

function GetTransition()

    local cfg := ReadConfigFile( "::days" );
    local elem := FindConfigElem( cfg , "Transition" );
    local length := GetConfigInt( elem , "Length" );

    return length;
endfunction

function GetDayLength()

    local cfg := ReadConfigFile( "::days" );
    local elem := FindConfigElem( cfg , "WholeDay" );
    local length := GetConfigInt( elem , "Length" );

    return length;
endfunction

function GetTimeofDay()

    local time_sec := GetTime();
    local length := (GetDayLength() * 60) * 1.0 ;

    local temp := length / time_sec;
    local raw_time := 24.0 / temp;

    local hours := cint(raw_time);
    local remainder := raw_time - hours;
    local minutes := cint( remainder * 60 );

    if( len(cstr(minutes)) = 1 )
	minutes := "0" + cstr(minutes);
    endif

    if( len(cstr(hours)) = 1 )
	hours := "0" + cstr(hours);
    endif

    return (hours+":"+minutes);

endfunction

