use cfgfile;
use os;
use util;

//
//  See starteqp.cfg for a description of what we expect in the config file.
//

var unique_items_created := {};
var starteqp_cfg;
//var template;
var magicspawnchance := 0;
var making_starting_equipment := 0;

function MakeLoot( corpse , lootgroup )
    local npcdesc := ReadConfigFile( "::npcdesc" );
    local elem := FindConfigElem( npcdesc, GetObjProperty( corpse, "npctemplate" ) );

    local mprop := GetConfigInt( elem , "Magicitemchance" );
    if( mprop > 0 )
       magicspawnchance := mprop;
    else
       magicspawnchance := 0;
    endif

    if ( (lootgroup >= 5) and (lootgroup <= 9) )
       magicspawnchance := 80+lootgroup;
print(magicspawnchance);
    endif

    if ( (lootgroup >= 109) and (lootgroup <= 119) )
       magicspawnchance := 1+(lootgroup-110);
    endif

    local lootgroups := GetConfigStringArray( elem, "lootgroup" );
    starteqp_cfg := ReadConfigFile( "::nlootgroup" );

    if( lootgroup )
        CreateFromGroup( corpse , lootgroup );
    else
        foreach str in lootgroups
            CreateFromGroup( corpse , cint(str) );
        endforeach
    endif
endfunction


function CreateStartingEquipment( who, skillids )
    making_starting_equipment := 1;
    starteqp_cfg := ReadConfigFile( "::starteqp" );
    foreach skillid in skillids
        var skillname := GetSkillName( CInt(skillid) );

        CreateFromGroup( who.backpack, skillname );
    endforeach
        CreateFromGroup(who.backpack, "all");
endfunction


function CreateFromGroup( who, group )
    var elem := starteqp_cfg[ group ];

    foreach str in GetConfigStringArray( elem, "Stack" )
        CreateFromStackString( who, str );
    endforeach

    foreach str in GetConfigStringArray( elem, "Item" )
        CreateFromItemString( who, str );
    endforeach

    foreach str in GetConfigStringArray( elem, "Random" )
        CreateFromRandomString( who, str );
    endforeach

    foreach str in GetConfigStringArray( elem, "Group" )
        CreateFromGroup( who, str );
    endforeach

    foreach str in GetConfigStringArray( elem, "Unique" )
        CreateFromUniqueString( who, str );
    endforeach
endfunction

function GetSkillName( skillid )
    local cfg := ReadConfigFile( "::skills" );
    return cfg[ skillid ].name;
endfunction


/////////////////////////////////////////////////////////////////////////////
//
//  An 'Item' string can have one of the following forms:
//
//  FORM                                    EXAMPLE
//  --------------------------------------------------------------------
//  "Item [ObjTypeName]"                    "Item Mortar"
//  "Item [Count] [ObjTypeName]"            "Item 4 EmptyBottle"
//  "Item [ObjTypeName] [Colornum]"         "Item Robe 0xE9"
//  "Item [Count] [ObjTypeName] [Colornum]"  "Item 2 Robe 0xE9"
//  "item [ObjTypeName] [Colornum] [chancetospawn]  "item Longsword 0 100"
//  "Item [Count] [ObjTypeName] [Colornum] [chancetospawn]  "item 1 Longsword 0 100"
//
//Item 1 level1map 0 1
//
/////////////////////////////////////////////////////////////////////////////

