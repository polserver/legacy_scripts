// TUSPortTools
//  Tools used to help port TUS Data to POL

include "TUSPortStorage"; // These are the storage functions tools needs to run
include "include/math";

const MAX_CONTAINER_SIZE := 100;

global intCurAvailSerial;
global intNextAvailKey;

// Purpose: Converts the TUS accounts to POL accounts
// Parameters: None
// Return: Nothing
function PortAccounts()
  var tusfile := LoadTusScpFile( "tusaccu" ); // Is it this file?
  var AdTest := 0;

  if (!tusfile)
    print("Unable to load: " + tusfile.errortext );
    return;
  endif


  var n := GetConfigMaxIntKey( tusfile );
  var i;
  for( i := 1; i <= n; i := i + 1 )
   var acct := tusfile[i];
   var props array;
   props[1] := {"Name" , acct._key};
   props[2] := {"Password" , acct.password};
   if (acct._key == "admin")
     AdTest := 1;
   endif

   AppendConfigFileElem( "pol_accounts", "Account", "", props );
  endfor

  if (adTest != 1)

   var props array;
   props[1] := {"Name" , "admin"};
   props[2] := {"Password" , "admin"};

   AppendConfigFileElem( "pol_accounts", "Account", "", props );
  endif


endfunction

// Purpose: Converts the object to an Item
// Parameters: TUSObject - The object to convert
// Return: Nothing
function PortItem( TUSObject )
  var POLItem array; // Array of properties to the new POL Item to be created
  var i; // Index on the array of where we wish to place the current data
  var ExportFile; // The file we wish to export this data to.
  var bpserial; // The serial to master backpack

  i := 1;

  // Skip Spawns
  if (IsSpawn(TUSObject))
    return;
  endif

  // Skip Telepads (for now)
  if (IsTelepad(TUSObject))
    return;
  endif

  // Skip Memory Objects
  if (IsMemObj(TUSObject))
    return;
  endif

  // Set the items Serial Number <No need to test if exists because everything SHOULD have a Serial>
  POLItem[i] := {"Serial", hextotxt(TUSObject.SERIAL)};
  i := i + 1; // Increment "i" after every use.

  // Store the serial number, it may be a container and we may have to refer to it"s id later.

  // Is the object a standard object
  if (TUSObject.worlditem < UOBJ_MAX_ITEM_ID)
    // Set the ObjType and Graphic to be the same thing
    if ((IsDoor(TUSObject.worlditem)) && (TUSObject.ID != error))
      POLItem[i] := {"ObjType", HexToTxt(ConvDoor(TUSObject.ID))};
    elseif (IsKeyRing(TUSObject.worlditem))
      POLItem[i] := {"ObjType", "0x1011"}; // Set to type Keyring
    elseif ((IsHouseSign(TUSObject)) && (IsComponent( TUSObject )))
      POLItem[i] := {"ObjType", HexToTxt(TUSObject.ID)};// Sign's graphic is the type
    else
      POLItem[i] := {"ObjType", HexToTxt(TUSObject.worlditem)};// Set the ObjType
    endif
    i := i + 1;

    // Set the Graphic
    if (!IsDoor(TUSObject.worlditem))
      if (TUSObject.ID != error)
        POLItem[i] := {"Graphic", HexToTxt(TUSObject.ID)}; // if available, this is the official ID
      else
        POLItem[i] := {"Graphic", HexToTxt(TUSObject.worlditem)};// Set the Graphic
      endif
    endif
    i := i + 1;
  else
    // The object is a custom object
    // This is a bit of a hack for now.  Needs refining to match truly custom Items - Sentack 9/15/1999
    POLItem[i] := {"ObjType", HexToTxt(TUSObject.ID)}; // The ID is the official Object Type
    i := i + 1;
    POLItem[i] := {"Graphic", HexToTxt(TUSObject.ID)}; // The ID is the Graphic.
    i := i + 1;
  endif

  // Color
  if (TUSObject.COLOR != error)
    POLItem[i] := {"Color", HexToTxt(TUSObject.COLOR)};
    i := i+1;
  endif

  // Name
  if (TUSObject.NAME != error)
    POLItem[i] := {"Name", TUSObject.NAME};
    i := i+1;
  endif

  // Location
  if (TUSObject.P != error)
    // Set the location of the object
    var x := TUSObject.P; // x is now "x,y,z"
    x[","] := " "; // x is now "x y,z";
    x[","] := " "; // x is now "x y z";
    x := splitwords( x ); // x is now an array { "x", "y", "z" }

    POLItem[i] := {"X", x[1]};
    i := i + 1;
    POLItem[i] := {"Y", x[2]};
    i := i + 1;
    POLItem[i] := {"Z", x[3]};
    i := i + 1;
  else
    if (IsOnPlayer(TUSObject) && !(IsFacial(TUSObject.worlditem) || (TUSObject.worlditem == 0x0e75)))
      // This is an item that was on a players body. I'm just going to put it in the backpack for now at this location.  No good reason why this location but this location.
      POLItem[i] := {"X", 0x32};
      i := i + 1;
      POLItem[i] := {"Y", 0x46};
      i := i + 1;
      POLItem[i] := {"Z", 0};
      i := i + 1;
    else
      // Note, everything has to have an X,Y,Z location.  even if it's all 0
      POLItem[i] := {"X", 0};
      i := i + 1;
      POLItem[i] := {"Y", 0};
      i := i + 1;
      POLItem[i] := {"Z", 0};
      i := i + 1;
    endif
  endif

  // Amount
  if (TUSObject.AMOUNT != error)
    POLItem[i] := {"Amount", HextoInt(TUSObject.AMOUNT)};
    i := i+1;
  endif

  if ((IsKey(TUSObject.worlditem) || IsDoor(TUSObject.worlditem) || IsContainer(TUSObject.worlditem)) && (TUSObject.MORE1 != error))
    var intNewKeyID;

    intNewKeyID := FindKeyID(TUSObject.MORE1);

    if (!intNewKeyID)
      intNewKeyID := GetAvailKey();
      StoreKeyID(TUSObject.MORE1, intNewKeyID);
    endif

    POLItem[i] := {"CProp", "lockid  i" + CStr(intNewKeyID)};
    i := i+1;

  endif

  // is the object contained in something
  if (TUSObject.CONT != error)
    // Find the new ID for the container that holds this object
    //print("  My Container is " + HexToTxt(TUSObject.CONT));
    if (IsOnPlayer(TUSObject) && !(IsFacial(TUSObject.worlditem) || (TUSObject.worlditem == 0x0e75)))
      // Find New Backpack
      //print("Putting item " + hextotxt(TUSObject.worlditem) + " in backpack " + hextotxt(FindMasterBackpack(TUSobject.CONT)));
      POLItem[i] := {"Container", HextoTxt(FindMasterBackpack(TUSobject.CONT))};
      i := i+1;
    elseif (IsKey(TUSObject.worlditem) && InOnKeyRing(TUSObject.CONT))
      var KeyRingCont := InOnKeyRing(TUSObject.CONT);
      if (KeyRingCont == 1) // warning!
      else
        POLItem[i] := {"Container", HextoTxt(KeyRingCont)};
        i := i+1;
      endif
    else
      POLItem[i] := {"Container", HextoTxt(TUSObject.CONT)};
      i := i+1;
    endif
  endif

  // if the object has any Attributes at all then test of the following
  if (TUSObject.ATTR != error)

    // Moveable - Is the object moveable
    if (IsMobile( TUSObject ))
      POLItem[i] := {"Movable", "1"};
      i := i+1;
    elseif  (IsImmobile( TUSObject ))
      // Immobile - Item can not be moved
      POLItem[i] := {"Movable", "0"};
      i := i+1;
    endif

    // Newbie - Is this a Newbie object?
    if (IsNewbied( TUSObject ))
      POLItem[i] := {"Newbie", "1"};
      i := i+1;
    endif

    // DecayAt - How long does the object last before it decays
    // Set only if an object has the attribute, Decay.
    if ((TUSObject.TIMER != error) and (IsDecaying( TUSObject )))
      POLItem[i] := {"DecayAt", HextoInt(TUSObject.TIMER)};
      i := i+1;
    endif

  endif

  if( IsMarkedRune(TUSObject ) )
    // print("Porting Rune");

    var x := TUSObject.MOREP; // x is now "x,y,z"
    x[","] := " "; // x is now "x y,z";
    x[","] := " "; // x is now "x y z";
    x := splitwords( x ); // x is now an array { "x", "y", "z" }

    POLItem[i] := {"Cprop", "x i" + x[1]};
    i := i + 1;
    POLItem[i] := {"Cprop", "y i" + x[2]};
    i := i + 1;
    POLItem[i] := {"Cprop", "z i" + x[3]};
    i := i + 1;
  endif

  // Test to see if object is locked
  if (IsLocked(TUSObject))
    POLItem[i] := {"Locked", "1"};
    i := i+1;
  endif

  // Check to see if Object requires a layre lable

  // Testing for beard or Head hair
  if (IsFacial(TUSObject.worlditem))
    if (IsBeard(TUSObject.worlditem))
      POLItem[i] := {"Layer", LAYER_BEARD};
      i := i+1;
    elseif (IsHeadHair(TUSObject.worlditem))
      POLItem[i] := {"Layer", LAYER_HAIR};
      i := i+1;
    else
      // Error: unknown face object type
      print("ERROR: Unknown face object found.");
    endif
  endif

  // Test for Master Backpack
  if ((TUSObject.worlditem == 0x0e75) && (IsOnPlayer(TUSObject)))
    POLItem[i] := {"Layer", LAYER_PACK};
    i := i+1;
  endif

  if (IsComponent(TUSObject))
    POLItem[i] := {"CProp", "house_serial i" + TUSObject.LINK};
    i := i+1;

    if (IsHouseSign(TUSObject))
      POLItem[i] := {"CProp", " lastownername sAdmin"};
      i := i+1;

      var intNewKeyID;

      intNewKeyID := FindKeyID(TUSObject.LINK);

      if (!intNewKeyID)
        intNewKeyID := GetAvailKey();
        StoreKeyID(TUSObject.LINK, intNewKeyID);
      endif

      POLItem[i] := {"CProp", "lockid  i" + CStr(intNewKeyID)};
      i := i+1;

    endif

    StoreComponent(TUSObject.LINK, TUSObject.SERIAL);

  endif


  // Now determine where exactly this item will go.
  if (TUSObject.CONT != error)
    if (IsOnPlayer(TUSObject)) // If on a player
      if (TUSObject.LAYER == LAYER_BANKBOX) // If this is object is the Bank Box
        ExportFile := "pol_storage"; // It's the bank box
        StoreBankBoxes( TUSObject.SERIAL, TUSObject.CONT );
      elseif (IsFacial(TUSObject.worlditem))
        ExportFile := "pol_pcs";
      elseif ((TUSObject.worlditem == 0x0e75) && (IsOnPlayer(TUSObject))) // MasterBackpack
        ExportFile := "pol_pcs";
        StorePlayerBag(TUSObject.SERIAL, TUSObject.CONT);
      else
        // The items is something the player was wearing.  have it contained in the players master backpack
        ExportFile := "pol_pcequip";
      endif
    else
      if (InPlayerBag(TUSObject.CONT)) // If its in a PC's container
        ExportFile := "pol_pcequip";
        if (IsContainer(TUSObject.worlditem))
          StorePlayerBag(TUSObject.SERIAL, TUSObject.CONT);
        endif
      elseif (InBankBoxes(TUSObject.CONT))
        ExportFile := "pol_storage"; // It goes in the bank box
        if (IsContainer(TUSObject.worlditem))
          StoreBankBoxes(TUSObject.SERIAL, TUSObject.CONT);
        endif
      elseif (IsKey(TUSObject.worlditem) && InOnKeyRing(TUSObject.CONT)) // Item is on a keyring.
        ExportFile := "pol_items";
      else
        ExportFile := "pol_items";
      endif
    endif
  else
    ExportFile := "pol_items";
  endif

  // "Items" is the type, and elemkey can be ignored mostly
  AppendConfigFileElem( ExportFile, "Item", "",  POLItem);  // Last argument has to be an array of structs.

  if (TUSObject.worlditem == UOBJ_SPELLBOOK)
    //print("Createing Spellbook entries");
    CreateSpellBookEntries( TUSObject, ExportFile );
  endif

  if (IsKeyRing(TUSObject.worlditem))
    CreateKeyBankBox( TUSObject );
  endif

