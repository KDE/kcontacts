#!/usr/bin/perl

if ( @ARGV != 1 ) {
  print STDERR "Missing arg: filename\n";
  exit 1;
}

$file = $ARGV[0];

if ( !open( IN, "$file" ) ) {
  print STDERR "Unable to open '$file'\n";
  exit 1;
}

while( <IN> ) {
  if (/^VERSION:(.*)$/ ) {
    $version = $1;
    if ( $version eq "2.1" ) { $options = "--vcard21"; }
  }
}

close IN;

$ref = "$file.ref";

if ( !open( REF, "$ref" ) ) {
  print STDERR "Unable to open $ref\n";
  exit 1;
}

while( <REF> ) {
  push @ref, $_;
}

close REF;

if ( !open( READ, "./testread $file $options 2> /dev/null |" ) ) {
  print STDERR "Unable to open testread\n";
  exit 1;
}

print "Checking '$file':\n";

$error = 0;
$i = 0;
while( <READ> ) {
  $out = $_;
  $ref = @ref[$i++];

  if ( $out ne $ref ) {
    if ( $ref =~ /^UID/ && $out =~ /^UID/ ) { next; }
    $error++;
    print "  Expected      : $ref";
    print "  Parser output : $out";
  }
}

close READ;

if ( $error > 0 ) {
  print "\n  FAILED: $error errors found.\n";
  system "touch FAILED";
  exit 1;
} else {
  print "  OK\n";
}

exit 0;
