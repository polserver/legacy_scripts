
function get_direction (character, x, y, z)

	local ns_desc := "";
	local ew_desc := "";
	local thedesc;

	if (y < character.y)
        	ns_desc := "north";
	elseif  (y > character.y)
        	ns_desc := "south";
   	endif
 
 	if (x < character.x)
        	ew_desc := "west";
   	elseif  (x > character.x)
        	ew_desc := "east";
   	endif
    
	if (ns_desc = "" and ew_desc = "")
		thedesc := "nearby";
   	else 
   		thedesc := ns_desc + ew_desc ;
   	endif

	return thedesc;

endfunction

Function CreateNorthSouthField(caster, fieldtype,x,y,z, length := 5)

	local counter := 0;
	local i;
	local n;
	local fires := { };
	
	for ( i := x - 2; i <= x - 2 + length; i := i + 1)

		n := CreateItemAtLocation( i,y,z,fieldtype, 1 );
		if (n)
               		counter := counter  + 1;
               		fires[counter] := n;
               		setobjproperty(fires[counter], "creater", caster.serial);
               		SetObjProperty(fires[counter] , "f","1");
		endif

	endfor
	
	return fires;		

endfunction

Function CreateEastWestField(caster, fieldtype,x,y,z, length := 5)
			
	local counter := 0;
	local i;
	local n;
	local fires := { };

	for ( i := y - 2; i <= y - 2 + length; i := i + 1)

		n := CreateItemAtLocation( x,i,z,fieldtype, 1 );
		if (n)
               		counter := counter  + 1;
               		fires[counter] := n;
               		SetObjProperty(fires[counter] , "f","1");
               		setobjproperty(fires[counter], "creater", caster.serial);
		endif
	
	endfor
	
	return fires;	

endfunction	
