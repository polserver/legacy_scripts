// TUS Port Storage Functions.  <They just got too big to put in one file..
global PlayerBag array;
global PlayerBagCount;

global BankBoxes array;
global BankBoxesCount;

global MasterBackPack array;
global MasterBackCount;

global NPCBag array;
global NPCBagCount;

global KeyID array;
global KeyIDCount;

global KeyRing array;
global KeyRingCount;

global NewMultis array;
global NewMultisCount;

global Components array;
global ComponentsCount;

// Purpose: Store all the items serials of Players master backpacks or containers inside that bag
// Parameters: serialid - The serial number
// Return: Nothing
function StorePlayerBag( serialid, ContainedID )
// PlayerBag
  MasterStoreSerial( serialid, ContainedID, "PlayerBag" );
endfunction

// Purpose: Used to find if an container exists in (or is) the players master backpack.
// Parameters: serialid - The serial number of the object you seek
// Return: 1 if found, 0 if not found
function InPlayerBag( serialid )
// PlayerBag
  return MasterFindSerial( serialid, "PlayerBag" );
endfunction

// Purpose: Store all the items serials of Players bankbox or containers inside the bankbox
// Parameters: serialid - The serial number
// Return: Nothing
function StoreBankBoxes( serialid, ContainedID )
// BankBoxes
  MasterStoreSerial( serialid, ContainedID, "BankBoxes" );
endfunction

// Purpose: Used to find if an container exists in (or is) the players BankBox.
// Parameters: serialid - The serial number of the object you seek
// Return: 1 if found, 0 if not found
function InBankBoxes( serialid )
// BankBoxes
  return MasterFindSerial( serialid, "BankBoxes" );
endfunction

// Purpose: Store all the items serials of Players master backpacks and the PlayerID's
// Parameters: PlayerID - The serial number
// Return: Nothing
function StoreMasterBackpack( PlayerID, BackPackID )
// MasterBackPack
  MasterStoreSerial( PlayerID, BackPackID, "MasterBackPack" );
endfunction

// Purpose: Used to find if an container exists in (or is) the players master backpack.
// Parameters: PlayerID - The serial number of the object you seek
// Return: 1 if found, 0 if not found
function FindMasterBackpack( PlayerID )
// MasterBackPack
  return MasterFindSerial( PlayerID, "MasterBackPack" );
endfunction

// Purpose: Store all the items serials of NPC's, NPC's backpacks or containers inside that bag
// Parameters: serialid - The serial number
// Return: Nothing
function StoreNPCBag( serialid )
// NPCBag
  MasterStoreSerial( serialid, "1", "NPCBag" );
endfunction

// Purpose: Used to find if an container exists in (or is) an NPC
// Parameters: serialid - The serial number of the object you seek
// Return: 1 if found, 0 if not found
function InNPCBag( serialid )
// NPCBag
  return MasterFindSerial( serialid, "NPCBag" );
endfunction

// Purpose: Store all the Key ID's that TUS uses and the new POL Key ID we have made.
// Parameters: TUSKeyID - The Old Key ID
//             POLKeyID - The New Key ID
// Return: Nothing
function StoreKeyID( TUSKeyID, POLKeyID )
// KeyID
  MasterStoreSerial( TUSKeyID, POLKeyID, "KeyID" );
endfunction

// Purpose: Used to find the new id of a key/container
// Parameters: TUSKeyID - The serial number of the object you seek
// Return: 1 if found, 0 if not found
function FindKeyID( TUSKeyID )
// KeyID
  return MasterFindSerial( TUSKeyID, "KeyID" );
endfunction

// Purpose: Store all the KeyRing serials
// Parameters: serialid - The serial number
// Return: Nothing
function StoreKeyRing( serialid, newserialid )
  // KeyRing
  MasterStoreSerial( serialid, newserialid, "KeyRing" );
endfunction

// Purpose: Used to find if an item is on a key ring
// Parameters: serialid - The serial number of the object you seek
// Return: 1 if found, 0 if not found
function InOnKeyRing( serialid )
  //  KeyRing
  return MasterFindSerial( serialid, "KeyRing" );
