#!/usr/bin/perl -w
use strict;


my $filename = shift;

open(CL, "<$filename") or die "Can't open cl.h";

my ($funcRet, $funcName, $args);

print "#include \"dynamiclib.h\"\n";
print "#include \"$filename\"\n\n\n";

while(<CL>) {
	chomp;
	next unless $_ =~ /CL_API_ENTRY\s*(.*)\s*CL_API_CALL/;
	$funcRet = $1;
	my $fa = "";
	while(<CL>) {
		$fa .= $_;
		last if $_ =~ /;$/;
	}

	chomp $fa;
	$fa =~ s/\s*\).*;\s*$/\)/g; # remove api
	$fa =~ /([^(]*)\((.*)\)/s;
 	$funcName = $1;
	$args = $2;
	
	# split params by , (ignoring function pointers)
	(my @args) = $args =~  m/([^(,]*(?:\([^)]+\)[^(),]*)*[^),]*),?/g;

	my (@argsTypes, @argsNames);

	my $funcArgSig = "";
	my $tmp;
	foreach my $arg (@args) {
		
		next if ($arg eq "");
		my ($type, $name) = $arg =~ m|\s*(.*)\s*/\*\s*(.*)\s*\*/|s;


		if ($arg =~ /,/) {
			$tmp = $type;
			$type =~ s/\(\*.*?\)/\(\*\)/g 
		}
		
		push @argsTypes, $type;
		push @argsNames, $name;

		if ($arg =~ /,/) {
			$funcArgSig .= "$tmp,";
		} else {
			$funcArgSig .= "$type $name,";
		}
	}

	chop($funcArgSig); #last ,

	my $funcArgsNames = ($#argsNames != -1)? join ', ',@argsNames : "";
	my $funcArgsTypes = ($#argsTypes != -1)? join ', ',@argsTypes : "";

	$args =~ s$(/\*|\*/)$$g; # uncomment names
 	print "$funcRet $funcName ($funcArgSig) {\n";
	print "\tstatic $funcRet (*func)($funcArgsTypes) = NULL;\n";
	print "\tif(!func) func = getFunction(\"$funcName\");\n";
	print "\treturn (*func)($funcArgsNames);\n";
 	print "}\n\n";
}

close(CL);
