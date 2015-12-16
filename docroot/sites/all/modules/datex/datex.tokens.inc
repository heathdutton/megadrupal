<?php

/**
 * @file
 * Token module integration.
 */

/**
 * Implements hook_token_info().
 */
function datex_token_info() {
  $info['tokens']['datex']['jalali_now'] = t('Jalali date (now).');

  $types['datex'] = array(
    'name' => t("Localized Date (Datex)"),
    'description' => t("Tokens related to localized times and dates using datex."),
  );

  $format = variable_get('date_format_short');
  $date['short'] = array(
    'name' => t("Short format"),
    'description' => t("A date in 'short' format. (%date)", array('%date' => datex_format_date('fa', REQUEST_TIME, $format))),
  );

  $format = variable_get('date_format_medium');
  $date['medium'] = array(
    'name' => t("Medium format"),
    'description' => t("A date in 'medium' format. (%date)", array('%date' => format_date(REQUEST_TIME, $format))),
  );

  $format = variable_get('date_format_long');
  $date['long'] = array(
    'name' => t("Long format"),
    'description' => t("A date in 'long' format. (%date)", array('%date' => format_date(REQUEST_TIME, $format))),
  );

  $date['custom'] = array(
    'name' => t("Custom format"),
    'description' => t("A date in a custom format and a select calendar. See !php-date for details and check datex for available calendars.", array('!php-date' => l(t('the PHP documentation'), 'http://php.net/manual/en/function.date.php'))),
    'dynamic' => TRUE,
  );

  $tokens = array(
    'types' => $types,
    'tokens' => array(
      'datex' => $date,
    )
  );

  return $tokens;
}

/**
 * Implements hook_tokens().
 */
function datex_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();

  if ($type == 'datex') {
    $date = empty($data['date']) ? REQUEST_TIME : $data['date'];

    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'short':
          $format = variable_get('date_format_short');
          $replacements[$original] = datex_format_date('fa', $date, $format);
          break;

        case 'medium':
          $format = variable_get('date_format_short');
          $replacements[$original] = datex_format_date('fa', $date, $format);
          break;

        case 'long':
          $format = variable_get('date_format_short');
          $replacements[$original] = datex_format_date('fa', $date, $format);
          break;
      }
    }
    if ($created_tokens = token_find_with_prefix($tokens, 'custom')) {
      foreach ($created_tokens as $name => $original) {
        list($calendar, $format) = @explode(':', $name);
        if (isset($format) && _datex_calendar_is_valid($calendar)) {
          $replacements[$original] = datex_format_date($calendar, $date, $format);
        }
        else {
          watchdog(WATCHDOG_WARNING, 'Invalid token arguments for datex. Format is not given or requested calerndar is not available. Token replacement ignored.');
        }
      }
    }
  }

  return $replacements;
}

