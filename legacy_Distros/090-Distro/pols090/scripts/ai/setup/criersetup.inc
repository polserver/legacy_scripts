include "include/vetement";

global me := self();
drop_anchor();
var npccfg := readconfigfile("npcdesc");
var speechelem := npccfg[me.npctemplate];
var speech := getconfigstring(speechelem, "speech") ;
var saywords := speechelem.saywords;
var equiptm;
var flee_point := speechelem.flee_point;
if (!flee_point)
        flee_point := 10;
endif

if (!me.backpack)
	local newbackpack := CreateItemAtLocation(me.x, me.y, me.z, 0xe75,1);
	EquipItem(me, newbackpack);
	if( me.name["<random>"] )
		me.graphic := 400 + RandomInt(2);
		SetName ( me , RandomName( me ) );
		equiptm := GetObjProperty( me, "equipt");

		if ( equiptm )
			set_priority(50);		
    			Habille( me , equiptm );
    		set_priority(1);
		endif

	endif
	local myhome := { };
	myhome[1] := me.x;
	myhome[2] := me.y;
	myhome[3] := me.z;
	SetObjProperty(me,"myhome",myhome);
endif
