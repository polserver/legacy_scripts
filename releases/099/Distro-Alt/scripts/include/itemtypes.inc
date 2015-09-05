//////////////////////////////////////////////////////////////////////////////////////////////////
//												//
// 	This Include contains the following Functions.						//
//												//
// 	IsDoor(door) 		IsHair(hair) 			IsFacialHair(beard)		//
// 	IsJewelry(jewelry) 	IsWeapon(weapon)		IsWoodenWeapon(weapon)		//
// 	IsMetalWeapon(weapon) 	IsBow(weapon) 			IsArmor(armor)			//
//	IsLeatherArmor(armor) 	IsMetalArmor(armor)		IsBoneArmor(armor		//
//	IsShield(shield)	IsMetalShield(shield) 		IsWoodenShield(Shield)		//
//	IsClothing(clothes)	IsMusicalInstrument(item)	IsStaff(staff)			//
//	IsLog(log)		IsPoinableArrows(arrow)		IsReagent(reagent)		//
//	IsOre(ore)		IsIngot(ingot)			IsStackableItem(item)		//
//	IsSteedContainer(cont)	IsBone(bone)			IsAKey(key)			//
//	IsEquippable(item)	IsAGem(gemobjtype)		IsABoard(use_on)		//
//												//
//////////////////////////////////////////////////////////////////////////////////////////////////

use cfgfile;

include "include/objtype";
include "include/client";

var tiles_cfg := ReadConfigFile("tiles");

function IsDoor(byref door)

	if (door.objtype >= 0xe8 && door.objtype <= 0xf7)
		return 1;
	elseif (door.objtype >= 0x314 && door.objtype <= 0x365)
		return 1;
	elseif (door.objtype >= 0x675 && door.objtype <= 0x6f4)
		return 1;
	elseif (door.objtype >= 0x824 && door.objtype <= 0x833)
		return 1;
	elseif (door.objtype >= 0x839 && door.objtype <= 0x848)
		return 1;
	elseif (door.objtype >= 0x84c && door.objtype <= 0x85b)
		return 1;
	elseif (door.objtype >= 0x866 && door.objtype <= 0x875)
		return 1;
	elseif (door.objtype >= 0x1fed && door.objtype <= 0x1ffc)
		return 1;
	endif

	return 0;

endfunction

function IsHair(byref hair)

	case (hair.objtype)
		0x203b:	//Short Hair
		0x203c:	//Long Hair 
		0x203d:	//Pony Tail
		0x2044:	//MoHawk
		0x2045:	//Page Boy
		0x2046:	//Buns
		0x2047:	//Afro
		0x2048:	//Receding
		0x2049:	//Pigtails
		0x204a:	//Krisna
		return 1;
		default: return 0;
	endcase

endfunction

function IsFacialHair(byref beard)

	case (beard.objtype)
		0x203e:	//Long Beard
		0x203f:	//Short Beard
		0x2040:	//Goatee
		0x2041:	//Mustache
		0x204b:	//Short beard w/mustache
		0x204c:	//Long beard w/mustache
		0x204d:	//Vandyke
		return 1;
	endcase
		return 0;
endfunction

function IsJewelry(byref jewelry)

	if (jewelry.objtype >= 0x1085 && jewelry.objtype <= 0x108a)
		return 1;
	elseif (jewelry.objtype >= 0x1f05 && jewelry.objtype <= 0x1f0a)
		return 1;
	endif
		return 0;
endfunction

function IsWeapon(byref weapon)

	if (weapon.objtype >= 0x0df0 && weapon.objtype <= 0x0df5)
		return 1;
	elseif (weapon.objtype >= 0x0e81 && weapon.objtype <= 0x0e8a && weapon.objtype != 0x0e83)
		return 1;
	elseif (weapon.objtype >= 0x0ec2 && weapon.objtype <= 0x0ec5)
		return 1;
	elseif (weapon.objtype >= 0x0f43 && weapon.objtype <= 0x0f52)
		return 1;
	elseif (weapon.objtype >= 0x0f5c && weapon.objtype <= 0x0f63)
		return 1;
	elseif (weapon.objtype >= 0x0fb4 && weapon.objtype <= 0x0fb5)
		return 1;
	elseif (weapon.objtype >= 0x13af && weapon.objtype <= 0x13ba)
		return 1;
	elseif (weapon.objtype >= 0x13e3 && weapon.objtype <= 0x13e4)
		return 1;
	elseif (weapon.objtype >= 0x13f4 && weapon.objtype <= 0x1407)
		return 1;
	elseif (weapon.objtype >= 0x1438 && weapon.objtype <= 0x1443)
		return 1;
	endif
		return 0;
