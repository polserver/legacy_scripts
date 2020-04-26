/////////////////////////////////////////////////////////////////////
//
//  lookForTrouble_AI
//
//  This file contains a set of lookForTrouble() functions
//  for creature AI.
//
//  Some are "fast", performing a strict set of functions
//  with minimal logic.  Others are "comprehensive", switching
//  functionality based on variables or string properties.
//
//  This library should be included after all other includes.
//  Functions might assume quite a lot.
//
//  Each function in this family returns the number of mobiles found.
//
/////////////////////////////////////////////////////////////////////////////

Const SHORT_RANGE    := 3;
Const DEFAULT_RANGE  := 5;
Const PARANOID_RANGE := 7;
Const HUNTER_RANGE   := 9;

//  lookForTroubleAll
//
//  This is a comprehensive function, testing for all possible behaviors.
//
function lft_lookForTroubleAll()

   local nearby := ListMobilesInLineOfSight(me, DEFAULT_RANGE);
   local crowd  := len( nearby );

   foreach dude in nearby
   
      if ( bu_setLeader( dude ) = 0 )
         bu_setBuddy( dude );
      endif

      local annoyed := Hunger + aggression/4;

      if (     nature = "meek"     )
         if ( cr_justLikeMe( dude ) = 0 )
            RunAwayFrom( dude );
            return crowd;
         endif
      elseif ( nature = "prey"     )
         if ( GetObjProperty( dude, "BadGuy" ) = me.serial )
            co_target( dude );
            return crowd;
         endif
      elseif ( nature = "predator" )
         if ( annoyed > Cint( GetObjProperty( dude, "aggression" ) ) // hunting
         or ( GetObjProperty( dude, "BadGuy" ) = me.serial ) )       // hunted
            co_target( dude );
            return crowd;
         endif
      elseif ( nature = "killer"   )
         if ( annoyed * ( 1 + co_human(dude) ) > Cint( GetObjProperty( dude, "aggression" ) )
         or ( GetObjProperty( dude, "BadGuy" ) = me.serial ) )
            co_target( dude );
            return crowd;
         endif
      elseif ( nature = "undead"   )
         if ( ! (GetObjProperty( dude, "alignment" ) = "evil") )
            co_target( dude );
            return crowd;
         endif
      endif

   endforeach

endfunction

//  lookForTrouble:
//
//  For each mobile within IN_RANGE units of me which I can see:
//     attack if I'm desparate enough.
//
//  This is the standard function used by default predators and prey.
//
function lft_lookForTrouble()

   local nearby := ListMobilesInLineOfSight(me, DEFAULT_RANGE);
   local crowd  := len( nearby );

   foreach dude in nearby
      if ( Hunger + Cint( GetObjProperty( me,   "aggression" ) )/4
                  > Cint( GetObjProperty( dude, "aggression" ) ) // hunting
         or ( GetObjProperty( dude, "BadGuy" ) = me.serial ) )   // hunted
         co_target( dude );
         return crowd;
      endif

   endforeach

   return crowd;

endfunction

//  lookForTroubleAndFriends
//
//  This function is like lookForTrouble(), with some leader/buddy
//  logic added.
function lft_lookForTroubleAndFriends()

   local nearby := ListMobilesInLineOfSight(me, DEFAULT_RANGE);
   local crowd  := len( nearby );

   foreach dude in nearby
      if ( ! cr_justLikeMe( dude ) )
         if ( Hunger + Cint( GetObjProperty( me,   "aggression" ) )/4
                     > Cint( GetObjProperty( dude, "aggression" ) ) // hunting
              or ( GetObjProperty( dude, "BadGuy" ) = me.serial ) ) // hunted
            co_target( dude );
            return crowd;
         endif
      elseif ( bu_setLeader( dude ) or bu_setBuddy( dude ) )
         return crowd;
      endif

   endforeach

   return crowd;

endfunction


//  meekLookForTrouble
//
//  If there are any dudes nearby that are of a different kind,
//  then flee!
//
function lft_meekLookForTrouble()

   local nearby := ListMobilesInLineOfSight( me, PARANOID_RANGE );
   local crowd  := len( nearby );

   foreach dude in nearby
      if ( ! cr_justLikeMe( dude ) )
         RunAwayFrom( dude );
         return crowd;
      endif
   endforeach

   return crowd;

endfunction

//  meekLookForTroubleAndFriends
//
//  Like meekLookForTrouble, with leader/buddy logic added.
//
function lft_meekLookForTroubleAndFriends()

   local nearby := ListMobilesInLineOfSight( me, PARANOID_RANGE );
   local crowd  := len( nearby );

   foreach dude in nearby
      if ( ! cr_justLikeMe( dude ) )
         RunAwayFrom( dude );
         return crowd;
      elseif ( bu_setLeader( dude ) or bu_setBuddy( dude ) )
         return crowd;
      endif
   endforeach

   return crowd;

endfunction

// killerLookForTrouble:
//
// If you find someone of a different type, then attack.
//
function lft_killerLookForTrouble()
 
   local nearby  := ListMobilesInLineOfSight( me, HUNTER_RANGE );
   local crowd   := len( nearby );
   local annoyed := Hunger + aggression;

   foreach dude in nearby
      if ( annoyed * ( 1 + co_human( dude ) ) > Cint( GetObjProperty( dude, "aggression" ) )
      or ( GetObjProperty( dude, "BadGuy" ) = me.serial ) )
         co_target( dude );
         return crowd;
      endif
   endforeach

   return crowd;

endfunction

// killerLookForTroubleAndFriends
//
// As per killerLookForTrouble, with leader/buddy logic.
//
function lft_killerLookForTroubleAndFriends()
 
   local nearby  := ListMobilesInLineOfSight( me, HUNTER_RANGE );
   local crowd   := len( nearby );
   local annoyed := Hunger + aggression;

   foreach dude in nearby
      if ( annoyed * ( 1 + co_human( dude ) ) > Cint( GetObjProperty( dude, "aggression" ) )
           or ( GetObjProperty( dude, "BadGuy" ) = me.serial ) )
         co_target( dude );
         return crowd;
      elseif ( cr_justLikeMe( dude ) 
               and ( bu_setLeader( dude ) or bu_setBuddy( dude ) ) )
         return crowd;
      else
      endif
   endforeach

   return crowd;

endfunction


// undeadLookForTrouble
//
// If you find someone non-evil, then attack.
//
function lft_undeadLookForTrouble()

   local nearby := ListMobilesInLineOfSight( me, DEFAULT_RANGE );
   local crowd  := len( nearby );

   foreach dude in nearby
      local dude := nearby[i];
      if (!GetObjProperty( dude, "alignment" ) = "evil")
         co_target( dude );
         return crowd;
      endif
   endforeach

   return crowd;

endfunction

// undeadLookForTroubleAndFriends
//
// As lft_undeadLookForTrouble, with leader/buddy logic added.
//
function lft_undeadLookForTroubleAndFriends()

   local nearby := ListMobilesInLineOfSight( me, DEFAULT_RANGE );
   local crowd  := len( nearby );

   foreach dude in nearby
      if ( ! GetObjProperty( dude, "alignment" ) = "evil")
         co_target( dude );
         return crowd;
      elseif ( bu_setLeader( dude ) = 0 )
         bu_setBuddy( dude );
         return crowd;
      endif
   endforeach

   return crowd;


endfunction