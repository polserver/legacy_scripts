

function drop_anchor()

// assumes that global me = npc::self



local cfg := readconfigfile("npcdesc");

local dstart := cfg[me.npctemplate].dstart;
local psub := cfg[me.npctemplate].psub;

if (!psub)
        psub := 20;
endif

if (dstart)
       SetAnchor(me.x, me.y, dstart, psub);
endif

endfunction