endfunction

function IsWoodenWeapon(byref weapon)

	if (weapon.objtype >= 0x0df0 && weapon.objtype <= 0x0df1)
		return 1;
	elseif (weapon.objtype >= 0x0e81 && weapon.objtype <= 0x0e82)
		return 1;
	elseif (weapon.objtype >= 0x0e89 && weapon.objtype <= 0x0e8a)
		return 1;
	elseif (weapon.objtype >= 0x0f4f && weapon.objtype <= 0x0f50)
		return 1;
	elseif (weapon.objtype >= 0x13b0 && weapon.objtype <= 0x13b4)
		return 1;
	elseif (weapon.objtype >= 0x13f4 && weapon.objtype <= 0x13f5)
		return 1;
	elseif (weapon.objtype >= 0x13f8 && weapon.objtype <= 0x13f9)
		return 1;
	elseif (weapon.objtype >= 0x13fc && weapon.objtype <= 0x13fd)
		return 1;
	elseif (weapon.objtype == 0x7068 || weapon.objtype == 0x706a)
		return 1;
	elseif (weapon.objtype >= 0x13ac && weapon.objtype <= 0x13ae)
		return 1;
	elseif (weapon.objtype == 0x7065)
		return 1;
	endif
		return 0;
endfunction

function IsMetalWeapon(byref weapon)

	if (weapon.objtype >= 0x0df2 && weapon.objtype <= 0x0df5)
		return 1;
	elseif (weapon.objtype >= 0x0e85 && weapon.objtype <= 0x0e88)
		return 1;
	elseif (weapon.objtype >= 0x0f43 && weapon.objtype <= 0x0f4e)
		return 1;
	elseif (weapon.objtype >= 0x0f51 && weapon.objtype <= 0x0f52)
		return 1;
	elseif (weapon.objtype >= 0x0f5c && weapon.objtype <= 0x0f63)
		return 1;
	elseif (weapon.objtype >= 0x0fb4 && weapon.objtype <= 0x0fb5)
		return 1;
	elseif (weapon.objtype >= 0x13af && weapon.objtype <= 0x13b0)
		return 1;
	elseif (weapon.objtype >= 0x13b5 && weapon.objtype <= 0x13ba)
		return 1;
	elseif (weapon.objtype >= 0x13e3 && weapon.objtype <= 0x13e4)
		return 1;
	elseif (weapon.objtype >= 0x13f6 && weapon.objtype <= 0x13f7)
		return 1;
	elseif (weapon.objtype >= 0x13fa && weapon.objtype <= 0x13fb)
		return 1;
	elseif (weapon.objtype >= 0x13fe && weapon.objtype <= 0x1407)
		return 1;
	elseif (weapon.objtype >= 0x1438 && weapon.objtype <= 0x1443)
		return 1;
	elseif (weapon.objtype == 0xcdb9 || weapon.objtype == 0xf53 || weapon.objtype == 0xf46)
		return 1;
	elseif (weapon.objtype == 0xf3d || weapon.objtype == 0xf40 || weapon.objtype == 0xf41)
		return 1;
	elseif (weapon.objtype == 0xf42 || weapon.objtype == 0xf44 || weapon.objtype == 0xf4d)
		return 1;
	elseif (weapon.objtype == 0xf50 || weapon.objtype == 0xf52)
        return 1;
	endif
		return 0;
endfunction

function IsBow(byref weapon)

	if (weapon.objtype >= 0x0f4f && weapon.objtype  <= 0x0f50)
		return 1;
	elseif (weapon.objtype == 0xf55 || weapon.objtype == 0xf54)
		return 1;
	elseif (weapon.objtype == 0xf4c || weapon.objtype == 0xf48 || weapon.objtype == 0xf4a)
		return 1;
	elseif (weapon.objtype >= 0x13b0 && weapon.objtype <= 0x13b2)
		return 1;
	elseif (weapon.objtype >= 0x13fc && weapon.objtype <= 0x13fd)
		return 1;
	elseif (weapon.objtype == 0x6050)
		return 1;
	elseif (weapon.objtype == 0x26c2)
		return 1;
	elseif (weapon.objtype >= 0x13ac && weapon.objtype <= 0x13ae)
		return 1;
	endif
		return 0;
endfunction

