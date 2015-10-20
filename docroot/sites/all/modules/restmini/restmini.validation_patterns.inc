<?php
/**
 * @file
 * Drupal RestMini module validation patterns.
 *
 * @see RestMini::validationPattern()
 */

$validation_patterns = array(
  // Default data type is string, so we don't need to declare 'type' => 'string' here.
  // Default 'optional' is FALSE, and RestMini::validationPattern() will add that bucket.

  // Patterns using predefined 'check'.
  // Simple checks.
  'num' => array(
    'title' => 'digits only',
    'check' => 'num',
  ),
  'alphanum' => array(
    'title' => 'ASCII letters and numbers',
    'check' => 'alphanum',
  ),
  'alphanum_lowercase' => array(
    'title' => 'lowercase ASCII letters and numbers',
    'check' => 'alphanum_lowercase',
  ),
  'alphanum_uppercase' => array(
    'title' => 'uppercase ASCII letters and numbers',
    'check' => 'alphanum_uppercase',
  ),
  'hex' => array(
    'title' => 'hexadecimal',
    'check' => 'hex',
  ),
  'hex_lowercase' => array(
    'title' => 'lowercase hexadecimal',
    'check' => 'hex_lowercase',
  ),
  'hex_uppercase' => array(
    'title' => 'uppercase hexadecimal',
    'check' => 'hex_uppercase',
  ),
  'name' => array(
    'title' => 'a name',
    'check' => 'name',
  ),
  'name_lowercase' => array(
    'title' => 'a lowercase name',
    'check' => 'name_lowercase',
  ),

  // Complex checks.
  'ascii' => array(
    'title' => 'printable ASCII',
    'check' => 'ascii',
  ),
  'ascii_multiline' => array(
    'title' => 'printable ASCII plus newline and carriage return',
    'check' => 'ascii_multiline',
  ),
  'printable' => array(
    'title' => 'printable any charset',
    'check' => 'printable',
  ),
  'printable_multiline' => array(
    'title' => 'printable any charset plus newline and carriage return',
    'check' => 'printable_multiline',
  ),
  'plain' => array(
    'title' => 'no-tags printable any charset',
    'check' => 'plain',
  ),
  'plain_multiline' => array(
    'title' => 'no-tags printable any charset plus newline and carriage return',
    'check' => 'plain_multiline',
  ),
  'email' => array(
    'title' => 'an ASCII email address',
    'max_length' => 255,
    'check' => 'email',
  ),
  'url' => array(
    'title' => 'a URL',
    'max_length' => 255,
    'check' => 'url',
  ),
  'http_url' => array(
    'title' => 'a http(s) URL',
    'max_length' => 255,
    'check' => 'http_url',
  ),
  'ip' => array(
    'title' => 'an IP address',
    'max_length' => 45, // IPv6.
    'check' => 'ip',
  ),

  // Patterns that don't use predefined 'check'.
  'integer_positive' => array(
    'title' => 'a positive integer',
    'type' => 'integer',
    'min' => 1,
  ),
  'base64' => array(
    'title' => 'base 64 encoded',
    'regex' => '/^[a-zA-Z\d\+\/\=]+$/',
  ),
  'uuid' => array(
    'title' => 'a lowercase uuid',
    'exact_length' => 36,
    'regex' => '/^[\da-f]{8}\-[\da-f]{4}\-[\da-f]{4}\-[\da-f]{4}\-[\da-f]{12}$/',
  ),
  'time_iso8601' => array(
    'title' => 'an ISO-8601 YYYY-MM-DDTHH:ii:ss(.mmm)?(Z|+00:00) timestamp',
    // max_length:29 ~ 'YYYY-MM-DDTHH:ii:ss.mmm+00:00'.
    // exact_length/max_length/min_length are applied before regex.
    'max_length' => 29,
    'regex' => '/^\d{4}\-\d{2}\-\d{2}T\d{2}:\d{2}:\d{2}(\.\d{3})?(Z|[+\-]\d{2}:\d{2})$/',
  ),
  'time_iso8601_nozone' => array(
    'title' => 'an ISO-8601 YYYY-MM-DD HH:ii:ss timestamp',
    // exact_length:29 ~ 'YYYY-MM-DD HH:ii:ss'.
    // exact_length/max_length/min_length are applied before regex.
    'exact_length' => 19,
    'regex' => '/^\d{4}\-\d{2}\-\d{2} \d{2}:\d{2}:\d{2}$/',
  ),
  'date_iso8601_nozone' => array(
    'title' => 'an ISO-8601 YYYY-MM-DD date',
    // max_length:29: to support 'YYYY-MM-DDTHH:ii:ss.mmm+00:00'.
    // exact_length/max_length/min_length are applied before truncate.
    'max_length' => 29,
    // truncate:10: we only use the 'YYYY-MM-DD' part.
    // truncate is applied before regex.
    'truncate' => 10,
    'regex' => '/^\d{4}\-\d{2}\-\d{2}$/',
  ),
);
