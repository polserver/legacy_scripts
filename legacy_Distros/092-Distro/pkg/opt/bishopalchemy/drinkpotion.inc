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

function checkifcandrink(character, type)
  if(getobjproperty(character, type))
    if(getobjproperty(character, type) > readgameclock())
      SendSysmessage(character, "You must wait until you can drink another potion!");
      return 0;
    else
      setobjproperty(character, type, readgameclock() + 10);
      return 1;
    endif
  else
    setobjproperty(character, type, readgameclock() + 10);
     return 1;
  endif
endfunction

function empty_bottle(character, potion)
  PlaySoundEffect (character, DRINK_SOUND);
  CreateItemInBackpack(character, UOBJ_EMPTY_BOTTLE, 1);
  DestroyItem(potion);
endfunction

function get_potion_strength(potion)
  local potion_strength := itemdefs[potion.objtype].Strength;
  return potion_strength;
endfunction

function is_poison(potion)
  local poison_level := GetObjProperty(potion, "poison_level");
  return poison_level;
endfunction

function do_poison(character, potion)
  local potion_strength := get_potion_strength(potion);
print(potion_strength);
  empty_bottle(character, potion);
  PrintTextAbovePrivate (character, "You have been poisoned!", character);
  local poison_level :=Cint(GetObjProperty(character, "poison_level"));
  if(poison_level > potion_strength)
    return;
  else
    SetObjProperty (character, "poison_level", potion_strength);
    local passparms := {character, character, "poison potion"};
    detach();
    SetObjProperty(character,"PoisonStamp", ReadGameClock());
    start_script( ":spells:poisondamage", passparms);
  endif
endfunction