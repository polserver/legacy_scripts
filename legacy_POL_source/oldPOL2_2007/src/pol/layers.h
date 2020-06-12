#ifndef __LAYERS_H
#define __LAYERS_H

// layers 1 .. 25 exist.
#define LOWEST_LAYER    1
#define HIGHEST_LAYER   25
#define NUM_LAYERS      25

// FIXME - figure out which is left and right
#define LAYER_EQUIP__LOWEST 1
#define LAYER_HAND1			1
#define LAYER_HAND2			2
#define LAYER_SHOES			3
#define LAYER_PANTS			4
#define LAYER_SHIRT			5
#define LAYER_HAT			6
#define LAYER_GLOVES		7
#define LAYER_RING			8
#define LAYER_TALISMAN      9
#define LAYER_GORGET		10
#define LAYER_HAIR			11
#define LAYER_HALFAPRON		12
#define LAYER_CHEST_ARMOR	13
#define LAYER_BRACELET		14
#define LAYER_BACKPACK_WTF	15
#define LAYER_BEARD			16
#define LAYER_TUNIC			17
#define LAYER_EARRINGS		18
#define LAYER_ARM_ARMOR		19
#define LAYER_CLOAK			20
#define LAYER_BACKPACK		21
#define LAYER_ROBE_DRESS	22
#define LAYER_SKIRT			23
#define LAYER_LEG_ARMOR		24
#define LAYER_MOUNT         25
#define LAYER_EQUIP__HIGHEST 25

#define LAYER_BANKBOX       29
/*
layering.  'worn items' for a character seem to occupy different
'layer' locations.  I've seen the following:
0x01 1 dagger (right-hand?)
0x02 2 left hand - staffs, pitchfork, crook etc.
0x03 3 shoes
0x04 4 pants
0x05 5 shirt
0x06 6 hat
0x07 7 gloves
0x08 8 ring?
0x09 9 talismans
0x0A 10 gorget
0x0B 11 hair
0x0C 12 half-apron
0x0D 13 (armor)tunic/chest armor, belt pouch (?)
0x0E 14 bracelet
0x0F 15 backpack (wtf? the "real" backpack sits in layer 21)
0x10 16 beard
0x11 17 full apron, body sash, doublet, jester suit, tunic, surcoat
0x12 18 earrings
0x13 19 arms
0x14 20 cloak
0x15 21 backpack
0x16 22 dresses, robes
0x17 23 skirt, kilt
0x18 24 platemail legs
0x19 25 horse?
0x1A 26 unknown   - temporary storage for backpack??
0x1B 27 unknown
0x1C 28 unknown
0x1D 29 bankbox
0x1E 30 unknown
0x1F 31 unknown

 

I assume that there can only be a single item in each layer.
Note that for the most part, the code doesn't need to know this.  The
data is in tiledata.mul (we don't check this yet), and the client 
makes sure to drop items only in the correct layer (again, server 
needs to check this).  The only spcial case is the backpack - 
Character::backpack() uses LAYER_BACKPACK to return the backpack item.
*/

#endif
