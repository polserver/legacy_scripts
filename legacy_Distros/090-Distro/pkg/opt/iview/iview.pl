#####################################################################
## Written by: Mercy
## Version 1.0
## Created: Aug 2, 2000.
## Searches POL directories for all itemdesc.cfg files and parses them
## displaying the information in human readable format.
#####################################################################

use strict;

## CONFIG ##
my $ITEMFILE	= 'itemview.txt';
my $FREEFILE	= 'itemfree.txt';
my $POLEXE		= 'pol.exe';

## GLOBALS ##
my %itemid;	## Key = itemid  Value = @(type, name, fileFound)
my %freeid;	## Key = start-end  Value = 1;

## CODE ##
## Check if we are in POL root dir.
die "This is not the POL root directory!\n" if (!-e $POLEXE);
process(".");
findFreeRanges();
createReportFiles();
exit;

## FUNCTIONS ##
## Recursive function...
sub process {
	my ($file, $path) = @_;
	my $curpath;

	## To make the filepath display correct.
	if ($path) {
		$curpath = $path."/".$file;
	} else {
		$curpath = $file;
	}

	## Only searches itemdesc.cfg files right now.
	if ($file =~ m/itemdesc.cfg/i) {
		open(ITEM, "$curpath") || die "Cannor open $curpath: $!\n";
		my $line;
		my ($type, $curid);
		my @data;
		while ($line = <ITEM>) {
			chomp($line);
			if ($line =~ m/^((Item)|(Container)|(Door)|(Armor)|(Weapon))/i) {
				($type, $curid) = (split(/\s+/, $line))[0,1];
				$curid = hex($curid);
				$data[0] = $type;
			}
			if ($line =~ m/Name/i) {
				$line =~ s/Name//i;
				$line =~ s/\s//g;
				$data[1] = $line;
			}
			if ($line =~ m/\{/) {
				$data[2] = $curpath;
				@{$itemid{$curid}} = @data;
				@data = {};
			}
		}
	}

	## If directory...
	if (opendir(DIR, "$curpath")) {
		print "Processing: $curpath\n";
		my @rootdir = readdir(DIR);
		close(DIR);
		my $fil;
		foreach $fil (@rootdir) {
			next if ($fil =~ m/^\./);
			process($fil, $curpath);
		}
	}
}

sub findFreeRanges {
	my ($id, $curid);
	my $maxid = int(0xFFFF);
	my ($pid, $ide);
	my $ids = -1;
	my $start;
	my $ended;

	## Find the free IDs.
	for ($id = 0; $id <= $maxid; $id++) {
		if ($ids == -1 && !defined($itemid{$id})) {
			$ids = $id;
		}
		$ide = $id - 1;
		if ($ids != -1 && defined($itemid{$id})) {
			my $rng = sprintf("0x%04x-0x%04x", $ids, $ide);
			if ($ids == $ide) {
				$rng = sprintf("0x%04x", $ids);
			}
			$freeid{$rng} = 1;
			$ids = -1;
		}
	}
}


sub createReportFiles {
	open(VIEW, ">$ITEMFILE") || die "Could not create: $ITEMFILE\n";
	open(FREE, ">$FREEFILE") || die "Could not create: $FREEFILE\n";

	my ($itm, $rng, $out);
	foreach $itm (sort {hex($a) <=> hex($b)} (keys(%itemid))) {
		my @dat = @{$itemid{$itm}};
 		$out = sprintf("%-10s 0x%04x %-30s %-40s\n", $dat[0], $itm, $dat[1], $dat[2]);
		print VIEW $out;
	}

	foreach $rng (sort(keys(%freeid))) {
		print FREE "$rng\n";
	}

	close(VIEW);
	close(FREE);

	print "Look in $ITEMFILE for item IDs\n";
	print "Look in $FREEFILE for free item ID ranges\n";
}