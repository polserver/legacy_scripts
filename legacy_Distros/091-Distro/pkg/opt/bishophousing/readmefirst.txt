this package comes from several weeks of repetative trips to the compiler and alot of borrowed work.. 

THIS PACKAGE IS NOT COMPATIBLE WITH EXISTING HOUSES!

due to the amount of new features and security checks, this package uses many more paramaters that get
attached to the house sign, and thus pre-existing houses will need to be redeeded and placed anew in 
order to get all the necessary information properly "attached" to the house sign (the sign is the most
important piece of the house). This does not mean that all existing houses will vanish on server startup, 
they will remain very much intact and usable as always have been, however the signcontrol script will
not recognize them as the owner of the house, nor will the sign display the ownership gump when used.
You will only have to go through this upon initial installation for existing houses.. all newly created 
houses will function properly... I realize that this can be alot of effort for those of you with a large
playerbase, but there was no good solution to this problem..  So i release this package with the intention
that it be used on new shards, or shards that wont mind replacing all their current houses for the upgraded
package.. for those of you not willing to go through all the hastle, feel free to use my gump layouts
you will find that they are very similar to OSI's housing gumps except for the friends menus..

I'd like to thank the following people:
Racalac (for the original housing package that this was based on)
Dream Weaver (for his ListRootItemsInContainer function)
Madman Across the Water  (for his doors package)
Cassandra White (for helping me figure out coding problems and providing a testing shard)
me (for whatever I had contibuted to the package [probably not much] cause i suck)
billy the toothless balerina (for things better left unsaid)
Myrathi (for answering countless questions from a paste eating moron)
and um.. anyone else i stole *ahem* borrowed code from (you will know who you are).. 

This package comes with both Static and Multi type housing and will be updated with banning code as 
soon as multi.WalkOnScript becomes available(wishful thinking).. This package offers the following
features:

friending

co-owners

house enemies (soon to be with banning feature[it is included but currently doesnt do anything on multis])

securing players containers (that update their contents)

lockdown, release, secure, unsecure, "remove thyself", and "I ban thee" functions

house decay (*very important* you use the enclosed doors package or integrate the house refreshing
code into your existing doors package or players houses will decay if they dont use the house sign
once every 10 days) upon a house decaying, the multi is destroyed and all locked down items within
become unlocked and all secure items become unsecure..

secure house trading deeds

house destruction that gives the owner a replacement deed

very much advanced gump with built in friends, coowners, and enemies lists..

house sign functions for adding friends and co-owners (no longer a verbal command.. much easier to 
maintain

different gumps for different relations to the house:
home owners have unrestricted access to the house
co-owners can do everything but remove or add co-owners and redded the house
friends may only change the houses name and view friends/co-owners lists but can add or remove house enemies
non-friended players will get a gump that only displays the house owners name and the houses name

real-time tracing of a houses age (single clicking the house sign will show the amount of time passed since a house
was last used:
"this house is like new"     50 minutes or less
"this house is slightly worn" 3 days
"this house is fairly worn"   5 days
"this house is greatly worn"  9 days
"this house is in danger of collapsing" 10 days (lasts up to 24 hours, then the house deletes)



included helper files:
recall spell, gate spell, teleport spell modifications to prevent magic travel into a house.. 
mark modifications to prevent marking into a house
telekinesis modifications to prevent stealing through walls (this is a bug in the distro version)

Madman Across The Water's doors package with house refreshing code *important*

This package has been tested extensively, however there is always the possibility that i missed something
please dont hesitate to report any bugs to me so i may fix them to re-submit.. its all im asking for



 