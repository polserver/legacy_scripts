Magic Wands - ver .6 updated April 13, 2000
****************************************
Zulu (zuluhotel@hotmail.com)

Feb 2
added 64 magic wands with charges.
added animation with casting.
added a delay in casting of wands.

Mar 14
added toxic cloud
added wall of death

April 13
added the golem wand (based on MeteorRaine's test code)


players need to use item id to find out what kind
of wand and how many charges are left.

unzip the files into the \pol\pkg\opt\zulumagic

** additional **

you might want to put in \pol\scripts\misc\logon.src

local temp := GetObjProperty(character,"origform");

if (temp)
	character.graphic := temp;
	EraseObjProperty(character,"origform");
	character.frozen := 0;
endif
