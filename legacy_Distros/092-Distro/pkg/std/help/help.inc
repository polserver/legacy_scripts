//////////////////////////////////////////////////////			//
//Help system include							//
//functions here							//
//Icanpage(thechar) //returns if the char can use the page system now	//
//staffcheck() //set the number of online gms and counselors		//
//pagecheck(who, type) //check if who have paged, return  page num or 0 //
//pagewarning(type, message) //send to all staff types the message 	//
//////////////////////////////////////////////////////////////////////////


//globals variables
global counpages := {};
global gmpages := {};
global OnlineCounselors;
global OnlineStaff;

counpages := GetGlobalProperty("counpages");
gmpages := GetGlobalProperty("gmpages");
OnlineCounselors := GetGlobalProperty("OnlineCounselors");
OnlineStaff := GetGlobalProperty("OnlineStaff");


function Icanpage(thechar)
foreach person in enumerateonlinecharacters()
if ((GetObjProperty(person,"canpage")) and (person.serial != thechar.serial))
	return 0;
endif
endforeach
setobjproperty(thechar, "canpage", 1);
return 1;
endfunction


function staffcheck()

local staff := 0;
local counselor := 0;
foreach char in EnumerateOnlineCharacters()
  if (char.cmdlevel > 2)
    staff := staff + 1;
  elseif (char.cmdlevel = 1)
    counselor := counselor + 1;
  endif
endforeach

counselor := cint(counselor);
staff := cint(staff);

SetGlobalProperty( "OnlineCounselors", cint(counselor) );
SetGlobalProperty( "OnlineStaff", cint(staff) );

endfunction

function pagecheck(serial, type)

local cserial := cint(serial);
local retval := 0;
local i;
//type if 1 for gm pages and 2 for counpages

case(cint(type))
1:	local maxi := gmpages.size(); 
	
	for(i := 0; i < maxi; i := i + 1)
		if (cserial = cint(gmpages[i]))
			retval := i;
			break;
		endif	
	endfor


2:	local maxi := counpages.size();
	for(i := 0; i < maxi; i := i + 1)
		if (cserial = cint(counpages[i]))
			retval := i;
			break;
		endif	
	endfor

endcase

return retval;

endfunction


//1 gm 2 counselor
function pagewarning(type, thepage, who)

case(type)

1:	foreach char in EnumerateOnlineCharacters()
		if (char.cmdlevel > 2)
			SendSysmessage( char, "GM Page from " + who.name + "["+ readgameclock() +"]: " + thepage);
		endif
	endforeach
    break;

2:	foreach char in EnumerateOnlineCharacters()
		if (char.cmdlevel > 0)
			SendSysmessage( char, "Coun Page from " + who.name + "["+ readgameclock() +"]: " + thepage);
		endif
	endforeach
	break;
endcase

endfunction

function CheckQueueExistance()
  if(gmpages = error)
    gmpages := {};
	SetGlobalProperty("gmpages", gmpages );
  endif
  if(counpages = error)
	counpages := {};
	SetGlobalProperty("counpages", counpages );	
  endif
endfunction