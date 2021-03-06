/* $Id: gameTime.inc 373 2006-06-17 18:27:33Z austinheilman $
 *
 *
 */
 
use uo;
use os;

/*
 * GetGameTime(day_length)
 *
 * Purpose
 * Takes the value from ReadGameClock() and converts it into a time.
 *
 * Parameters
 * day_length: Number of real life seconds a game day should last.
 *
 * Return Value
 * Returns a string
 *
 */
function TIME_GetGameTime(day_length)
	var cur_time := ReadGameClock();
endfunction
