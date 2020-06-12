

Left( Str, Count );

	
	// returns the one-based index of Search within Str after position Start
Find( Str, Search, Start );


	// len(): For strings, returns string length
	//        For arrays, returns number of elements.
Len( Str_or_array );

	// upper(): Convert whole string to upper-case
	// lower(): Convert whole string to lower-case
Upper( Str );
Lower( Str );

	// Various Conversion Functions
	//
CInt( expr );           // Convert to integer
CDbl( expr );           // Convert to double
CStr( expr );           // Convert to string
CAsc( str );            // Convert first character to Ascii value (0-255)
CChr( number );         // Convert Ascii value to character (type: string)
CAscZ( str );           // Convert string to array of Ascii values (0-255)
CChrZ( intarray );      // Convert array of Ascii values to a string

Bin( number) ;			// Convert 25 to 11001
Hex( number );          // Convert to 0x[hex val] - 0x0 to 0xFFFFFFFF

	
	// SplitWords: returns an array of words contained in str, based
	// on optional delimiter. Default delim is blank space.
SplitWords( str, delimiter := " " );

    // Pack and Unpack: convert an object to a storable string form
    // Pack( 5 ) returns "i5"
    // Unpack( "i5" ) returns 5
    // strings, integers, reals, and arrays and dictionaries of these can be packed.
Pack( expr );
Unpack( str );

TypeOf( expr ); // returns "Integer", "Real" etc
SizeOf( expr ); // returns estimate of memory used
