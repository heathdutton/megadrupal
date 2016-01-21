<?php
/**
 * @file
 * The email validator.
 */

/**
 * The email validator callback.
 */
function magic_forms_validators_email_validate($value, array $config) {
  return empty($value) || filter_var($value, FILTER_VALIDATE_EMAIL) !== FALSE;
}

/**
 * The email domain validator callback.
 */
function magic_forms_validators_email_domain_validate($value, array $config) {
  $valid = FALSE;
  if (magic_forms_validators_email_validate($value, $config)) {
    $valid = TRUE;

    $host = substr(strrchr($value, "@"), 1);
    foreach ((array) $config['domains'] as $domain) {
      $valid = preg_match("/^{$domain}$/", $host) == 1;
      if ($valid) {
        break;
      }
    }
  }

  return $valid;
}