endfunction

// Purpose: Converts the object to an Player file
// Parameters: TUSObject - The object to convert
// Return: Nothing
function PortPC( TUSObject )

  var cmdlevel;  // The command level we are transfering over
  var PLevel;    // TUS's PLevel
  var x, y, z;   // The Location of the Player we are transfering over
  var CharNumber; // The characters number for this account
  var pos;
  var str, intt, dex;
  //var TUSObject;
  var MaxItems;
  var CurrItem;
  var gend;
  var chridx;

  Print("Opening TUS Account File...");
  var filename2 := "tusaccu";
  var tusacct := LoadTUSScpFile( filename2 ); // Open TUS account files

  if (!tusacct)
    //SendSysMessage( who, "Unable to load: " + filename2 );
    Print("Unable to load TUS account file: " + filename2 + ".scp" );
    Print("Reason: " + tusacct.errortext );
    return;
  endif

  str  := BaseSkillToRawSkill(HextoInt(TUSObject.str)*10);                   //these cannot use the BaseHextoRawDec() function
  intt := BaseSkillToRawSkill(HextoInt(GetConfigInt(TUSObject, "int"))*10);  //since the int values must be multiplied by 10
  dex  := BaseSkillToRawSkill(HextoInt(TUSObject.dex)*10);                   //before getting converted to raw skill.

  // TUS doesn't store genders, but I can figure it out from the model.
  // Note, some worlds have players as deamons or dragons, etc.  We can't
  //  guess by the model but we have to set it anyway.
  if( TUSObject.obody = 0x190 )
    gend := 0; // Male
  else
    gend := 1; // Female
  endif

  // Get and convert the X,Y,Z
  pos := TUSObject.P;
  pos[","] := " "; // x is now "x y,z";
  pos[","] := " "; // x is now "x y z";
  pos := splitwords( pos ); // x is now an array { "x", "y", "z" }
  x := pos[1];
  y := pos[2];
  z := pos[3];

  PLevel := tusacct[TUSObject.account].plevel;
