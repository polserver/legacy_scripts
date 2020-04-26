include "include/objtype";

function IsExtended( plank )
    return plank.graphic in { GID_PLANK_EXTENDED_FACING_EAST, 
                              GID_PLANK_EXTENDED_FACING_WEST,
                              GID_PLANK_EXTENDED_FACING_NORTH,
                              GID_PLANK_EXTENDED_FACING_SOUTH };
endfunction                           

function IsPlankOccupied( plank )
    return ListMobilesNearLocation( plank.x, plank.y, plank.z, 0 ).size();
endfunction

function Extend( plank )
    if (!IsExtended(plank))
        TogglePlank(plank);
    endif
endfunction

function Retract( plank )
    if (IsExtended(plank))
        TogglePlank(plank);
    endif
endfunction

function TogglePlank( plank )
    local partnertype := PlankPartner( plank.graphic );
    if (partnertype)
        plank.graphic := partnertype;
    endif
endfunction


function PlankPartner( planktype )
    case planktype
             // boat 0x4000, 0x4002 - see config/boats.cfg
        GID_PLANK_EXTENDED_FACING_EAST:  return GID_PLANK_RETRACTED_FACING_EAST;   // 0x3ed5: return 0x3eb1;
        GID_PLANK_RETRACTED_FACING_EAST: return GID_PLANK_EXTENDED_FACING_EAST;    // 0x3eb1: return 0x3ed5;
        GID_PLANK_EXTENDED_FACING_WEST:  return GID_PLANK_RETRACTED_FACING_WEST;   // 0x3ed4: return 0x3eb2;
        GID_PLANK_RETRACTED_FACING_WEST: return GID_PLANK_EXTENDED_FACING_WEST;    // 0x3eb2: return 0x3ed4;
        
             // boat 0x4001, 0x4003
        GID_PLANK_EXTENDED_FACING_NORTH:  return GID_PLANK_RETRACTED_FACING_NORTH; // 0x3e84: return 0x3e85;
        GID_PLANK_RETRACTED_FACING_NORTH: return GID_PLANK_EXTENDED_FACING_NORTH;  // 0x3e85: return 0x3e84;
        GID_PLANK_EXTENDED_FACING_SOUTH:  return GID_PLANK_RETRACTED_FACING_SOUTH; // 0x3e89: return 0x3e8a;
        GID_PLANK_RETRACTED_FACING_SOUTH: return GID_PLANK_EXTENDED_FACING_SOUTH;  // 0x3e8a: return 0x3e89;
            
        default:
            syslog( "Unknown plank type 0x" + Hex( planktype ) );
            return error;
    endcase
endfunction
