//ver 0.9.1a

use uo;
use os;
use util;

const CLEAR_RANGE :=100;



function spawnnet_clear(obj)
if(GetObjProperty(obj,"spawnnet"))
	AddDeath(cint(GetObjProperty(obj,"spawnnet")));
	EraseObjProperty(obj,"spawnnet");
	return 1;
endif

return 0;
endfunction

//adds the rune object the array should only be executed after a prop check is done
Function AddCreated(rune)
	set_critical(1);
	Local pool:=GetGlobalProperty("createpool");
	if(!pool)
		pool := {};
	endif
	if(rune in pool)
		return 0;
	endif
	pool.append(rune);

	SetGlobalProperty("createpool", pool );
	set_critical(0);
	return 1;

endfunction


//rune is the serial of the rune in question.
Function AddRefresh(rune)
	set_critical(1);
	Local pool:=GetGlobalProperty("refreshpool");
	if(rune in pool)
		return 0;
	endif
	if(!pool)
		pool :={};
	endif
	pool.append(rune);
	SetGlobalProperty("refreshpool", pool);
	set_critical(0);
	return 1;
endfunction


//rune is the "spawnnet" cprop of the rune to the pool
Function AddDeleted(rune)
	set_critical(1);
	Local pool:=GetGlobalProperty("deletedpool");
	if(!pool)
		pool :={};
	endif
	pool.append(rune);
	SetGlobalProperty("deletedpool", pool);

endfunction


Function AddDeath(rune)
	syslog("creature died, rune: "+rune);
	rune := cint(rune);
	set_critical(1);
	local pool:=GetGlobalProperty("deathpool");
	if(!pool)
		pool := {};
	endif
	pool.append(rune);
	SetGlobalProperty("deathpool", pool);
	set_critical(0);
endfunction



function ResetSpawn(item, flag)

local dist:=cint(GetObjProperty(item, "range")) + CLEAR_RANGE;
local spawnnet:=cint(GetObjProperty( item, "spawnnet"));
if(!spawnnet)
	return 0;
endif

if (flag = 1)
	AddDeleted(SpawnNet);
endif

local props :=cstr(hex(cint(GetObjProperty(item, "flags"))));
local type := props[3,1];
type := cint(type);
if(!type)
	return 0;
endif
local flags := props[4,1];
flags := "0x"+flags;
flags := cint(flags);
flags := GetFlags(flags);

local aux;

case(type)
	1: foreach npc in ListMobilesNearLocation( item.x, item.y, item.z, dist );
		Aux:=cint( GetObjProperty( npc, "spawnnet" ));
		if (Aux = SpawnNet)
			if (flag = 1)
				EraseObjProperty( npc, "spawnnet");
			endif
			RevokePrivilege( npc, "invul" );
			setobjproperty(npc, "guardkill", 1);
			Damage( npc, (npc.maxhp) );
		endif
	   endforeach

	2: local list;
	   if(flags[2])
	   	aux := item.container;
	   	while(aux.container.serial)
	   		aux := aux.container;
	   	endwhile
	   	if(!aux)
	   		SysLog("Item spawn with inside container flag is outside of any");
	   		return 0;
	   	endif

		list := EnumerateItemsInContainer(aux);
		local tmp := cint(GetObjProperty(aux, "spawnnet"));
		if(tmp = spawnnet)
			EraseObjProperty(aux, "spawnnet");
		endif
 	   else
		list := ListObjectsInBox( (item.x-dist), (item.y-dist), -127, (item.x+dist), (item.y+dist), 127 );
	   endif
	   foreach it in list
		if(it.amount)
			Aux:=cint( GetObjProperty( it, "spawnnet" ));
			if(Aux = SpawnNet and it.objtype != 0xa300)
				 destroyitem(it);
			endif
		endif
	   endforeach
	3: aux := cint(GetObjProperty(item, "aux"));
	   if(aux)
		local cnt := SystemFindObjectBySerial(aux);
		if(cnt)
			destroyitem(cnt);
		endif
	   endif
endcase

endfunction



function GetFlags(x)
x:=cint(x);

local result := {0,0,0,0};

if(x>7)
	x:=x-8;
	result[4]:=1;
endif

if(x>3)
	x:=x-4;
	result[3]:=1;
endif
if(x>1)
	x:=x-2;
	result[2]:=1;
endif

if(x>0)
	x:=x-1;
	result[1]:=1;
endif

return result;

endfunction


function is_item_spawn(rune)

local props :=cstr(hex(runes.flags[rune]));
local type := props[3,1];
type := cint(type);
if(type >1)
	return 1;
endif
return 0;
endfunction


function exp2( power) //optimized version from sigmund one's
local retval := 1,i;
for (i:=1; i<=power; i:=i+1)
	retval := retval *2;
endfor
return retval;
endfunction