endfunction

// Purpose: Store all the Components serials
// Parameters: serialid - The serial number
// Return: Nothing
function StoreComponent( linkid, serialid )
  MasterStoreSerial( linkid, serialid, "Component");
endfunction

// Purpose: Used to find if an item is on a key ring
// Parameters: multiid - The Multi serial number that a component may have which you seek
// Return: A Serial if found, 0 if not found
function GetComponent( multiid )
  return MasterRemoveSerial( multiid, "Component");
endfunction

// Purpose: Store all Multi Objects
// Parameters: serialid - The serial number
// Return: Nothing
function StoreMulti( multiobj )
  NewMultis[NewMultisCount] := multiobj;
  NewMultisCount := NewMultisCount + 1;
endfunction

// Purpose: Store all the items serials
// Parameters: SerialOne - The Primary Serial number
//             SerialTwo - The Secondary Serial Number
//             StorageID - The array to put the id in.
// Return: Nothing
function MasterStoreSerial( SerialOne, SerialTwo, StorageID )

  var StoreArray array;
  var StoreIndex;

  var high;
  var low;
  var current;
  var modify;

  //print("Inserting " + SerialOne + " And " + SerialTwo);

  case (StorageID)
    "PlayerBag" :      StoreArray := PlayerBag;
                       high := PlayerBagCount - 1;
                       StoreIndex := PlayerBagCount;
    "BankBoxes" :      StoreArray := BankBoxes;
                       high := BankBoxesCount - 1;
                       StoreIndex := BankBoxesCount;
    "MasterBackPack" : StoreArray := MasterBackPack;
                       high := MasterBackCount - 1;
                       StoreIndex := MasterBackCount;
    "NPCBag" :         StoreArray := NPCBag;
                       high := NPCBagCount - 1;
                       StoreIndex := NPCBagCount;
    "KeyID" :          StoreArray := KeyID;
                       high := KeyIDCount - 1;
                       StoreIndex := KeyIDCount;
    "KeyRing" :        StoreArray := KeyRing;
                       high := KeyRingCount - 1;
                       StoreIndex := KeyRingCount;
    "Component" :      StoreArray := Components;
                       high := ComponentsCount - 1;
                       StoreIndex := ComponentsCount;
  endcase

   // itemindex always points to the next item to insert, which at this time is invalid
  low  := 1;
  current := CInt(high / 2);

  if (StoreIndex == 1)
    case (StorageID)
      "PlayerBag" :      PlayerBag[1] := {SerialOne, SerialTwo };
                         PlayerBagCount := 2;
      "BankBoxes" :      BankBoxes[1] := {SerialOne, SerialTwo };
                         BankBoxesCount := 2;
      "MasterBackPack" : MasterBackPack[1] := {SerialOne, SerialTwo };
                         MasterBackCount := 2;
      "NPCBag" :         NPCBag[1] := {SerialOne, SerialTwo };
                         NPCBagCount := 2;
      "KeyID" :          KeyID[1] := {SerialOne, SerialTwo };
                         KeyIDCount := 2;
      "KeyRing" :        KeyRing[1] := {SerialOne, SerialTwo };
                         KeyRingCount := 2;
      "Component" :      Components[1] := {SerialOne, SerialTwo };
                         ComponentsCount := 2;
    endcase

    //print("  Inserting " + SerialOne + " at location " + 1);
    return 1;
  endif

  if (StoreArray[1][1] > SerialOne)
    case (StorageID)
      "PlayerBag" :      PlayerBag.insert(1, {SerialOne, SerialTwo });
                         PlayerBagCount := PlayerBagCount + 1;
      "BankBoxes" :      BankBoxes.insert(1, {SerialOne, SerialTwo });
                         BankBoxesCount := BankBoxesCount + 1;
      "MasterBackPack" : MasterBackPack.insert(1, {SerialOne, SerialTwo });
                         MasterBackCount := MasterBackCount + 1;
      "NPCBag" :         NPCBag.insert(1, {SerialOne, SerialTwo });
                         NPCBagCount := NPCBagCount + 1;
      "KeyID" :          KeyID.insert(1, {SerialOne, SerialTwo });
                         KeyIDCount := KeyIDCount + 1;
      "KeyRing" :        KeyRing.insert(1, {SerialOne, SerialTwo });
                         KeyRingCount := KeyRingCount + 1;
      "Component" :      Components.insert(1, {SerialOne, SerialTwo });
                         ComponentsCount := 2;
    endcase

    //print("  Inserting " + SerialOne + " before location " + 1);
    return 1;
  endif

  if (StoreArray[(StoreIndex-1)][1] < SerialOne)
    //StoreArray.insert(Contindex, SerialOne);
    case (StorageID)
      "PlayerBag" :      PlayerBag[StoreIndex] := {SerialOne, SerialTwo };
                         PlayerBagCount := PlayerBagCount + 1;
      "BankBoxes" :      BankBoxes[StoreIndex] := {SerialOne, SerialTwo };
                         BankBoxesCount := BankBoxesCount + 1;
      "MasterBackPack" : MasterBackPack[StoreIndex] := {SerialOne, SerialTwo };
                         MasterBackCount := MasterBackCount + 1;
      "NPCBag" :         NPCBag[StoreIndex] := {SerialOne, SerialTwo };
                         NPCBagCount := NPCBagCount + 1;
      "KeyID" :          KeyID[StoreIndex] := {SerialOne, SerialTwo };
                         KeyIDCount := KeyIDCount + 1;
      "KeyRing" :        KeyRing[StoreIndex] := {SerialOne, SerialTwo };
                         KeyRingCount := KeyRingCount + 1;
      "Component" :      Components[StoreIndex] := {SerialOne, SerialTwo };
                         ComponentsCount := ComponentsCount + 1;
    endcase

    //print("  Inserting " + SerialOne + " before location End " + (Contindex-1));
    return 1;
  endif

  while (1)
    if ( (StoreArray[current][1]) > SerialOne )
      high := current;
      modify := CInt((current-low)/2);
      if (!modify)
        modify := 1;
      endif
      current := modify + low;
    else
      low := current;
      modify := CInt((high-current)/2);
      if (!modify)
        modify := 1;
      endif
      current := modify + current;
    endif
    //print("  High = " + high + " Low = " + low + " Current = " + current);
    if ((current >= high) || (current <= low))
      //StoreArray.insert(current, {SerialOne, SerialTwo });
      case (StorageID)
        "PlayerBag"  :     PlayerBag.insert(current, {SerialOne, SerialTwo });
                           PlayerBagCount := PlayerBagCount + 1;
        "BankBoxes" :      BankBoxes.insert(current, {SerialOne, SerialTwo });
                           BankBoxesCount := BankBoxesCount + 1;
        "MasterBackPack" : MasterBackPack.insert(current, {SerialOne, SerialTwo });
                           MasterBackCount := MasterBackCount + 1;
        "NPCBag" :         NPCBag.insert(current, {SerialOne, SerialTwo });
                           NPCBagCount := NPCBagCount + 1;
        "KeyID" :          KeyID.insert(current, {SerialOne, SerialTwo });
                           KeyIDCount := KeyIDCount + 1;
        "KeyRing" :        KeyRing.insert(current, {SerialOne, SerialTwo });
                           KeyRingCount := KeyRingCount + 1;
        "Component" :      Components.insert(current, {SerialOne, SerialTwo });
                           ComponentsCount := ComponentsCount + 1;
      endcase
      //print("  Inserting " + SerialOne + " before location " + current);
      return 1;
    endif
  endwhile
