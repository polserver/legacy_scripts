Instalation Process
========================
	Go to the pkg folder over on your server tree, create a new directory with a name of your liking, try to make it remember that's a guildstone package, you can create it under the base ones, std, opt or dev. Here I'm using /pkg/opt/guildstone.
	Now unzip all files on the new place. Make sure You are not using objtype 0xa390 and 0xa391, or you will have to make some changes on itemdesc and guildstone.inc const declarations.
	Start, or restart, your server and here we go, just create 0x391 or guilddeed. To add'em on vendors you will have to change /config/mrcspawn.cfg and that's out of the scope of this readme.

Features
======================
	Everything from standard OSI guildstones but charter and declare guild alignment, these are out since I'm very lazzy these days to finish them.
	Guild aliance is planned but not implemented. It's something for the future.


FAQ
======================
	1) What are these weird message my players are receiving when they try to use view charter, set charter or change guild alignment?
	It's the portuguese phrase for "This feature still not implemented".

	2)When will you implement the last features?
	As soon as my mood and my free time improve.

	3)What are these commented lines at the begining of guildstone.inc?
	It's my reference table for the guild structure, this should help people venturing on undestanding how it works.

	4)Why do you comment the code is both portuguese and english?
	Because my mood changes often.

	5)How can I thank you for this great pkg?
	Send me a few bucks :-)

	6)Why your players are getting flagged criminal even if their guilds have declared war?
	This pkg only handle guild stuff, nothing related to reputation system.

	7)I was looking inside your code and found the "Murderer" word, what it its use?
	It's for my murdering system, you can rewrite this part of the code and clean-up this.


My thanks
=====================
	To Sigismund for doing such wonderfull yes/no gump, sure I stole it from your guildstone pkg;
	Myrathi for showing the how to fix the last issue of the pkg;
	My new monster sound system, don't know how I could live without you;
	And finnaly to all players of my server test shard for wasting a weekend testing the code.

Contact
====================
	Email: louds@geocities.com (primary)