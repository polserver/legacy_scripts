// Bookcase Spawning system  version .3
// created December 7, 2000
// designed by Bishop Ebonhand and Zulu

This is based on syzygy book system.  The requires version .92 of POL.

// updated by Syzygy 12/7/2000 to simplify install process and fix
//    first page "7 lines only" bug


*** edit book1 and book2 in the zulubooks.cfg ***

** Installing **
1. unzip package into any new folder under /pol/pkg, for example /pol/pkg/opt/sysbook-0.2/
2. Comment out or delete Containers 0xA97 through 0xA9E in /pol/config/itemdesc.cfg
3. Disable the 'book' package
4. compile scripts

** To add new books **

1. Load the zulubooks.cfg file
2. go to the bottom of the file
3. add new entry and save the file
4. add book item to itemdesc.cfg
5. add the book to an npc for sale?

** book info **
- books can be up to 16 pages max.
- page 1 can only have 7 lines,
   the rest can have up to 8.
- p#l# = page # line #

book #
{
pages #
title <book title>
author <author name>
p#l# <max 25 chars>
}
