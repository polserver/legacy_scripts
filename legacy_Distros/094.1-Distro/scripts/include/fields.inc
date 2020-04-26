function get_direction(character, x, y, z)
   var ns_desc := "";
   var ew_desc := "";
   var thedesc;
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
   if (ns_desc == "" && ew_desc == "")
   thedesc := "nearby";
   else
   thedesc := ns_desc + ew_desc;
   endif
   print("the desc is " + thedesc);
   return thedesc;
endfunction

function CreateNorthSouthField(caster, fieldtype,x,y,z, length := 5)
  var counter := 0;
  var i, lst;
  var fires := { };
  var tester := CreateItemAtLocation(x,y,z, 0xeed, 1);
  for ( i := x - 2; i <= x - 2 + length; i := i + 1)
    counter := counter  + 1;
    if(fieldtype == 0x80)
      fires[counter] := CreateItemAtLocation( i, y, z, fieldtype, 1);
      SetObjProperty(fires[counter], "f","1");
      SetObjProperty(fires[counter], "creater", caster.serial);
      lst := ListMobilesNearLocation(i, y, z, 0);
      if(len(lst))
	    DestroyItem(fires[counter]);
	  endif
    else
      fires[counter] := CreateItemAtLocation( i,y,z,fieldtype, 1 );
      SetObjProperty(fires[counter], "creater", caster.serial);
      SetObjProperty(fires[counter], "f","1");
    endif
  endfor
  DestroyItem(tester);
  return fires;
endfunction

function CreateEastWestField(caster, fieldtype,x,y,z, length)
  var counter := 0;
  var i, lst;
  var fires := { };
  var tester := CreateItemAtLocation(x,y,z, 0xeed, 1);
  for ( i := y - 2; i <= y - 2 + length; i := i + 1)
    counter := counter  + 1;
    if(fieldtype == 0x80)
      fires[counter] := CreateItemAtLocation( x,i,z,fieldtype, 1 );
      SetObjProperty(fires[counter], "f","1");
      SetObjProperty(fires[counter], "creater", caster.serial);
      lst := ListMobilesNearLocation(x, i, z, 0);
      if(len(lst))
        DestroyItem(fires[counter]);
      endif
    else
      fires[counter] := CreateItemAtLocation( x,i,z,fieldtype, 1 );
      SetObjProperty(fires[counter], "f","1");
      SetObjProperty(fires[counter], "creater", caster.serial);
    endif
  endfor
  DestroyItem(tester);
  return fires;
endfunction