#!/usr/bin/perl

use warnings;
use strict;

my $case = 0;
for my $a (0..3) {
for my $b (0..3) {
	next if($b == $a);
for my $c (0..3) {
	next if($c == $a);
	next if($c == $b);
for my $d (0..3) {
	next if($d == $a);
	next if($d == $b);
	next if($d == $c);
	print "case " . ($case++) . ": return " .
		(($a << 6) | ($b << 4) | ($c << 2) | ($d)) .
		";\n";
}
}
}
}
