use cfgfile;
include "include/attributes";

var mrcspawn_cfg:=ReadConfigFile("mrcspawn");

function RestockInventory(merchanttype, inventory)
  foreach productgroup in ProductGroups(merchanttype)
    Restock(inventory, productgroup);
  endforeach
endfunction

function find_or_create_merc(name, sn, merchanttype)
  var item;
  if(sn)
    item := SystemFindObjectBySerial(Cint(sn));
  endif
  if(item)
    RestockInventory(merchanttype, item);
    return item;
  else
    item := CreateItemAtLocation(5750, 1200, 0, 0x966a, 1);
    item.movable := 0;
    SetObjProperty(me,"restockserial", item.serial);
    item.name := name;
    RestockInventory(merchanttype, item);
    return item;
  endif
endfunction

function RestockMyInventory(source, destination)
  check_depletion(source, destination);
  foreach item in EnumerateItemsInContainer(source)
    SpawnIt(destination, item, item.amount, item.sellprice, item.buyprice);
    sleepms(200);
  endforeach
endfunction

function check_depletion(source, destination)
  var check := 0;
  var multiplier;
  var amt;
  foreach thing in EnumerateItemsInContainer(source)
    check := 0;
    amt := 0;
    foreach thingie in EnumerateItemsInContainer(destination)
      if(thingie.objtype == thing.objtype)
        amt := amt + thingie.amount;
      endif
    endforeach
    sleepms(10);
    if(amt > thing.amount)
      foreach thingie in EnumerateItemsInContainer(destination)
        if(thingie.objtype == thing.objtype)
          DestroyItem(thingie);
        endif
      endforeach
      CreateItemInContainer(destination, thing.objtype, 1);
      amt := 1;
      sleepms(1);
    endif
    if(amt = 0)
      multiplier := CInt(GetObjProperty(thing, "Multiplier"));
      if(multiplier <= 20)
        multiplier := multiplier + 1;
        SetObjProperty(thing, "Multiplier", multiplier);
      endif
    elseif(amt < thing.amount)
      SetObjProperty(thing, "Multiplier", 1);
    endif
    sleepms(5);
  endforeach
endfunction

function SpawnIt(destination, item, count, sellprice, buyprice)
  var existing := CountExisting(destination, item.objtype);
  var mygold := CInt(GetObjProperty(me, "MyGold"));
  if(!mygold)
    mygold := 5000;
  endif
  if(existing > count)
    foreach thing in EnumerateItemsInContainer(destination)
      if(thing.objtype = item.objtype)
        DestroyItem(thing);
      endif
    endforeach
    existing := 0;
  endif
  count := count - existing;
  if(existing == 0)
    var multiplier := CInt(GetObjProperty(item, "Multiplier"));
    if(multiplier == 0)
      multiplier := 1;
    endif
    count := (CInt(count) * multiplier);
  endif
  if(count > 0)
    var i := 0;
    for(i:=1; i<=count; i:=i+1)
      if(mygold > buyprice)
        var res := CreateItemInInventory(destination, item.objtype, 1);
        res.sellprice := sellprice;
        mygold := mygold - buyprice;
      endif
      sleepms(2);
    endfor
  endif
  SetObjProperty(me, "MyGold", CInt(mygold));
endfunction

function ProductGroups(mtype)
  var mtype_elem := FindConfigElem(mrcspawn_cfg, mtype);
  if(!mtype_elem)
    syslog("Unable to find config elem " + mtype + " for npc: " + me.serial);
    return 0;
  endif
  return GetConfigStringArray(mtype_elem, "Sells");
endfunction

function Restock(inventory, productgroup)
  var pg_elem := FindConfigElem(mrcspawn_cfg, productgroup);
  var checker := 0;
  foreach itemstr in GetConfigStringArray(pg_elem, "Item")
    SpawnItem(inventory, itemstr);
  endforeach
endfunction

