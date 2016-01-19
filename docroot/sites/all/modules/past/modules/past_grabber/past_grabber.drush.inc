<?php
/**
 * @file
 * Past Grabber drush file.
 */

/**
 * Implements hook_drush_command().
 */
function past_grabber_drush_command() {
  $items = array();

  $items['past-grab'] = array(
    'callback' => 'past_grabber_drush_grab',
    'description' => 'Will run Past Grabber on provided error log file and extract PHP errors of given type.',
    'arguments' => array(
      'path' => 'Path to the error log file',
      'type' => 'PHP Error type. Use names as they appear in the log file.'
    ),
    'options' => array(
      'position' => 'Pointer integer from which to start parsing the file for errors.',
      'count' => 'Count of records to process.',
      'severity' => 'Severity of the Past event. Use PAST_SEVERITY_* constant values.',
      'machine_name' => 'Machine name of the Past event.',
    ),
    'examples' => array(
      'drush past-grab /var/log/apache2/error.log "PHP Fatal error" --count=100' =>
      'Will parse all records for PHP Fatal error type and will process first 100.',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  );

  return $items;
}

/**
 * Will run grabber for provided error log file.
 */
function past_grabber_drush_grab($path = NULL, $type = NULL) {
  if ($path === NULL) {
    $path = variable_get('past_grabber_file_path');
  }
  if ($type === NULL) {
    $type = PAST_GRABBER_DEFAULT_TYPE;
  }

  $position = drush_get_option('position', NULL);
  $count = drush_get_option('count', 100);
  $severity = drush_get_option('severity', PAST_SEVERITY_CRITICAL);
  $machine_name = drush_get_option('machine_name', NULL);

  if (!is_readable($path)) {
    return drush_set_error('error', dt('Provided path @path does not exist or is not readable.', array('@path' => $path)));
  }

  $result = past_grabber_parse_error_log($path, $position, $type, $count, $severity, $machine_name);

  drush_print(dt('Position: @position | Processed log records: @count', array('@position' => $result['position'], '@count' => $result['count'])));
}
