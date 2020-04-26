//
// bank.inc: global bankbox functions
//
// public functions:
//      FindBankBox( character)      returns World Bank bankbox for a character
//      WipeBankBox( character )     does what you might think.
//      OpenWorldBank()              returns handle to World Bank.
//

use os;

include "include/objtype";

function FindBankBox( character )
    local worldbank := OpenWorldBank();

    local bank_obj_name := "Bankbox of " + character.serial;
    local bankbox := FindRootItemInStorageArea( worldbank, bank_obj_name );

    if (!bankbox)
        bankbox := CreateRootItemInStorageArea( worldbank, bank_obj_name, UOBJ_BANKBOX );
    endif

        // should never happen.  FIXME make sure and document that fact
    if (!bankbox)
        syslog( "Unable to find or create bankbox for " + character.serial );
    endif
    
    return bankbox;
endfunction

function WipeBankBox( character )
    local worldbank := OpenWorldBank();

    local bank_obj_name := "Bankbox of " + character.serial;
    
    return DestroyRootItemInStorageArea( worldbank, bank_obj_name );
endfunction

function OpenWorldBank()
    local bank := FindStorageArea( "World Bank" );
    if (!bank)
        bank := CreateStorageArea( "World Bank" );
    endif

        // this should not happen. FIXME, make sure and document that fact.
    if (!bank)
        syslog( "Unable to open or create world bank!" );
    endif

    return bank;
endfunction