endfunction

// Purpose: Used to find if an container exists.
// Parameters: SerialOne - The serial number of the object you seek
//             StorageID - The array to put the id in.
// Return: 1 if found, 0 if not found
function MasterFindSerial( SerialOne, StorageID )

  var StoreArray array;
  var StoreIndex;

  var high;
  var low;
  var current;
  var modify;

  //print("Finding " + SerialOne);


  case (StorageID)
    "PlayerBag" :      StoreArray := PlayerBag;
                       high := PlayerBagCount - 1;
                       StoreIndex := PlayerBagCount;
    "BankBoxes" :      StoreArray := BankBoxes;
                       high := BankBoxesCount - 1;
                       StoreIndex := BankBoxesCount;
    "MasterBackPack" : StoreArray := MasterBackPack;
                       high := MasterBackCount - 1;
                       StoreIndex := MasterBackCount;
    "NPCBag" :         StoreArray := NPCBag;
                       high := NPCBagCount - 1;
                       StoreIndex := NPCBagCount;
    "KeyID" :          StoreArray := KeyID;
                       high := KeyIDCount - 1;
                       StoreIndex := KeyIDCount;
    "KeyRing" :        StoreArray := KeyRing;
                       high := KeyRingCount - 1;
                       StoreIndex := KeyRingCount;
    "Component" :      StoreArray := Components;
                       high := ComponentsCount - 1;
                       StoreIndex := ComponentsCount;
  endcase

  low  := 1;
  current := CInt(high / 2);

  if (StoreIndex == 1)
    //print("  " + SerialOne + " Not found (Empty array)");
    return 0;
  endif

  if ((StoreArray[1][1] > SerialOne) || (StoreArray[StoreIndex-1][1] < SerialOne))
    //print("  " + SerialOne + " Not found (Outside boundries)");
    return 0;
  endif

  if (StoreArray[1][1] == SerialOne)
    //print("  " + SerialOne + " Found (At location 1)");
    return StoreArray[1][2];
  endif

  if (StoreArray[StoreIndex-1][1] == SerialOne)
    //print("  " + SerialOne + " Found (At location " + (Contindex-1) + ")");
    return StoreArray[StoreIndex-1][2];
  endif

  //print("  High = " + high + " Low = " + low + " Current = " + current);
  while (1)

    if (StoreArray[current][1] == SerialOne)
      //print("  " + SerialOne + " Found");
      return StoreArray[current][2];
    else
      if (StoreArray[current][1] > SerialOne)
        //print("  StoreArray[current] > SerialOne");
        high := current;
        modify := CInt((current-low)/2);
        if (!modify)
          modify := 1;
        endif
        current := modify + low;
      else
        //print("  StoreArray[current] <= SerialOne");
        low := current;
        modify := CInt((high-current)/2);
        if (!modify)
          modify := 1;
        endif
        current := modify + current;
      endif
      //print("  High = " + high + " Low = " + low + " Current = " + current);
      if ((current >= high) || (current <= low))
        //print("  " + SerialOne + " Not Found");
        return 0;
      endif
    endif

  endwhile

