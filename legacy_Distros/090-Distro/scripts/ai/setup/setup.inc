include "include/vetement";
global speech;
global ammotype;
global ammoamount;
global theammo;

global master;
global spells := {};
global me := self();

global cast_pct;
global num_casts;
global count_casts ;
global saywords := 1;
global summons := 3;

global graphics;
global num_changes;
global will_cast;
global will_breathe;
global flee_point;

	drop_anchor();

var npccfg := readconfigfile("npcdesc");
var speechelem := npccfg[me.npctemplate];
speech := getconfigstring(speechelem, "speech") ;
spells := GetConfigStringArray( speechelem, "spell" );
cast_pct := speechelem.cast_pct;
num_casts  := speechelem.num_casts;
count_casts  := speechelem.count_casts;
var equipname := speechelem.equip;
saywords := speechelem.saywords;
var equiptm := npccfg[me.npctemplate].equip;
if (!cast_pct)
        cast_pct := 10;
endif
flee_point := speechelem.flee_point;
if (!flee_point)
        flee_point := 10;
endif
global mybow := npccfg[me.npctemplate].missileweapon;
if (!me.backpack)
	local newbackpack := CreateItemAtLocation(me.x, me.y, me.z, 0xe75,1);
	EquipItem(me, newbackpack);
	if (npccfg[me.npctemplate].colorrange)
		var parms := splitwords(npccfg[me.npctemplate].colorrange);
		me.color := randomint(cint(parms[2]) - cint(parms[1]))+cint(parms[1]);
	endif
	if (npccfg[me.npctemplate].dress)
		var parms := {};
		parms[1]:= me;
		parms[2]:= npccfg[me.npctemplate].dress;
		start_script("::/misc/dressme", parms);
	elseif (equiptm)
    		set_priority(50);
       		Habille( me , equiptm );
       		set_priority(1);
	elseif (equipname)
        	equipfromtemplate(me, equipname);
	elseif (npccfg[me.npctemplate].ammotype)
		ammotype := npccfg[me.npctemplate].ammotype;
		if (ammotype)
        		if (me.backpack)
                		destroyitem(me.backpack);
        		endif
        		equipfromtemplate(me, mybow);
        		ammoamount := npccfg[me.npctemplate].ammoamount;
			if (!ammoamount)
				ammoamount := 30;
			endif
			theammo := createiteminbackpack(me, ammotype, ammoamount);
		endif
	endif
endif
if( me.name["<random>"] )
	if ((me.graphic = 400) or (me.graphic = 401))
		me.graphic := 400 + RandomInt(2);
		if(me.graphic = 401)
			me.gender := 1;
		endif
	endif
	if (npccfg[me.npctemplate].title)
		local title := randomint(20);
		local ttitle := "";

		case (title) 
          	0: ttitle := "";
          	1: ttitle := " the townsperson";
          	2: ttitle := " the farmer";
          	3: ttitle := " the sailor";
          	4: ttitle := " the drunkard";
          	5: ttitle := " the wanderer";
          	6: ttitle := " the villager";
          	7: ttitle := " the gypsie";
          	8: ttitle := " the vagabond";
          	9: ttitle := " the laborer";
        	  default: ttitle := "";
		endcase

		local tname := randomname(me);
		setname(me, tname + ttitle);
	else
		SetName ( me , RandomName( me ) );
	endif
endif
