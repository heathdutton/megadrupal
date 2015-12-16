<?php

/**
 * @file
 * Statistics API hooks and functions.
 */


/**
 * Implements hook_better_statistics_log().
 */
function syslog_access_better_statistics_log($data) {
  // Open the logging facility.
  $default_facility = defined('LOG_LOCAL0') ? LOG_LOCAL0 : LOG_USER;
  openlog(variable_get('syslog_access_identity', 'drupal-access'), LOG_NDELAY, variable_get('syslog_access_facility', $default_facility));

  foreach ($data as $key => $value) {
    $message_data['!' . $key] = $value;
  }

  // Generate the log message.
  $message = strtr(variable_get('syslog_access_format', '!timestamp|!hostname|!path|!title|!url|!uid|!sid|!timer'), $message_data);

  // Log the access and close the facility.
  syslog(LOG_INFO, $message);
  closelog();
}
