///////////////////////////////////////////////
//
//   MRCSpawn Include v1.0 (2000/10/25)
//
//     based on MRCSpawn Include
//       Author: ???
//
//     Author: Shinigami
//     eMail : Shinigami@gmx.net
//
//   Modifications:
//     2000/10/25 Shinigami: Complete ReWrite
//                           SellLists added
//
///////////////////////////////////////////////
//
// ToDo: use stackability to create single items or add to stacks.
//       stacks are always added to, currently.
//       calling CreateItemInContainer 'count' times would do this
//       automatically, actually
//
// this would be nice, and fast, but this doesn't handle multiple stacks.
// in particular, it doesn't handle multiple single nonstackable items.
//   Local itemref:=FindObjtypeInContainer(inventory, objtype);
//   If (itemref)
//     count:=count-itemref.amount;
//   EndIf

////////////
// Library
////////////

Use cfgfile;

//////////////////////////////////////////////////
// RestockInventory - Restock Vendor's Inventory
//////////////////////////////////////////////////

Function RestockInventory(merchanttype, inventory)
  Local mrcspawnfile:=ReadConfigFile("mrcspawn");
  If (mrcspawnfile)
    Local productgroups:=FindConfigElem(mrcspawnfile, merchanttype);
    If (productgroups)
      Local sellitem;
      Local objtype;
      Local count;
      Local i;

      ForEach productgroup in GetConfigStringArray(productgroups, "Sells")
        ForEach itemstr in GetConfigStringArray(FindConfigElem(mrcspawnfile, productgroup), "Item")
          SleepMS(100);

          sellitem:=SplitWords(itemstr);
          objtype:=sellitem[1];
          If (CInt(objtype))
            objtype:=CInt(objtype);
          Else
            objtype:=GetObjtypeByName(objtype);
          EndIf

          count:=CInt(sellitem[2]);
          Foreach item in EnumerateItemsInContainer(inventory)
            If (item.objtype=objtype)
              count:=count-item.amount;
            EndIf
          EndForEach

          If (count>0)
            For (i:=1; i<=count; i:=i+1)
              If (!CreateItemInInventory(inventory, objtype, 1))
                SysLog("MRCSpawn: Das Item "+Hex(objtype)+" konnte nicht erzeugt werden!");
              EndIf
            EndFor
          EndIf
        EndForEach
      EndForEach
    Else
      SysLog("MRCSpawn: Kann keine Eintraege des MerchantTypes '"+merchanttype+"' finden!");
    EndIf

    UnloadConfigFile("mrcspawn");
  Else
    SysLog("MRCSpawn: Kann die Datei 'mrcspawn.cfg' nicht oeffnen !");
  EndIf
EndFunction

///////////////////////////////////////////////////////////////
// ModifyPCSellList - Mark sellable Items in Players Backpack
///////////////////////////////////////////////////////////////

Function ModifyPCSellList(merchanttype, backpack)
  Local backpackitems:=EnumerateItemsInContainer(backpack);

  Local mrcspawnfile:=ReadConfigFile("mrcspawn");
  If (mrcspawnfile)
    Local productgroups:=FindConfigElem(mrcspawnfile, merchanttype);
    If (productgroups)
      Local buys:=GetConfigStringArray(productgroups, "Buys");
      Local nobuys:=GetConfigStringArray(productgroups, "NoBuys");

      Local iteminit:=0; // Buys everything
      Local itemmark;
      Local productgroups:={ };

      If (buys.size()>=1) // Buys nothing, but defined Items
        iteminit:=-1;itemmark:=0;
        productgroups:=buys;
      ElseIf (nobuys.size()>=1) // Buys everything, but not defined Items
        iteminit:=0;itemmark:=-1;
        productgroups:=nobuys;
      EndIf

      Foreach item in backpackitems
        item.buyprice:=iteminit;
      EndForEach

      Local buynobuyable:={ };
      Local objtype;

      If (productgroups.size())
        ForEach productgroup in productgroups
          ForEach itemstr in GetConfigStringArray(FindConfigElem(mrcspawnfile, productgroup), "Item")
            objtype:=itemstr;
            If (CInt(objtype))
              objtype:=CInt(objtype);
            Else
              objtype:=GetObjtypeByName(objtype);
            EndIf

            buynobuyable.append(objtype);
          EndForEach
        EndForEach

        If (buynobuyable.size())
          Foreach item in backpackitems
            If (item.objtype in buynobuyable)
              item.buyprice:=itemmark;
            EndIf
          EndForEach
        EndIf
      EndIf
    Else // Buys nothing
      Foreach item in backpackitems
        item.buyprice:=-1;
      EndForEach

      SysLog("MRCSpawn: Kann keine Eintraege des MerchantTypes '"+merchanttype+"' finden!");
    EndIf

    UnloadConfigFile("mrcspawn");
  Else // Buys nothing
    Foreach item in backpackitems
      item.buyprice:=-1;
    EndForEach

    SysLog("MRCSpawn: Kann die Datei 'mrcspawn.cfg' nicht oeffnen !");
  EndIf
