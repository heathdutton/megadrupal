<?php

/**
 * Implementation of hook_drush_command().
 */
function past_db_drush_command() {
  $items['past-show'] = array(
    'description' => 'Show past events.',
    'drupal dependencies' => array('past_db'),
    'arguments' => array(
      'event_id' => 'Optional id of a past event to show in detail. If not provided, a listing of most recent 10 events will be displayed.',
    ),
    'options' => array(
      // Filter
      'count' => 'The number of messages to show. Defaults to 10.',
      'severity' => 'Restrict to messages of a given severity level.',
      'module' => 'Restrict to messages of a given module.',
      'machine_name' => 'Restrict to messages of a given machine name.',
      'tail' => 'Continuously show new past events until interrupted.',
      'sleep-delay' => 'To be used in conjunction with --tail. This is the number of seconds to wait between each poll to the database. Delay is 1 second by default.',
      // Presentation
      'full' => 'Full table information about each message.',
      'detail' => 'Return extended information about each message.',
      'detail-filter' => 'Return extended information about messages containing filter.',
      'nested' => 'Display nested argument data using print_r().',
    ),
    'examples' => array(
      'drush past-show' => 'Show a tablelisting of most recent 10 messages.',
      'drush past-show 64' => 'Show in detail message with id 64.',
      'drush past-show "cron run succesful"' => 'Show a listing of most recent 10 messages containing the string "cron run succesful".',
      'drush past-show --count=46' => 'Show a listing of most recent 46 messages.',
      'drush past-show --severity=notice' => 'Show a listing of most recent 10 messages with a severity of notice.',
      'drush past-show --type=php' => 'Show a listing of most recent 10 messages of type php.',
      'drush past-show --tail --full' => 'Show a table 5listing of most recent 10 messages with extended information about each one and continue showing messages as they are registered in the past log.',
      'drush past-show --tail --detail' => 'Show most recent 10 messages with detail information about each one and continue showing messages as they are registered in the past log.',
      'drush past-show --tail --detail-filter=debug' => 'Show a listing of most recent 10 messages with extended information about each containing "debug" items and continue showing messages as they are registered in the past log.',
      'drush past-show --tail --sleep-delay=2' => 'Do a tail of the past log with a delay of two seconds between each poll to the database.',
    ),
    'aliases' => array('ps'),
  );
  /*$items['watchdog-delete'] = array(
    'description' => 'Delete watchdog messages.',
    'drupal dependencies' => array('dblog'),
    'options' => array(
      'severity' => 'Delete messages of a given severity level.',
      'type' => 'Delete messages of a given type.',
    ),
    'examples' => array(
      'drush watchdog-delete all' => 'Delete all messages.',
      'drush watchdog-delete 64' => 'Delete messages with id 64.',
      'drush watchdog-delete "cron run succesful"' => 'Delete messages containing the string "cron run succesful".',
      'drush watchdog-delete --severity=notice' => 'Delete all messages with a severity of notice.',
      'drush watchdog-delete --type=cron' => 'Delete all messages of type cron.',
    ),
    'aliases' => array('wd-del', 'wd-delete'),
  );*/
  return $items;
}

/**
 * Command callback.
 */
function drush_past_db_past_show($arg = NULL) {
  if (is_numeric($arg)) {
    past_db_drush_show_one($arg);
  }
  else {
    past_db_drush_show_many($arg);
  }
}

/**
 * Print a watchdog message.
 *
 * Display message in ONE form.
 * Key: Value
 * Keys are ucfirst machine keys with removed underscores.
 *
 * @param $event_id
 *    The id of the message to show.
 */
function past_db_drush_show_one($event_id) {
  $event = entity_load_single('past_event', $event_id);
  if (!$event) {
    return drush_set_error(dt('Past event #!event_id not found.', array('!event_id' => $event_id)));
  }

  // Preparing the event data.
  $nested = drush_get_option('nested', FALSE);
  $result = past_db_event_format_result($event, TRUE, $nested);

  // Preparing the ONE representation.
  $upper = new stdClass();
  foreach ($result as $key => $value) {
    $uc = ucfirst(str_replace('_', ' ', $key));
    if (is_array($value)) {
      $value = implode("\n", $value);
    }
    $upper->$uc = $value;
  }

  // Yay, display!
  drush_print_table(drush_key_value_to_array_table($upper));
  print "\n";
}

/**
 * Print a table of watchdog messages.
 *
 * @param $filter
 *   String to filter the message's text by.
 */
