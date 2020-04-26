use cfgfile;
use basic;

function open_door(door)

    local doordesc := ReadConfigFile(":newdoors:itemdesc");

    local xmod := CInt(doordesc[door.objtype].xmod);
    local ymod := CInt(doordesc[door.objtype].ymod);

    local newx := (door.x + xmod);
    local newy := (door.y + ymod);
    local newz := door.z;

    local opengraphic := GetObjProperty(door, "opengraphic");
    if ((!opengraphic) or (opengraphic == ""))
        opengraphic := door.graphic + 1;
    endif

    door.graphic := opengraphic;
    door.movable := 1;
    MoveItemToLocation (door, newx, newy, newz, MOVEITEM_FORCELOCATION);    
    door.movable := 0;

endfunction

function close_door(door)

    local doordesc := ReadConfigFile(":newdoors:itemdesc");

    local xmod := CInt(doordesc[door.objtype].xmod);
    local ymod := CInt(doordesc[door.objtype].ymod);

    local newx := (door.x - xmod);
    local newy := (door.y - ymod);
    local newz := (door.z);

    door.graphic := door.objtype;
    door.movable := 1;
    MoveItemToLocation (door, newx, newy, newz, MOVEITEM_FORCELOCATION);
    door.movable := 0;

endfunction
