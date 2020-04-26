Advanced Alchemy Version 0.7

Instructions
============

Just create a new dir in /pkg/opt and put the files in it. Also you'll need to disable normal alchemy.

Don't Forget to compile all files

How Does it Work?
=================

  Each player, in order to be able to make a potion, will need the recipe for this potion. So
the player must find/buy a recipe and add to his book, then he will be able to try to make that
potion. To add a recipe to the book, double click on the recipe then in the book.

Changes From Normal Alchemy
===========================

  - On Failure the players has chances to get random events, like explosion and stuff like that
  - It's possible for each potion to have up to 4 diferents reagents

How to Change/Add Potions?
==========================

  You'll have to edit the alchemy2.cfg, there's where are the makable potions for players.
(read below how to make new Recipes).

Potion #: here you set the ID of the potion, you will need this number when making a Recipe for
this potion. 

name : This is obvious, the name that will appear in the book.
Skill: the skill needed to make this potion
For reagente and amount you can have up to 4 entries,
reagent: here you must put the hexa code of the reagent you wish the potion to use
amount : the amount that will be need of the reagent
points : the points that each potion gives
objtype: the objtype of the potion(see itemdesc.cfg)
icon   : the picture that will be shown in the book

Here is the example of the Lesser Heal Potion

potion 1
{
	name		Lesser Heal
        skill           15
        reagent         0x0f85
        amount		 1
        points          35
        objtype         0xDC01
        icon		0x0f0c
}


How to Add Recipes
==================

  Go to itemdes.cfg, copy a existing recipe and change some props. The most importante one is the
PotionID, that must be the same as in the alchemy2.cfg, and 2 recipes/potions can't have the same
ID or one of both wont work. Then change the name(hint: put recipe##, where ## is the ID of the potion),
Desc, and if necessary VendorSellsFor and VendorBuysFor.

  
  Item 0xA120
{
     Name Recipe20
     Desc Receita de Cura Fraca
     Script wbook
     Graphic 0x0e38
     Cprop PotionID i20
     VendorSellsFor  100
     VendorBuysFor   25
}

ToDo
====

  Add some more nice random events... anyone with concrect ideas?
  Organize better the book, with index or something like this

Charles Haustron