function past_db_drush_show_many($filter = NULL) {
  $count = drush_get_option('count', 10);
  $module = drush_get_option('module');
  $machine_name = drush_get_option('machine_name');
  $severity = drush_get_option('severity');
  $tail = drush_get_option('tail', FALSE);
  $full = drush_get_option('full', FALSE);
  $detail = drush_get_option('detail', FALSE);
  $display = $full?'full':'normal';
  if ($detail) {
    $display = 'detail';
  }
  $detail_filter = drush_get_option('detail-filter', NULL);

  // Prepare table data.
  $none = TRUE;
  $table = array();
  $tbl = new Console_Table(CONSOLE_TABLE_ALIGN_LEFT, '');

  drush_log(dt('Most recent !count past event messages:', array('!count' => $count)));

  // Prepare header.
  $header = NULL;
  if ($display!='normal') {
    $header = array(dt('Id'), dt('Date'), dt('Severity'), dt('Module'), dt('Machine name'), dt('Message'));
    if ($display=='full') {
      $header[] = dt('Arguments');
    }
    //array_unshift($table, $header);
    $tbl->setHeaders($header);
  }

  // We will reuse the table object to display each line generated while in tail mode.
  // To make it possible some hacking is done on the object: remove the header and reset the rows on each iteration.
  $last_event_id = 0;
  $last_format = NULL;
  // sleep-delay
  $sleep_delay = drush_get_option('sleep-delay', 1);

  while (TRUE) {
    $query = past_db_drush_query_events($filter, $severity, $module, $machine_name);
    if ($last_event_id) {
      $query->propertyCondition('event_id', $last_event_id, '>');
    }
    if (!$last_event_id) {
      // For the first tail query, limit the result.
      $query->range(0, $count);
    }
    $result = $query->execute();
    if ($result) {
      $none = FALSE;
      // Initi this cycle.
      $table = array();
      // Reset table rows.
      $tbl->_data = array();
      $events = entity_load('past_event', array_keys($result['past_event']));
      // Process oldest first!
      $events = array_reverse($events);

      foreach ($events as $event) {
        $detail_line = $display=='detail';
        $full_line = $display=='full';
        // Check arguments if this line is a special case.
        $arguments = $event->getArguments();
        if ($detail_filter && ($arguments[$detail_filter])) {
          $detail_line = TRUE;
        }
        // Consider full-filter
        if ($full_filter && ($arguments[$full_filter])) {
          $full_line = TRUE;
        }
        // Prepare result, with arguments if requested
        $arguments_line = $full_line || $detail_line;
        $result = past_db_event_format_result($event, $arguments_line, $nested);
    
        if ($detail_line) {
          // preflush table.
          past_drush_table($tbl, $table, $header, $last_format);
          // display single detail record.
          past_db_drush_show_one($event->event_id);
          $last_format = 'detail';
        }
        else {
          $row = array(
            $result->event_id, $result->date, $result->severity, $result->module, $result->machine_name, $result->message
          );
          if ($full_line) {
            $argstr = '';
            foreach ($arguments as $key => $value) {
              $data = $value->getData();
              if (is_scalar($data)) {
                $argstr .= ucfirst($key) . ': ' . $data . "\n";
              }
              else {
                $argstr .= ucfirst($key) . ':' . "\n";
                $argstr .= $result->{'argument_' . $key} . "\n";
              }
            }
            $row[] = $argstr;
          }
          // enqueue.
          $table[] = $row;
        }
      }
      // display table.
      past_drush_table($tbl, $table, $header, $last_format);
      // terminate this loop.
      $last = array_pop($events);
      $last_event_id = $last->event_id;
    }
    if (!$tail) {
      print "\n";
      break;
    }
    // Only load 10 new events each time to avoid an overload.
    $count = 10;
    sleep($sleep_delay);
  }
  if ($none) {
    return drush_log(dt('No past events available.'), 'ok');
  }
}

function past_drush_table(&$tbl, &$table, $header, &$last_format) {
  if (!count($table)) {
    return;
  }
  // @todo for some reason the first header output is misaligned. fix!
  // @todo this is not possible with setHeaders(); fix drush!
  $tbl->_headers = NULL;
  if ($last_format == 'detail') {
    $tbl->setHeaders($header);
  }
  $tbl->addData($table);
  print $tbl->getTable();

  $last_format = 'table';
  $table = array();
}

