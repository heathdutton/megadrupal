<?php

$matrices = array();
$matrices[] = array('nv' => 0, 'dv' => 1, 'snv' => 1, 'sdv' => 0);
$matrices[] = array('nv' => 1, 'dv' => 3, 'snv' => 0, 'sdv' => 3);
$matrices[] = array('nv' => -3, 'dv' => 2, 'snv' => 5, 'sdv' => -3);
$matrices[] = array('nv' => 2, 'dv' => 8, 'snv' => 5, 'sdv' => 13);
$matrix = matrixproduct($matrices);

printmatrices($matrices, $matrix);
print nd2path($matrix['nv'], $matrix['dv']);
#return;

$paths = array();
$paths[] = "3";
$paths[] = "4";
$paths[] = "6";
$paths[] = "1.3";
$paths[] = "1.2";
$paths[] = "1.3.1";
$paths[] = "1.2.2";

foreach ($paths as $path) {
  print "Path: $path\n";
  $matrices = path2matrices($path);
  $matrix = matrixproduct($matrices);
  printmatrices($matrices, $matrix);
  #var_dump(euclid($matrix['nv'], $matrix['dv']));
  print nd2path($matrix['nv'], $matrix['dv']) . "\n";

  print "left: " . ($matrix['nv'] / $matrix['dv']) . "\n";
  print "right: " . ($matrix['snv'] / $matrix['sdv']) . "\n";
  print "\n";
#  return;
}

return;

$move = array(
  "1.1.1",
  "1.1.1.1",
  "1.1.1.2",
  "1.1.1.3",
);
$move2 = array(
  "4",
  "4.1",
  "4.2",
  "4.3",
);
#$p0 = matrixproduct(path2matrices("1.1"));
#$p1 = matrixproduct(path2matrices(""));
$p0 = matrixproduct(path2matrices(""));
$p1 = matrixproduct(path2matrices("1.1"));
printmatrices(array($p0));
printmatrices(array($p1));
foreach ($move2 as $path) {
  $m0 = matrixproduct(path2matrices($path));
  $smp = array(
    $p1,
    array('nv' => 1, 'dv' => -3, 'snv' => 0, 'sdv' => 1),
    matrixinverse($p0),
  );
  #printmatrices($smp, matrixproduct($smp));
  $m1 = array();
  $m1[] = matrixproduct($smp);
  $m1[] = $m0;
  #printmatrices(array($m0));
  #printmatrices(array($p0));
  #printmatrices(array($p1));
  $dst = matrixproduct($m1);
  printmatrices($m1, $dst);
  #printmatrices(array($dst));
  print "$path => " . nd2path($dst['nv'], $dst['dv']) . "\n";
}


