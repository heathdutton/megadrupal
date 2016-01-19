<?php
/**
 * @file
 * The URL validator.
 */

/**
 * The URL validator callback.
 */
function magic_forms_validators_url_validate($value, array $config) {
  return empty($value) || filter_var($value, FILTER_VALIDATE_URL) !== FALSE;
}

/**
 * The URL domain validator callback.
 */
function magic_forms_validators_url_domain_validate($value, array $config) {
  $valid = FALSE;
  if (magic_forms_validators_url_validate($value, $config)) {
    $url = parse_url($value);
    if ($url) {
      $valid = TRUE;

      if ($valid && isset($config['schemes'])) {
        $valid = in_array($url['scheme'], (array) $config['schemes']);
      }

      if ($valid && isset($config['domains'])) {
        foreach ((array) $config['domains'] as $domain) {
          $valid = preg_match("/^{$domain}$/", $url['host']) == 1;
          if ($valid) {
            break;
          }
        }
      }
    }
  }

  return $valid;
}
