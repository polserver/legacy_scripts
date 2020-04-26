include "include/client";

function PlayMusic( character, music_id )

    local packet_str :=  ( "6D00" + music_id );

       print("packetstr: " + packet_str);

    SendPacket( character , packet_str );

endfunction
