<?php

/**
 * @file
 * Drush interface of Account Sentinel.
 */

/**
 * Implements hook_drush_command().
 */
function account_sentinel_drush_command() {
  $items['account-sentinel-show'] = array(
    'callback' => 'account_sentinel_drush_show',
    'description' => 'Shows the latest events Account Sentinel recorded.',
    'aliases' => array('as-show'),
    'arguments' => array(
      'eid' => 'Optional id of Account Sentinel event to show in detail. If not set, the most recent events will be shown.',
    ),
    'options' => array(
      'count' => 'Number of events to show. Defaults to 15.',
      'uid' => "Filter by the subject's UID.",
      'by_uid' => "Filter by the invoker's UID.",
      'origin' => "Filter by the event's origin.",
      'type' => "Filter by the event's type.",
      'ip' => "Filter by the invoker's IP address.",
      'before' => 'Show events occurred before given date. Supports a wide range of date formats (see http://php.net/manual/en/datetime.formats.php).',
      'after' => 'Show events occurred after given date. Supports a wide range of date formats (see http://php.net/manual/en/datetime.formats.php).',
    ),
    'outputformat' => array(
      'default' => 'table',
      'pipe-format' => 'var_export',
      'field-labels' => array(
        'eid' => 'ID',
        'uid' => 'UID',
        'origin' => 'Origin',
        'type' => 'Type',
        'data' => 'Raw data',
        'message' => 'Message',
        'by_uid' => 'By UID',
        'ip' => 'IP address',
        'date' => 'Date',
      ),
      'fields-default' => array('eid', 'date', 'uid', 'message', 'by_uid', 'ip'),
      'output-data-type' => 'format-table',
    ),
  );

  $items['account-sentinel-audit'] = array(
    'callback' => 'account_sentinel_drush_audit',
    'description' => "Runs Account Sentinel's database audit.",
    'aliases' => array('as-audit'),
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function account_sentinel_drush_help($section) {
  switch ($section) {
    case 'account-sentinel-show':
      return dt('');

    case 'account-sentinel-audit':
      return dt('');
  }
}


/**
 * Implements the account-sentinel-show Drush command.
 *
 * @param int|null $eid
 *   EID of the event, or NULL if multiple events should be shown.
 *
 * @return array|bool|null
 *   Output for Drush.
 */
function account_sentinel_drush_show($eid = NULL) {
  drush_include_engine('drupal', 'environment');

  if (is_numeric($eid)) {
    return account_sentinel_drush_show_one($eid);
  }
  else {
    return account_sentinel_drush_show_many();
  }
}

/**
 * Shows a single event's details.
 *
 * @param int $eid
 *   EID of the event.
 *
 * @return array|bool
 *   Output for Drush.
 */
function account_sentinel_drush_show_one($eid) {
  // Show a nicer output than a single-row table.
  drush_set_default_outputformat('key-value-list');

  // Query the event.
  $rsc = drush_db_select('account_sentinel_logs', '*', 'eid = :eid', array(':eid' => $eid), 0, 1);
  $result = drush_db_fetch_object($rsc);
  if (!$result) {
    return drush_set_error(dt('Account Sentinel event #!eid not found.', array('!eid' => $eid)));
  }
  $event = account_sentinel_drush_format_event($result);
  return array($event->eid => (array) $event);
}

/**
 * Shows multiple events' details.
 *
 * @return array
 *   Output for drush.
 */
function account_sentinel_drush_show_many() {
  $where = account_sentinel_drush_show_many_where();
  $count = drush_get_option('count', 15);
  $rsc = drush_db_select('account_sentinel_logs', '*', $where['expr'], $where['args'], 0, $count, 'eid', 'DESC');
  $table = array();
  while ($result = drush_db_fetch_object($rsc)) {
    $row = account_sentinel_drush_format_event($result);
    $table[$row->eid] = (array) $row;
  }
  if (empty($table)) {
    drush_log(dt('No log events available.'), 'ok');
    return array();
  }
  return $table;
}

/**
 * Constructs the WHERE expression for the show command.
 *
 * @see account_sentinel_drush_show_many()
 *
 * @return array
 *   An associative array containing:
 *   - expr: The WHERE expression.
 *   - args: Values of the named placeholders.
 */
function account_sentinel_drush_show_many_where() {
  // Collect filter values from options.
  $filters = array(
    array('uid', drush_get_option('uid')),
    array('by_uid', drush_get_option('by_uid')),
    array('origin', drush_get_option('origin')),
    array('type', drush_get_option('type')),
    array('ip', drush_get_option('ip')),
    array('timestamp', drush_get_option('before'), '<='),
    array('timestamp', drush_get_option('after'), '>='),
  );

  // Convert some filter values.
  foreach ($filters as $key => $filter) {
    // Dates should be converted to UNIX timestamps.
    if ($filter[0] == 'timestamp' && $filter[1] !== NULL) {
      if (($time = strtotime($filter[1])) !== FALSE) {
        $filters[$key][1] = $time;
      }
      else {
        $filters[$key][1] = NULL;
      }
    }
  }

  // Build the expression.
  $where = array(
    'expr' => array(),
    'args' => array(),
  );
  $placeholder_cnt = 0;
  foreach ($filters as $filter) {
    $col = $filter[0];
    $val = $filter[1];
    $op = isset($filter[2]) ? $filter[2] : '=';
    if ($val !== NULL) {
      $placeholder_cnt++;
      $expr = "($col $op :ph_$placeholder_cnt)";
      $where['expr'][] = $expr;
      $where['args'][":ph_$placeholder_cnt"] = $val;
    }
  }

  $where['expr'] = implode(' AND ', $where['expr']);

  // Return the WHERE expression.
  return $where;
}

/**
 * Formats an event row for output.
 *
 * @param object $event
 *   A row object of account_sentinel_logs.
 *
 * @return object
 *   The event with formatted data.
 */
function account_sentinel_drush_format_event($event) {
  // Collect and construct additional information.
  $type = $event->type;
  $origin = $event->origin;
  $data = unserialize($event->data);
  $message = account_sentinel_get_event_message($type, $data);
  $message = account_sentinel_drush_html_to_text($message);

  // Set formatted information.
  $event->date = format_date($event->timestamp, 'short');
  $event->data = print_r($data, TRUE);
  $event->message = $message;
  $event->type = account_sentinel_event_type_get_string($type);
  $event->origin = account_sentinel_event_origin_get_string($origin);

  // Return formatted event.
  return $event;
}

/**
 * Converts an HTML message to plain text.
 *
 * Uses drush_html_to_text(), but also removes some unwanted whitespace that the
 * said command leaves in its output.
 *
 * @param string $message
 *   The input HTML message.
 *
 * @see drush_html_to_text()
 *
 * @return string
 *   The plaintext message.
 */
function account_sentinel_drush_html_to_text($message) {
  return str_replace(' .', '.', drush_html_to_text($message));
}

/**
 * Implements the account-sentinel-audit Drush command.
 */
function account_sentinel_drush_audit() {
  watchdog('account_sentinel', 'Invoked database audit from Drush.');
  module_load_include('inc', 'account_sentinel', 'account_sentinel.audit');
  account_sentinel_audit();
  drush_log(dt('Account Sentinel audit run successful.'), 'success');
}
