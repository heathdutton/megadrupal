<?php

/*
 * @file
 *
 * Validation file for nif/cif/nie.
 * This file is included whenever the module needs to validate a nif.
 */

/**
 * Helper function for validating NIF/CIF/NIE number.
 *
 * @param $cif
 *  NIF/CIF/NIE number to validate.
 * @param $options
 *  Options to validate the numbers such instance otpions to have just some of
 *  the numbers, i.e exclude NIE.
 *
 * @return
 * Array of values if the number is correct:
 * - first_letter: First letter of the id number.
 * - last_letter: Last letter of the id number.
 * - number: Clean number (without letters).
 * - type: NIF, CIF or NIE
 * If the number is not correct, the function returns FALSE.
 */
function _field_nif_validate_dni_cif_nie($cif, $options = array()) {
  $cif = strtoupper($cif);
  $num = array();
  for ($i = 0; $i < 9; $i++) {
    $num[$i] = substr($cif, $i, 1);
  }

  // Check the general format of the NIF/CIF/NIE.
  if (!preg_match('/((^[A-Z]{1}[0-9]{7}[A-Z0-9]{1}$|^[T]{1}[A-Z0-9]{8}$)|^[0-9]{8}[A-Z]{1}$)/', $cif)) {
    return FALSE;
  }

  // Standard NIF numbers.
  if (isset($options['nif']) && $options['nif'] === 'nif') {
    if (preg_match('/(^[0-9]{8}[A-Z]{1}$)/', $cif)) {
      if ($num[8] == substr('TRWAGMYFPDXBNJZSQVHLCKE', substr($cif, 0, 8) % 23, 1)) {
        $last_letter = $num[8];
        array_pop($num);
        return array(
          'last_letter' => $last_letter,
          'number' => implode('', $num),
          'type' => 'NIF',
        );
      }
      else {
        return FALSE;
      }
    }
  }

  // CIF numbers.
  $suma = $num[2] + $num[4] + $num[6];
  for ($i = 1; $i < 8; $i += 2)
    $suma += substr((2 * $num[$i]), 0, 1) + substr((2 * $num[$i]), 1, 1);
  $n = 10 - substr($suma, strlen($suma) - 1, 1);

  // Special NIF numbers that are calculated the same way as the CIF.
  if (isset($options['nif']) && $options['nif'] === 'nif') {
    if (preg_match('/^[KLM]{1}/', $cif)) {
      if ($num[8] == chr(64 + $n)) {
        $last_letter = $num[8];
        $first_letter = $num[0];
        array_pop($num);
        array_shift($num);
        return array(
          'first_letter' => $first_letter,
          'last_letter' => $last_letter,
          'number' => implode('', $num),
          'type' => 'NIF',
        );
      }
      else {
        return FALSE;
      }
    }
  }

  // CIF number.
  if (isset($options['cif']) && $options['cif'] === 'cif') {
    if (preg_match('/^[ABCDEFGHJNPQRSUVW]{1}/', $cif)) {
      if ($num[8] == chr(64 + $n) || $num[8] == substr($n, strlen($n) - 1, 1)) {
        $last_letter = $num[8];
        $first_letter = $num[0];
        array_pop($num);
        array_shift($num);
        return array(
          'first_letter' => $first_letter,
          'last_letter' => $last_letter,
          'number' => implode('', $num),
          'type' => 'CIF',
        );
      }
      else {
        return FALSE;
      }
    }
  }

  // NIE check.
  if (isset($options['nie']) && $options['nie'] === 'nie') {
    //T
    if (preg_match('/^[T]{1}/', $cif)) {
      if ($num[8] == preg_match('/^[T]{1}[A-Z0-9]{8}$/', $cif)) {
        $last_letter = $num[8];
        $first_letter = $num[0];
        array_pop($num);
        array_shift($num);
        return array(
          'first_letter' => $first_letter,
          'last_letter' => $last_letter,
          'number' => implode('', $num),
          'type' => 'NIE',
        );
      }
      else {
        return FALSE;
      }
    }

    //XYZ
    if (preg_match('/^[XYZ]{1}/', $cif)) {
      if ($num[8] == substr('TRWAGMYFPDXBNJZSQVHLCKE', substr(str_replace(array('X', 'Y', 'Z'), array('0', '1', '2'), $cif), 0, 8) % 23, 1)) {
        $last_letter = $num[8];
        $first_letter = $num[0];
        array_pop($num);
        array_shift($num);
        return array(
          'first_letter' => $first_letter,
          'last_letter' => $last_letter,
          'number' => implode('', $num),
          'type' => 'NIE',
        );
      }
      else {
        return FALSE;
      }
    }
  }

  // If the number hasn't been validated yet, is not a valid NIF/CIF/NIE.
  return FALSE;
}