//  print("Plevel = " + PLevel);

  if (PLevel != error)
    // I'm makeing grand assumptions here, but they should be good
    case (PLevel)
      1 : cmdlevel := "player";
      2 : cmdlevel := "coun";
      3 : cmdlevel := "seer";
      4 : cmdlevel := "gm";
      5 : cmdlevel := "seer";
      6 : cmdlevel := "admin";
    endcase
  else
    cmdlevel := "player";
  endif

  //TUS doesn't store the chridx, so we have to keep track of how
  // NOTE: Quite cleaver I must admit, but what of a large world file?
  chridx := GetGlobalProperty( "#" + TUSObject.account );
  if( chridx = error )                                //many chars we've seen for this account.
    SetGlobalProperty( "#" + TUSObject.account , 0 ); //next char idx for this account
    CharNumber := 0;
  else
    CharNumber := CInt(chridx);
  endif
  SetGlobalProperty( "#" + TUSObject.account , CharNumber + 1 );

  // NOTE: Still have to translate flags....


  // This will hold the properties we wish to edit
  var props array;

  props[1]  := {"Account" ,               TUSObject.account};
  props[2]  := {"CharIdx" ,               CharNumber};
  props[3]  := {"Name",                   TUSObject.name};
  props[4]  := {"Serial",                 HexToTxt(TUSObject.serial)};
  props[5]  := {"Objtype",                HexToTxt(TUSObject.worldchar)};
  props[6]  := {"Graphic",                HexToTxt(TUSObject.obody)};
  if (TUSObject.color != error)
    props[7]  := {"Color",                HexToTxt(TUSObject.color)};
  else
    props[7]  := {"Color",                "0x0"};
  endif
  props[8]  := {"X",                      x};
  props[9]  := {"Y",                      y};
  props[10] := {"Z",                      z};
  props[11] := {"Facing",                 0};
  props[12] := {"Cmdlevel",               cmdlevel};
  props[13] := {"TrueColor",              HexToTxt(TUSObject.oskin)};
  props[14] := {"TrueObjtype",            HexToTxt(TUSObject.worldchar)};
  props[15] := {"Gender",                 gend};
  props[16] := {"STR",                    str};
  props[17] := {"INT",                    intt};
  props[18] := {"DEX",                    dex};
  props[19] := {"HITS",                   HextoInt(TUSObject.hitpoints)};
  props[20] := {"MANA",                   HextoInt(TUSObject.mana)};
  props[21] := {"STAM",                   HextoInt(TUSObject.stamina)};
  props[22] := {"CreatedAt",              0};
  props[23] := {"Alchemy",                BaseHextoRawDec(TUSObject.alchemy)};
  props[24] := {"Anatomy",                BaseHextoRawDec(TUSObject.Anatomy)};
  props[25] := {"AnimalLore",             BaseHextoRawDec(TUSObject.AnimalLore)};
  props[26] := {"ItemId",                 BaseHextoRawDec(TUSObject.ItemId)};
  props[27] := {"ArmsLore",               BaseHextoRawDec(TUSObject.ArmsLore)};
  props[28] := {"Parry",                  BaseHextoRawDec(TUSObject.Parrying)};
  props[29] := {"Begging",                BaseHextoRawDec(TUSObject.Begging)};
  props[30] := {"Blacksmithy",            BaseHextoRawDec(TUSObject.Blacksmithing)};
  props[31] := {"Bowcraft",               BaseHextoRawDec(TUSObject.Bowcraft)};
  props[32] := {"Peacemaking",            BaseHextoRawDec(TUSObject.Peacemaking)};
  props[33] := {"Camping",                BaseHextoRawDec(TUSObject.Camping)};
  props[34] := {"Carpentry",              BaseHextoRawDec(TUSObject.Carpentry)};
  props[35] := {"Cartography",            BaseHextoRawDec(TUSObject.Cartography)};
  props[36] := {"Cooking",                BaseHextoRawDec(TUSObject.Cooking)};
  props[37] := {"DetectingHidden",        BaseHextoRawDec(TUSObject.DetectingHidden)};
  props[38] := {"Enticement",             BaseHextoRawDec(TUSObject.Enticement)};
  props[39] := {"EvaluatingIntelligence", BaseHextoRawDec(TUSObject.EvaluatingIntel)};
  props[40] := {"Healing",                BaseHextoRawDec(TUSObject.Healing)};
  props[41] := {"Fishing",                BaseHextoRawDec(TUSObject.Fishing)};
  props[42] := {"ForensicEvaluation",     BaseHextoRawDec(TUSObject.Forensics)};
  props[43] := {"Herding",                BaseHextoRawDec(TUSObject.Herding)};
  props[44] := {"Hiding",                 BaseHextoRawDec(TUSObject.Hiding)};
  props[45] := {"Provocation",            BaseHextoRawDec(TUSObject.Provocation)};
  props[46] := {"Inscription",            BaseHextoRawDec(TUSObject.Inscription)};
  props[47] := {"Lockpicking",            BaseHextoRawDec(TUSObject.Lockpicking)};
  props[48] := {"Magery",                 BaseHextoRawDec(TUSObject.Magery)};
  props[49] := {"MagicResistance",        BaseHextoRawDec(TUSObject.MagicResistance)};
  props[50] := {"Tactics",                BaseHextoRawDec(TUSObject.Tactics)};
  props[51] := {"Snooping",               BaseHextoRawDec(TUSObject.Snooping)};
  props[52] := {"Musicianship",           BaseHextoRawDec(TUSObject.Musicianship)};
  props[53] := {"Poisoning",              BaseHextoRawDec(TUSObject.Poisoning)};
  props[54] := {"Archery",                BaseHextoRawDec(TUSObject.Archery)};
  props[55] := {"SpiritSpeak",            BaseHextoRawDec(TUSObject.SpiritSpeak)};
  props[56] := {"Stealing",               BaseHextoRawDec(TUSObject.Stealing)};
  props[57] := {"Tailoring",              BaseHextoRawDec(TUSObject.Tailoring)};
  props[58] := {"AnimalTaming",           BaseHextoRawDec(TUSObject.Taming)};
  props[59] := {"TasteIdentification",    BaseHextoRawDec(TUSObject.TasteID)};
  props[60] := {"Tinkering",              BaseHextoRawDec(TUSObject.Tinkering)};
  props[61] := {"Tracking",               BaseHextoRawDec(TUSObject.Tracking)};
  props[62] := {"Veterinary",             BaseHextoRawDec(TUSObject.Veterinary)};
  props[63] := {"Swordsmanship",          BaseHextoRawDec(TUSObject.Swordsmanship)};
  props[64] := {"Macefighting",           BaseHextoRawDec(TUSObject.Macefighting)};
  props[65] := {"Fencing",                BaseHextoRawDec(TUSObject.Fencing)};
  props[66] := {"Wrestling",              BaseHextoRawDec(TUSObject.Wrestling)};
  props[67] := {"Lumberjacking",          BaseHextoRawDec(TUSObject.Lumberjacking)};
  props[68] := {"Mining",                 BaseHextoRawDec(TUSObject.Mining)};
  props[69] := {"Meditation",             BaseHextoRawDec(TUSObject.Meditation)};
  props[70] := {"Stealth",                BaseHextoRawDec(TUSObject.Stealth)};
  props[71] := {"RemoveTrap",             BaseHextoRawDec(TUSObject.RemoveTrap)};

  AppendConfigFileElem( "pol_pcs", "Character", "", props );


