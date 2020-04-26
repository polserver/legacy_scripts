/****************************************************************/
/*	coords.inc - Coordinates related stuff - v1.0 				*/
/*																*/
/*	HellRaider <zion@uo.com.br>									*/
/*		v1.0 - July 13, 2000									*/
/*																*/
/*	string::ToSextant(int x, int y); Function returns a string	*/
/*		containing the OSI style sextant coordinates for the	*/
/*		referred x,y coordinates.								*/
/*		Note: Some coords seem to make this function return		*/
/*			the minutes with 1 tile of difference from the OSI	*/
/*			and UOAM minutes. It's not common, and it's only	*/
/*			one tile... it'd need a special rounding function	*/
/*			to be fixed (I don't know exactly how it should be	*/
/*			like, but that's what I got from UOAM site).		*/
/*																*/
/*	Feel free to contact me at zion@uo.com.br or				*/
/*								 hellraider@uo.com.br			*/
/*																*/
/****************************************************************/

// Example (test this out):
// - Put this file at pol/scripts/include/
// - Replace the contents of your pol/scripts/items/coords.src script with:
//		use uo;
//
//		include "include/coords";
//
//		program get_coords(char, item)
//		    PrintTextAbovePrivate(char, ToSextant(char.x, char.y), char);
//		endprogram
//
//	- Restart POL, and type in .create sextant
//	- Double click the sextant (DOH)


use basic;
use math;

function ToSextant(x, y)
	var latitude_direction, longitude_direction;
	var latitude_degrees, longitude_degrees;
	var latitude_minutes, longitude_minutes;

	latitude_degrees := (y-1624)*0.087890625;
	longitude_degrees := (x-1323)*0.0703125;

	if (latitude_degrees < 0)
		latitude_direction := "N";
	else
		latitude_direction := "S";
	endif

	if (longitude_degrees < 0)
		longitude_direction := "W";
	else
		longitude_direction := "E";
	endif

	latitude_degrees := Abs(latitude_degrees);
	longitude_degrees := Abs(longitude_degrees);

	/*	The next lines get the first 2 digits from the decimal part of the real variables 	*/
	/*	and multiply by 60, returning an integer.											*/
	latitude_minutes := CInt((CInt((latitude_degrees-Floor(latitude_degrees))*100)*60)/100);
	longitude_minutes := Cint((CInt((longitude_degrees-Floor(longitude_degrees))*100)*60)/100);

	return(	Floor(latitude_degrees)+"o "+latitude_minutes+"' "+latitude_direction+" "+
			Floor(longitude_degrees)+"o "+longitude_minutes+"' "+longitude_direction		);
endfunction