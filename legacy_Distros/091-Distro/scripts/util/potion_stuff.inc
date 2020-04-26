/*use uo;*/

////////////////////////////////////////
//
//  We're going to build a 'type' here.
//
global FlaskContentsType array;
FlaskContentsType .+ reg;
FlaskContentsType .+ amount;
//
//  Now, when you want one of these in an array, or a variable, you just
//  write,
//  local variable := FlaskContentsType;
//
////////////////////////////////////////

function write_contents (thecontents, thecontainer)

    // the contents is an array of up to 3 structures, containing .reg and .amount members

    local content_string := "";
    // local counter := 0;
    // local tlen := len(thecontents);

    foreach item in thecontents
        if (item.amount > 0)
            if (content_string)
                content_string := content_string + ",";
            endif

            content_string := item.reg + ConvertNumber(item.amount);
        endif
    endforeach

    if (content_string > "")
        EraseObjProperty(thecontainer, "contents");
        EraseObjProperty(thecontainer, "heated");
    else
        SetObjProperty(thecontainer, "contents", content_string);
    endif

endfunction


function ConvertNumber(thenumber)
    if (thenumber < 10)
        return "0" + cstr(thenumber);
    else
        return cstr(thenumber);
    endif
endfunction

Function GetContents (container)

    // reads the properties of a container, and returns an array of structures. Each structure has .reg and .amount members 

    local contents := GetObjProperty(container,"contents");

    if (!contents)
            return 0;
    endif

    local thecontents array;

    local counter;
    local tlen := len(contents);

    if (tlen = 0)
            return -1;
    endif

    local num_regs := 0;
    local tempstr := "";

    for (counter := 1; counter <= tlen; counter := counter + 1)
        if (counter = 1)
                num_regs := num_regs + 1;
                thecontents[num_regs] := FlaskContentsType;
                thecontents[num_regs].reg := contents[counter];
        elseif (thecontents[counter - 1] = ",")
                num_regs := num_regs + 1;
                thecontents[num_regs] := FlaskContentsType;
                thecontents[num_regs].reg := contents[counter];
                thecontents[num_regs - 1].amount := cint(tempstr);
                tempstr := "";
        else
                tempstr := tempstr + thecontents[counter];
                if (counter = tlen)
                        thecontents[num_regs] := FlaskContentsType;
                        thecontents[num_regs].amount := cint(tempstr);
                endif
        endif
    endfor

return thecontents;

endfunction


function AddRegToFlask(flask, reg, amount)

    local contents := GetContents(flask);

    if (amount > 99)
            amount := 99;
    endif

    if (contents = -1)
            local stuff := FlaskContentsType;
            stuff[1].reg := reg;
            stuff[1].amount := amount;
            write_contents(stuff, flask);
    else

        local i;
        local tlen := len(contents);
        local inthere := 0;


        while (i <= tlen)
                i := i + 1;
                if (contents[i] = reg)
                        inthere := 1;
                        break;
                endif
        endwhile

        if (inthere)
                contents[i].amount := contents[i].amount + amount;
        else
                contents[tlen + 1].reg := reg;
                contents[tlen + 1].amount := amount;
        endif

        write_contents(contents, flask);

    endif
endfunction

function TakeRegsFromFlask(flask, reg, amount)

    local contents := GetContents(flask);

    if (contents = -1) //the flask is empty
            return -1;
    else

        local i;
        local tlen := len(contents);
        local inthere := 0;


        while (i <= tlen)
                i := i + 1;
                if (contents[i] = reg)
                        inthere := 1;
                        break;
                endif
        endwhile

        if (inthere)
                contents[i].amount := contents[i].amount - amount;
                if (contents[i].amount < 0)
                        return contents[i];
                endif
        else
                return -1;
        endif

        write_contents(contents, flask);

    endif
endfunction

/* added for testing
program potion_stuff()
local flask := CreateItemAtLocation(1,1,1,0xeed,1);

 AddRegToFlask(flask, "n", 1);
 takeregsfromflask(flask, "n",5);
 
endprogram
*/