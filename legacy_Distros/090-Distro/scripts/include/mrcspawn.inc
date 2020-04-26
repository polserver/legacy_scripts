use cfgfile;

global mrcspawn_cfg := ReadConfigFile( "mrcspawn" );


function RestockInventory( merchanttype, inventory )
    foreach productgroup in ProductGroups(merchanttype)
        Restock( inventory, productgroup );
    endforeach
endfunction

function ProductGroups( mtype )
    local mtype_elem := FindConfigElem( mrcspawn_cfg, mtype );
    if (!mtype_elem)
        syslog( "Unable to find config elem " + mtype );
        return 0;
    endif

    return GetConfigStringArray( mtype_elem, "Sells" );
endfunction

function Restock( inventory, productgroup )
    // print( "Restocking " + inventory.name + " with " + productgroup );
    local pg_elem := FindConfigElem( mrcspawn_cfg, productgroup );
    foreach itemstr in GetConfigStringArray( pg_elem, "Item" )
        sleepms(100);
        SpawnItem( inventory, itemstr );
    endforeach
endfunction

function SpawnItem( inventory, itemstr )
    // print( "SpawnItem: " + itemstr );
    local ii := SplitWords( itemstr );
    local objtype := ii[1];
    if (cint(objtype))
       objtype := cint(objtype);
    endif
    local count := CInt(ii[2]);

// TODO: use stackability to create single items or add to stacks
//       Stacks are always added to, currently.
//       Calling CreateItemInContainer 'count' times would do this
//           automatically, actually
    
    count := count - CountExisting( inventory, objtype );

    if (count > 0)
        // print( "Creating " + count + " of objtype " + objtype + " in " + inventory.name );
        local i;
        for( i := 1; i <= count; i := i + 1 )
            // print( " - Creating 1 objtype " + objtype + " in " + inventory.name );
            local res := CreateItemInInventory( inventory, objtype, 1 );
            if (!res)
                //syslog( "Unable to spawn " + objtype + ": " + res.errortext );
            endif
        endfor
    endif
endfunction

// This would be nice, and fast, but this doesn't handle multiple stacks
// In particular, it doesn't handle multiple single nonstackable items.
//    local itemref := FindObjtypeInContainer( inventory, objtype );
//    if (itemref)
//        count := count - itemref.amount;
//    endif

function CountExisting( container, objtype )
    local count := 0;
    foreach item in EnumerateItemsInContainer( container )
        if (item.objtype = objtype)
            count := count + item.amount;
        endif
    endforeach
    return count;
endfunction



function MerchantGroups()
    local mrcgroups_elem := FindConfigElem( mrcspawn_cfg, "MerchantGroups" );
    if (!mrcgroups_elem)
        syslog( "Unable to find 'MerchantGroups' element in mrcspawn.cfg" );
        return 0;
    endif

    return GetConfigStringArray( mrcgroups_elem, "MerchantGroup" );
endfunction

function Merchants( MerchantGroup )
    local result := array;
    local mrc := array;
    mrc.+name;
    mrc.+type;

    local group_elem := FindConfigElem( mrcspawn_cfg, MerchantGroup );
    local count := GetConfigInt( group_elem, "MerchantCount" );
    local i;
    for( i := 1; i <= count; i := i + 1 )
        mrc.name := GetConfigString( group_elem, "Merchant" + i );
        mrc.type := GetConfigString( group_elem, "MerchantType"+i );
        result[i] := mrc;
    endfor

    return result;
endfunction
