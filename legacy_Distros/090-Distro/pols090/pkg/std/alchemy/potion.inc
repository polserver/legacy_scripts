global conf := ReadConfigFile("alchemy"); //do I pu this here?

function is_reagent (what)
  
        if( (what.objtype >=  REAGENT_START) AND (what.objtype <= REAGENT_STOP) )
                return  1;
        else
                return 0;
        endif
        
endfunction



function PourPotion(user,mortar)
    
        try_to_make_potion(user, "spacefill", 1, mortar, 0);

endfunction


Function make_the_potion (user, mortar)


Local use_on := Target( user );
    if (!use_on)
        SendSysMessage(user, "Targetting Cancelled");
        return;
    endif

if ( is_reagent(use_on) )

        local conf := ReadConfigFile("alchemy");
        if (!conf)
                print( "Couldn't open config file!");
                return;
        endif
            
        local elem := conf[use_on.objtype]; 
                if (!conf)
                print( "Couldn't find the element(elem)!");
        endif
            
        local possible := array;
        local tmenu := CreateMenu("Select a potion to make");
        local num_possibilities := 0;
        local selections := array;

        possible := GetConfigStringArray( elem, "potion");


        foreach thingie in possible
                if (can_make(user, thingie, use_on))
                        local titem := conf[thingie].objtype;
                        if(! titem)
                                
                        else
                                AddMenuItem( tmenu, titem, thingie );
                                num_possibilities := num_possibilities + 1;
                        endif
                else
                        
                endif
        endforeach
        
        
        
        if (num_possibilities = 0)
                SendSysMessage(user, "You can't make any potions!");
                return;
        endif

        local Citem := SelectMenuItem2(user, tmenu);
        if (!citem.objtype)
                SendSysmessage(user, "Targetting canceled");
                return -1;
        endif        
        try_to_make_potion(user, possible[Citem.index], 0, mortar, use_on);
else

        SendSysMessage(user, "That's not a reagent!");

endif

endfunction

function can_make(user, potion, reg)

local skillneeded:= conf[potion].skill;

local amount_needed:= conf[potion].reagent_amount;

if (GetSkill(user, SKILLID_ALCHEMY) >= skillneeded)
        if (reg.amount >= amount_needed)
                return 1;
        else
                return 0;
        endif
else
        return 0;
endif

endfunction


function take_regs(user, potion, reg)
        
        local amount_needed:= conf[potion].reagent_amount;
        set_critical(1);
        if (cint(amount_needed) <= reg.amount)
                subtractamount(reg, amount_needed);
                set_critical(0);
                return 1;
        else
                SendSysmessage(user, "You don't have enough reagents!");
                set_critical(0);
                return 0;
        endif
        
endfunction


function try_to_make_potion(user, potion, in_mortar, mortar, reg)

if (!in_mortar)             //we have to check the skill, 'cause it's not in the mortar

 
        local difficulty := conf[potion].skill;
        local points := conf[potion].points;
        local product := conf[potion].objtype;
        local cntr;
        for (cntr:= 1; cntr <= 4; cntr := cntr + 1)
                playsoundeffect(user, SOUND_EFFECT_GRIND);
                printtextaboveprivate(user, "You grind the reagent",user);
                sleepms(1500);
        endfor
        if (!take_regs(user, potion, reg))
                return;
        endif        
        if (checkskill(user, SKILLID_ALCHEMY, difficulty, points))
                local hasbottle:= 0;
                set_critical(1);
                foreach item in EnumerateItemsInContainer(user.backpack)
                        if (item.objtype = UOBJECT_EMPTY_BOTTLE)
                                SubtractAmount(item, 1);
                                hasbottle := 1;
                                break;
                        endif
                endforeach
                set_critical(0);
                if (hasbottle)
                        local thing := CreateItemInBackpack(user, product, 1);
                        
                        PlaySoundEffect(user, SOUND_EFFECT_POUR);
                        PrintTextAbovePrivate(user, "You create the potion and put it in your backpack", user);
                        return;
                else
                        PrintTextAbovePrivate(user, "With no bottles, you are forced to store the mixture in your mortar", user);
                        SetObjProperty(mortar, "contents", product);
                        return;
                endif
        else
                PlaySoundEffect(user, SOUND_EFFECT_POUR);
                PrintTextAbovePrivate(user, "You pour out the useless mixture", user);
                return;
        endif

else

        local hasbottle:= 0;
        set_critical(1);
        foreach item in EnumerateItemsInContainer(user.backpack)
                if (item.objtype = UOBJECT_EMPTY_BOTTLE)
                        SubtractAmount(item, 1);
                        hasbottle := 1;
                        break;
                endif
                endforeach
                
                set_critical(0);

                if (hasbottle)
                        local product := GetObjProperty(mortar, "contents");
                        local thing := CreateItemInBackpack(user, product, 1);
                        PlaySoundEffect(user, SOUND_EFFECT_POUR);
                        PrintTextAbovePrivate(user, "You create the item and put it in your backpack", user);
                        EraseObjProperty( mortar, "contents" );
                        return;
                else
                        PrintTextAbovePrivate(user, "You have no bottles to pour this into", user);
                        return;
                endif
                
endif


endfunction
