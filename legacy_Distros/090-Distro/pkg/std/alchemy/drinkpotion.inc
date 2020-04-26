global itemdefs := ReadConfigFile("::itemdesc");
Const POISON_SOUND := 0x247;
Const DRINK_SOUND := 0x32;
Const UOBJ_EMPTY_BOTTLE := 0x0f0e;
Const EFFECT_BLUE_SPARKLE  := 0x373a;
Const SOUND_EFFECT_CURE	:= 0x1e1; 
Const EFFECT_SPARKLE  := 0x374a;
Const HIT_ACTION := 0x148b;
Const MOVING_EFFECT_PURPLE_POTION  := 0x0F0D; 
Const SOUND_EFFECT_EXPLOSION	:= 0x208; 
Const SOUND_EFFECT_HEAL	:= 0x1f3; 
Const EFFECT_EXPLOSION := 0x36b0;
Const SKILLID_TASTEID := 36;

function checkifcandrink(character)

        if (getobjproperty(character, "#potion"))
                if (getobjproperty(character, "#potion") > readgameclock() )
                        SendSysmessage(character, "You must wait until you can drink another potion!");
                        return 0;
                else
                
                        setobjproperty(character, "#potion", readgameclock() + 10);
                        return 1;
                endif
        else
                setobjproperty(character, "#potion", readgameclock() + 10);
                return 1;
        endif
        

endfunction


function candrink(character)
        eraseobjproperty(character, "#potion");
endfunction

function empty_bottle(character, potion)

    PlaySoundEffect (character, DRINK_SOUND);
    CreateItemInBackpack(character, UOBJ_EMPTY_BOTTLE, 1);
    DestroyItem(potion);
    PlayObjectCenteredEffect( character, EFFECT_BLUE_SPARKLE, 10,10);  
    
endfunction

function get_potion_strength(potion)

        local potion_strength := itemdefs[potion.objtype].strength;

        return potion_strength;

endfunction

function is_poison(potion)

        local poison_level := GetObjProperty(potion, "poison_level");
        return poison_level;

endfunction

function do_poison(character, potion)


  local potion_strength := is_poison(potion);
        if (Checkskill(character, SKILLID_TASTEID, (potion_strength * 15),  (potion_strength * 15)))
                SendSysMessage(character, "That potion is poisonous!");
                return;
        endif
  empty_bottle(character, potion);
  PrintTextAbovePrivate (character, "You have been poisoned!", character);
  local poison_level :=Cint(GetObjProperty(character, "poison_level"));

    if ( poison_level > 0)
        poison_level := poison_level + potion_strength;
        SetObjProperty (character, "poison_level",cstr(poison_level));
    else
        poison_level := poison_level + potion_strength;
        SetObjProperty (character, "poison_level",cstr(poison_level));
        start_script( ":spells:poisondamage", character );
    endif 

endfunction