function IsArmor(byref armor)

	if (armor.objtype == 0x13bb)
		return 1;
	elseif (armor.objtype >= 0x13be && armor.objtype <= 0x13c0)
		return 1;
	elseif (armor.objtype >= 0x13c3 && armor.objtype <= 0x13c7)
		return 1;
	elseif (armor.objtype >= 0x13cb && armor.objtype <= 0x13ce)
		return 1;
	elseif (armor.objtype >= 0x13d2 && armor.objtype <= 0x13d6)
		return 1;
	elseif (armor.objtype >= 0x13da && armor.objtype <= 0x13dd)
		return 1;
	elseif (armor.objtype >= 0x13e1 && armor.objtype <= 0x13e2)
		return 1;
	elseif (armor.objtype >= 0x13eb && armor.objtype <= 0x13f2)
		return 1;
	elseif (armor.objtype >= 0x1408 && armor.objtype <= 0x141a)
		return 1;
	elseif (armor.objtype >= 0x144e && armor.objtype <= 0x1457)
		return 1;
	elseif (armor.objtype >= 0x1c00 && armor.objtype <= 0x1c0d)
		return 1;
	elseif (armor.objtype >= 0x1f0b && armor.objtype <= 0x1f0c)
		return 1;
	endif
		return 0;
endfunction

function IsLeatherArmor(byref armor)
        if (armor.objtype == 0x708b)
                return 1;
	elseif (armor.objtype >= 0x13c5 && armor.objtype <= 0x13c7)
		return 1;
	elseif (armor.objtype >= 0x13cb && armor.objtype <= 0x13ce)
		return 1;
	elseif (armor.objtype >= 0x13d2 && armor.objtype <= 0x13d6)
		return 1;
	elseif (armor.objtype >= 0x13da && armor.objtype <= 0x13dd)
		return 1;
	elseif (armor.objtype >= 0x13e1 && armor.objtype <= 0x13e2)
		return 1;
	elseif (armor.objtype >= 0x1453 && armor.objtype <= 0x1457)
		return 1;
	elseif (armor.objtype >= 0x1c00 && armor.objtype <= 0x1c0d)
		return 1;
	elseif (armor.objtype == 0x1db9 || armor.objtype == 0x1dba)
		return 1;
	endif
		return 0;
endfunction

function IsMetalArmor(byref armor)

	if (armor.objtype == 0x13bb)
		return 1;
	elseif (armor.objtype >= 0x13be && armor.objtype <= 0x13c0)
		return 1;
	elseif (armor.objtype >= 0x13c3 && armor.objtype <= 0x13c4)
		return 1;
	elseif (armor.objtype >= 0x13eb && armor.objtype <= 0x13f2)
		return 1;
	elseif (armor.objtype >= 0x1408 && armor.objtype <= 0x141a)
		return 1;
	elseif (armor.objtype >= 0x1c04 && armor.objtype <= 0x1c05)
		return 1;
	elseif (armor.objtype == UOBJ_CHAINMAIL_GLOVES)
		return 1;
	elseif (armor.objtype == UOBJ_RINGMAIL_COIF)
		return 1;
	endif
	return 0;
endfunction

function IsBoneArmour(byref armor)

	if (armor.objtype >= 0x144e && armor.objtype <= 0x1452)
		return 1;
        endif
	return 0;

endfunction


function IsShield(byref shield)

	if (shield.objtype >= 0x1b72 && shield.objtype <= 0x1b7b)
		return 1;
	elseif (shield.objtype >= 0x1bc3 && shield.objtype <= 0x1bc5)
		return 1;
	endif
		return 0;
endfunction

function IsMetalShield(byref shield)

	if (shield.objtype >= 0x1b72 && shield.objtype <= 0x1b77)
		return 1;
	elseif (shield.objtype == 0x1b7b)
		return 1;
	elseif (shield.objtype >= 0x1bc3 && shield.objtype <= 0x1bc5)
		return 1;
	endif
		return 0;
endfunction

function IsWoodenShield(byref shield)

	if (shield.objtype  >= 0x1b78 && shield.objtype <= 0x1b7a)
		return 1;
	endif
		return 0;
endfunction

