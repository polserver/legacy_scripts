/*

this really has one useful function: make_loot. This function takes an objtype, for which it then searches in corpse.cfg.
if found, it fills the container with loot from the appropriate lootgroup.

TODO: it sure would be nice to base this on template, rather than objtype
Do we need an "always" loot entry for the lootgroup, so certain items are guaranteed to be on the corpse?
also, shoudl I check to be sure they don't get the same item twice? I don't think so, but ...

*/


function make_loot (objtype, container)
        
    set_critical(1);
    local corpsecfg := ReadConfigFile("corpses");
    local elem := FindConfigElem(corpsecfg, objtype);
    local destroy_corpse := GetConfigInt(elem, "destroy");
    if (!elem)
        return;
    endif

    local lootgroup := GetConfigInt(elem, "lootgroup");

    if (!lootgroup)
        if (destroy_corpse)
            DestroyItem(container);
        endif
        return;
    endif
    
    local mypack := FindMyPack(GetObjProperty(container, "serial"));
    if (mypack)
        if (GetObjProperty(container, "summoned"))
            foreach thingie in EnumerateItemsInContainer(mypack)
                MoveItemToLocation(thingie, container.x, container.y, container.z, MOVEITEM_NORMAL );
            endforeach
        else
            foreach thingie in EnumerateItemsInContainer(mypack)
                MoveItemToContainer(thingie, container);
            endforeach
        endif
    endif

    if (GetObjProperty(container, "summoned"))
        DestroyItem(container);
        return;
    endif
    foreach lootitem in LoadAndProcessLootgroup( lootgroup )
        if (destroy_corpse)
            CreateLootItemAtLocation(container.x, container.y, container.z, lootitem);
        else
            CreateLootItemInContainer( container, lootitem );
        endif
    endforeach
    
    if (destroy_corpse)
        DestroyItem(container);
    endif
    
    set_critical(0);
        
endfunction

function LoadAndProcessLootgroup(lootgroup)

    local lootgrpcfg := ReadConfigFile("lootgroup");
    local elem := FindConfigElem(lootgrpcfg, lootgroup);

    local return_loot := GetConfigStringArray( elem, "always" );
    foreach lootitem in return_loot
        lootitem := CInt( lootitem );
    endforeach

    local lootitems := GetConfigStringArray( elem, "item" );
    local thecount := len( lootitems );
    
    // OK, lootitems now contains an array containing all 
    // the possible items for the lootgroup
    // return_loot already contains mandatory loot

    local diestring := GetConfigString(elem, "get_items");
    local first := len( return_loot ) + 1;
    local last := first + RandomDiceRoll( diestring );
    local i;
    for (i := first; i <= last; i := i + 1)
        return_loot[i] := CInt(lootitems[randomint(thecount) + 1]);
    endfor
    
    return return_loot;

endfunction

function CreateLootItemInContainer(corpse, lootitem)
    local lootcfg       := ReadConfigFile("loot");
    local elem          := FindConfigElem(lootcfg,lootitem);
    local theitem       := GetConfigInt(elem, "item");
    local amount        := GetConfigString(elem, "amount");
    local propname      := GetConfigString(elem, "propname");
    local proptype      := GetConfigString(elem, "proptype");
    local propval       := GetConfigString(elem, "propval");
    local thename       := GetConfigString(elem, "name");
    local theusescript  := GetConfigString(elem, "usescript");
    local intAmount     := RandomDiceRoll(amount);
    
    if (!theitem)
        theitem := GetObjtypeByName( elem.item );
        if (!theitem)
            syslog( "loot.cfg[" + lootitem + "]: Item " + elem.item + " not found." );
        endif
    endif

    if (propname or thename or theusescript)
        local cntr;
        for (cntr := 1; cntr <= intAmount; cntr := cntr + 1)
            local created := CreateItemInContainer (corpse, cint(theitem), 1);
            if (propname)
                    case proptype
                        "i" : SetObjProperty(created, propname, cint(propval));
                        "r" : SetObjProperty(created, propname, cstr(propval));
                        "s" : SetObjProperty(created, propname, propval);
                        default: SetObjProperty(created, propname, propval);
                    endcase
            endif

            if (thename)
                created.name := thename;
            endif

            if (theusescript)
                created.usescript := theusescript;
            endif
        endfor
    else
        CreateItemInContainer(corpse, cint(theitem), intAmount);
    endif
endfunction

function CreateLootItemAtLocation(x,y,z, lootitem)
    local lootcfg       := ReadConfigFile("loot");
    local elem          := FindConfigElem(lootcfg,lootitem);
    local theitem       := GetConfigInt(elem, "item");
    local amount        := GetConfigString(elem, "amount");
    local propname      := GetConfigString(elem, "propname");
    local proptype      := GetConfigString(elem, "proptype");
    local propval       := GetConfigString(elem, "propval");
    local thename       := GetConfigString(elem, "name");
    local theusescript  := GetConfigString(elem, "usescript");
    local intAmount     := RandomDiceRoll(amount);

    if (propname or thename or theusescript)
        local cntr;
        for (cntr := 1; cntr <= intAmount; cntr := cntr + 1)
                local created := CreateItemAtLocation (x,y,z, theitem, 1);
                if (propname)
                    case proptype
                        "i" : SetObjProperty(created, propname, cint(propval));
                        "r" : SetObjProperty(created, propname, cstr(propval));
                        "s" : SetObjProperty(created, propname, propval);
                        default: SetObjProperty(created, propname, propval);
                    endcase
                endif

                if (thename)
                    created.name := thename;
                endif

                if (theusescript)
                    created.usescript := theusescript;
                endif
        endfor
    else
        CreateItemAtLocation(x,y,z, theitem, intAmount);
    endif
endfunction

function OpenTamedStorageAreas()
    local bank := FindStorageArea( "Tamed Storage" );
    if (!bank)
        bank := CreateStorageArea( "Tamed Storage" );
    endif

    if (!bank)
        syslog( "Unable to open or create world bank!" );
    endif

    return bank;
endfunction

function FindMyPack(serial)
    local mybank := OpenTamedStorageAreas();

    local bank_obj_name := "Bankbox  " + hex(serial);
    local bankbox := FindRootItemInStorageArea( mybank, bank_obj_name );

    if (!bankbox)
        RETURN 0;
    endif
    
    return bankbox;

endfunction