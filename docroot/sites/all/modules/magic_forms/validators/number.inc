<?php
/**
 * @file
 * The number validator.
 */

/**
 * The number validator callback.
 */
function magic_forms_validators_number_validate($value, array $config) {
  if (isset($config['decimal']) && $config['decimal']) {
    // @todo
  }
  else {
    if (
      !ctype_digit(strval($value)) ||
      (isset($config['min']) && $value < $config['min']) ||
      (isset($config['max']) && $value > $config['max'])
    ) {
      return FALSE;
    }
  }

  return TRUE;
}
