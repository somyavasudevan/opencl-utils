#!/usr/bin/perl -w
use strict;

open(CL, '<../include/CL/cl.h') or die "Can't open cl.h";
open(CLGL, '<../include/CL/cl_gl.h') or die "Can't open cl_gl.h"; 

my ($ret, $func, $args);

print "#include \"dynamiclib.h\"\n\n\n";

while(<CL>) {
	chomp;
	next unless $_ =~ /CL_API_ENTRY\s*(.*)\s*CL_API_CALL/;
	$ret = $1;
	my $fa = "";
	while(<CL>) {
		$fa .= $_;
		last if $_ =~ /;$/;
	}
	chomp $fa;

	my ($argType, $argName);
	$fa =~ s/\s+?\).*;$/\)/g; # remove API end
	$fa =~ /(.*)\((.*)\)/s;
	$func = $1;
	$args = $2;
	$argType = $argName = $args;
	$argType =~ s|/\*.*\*/||g;
	$argType =~ s/\n//sg;
	$argType =~ s/\s+/ /sg;
	$argName =~ s|.*/\*||g;
	$argName =~ s|\*/||g;
	$argName =~ s/(\s*)?(\n)?(\s*)?//sg;
	$args =~ s$(/\*|\*/)$$g; # uncomment names
	
	print "$ret $func ($args) {\n";
	inside_function($ret, $func, $argType, $argName);
	print "}\n\n";
}

close(CL);
close(CLGL);

sub inside_function {
	my $funcRet = shift;
	my $funcName = shift;
	my $funcArgsType = shift;
	my $funcArgsName = shift;

	print "\tstatic ($funcRet)(*func)($funcArgsType) = NULL;\n";
	print "\tif(!func) func = getFunction(\"$funcName\");\n";
	print "\t(*func)($funcArgsName);\n";
}
