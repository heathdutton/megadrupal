<?php
/**
 * @file
 * File containing the mathematical functions wrt the matrix encoding.
 */

require_once 'BigNum.class.php';

/**
 * Extended euclidan GCD.
 *
 * @param string $a
 *   First operand.
 * @param string $b
 *   Second operand.
 *
 * @return array
 *   Divisors?
 */
function euclid($a, $b) {
  $result = array();
  $a = (string) $a;
  $b = (string) $b;
  while ($a !== '0' && $b !== '0') {
    $mod = BigNum::mod($a, $b);
    $result[] = BigNum::div(BigNum::sub($a, $mod), $b);
    $a = $b;
    $b = $mod;
  }
  return $result;
}

/**
 * Convert a numerator and denominator to an enumerated path.
 *
 * @param string $num
 *   Numerator.
 * @param string $den
 *   Denominator.
 *
 * @return string
 *   Enumerated path
 */
function nd2path($num, $den) {
  $path = euclid($num, $den);
  if (count($path) % 2 == 0) {
    array_pop($path);
    array_push($path, 1, 1);
  }
  $np = array();
  for ($idx = 0; $idx < count($path); $idx += 2) {
    $np[] = $path[$idx];
  }
  return implode('.', $np);
}

/**
 * Convert numerator and denominator into a path.
 *
 * @param string $num
 *   Numerator.
 * @param string $den
 *   Denominator.
 *
 * @return string
 *   Path.
 */
function nd2path2($num, $den) {
  $path = euclid($num, $den);
  $np = array();
  for ($idx = 0; $idx < count($path); $idx++) {
    $np[] = $path[$idx];
  }
  return implode('.', $np);
}

/**
 * Convert a matrix to an enumerated path.
 *
 * @param object $matrix
 *   Matrix.
 *
 * @return string
 *   Enumerated path.
 */
function matrix2path($matrix) {
  return nd2path($matrix['nv'], $matrix['dv']);
}

/**
 * Pretty print a set of matrices.
 *
 * @param array $matrices
 *   Array of matrices to pretty print.
 * @param array $result
 *   (optional) Pretty print a resulting matrix. Defaults to NULL.
 */
function printmatrices($matrices, $result = NULL) {
  $rows = array();
  foreach ($matrices as $matrix) {
    $rows[0][] = '+---+---+';
    $rows[1][] = sprintf("|%3s|%3s|", $matrix['nv'], $matrix['snv']);
    $rows[2][] = sprintf("|%3s|%3s|", $matrix['dv'], $matrix['sdv']);
    $rows[3][] = '+---+---+';
  }
  if ($result) {
    $rows[0][] = '         ';
    $rows[1][] = '   ===   ';
    $rows[2][] = '   ===   ';
    $rows[3][] = '         ';
    $rows[0][] = '+---+---+';
    $rows[1][] = sprintf("|%3s|%3s|", $result['nv'], $result['snv']);
    $rows[2][] = sprintf("|%3s|%3s|", $result['dv'], $result['sdv']);
    $rows[3][] = '+---+---+';
  }
  foreach ($rows as $row) {
    print implode('  ', $row) . "\n";
  }
}

/**
 * Convert a path into a set of matrices.
 *
 * @param string $path
 *   Enumerated path.
 *
 * @return array
 *   Matrices.
 */
function path2matrices($path) {
  $matrices = array();
  $matrices[] = array('nv' => '0', 'dv' => '1', 'snv' => '1', 'sdv' => '0');

  if ($path != '') {
    $parts = explode('.', $path);
    foreach ($parts as $part) {
      $matrices[] = array(
        'nv' => '1',
        'dv' => (string) $part,
        'snv' => '1',
        'sdv' => (string) ($part + 1)
      );
    }
  }
  return $matrices;
}

/**
 * Convert a path to an array of matrices.
 *
 * @param string $path
 *   Path to convert.
 *
 * @return array
 *   Matrices.
 */
function path2matrices2($path) {
  $matrices = array();
  #$matrices[] = array('nv' => '0', 'dv' => '1', 'snv' => '1', 'sdv' => '0');
  #$matrices[] = array('nv' => '1', 'dv' => '0', 'snv' => '0', 'sdv' => '1');

  if ($path != '') {
    $parts = explode('.', $path);
    foreach ($parts as $part) {
      $matrices[] = array('nv' => (string) $part, 'dv' => 1, 'snv' => '1', 'sdv' => 0);
    }
  }
  return $matrices;
}

/**
 * Invert a matrix.
 *
 * @param array $matrix
 *   Matrix to invert.
 *
 * @return array
 *   Inverted matrix.
 */
function matrixinverse($matrix) {
  return array(
    'nv' => -$matrix['sdv'],
    'dv' => $matrix['dv'],
    'snv' => $matrix['snv'],
    'sdv' => -$matrix['nv'],
  );
}

/**
 * Another inverse matrix...
 */
function matrixinverse2($matrix) {
  return array(
    'nv' => $matrix['sdv'],
    'dv' => -$matrix['dv'],
    'snv' => -$matrix['snv'],
    'sdv' => $matrix['nv'],
  );
}

/**
 * Multiply a set of matrices.
 *
 * @param array $matrices
 *   Array of matrices.
 *
 * @return array
 *   Resulting matrix.
 */
function matrixproduct($matrices) {
  $prev = array_shift($matrices);
  if (!$matrices) {
    return $prev;
  }
  foreach ($prev as &$v) {
    $v = (string) $v;
  }

  foreach ($matrices as $m) {
    foreach ($m as &$v) {
      $v = (string) $v;
    }

    $matrix['nv'] = BigNum::add(BigNum::mul($prev['nv'], $m['nv']), BigNum::mul($prev['snv'], $m['dv']));
    $matrix['dv'] = BigNum::add(BigNum::mul($prev['dv'], $m['nv']), BigNum::mul($prev['sdv'], $m['dv']));
    $matrix['snv'] = BigNum::add(BigNum::mul($prev['nv'], $m['snv']), BigNum::mul($prev['snv'], $m['sdv']));
    $matrix['sdv'] = BigNum::add(BigNum::mul($prev['dv'], $m['snv']), BigNum::mul($prev['sdv'], $m['sdv']));
    $prev = $matrix;
  }

  foreach ($matrix as &$v) {
    $v = (string) $v;
  }

  return $matrix;
}

/**
 * Calculate left value of matrix (nv / dv).
 */
function _get_lft($matrix) {
  return BigNum::div(BigNum::mul($matrix['nv'], variable_get('taxonomy_nested_intervals_factor', TAXONOMY_NESTED_INTERVALS_FACTOR)), $matrix['dv'], variable_get('taxonomy_nested_intervals_decimals', TAXONOMY_NESTED_INTERVALS_DECIMALS));
}

/**
 * Calculate right value of matrix (snv / sdv).
 */
function _get_rgt($matrix) {
  return BigNum::div(BigNum::mul($matrix['snv'], variable_get('taxonomy_nested_intervals_factor', TAXONOMY_NESTED_INTERVALS_FACTOR)), $matrix['sdv'], variable_get('taxonomy_nested_intervals_decimals', TAXONOMY_NESTED_INTERVALS_DECIMALS));
}