function CreateFromItemString( who, str )
    var params := SplitWords( str );
    var count;
    var objname;
    var color;
    var chance;
    if ( RandomDiceRoll( params[1] ) )
        count := RandomDiceRoll( params[1] );
        params[1] := params[2];
        params[2] := params[3];
        params[3] := params[4];
    else
        count := 1;
    endif
    objname := params[1];

    if ( CInt(params[2]) )
        color := CInt( params[2] );
    else
        color := 0;
    endif

    if ( CInt(params[3]) )
        chance := CInt( params[3] );
    else
        chance := 100;
    endif

    //syslog(   "count=" + count + ", objname=" + objname + ", chance=" + chance );

  if( cint(RandomInt(100)+1) <= cint(chance) )

    var i;
    var canbemagic;
    for( i := 1; i <= count; i := i + 1 )

        var item := CreateItemInContainer( who, objname );
        item.color := color;
        if(making_starting_equipment)
           item.newbie := 1;
        endif

      if( RandomInt(100)+1 <= magicspawnchance )

        canbemagic := MagicAllowedandType( objname ); //for magic items!
        case (canbemagic)
           1: case (RandomInt(4)+1)       //weapon
                1: ApplyDmgMod( item );
                2: ApplyHPMod( item ); 
                3: SilverMod( item );
                4: ApplyAccMod( item );
              endcase
              AddName( item );
           2: case (RandomInt(2)+1)       //armor
                1: ApplyARMod( item );
                2: ApplyHPMod( item );
              endcase
              AddName( item );
           default:
        endcase
      endif

    endfor
  endif

endfunction

/////////////////////////////////////////////////////////////////////////////
//
//  A 'Random' string follows the following form:
//
//  FORM                                    EXAMPLE
//  --------------------------------------------------------------------
//  "Random [Count] [GroupName]"            "Random 4 Reagents"
//  "Random [Count] [GroupName] [chancetospawn]"   "Random 4 Reagents 50"
//
//  'Count' entries from 'GroupName' are chosen randomly.  The same entry
//  will not be chosen twice.
//
/////////////////////////////////////////////////////////////////////////////


function CreateFromRandomString( who, desc )

    var params := SplitWords( desc );
    var count := RandomDiceRoll( params[1] );
    var group := params[2];
    var chance := params[3];
    //syslog( "count=" + count + ", group=" + group + ", chance=" + chance);
    
    var elem := FindConfigElem( starteqp_cfg , group);
    if (elem = error) 
//        syslog( "Unable to find group " + group );
        return;
    endif
    
    var items := GetConfigStringArray( elem, "Item" );
    if (items = error) 
//        syslog( "Group " + group + " does not contain items!" );
        return; 
    endif

    if ( !chance )
        chance := 100;
    endif

    // print( "count=" + count + ", objname=" + objname + ", color=" + color );
    local rand := RandomInt(100)+1;

    if( cint(rand) <= cint(chance) )

    if( count > len(items))
        count := len(items);
    endif

        while (count)
            local n := RandomInt( len(items) ) + 1;
            if (items[n])
                CreateFromItemString( who, items[n] );

                if(making_starting_equipment)
                   items[n] := "";
                endif

                count := count - 1;
            endif
        endwhile
    endif
endfunction


/////////////////////////////////////////////////////////////////////////////
//
//  A 'Unique' string has the following form:
//
//  FORM                                    EXAMPLE
//  --------------------------------------------------------------------
//  "Unique [ObjTypeName]"                  "Unique Spellbook"
//
//  Only one of a unique item will be created as starting equipment.
//
/////////////////////////////////////////////////////////////////////////////

function CreateFromUniqueString( who, str )
    var objtype := GetObjtypeByName( str );
    if (!objtype)
//        syslog( "Don't know what a " + str + " is" );
        return;
    endif
        
    if (objtype in unique_items_created)
        return;
    endif

    unique_items_created[ len(unique_items_created) + 1 ] := objtype;
    local uitem := CreateItemInContainer( who, objtype );
    if(making_starting_equipment)
           uitem.newbie := 1;
    endif
endfunction


/////////////////////////////////////////////////////////////////////////////
//
//  A 'Stack' string is just like "Item" but stacks the item instead of making
//  count number of individual items.
//
/////////////////////////////////////////////////////////////////////////////

