use uo;
use os;
use util;


Function AddDeath(rune)
  Local deathpool:=GetGlobalProperty("deathpool");
  DeathPool[Len(DeathPool)+1]:=rune;
  syslog("Creature Added to DeathPool");
  SetGlobalProperty("deathpool", DeathPool);  
endfunction


//adds the rune object the array should only be executed after a prop check is done
Function AddCreated(rune)
  Local createdpool:=GetGlobalProperty("createdpool");
  createdpool[Len(createdpool)+1]:=rune;
  syslog("Rune Added to CreatedPool");
  SetGlobalProperty("createdpool", createdpool);  
endfunction


//rune is the "spawnnet" cprop of the rune to the pool
Function AddDeleted(rune)
  Local deletedpool:=GetGlobalProperty("deletedpool");
  deletedpool[Len(deletedpool)+1]:=rune;
  syslog("Creature Added to DeletedPool");
  SetGlobalProperty("deletedpool", deletedpool);  
endfunction

//rune is the serial of the rune in question.
Function AddRefresh(rune)
  Local refreshpool:=GetGlobalProperty("refreshpool");
  refreshpool[Len(refreshpool)+1]:=rune;
  syslog("Creature Added to RefreshPool");
  SetGlobalProperty("refreshpool", refreshpool);  
endfunction


//transfer the global prop pool the the engine pool
Function CheckDeathpool(DeathPool)
  Local gdeathpool:=GetGlobalProperty("deathpool");
  syslog("Global DeathPool: "+len(gdeathpool));
  syslog("DeathPool: "+len(deathpool));
  Local i;
  Local ALen:=Len(DeathPool);
  For (i:=1;i<=Len(gdeathpool);i:=i+1);
    DeathPool[i+ALen]:=gdeathpool[i];
  endfor;
  gdeathpool:={};
  SetGlobalProperty("deathpool", gdeathpool);
  Return DeathPool;
endfunction;


//transfer the global prop pool the the engine pool
Function CheckCreatedPool(CreatedPool)
  local gcreatedpool:=GetGlobalProperty("createdpool");
  syslog("Global CreatedPool: "+len(gcreatedpool));
  syslog("CreatedPool: "+len(CreatedPool));
  Local i;
  Local ALen:=Len(CreatedPool);
  For (i:=1;i<=Len(gcreatedpool);i:=i+1);
    CreatedPool[i+ALen]:=gcreatedpool[i];
  endfor;
  gcreatedpool:={};
  SetGlobalProperty("createdpool", gcreatedpool);
  Return CreatedPool;
endfunction;

//transfer the global prop pool the the engine pool
Function CheckDeletedPool(deletedpool)
  Local gdeletedpool:=GetGlobalProperty("deletedpool");
  syslog("Global DeletedPool: "+len(gdeletedpool));
  syslog("DeletedPool: "+len(deletedpool));
  Local i;
  Local ALen:=Len(deletedpool);
  For (i:=1;i<=Len(gdeletedpool);i:=i+1);
    deletedpool[i+ALen]:=deletedpool[i];
  endfor;
  gdeletedpool:={};
  SetGlobalProperty("deletedpool", gdeletedpool);
  Return deletedpool;
endfunction;

//transfer the global prop pool the the engine pool
Function CheckRefreshpool(RefreshPool)
  Local grefreshpool:=GetGlobalProperty("refreshpool");
  syslog("Global refreshPool: "+len(grefreshpool));
  syslog("refreshPool: "+len(refreshpool));
  Local i;
  Local ALen:=Len(refreshPool);
  For (i:=1;i<=Len(grefreshpool);i:=i+1);
    refreshPool[i+ALen]:=grefreshpool[i];
  endfor;
  grefreshpool:={};
  SetGlobalProperty("refreshpool", grefreshpool);
  Return refreshPool;
endfunction;