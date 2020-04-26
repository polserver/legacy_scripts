//////////////////////////////////////////////////////////
//
//   creature.inc
//
//   Author: rje
//
//   Date: 12/31/98
//
//   This module handles general creature behavior.
//
//////////////////////////////////////////////////////////
use util;

global me           := npc::Self();

global nature       := GetObjProperty( me, "nature"       );

global alignment    := GetObjProperty( me, "alignment"    );
global pack_type    := GetObjProperty( me, "pack_type"    );
global pack_order   := GetObjProperty( me, "pack_order"   );

global dx           := 0; // for cr_wander()
global dy           := 0; // for cr_wander()

/////////////////////////////////////////////////////////
//
//  wander()
//
//  This function moves me.  Default movement is based on 
//  a vector pair (dx,dy), which is occasionally modified.  
//  Thus my movement, though purposeless, doesn't appear 
//  to be too abrupt.
//
//  OTOH, if you've defined a home turf for me to frolic
//  in, I'll move inside there instead.
//
/////////////////////////////////////////////////////////
function cr_wander( homeTurf := 0 )

   if ( homeTurf )
      move( homeTurf );
      return;
   endif

   if (randomint(100) > 90)
      dx := randomint(3) - 1;
   elseif ( randomint(100) > 90)
      dy := randomint(3) - 1;
   endif

   if ( dx > 0 )
      move( "E" );
   elseif ( dx < 0 )
      move( "W" );
   endif

   if ( dy > 0 )
      move( "S" );
   elseif ( dy < 0 )
      move( "N" );
   endif

endfunction

/////////////////////////////////////////////////////
//
//  flee()
//
//  Run from BadGuy.
//  flee() returns 0 until dude is 15+ units distant,
//  after which 1 is returned.
//
/////////////////////////////////////////////////////
function cr_flee( dude )

   local d    := Distance( me, dude );
   local gone := 0;

   if ( d < 8 )     RunAwayFrom( dude );
   elseif (d < 16 ) cr_wander();
   else             gone := 1;
   endif

   return gone;

endfunction

/////////////////////////////////////////////////////
//
//  justLikeMe()
//
//  Returns 1 if the object type is the same as
//  my object type.  Otherwise, it returns 0.
//
/////////////////////////////////////////////////////
function cr_justLikeMe( dude )

   if ( dude.objtype = me.objtype )
      return 1;
   else
      return 0;
   endif

endfunction

