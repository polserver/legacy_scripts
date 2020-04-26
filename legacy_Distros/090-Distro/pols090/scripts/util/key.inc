/*
 * key.inc - key functions
 */
include "include/client";
include "include/objtype";

/*
 * AllocLockId - allocate a lock ID.
 * 
 * Locks are allocated a new lockid when they are created or changed.
 * If a lock's lockid is changed, all old keys will no longer work.
 *
 * TODO: go critical.  The only place this is called is already critical, so...
 */
function AllocLockId()
    local lockid := GetGlobalProperty( "nextlockid" );
    if (!lockid)
        lockid := 1;
    endif
    SetGlobalProperty( "nextlockid", lockid+1 );
    return lockid;
endfunction

// does 'who' have a key matching this lockid?
function FindKey(who, what)
    var lockid := GetObjProperty( what, "lockid" );
    
    if (!lockid) // not keyed
        return 1;
    endif
    
    var keytypes := { UOBJ_COPPER_KEY,
                      UOBJ_GOLD_KEY,
                      UOBJ_IRON_KEY,
                      UOBJ_MAGIC_KEY,
                      UOBJ_RUSTY_IRON_KEY };
    
    foreach item in EnumerateItemsInContainer( who.backpack )
        if (item.graphic in keytypes)
            if (GetObjProperty( item, "lockid" ) = lockid)
                return item;
            endif
        endif
    endforeach

    return 0;
endfunction