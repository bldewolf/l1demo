#!/usr/bin/perl

use warnings;
use strict;

use List::Util qw(shuffle);

my $maze;
my $w = 64;
my $h = 48;

sub showmaze {
	foreach my $y (0 .. $h) {
		print join('', @{$maze->[$y]}), "\n";
	}
}

sub in_boundary {
	my ($x, $y) = @_;

	if($x < 1 or $x > $w - 1 or
		$y < 1 or $y > $h - 1) {
		return 0;
	}
	return 1;
}

for my $y (0 .. $h) {
	$maze->[$y] = [];
	for my $x (0 .. $w) {
		$maze->[$y][$x] = '#';
	}
}

#my @cells = ([rand($w), rand($h)]);
my @cells = ([1, 1]);
my $max = 0;

while(@cells) {
#	my $cell = splice(@cells, rand @cells, 1);
	$max = @cells if(@cells > $max);
	my $cell = pop @cells;
	my $added;
	my @dirs = shuffle([2, 0], [0, 2], [-2, 0], [0, -2]);
	for my $dir (@dirs) {
		my ($x, $y) = ($cell->[0] + $dir->[0], $cell->[1] + $dir->[1]);
		next if !in_boundary($x,$y);
		next if $maze->[$y][$x] ne '#';

		$maze->[$y][$x] = ' ';
		$maze->[$y - $dir->[1]/2][$x - $dir->[0]/2] = ' ';
		
		push @cells, [$x, $y];
	}
}

showmaze;
print "$max\n";

