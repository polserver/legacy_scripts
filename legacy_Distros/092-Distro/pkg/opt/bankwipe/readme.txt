Bankwipe.src
by Jeff Freeman and Zulu

Our storage.txt file is now 39 MB and something had to be done pretty quick. Jeff Freeman was nice enough to share his storage wipe script.  His script is based on accounts that when they log in, they have a cprop written to them.  It would then go through each account and removed the old ones without that cprop.

I have updated the script to do this: This script will clean out all of the old bank accounts that belong to accounts that were deleted.

- It will find all of the accounts and store each accounts player
characters.

- Search the storage.txt file's bank storage section and remove the bank boxes for accounts that were deleted.

*Installing*
copy bankwipe to          \pol\scripts
edit start.src to add        start_script( "bankwipe");

Zulu
http://www.zuluhotel.com