function IsClothing(byref clothes)

	if (clothes.objtype >= 0x1515 && clothes.objtype <= 0x1518)
 		return 1;
	elseif (clothes.objtype >= 0x152e && clothes.objtype <= 0x1531)
 		return 1;
	elseif (clothes.objtype >= 0x1537 && clothes.objtype <= 0x1544)
		return 1;
	elseif (clothes.objtype >= 0x1708 && clothes.objtype <= 0x171c)
		return 1;
	elseif (clothes.objtype >= 0x1efd && clothes.objtype <= 0x1f07)
		return 1;
	elseif (clothes.objtype >= 0x1f7b && clothes.objtype <= 0x1f7c)
		return 1;
	elseif (clothes.objtype >= 0x1f9f && clothes.objtype <= 0x1fa2)
		return 1;
	elseif (clothes.objtype >= 0x1ffd && clothes.objtype <= 0x1ffe)
		return 1;
	elseif (clothes.objtype >= 0x204e && clothes.objtype <= 0x204f)
		return 1;
	endif
		return 0;
endfunction

function IsMusicalInstrument(byref item)

	case(item.objtype)
		0x7067:    //FLUTE	
		0x0e9d:    //TAMBORINE
		0x0e9e:    //TAMBORINE2
		0x0eb3:    //LUTE
		0x0eb4:    //LUTE2	
		0x0e9c:    //DRUM
		0x0eb2:    //HARP	
		0x0eb1:    //STANDING HARP
		return 1;
		default: return 0;
	endcase
endfunction

function IsStaff(byref item)

	case(item.objtype)
		0x13f9:          //club
                0x0e81:          //shepherd
                0x0e8a:          //Quater
                0x13f9:          //gnarled
                0x7065:          //mage
                0x0df0:          //black
                0x706a:          //lich/nerco
		0x26bf:		// Double Bladed
		0x26bd:		// Bladed
		0x26ba:		// Scythe (grim reaper's staff)
		return 1;
		default: return 0;
	endcase
endfunction

function IsLog(byref item)

	if (item.objtype >= 0x6351 && item.objtype <= 0x6367)
		return 1;
        elseif(item.objtype == 0x1BDD)
	        return 1;
        else
                return 0;
	endif
endfunction

function IsPoisonAbleArrows(byref item)

	case(item.objtype)
		0x0f3f:    //Arrows
                0x1bfb:    //Crossbow bolts
		return 1;
		default: return 0;
	endcase
endfunction

///////////////////////////Needs work////////////////////////
/////////////////////////////////////////////////////////////
//////////////////////Finish me NOW!/////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
function IsReagent(byref item)

	case(item.objtype)
////////////////// 8 Original Reagents ///////////////
		0x0f7a:	//BLACK PEARL
		0x0f7b:	//BLOOD MOSS
		0x0f84:	//GARLIC
		0x0f85:	//GINSENG
		0x0f86:	//MANDRAKE ROOT
		0x0f88:	//NIGHTSHADE
		0x0f8c:	//SULFUROUS ASH
		0x0f8d:	//SPIDER SILK

//////////////////// Pagan Reagents //////////////////
		0x0f78:	//BATWING
		0x0f79:	//BLACKMOOR
		0x0f7c:	//BLOODSPAWN
		0x0f7e:	//BONE
		0x0f7f:	//BRIMSTONE
		0x0f80:	//demon BONE
		0x0f90:	//DEAD WOOD
		0x0f82:	//DRAGONS BLOOD
		0x0f83:	//EXECUTIONERS CAP
		0x0f81:	//FERTILE DIRT
		0x0f8e:	//SERPENT SCALE
		0x0f7d:	//VIAL OF BLOOD
		0x0f8f:	//VOLCANIC ASH
		0x0f91:	//WYRMS HEART
		return 1;
		default: return 0;
	endcase
endfunction




///////////////////
//  Determines if the item passed is a type of metal ore
///////////////////

function IsOre(byref use_on)
  if ((use_on.objtype >= 0x6300 && use_on.objtype <= 0x6308 || 
  use_on.objtype >= 0x6205 && use_on.objtype <= 0x6213))
    return 1;
  else
    return 0;
  endif
endfunction

///////////////////
//  Determines if the item passed is a type of metal ingot
///////////////////

function IsIngot(use_on)
  if ((use_on.objtype >= 0x6309 && use_on.objtype <= 0x6311 || 
  use_on.objtype >= 0x6245 && use_on.objtype <= 0x6254))
    return 1;
  else
    return 0;
  endif
endfunction




///////////////////
//  determines if the item can stack with other items that the character already has
///////////////////

