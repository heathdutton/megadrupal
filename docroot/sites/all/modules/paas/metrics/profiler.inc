<?php

function paas_metric_profiler_boot() {
  global $_paas_queries, $_paas_query_count, $_paas_track, $_paas_vars, $_paas_cache;
  if (preg_match('/^paas\\-testing/', $_GET['q'])) {
    return;
  }
  $_paas_queries = array();
  $_paas_cache = array();
  $_paas_query_count = 0;
  $_paas_track = TRUE;
  $_paas_vars = array(
    'boot' => timer_read('page'),
  );
}

function paas_metric_profiler_init() {
  global $_paas_vars, $user;
  if (preg_match('/^paas\\-testing/', $_GET['q'])) {
    return;
  }
  $_paas_vars['init'] = timer_read('page');
  $_paas_vars['uid'] = $user->uid;
  if (variable_get('dev_query', 0)) {
    variable_set('dev_query', 0);
  }
  if (function_exists('memory_get_peak_usage')) {
    $_paas_vars['memory_boot'] = memory_get_peak_usage(TRUE);
  }
  else {
    $_paas_vars['memory_boot'] = 0;
  }
  $data = paas_metric_profiler_check_static();
  $_paas_vars['boot_static'] = $data['static'];
}

function paas_metric_profiler_preprocess_page() {
  global $_paas_vars;
  $_paas_vars['preprocess_page'] = timer_read('page');
  if (function_exists('memory_get_peak_usage')) {
    $_paas_vars['memory_preprocess_page'] = memory_get_peak_usage(TRUE);
  }
  else {
    $_paas_vars['memory_preprocess_page'] = 0;
  }
}

function paas_metric_profiler_preprocess_node() {
  global $_paas_vars;
  $_paas_vars['preprocess_node'] = timer_read('page');
}

function paas_metric_profiler_preprocess_block() {
  global $_paas_vars;
  $_paas_vars['preprocess_block'] = timer_read('page');
}

function paas_metric_profiler_query_start() {
  timer_start('paas_profiler_query');
}

function paas_metric_profiler_query_stop($sql) {
  global $_paas_queries, $_paas_query_count, $_paas_track;
  if (preg_match('/^paas\\-testing/', $_GET['q'])) {
    return;
  }
  
  if (!$_paas_track) {
    return;
  }
  
  $timer = timer_read('paas_profiler_query');
  
  if ($timer <= 0 || $timer > 3600000) {
    // Error in measure, ignore query.
    return;
  }
  
  $trace = array();
  $bt = debug_backtrace();
  foreach ($bt as $row) {
    $trace[] = (empty($row['class']) ? '' : $row['class'] . '::') . $row['function'];
  }
  array_shift($trace);
  array_shift($trace);
  array_shift($trace);
  
  ++$_paas_query_count;

  $_paas_queries[] = array($sql, implode(',', $trace), timer_read('page'), $timer);
}

function paas_metric_profiler_cache($action) {
  global $_paas_cache;
  $trace = array();
  $bt = debug_backtrace();
  foreach ($bt as $row) {
    $trace[] = (empty($row['class']) ? '' : $row['class'] . '::') . $row['function'];
  }
  array_shift($trace);
  array_shift($trace);
  $_paas_cache[] = array($action, implode(',', $trace), timer_read('page'));
}

function paas_metric_profiler_flush() {
  global $_paas_queries, $_paas_cache, $_paas_track;
  static $first = TRUE;
  $_paas_track = FALSE;
  if ($first) {
    $first = FALSE;
    db_query('TRUNCATE TABLE {paas_profiler_query}');
    db_query('TRUNCATE TABLE {paas_profiler_cache}');
  }
  db_query('LOCK TABLES {paas_profiler_query} WRITE');
  $sql = 'INSERT INTO {paas_profiler_query} (query, trace, time, timer) VALUES (:query, :trace, :time, :timer)';

  foreach ($_paas_queries as $query) {
    db_query($sql, array(
      ':query' => $query[0],
      ':trace' => $query[1],
      ':time' => $query[2],
      ':timer' => $query[3],
    ));
  }
  db_query('UNLOCK TABLES');
  db_query('LOCK TABLES {paas_profiler_cache} WRITE');
  $sql = 'INSERT INTO {paas_profiler_cache} (action, trace, time) VALUES (:action, :trace, :time)';
  foreach ($_paas_cache as $cache) {
    db_query($sql, array(
      ':action' => $cache[0],
      ':trace' => $cache[1],
      ':time' => $cache[2],
    ));
  }
  db_query('UNLOCK TABLES');
  $_paas_track = TRUE;
  $_paas_queries = array();
  $_paas_cache = array();
}