EndFunction

//////////////////
// Old Functions
//////////////////
//
//Global mrcspawn_cfg:=ReadConfigFile("mrcspawn");
//
//Function RestockInventory(merchanttype, inventory)
//  ForEach productgroup in ProductGroups(merchanttype)
//    Restock(inventory, productgroup);
//  EndForEach
//EndFunction
//
//Function ProductGroups(mtype)
//  Local mtype_elem:=FindConfigElem(mrcspawn_cfg, mtype);
//  If (mtype_elem)
//    Return (GetConfigStringArray(mtype_elem, "Sells");
//  Else
//    SysLog("MRCSpawn: Kann keine Eintraege des MerchantTypes '"+mtype+"' finden!");
//    Return (0);
//  EndIf
//EndFunction
//
//Function Restock(inventory, productgroup)
//  Local pg_elem:=FindConfigElem(mrcspawn_cfg, productgroup);
//
//  ForEach itemstr in GetConfigStringArray(pg_elem, "Item")
//    SleepMS(100);
//    SpawnItem(inventory, itemstr);
//  EndForEach
//EndFunction
//
//Function SpawnItem(inventory, itemstr)
//  Local ii:=SplitWords(itemstr);
//  Local objtype:=ii[1];
//  If (CInt(objtype))
//    objtype:=CInt(objtype);
//  EndIf
//
//  Local count:=CInt(ii[2])-CountExisting(inventory, objtype);
//  If (count>0)
//    Local i;
//    For (i:=1; i<=count; i:=i+1)
//      Local res:=CreateItemInInventory(inventory, objtype, 1);
//      If (!res)
//        SysLog("MRCSpawn: Das Item "+Hex(objtype)+" konnte nicht erzeugt werden!");
//      EndIf
//    EndFor
//  EndIf
//EndFunction
//
//Function CountExisting(container, objtype)
//  Local count:=0;
//
//  Foreach item in EnumerateItemsInContainer(container)
//    If (item.objtype=objtype)
//      count:=count+item.amount;
//    EndIf
//  EndForEach
//
//  Return (count);
//EndFunction
//
//Function MerchantGroups()
//  Local mrcgroups_elem:=FindConfigElem(mrcspawn_cfg, "MerchantGroups");
//  If (mrcgroups_elem)
//    Return (GetConfigStringArray(mrcgroups_elem, "MerchantGroup"));
//  Else
//    SysLog("MRCSpawn: Kann 'MerchantGroups' nicht finden!");
//    Return (0);
//  EndIf
//EndFunction
//
//Function Merchants(MerchantGroup)
//  Local result:={ };
//  Local mrc:={ };
//  mrc.+name;
//  mrc.+type;
//
//  Local group_elem:=FindConfigElem(mrcspawn_cfg, MerchantGroup);
//  Local count:=GetConfigInt(group_elem, "MerchantCount");
//  Local i;
//  For (i:=1; i<=count; i:=i+1)
//    mrc.name:=GetConfigString(group_elem, "Merchant"+i);
//    mrc.type:=GetConfigString(group_elem, "MerchantType"+i);
//
//    result[i]:=mrc;
//  EndFor
//
//  Return (result);
//EndFunction
