<?php

/**
 * Convert given time to Jalali calendar.
 * Two different formats need to be handled.
 * When using PHP's internal date conversion, the formats follow the pattern
 * explained here: http://userguide.icu-project.org/formatparse/datetime
 * Otherwise the date format follow this pattern: http://ir2.php.net/manual/en/function.date.php
 * @param $time
 *   time in the Gregorian format
 * @return
 *   jalalidate
 */
function jalalidate_convert($time, $format=NULL) {
  $date_format = variable_get('jalalidate_nodes_format');

  if (class_exists('IntlDateFormatter')) {
    if ($format == NULL) {
      switch ($date_format) {
        case 'short':
          $format = 'Y/M/d';
          break;
        case 'medium':
          $format = 'Y/M/d - H:m';
          break;
        case 'long':
          $format = 'EEEE Y/M/d - H:m';
          break;
        case 'custom':
          $format = variable_get('jalalidate_nodes_custom');
          break;
      }
    }    
    $fmt = new IntlDateFormatter("fa_IR@calendar=persian", IntlDateFormatter::FULL, IntlDateFormatter::SHORT, jalalidate_default_timezone(), IntlDateFormatter::TRADITIONAL);
    $fmt->setPattern($format);
    $date = $fmt->format(intval($time));
  }
  else {
    if ($format == NULL) {
      switch ($date_format) {
        case 'short':
          $format = 'Y/n/j';
          break;
        case 'medium':
          $format = 'Y/n/j - G:i';
          break;
        case 'long':
          $format = 'l Y/n/j - G:i';
          break;
        case 'custom':
          $format = variable_get('jalalidate_nodes_custom');
          break;
      }
    }
    module_load_include("php", "persiantools", "includes/Date");
    $date = \Shamsi\date($format, $time);
  }

  return $date;
}

function jalalidate_convert_format($time, $format) {

  module_load_include("php", "persiantools", "includes/Date");
  $date = \Shamsi\date($format, $time);

  return $date;
}

/**
 * This function checks dependencies for using PHP's internal jalalidate. If 
 * these dependencies are not met, then the 'Date.php' file included with this
 * module is used for converting dates to jalali calendar.
 * This function is called when 'Run test' is clicked in the module's settings page.
 */
function jalalidate_verify_requirements() {
  $php_allowed_version  = 50300;
  $intl_allowed_version = 10000;
  $satisfied = 1;

  //Get current PHP version
  $php_id = phpversion();
  $php_version  = jalalidate_str2num($php_id);

  //Get current PECL intl version
  $intl_id = phpversion('intl');
  $intl_version = jalalidate_str2num($intl_id);

  if ($php_version <= $php_allowed_version) {
    $satisfied = 0;
    drupal_set_message(t('Current PHP version is less than 5.3.0'), 'warnings');
  }
  if ($intl_version <= $intl_allowed_version) {
    $satisfied = 0;
    drupal_set_message(t('Current PECL intl version is less than 1.0.0'), 'warnings');
  }

  if ($satisfied) {
    drupal_set_message(t('All dependencies are satisfied. Jalali Date will use the internal PHP date (Intl).'), 'status');
  }
  else {
    drupal_set_message(t('The internal PHP date will not be used.'), 'warnings');
  }
}

/**
 * Convert string ID version of given input to int
 * @param $input
 *   ID in the form of string
 * @return
 *   ID in the form of int
 */
function jalalidate_str2num($input) {
  if ($input == '') return 0;
  $input_parts = explode('.', $input);
  $version = $input_parts[0] * 10000 + $input_parts[1] * 100 + $input_parts[2];
  return $version;
}

function jalalidate_default_timezone($check_user = TRUE) {
  global $user;
  if ($check_user && variable_get('configurable_timezones', 1) && !empty($user->timezone)) {
    return $user->timezone;
  }
  else {
    $default = variable_get('date_default_timezone', '');
    return empty($default) ? 'Asia/Tehran' : $default;
  }
}

