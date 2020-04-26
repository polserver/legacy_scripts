use cfgfile;
include "include/attributes";

var mrcspawn_cfg := ReadConfigFile("::mrcspawn");
var itemconfig := ReadConfigFile(":*:itemdesc");

function find_or_create_merc(name, sn, merchanttype)
  var item;
  if(sn)
    item := SystemFindObjectBySerial(Cint(sn));
  endif
  if(item)
    return item;
  else
    item := CreateItemAtLocation(5750, 1200, 0, 0x966a, 1);
    item.movable := 0;
    SetObjProperty(me,"restockserial", item.serial);
    item.name := name;
    StockInventory(merchanttype, item);
    return item;
  endif
endfunction

function StockInventory(merchanttype, inventory)
  foreach productgroup in ProductGroups(merchanttype)
    Restock(inventory, productgroup);
  endforeach
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
  var buyprice  := itemconfig[objtype].VendorBuysFor;
  var sellprice := itemconfig[objtype].VendorSellsFor;
  if(Cint(objtype))
    objtype := Cint(objtype);
  else
    objtype := Cint(GetObjtypeByName(objtype));
  endif
  if(!objtype)
    syslog("could not determine objtype for itemstr: " + itemstr);
    return;
  endif
  var res;
  if(amt > 0)
    var i;
    for( i := 1; i <= amt; i := i + 1 )
      res := CreateItemInInventory(inventory, objtype, 1);
      res.buyprice  := buyprice;
      res.sellprice := sellprice;
    endfor
  endif
endfunction

function checkstock(me, pak)
  var mygold := GetObjProperty(me, "MyGold");
  if(!mygold)
    mygold := 2000;
  endif
  foreach thing in EnumerateItemsInContainer(pak)
    var crs := Cint(GetObjProperty(thing, "ClearRestock"));
    var cst;
    if(crs)
      if(ReadGameClock() > CInt(crs))
        cst := CInt(itemconfig[thing.objtype].VendorBuysFor) + 5;
        if(!cst)
          cst := 1;
        endif
        mygold := CInt(mygold + (cst * thing.amount));
        DestroyItem(thing);
      endif
    endif
  endforeach
  SetObjProperty(me, "MyGold", mygold);
endfunction

function RestockMyInventory(source, destination)
  var multiplier, amt, mlt, things, mygold;
  var newarray := {};
  foreach thing in EnumerateItemsInContainer(source)
    newarray.append(thing.objtype);
    if(!me)
      break;
    endif
    amt := 0;
    mygold := GetObjProperty(me, "MyGold");
    multiplier := CInt(GetObjProperty(thing, "Multiplier"));
    var chk := CountExisting(destination, thing.objtype);
    things := chk[2];
    amt := chk[1];
    if(amt >= (thing.amount * multiplier))
      multiplier := multiplier - 1;
      if(multiplier < 1)
        multiplier := 1;
      endif
    elseif(amt = 0)
      multiplier := multiplier + 1;
      if(multiplier > 20)
        multiplier := 20;
      endif
    endif
    SetObjProperty(thing, "Multiplier", multiplier);
    mlt := Cint(thing.amount * multiplier);
    var buyprice := Cint(thing.buyprice);
    var sellprice := Cint(thing.sellprice);
    var res;
    if(amt != mlt)
      foreach widget in things
        mygold := mygold + Cint(widget.amount * buyprice);
        DestroyItem(widget);
      endforeach
      if(amt > mlt)
        mygold := mygold + (buyprice * (amt - mlt));
        res := CreateItemInInventory(destination, thing.objtype, mlt);
        if(res)
          res.sellprice := sellprice;
          res.buyprice := buyprice;
        endif
      elseif(amt < mlt)
        var i;
        for(i:=1; i<= mlt; i := i+1)
          if(mygold > buyprice)
            var res := CreateItemInInventory(destination, thing.objtype, 1);
            res.sellprice := sellprice;
            res.buyprice := buyprice;
            mygold := mygold - buyprice;
          endif
          sleepms(1);
        endfor
      endif
    endif
    SetObjProperty(me, "MyGold", mygold);
    sleepms(1);
  endforeach
  mygold := GetObjProperty(me, "MyGold");
  foreach thing in EnumerateItemsInContainer(destination)
    if((!(thing.objtype in newarray)) and (!GetObjProperty(thing, "ClearRestock")))
      mygold := mygold + Cint(thing.amount * thing.buyprice);
      DestroyItem(thing);
    endif
  endforeach
  SetObjProperty(me, "MyGold", mygold);
endfunction

function CountExisting(container, objtype)
  var amt := 0;
  var things := {};
  foreach thing in EnumerateItemsInContainer(container)
    if((thing.objtype == objtype) and (!GetObjProperty(thing, "ClearRestock")))
      things.append(thing);
      amt := amt + thing.amount;
    endif
  endforeach
  var ary := {};
  ary.append(amt);
  ary.append(things);
  return ary;
endfunction

function ModifyPCSellList(merchanttype, backpack)
  var backpackitems := ListRootItemsInContainer(backpack);
  var prodgroups    := FindConfigElem(mrcspawn_cfg, merchanttype);
  var buys          := GetConfigStringArray(prodgroups, "Buys");
  var nobuys        := GetConfigStringArray(prodgroups, "NoBuys");
  var iteminit := 0;
  var itemmark, normcost, normsell, percentage;
  if(buys.size() >= 1)
    iteminit := -1;
    itemmark := 0;
    prodgroups := buys;
  elseif(nobuys.size() >= 1)
    iteminit := 0;
    itemmark := -1;
    prodgroups := nobuys;
  endif
  foreach item in backpackitems
    item.buyprice := iteminit;
  endforeach
  var buynobuyable := { };
  var objtype;
  var counter := 0;
  if(prodgroups.size())
    foreach prodgroup in prodgroups
      foreach itemstr in GetConfigStringArray(FindConfigElem(mrcspawn_cfg, prodgroup), "Item")
        itemstr := SplitWords(itemstr);
        objtype := itemstr[1];
        if(CInt(objtype))
          objtype := CInt(objtype);
        else
          objtype := GetObjtypeByName(objtype);
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

/*
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
*/