function CreateFromStackString( who, str )
    var params := SplitWords( str );
    var count;
    var objname;
    var color;
    var chance;
    if ( RandomDiceRoll( params[1] ) )
        count := RandomDiceRoll( params[1] );
        params[1] := params[2];
        params[2] := params[3];
        params[3] := params[4];
    else
        count := 1;
    endif
    objname := params[1];
    if ( CInt(params[2]) )
        color := CInt( params[2] );
    else
        color := 0;
    endif

    if ( CInt(params[3]) )
        chance := CInt( params[3] );
    else
        chance := 100;
    endif

    // print( "count=" + count + ", objname=" + objname + ", color=" + color );
    local rand := RandomInt(100)+1;

  if( cint(rand) <= cint(chance) )

        var item := CreateItemInContainer( who, objname, count );
        if (!item)
//            syslog( "Unable to create " + objname );
        endif
        // print( "Created: " + item.desc );
        item.color := color;
        if(making_starting_equipment)
           item.newbie := 1;
        endif
  endif

endfunction


////
//Checks to see if that item is allowed to be magic and returns the type
////
function MagicAllowedandType( objname )

   local magicweapcfg := ReadConfigFile( ":combat:magicweaps" );
   local magicarmorcfg := ReadConfigFile( ":combat:magicarmor" );
   
   local elem1 := FindConfigElem( magicweapcfg, "1" );
   local data1 := GetConfigInt( elem1 , objname );

   if( data1 )
      return 1;
   endif

   local elem2 := FindConfigElem( magicarmorcfg, "1" );
   local data2 := GetConfigInt( elem2 , objname );

   if( data2 )
      return 2;
   endif

   return 0;

endfunction


////
//Picks and assigns a damagemod and a suffix for the item
////
function ApplyDmgMod( item )

   var numb := RandomInt(100)+1;
   var pricemod := 0;

   if( numb >= 50 )
       item.dmg_mod := 1;
       SetObjProperty( item , "suffix", "of Ruin" );
       pricemod := 500;
   elseif( numb >= 25 )
       item.dmg_mod := 3;
       SetObjProperty( item , "suffix", "of Might" );
       pricemod := 1000;
   elseif( numb >= 15 )
       item.dmg_mod := 5;
       SetObjProperty( item , "suffix", "of Force" );
       pricemod := 1500;
   elseif( numb >= 5 )
       item.dmg_mod := 7;
       SetObjProperty( item , "suffix", "of Power" );
       pricemod := 3000;
   elseif( numb >= 1 )
       item.dmg_mod := 9;
       SetObjProperty( item , "suffix", "of Vanqusihing" );
       pricemod := 4000;
   endif

   item.buyprice := item.buyprice + pricemod;
print(item.buyprice);
   if( RandomInt(100)+1 <= 10 )
       ApplyHPMod( item );
   endif

endfunction


////
//Picks and assigns a new AR and prefix for the item
////
function ApplyHPMod( item )

   var numb := RandomInt(100)+1;
   var hp := 0;
   var pricemod := 0;

   if( numb >= 50 )
       item.quality := 1.2;
	   hp := 10;
       SetObjProperty( item , "prefix", "Durable" );
       pricemod := 100;
   elseif( numb >= 25 )
       item.quality := 1.2;
	   hp := 20;
       SetObjProperty( item , "prefix", "Substantial" );
       pricemod := 200;
   elseif( numb >= 15 )
       item.quality := 1.2;
	   hp := 30;
       SetObjProperty( item , "prefix", "Massive" );
       pricemod := 400;
   elseif( numb >= 5 )
       item.quality := 1.2;
	   hp := 40;
       SetObjProperty( item , "prefix", "Fortified" );
       pricemod := 600;
   elseif( numb >= 1 )
       item.quality := 1.2;
	   hp := 50;
       SetObjProperty( item , "prefix", "Indestructable" );
       pricemod := 1000;
   endif
   
   item.buyprice := item.buyprice + pricemod;
print(item.buyprice);
   item.hp_mod := hp;
   item.maxhp_mod := hp;
   item.hp := item.maxhp;

endfunction

