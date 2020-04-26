
function Icanspawn(thechar)
foreach person in enumerateonlinecharacters()
        if ((GetObjProperty(person,"canspawn")) and (person.serial != thechar.serial))
                return 0;
        endif
endforeach
setobjproperty(thechar, "canspawn", 1);
return 1;
endfunction