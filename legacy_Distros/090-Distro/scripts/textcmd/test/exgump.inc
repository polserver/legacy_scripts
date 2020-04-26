use uo;

// includer must define:

// var layout;
// var data;

program textcmd_testgump( who )
    var res := SendDialogGump( who, layout, data );             
    
    print( "Dialog Gump Result: " );
    foreach key in (res.keys)
        var value := res[key];
        print( "Key=" + key + ", value=" + value );
    endforeach
endprogram
