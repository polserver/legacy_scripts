

sub iteritem
{
  my @dircontent;

  print "$currentdir\n";
  opendir (CHECKDIR, $currentdir) or die ("painfully");  
  @dircontent = readdir(CHECKDIR);
  foreach $dirobject (@dircontent)
  {
     $dirobject =~ s/\n//;
 #    print "$dirobject before itemdesc check\n";
     if ($dirobject eq "itemdesc.cfg")
     {
        addtofile($currentdir);
        next;
     }
#     print "$dirobject before directory check\n";
#     $anykey = <STDIN>;
     if (opendir (TESTDIR, "$currentdir/$dirobject"))
     {
        close TESTDIR;
        if (($dirobject ne ".") && ($dirobject ne ".."))
        {
#          print "I'm in! $dirobject\n";
           push @dirstocheck, "$currentdir/$dirobject";
        }
     }
  }  

}

sub addtofile
{
  my $dirpath;
  $dirpath = pop @_;
  $fullpath= "$dirpath/itemdesc.cfg";

open (ITEMFILE, "<$fullpath") or die "error opening $fullpath.";

print ITEMLOG "\n\n$fullpath\n";
while ($linefromfile = <ITEMFILE>)
   {
     $toadd = "";
     next if ($linefromfile =~ /^[\/#{]/);
     if ($linefromfile =~ /^\s*((Item)|(Container)|(Door))/i) 
     {
        $toadd = $linefromfile;
        $toadd =~ s/\n//g;
        $toadd = "\n$toadd";
     }
     if ($linefromfile =~ /^\s*name/i)
     {
        $linefromfile =~ s/^\s*//;
        $linefromfile =~ s/\n//g;
        $toadd = "\t\t\t$linefromfile";
     }
     print ITEMLOG $toadd if ($toadd);
   }

close (ITEMFILE);

}

### MAIN

## Check to make sure it is in the proper directory

opendir(DIR00, '.') or die "Couldn't access directory";
@rootdircontent = readdir(DIR00);

foreach $rootentry (@rootdircontent)
{
   if ($rootentry =~ /pol\.exe/) {$currectdir = "yes"}
}

unless ($currectdir) {die "Please place this file in the POL root directory."}
@dirstocheck[0]=".";

closedir DIR00;

open (ITEMLOG, ">>itemlog.txt");
#call the main function

while ($currentdir = shift(@dirstocheck))
{
   iteritem;
}