endfunction

// Purpose: Converts the object to an Multi object
// Parameters: TUSObject - The object to convert
// Return: Nothing
function PortMulti( TUSObject )
  var props array;
  var MultiType := (TUSObject.worlditem - 0x4000);

  // Get and convert the X,Y,Z
  var pos := TUSObject.P;
  pos[","] := " "; // x is now "x y,z";
  pos[","] := " "; // x is now "x y z";
  pos := splitwords( pos ); // x is now an array { "x", "y", "z" }


  props[1] := {"Serial"  , HextoTxt(TUSObject.SERIAL)};
  props[2] := {"ObjType" , HextoTxt(TUSObject.worlditem)};
  props[3] := {"Graphic" , HextoTxt(TUSObject.worlditem)};
  props[4] := {"X" , pos[1]};
  props[5] := {"Y" , pos[2]};
  props[6] := {"Z" , pos[3]};

  if (MultiType >= 0x64)
    props[7] := {"CProp", "NumLockDowns   i" + FindNumLockdowns(TUSObject.worlditem)};
    props[8] := {"CProp", "NumSecure   i" + FindNumSecure(TUSObject.worlditem)};
    props[9] := {"CProp", "OwnerSerial   i1"};
    props[10] := {"DecayAt", "55000"};
  else
    props[7] := {"DecayAt", "55000"};
  endif

  StoreMulti(props);

endfunction

function PortMultis()
  var i;
  var OneBP := InPlayerBag(0x01);
  var CurrBP := 0;

  for (i := 1; i < NewMultisCount; i := i + 1)
    var CurrMulti := NewMultis[i];
    var MultiType := CurrMulti[2][2];
    var MultiSerial := CurrMulti[1][2];
    var CompIndex;
    var NewX := CurrMulti[4][2];
    var NewY := CurrMulti[5][2];
    var NewZ := CurrMulti[6][2];


    if (MultiType >= "0x4064")
      CompIndex := 11;
    else
      CompIndex := 8;
    endif

    while (1)
      var NewMulti := cint(CurrMulti[1][2]);
      //print("NewMulti: " + NewMulti);

      var NewComp := GetComponent(NewMulti);
      //print("NewComp: " + NewComp);

      if (NewComp == 0)
        break;
      endif

      CurrMulti[CompIndex] := {"Component", HextoTxt(NewComp)};
      CompIndex := CompIndex + 1;

    endwhile

    AppendConfigFileElem( "pol_multis", "MULTI", "", CurrMulti );

    if (mod(i, MAX_CONTAINER_SIZE) == 1)
      // Create new backpack!
      var bpprops array;

      CurrBP := GetAvailSerial();

      bpprops[1] := {"Serial",  HextoTxt(CurrBP)};
      bpprops[2] := {"ObjType", "0xE75"};
      bpprops[3] := {"Graphic", "0xE75"};
      bpprops[4] := {"X", "50"};
      bpprops[5] := {"Y", "50"};
      bpprops[6] := {"Z", "0"};
      bpprops[7] := {"Container", HextoTxt(OneBP)};
      bpprops[8] := {"Newbie", "1"};

      AppendConfigFileElem( "pol_pcequip", "Item", "", bpprops );
    endif

    // create ownership deed
    var OwnerDeed array;

    if (MultiType >= "0x4064")
      OwnerDeed[1] := {"Name", FindHouseName( MultiType ) + " at " + NewX + ", " + NewY + ", " + NewZ +" (Built)"};
      OwnerDeed[2] := {"Serial", HextoTxt(GetAvailSerial())};
      OwnerDeed[3] := {"ObjType", "0x14ef"};
      OwnerDeed[4] := {"Graphic", "0x14ef"};
      OwnerDeed[5] := {"X", "50"};
      OwnerDeed[6] := {"Y", "50"};
      OwnerDeed[7] := {"Z", "0"};
      OwnerDeed[8] := {"cProp", "builtserial i" + Cint(MultiSerial)};
      OwnerDeed[9] := {"container", HextoTxt(CurrBP)};
      OwnerDeed[10] := {"decayat", "56000"};
    else
      OwnerDeed[1] := {"Name", " (Built)"};
      OwnerDeed[2] := {"Serial", HextoTxt(GetAvailSerial())};
      OwnerDeed[3] := {"ObjType", "0x14f1"};
      OwnerDeed[4] := {"Graphic", "0x14f1"};
      OwnerDeed[5] := {"X", "50"};
      OwnerDeed[6] := {"Y", "50"};
      OwnerDeed[7] := {"Z", "0"};
      OwnerDeed[8] := {"cProp", "builtserial i" + Cint(MultiSerial)};
      OwnerDeed[9] := {"container", HextoTxt(CurrBP)};
      OwnerDeed[10] := {"decayat", "56000"};
    endif


    AppendConfigFileElem( "pol_pcequip", "Item", "", OwnerDeed );

  endfor

endfunction

function FindNumLockdowns( MultiType )
  case (Hextotxt(MultiType))
    "0x4064" : return 25;
    "0x4066" : return 25;
    "0x406A" : return 25;
    "0x406C" : return 25;
    "0x406E" : return 25;
    "0x4074" : return 86;
    "0x4076" : return 108;
    "0x4078" : return 108;
    "0x407A" : return 244;
    "0x407C" : return 375;
    "0x407E" : return 577;
    "0x4087" : return 86;
    default: return 0;
  endcase
endfunction

function FindNumSecure( MultiType )
  case (Hextotxt(MultiType))
    "0x4064" : return 1;
    "0x4066" : return 1;
    "0x406A" : return 1;
    "0x406C" : return 1;
    "0x406E" : return 1;
    "0x4074" : return 3;
    "0x4076" : return 5;
    "0x4078" : return 5;
    "0x407A" : return 12;
    "0x407C" : return 19;
    "0x407E" : return 29;
    "0x4087" : return 4;
    default: return 0;
  endcase
