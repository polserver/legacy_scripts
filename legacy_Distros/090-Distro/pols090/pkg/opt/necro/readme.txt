
Copy over the inscription.src and .cfg files into /pkg/std/inscription/. If you have already made changes to inscription, you can append the necro changes: add the last function (InscribeNecroSpell), the 'include "include/bitwise";', and the last "elseif" from the main inscription program (the one that calls the InscribeNecroSpell function). For the inscription.cfg, just copy the last 16 items, which are the necro scrolls. 
 

 Necromancy uses item #s 0xA100 through 0xA110. If this duplicates your own, and you need to change mine, you need to change them in the following places: 

- in the necromancy itemdesc.cfg file 
- in the inscription.cfg file, last 16 items. 
- in the inscription.src file, change the bounds for the if (the_item.objtype := ...) and change the objtype for the necrobook in the last function (if (the_book.objtype := 0xA100)) 
- in the necroscroll.cfg file. 

Using Necromancy:

First, you need a "Codex Damnorum" and a couple of necromatic scrolls. Inscribe the scrolls into your codex (yes, it's very hard; edit inscription.cfg if you want to change it), then double-click the book to cast spells.

Spell list:
animate  dead: targeta  corpse, get  a tamed zombie
wraith form: makes you look like a wraith, and damages all critters around you(adding thier health to your mana)
wraith breath:  like wraith form, but damage only applied once
summon spirit: summons a shade, wraith, or liche
spellbind: tames a critter
sacrifice: Kills one of your pets or undead, but damages all critters around it
release : Kills one undead (need to change npcdesc.cfg and put in a cprop for this to work)
raise dead: summons a skel, zombie, or spectre
plague : area-effect poison. Does raw damage, and sets poison level
liche: if successful, turns caster into a liche. Otherwise just kills caster
kill: if successful kills target, otherwise applies damage
darkness: blinds target
control undead: tames undead, duh!
commune: allows player to hear ghosts
wyvernstrike: heavy poison on a group