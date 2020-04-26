Penultima Online (POL)
Copyright (C) 1993-2002 Eric N. Swanson
Do not distribute this package

THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR 
IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED 
WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.

NOTE:
=====
Make SURE the utility you are using to unzip POL supports long filenames (like WINZIP)!
If any of the filenames are truncated the server will now start.

QUICK START:
============
 SERVER
 ------
  In POL.CFG, Edit the UoDatafileRoot= line to match your UO directory.

  in CONFIG\SERVERS.CFG, Edit the first entry's IP address to match your own.
  (Most don't need to do this, as POL detects your IP automatically)

  You may want to edit DATA\ACCOUNTS.TXT to create accounts

  From this directory, run POL.EXE

 CLIENT
 ------    
  in your UO\LOGIN.CFG, Change all LoginServer entries to match your 
    IP address, Port 5003.

  Run CLIENT.EXE    

Please report any bugs you find to pol-distro@yahoogroups.com.

If you are running Linux kernal version 2.4.*, you will need to turn
multithreading off in pol.cfg.

IF YOU ARE RUNNING THE 094 DISTRO:

An Admin account exists by default. Username/Password: admin/admin

To figure out why your new merchants aren't responding to your words,
look at docs/nodes.txt.

The first time you start your server, it will first build the spawn regions. To
get them to spawn you must either log in and run .restartregionmanager or restart
the server- either will work fine. (This is considered a feature- we hope to 
figure out why it isn't doing it automatically.) Please do not shut down the server 
while it is building the datafiles (it is safe to shutdown while it is spawning). 
You will likely get messages saying spawnregion is running for several hours after 
startup. It is safe to play- this  just tells you that it is spawning the world.
