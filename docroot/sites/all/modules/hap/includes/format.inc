<?php

/**
 * Implements hook_hap_info().
 */
function hap_format_hap_info() {
  return array(
    'name' => 'Format',
    'description' => t('Theme wrappers and formatted output functions.'),
    'hooks' => array('theme'),
  );
}


/**
 * Implements hook_theme().
 */
function hap_format_theme() {
  return array(
    // Renders a definition list.
    'hap_format_definition_list' => array(
      'path' => hap_path('theme'),
      'file' => 'format.inc',
      'variables' => array(
        'items' => array(),
        'attributes' => array(),
      ),
    ),

    // Formats a date. This was originally created for Drupal's date form
    // element.
    'hap_format_date' => array(
      'path' => hap_path('theme'),
      'file' => 'format.inc',
      'variables' => array(
        'year' => NULL,
        'month' => NULL,
        'day' => NULL,
        'hour' => NULL,
        'minute' => NULL,
        'second' => NULL,
        'format' => 'm/d/Y',
      ),
    ),
  );
}


/**
 * Output a number in its ordinal form.
 *
 * Example 9 becomes 9th, 32 becomes 32nd, 101 becomes 101st, and so on. Note
 * that this is appropriate for English-based strings and may not translate
 * well in other languages.
 */
function hap_format_ordinal($number){
  $ones_digit = substr($number,-1);
  $tens_digit = substr($number, -2, -1);
  return $number . (
    ($ones_digit == 1 && $tens_digit == 1) ||
    ($ones_digit == 2 && $tens_digit == 1) ||
    ($ones_digit == 3 && $tens_digit == 1) ||
    ($ones_digit > 3) ||
    ($ones_digit == 0)
    ? 'th'
    : ($ones_digit == 3 ? 'rd' : ($ones_digit == 2 ? 'nd' : 'st'))
  );
}

/**
 * Get a whatever-separated list of applicable elements in an array. For
 * example:
 *  - printing "City", "State" when you don't know if those variables are even set.
 */
function hap_format_seperator() {
  $elements = func_get_args();
  $seperator = array_shift($elements);

  foreach ($elements as $key => &$element) {
    if (empty($element)) {
      unset($elements[$key]);
    }
  }
  return implode($seperator, $elements);
}

/**
 * This replicates the functionality PHP's http_build_url(). You can use this
 * function if your copy of PHP was not compiled with the HTTP extension.
 *
 * @param  array $parts return value of parse_url()
 * @return a string representation of the URL.
 */
function hap_format_unparse_url($parts) {
  $scheme = isset($parts['scheme']) ? $parts['scheme'] . '://' : '';
  $host = isset($parts['host']) ? $parts['host'] : '';
  $port = isset($parts['port']) ? ':' . $parts['port'] : '';
  $user = isset($parts['user']) ? $parts['user'] : '';
  $pass = isset($parts['pass']) ? ':' . $parts['pass'] : '';
  $pass = ($user || $pass) ? "$pass@" : '';
  $path = isset($parts['path']) ? $parts['path'] : '';
  $query = isset($parts['query']) ? '?' . $parts['query'] : '';
  $fragment = isset($parts['fragment']) ? '#' . $parts['fragment'] : '';
  return "$scheme$user$pass$host$port$path$query$fragment";
}
