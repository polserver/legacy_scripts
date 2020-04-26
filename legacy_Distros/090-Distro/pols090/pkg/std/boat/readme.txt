the main pol/scripts/misc/boat.src now just calls
    start_script( ":boat:boat", param );
to transfer control to the boat control script, which is now here.

FILES HAVING TO DO WITH BOATS
=============================
pol/config/boats.cfg defines boat components and locations for the core.

boat.src          the boat control script
                  responds to commands from the tillerman,
                  and moves the boat.

plank.src         gangplank open/close (doubleclick)
plankwalk.src     handles teleporting to land when it's too far away
plankcontrol.src  handles closing an open, but locked, plank on startup

OUTSTANDING BUGS
================
 *    small boat at 1449 1804 seems to be too close to shore.


RECENTLY FIXED
=============
 *    more than one mob can stand on a plank at once
 *    someone w/o a key can walk onto a boat over a locked but open plank
 *    hole in the bounceback 
 *    planks can be locked/unlocked while extended (changed key.src)
 *    plank autoretracts with mobs on it
 *    the deck is not empty
 *    boats still don't only listen to the closest mob
 *    can't command from the hold
 *    locked open plank saved (say if someone is standing on it on shutdown)
 *    boat listen range needs to be extended to 11 or 12
 *    logoff/shutdown on a locked open plank?
 *    doubleclicking a plank w/ someone on it
 *    keyholder walking onto a locked plank
