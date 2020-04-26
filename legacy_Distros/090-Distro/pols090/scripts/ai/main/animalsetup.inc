global me := self();

global flee_point;

drop_anchor();

var npccfg := readconfigfile("npcdesc");
var speechelem := npccfg[me.npctemplate];
flee_point := speechelem.flee_point;
if (!flee_point)
        flee_point := 10;
endif

if (!me.backpack)
	local newbackpack := CreateItemAtLocation(me.x, me.y, me.z, 0xe75,1);
	EquipItem(me, newbackpack);
	if (npccfg[me.npctemplate].colorrange)
		var parms := splitwords(npccfg[me.npctemplate].colorrange);
		me.color := randomint(cint(parms[2]) - cint(parms[1]))+cint(parms[1]);
	endif
endif

if( me.name["<random>"] )
	SetName ( me , RandomName( me ) );
endif