function paas_metric_profiler_exit() {
  global $_paas_vars, $_paas_track;
  
  $_paas_vars['exit'] = timer_read('page');
  
  // Add memory usage.
  if (function_exists('memory_get_peak_usage')) {
    $_paas_vars['memory'] = memory_get_peak_usage(TRUE);
  }
  else {
    $_paas_vars['memory'] = 0;
  }
  
  // Add information about static vars.
  $_paas_vars += paas_metric_profiler_check_static();
  
  // Flush queries to database.
  paas_metric_profiler_flush();
  
  // Stop tracking queries.
  $_paas_track = FALSE;
  
  // Write variables to database.
  db_query('TRUNCATE TABLE {paas_profiler_var}');
  $sql = 'INSERT INTO {paas_profiler_var} (name, value) VALUES (:name, :value)';
  foreach ($_paas_vars as $name => $value) {
    db_query($sql, array(
      ':name' => $name,
      ':value' => $value,
    ));
  }
}

/**
 * Get memory usage for static variables.
 */
function paas_metric_profiler_check_static() {
  global $conf;
  
  $data = array();
  $total = 0;
  $functions = get_defined_functions();
  
  $functions['user'][] = 'conf';
  foreach ($functions['user'] as $function) {
    $var = $function == 'conf' ? $conf : drupal_static($function);
    if ($var) {
      $var = serialize($var);
      $startMemory = memory_get_usage();
      $var2 = unserialize($var);
      $usage = memory_get_usage() - $startMemory - PHP_INT_SIZE;
      $total += $usage;
      $usage = round($usage / 1024);
      if ($usage) {
        $data["static_$function"] = $usage;
      }
      unset($var);
      unset($var2);
    }
  }
  $data['static'] = round($total / 1024);
  return $data;
}

function paas_metric_profiler_call($value) {
  switch ($value) {
    case 'queries':
      paas_dump_csv('SELECT query, trace, time, timer FROM {paas_profiler_query}');
      break;
    case 'cache':
      paas_dump_csv('SELECT action, trace, time FROM {paas_profiler_cache}');
      break;
    case 'vars':
      paas_dump_csv('SELECT name, value FROM {paas_profiler_var}');
      break;
  }
}

function paas_metric_profiler_schema() {
  $schema = array();
  
  $schema['paas_profiler_var'] = array(
    'fields' => array(
      'name' => array(
        'type' => 'varchar',
        'length' => 64,
        'not null' => TRUE,
      ),
      'value' => array(
        'type' => 'float',
        'unsigned' => TRUE,
        'not null' => TRUE,
      ),
    ),
    'primary key' => array('name'),
  );
  
  $schema['paas_profiler_query'] = array(
    'fields' => array(
      'qid' => array(
        'type' => 'serial',
        'unsigned' => TRUE,
        'not null' => TRUE,
      ),
      'query' => array(
        'type' => 'text',
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
      'timer' => array(
        'type' => 'float',
        'unsigned' => TRUE,
        'not null' => TRUE,
      ),
    ),
    'primary key' => array('qid'),
  );
  
  $schema['paas_profiler_cache'] = array(
    'fields' => array(
      'cid' => array(
        'type' => 'serial',
        'unsigned' => TRUE,
        'not null' => TRUE,
      ),
      'action' => array(
        'type' => 'text',
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