function IsStackableItem(byref item)
	if (item.amount > 1)
		return 1;
	endif

        if (IsIngot(item))
        		return 1;
	endif

        if (IsReagent(item))
        		return 1;
	endif

        if (IsOre(item))
        		return 1;
	endif

	case (item.objtype)
		0x5001:			// Snow
		0xEE18:			// Sand
		0xEE19:			// Clay
		0xEE20:			// Glass
		0xEE21:			// Porcelain
		UOBJ_GOLD_COIN:
		UOBJ_BATWING:
		UOBJ_BLACKMOOR:
		UOBJ_BLACK_PEARL:
		UOBJ_BLOOD_MOSS:
		UOBJ_BLOODSPAWN:
		UOBJ_VIAL_OF_BLOOD:
		UOBJ_BONE:
		UOBJ_BRIMSTONE:
		UOBJ_DEMON_BONE:
		UOBJ_FERTILE_DIRT:
		UOBJ_DRAGONS_BLOOD:
		UOBJ_EXECUTIONERS_CAP:
		UOBJ_GARLIC:
		UOBJ_GINSENG:
		UOBJ_MANDRAKE_ROOT:
		UOBJ_EYE_OF_NEWT:
		UOBJ_NIGHTSHADE:
		UOBJ_OBSIDIAN:
		UOBJ_PIG_IRON:
		UOBJ_PUMICE:
		UOBJ_SULFUROUS_ASH:
		UOBJ_SPIDER_SILK:
		UOBJ_SERPENT_SCALE:
		UOBJ_VOLCANIC_ASH:
		UOBJ_DEAD_WOOD:
		UOBJ_WYRMS_HEART:
		UOBJ_GEM_AMBER:
		UOBJ_GEM_STAR_SAPPHIRE:
		UOBJ_GEM_EMERALD:
		UOBJ_GEM_SAPPHIRE:
		UOBJ_GEM_RUBY:
		UOBJ_GEM_CITRINE:
		UOBJ_GEM_AMETHYST:
		UOBJ_GEM_TOURMALINE:
		UOBJ_GEM_DIAMOND:
		UOBJ_FEATHER:
		UOBJ_EGG:
                UOBJ_HIDE:
                UOBJ_BROWN_HIDE:
                UOBJ_RED_HIDE:
                UOBJ_BLUE_HIDE:
                UOBJ_GREEN_HIDE:
		UOBJ_SILVER_HIDE:
		UOBJ_BANDAGE:
		UOBJ_ARROW:
		UOBJ_BOLT:
		UOBJ_FIRE_ARROW:
		UOBJ_REAPER_HEARTWOOD:
		UOBJ_ENT_HEARTWOOD:
		UOBJ_ENCHANTED_BONE:
                UOBJ_RAW_RIB:
                UOBJ_RAW_FISHSTEAK:
                UOBJ_RAW_BIRD:
                UOBJ_RAW_HAM:
                UOBJ_BACON_SLAB:
                UOBJ_RAW_SAUSAGE:
                UOBJ_HONEY:
                UOBJ_CARROT:
                UOBJ_APPLE:
                UOBJ_PEACH:
                UOBJ_LEMON:
                UOBJ_LIME:
                UOBJ_BANANA:
                UOBJ_PEAR:
                UOBJ_GRAPES:
                UOBJ_DATES:
                UOBJ_PUMPKIN:
                UOBJ_LETTUCE:
                UOBJ_CABBAGE:
                UOBJ_ONION:
                UOBJ_TURNIP:
                UOBJ_COCONUT:
                UOBJ_RAW_CHICKEN_LEG:
                UOBJ_RAW_LEG_OF_LAMB:
                return 1;
	endcase

	//Already went through the common stackable items and still haven't been able to figure out if its stackable or not, so
	//we'll see if we can create 2 of the item as a last resort.
	set_critical(1);
	var newitem := CreateItemAtLocation(0, 0, 0, item.objtype, 2, "britannia_alt");
	if (newitem.amount == 2)
		DestroyItem(newitem);
		set_critical(0);
		return 1;
	endif
	set_critical(0);
	return 0;
endfunction

