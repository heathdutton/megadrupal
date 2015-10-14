<?php

/**
 * @file
 * Hansel switches
 *
 * Switches from Hansel core are located in this file.
 * The definitions resides in hansel.module
 *
 * @see hansel_hansel_switch_types()
 */

/**
 * Callback for "url argument" switch to compare a given value.
 *
 * @param array $arguments
 * @param string $value
 * @return boolean
 */
function hansel_switch_url_argument_compare($arguments, $value) {
  if (empty($arguments['argument'])) {
    $arg = hansel_arg(0);
  }
  else {
    $arg = hansel_arg(((int) $arguments['argument']) - 1);
  }

  // Check for <empty>
  if ($arg == '' && $value == '<empty>') {
    return TRUE;
  }
  
  // Check for <front>
  global $_hansel_test_path;
  $path = empty($_hansel_test_path) ? $_GET['q'] : $_hansel_test_path;
  if ($value == '<front>' && $path == variable_get('site_frontpage', 'node')) {
    return TRUE;
  }

  if (empty($arguments['regex'])) {
    if (empty($arguments['cs'])) {
      // Use case insensitive matching.
      return drupal_strtolower($arg) == drupal_strtolower($value);
    }
    else {
      return $arg == $value;
    }
  }
  else {
    // Backslash forward slashes
    $pattern = str_replace('/', '\\/', $value);
    $pattern = "/$pattern/s";
    if (empty($arguments['cs'])) {
      // Use case insensitive matching.
      $pattern .= 'i';
    }
    return (bool) @preg_match($pattern, $arg);
  }
}

/**
 * Callback for "url argument" switch to generate the information line.
 *
 * @param array $arguments
 * @return string
 */
function hansel_switch_url_argument_info($arguments) {
  if (empty($arguments)) {
    return '1';
  }
  return $arguments['argument'];
}

/**
 * Callback for "PHP code" switch to compare a given value.
 *
 * @param array $arguments
 * @param string $value
 * @return boolean
 */
function hansel_switch_php_argument_compare($arguments, $value) {
  if (module_exists('php')) {
    return php_eval($arguments['php_argument']) == $value;
  }
}

/**
 * Callback for "node id" switch to compare a given value.
 *
 * @param array $arguments
 * @param string $value
 * @return boolean
 */
function hansel_switch_node_id_compare($arguments, $value) {
  if (hansel_arg(0) == 'node' && is_numeric(hansel_arg(1))) {
    return hansel_arg(1) == $value;
  }
  return FALSE;
}

/**
 * Callback for "node id" switch to compare a given value.
 *
 * @param array $arguments
 * @param string $value
 * @return boolean
 */
function hansel_switch_node_type_compare($arguments, $value) {
  if (drupal_strtolower(hansel_arg(0)) == 'node' && is_numeric(hansel_arg(1))) {
    if ($node = node_load(hansel_arg(1))) {
      return drupal_strtolower($node->type) == drupal_strtolower($value);
    }
  }
  return FALSE;
}

/**
 * Callback for "node age" switch to compare a given value.
 *
 * @param array $arguments
 * @param string $value
 * @return boolean
 */
function hansel_switch_node_age_compare($arguments, $value) {
  if (drupal_strtolower(hansel_arg(0)) == 'node' && is_numeric(hansel_arg(1))) {
    if ($node = node_load(hansel_arg(1))) {
      $unit = empty($arguments['unit']) ? 86400 : $arguments['unit'];
      $age = (REQUEST_TIME - $node->created) / $unit;
      // Check for ranges (e.g. "0-100" or "0.5-1").
      if (preg_match('/^([0-9\\.]+)\\-([0-9\\.]+)$/si', $value, $match)) {
        $min = (float) $match[1];
        $max = (float) $match[2];
        return ($age >= $min && $age < $max);
      }
      // Check for value with operator (e.g. "< 3", "<3", "> 4" or "<=0.5").
      elseif (preg_match('/^(\\<|\\>|\\<\\=|\\>\\=) *([0-9\\.]+)$/si', $value, $match)) {
        $operator = $match[1];
        $check_value = (float) $match[2];
        switch ($operator) {
          case '<':
            return ($age < $check_value);
          case '>':
            return ($age > $check_value);
          case '<=':
            return ($age <= $check_value);
          case '>=':
            return ($age >= $check_value);
        }
      }
    }
  }
  return FALSE;
}

/**
 * Callback for "node age" switch to generate the information line.
 *
 * @param array $arguments
 * @return string
 */
function hansel_switch_node_age_info($arguments) {
  $unit = empty($arguments['unit']) ? 86400 : $arguments['unit'];
  return t('with units of %interval', array('%interval' => format_interval($unit)));
}

/**
 * Callback for "path alias" switch to compare a given value.
 *
 * @param array $arguments
 * @param string $value
 * @return boolean
 */
function hansel_switch_path_alias_compare($arguments, $value) {
  if (!module_exists('path')) {
    return FALSE;
  }

  $alias = drupal_get_path_alias(hansel_path());
  return _hansel_switch_string_compare($arguments, $value, $alias);
}

/**
 * Helper function to compare a path with a given value.
 *
 * @param array $arguments
 * @param string $value
 * @return boolean
 */

function _hansel_switch_string_compare($arguments, $value, $path) {
  $mode = empty($arguments['mode']) ? 0 : $arguments['mode'];
  switch ($mode) {
    case 0:
      $pattern = '^' . preg_quote($value);
      break;
    case 1:
      $pattern = preg_quote($value) . '$';
      break;
    case 2:
      $pattern = preg_quote($value);
      break;
    case 3:
      $pattern = $value;
      break;
  }
  $pattern = str_replace('/', '\\/', $pattern);
  $pattern = "/$pattern/s";
  if (empty($arguments['cs'])) {
    $pattern .= 'i';
  }

  return (bool) @preg_match($pattern, $path);
}

/**
 * Callback for "path alias" switch to generate the information line.
 *
 * @param array $arguments
 * @return string
 */
function hansel_switch_path_alias_info($arguments) {
  $mode = empty($arguments['mode']) ? 0 : $arguments['mode'];
  switch ($mode) {
    case 0:
      return '(' . t('starts with') . ')';
    case 1:
      return '(' . t('ends with') . ')';
    case 2:
      return '(' . t('contains') . ')';
    case 3:
      return '(' . t('matches regular expression') . ')';
  }
}

/**
 * Callback for "request uri" switch to generate the information line.
 *
 * @param array $arguments
 * @return string
 */
function hansel_switch_request_uri_info($arguments) {
  return hansel_switch_path_alias_info($arguments);
}


/**
 * Callback for "Request URI" switch.
 *
 * @param array $arguments
 * @param string $value
 * @return boolean
 */
function hansel_switch_request_uri_compare($arguments, $value) {
  $uri = trim(request_uri(), '/');
  return _hansel_switch_string_compare($arguments, $value, $uri);
}
