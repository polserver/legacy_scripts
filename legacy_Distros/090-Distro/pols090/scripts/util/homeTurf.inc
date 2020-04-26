///////////////////////////////////////////////////////////
//
//   homeTurf
//
//   Author: rje
//
//   Date: 1/99
//
//   This small include file contains data and logic for
//   creating a bounding box for some mobile.
//
///////////////////////////////////////////////////////////
global homeTurf := 0; // for ht_setTurf()

/////////////////////////////////////////////////////////
//
//  setHomeTurf()
//
//  This sets a boundingbox in which the creature 'stakes
//  out' as its own.
//
/////////////////////////////////////////////////////////
function ht_setHomeTurf()

   if ( GetObjProperty( me, "bounded" ) )

      local x1, x2, y1, y2;
      local myString;
  
      x1 := me.x - 10;
      y1 := me.y - 10;
      x2 := x1 + (randomint(50) * 4) + 1;
      y2 := y1 + (randomint(50) * 4) + 1;
        
      myString := cstr(x1) + " " + cstr(y1) + " " + cstr(x2) + " " + cstr(y2);
      homeTurf := MakeBoundingBox( myString );

   endif

endfunction