////
//Picks a AR mod and suffix for the item
////
function ApplyARMod( item )

   var numb := RandomInt(100)+1;
   var pricemod := 0;
   
   if( numb >= 50 )
       item.ar_mod := 1;
       SetObjProperty( item , "suffix", "of Defense" );
       pricemod := 250;
   elseif( numb >= 25 )
       item.ar_mod := 2;
       SetObjProperty( item , "suffix", "of Guarding" );
       pricemod := 400;
   elseif( numb >= 15 )
       item.ar_mod := 4;
       SetObjProperty( item , "suffix", "of Protection" );
       pricemod := 750;
   elseif( numb >= 5 )
       item.ar_mod := 6;
       SetObjProperty( item , "suffix", "of Fortification" );
       pricemod := 1000;
   elseif( numb >= 1 )
       item.ar_mod := 9;
       SetObjProperty( item , "suffix", "of Invulnerability" );
       pricemod := 1500;
   endif

   item.buyprice := item.buyprice + pricemod;
print(item.buyprice);
   if( RandomInt(100)+1 <= 10 )
       ApplyHPMod( item );
   endif

endfunction

//
// Adds the silverhit script to the weapon's hitscripts
//
function SilverMod( item )

	var num := RandomInt(100)+1;
	if( num <= 100 )
		var hits := GetObjProperty(item,"OnHit");
		if(hits)
			hits.append("silverhit");
			SetObjProperty(item,"OnHit",hits);
		else
			SetObjProperty(item,"OnHit",{"silverhit"});
		endif
		var prefix := GetObjProperty(item,"prefix");
		if(prefix)
			prefix := "Silver, " + prefix;
			SetObjProperty(item,"prefix",prefix);
		else
			SetObjProperty(item,"prefix","Silver");
		endif
	endif
	
	item.buyprice := item.buyprice + 3000;
	print(item.buyprice);
	var randnum := RandomInt(100)+1;
	if( randnum <= 10 )
		ApplyHPMod( item );
	elseif( randnum <= 20 )
		ApplyDmgMod( item );
	endif
	
endfunction

////
//Picks a Accuracy mod and suffix for the item
////
function ApplyAccMod( item )

   var numb := RandomInt(100)+1;
   var currpref;
   var pref;
   var pricemod := 0;

   if( numb >= 50 )
       SetObjProperty( item , "skill_adj" , 5 );
       pref := "Accurate";
       pricemod := 300;
   elseif( numb >= 25 )
       SetObjProperty( item , "skill_adj" , 10 );
       pref := "Surpassingly Accurate";
       pricemod := 500;
   elseif( numb >= 15 )
       SetObjProperty( item , "skill_adj" , 15 );
       pref := "Eminately Accurate";
       pricemod := 900;
   elseif( numb >= 5 )
       SetObjProperty( item , "skill_adj" , 20 );
       pref := "Exceedingly Accurate";
       pricemod := 1300;
   elseif( numb >= 1 )
       SetObjProperty( item , "skill_adj" , 25 );
       pref := "Supremely Accurate";
       pricemod := 2000;
   endif

   currpref := GetObjProperty(item,"prefix");
   if(currpref != error)
      currpref := currpref + ", " + pref;
      SetObjProperty(item,"prefix",currpref);
   else
   	  SetObjProperty(item,"prefix",pref);
   endif

	item.buyprice := item.buyprice + pricemod;

	var randnum := RandomInt(100)+1;
	if( randnum <= 10 )
		ApplyHPMod( item );
	elseif( randnum <= 20 )
		ApplyDmgMod( item );
	elseif( randnum <= 30 )
		SilverMod( item );
	endif

endfunction

////
//Adds "a magic " to the beginning of the item's desc.
////
function AddName( item )

   local currdesc := item.desc;
   if( currdesc[1] = "a" )
      local space := find( item.desc, " ", 1 );
      local newname := currdesc[space+1,len(item.desc)];
      SetName( item , "a magic " + newname );
   else
      SetName( item , "magic " + currdesc );
   endif
endfunction