endfunction

function FindHouseName( MultiType )
  case (Hextotxt(MultiType))
    "0x4064" : return "deed to a small stone and plaster house";
    "0x4066" : return "deed to a small stone house";
    "0x406A" : return "deed to a small wooden house";
    "0x406C" : return "deed to a small plaster house";
    "0x406E" : return "deed to a small wood and straw house";
    "0x4070" : return "deed to a blue tent";
    "0x4072" : return "deed to a green tent";
    "0x4074" : return "deed to a large brick house";
    "0x4076" : return "deed to a two story wood and plaster house";
    "0x4078" : return "deed to a two story stone and plaster house";
    "0x407A" : return "deed to a tower";
    "0x407C" : return "deed to a keep";
    "0x407E" : return "deed to a castle";
    "0x4087" : return "deed to a large patio house";
    default: return 0;
  endcase
endfunction

// Purpose: Converts the object to an bankbox object
// Parameters: TUSObject - The object to convert
// Return: Nothing
function PortBankBox( TUSObject )
  var props array;

  // Very simple, Not a big thing to make.
  props[1] := {"Name", "Bankbox of " + TUSObject.CONT};
  props[2] := {"Serial", HexToTxt(TUSObject.SERIAL)};
  props[3] := {"ObjType", "0xe7c"};
  props[4] := {"Graphic", "0xe7c"};
  props[5] := {"X", "1433"};
  props[6] := {"Y", "1687"};
  props[7] := {"Z", "0"};
  props[8] := {"DecayAt", "5000"};

  StoreBankBoxes(TUSObject.SERIAL, TUSObject.CONT);

  AppendConfigFileElem( "pol_storage", "Item", "", props );
endfunction

// Purpose: Creates the POL File on completion.
// Parameters: TUSObject - The object to convert
// Return: Nothing
function PortPOLTxt()
  var props array;

  props[1] := {"CProp", "gameclock s58000"};
  props[2] := {"CProp", "nextlockid i" + GetAvailKey()};

  AppendConfigFileElem( "pol_pol", "GlobalProperties", "", props );
endfunction

// Purpose: Converts the spells found in a TUS Spell book to spell objects in POL
// Parameters: TUSObject - The object to convert
//             ExportFile - Where everything goes once it's created.
// Return: Nothing
function CreateSpellBookEntries( TUSObject, ExportFile )
  var i, j;
  var MoreBin;
  var NewSpell;
  var intMoreLen;
  var intText;


  if (TUSObject.MORE1 != error)
    intText := HextoTxt(TUSObject.MORE1);
    intMoreLen := len(intText);
    //print("TUSObject.MORE1 = " + HextoTxt(TUSObject.MORE1) + "  Length = " + intMoreLen);
    for (i := 0; i < (intMoreLen-2); i := i + 1)
      //print("  intText[intMoreLen-i] = " + intText[intMoreLen-i]);
      MoreBin := HextoBin(intText[intMoreLen-i]);
      //print("MoreBin = " + MoreBin);
      for (j := 4; j > 0; j := j - 1)
        if (MoreBin[j] == "1")
          var POLItem array;
          NewSpell := 0x1F2D + (i*4) + (4-j);
          if (NewSpell < 0x1F34)
            NewSpell := NewSpell + 1;
            if (NewSpell == 0x1F34)
              NewSpell := 0x1F2D;
            endif
          endif
          //print("Createing Spell Scroll " + HextoTxt(NewSpell) + " to File " + ExportFile);

          POLItem[1] := {"Serial",    HextoTxt(GetAvailSerial())};
          POLItem[2] := {"ObjType",   HextoTxt(NewSpell)};
          POLItem[3] := {"Graphic",   HextoTxt(NewSpell)};
          POLItem[4] := {"X",         "40"};
          POLItem[5] := {"Y",         "40"};
          POLItem[6] := {"Z",         "0"};
          POLItem[7] := {"Container", HextoTxt(TUSObject.SERIAL)};
          POLItem[8] := {"DecayAt",   "40000"};

          AppendConfigFileElem( ExportFile, "Item", "",  POLItem);
        endif
      endfor
    endfor
  else
    //print("Error: Couldn't find More1.");
  endif

  if (TUSObject.MORE2 != error)
    intText := HextoTxt(TUSObject.MORE2);
    intMoreLen := len(intText);
    //print("TUSObject.MORE2 = " + HextoTxt(TUSObject.MORE2) + "  Length = " + intMoreLen);
    for (i := 0; i < (intMoreLen-2); i := i + 1)
      //print("  intText[intMoreLen-i] = " + intText[intMoreLen-i]);
      MoreBin := HextoBin(intText[intMoreLen-i]);
      //print("MoreBin = " + MoreBin);
      for (j := 4; j > 0; j := j - 1)
        if (MoreBin[j] == "1")
          var POLItem array;
          NewSpell := 0x1F4D + (i*4) + (4 - j);
          //print("Createing Spell Scroll " + HextoTxt(NewSpell) + " to File " + ExportFile);

          POLItem[1] := {"Serial",    HextoTxt(GetAvailSerial())};
          POLItem[2] := {"ObjType",   HextoTxt(NewSpell)};
          POLItem[3] := {"Graphic",   HextoTxt(NewSpell)};
          POLItem[4] := {"X",         "40"};
          POLItem[5] := {"Y",         "40"};
          POLItem[6] := {"Z",         "0"};
          POLItem[7] := {"Container", HextoTxt(TUSObject.SERIAL)};
          POLItem[8] := {"DecayAt",   "40000"};

          AppendConfigFileElem( ExportFile, "Item", "",  POLItem);
        endif
      endfor
    endfor
  else
    //print("Error: Couldn't find More2.");
  endif
endfunction


// Purpose: Creates the Bank box entry for the key ring
// Parameters: None
// Return: Nothing
function CreateKeyBankBox( TUSObject )
/*
    var keybox array;

    var KeyBoxSerial := GetAvailSerial();

    keybox[1] := {"Name",    "Bankbox " + HextoTxt(TUSObject.SERIAL)};
    keybox[2] := {"Serial",   HextoTxt(KeyBoxSerial)};
    keybox[3] := {"objtype", "0xe7c"};
    keybox[4] := {"graphic", "0xe7c"};
    keybox[5] := {"x",       "0"};
    keybox[6] := {"y",       "0"};
    keybox[7] := {"z",       "0"};
    keybox[8] := {"Decay",   "56000"};

    AppendConfigFileElem( "pol_storage", "Item", "", keybox);
*/  // Have to skip for now.  POL Will generate all the info for me but I need to have the keys go in the same container the keyring is so we'll store things that way.
    if (TUSObject.CONT != error)
      StoreKeyRing(TUSObject.SERIAL, TUSObject.CONT); //Place all keys in Container now
      if (InPlayerBag(TUSObject.CONT))
        StorePlayerBag(TUSObject.SERIAL, TUSObject.CONT);
      else
       if (InBankBoxes(TUSObject.CONT))
        StoreBankBoxes(TUSObject.SERIAL, TUSObject.CONT);
       endif
      endif
        // Some other container.
        StoreKeyRing(TUSObject.SERIAL, 1); //This is a warning, This key is on the worldmap
    endif
