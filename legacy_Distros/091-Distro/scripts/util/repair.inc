

function repair_item(character, what, skillid )

    local maxhp;
    maxhp := what.maxhp;
    maxhp := cint(maxhp * what.quality);

    local thehp := what.hp; 
    local damaged := maxhp - thehp;

    local hisskill := getskill(character, skillid);

    local damageamount := 0.0;
    damageamount := (125 -  hisskill)/3000.0;

    if (what.hp = what.maxhp)
        //then the item is fully repaired already
        SendSysmessage(character, "That item is of full repair.");
        return;  
    endif

    if ((what.hp <= 0) or (what.quality <= 0.1) or (!what.quality))
        SendSysmessage(character, "The brittle material breaks when you handle it");
        destroyitem(what);
        print("Destroyed item when " + character.name + " repaired it");
        return;
    endif
    if (what.quality <= damageamount) 
        SendSysmessage(character, "The brittle material breaks when you handle it");
        destroyitem(what);
        print("Destroyed item when " + character.name + " repaired it");
        return;
    endif


    what.quality := what.quality - damageamount;
    print ("quality is now " + what.quality);
    print (character.name + " is repairing " + what.name + ". The old quality was " + what.quality + damageamount + " but it is now " + what.quality);

    if ((what.hp <= 0) or (what.quality <= 0.1) or (!what.quality))
        SendSysmessage(character, "The brittle material breaks when you handle it");
        destroyitem(what);
        print("Destroyed item when " + character.name + " repaired it");
        return;
    endif

    if (checkskill(character, skillid, cint(damaged/3), 0))

        local repaired_amount := cint( ((randomint(hisskill) + 1))/2);
        print (character.name + " repaired " + repaired_amount + "hitpoints from an item with " + what.hp + " hp ");
        if (damaged = 1)
            repaired_amount := 1;
        endif

        if (thehp + repaired_amount > what.maxhp)
            what.hp := what.maxhp;
            SendSysmessage (character, "You  repair the item completely");        
        else
            what.hp := thehp + repaired_amount;
            SendSysmessage (character, "You successfully repair some of the damage");
        endif

        if ((what.hp <= 0) or (what.quality <= 0.1) or (!what.quality))
            SendSysmessage(character, "The brittle material breaks when you handle it");
            destroyitem(what);
            print("Destroyed item when " + character.name + " repaired it");
            return;
        endif      
        
    else
        local damage_amount := randomint(10);
        if (thehp <= damage_amount)
                Destroyitem(what);
                SendSysmessage (character, "You fail and destroy the item");
        endif
        thehp := thehp - damage_amount;
        what.hp := thehp; 
        
        if (randomint(2) = 1)
            what.quality := what.quality - (damaged/100);
        endif
        
        if (thehp <= 0)
            Destroyitem(what);
            SendSysmessage (character, "You fail and destroy the item");
        else
            SendSysmessage (character, "You fail and damage the item");
        endif
 
        if ((what.hp <= 0) or (what.quality <= 0.1) or (!what.quality))
            SendSysmessage(character, "The brittle material breaks when you handle it");
            destroyitem(what);
            print("Destroyed item when " + character.name + " repaired it");
            return;
        endif      
    endif

endfunction
