///////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////
//  MOONGATES.INC - Places moongates into the world.
//
//  This file is to be included into start.src and the function
//  CreateTeleporters() called from there.
//
//  Author: Ignatius 1999/05/27
//
//  Notes: System moongates are not saved, they are created upon
//  each system startup.
//
////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////

use uo;
use os;

// This should be a global variable since other scripts need it
// to cycle the moongates via the index variable.



function CreateMoongates();

    // since the language don't have truely global variables
    // (cross-script) we have to put this into the systemmoongate.src
    // aswell.
    var moongates := { 
	    {4467,	1283,	5},		// moonglow
    	{1336,	1997,	5},		// britain
    	{1499,	3771,	5},		// jehlom
	    {771,	753,	5},		// yew
    	{2701,	693,	5},		// minoc
    	{1828,	2948,	-20},	// trinsic
    	{643,	2067,	5},		// skara
    	{3564,	2140,	34},	// magincia
    };

	var moongate;
    var index := 1;

    syslog( "Creating moongates." );

	foreach gate in moongates
		moongate := CreateItemAtLocation( gate[1] , gate[2] , gate[3] , "systemmoongate" , 1 );

		// set the destination to nearly itself at the start
  	    SetObjProperty( moongate, "IndexSelf"   , index );
        if( index < 8 )
    	    SetObjProperty( moongate, "IndexCurrent", index + 1 );
		else
    	    SetObjProperty( moongate, "IndexCurrent", 1 );
        endif

		// set the properties needed.
      	moongate.facing := 1;
      	moongate.decayat := 0;
      	moongate.movable := 0;
 
        index := index + 1;        
   endforeach
endfunction