endfunction

// Purpose: Creates the inital Storage data
// Parameters: None
// Return: Nothing
function CreateStorage()
  var props array;

  props[1] := {"Name", "Merchant Storage"};

  AppendConfigFileElem( "pol_storage", "StorageArea", "", props );

  props[1] := {"Name", "World Bank"};

  AppendConfigFileElem( "pol_storage", "StorageArea", "", props );

endfunction

// Purpose: Creates Character one and his backpack
// Parameters: None
// Return: Nothing
function CreateCharOne()

  // XYZ: 1390, 1625, 30
  // This will hold the properties we wish to edit
  var CharNumber;
  var chridx := GetGlobalProperty( "#admin" );
  if( chridx = error )                                //many chars we've seen for this account.
    SetGlobalProperty( "#admin" , 0 ); //next char idx for this account
    CharNumber := 0;
  else
    CharNumber := CInt(chridx);
  endif
  SetGlobalProperty( "#admin" , CharNumber + 1 );

  var props array;

  props[1]  := {"Account" ,               "admin"};
  props[2]  := {"CharIdx" ,               CharNumber};
  props[3]  := {"Name",                   "admin"};
  props[4]  := {"Serial",                 "0x01"};
  props[5]  := {"Objtype",                "0x190"};
  props[6]  := {"Graphic",                "0x190"};
  props[7]  := {"Color",                  "0x83EA"};
  props[8]  := {"X",                      "1390"};
  props[9]  := {"Y",                      "1625"};
  props[10] := {"Z",                      "30"};
  props[11] := {"Facing",                 0};
  props[12] := {"Cmdlevel",               "test"};
  props[13] := {"TrueColor",              "0x83EA"};
  props[14] := {"TrueObjtype",            "0x190"};
  props[15] := {"Gender",                 "1"};
  props[16] := {"STR",                    "26224"};
  props[17] := {"INT",                    "26224"};
  props[18] := {"DEX",                    "26224"};
  props[19] := {"HITS",                   "46"};
  props[20] := {"MANA",                   "46"};
  props[21] := {"STAM",                   "46"};
  props[22] := {"CreatedAt",              0};
  props[23] := {"Alchemy",                "0"};

  AppendConfigFileElem( "pol_pcs", "Character", "", props );

  var backp array;
  var BPSerial := GetAvailSerial();
  backp[1] := {"Serial",  HextoTxt(BPSerial)};
  backp[2] := {"ObjType", "0xE75"};
  backp[3] := {"Graphic", "0xE75"};
  backp[4] := {"X", "0"};
  backp[5] := {"Y", "0"};
  backp[6] := {"Z", "0"};
  backp[7] := {"Container", "0x1"};
  backp[8] := {"Movable", "0"};
  backp[9] := {"Newbie", "1"};
  backp[10] := {"Layer", "21"};

  AppendConfigFileElem( "pol_pcs", "Item", "", backp );

  StorePlayerBag(0x01, BPSerial);

endfunction


// Purpose: Convert the Hex number to a decimal one
// Parameters: hexval - The Hex string you wish to convert
// Return: The number as a integer decimal.
function HextoInt( hexval )
  return CInt( CStr(Hex(hexval)) - "0x" );
endfunction

// Purpose: Convert the number to a hex string
// Parameters: hexval - the number you wish to convert
// Return: A string which is in hex format of the number
function HextoTxt( hexval )
  return CStr(Hex(hexval));
endfunction

// Purpose: Convert the Base Hex number to a raw decimal one
// Parameters: hexval - The Hex string you wish to convert
// Return: The raw skill points for the hex number.
function BaseHextoRawDec( hexval )
  return BaseSkillToRawSkill(HexToInt(hexval));
endfunction

// Purpose: Convert the Base Hex number to a raw decimal one then to a hex string
// Parameters: hexval - The Hex string you wish to convert
// Return: The number as a integer decimal.
function HexToRawToTxt( hexval )
  return HextoTxt( BaseHextoRawDec( hexval ) );
endfunction

// Purpose: To translate a Hex number into A binary String
// Parameters: hexval - The Hex Number to translate
// Return: A four character string of 1"s or 0"s that is equivalent to the binary format of the hex number
function HextoBin( hexval )
  case ( CStr(hexval) )
    "0": return "0000";
    "1": return "0001";
    "2": return "0010";
    "3": return "0011";
    "4": return "0100";
    "5": return "0101";
    "6": return "0110";
    "7": return "0111";
    "8": return "1000";
    "9": return "1001";
    "A": return "1010";
    "B": return "1011";
    "C": return "1100";
    "D": return "1101";
    "E": return "1110";
    "F": return "1111";
    default: return "Error";
  endcase
endfunction

// Purpose: To convert a bin number to a number that identifies what location that number can be found on a four bit binary number.
// Parameters: hexval - The hex number to translate
// Return: A number from 1-4 specifying where hexval would be located in a binary nibble.
function HextoLoc( hexval ) // Hex to Location
  case ( hexval )
    "1": return 4;
    "2": return 3;
    "4": return 2;
    "8": return 1;
    // Default to 0 for errors
  endcase
endfunction

// Purpose: To test to see if a binary flag is true
// Parameters: hexval - The Hex number to test
//             flag - The flag to test
// Return: 1 if Set, 0 if not set.
function FlagTest( hexval, flag )
  var FlagLoc;  // The location of the flag in a binary string
  var BinStr;   // The Binary string translation of the Hex value

  BinStr := HextoBin( hexval ); // Translate the hex number

  FlagLoc := HextoLoc( Flag ); // Find the location of the number to test

 // print("  FlagTest hexval: " + hexval + " flag: " + flag + " BinStr: " + BinStr + " FlagLoc: " + FlagLoc);

  if ( BinStr[FlagLoc] = "1")
    return 1;
  else
    return 0;
  endif

endfunction

