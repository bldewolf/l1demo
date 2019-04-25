#!/usr/bin/perl

use warnings;
use strict;

my $x = 0;
while(defined(my $c = getc())) {
	$c = unpack("C", $c);
	printf "0x%02x, ", $c;
	if(++$x > 8) {
		$x = 0;
		printf "\n";
	}

}