endfunction



// Purpose: Used to find if an container exists.
// Parameters: SerialOne - The serial number of the object you seek
//             StorageID - The array to put the id in.
// Return: 1 if found, 0 if not found
function MasterRemoveSerial( SerialOne, StorageID )

  var StoreArray array;
  var StoreIndex;

  var high;
  var low;
  var current;
  var modify;

  //print("Finding " + SerialOne);


  case (StorageID)
    "PlayerBag" :      StoreArray := PlayerBag;
                       high := PlayerBagCount - 1;
                       StoreIndex := PlayerBagCount;
    "BankBoxes" :      StoreArray := BankBoxes;
                       high := BankBoxesCount - 1;
                       StoreIndex := BankBoxesCount;
    "MasterBackPack" : StoreArray := MasterBackPack;
                       high := MasterBackCount - 1;
                       StoreIndex := MasterBackCount;
    "NPCBag" :         StoreArray := NPCBag;
                       high := NPCBagCount - 1;
                       StoreIndex := NPCBagCount;
    "KeyID" :          StoreArray := KeyID;
                       high := KeyIDCount - 1;
                       StoreIndex := KeyIDCount;
    "KeyRing" :        StoreArray := KeyRing;
                       high := KeyRingCount - 1;
                       StoreIndex := KeyRingCount;
    "Component" :      StoreArray := Components;
                       high := ComponentsCount - 1;
                       StoreIndex := ComponentsCount;
  endcase

  low  := 1;
  current := CInt(high / 2);

  if (StoreIndex == 1)
    //print("  " + SerialOne + " Not found (Empty array)");
    return 0;
  endif

  if ((StoreArray[1][1] > SerialOne) || (StoreArray[StoreIndex-1][1] < SerialOne))
    //print("  " + SerialOne + " Not found (Outside boundries)");
    return 0;
  endif

  if (StoreArray[1][1] == SerialOne)
    //print("  " + SerialOne + " Found (At location 1)");
    case (StorageID)
      "PlayerBag" :      PlayerBag.erase(1);
                         PlayerBagCount  := PlayerBagCount - 1;
      "BankBoxes" :      BankBoxes.erase(1);
                         BankBoxesCount  := BankBoxesCount - 1;
      "MasterBackPack" : MasterBackPack.erase(1);
                         MasterBackCount := MasterBackCount - 1;
      "NPCBag" :         NPCBag.erase(1);
                         NPCBagCount     := NPCBagCount - 1;
      "KeyID" :          KeyID.erase(1);
                         KeyIDCount      := KeyIDCount - 1;
      "KeyRing" :        KeyRing.erase(1);
                         KeyRingCount    := KeyRingCount - 1;
      "Component" :      Components.erase(1);
                         ComponentsCount := ComponentsCount - 1;
    endcase

    return StoreArray[1][2];
  endif

  if (StoreArray[StoreIndex-1][1] == SerialOne)
    //print("  " + SerialOne + " Found (At location " + (StoreIndex-1) + ")");
    case (StorageID)
      "PlayerBag" :      PlayerBag.erase(StoreIndex-1);
                         PlayerBagCount  := PlayerBagCount - 1;
      "BankBoxes" :      BankBoxes.erase(StoreIndex-1);
                         BankBoxesCount  := BankBoxesCount - 1;
      "MasterBackPack" : MasterBackPack.erase(StoreIndex-1);
                         MasterBackCount := MasterBackCount - 1;
      "NPCBag" :         NPCBag.erase(StoreIndex-1);
                         NPCBagCount     := NPCBagCount - 1;
      "KeyID" :          KeyID.erase(StoreIndex-1);
                         KeyIDCount      := KeyIDCount - 1;
      "KeyRing" :        KeyRing.erase(StoreIndex-1);
                         KeyRingCount    := KeyRingCount - 1;
      "Component" :      Components.erase(StoreIndex-1);
                         ComponentsCount := ComponentsCount - 1;
    endcase

    return StoreArray[StoreIndex-1][2];
  endif

  //print("  High = " + high + " Low = " + low + " Current = " + current);
  while (1)

    if (StoreArray[current][1] == SerialOne)
      //print("  " + SerialOne + " Found");
    case (StorageID)
        "PlayerBag" :      PlayerBag.erase(current);
                           PlayerBagCount  := PlayerBagCount - 1;
        "BankBoxes" :      BankBoxes.erase(current);
                           BankBoxesCount  := BankBoxesCount - 1;
        "MasterBackPack" : MasterBackPack.erase(current);
                           MasterBackCount := MasterBackCount - 1;
        "NPCBag" :         NPCBag.erase(current);
                           NPCBagCount     := NPCBagCount - 1;
        "KeyID" :          KeyID.erase(current);
                           KeyIDCount      := KeyIDCount - 1;
        "KeyRing" :        KeyRing.erase(current);
                           KeyRingCount    := KeyRingCount - 1;
        "Component" :      Components.erase(current);
                           ComponentsCount := ComponentsCount - 1;
      endcase

      return StoreArray[current][2];
    else
      if (StoreArray[current][1] > SerialOne)
        //print("  StoreArray[current] > SerialOne");
        high := current;
        modify := CInt((current-low)/2);
        if (!modify)
          modify := 1;
        endif
        current := modify + low;
      else
        //print("  StoreArray[current] <= SerialOne");
        low := current;
        modify := CInt((high-current)/2);
        if (!modify)
          modify := 1;
        endif
        current := modify + current;
      endif
      //print("  High = " + high + " Low = " + low + " Current = " + current);
      if ((current >= high) || (current <= low))
        //print("  " + SerialOne + " Not Found");
        return 0;
      endif
    endif

  endwhile

endfunction