<?php
/**
 * @file
 * Drupal RestMini module validation check.
 */

/**
 * @throws Exception
 *   If $check isn't supported.
 *   If $check is empty, and $value not NULL.
 *
 * @param string $check
 *   Default: empty (~ return array with check name as key, and regex or description as value).
 * @param string|number $value
 *
 * @return boolean|array
 */
function restmini_validation_check($check = '', $value = NULL) {
  static $simple = array(
    'num' => '/^\d+$/',
    'alphanum' => '/^[\da-zA-Z]+$/',
    'alphanum_lowercase' => '/^[\da-z]+$/',
    'alphanum_uppercase' => '/^[\dA-Z]+$/',
    'hex' => '/^[\da-fA-F]+$/',
    'hex_lowercase' => '/^[\da-f]+$/',
    'hex_uppercase' => '/^[\dA-F]+$/',
    'name' => '/^[_a-zA-Z][_\da-zA-Z]*$/',
    'name_lowercase' => '/^[_a-z][_\da-z]*$/',
    'name_uppercase' => '/^[_A-Z][_\dA-Z]*$/',
  );

  if ($check) {
    // Prevent switch coersion error.
    $check = '' . $check;
    // String numbers etc.
    $value = '' . $value;

    if (isset($simple[$check])) {
      return (bool)preg_match($simple[$check], $value);
    }

    // PHP:Filter.
    switch ($check) {
      case 'email':
        // FILTER_VALIDATE_EMAIL doesn't reliably require .tld.
        return filter_var($value, FILTER_VALIDATE_EMAIL) && preg_match('/\.[a-zA-Z\d]+$/', $value);
      case 'url':
        return (bool)filter_var($value, FILTER_VALIDATE_URL);
      case 'http_url':
        return strpos($value, 'http') === 0 && filter_var($value, FILTER_VALIDATE_URL);
      case 'ip':
        return (bool)filter_var($value, FILTER_VALIDATE_IP);
    }

    // Printables.

    // Check all for char 127 (DEL).
    if (strpos(' ' . $value, chr(127))) {
      return FALSE;
    }

    // If multiline, escape newline and carriage return before checking for ASCII low.
    if (strpos($check, '_multiline')) {
      $value = str_replace(array("\r", "\n"), array('_R_', '_N_'), $value);
    }

    // ascii forbids lower+upper.
    if ($check == 'ascii' || $check == 'ascii_multiline') {
      // strcmp returns truthy if different.
      return !strcmp($value, filter_var($value, FILTER_UNSAFE_RAW, FILTER_FLAG_STRIP_LOW | FILTER_FLAG_STRIP_HIGH));
    }

    // The rest only forbids ASCII lower.
    if (strcmp($value, filter_var($value, FILTER_UNSAFE_RAW, FILTER_FLAG_STRIP_LOW))) {
      return FALSE;
    }

    switch ($check) {
      case 'printable':
      case 'printable_multiline':
        // All done.
        return TRUE;
      case 'plain':
      case 'plain_multiline':
        // Check against tags removed.
        return !strcmp($value, strip_tags($value));
    }

    throw new Exception('Failed to do validation check, $check[' . $check . '] type[' . gettype($check) . '] not supported');
  }
  elseif ($value === NULL) {
    return array_merge(
      $simple,
      array(
        'email' => 'ASCII email address',
        'url' => 'URL',
        'http_url' => 'http(s) URL',
        'ip' => 'IP address',
        'ascii' => 'printable ASCII',
        'ascii_multiline' => 'printable ASCII plus newline and carriage return',
        'printable' => 'printable any charset',
        'printable_multiline' => 'printable any charset plus newline and carriage return',
        'plain' => 'no-tags printable any charset',
        'plain_multiline' => 'no-tags printable any charset plus newline and carriage return',
      )
    );
  }

  throw new Exception('Failed to do validation check, arg $check empty');
}