// Purpose: Checks to see if the item is a container
// Parameters: item - The item we wish to check
// Return: 1 if it is, 0 if it's not.
function IsContainer( item )
  if ( (item == UOBJ_SPELLBOOK) ||
       (item == UOBJ_CORPSE) ||
       (item == UOBJ_METALCHEST_EW) ||
       (item == UOBJ_WOODENCHEST_EW) ||
       (item == UOBJ_WOODENCHEST_NS) ||
       (item == UOBJ_BACKPACK) ||
       (item == UOBJ_BAG) ||
       (item == UOBJ_POUCH) ||
       (item == UOBJ_BARREL) ||
       (item == UOBJ_METALCHEST2_EW) ||
       (item == UOBJ_STRONGBOX) ||
       (item == UOBJ_SHOPKEEPER_CONT) ||
       (item == UOBJ_BRASS_BOX) ||
       (item == UOBJ_D_WOODEN_CRATE) ||
       (item == UOBJ_WOODEN_BOX) ||
       (item == UOBJ_METAL_CHEST) ||
       (item == UOBJ_POUCH2) ||
       (item == UOBJ_LARGE_CRATE_W) ||
       (item == UOBJ_LARGE_CRATE_N) ||
       (item == UOBJ_MEDIUM_CRATE_W) ||
       (item == UOBJ_MEDIUM_CRATE_N) ||
       (item == UOBJ_METALCHEST_N) ||
       (item == UOBJ_PICNIC_BASKET) ||
       (item == UOBJ_WOODEN_BOX_W) ||
       (item == UOBJ_SMALL_CRATE) ||
       (item == UOBJ_KEG) ||
       (item == UOBJ_EMPTY_TUB) ||
       (item == UOBJ_BARREL2) ||
       (item == UOBJ_WOODEN_CRATE_1) ||
       (item == UOBJ_WOODEN_CRATE_2) ||
       (item == UOBJ_LARGE_BASKET) ||
       (item == UOBJ_SMALL_BASKET) ||
       (item == UOBJ_BASIN) ||
       (item == UOBJ_CHEST) ||      //furniture
       (item == UOBJ_DRAWER1) ||
       (item == UOBJ_DRAWER2) ||
       (item == UOBJ_DRAWER3) ||
       (item == UOBJ_CHEST2) ||
       (item == UOBJ_DRAWER_DARK1) ||
       (item == UOBJ_DRAWER_DARK2) ||
       (item == UOBJ_DRAWER_DARK3) ||
       (item == UOBJ_CHEST_SW) ||
       (item == UOBJ_DRAWER_SW1) ||
       (item == UOBJ_DRAWER_SW2) ||
       (item == UOBJ_DRAWER_SW3) ||
       (item == UOBJ_CHEST_DARK_SW1) ||
       (item == UOBJ_CHEST_DARK_SW2) ||
       (item == UOBJ_CHEST_DARK_SW3) ||
       (item == UOBJ_CHEST_DARK_SW4) ||
       (item == UOBJ_DRESSER1) ||
       (item == UOBJ_DRESSER2) ||
       (item == UOBJ_DRESSER3) ||
       (item == UOBJ_DRESSER4) ||
       (item == UOBJ_DRESSER5) ||
       (item == UOBJ_DRESSER6) ||
       (item == UOBJ_DRESSER7) ||
       (item == UOBJ_DRESSER8) ||
       (item == UOBJ_DRESSER9) ||
       (item == UOBJ_DRESSER10) ||
       (item == UOBJ_DRESSER11) ||
       (item == UOBJ_DRESSER12) ||
       (item == UOBJ_DRESSER13) ||
       (item == UOBJ_DRESSER14) ||
       (item == UOBJ_DRESSER15) ||
       (item == UOBJ_DRESSER16) ||
       (item == UOBJ_ARMOIRE_DARK) ||
       (item == UOBJ_ARMOIRE_DARK2) ||
       (item == UOBJ_ARMOIRE_LIGHT) ||
       (item == UOBJ_ARMOIRE_LIGHT2) ||
       (item == UOBJ_ARMOIRE_DARK3) ||
       (item == UOBJ_ARMOIRE_DARK4) ||
       (item == UOBJ_ARMOIRE_LIGHT3) ||
       (item == UOBJ_ARMOIRE_LIGHT4) ||
       (item == UOBJ_BOOKCASE) ||
       (item == UOBJ_BOOKCASE2) ||
       (item == UOBJ_BOOKCASE3) ||
       (item == UOBJ_BOOKCASE4) ||
       (item == UOBJ_BOOKCASE5) ||
       (item == UOBJ_BOOKCASE6) ||
       (item == UOBJ_BOOKCASE7) ||
       (item == UOBJ_BOOKCASE8)
     )
    return 1;
  else
    return 0;
  endif
endfunction

// Purpose: Checks to see if the item is a spawn point
// Parameters: item - the item we wish to check
// Return: 1 if it is, 0 if it's not
function IsSpawn( item )

  if ( (item.Type == TUS_TYPE_SPAWN_ITEM) ||
       (item.Type == TUS_TYPE_SPAWN_NPC)
     )
    return 1;
  else
    return 0;
  endif

endfunction

// Purpose: Checks to see if the item is a teleporter point
// Parameters: item - the item we wish to check
// Return: 1 if it is, 0 if it's not
function IsTelepad( item )
  if  (item.Type == TUS_TYPE_TELEPAD)
    return 1;
  else
    return 0;
  endif

endfunction

// Purpose: Checks to see if the item is a door
// Parameters: item - the item we wish to check
// Return: 1 if it is, 0 if it's not
function IsDoor( item )
  if ( (item >= UOBJ_DOORS_START) &&
       (item <= UOBJ_DOORS_END)
     )
    return 1;
  else
    return 0;
  endif
endfunction

// Purpose: Checks to see if the item is locked (Door, chest, etc)
// Parameters: item - the item we wish to check
// Return: 1 if it is, 0 if it's not
function IsLocked ( item )
  if (item.Type == TUS_TYPE_LOCKED)
    return 1;
  else
    return 0;
  endif
endfunction

// Purpose: To alter the door item to one that POL is acceptable with
// Parameters: item - the door we wish to alter
// Return: The corrected door tile
function ConvDoor( item )
  local newdoor := 0;
  local itemstring;

  itemstring := HextoTxt(item);

  case (CStr(itemstring[5]))
    "5" : return item;
    "6" : return (item - 1);
    "7" : return item;
    "8" : return (item - 1);
    "9" : return (item - 4);
    "A" : return (item - 5);
    "B" : return (item - 4);
    "C" : return (item - 5);
    "D" : return item;
    "E" : return (item - 1);
    "F" : return item;
    "0" : return (item - 1);
    "1" : return (item - 4);
    "2" : return (item - 5);
    "3" : return (item - 4);
    "4" : return (item - 5);
    // Default return ERROR;
  endcase

  return 0xffff;
endfunction

// Purpose: Checks to see if the item is a magic wand (Also used for magic items)
// Parameters: item - the item we wish to check
// Return: 1 if it is, 0 if it's not
function IsWand ( item )
  if (item.Type == TUS_TYPE_WAND)
    return 1;
  else
    return 0;
  endif
endfunction

// Purpose: Checks to see if the item is a magical
// Parameters: item - the item we wish to check
// Return: 1 if it is, 0 if it's not
function IsMagical ( item )
  var str := HextoTxt(item.ATTR);
  var strl := len(str);
  if (strl < 4)
    return 0;
  endif
  if (FlagTest(cstr(str[(strl-1)]), "2"))
    return 1;
  else
    return 0;
  endif
