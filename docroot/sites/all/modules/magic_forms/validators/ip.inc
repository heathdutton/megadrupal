<?php
/**
 * @file
 * The IP validator.
 */

/**
 * The IP validator callback.
 */
function magic_forms_validators_ip_validate($value, array $config) {
  return empty($value) || filter_var($value, FILTER_VALIDATE_IP) !== FALSE;
}
