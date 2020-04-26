function possess( me, npc )

	if ( getobjproperty(me,"possess") )
		local oldnpc := SystemFindObjectBySerial(getobjproperty(me,"possess"));
		if ( oldnpc )
			unpossess(me);
		endif
	endif

	if (npc.enable( "invul" ) )
		GrantPrivilege( me, "invul" );
		me.enable( "invul" );
	else
		RevokePrivilege( me, "invul" );
	endif

	me.concealed := 1;
	local oldpack := MoveMyPack(me);

	setobjproperty(me,"oldname",me.name);
	setobjproperty(me,"oldprefix",me.title_prefix);
	setobjproperty(me,"oldsuffix",me.title_suffix);

	clone(npc, me);
	EquipItem(npc, oldpack);
	setobjproperty(me,"possess",npc.serial);

endfunction

function MoveMyPack(me)

	if (!me.backpack)
		local newbackpack := CreateItemAtLocation(5123, 1779, 0, 0xe75,1);
		EquipItem(me, newbackpack);
	endif

	local mypack := me.backpack;
	foreach thing in listequippeditems(me)
		if ( thing.serial != mypack.serial )
			MoveItemToContainer(thing, mypack);
		endif
	endforeach

	MoveItemToLocation(mypack,5123, 1779, 0, MOVEITEM_FORCELOCATION);

	return mypack;

endfunction

function clone(npc, me)

	if (!npc.backpack)
		local newbackpack := CreateItemAtLocation(5123, 1779, 0, 0xe75,1);
		EquipItem(npc, newbackpack);
	endif

	if (npc.color = cint(0x4631))
		npc.color := 1002;
	endif

	me.graphic := npc.graphic;
	me.color := npc.color;
	me.name := npc.name;
	me.facing := npc.facing;
	me.hidden := npc.hidden;

	me.setstr( npc.strength );
	me.setint( npc.intelligence );
	me.setdex( npc.dexterity );
	me.hp := npc.hp;
	me.mana := npc.mana;
	me.stamina := npc.stamina;

	foreach it in listequippeditems(npc)
		equipitem(me,it);
	endforeach

	local i;
	for (i := 0; i <= SKILLID__HIGHEST; i := i + 1)
		SetRawSkill( me, i, GetRawSkill(npc,i) );
	endfor

	MoveCharacterToLocation(me,npc.x, npc.y, npc.z, MOVECHAR_FORCELOCATION);	
	MoveCharacterToLocation(npc,5276, 1164, 0, MOVECHAR_FORCELOCATION);
	me.concealed := 0;

	npc.hidden := 1;
	npc.frozen := 1;

endfunction

function unpossess( me )

	local npc;
	if ( getobjproperty(me,"possess") )
		npc := SystemFindObjectBySerial(getobjproperty(me,"possess"));
	endif
	if (!npc)
		eraseobjproperty(me,"possess");
		if ( getobjproperty(me,"oldname") )
			me.name := getobjproperty(me,"oldname");
		endif
		SendSysMessage(me, "NPC not found.");
		return;
	endif	


	me.concealed := 1;
	SwapClothes(me,npc);
	eraseobjproperty(me,"possess");
	if ( getobjproperty(me,"oldname") )
		me.name := getobjproperty(me,"oldname");
	endif

	me.name := getobjproperty(me,"oldname");
	me.title_prefix := getobjproperty(me,"oldprefix");
	me.title_suffix	:= getobjproperty(me,"oldsuffix");

	eraseobjproperty(me,"oldname");
	eraseobjproperty(me,"oldprefix");
	eraseobjproperty(me,"oldsuffix");

	npc.hidden := me.hidden;
	npc.facing := me.facing;
	npc.frozen := 0;
	npc.stamina := me.stamina;
	npc.hp := me.hp;
	npc.mana := me.mana;

	movecharactertolocation(npc,me.x, me.y, me.z,MOVECHAR_FORCELOCATION);


	local i;
	for (i := 0; i <= SKILLID__HIGHEST; i := i + 1)
		SetRawSkill( me, i, 1000000 );
	endfor

	me.setstr( 100 );
	me.setint( 100 );
	me.setdex( 100 );
	me.hp := me.maxhp;
	me.mana := me.maxmana;
	me.stamina := me.maxstamina;

	me.graphic := me.trueobjtype;
	me.color := me.truecolor;

	local mypack := me.backpack;
	foreach thing in enumerateitemsincontainer(mypack)
		EquipItem(me, thing);
	endforeach

	local weaponone := GetEquipmentByLayer( me,  LAYER_HAND1 );
	local weapontwo := GetEquipmentByLayer( me,  LAYER_HAND2 );
	if (weaponone)
		MoveItemToContainer(weaponone, mypack );
	endif
	if (weapontwo)
		MoveItemToContainer(weapontwo, mypack );
	endif

endfunction

function SwapClothes(me, npc)

	if (!me.backpack)
		local newbackpack := CreateItemAtLocation(5123, 1779, 0, 0xe75,1);
		EquipItem(me, newbackpack);
	endif

	local hispack := npc.backpack;
	local mypack := me.backpack;

	if (hispack)
		MoveItemToLocation(hispack, 5123, 1779, 0, MOVEITEM_FORCELOCATION);
	else
		hispack := CreateItemAtLocation(5123, 1779, 0, 0xe75,1);
	endif

	foreach thing in listequippeditems(me)
		EquipItem(npc, thing);
	endforeach

	EquipItem(me, hispack);

endfunction

function unpossesscorpse( me, corpse )

	local npc;
	if ( getobjproperty(me,"possess") )
		npc := SystemFindObjectBySerial(getobjproperty(me,"possess"));
	endif
	if (!npc)
		eraseobjproperty(me,"possess");
		if ( getobjproperty(me,"oldname") )
			me.name := getobjproperty(me,"oldname");
		endif
		return;
	endif	

	me.concealed := 1;

	Resurrect( me );
	me.graphic := me.trueobjtype;
	me.color := me.truecolor;

	SwapClothes(me,npc);

	eraseobjproperty(me,"possess");

	me.name := getobjproperty(me,"oldname");
	me.title_prefix := getobjproperty(me,"oldprefix");
	me.title_suffix	:= getobjproperty(me,"oldsuffix");

	eraseobjproperty(me,"oldname");
	eraseobjproperty(me,"oldprefix");
	eraseobjproperty(me,"oldsuffix");

	movecharactertolocation(npc, 5288 , 1176 , 0, 0x40000000);
	setobjproperty(npc, "guardkill", 1);
	RevokePrivilege( npc, "invul" );
	Damage( npc, npc.hp );

	local i;
	for (i := 0; i <= SKILLID__HIGHEST; i := i + 1)
		SetRawSkill( me, i, 1000000 );
	endfor

	me.setstr( 100 );
	me.setint( 100 );
	me.setdex( 100 );
	me.hp := me.maxhp;
	me.mana := me.maxmana;
	me.stamina := me.maxstamina;

	me.graphic := me.trueobjtype;
	me.color := me.truecolor;

	local mypack := me.backpack;
	foreach thing in enumerateitemsincontainer(mypack)
		EquipItem(me, thing);
	endforeach

	local weaponone := GetEquipmentByLayer( me,  LAYER_HAND1 );
	local weapontwo := GetEquipmentByLayer( me,  LAYER_HAND2 );
	if (weaponone)
		MoveItemToContainer(weaponone, mypack );
	endif
	if (weapontwo)
		MoveItemToContainer(weapontwo, mypack );
	endif

endfunction

