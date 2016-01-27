<?php

function paas_metric_calltrace_schema() {
  $schema = array();
  
  $schema['paas_calltrace'] = array(
    'fields' => array(
      'cid' => array(
        'type' => 'serial',
        'unsigned' => TRUE,
        'not null' => TRUE,
      ),
      'trace' => array(
        'type' => 'text',
        'not null' => TRUE,
      ),
      'time' => array(
        'type' => 'float',
        'unsigned' => TRUE,
        'not null' => TRUE,
      ),
    ),
    'primary key' => array('cid'),
  );
  
  return $schema;
}

function paas_metric_calltrace_start() {
  global $_paas_calltrace, $_paas_track;
  $_paas_calltrace = array();
  $_paas_track = TRUE;
  register_tick_function('paas_metric_calltrace_log');
  declare(ticks=100);
}

function paas_metric_calltrace_stop() {
  unregister_tick_function('paas_metric_calltrace_flush');
  paas_metric_calltrace_flush();
}

function paas_metric_calltrace_call($value) {
  switch ($value) {
    case 'test':
      $filter = '';
      $function = $_GET['function'];
      $argument = $_GET['argument'];
      paas_metric_calltrace_start();
      switch ($function) {
        case 'path':
          $path = trim($argument, '/');
          if (empty($path)) {
            $path = variable_get('site_frontpage', 'node');
          }
          if ($router_item = menu_get_item($path)) {
            if (!empty($router_item['file'])) {
              require_once($router_item['file']);
            }
            call_user_func_array($router_item['page_callback'], $router_item['page_arguments']);
            $filter = 'call_user_func_array';
          }
          else {
            $filter = 'error';
          }
          break;
        case 'block':
          break;
        case 'view':
          break;
      }
      paas_metric_calltrace_stop();
      print $filter;
      break;
    case 'trace':
      paas_dump_csv('SELECT trace, time FROM {paas_calltrace} ORDER BY time ASC');
      break;
  }
}

function paas_metric_calltrace_log() {
  global $_paas_calltrace, $_paas_track;
  static $c = 0;
  
  if (!$_paas_track) {
    return;
  }
  $_paas_track = FALSE;
  
  $trace = array();
  $bt = debug_backtrace();
  foreach ($bt as $row) {
    $trace[] = (empty($row['class']) ? '' : $row['class'] . '::') . $row['function'];
  }
  array_pop($trace); // menu_execute_active_handler
  array_pop($trace); // call_user_func_array
  array_pop($trace); // paas_call_action
  array_pop($trace); // paas_metric_calltrace_call
  array_shift($trace); // Shift off this function.
  if ($trace) {
    $_paas_calltrace[] = array(implode(',', $trace), timer_read('page'));
  }
  if (++$c > 250) {
    paas_metric_calltrace_flush();
  }
  
  $_paas_track = TRUE;
}

function paas_metric_calltrace_flush() {
  global $_paas_calltrace, $_paas_track;
  static $first = TRUE;
  $_paas_track = FALSE;
  if ($first) {
    $first = FALSE;
    db_truncate('paas_calltrace')->execute();
  }
  
  $transaction = db_transaction();
  $sql = 'INSERT INTO {paas_calltrace} (trace, time) VALUES (:trace, :time)';
  foreach ($_paas_calltrace as $trace) {
    if (!strstr($trace[0], 'paas_metric_')) {
      db_query($sql, array('trace' => $trace[0], 'time' => $trace[1]));
    }
  }
  $_paas_calltrace = array();
  $_paas_track = TRUE;
}