endfunction

// Purpose: Checks to see if the item is a Immobile
// Parameters: item - the item we wish to check
// Return: 1 if it is, 0 if it's not
function IsImmobile ( item )
  var str := HextoTxt(item.ATTR);
  var strl := len(str);
  if (strl < 4)
    return 0;
  endif
  if (FlagTest(cstr(str[(strl-1)]), "1"))
    return 1;
  else
    return 0;
  endif
endfunction

// Purpose: Checks to see if the item is a Mobile
// Parameters: item - the item we wish to check
// Return: 1 if it is, 0 if it's not
function IsMobile ( item )
  var str := HextoTxt(item.ATTR);
  var strl := len(str);
  if (FlagTest(cstr(str[strl]), "8"))
    return 1;
  else
    return 0;
  endif
endfunction

// Purpose: Checks to see if the item is a Newbied
// Parameters: item - the item we wish to check
// Return: 1 if it is, 0 if it's not
function IsNewbied ( item )
  var str := HextoTxt(item.ATTR);
  var strl := len(str);
  //print("Length: " + strl + " of string " + str + " Text: " + str[strl]);
  if (FlagTest(cstr(str[strl]), "4"))
    //print("Pass! item " + HextoTxt(item.worlditem));
    return 1;
  else
    return 0;
  endif
endfunction

// Purpose: Checks to see if the item is a Decaying
// Parameters: item - the item we wish to check
// Return: 1 if it is, 0 if it's not
function IsDecaying ( item )
  var str := HextoTxt(item.ATTR);
  var strl := len(str);
  if (FlagTest(cstr(str[strl]), "2"))
    return 1;
  else
    return 0;
  endif
endfunction

// Purpose: Checks to see if the item is a face thing
// Parameters: item - the item we wish to check
// Return: 1 if it is, 0 if it's not
function IsFacial( item )
  if ( ( (item >= UOBJ_HAIR1_START)   &&
         (item <= UOBJ_HAIR1_END)   ) ||
       ( (item >= UOBJ_HAIR2_START)   &&
         (item <= UOBJ_HAIR2_END)   )
     )
    return 1;
  else
    return 0;
  endif
endfunction

// Purpose: Checks to see if the item is a beard
// Parameters: item - the item we wish to check
// Return: 1 if it is, 0 if it's not
function IsBeard( item )
  if ( (item == UOBJ_LONGBEARD)  ||
       (item == UOBJ_SHORTBEARD) ||
       (item == UOBJ_GOATEE)     ||
       (item == UOBJ_MUSTACHE)   ||
       (item == UOBJ_MSBEARD)    ||
       (item == UOBJ_MLBEARD)    ||
       (item == UOBJ_VANDYKE)
     )
    return 1;
  else
    return 0;
  endif
endfunction

// Purpose: Checks to see if the item is a Head hair object
// Parameters: item - the item we wish to check
// Return: 1 if it is, 0 if it's not
function IsHeadHair( item )
  if ( (item == UOBJ_SHORTHAIR) ||
       (item == UOBJ_LONGHAIR)  ||
       (item == UOBJ_PONYTAIL)  ||
       (item == UOBJ_MOHAWK)    ||
       (item == UOBJ_PAGEBOY)   ||
       (item == UOBJ_KRISNA)    ||
       (item == UOBJ_AFRO)      ||
       (item == UOBJ_RECEDING)  ||
       (item == UOBJ_PIGTAILS)  ||
       (item == UOBJ_BUNS)
     )
    return 1;
  else
    return 0;
  endif
endfunction


// Purpose: Checks to see if the item is a key object
// Parameters: item - the item we wish to check
// Return: 1 if it is, 0 if it's not
function IsKey( item )
  if ( (item == UOBJ_COPPER_KEY) ||
       (item == UOBJ_GOLD_KEY) ||
       (item == UOBJ_IRON_KEY) ||
       (item == UOBJ_MAGIC_KEY) ||
       (item == UOBJ_RUSTY_KEY)
     )
    return 1;
  else
    return 0;
  endif
endfunction

// Purpose: Checks to see if the item is on a player
// Parameters: item - the item we wish to check
// Return: 1 if it is, 0 if it's not
function IsOnPlayer( item )
 if (item.CONT != error)
  if (item.CONT < ITEMS_WORLD)
    return 1;
  else
    return 0;
  endif
 else
    return 0;
 endif
endfunction


// Purpose: Sets the first available serial number.
// Parameters: None
// Return: none
function SetAvailSerial( nextserial )
  intCurAvailSerial := nextserial + 1;
endfunction

// Purpose: Returns the next available serial number
// Parameters: None
// Return: Next available serial number
function GetAvailSerial()
  intCurAvailSerial := intCurAvailSerial + 1;
  return intCurAvailSerial;
endfunction

// Purpose: Sets the first available Key ID.
// Parameters: None
// Return: none
function InitAvailKey()
  intNextAvailKey := 0x0;
endfunction

// Purpose: Returns the next available KeyID
// Parameters: None
// Return: Next available serial number
function GetAvailKey()
  intNextAvailKey := intNextAvailKey + 1;
  return intNextAvailKey;
endfunction

// Purpose: Checks to see if the item is a Rune
// Parameters: item - the item we wish to check
// Return: 1 if it is, 0 if it's not
function IsMarkedRune( item )
  if ( (item.TYPE == TUS_TYPE_RUNE) && (item.MOREP != error) )
    return 1;
  else
    return 0;
  endif
endfunction

// Purpose: Checks to see if the item is a Keyring
// Parameters: item - the item we wish to check
// Return: 1 if it is, 0 if it's not
function IsKeyRing( item )
  if ( (item == UOBJ_KEYRING) ||
       (item == UOBJ_KEYRING_ONEKEY) ||
       (item == UOBJ_KEYRING_MULTIKEY) ||
       (item == UOBJ_KEYRING_MAGICKEY)
     )
    return 1;
  else
    return 0;
  endif
endfunction

// Purpose: Checks to see if the item is a Spell- or EQ Memory Marker
// Parameters: item - the item we wish to check
// Return: 1 if it is, 0 if it's not
function IsMemObj( item )
  if ( (item.TYPE == TUS_TYPE_SPELL) || (item.TYPE == TUS_TYPE_EQ_MEM) )
    return 1;
  else
    return 0;
  endif
endfunction

// Purpose: Checks to see if the item is a component
// Parameters: item - the item we wish to check
// Return: 1 if it is, 0 if it's not
function IsComponent( item )
  if (item.LINK != error)
    return 1;
  else
    return 0;
  endif
endfunction

// Purpose: Checks to see if the item is a House Sign
// Parameters: item - the item we wish to check
// Return: 1 if it is, 0 if it's not
function IsHouseSign( item )
  if ( (item.worlditem >= 0x0BCF) &&
       (item.worlditem <= 0x0BD2)
     )
    return 1;
  else
    return 0;
  endif
endfunction
