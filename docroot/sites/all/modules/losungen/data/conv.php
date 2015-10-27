<?php

/**
 * @file
 * csv-converter.
 *
 * (c) Micha Glave 2012
 */

if (count($argv) != 3) {
  print "Syntax: php $argv[0] input-file.csv YYYY\n\n";
  exit();
}

ini_set('auto_detect_line_endings', TRUE);
$in = fopen($argv[1], "r");
$out = fopen("losungen" . $argv[2] . ".dat", "w");
$buffer = "                                                                  ";
// Write header!
fwrite($out, "Binary File - don't touch!   \n" .
  $buffer . $buffer . $buffer . $buffer . $buffer . $buffer .
  $buffer . $buffer . $buffer . $buffer . $buffer . $buffer .
  $buffer . $buffer . $buffer . $buffer . $buffer . $buffer .
  $buffer . $buffer . $buffer . $buffer . $buffer . $buffer .
  $buffer . $buffer . $buffer . $buffer . $buffer . $buffer .
  $buffer . $buffer . $buffer . $buffer . "\n"
);

// Ignore header.
$data = fgetcsv($in, 1024, "\t");

$i = 0;
while (($data = fgetcsv($in, 1024, "\t")) !== FALSE) {
  $start = ftell($out);
  fseek($out, 30 + ($i * 6));
  fwrite($out, "$start");
  fseek($out, $start);
  fwrite($out, iconv("iso-8859-1", "utf8", implode($data, "\t")) . "\n");
  $i++;
}

$end = ftell($out);
fseek($out, 30 + $i * 6);
fwrite($out, "$end");

fclose($in);
fclose($out);
