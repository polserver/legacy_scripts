
function array2string(thearray)
        local retval;

        foreach word in thearray
                retval := retval + " " + thearray;
        endforeach

        retval := retval[2,1000000];

        return retval;
endfunction


function string2array(thestring)

        local retval := array;
        retval := splitwords(thestring);

endfunction


        