/**
 * Format a past event entity.
 *
 * @param PastEventInterface $event
 *   Past event entity.
 * @param $show_arguments
 *   Boolean. Return extended details.
 * @return
 *   Array. The result object with some attributes themed.
 *
 * @todo Unsetting $event variables is unclean. Build a new array.
 * @todo Return currently is object, NOT array.
 */
function past_db_event_format_result($event, $show_arguments = FALSE, $nested = FALSE) {
  $result = clone $event;
  // Severity.
  $severities = past_event_severities();
  if (isset($severities[$event->severity])) {
    $result->severity = $severities[$event->severity];
  }

  // Date.
  $result->date = format_date($event->timestamp, 'custom', 'd/M H:i');
  unset($result->timestamp);

  // Message.
  $message_length = 188;

  if ($show_arguments) {
    // Print all the data available.
    foreach ($event->getArguments() as $key => $argument) {
      $result->{'argument_' . $key} = past_db_argument($argument, $nested);
    }
    $message_length = PHP_INT_MAX;
  }

  $result->message = truncate_utf8(strip_tags(decode_entities($result->message)), $message_length, FALSE, FALSE);
  unset($result->rdf_mapping);

  return $result;
}

/**
 * Prepare argument for presentation.
 */
function past_db_argument($argument, $nested) {
  $data = array();
  $argument_data = $argument->getData();
  if (is_array($argument_data) || is_object($argument_data)) {
    foreach ($argument->getData() as $name => $value) {
      if (!is_scalar($value) && $nested) {
        $value = print_r($value, TRUE);
      }
      $data[] = $name . ' => ' . $value;
    }
  }
  else {
    $data = $argument_data;
  }
  return $data; 
}

/**
 * Command callback.
 *
 * @param $arg
 *   The id of the message to delete or 'all'.
 *
function drush_core_watchdog_delete($arg = NULL) {
  if ($arg == 'all') {
    drush_print(dt('All watchdog messages will be deleted.'));
    if (!drush_confirm(dt('Do you really want to continue?'))) {
      return drush_user_abort();
    }
    drush_db_delete('watchdog');
    drush_log(dt('All watchdog messages have been deleted.'), 'ok');
  }
  else if (is_numeric($arg)) {
    drush_print(dt('Watchdog message #!wid will be deleted.', array('!wid' => $arg)));
    if(!drush_confirm(dt('Do you really want to continue?'))) {
      return drush_user_abort();
    }
    $affected_rows = drush_db_delete('watchdog', 'wid=:wid', array(':wid' => $arg));
    if ($affected_rows == 1) {
      drush_log(dt('Watchdog message #!wid has been deleted.', array('!wid' => $arg)), 'ok');
    }
    else {
      return drush_set_error(dt('Watchdog message #!wid does not exist.', array('!wid' => $arg)));
    }
  }
  else {
    $type = drush_get_option('type');
    $severity = drush_get_option('severity');
    if ((is_null($arg))&&(is_null($type))&&(is_null($severity))) {
      return drush_set_error(dt('No options provided.'));
    }
    $where = core_watchdog_query($type, $severity, $arg, 'OR');
    if ($where === FALSE) {
      // Drush set error was already called by core_watchdog_query
      return FALSE;
    }
    drush_print(dt('All messages with !where will be deleted.', array('!where' => preg_replace("/message LIKE %$arg%/", "message body containing '$arg'" , strtr($where['where'], $where['args'])))));
    if(!drush_confirm(dt('Do you really want to continue?'))) {
      return drush_user_abort();
    }
    $affected_rows = drush_db_delete('watchdog', $where['where'], $where['args']);
    drush_log(dt('!affected_rows watchdog messages have been deleted.', array('!affected_rows' => $affected_rows)), 'ok');
  }
}
*/
/**
 * Build a EQF query based on given parameters.
 *
 * @param $severity
 *   Int or String for a valid watchdog severity message.
 * @param $filter
 *   String. Value to filter watchdog messages by.
 * @return
 *   False or array with structure ('where' => string, 'args' => array())
 **/
function past_db_drush_query_events($filter = NULL, $severity = NULL, $module = NULL, $machine_name = NULL) {
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'past_event');
  if ($severity) {
    $query->propertyCondition('severity', $severity, '<=');
  }
  if ($filter) {
    $query->propertyCondition('message', $filter, 'CONTAINS');
  }
  if ($module) {
    $query->propertyCondition('module', $module);
  }
  if ($machine_name) {
    $query->propertyCondition('machine_name', $machine_name);
  }

  $query->propertyOrderBy('event_id', 'DESC');
  return $query;
}
