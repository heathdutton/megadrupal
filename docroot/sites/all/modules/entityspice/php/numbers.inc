<?php

/**
 * Converts number to arbitrary base.
 *
 * @TODO has bug, does not work? a random big number throws undefiend index 63
 * see what ascii-63 or something like that is.
 */
function _entityspice_to_base($num, $base = 62) {
  $base_translate = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';

  $r = $num  % $base ;
  $res = $base_translate[$r];
  $q = floor($num / $base);

  while ($q) {
    $r = $q % $base;
    $q =floor($q / $base);
    $res = $base_translate[$r] . $res;
  }

  return $res;
}

/**
 * Converts number from arbitrary base to decimal.
 */
function _entityspice_to_decimal($num, $base = 62) {
  $base_translate = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';

  $limit = strlen($num);
  $res=strpos($base_translate, $num[0]);

  for($i = 1; $i < $limit; $i++) {
    $res = $base * $res + strpos($base_translate, $num[$i]);
  }

  return $res;
}