function IsSteedContainer(item)

	if (item.objtype == 0x0efa)                      //Spellbook
		return 1;
	elseif (item.objtype == 0x0e40)                  //MetalChest
		return 1;
	elseif (item.objtype >= 0x0e73 && item.objtype <= 0x0e76)    //Backpack-packhorseBackpack-GMBackpack-Bag
		return 1;
	elseif (item.objtype >= 0x0a2d && item.objtype <= 0x0a53)    //Furniture
		return 1;
	elseif (item.objtype >= 0x1e80 && item.objtype <= 0x1e81)    //Furniture
		return 1;
	elseif (item.objtype == 0x0e79)                  //Pouch
		return 1;
	elseif (item.objtype == 0x0e7c)                  //SilverChest
		return 1;
	elseif (item.objtype == 0x0e80)                  //BrassBox
		return 1;
	elseif (item.objtype == 0x09a8)                  //BrassBox1
		return 1;
	elseif (item.objtype == 0x09ab)                  //SilverChest1
		return 1;
	elseif (item.objtype == 0x09b0)                  //Pouch1
		return 1;
	elseif (item.objtype == 0x0e41)                  //MetalChest1
		return 1;
	elseif (item.objtype == 0x0e7a)                  //PicnicBasket
		return 1;
	elseif (item.objtype == 0x0e83)                  //EmptyTub
		return 1;
	elseif (item.objtype == 0x0fae)                  //Barrel1
		return 1;
	elseif (item.objtype == 0x09ac)                  //LargeBasket
		return 1;
	elseif (item.objtype == 0x09b1)                  //SmallBasket
		return 1;
	elseif (item.objtype == 0x6100)                  //Runebook
		return 1;
	elseif (item.objtype == 0x09aa)                  //WoodenBox
		return 1;
	elseif (item.objtype == 0x0e7d)                  //WoodenBox2
		return 1;
	elseif (item.objtype == 0x0e42)                  //WoodenChest
		return 1;
	elseif (item.objtype == 0x0e43)                  //WoodenChest2
		return 1;
	elseif (item.objtype == 0x09a9)                  //SmallCrate
		return 1;
	elseif (item.objtype == 0x0e7e)                  //SmallCrate2
		return 1;
	elseif (item.objtype == 0x0e3f)                  //MediumCrate
		return 1;
	elseif (item.objtype == 0x0e3e)                  //MediumCrate2
		return 1;
	elseif (item.objtype == 0x0e3c)                  //LargeCrate
		return 1;
	elseif (item.objtype == 0x0e3d)                  //LargeCrate2
		return 1;
	elseif (item.objtype == 0x0e77)                  //Barrel
		return 1;
	elseif (item.objtype == 0x0e7f)                  //Keg
		return 1;
	elseif (item.objtype >= 0x0a97 && item.objtype <= 0x0a9e)    //EmptyBookShelf
		return 1;
        endif
       
	return 0;

endfunction

function IsBone(byref item)

/*    if (item.objtype == 0x0f7e or item.objtype == 0x0f80 or item.objtype == 0xa153 or (item.objtype >= 0x0ed3 && item.objtype <= 0x0ed7))
        return 1;
    else
        return 0;
    endif*/
	case(item.objtype)
		0x0f7e:	//BONE
		0x0f80:	//DEMON
                0xa153: //ENCHANTED BONES
                0x0ed3: //liche
                0x0ed4: //scortched
                0x0ed5: //frostbitten
                0x0ed6: //drake
                0x0ed7: //dragon
		return 1;
		default: return 0;
	endcase 
endfunction

function IsAKey (byref item)
    case (item.objtype)
        0x100E:
        0x100F:
        0x1010:
        0x1012:
        0x1013:
            return 1;
    endcase
    return 0;
endfunction


function IsEquippable(byref item)
	var elem;
	var item_number := item.objtype;
	item_number := CInt(item_number);
	elem:=FindConfigElem(tiles_cfg,item_number);
	if(GetConfigString(elem,"Equippable"))
	    return 1;
	else
	    return 0;
	endif
endfunction

///////////////////////////////////////////////
//  Determines if the item is a standard gem
///////////////////////////////////////////////

function IsAGem(gemobjtype)

	case (gemobjtype)
		UOBJ_GEM_DIAMOND:
		UOBJ_GEM_SAPPHIRE:
		UOBJ_GEM_STAR_SAPPHIRE:
		UOBJ_GEM_EMERALD:
		UOBJ_GEM_RUBY:
		UOBJ_GEM_AMETHYST:
		UOBJ_GEM_CITRINE:
		UOBJ_GEM_TOURMALINE:
		UOBJ_GEM_AMBER:
			return 1;
		default:
			return 0;
	endcase

endfunction

////////////////////////////////////////
// Returns a 1 if the item is a board //
////////////////////////////////////////

function IsABoard(byref use_on)
    if (use_on.objtype == 0x1bd7)
	return 1;
    endif
    if ((use_on.objtype >= 0x6370 && use_on.objtype <= 0x6386))
	return 1;
    else
	return 0;
    endif
endfunction

