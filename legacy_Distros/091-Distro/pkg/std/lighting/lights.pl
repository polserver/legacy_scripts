#!/usr/bin/perl -w

open(F1,"items.txt") or die "$!";
open(F2,">it2.txt") or die "$!";

@face = ( 
	"", # 0
	"b26,a29,b1d,a27,b1a,a26,a23,a25,a18,e31,19aa,19bb,1f2b,197e,198a,", # 1  
        "1853,1854,1857,1858,1849,184a,184d,184e,a0f,a28,142c,142f,1430,1433,1434,1437,1c14,a24,de3,fac,e2d,e2f,", # 2
        "", # 3
        "", # 4
        "a0a,a0c,", # 5
        "", # 6
        "", # 7
        "a05,a07,", # 8
        "", # 9
        "", # 10
        "a02,a00,", # 11
        "46b,930,47b,945,961,", # 12
        "", # 13
        "9fd,9fb,", # 14
        "", # 15
        "252,3dd,", # 16
        "253,", # 17
        "", # 18
        "462,92b,475,937,953,", # 19
        "254,", # 20
        "", # 21
        "", # 22
        "", # 23
        "e,22,98,ba,bc,ca,5c,13a,154,156,15c,1c4,207,", # 24
        "f,23,99,b9,bb,cb,13b,155,157,15d,1c5,208,", # 25
        "", # 26
        "", # 27
        "3b,5e,83,8d,fc,102,108,163,1d3,209,295,29b,2ac,3de,", # 28
        "f64,a12,b20,b21,b22,b23,b24,b25,a22,", # 29 
        "a15,", # 30
);

while(defined($ligne=<F1>)) {

  if ($ligne =~ /^\tSerial/) {
    print F2 $ligne;
    $ecrit = 1;
    $facing = 0;
    $obj = "0";

    while(defined($lig=<F1>)) {

      if ($lig =~ /^\}/) {
	if ($facing==0) {
	  for($i = 0 ; $i <= 30 ; $i++) {
	    @itemlist = split(/,/ , $face[$i]);
            if (scalar(grep($_ eq $obj,@itemlist))>=1) {
	      $facing = $i;
              print "$obj\tFacing\t$i\n";
              print F2 "\tFacing\t$i\n";
            }
	  }
	}
	print F2 $lig;
	last;
      }
	  
      if ($lig =~ /\tObjType\t0x0*(.*)\n/) {
	$obj = $1;
      }
      if ($lig =~ /^\tFacing\t(.*)\n/) {
	$facing = $1;
	$ecrit = 0;
      }

      print(F2 "$lig") if ($ecrit==1);
    }
  }
  else
  { 
    print F2 $ligne;
  }

}

close F1;
close F2;