function SpawnItem(inventory, itemstr)
  var split := SplitWords(itemstr);
  var objtype := split[1];
  var amt := Cint(split[2]);
  if(Cint(objtype))
    objtype := Cint(objtype);
  else
    objtype := Cint(GetObjtypeByName(objtype));
  endif
  if(!objtype)
    syslog("could not determine objtype for itemstr: " + itemstr);
    return;
  endif
  var existing := CountExisting(inventory, objtype);
  if(existing > amt)
    foreach item in EnumerateItemsInContainer(inventory)
      if(item.objtype == objtype)
        DestroyItem(item);
      endif
      sleepms(2);
    endforeach
    CreateItemInContainer(inventory, objtype, 1);
    existing := 1;
  endif
  var count := amt - existing;
  if(count > 0)
    var i;
    for( i := 1; i <= count; i := i + 1 )
      var res := CreateItemInInventory(inventory, objtype, 1);
    endfor
  endif
endfunction

function CountExisting(container, objtype)
  var count := 0;
  foreach item in EnumerateItemsInContainer(container)
    if(item.objtype == objtype)
      count := count + item.amount;
    endif
    sleepms(2);
  endforeach
  return count;
endfunction

function Merchants(merchantgroup)
  var result := array;
  var mrc := array;
  mrc.+name;
  mrc.+type;
  var group_elem := FindConfigElem(mrcspawn_cfg, merchantgroup);
  var count := GetConfigInt(group_elem, "MerchantCount" );
  var i;
  for( i := 1; i <= count; i := i + 1 )
    mrc.name := GetConfigString(group_elem, "Merchant" + i);
    mrc.type := GetConfigString(group_elem, "MerchantType"+i);
    result[i] := mrc;
  endfor
  return result;
endfunction

function ModifyPCSellList(merchanttype, backpack)
  var backpackitems:=ListRootItemsInContainer(backpack);
  var mrcspawnfile:=ReadConfigFile("mrcspawn");
  var prodgroups := FindConfigElem(mrcspawnfile, merchanttype);
  var buys:=GetConfigStringArray(prodgroups, "Buys");
  var nobuys:=GetConfigStringArray(prodgroups, "NoBuys");
  var itemconfig := ReadConfigFile(":*:itemdesc");
  var iteminit:=0;
  var itemmark, normcost, normsell, percentage;
  if(buys.size() >= 1)
    iteminit:= -1;
    itemmark:=0;
    prodgroups:=buys;
  elseif(nobuys.size() >= 1)
    iteminit:=0;
    itemmark:=-1;
    prodgroups:=nobuys;
  endif
  foreach item in backpackitems
    item.buyprice:=iteminit;
  endforeach
  var buynobuyable:={ };
  var objtype;
  var counter := 0;
  if(prodgroups.size())
    foreach prodgroup in prodgroups
      foreach itemstr in GetConfigStringArray(FindConfigElem(mrcspawnfile, prodgroup), "Item")
        itemstr := SplitWords(itemstr);
        objtype:=itemstr[1];
        if(CInt(objtype))
          objtype:=CInt(objtype);
        else
          objtype:=GetObjtypeByName(objtype);
        endif
        buynobuyable.append(objtype);
      endforeach
    endforeach
    if(buynobuyable.size())
      foreach item in backpackitems
        if((item.objtype in buynobuyable) && (!item.newbie))
          counter := counter + 1;
          if(itemmark == 0)
            if(!item.quality)
              item.quality := 1;
            endif
            normcost := itemconfig[item.objtype].VendorBuysFor;
            normsell := itemconfig[item.objtype].VendorSellsFor;
	        normcost := normcost * item.quality;
	        normsell := normsell * item.quality;
            percentage := ((item.hp * 10) / item.maxhp);
            if(percentage)
              normcost := CInt((normcost * percentage) / 10);
              normsell := CInt((normsell * percentage) / 10);
              item.buyprice := normcost;
              item.sellprice := normsell;
            else
              item.buyprice := itemmark;
            endif
          else
            item.buyprice := itemmark;
          endif
        endif
      endforeach
    endif
  endif
  return counter;
endfunction