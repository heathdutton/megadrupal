<?php

/**
 * @file
 * Provides database driver specific report functions.
 */

define('MYSQL_KEY_USED_PERCENT', 75);
define('MYSQL_KEY_CURRENT_PERCENT', 75);
define('MYSQL_KEY_WRITE_HIT_PERCENT', 80); // <=
define('MYSQL_KEY_READ_HIT_PERCENT', 98); // <=
define('MYSQL_QUESTIONS_QC_HITS_PERCENT', 50);
define('MYSQL_QUESTIONS_COM_PERCENT', 5);
define('MYSQL_SELECT_SCAN_RATE', .5);
define('MYSQL_SELECT_SCAN_PERCENT', 15);
define('MYSQL_SELECT_FULL_JOIN_RATE', .5);
define('MYSQL_SELECT_FULL_JOIN_PERCENT', 10);
define('MYSQL_SELECT_SORT_MERGE_PASS_RATE', .25);
define('MYSQL_QUERY_CACHE_USED_PERCENT', 75);
define('MYSQL_QUERY_CACHE_FRAG_PERCENT', 12);
define('MYSQL_LOCKS_WAIT_RATE', 1);
define('MYSQL_LOCKS_WAIT_PERCENT', 10);
define('MYSQL_TABLE_CACHE_PERCENT', 95);
define('MYSQL_TABLE_RATE', 1);
define('MYSQL_CONNECTIONS_PERCENT', 100);
define('MYSQL_TMP_DISK_TABLE_RATE', 2);
define('MYSQL_THREADS_PERCENT', 65);
define('MYSQL_THREADS_RATE', 1);
define('MYSQL_SLOW_RATE', 1);
define('MYSQL_ABORT_CLIENT_RATE', .5);
define('MYSQL_ABORT_CONNECT_RATE', .5);
define('MYSQL_INNODB_BUFFER_PERCENT', 80);
define('MYSQL_INNODB_READ_HIT_PERCENT', 85);

/**
 * Get MySQL status message, method changes depending on version of MySQL
 * being used.
 */
function dba_mysql_report_status($data) {
  $value = new stdClass();
  $query = db_query('SELECT LOWER(variable_name) AS name, variable_value FROM information_schema.global_status');
  foreach ($query as $row) {
    $column = $row->name;
    $value->$column = $row->variable_value;
  }
  return $value;
}

/**
 * Return current MySQL version.
 */
function dba_mysql_report_version() {
  $result = db_query("SELECT variable_value FROM information_schema.global_variables WHERE variable_name = 'version'");
  foreach ($result as $row) {
    preg_match('/(\d{1,2})\.(\d{1,2})\.(\d{1,2})/', $row->variable_value, $matches) ;
    array_shift($matches);
    $version = new dba_report_version($matches[0], $matches[1], $matches[2]) ;
    return $version;
  }
}

/**
 * Return current MySQL variables.
 */
function dba_mysql_report_variables() {
  $variables = new stdClass();

  $query = db_query("SHOW VARIABLES");
  foreach ($query as $row) {
    $column = $row->Variable_name;
    $variables->$column = $row->Value;
  }
  return $variables;
}

/**
 * Return current MySQL uptime from status string.
 */
function dba_mysql_report_uptime($data) {
  $uptime = $data->status->uptime;
  $days = (int)($uptime / 86400);
  $uptime %= 86400 ;

  $hours = (int)($uptime / 3600);
  $uptime %= 3600 ;

  $minutes = (int)($uptime / 60);
  $seconds = $uptime % 60;

  $uptime = sprintf('%d days %02d:%02d:%02d', $days, $hours, $minutes, $seconds);
  return $uptime;
}

/**
 * Return the pretty name for this database type.
 */
function dba_mysql_report_name() {
  return t('MySQL');
}

/**
 * Return MySQL specific stuff.
 * Prepares MyISAM and InnoDB stats.
 */
function dba_mysql_report_driver($data) {
  $return = new stdClass();

  // MyISAM
  $return->myisam = new stdClass();
  $return->myisam->questions = $data->status->questions;

  $return->myisam->keyReadRatio = $data->status->key_read_requests ? (1.0 - ($data->status->key_reads / $data->status->key_read_requests)) * 100.0 : 0;
  $return->myisam->keyWriteRatio = $data->status->key_write_requests ?  (1.0 - ($data->status->key_writes / $data->status->key_write_requests)) * 100.0 : 0;

  $return->myisam->keyCacheBlockSize = isset($data->variables->key_cache_block_size) ?  $data->variables->key_cache_block_size : 1024;

  $return->myisam->keyBufferUsed = $data->status->key_blocks_used * $return->myisam->keyCacheBlockSize ;

  $return->myisam->keyBufferUsage = isset($data->status->key_blocks_unused) ?  $data->variables->key_buffer_size - ($data->status->key_blocks_unused * $return->myisam->keyCacheBlockSize) : -1;

  // Data Manipulation Statements: http://dev.mysql.com/doc/refman/5.0/en/data-manipulation.html
  $return->myisam->DMS = array('SELECT' => $data->status->com_select, 'INSERT' => $data->status->com_insert + $data->status->com_insert_select, 'REPLACE' => $data->status->com_replace + $data->status->com_replace_select, 'UPDATE' => $data->status->com_update + (isset($data->status->com_update_multi) ? $data->status->com_update_multi : 0 ), 'DELETE' => $data->status->com_delete + (isset($data->status->com_delete_multi) ? $data->status->com_delete_multi : 0 ));

  $return->myisam->DMS['DMS'] = array_sum($return->myisam->DMS) ;

  arsort($return->myisam->DMS);

  $return->myisam->slowQueryTime = $data->variables->long_query_time ;

  // InnoDB
  if ($data->version->compare_version(new dba_report_version(5, 0, 2), '>=')) {
    $return->innodb = new stdClass();
    $return->innodb->bufferPoolUsed = ($data->status->innodb_buffer_pool_pages_total - $data->status->innodb_buffer_pool_pages_free) * $data->status->innodb_page_size;

    $return->innodb->bufferPoolTotal = $data->status->innodb_buffer_pool_pages_total * $data->status->innodb_page_size;

    $return->innodb->bufferPoolReadRatio = $data->status->innodb_buffer_pool_read_requests != 0 ?  (1.0 - ($data->status->innodb_buffer_pool_reads / $data->status->innodb_buffer_pool_read_requests)) : 0;
  }

  return $return;
}

/**
 * Build mysqlreport output.
 */
function dba_mysql_report_output($data) {
  $output = array();
	
  // Key buffer section
  $output[] = dba_report_section(t('Key'));

  $percentage = dba_report_to_percentage($data->driver->myisam->keyBufferUsed / $data->variables->key_buffer_size);
  $alert = dba_report_alert(array($percentage), array(MYSQL_KEY_USED_PERCENT));
  $output[] = t('!openBuffer used    !used of !total %Used: !Used!close', array('!used' => sprintf('% 7s', dba_report_short($data->driver->myisam->keyBufferUsed, TRUE)), '!total' => sprintf('% 7s', dba_report_short($data->variables->key_buffer_size, TRUE)), '!Used' => $percentage, '!open' => _dba_alert_open($alert, dba_mysql_report_help_buffer_used(), $data->drush), '!close' => _dba_alert_close($alert, $data->drush)));

  if ($data->driver->myisam->keyBufferUsage != -1) {
    $percent = dba_report_to_percentage($data->driver->myisam->keyBufferUsage / $data->variables->key_buffer_size);
    $alert = dba_report_alert(array($percent), array(MYSQL_KEY_CURRENT_PERCENT));
    $output[] = t('!open  Current      !current            %Usage: !usage!close', array('!current' => sprintf('% 7s', dba_report_short($data->driver->myisam->keyBufferUsage, TRUE)), '!usage' => sprintf('%6.2f', $percent), '!open' => _dba_alert_open($alert, dba_mysql_report_help_buffer_used(), $data->drush), '!close' => _dba_alert_close($alert, $data->drush), '!open' => _dba_alert_open($alert, dba_mysql_report_help_buffer_used(), $data->drush), '!close' => _dba_alert_close($alert, $data->drush)));
    $percent = $data->driver->myisam->keyWriteRatio;
    $alert = dba_report_alert(array($percent), array(MYSQL_KEY_WRITE_HIT_PERCENT), '<=');
    $output[] = t('!openWrite hit      !hits!close', array('!hits' => sprintf('%6.2f%%', $percent), '!open' => _dba_alert_open($alert, dba_mysql_report_help_write_hit(), $data->drush), '!close' => _dba_alert_close($alert, $data->drush)));
    $percent = $data->driver->myisam->keyReadRatio;
    $alert = dba_report_alert(array($percent), array(MYSQL_KEY_READ_HIT_PERCENT), '<=');
    $output[] = t('!openRead hit       !hits!close', array('!hits' => sprintf('%6.2f%%', $percent), '!open' => _dba_alert_open($alert, dba_mysql_report_help_read_hit(), $data->drush), '!close' => _dba_alert_close($alert, $data->drush)));
  }

  // Questions section
  $output[] = dba_report_section(t('Questions'));
  $output[] = t('Total       !total  !rate/s', array('!total' => sprintf('%9s', dba_report_short($data->driver->myisam->questions)), '!rate' => sprintf('%6.2f', floatval($data->driver->myisam->questions) / floatval($data->status->uptime))));

  // format string is name, value, value, label, percentage
  $dtq_format = '  %-8s  %9s  %6.2f/s  %7s %6.2f';

  // Calculate the Total Com values
  $data->status->DTQ['Com_'] = 0;
  $data->status->COM = array();

  $temporary = get_object_vars($data->status);
  foreach (array_keys($temporary) as $key) {
    if (preg_match('/^Com_(.*)/', $key, $matches)) {
      $data->status->COM[$matches[1]] = $temporary[$key];
    }
  }

  arsort($data->status->COM);

  // delete commands that we count elsewhere
  unset($data->status->COM{'select'});
  unset($data->status->COM{'insert'});
  unset($data->status->COM{'insert_select'});
  unset($data->status->COM{'replace'});
  unset($data->status->COM{'replace_select'});
  unset($data->status->COM{'update'});
  unset($data->status->COM{'update_multi'});
  unset($data->status->COM{'delete'});
  unset($data->status->COM{'delete_multi'});

  $data->status->DTQ['Com_'] = array_sum($data->status->COM);

  $data->status->DTQ['DMS'] = $data->driver->myisam->DMS['DMS'];
	
  if ($data->status->qcache_hits != 0) {
    $data->status->DTQ['QC Hits'] = $data->status->qcache_hits;
  }

  $data->status->DTQ['COM_QUIT'] = ($data->status->connections - 2) - (int)($data->status->aborted_clients / 2);

  $total = array_sum($data->status->DTQ);

  if ($data->driver->myisam->questions != $total) {
    if ($data->driver->myisam->questions > $total) {
      $data->status->DTQ['+Unknown'] = abs($data->driver->myisam->questions - $total) ;
    }
    else {
      $data->status->DTQ['-Unknown'] = abs($data->driver->myisam->questions - $total) ;
    }
  }

  arsort($data->status->DTQ) ;

  $first = TRUE ;

  foreach ($data->status->DTQ as $key => $value) {
    $percentage = dba_report_to_percentage($value / $data->driver->myisam->questions);
    switch ($key) {
      case 'QC Hits':
        $alert = dba_report_alert(array($percentage), array(MYSQL_QUESTIONS_QC_HITS_PERCENT), '<=');
        $help = dba_mysql_report_help_qc_hits();
        break;
      case 'Com_':
        $alert = dba_report_alert(array($percentage), array(MYSQL_QUESTIONS_COM_PERCENT));
        $help = dba_mysql_report_help_com();
        break;
      default:
        $alert = FALSE;
        $help = '';
        break;
    }
    $output[] = t('!open!format!close', array('!format' => sprintf($dtq_format, $key, dba_report_short($value), ($value / $data->status->uptime), ($first ? t('%Total:') : ''), $percentage), '!open' => _dba_alert_open($alert, $help, $data->drush), '!close' => _dba_alert_close($alert, $data->drush)));
    $first = FALSE ;
  }

  $output[] = sprintf("Slow %-8s %7s  %6.2f/s          %6.2f  %%DMS: %6.2f  Log: %s", sprintf('%ds', $data->driver->myisam->slowQueryTime), dba_report_short($data->status->slow_queries), $data->status->slow_queries / $data->status->uptime, dba_report_to_percentage($data->status->slow_queries / $data->driver->myisam->questions), dba_report_to_percentage($data->status->slow_queries / $data->driver->myisam->DMS['DMS']), $data->variables->log_slow_queries);

  $format_dms = '%-10s  %9s  %6.2f/s          %6.2f' ;
  $format = '  %-8s  %9s  %6.2f/s          %6.2f        %6.2f' ;

  foreach ($data->driver->myisam->DMS as $key => $value) {
    if ($key == 'DMS') {
      $output[] = sprintf($format_dms, $key, dba_report_short($value), dba_report_to_percentage($value / $data->status->uptime), dba_report_to_percentage($value / $data->driver->myisam->questions));
    }
    else {
      $output[] = sprintf($format, $key, dba_report_short($value), dba_report_to_percentage($value / $data->status->uptime), dba_report_to_percentage($value / $data->driver->myisam->questions), dba_report_to_percentage($value / $data->driver->myisam->DMS['DMS']));
    }
  }

  $format_1 = 'Com_        %9s  %6.2f/s          %6.2f';
  $format_2 = '  %-11s %7s  %6.2f/s          %6.2f';

  $output[] = sprintf($format_1, dba_report_short($data->status->DTQ['Com_']), dba_report_short($data->status->DTQ['Com_'] / $data->status->uptime), dba_report_to_percentage($data->status->DTQ['Com_'] / $data->driver->myisam->DMS['DMS']));

  $i = variable_get('dba_mysql_report_com_lines', 5);
  foreach ($data->status->COM as $key => $value) {
    if ($value) {
      $output[] = sprintf($format_2, substr($key, 0, 11), dba_report_short($value), ($value / $data->status->uptime), dba_report_to_percentage($value / $data->driver->myisam->questions));
    }
    if (--$i == 0) {
      break;
    }
  }

  // SELECT and Sort section
  $output[] = dba_report_section(t('SELECT and Sort'));

  $percentage = dba_report_to_percentage($data->status->select_scan / $data->status->com_select);
  $rate = $data->status->select_scan / $data->status->uptime;
  $alert = dba_report_alert(array($rate, $percentage), array(MYSQL_SELECT_SCAN_RATE, MYSQL_SELECT_SCAN_PERCENT));
  $output[] = t('!openScan          !count   !rate/s %SELECT: !percent!close', array('!count' => sprintf('%7s', dba_report_short($data->status->select_scan)), '!rate' => sprintf('%5.2f', ($rate)), '!percent' => sprintf('%6.2f', $percentage), '!open' => _dba_alert_open($alert, dba_mysql_report_help_scan(), $data->drush), '!close' => _dba_alert_close($alert, $data->drush)));

  $output[] = t('Range         !count   !rate/s          !percent', array('!count' => sprintf('%7s', dba_report_short($data->status->select_range)), '!rate' => sprintf('%5.2f', ($data->status->select_range / $data->status->uptime)), '!percent' => sprintf('%6.2f', dba_report_to_percentage($data->status->select_range / $data->status->com_select)))); 

  $percentage = dba_report_to_percentage($data->status->select_full_join / $data->status->com_select);
  $rate = $data->status->select_full_join / $data->status->uptime;
  $alert = dba_report_alert(array($rate, $percentage), array(MYSQL_SELECT_FULL_JOIN_RATE, MYSQL_SELECT_FULL_JOIN_PERCENT));
  $output[] = t('!openFull join     !count   !rate/s          !percent!close', array('!count' => sprintf('%7s', dba_report_short($data->status->select_full_join)), '!rate' => sprintf('%5.2f', ($rate)), '!percent' => sprintf('%6.2f', $percentage), '!open' => _dba_alert_open($alert, dba_mysql_report_help_full_join(), $data->drush), '!close' => _dba_alert_close($alert, $data->drush)));

  $output[] = t('Range check   !count   !rate/s          !percent', array('!count' => sprintf('%7s', dba_report_short($data->status->select_range_check)), '!rate' => sprintf('%5.2f', ($data->status->select_range_check / $data->status->uptime)), '!percent' => sprintf('%6.2f', dba_report_to_percentage($data->status->select_range_check / $data->status->com_select)))); 

  $output[] = t('Full rng join !count   !rate/s          !percent', array('!count' => sprintf('%7s', dba_report_short($data->status->select_full_range_join)), '!rate' => sprintf('%5.2f', ($data->status->select_full_range_join / $data->status->uptime)), '!percent' => sprintf('%6.2f', dba_report_to_percentage($data->status->select_full_range_join / $data->status->com_select)))); 

  $output[] = t('Sort scan     !count   !rate/s', array('!count' => sprintf('%7s', dba_report_short($data->status->sort_scan)), '!rate' => sprintf('%5.2f', ($data->status->sort_scan / $data->status->uptime))));

  $output[] = t('Sort range    !count   !rate/s', array('!count' => sprintf('%7s', dba_report_short($data->status->sort_range)), '!rate' => sprintf('%5.2f', ($data->status->sort_range / $data->status->uptime))));

  $rate = $data->status->sort_merge_passes / $data->status->uptime;
  $alert = dba_report_alert(array($rate), array(MYSQL_SELECT_SORT_MERGE_PASS_RATE));
  $output[] = t('!openSort mrg pass !count   !rate/s!close', array('!count' => sprintf('%7s', dba_report_short($data->status->sort_merge_passes)), '!rate' => sprintf('%5.2f', ($data->status->sort_merge_passes / $data->status->uptime)), '!open' => _dba_alert_open($alert, dba_mysql_report_help_merge_passes(), $data->drush), '!close' => _dba_alert_close($alert, $data->drush)));

  // Query cache section (if enabled)
  if ((property_exists($data->variables, 'query_cache_size')) && ($data->variables->query_cache_size > 0)) {
    $qc_memory_used = $data->variables->query_cache_size - $data->status->qcache_free_memory ;

    $qc_hits_per_insert = $data->status->qcache_hits / ($data->status->qcache_inserts != 0 ? $data->status->qcache_inserts : 1);
    $qc_inserts_per_prune = $data->status->qcache_inserts / ($data->status->qcache_lowmem_prunes != 0 ? $data->status->qcache_lowmem_prunes : 1);

    $output[] = dba_report_section(t('Query Cache'));

    $percentage = dba_report_to_percentage($qc_memory_used / $data->variables->query_cache_size);
    $alert = dba_report_alert(array($percentage), array(MYSQL_QUERY_CACHE_USED_PERCENT));
    $output[] = t('!openMemory usage  !usage of !total  %Used: !used!close', array('!usage' => sprintf('%7s', dba_report_short($qc_memory_used, TRUE)), '!total' => sprintf('%7s', dba_report_short($data->variables->query_cache_size, TRUE)), '!used' => sprintf('%6.2f', $percentage), '!open' => _dba_alert_open($alert, dba_mysql_report_help_qc_usage(), $data->drush), '!close' => _dba_alert_close($alert, $data->drush)));

    $percentage = dba_report_to_percentage($data->status->qcache_free_blocks / $data->status->qcache_total_blocks);
    $alert = dba_report_alert(array($percentage), array(MYSQL_QUERY_CACHE_FRAG_PERCENT));
    $output[] = t('!openBlock Fragmnt !percent%!close', array('!percent' => sprintf('%6.2f', $percentage), '!open' => _dba_alert_open($alert, dba_mysql_report_help_qc_fragmented(), $data->drush), '!close' => _dba_alert_close($alert, $data->drush)));

    $output[] = t('Hits          !ratio   !rate/s', array('!ratio' => sprintf('%7s', dba_report_short($data->status->qcache_hits)), '!rate' => sprintf('%5.2f', $data->status->qcache_hits / $data->status->uptime)));
    $output[] = t('Inserts       !ratio   !rate/s', array('!ratio' => sprintf('%7s', dba_report_short($data->status->qcache_inserts)), '!rate' => sprintf('%5.2f', $data->status->qcache_inserts / $data->status->uptime)));
    $output[] = t('Insrt:Prune !ratio:1   !rate/s', array('!ratio' => sprintf('%7s', dba_report_short($qc_inserts_per_prune)), '!rate' => sprintf('%5.2f', (($data->status->qcache_inserts - $data->status->qcache_lowmem_prunes) / $data->status->uptime))));
    $output[] = t('Hit:Insert  !ratio:1   !rate/s', array('!ratio' => sprintf('%7s', dba_report_short($qc_hits_per_insert)), '!rate' => sprintf('%5.2f', ($qc_hits_per_insert / $data->status->uptime))));
  }

  $output[] = dba_report_section(t('Table Locks'));
  $percent = dba_report_to_percentage($data->status->table_locks_waited / ( $data->status->table_locks_waited + $data->status->table_locks_immediate));
  $rate = $data->status->table_locks_waited / $data->status->uptime;
  $alert = dba_report_alert(array($rate, $percent), array(MYSQL_LOCKS_WAIT_RATE, MYSQL_LOCKS_WAIT_PERCENT));
  $output[] = t('!openWaited      !count  !rate/s  %Total: !percent!close', array('!count' => sprintf('%9s', dba_report_short($data->status->table_locks_waited)), '!rate' => sprintf('%6.2f', $rate), '!percent' => sprintf('%6.2f', $percent), '!open' => _dba_alert_open($alert, dba_mysql_report_help_qc_fragmented(), $data->drush), '!close' => _dba_alert_close($alert, $data->drush)));

  $output[] = t('Immediate   !rate  !percent/s', array('!rate' => sprintf('%9s', dba_report_short($data->status->table_locks_immediate)), '!percent' => sprintf('%6.2f', $data->status->table_locks_immediate / $data->status->uptime)));

  $output[] = dba_report_section(t('Tables'));
	
  if (isset($data->variables->table_open_cache)) {
    $total = $data->variables->table_open_cache;
    $percent = dba_report_to_percentage($data->status->open_tables / $data->variables->table_open_cache);
  }
  else {
    $total = 0;
    $percent = 100;
  }
  $alert = dba_report_alert(array($percent), array(MYSQL_TABLE_CACHE_PERCENT));
  $output[] = t('!openOpen        !table of !total    %Cache: !percent!close', array('!table' => sprintf('%9d', $tables = $data->status->open_tables), '!total' => sprintf('%4d', $total), '!percent' => sprintf('%6.2f', $percent), '!open' => _dba_alert_open($alert, dba_mysql_report_help_table_cache(), $data->drush), '!close' => _dba_alert_close($alert, $data->drush)));

  $rate = $data->status->opened_tables / $data->status->uptime;
  $alert = dba_report_alert(array($rate), array(MYSQL_TABLE_RATE));
  $output[] = t('!openOpened      !count  !rate/s!close', array('!count' => sprintf('%9s', dba_report_short($data->status->opened_tables)), '!rate' => sprintf('%6.2f', $rate), '!open' => _dba_alert_open($alert, dba_mysql_report_help_tables(), $data->drush), '!close' => _dba_alert_close($alert, $data->drush)));

  $output[] = dba_report_section(t('Connections'));

  $percent = dba_report_to_percentage($data->status->max_used_connections / $data->variables->max_connections);
  $alert = dba_report_alert(array($percent), array(MYSQL_CONNECTIONS_PERCENT));
  $output[] = t('!openMax used    !count of !total      %Max: !percent!close', array('!count' => sprintf('%9d', $data->status->max_used_connections), '!total' => sprintf('%4d', $data->variables->max_connections), '!percent' => sprintf('%6.2f', $percent), '!open' => _dba_alert_open($alert, dba_mysql_report_help_connections(), $data->drush), '!close' => _dba_alert_close($alert, $data->drush)));

  $output[] = t('Total       !count  !rate/s', array('!count' => sprintf('%9s', dba_report_short($data->status->connections)), '!rate' => sprintf('%6.2f', $data->status->connections / $data->status->uptime)));

  $output[] = dba_report_section(t('Created Temp'));
	
  $rate = $data->status->created_tmp_disk_tables / $data->status->uptime;
  $alert = dba_report_alert(array($rate), array(MYSQL_TMP_DISK_TABLE_RATE));
  $output[] = t('!openDisk table  !number  !rate/s!close', array('!number' => sprintf('%9s', dba_report_short($data->status->created_tmp_disk_tables)), '!rate' => sprintf('%6.2f', $rate), '!open' => _dba_alert_open($alert, dba_mysql_report_help_tmp_disk_table(), $data->drush), '!close' => _dba_alert_close($alert, $data->drush)));

  $output[] = t('Table       !number  !rate/s    Size: !size', array('!number' => sprintf('%9s', dba_report_short($data->status->created_tmp_tables)), '!rate' => sprintf('%6.2f', $data->status->created_tmp_tables / $data->status->uptime), '!size' => sprintf('%6s', dba_report_short($data->variables->tmp_table_size, TRUE, 1))));

  $output[] = t('File        !number  !rate/s', array('!number' => sprintf('%9s', dba_report_short($data->status->created_tmp_files)), '!rate' => sprintf('%6.2f', $data->status->created_tmp_files / $data->status->uptime)));

  $output[] = dba_report_section(t('Threads'));

  $output[] = t('Running     !number of !total', array('!number' => sprintf('%9d', $data->status->threads_running), '!total' => sprintf('%4d', $data->status->threads_connected)));

  $percent = dba_report_to_percentage(1.0 - ($data->status->threads_created / $data->status->connections)) ;
  $alert = dba_report_alert(array($percent), array(MYSQL_THREADS_PERCENT), '<=');
  $output[] = t('!openCached      !number of !total      %Hit: !percent!close', array('!number' => sprintf('%9d', $data->status->threads_cached), '!total' => sprintf('%4d', $data->variables->thread_cache_size), '!percent' => sprintf('%6.2f', $percent), '!open' => _dba_alert_open($alert, dba_mysql_report_help_thread_cache(), $data->drush), '!close' => _dba_alert_close($alert, $data->drush)));

  $rate = $data->status->threads_created / $data->status->uptime;
  $alert = dba_report_alert(array($rate), array(MYSQL_THREADS_RATE));
  $output[] = t('!openCreated     !number  !rate/s!close', array('!number' => sprintf('%9s', dba_report_short($data->status->threads_created)), '!rate' => sprintf('%6.2f', $rate), '!open' => _dba_alert_open($alert, dba_mysql_report_help_thread_cache(), $data->drush), '!close' => _dba_alert_close($alert, $data->drush)));

  $rate = $data->status->slow_launch_threads / $data->status->uptime;
  $alert = dba_report_alert(array($rate), array(MYSQL_SLOW_RATE));
  $output[] = t('!openSlow        !number  !rate/s!close', array('!number' => sprintf('%9s', dba_report_short($data->status->slow_launch_threads)), '!rate' => sprintf('%6.2f', $rate), '!open' => _dba_alert_open($alert, dba_mysql_report_help_slow(), $data->drush), '!close' => _dba_alert_close($alert, $data->drush)));

  $output[] = dba_report_section(t('Aborted'));

  $rate = $data->status->aborted_clients / $data->status->uptime;
  $alert = dba_report_alert(array($rate), array(MYSQL_ABORT_CLIENT_RATE));
  $output[] = t('!openClients     !number  !rate/s!close', array('!number' => sprintf('%9s', dba_report_short($data->status->aborted_clients)), '!rate' => sprintf('%6.2f', $rate), '!open' => _dba_alert_open($alert, dba_mysql_report_help_abort_client(), $data->drush), '!close' => _dba_alert_close($alert, $data->drush)));

  $rate = $data->status->aborted_connects / $data->status->uptime;
  $alert = dba_report_alert(array($rate), array(MYSQL_ABORT_CONNECT_RATE));
  $output[] = t('!openConnects    !number  !rate/s!close', array('!number' => sprintf('%9s', dba_report_short($data->status->aborted_connects)), '!rate' => sprintf('%6.2f', $rate), '!open' => _dba_alert_open($alert, dba_mysql_report_help_abort_connect(), $data->drush), '!close' => _dba_alert_close($alert, $data->drush)));

  $output[] = dba_report_section(t('Bytes'));

  $output[] = t('Sent        !number  !rate/s', array('!number' => sprintf('%9s', dba_report_short($data->status->bytes_sent, TRUE)), '!rate' => sprintf('%6.2f', $data->status->bytes_sent / $data->status->uptime)));
  $output[] = t('Received    !number  !rate/s', array('!number' => sprintf('%9s', dba_report_short($data->status->bytes_received, TRUE)), '!rate' => sprintf('%6.2f', $data->status->bytes_received / $data->status->uptime)));

  // Be sure we're using a modern enough version of MySQL
  if ($data->version->compare_version(new dba_report_version(5, 0, 2), '<')) {
    return $output ;
  }

  // Be sure InnoDB is enabled
  if (!property_exists($data->status, 'innodb_page_size')) {
    return $output ;
  }

  $output[] = dba_report_section(t('InnoDB Buffer Pool'));

  $percent = dba_report_to_percentage($data->driver->innodb->bufferPoolUsed / $data->driver->innodb->bufferPoolTotal);
  $alert = dba_report_alert(array($percent), array(MYSQL_INNODB_BUFFER_PERCENT));
  $output[] = t('!openUsage         !number of !total  %Used: !percent!close', array('!number' => sprintf('%7s', dba_report_short($data->driver->innodb->bufferPoolUsed, TRUE)), '!total' => sprintf('%7s', dba_report_short($data->driver->innodb->bufferPoolTotal, TRUE)), '!percent' => sprintf('%6.2f', $percent), '!open' => _dba_alert_open($alert, dba_mysql_report_help_innodb_buffer(), $data->drush), '!close' => _dba_alert_close($alert, $data->drush)));
			
  $percent = dba_report_to_percentage($data->driver->innodb->bufferPoolReadRatio);
  $alert = dba_report_alert(array($percent), array(MYSQL_INNODB_READ_HIT_PERCENT), '<');
  $output[] = t('!openRead hit      !percent%!close', array('!percent' => sprintf('%6.2f', $percent), '!open' => _dba_alert_open($alert, dba_mysql_report_help_innodb_read_hit(), $data->drush), '!close' => _dba_alert_close($alert, $data->drush)));

  $output[] = t('Pages');
	
  $output[] = t('  Free      !number            %Total: !percent', array('!number' => sprintf('%9s', dba_report_short($data->status->innodb_buffer_pool_pages_free)), '!percent' => sprintf('%6.2f', dba_report_to_percentage($data->status->innodb_buffer_pool_pages_free / $data->status->innodb_buffer_pool_pages_total))));

  $output[] = t('  Data      !number                    !percent %Drty: !dirty', array('!number' => sprintf('%9s', dba_report_short($data->status->innodb_buffer_pool_pages_data)), '!percent' => sprintf('%6.2f', dba_report_to_percentage($data->status->innodb_buffer_pool_pages_data / $data->status->innodb_buffer_pool_pages_total)), '!dirty' => sprintf('%6.2f', dba_report_to_percentage($data->status->innodb_buffer_pool_pages_dirty / $data->status->innodb_buffer_pool_pages_data))));

  $output[] = t('  Misc      !number            %Total: !percent', array('!number' => sprintf('%9s', dba_report_short($data->status->innodb_buffer_pool_pages_misc)), '!percent' => sprintf('%6.2f', dba_report_to_percentage($data->status->innodb_buffer_pool_pages_misc / $data->status->innodb_buffer_pool_pages_total))));
			
  // This only exists from MySQL 5.0.2 until MySQL 5.0.68.  It exists in
  // 5.0.68+ if the UNIX_DEBUG system is defined at build time. From:
  // http://dev.mysql.com/doc/refman/5.0/en/server-status-variables.html
  if (isset($data->status->innodb_buffer_pool_pages_latched)) {
    $output[] = t('  Latched   !number            %Total: !percent', array('!number' => sprintf('%9s', dba_report_short($data->status->innodb_buffer_pool_pages_latched)), '!percent' => sprintf('%6.2f', dba_report_to_percentage($data->status->innodb_buffer_pool_pages_latched / $data->status->innodb_buffer_pool_pages_total))));
  }
			
  $output[] = t('Reads       !number  !rate/s', array('!number' => sprintf('%9s', dba_report_short($data->status->innodb_buffer_pool_read_requests)), '!rate' => sprintf('%6.2f', $data->status->innodb_buffer_pool_read_requests / $data->status->uptime)));
  $output[] = t('  From file !number  !rate/s          !percent', array('!number' => sprintf('%9s', dba_report_short($data->status->innodb_buffer_pool_reads)), '!rate' => sprintf('%6.2f', $data->status->innodb_buffer_pool_reads / $data->status->uptime), '!percent' => sprintf('%6.2f', dba_report_to_percentage($data->status->innodb_buffer_pool_reads / $data->status->innodb_buffer_pool_read_requests))));
  if (isset($data->status->innodb_buffer_pool_read_ahead_rnd)) {
    $output[] = t('  Ahead Rnd !number  !rate/s', array('!number' => sprintf('%9s', dba_report_short($data->status->innodb_buffer_pool_read_ahead_rnd)), '!rate' => sprintf('%6.2f', $data->status->innodb_buffer_pool_read_ahead_rnd / $data->status->uptime)));
  }
  if (isset($data->status->innodb_buffer_pool_read_ahead_seq)) {
    $output[] = t('  Ahead Sql !number  !rate/s', array('!number' => sprintf('%9s', dba_report_short($data->status->innodb_buffer_pool_read_ahead_seq)), '!rate' => sprintf('%6.2f', $data->status->innodb_buffer_pool_read_ahead_seq / $data->status->uptime)));
  }

  $output[] = t('Writes      !number  !rate/s', array('!number' => sprintf('%9s', dba_report_short($data->status->innodb_buffer_pool_write_requests)), '!rate' => sprintf('%6.2f', $data->status->innodb_buffer_pool_write_requests / $data->status->uptime)));

  $rate = $data->status->innodb_buffer_pool_pages_flushed / $data->status->uptime;
  if ($data->variables->innodb_flush_log_at_trx_commit == 1) {
    $alert = TRUE;
  }
  else {
    $alert = FALSE;
  }
  $output[] = t('!openFlushes     !number  !rate/s!close', array('!number' => sprintf('%9s', dba_report_short($data->status->innodb_buffer_pool_pages_flushed)), '!rate' => sprintf('%6.2f', $rate), '!open' => _dba_alert_open($alert, dba_mysql_report_help_innodb_flush(), $data->drush), '!close' => _dba_alert_close($alert, $data->drush)));

  $output[] = t('Wait Free   !number  !rate/s', array('!number' => sprintf('%9s', dba_report_short($data->status->innodb_buffer_pool_wait_free)), '!rate' => sprintf('%6.2f', $data->status->innodb_buffer_pool_wait_free / $data->status->uptime)));

  if ($data->version->compare_version(new dba_report_version(5, 0, 3), '>=')) {
    $output[] = dba_report_section(t('InnoDB Lock'));
		
    $output[] = t('Waits       !number  !rate/s', array('!number' => sprintf('%9s', dba_report_short($data->status->innodb_row_lock_waits)), '!rate' => sprintf('%6.2f', $data->status->innodb_row_lock_waits / $data->status->uptime)));
    $output[] = t('Current     !number', array('!number' => sprintf('%9s', dba_report_short($data->status->innodb_row_lock_current_waits))));
		
    $output[] = t('Time acquiring');

    $output[] = t('  Total     !number ms', array('!number' => sprintf('%9s', $data->status->innodb_row_lock_time)));
    $output[] = t('  Average   !number ms', array('!number' => sprintf('%9s', $data->status->innodb_row_lock_time_avg)));
    $output[] = t('  Max       !number ms', array('!number' => sprintf('%9s', $data->status->innodb_row_lock_time_max)));
  }

  $output[] = dba_report_section(t('InnoDB Data, Pages, Rows'));
  $output[] = t('Data');

  $output[] = t('  Reads     !number  !rate/s', array('!number' => sprintf('%9s', dba_report_short($data->status->innodb_buffer_pool_reads)), '!rate' => sprintf('%6.2f', $data->status->innodb_buffer_pool_reads / $data->status->uptime)));
  $output[] = t('  Writes    !number  !rate/s', array('!number' => sprintf('%9s', dba_report_short($data->status->innodb_data_writes)), '!rate' => sprintf('%6.2f', $data->status->innodb_data_writes / $data->status->uptime)));
  $output[] = t('  fsync     !number  !rate/s', array('!number' => sprintf('%9s', dba_report_short($data->status->innodb_data_fsyncs)), '!rate' => sprintf('%6.2f', $data->status->innodb_data_fsyncs / $data->status->uptime)));
			
  $output[] = t('  Pending');
	
  $output[] = t('    Reads   !number', array('!number' => sprintf('%9s', dba_report_short($data->status->innodb_data_pending_reads))));
  $output[] = t('    Writes  !number', array('!number' => sprintf('%9s', dba_report_short($data->status->innodb_data_pending_writes))));
  $output[] = t('    fsync   !number', array('!number' => sprintf('%9s', dba_report_short($data->status->innodb_data_pending_fsyncs))));

  $output[] = t('Pages');
	
  $output[] = t('  Created   !number  !rate/s', array('!number' => sprintf('%9s', dba_report_short($data->status->innodb_pages_created)), '!rate' => sprintf('%6.2f', $data->status->innodb_pages_created / $data->status->uptime)));
  $output[] = t('  Read      !number  !rate/s', array('!number' => sprintf('%9s', dba_report_short($data->status->innodb_pages_read)), '!rate' => sprintf('%6.2f', $data->status->innodb_pages_read / $data->status->uptime)));
  $output[] = t('  Written   !number  !rate/s', array('!number' => sprintf('%9s', dba_report_short($data->status->innodb_pages_written)), '!rate' => sprintf('%6.2f', $data->status->innodb_pages_written / $data->status->uptime)));

  $output[] = t('Rows');
  $output[] = t('  Deleted   !number  !rate/s', array('!number' => sprintf('%9s', dba_report_short($data->status->innodb_rows_deleted)), '!rate' => sprintf('%6.2f', $data->status->innodb_rows_deleted / $data->status->uptime)));
  $output[] = t('  Inserted  !number  !rate/s', array('!number' => sprintf('%9s', dba_report_short($data->status->innodb_rows_inserted)), '!rate' => sprintf('%6.2f', $data->status->innodb_rows_inserted / $data->status->uptime)));
  $output[] = t('  Read      !number  !rate/s', array('!number' => sprintf('%9s', dba_report_short($data->status->innodb_rows_read)), '!rate' => sprintf('%6.2f', $data->status->innodb_rows_read / $data->status->uptime)));
  $output[] = t('  Updated   !number  !rate/s', array('!number' => sprintf('%9s', dba_report_short($data->status->innodb_rows_updated)), '!rate' => sprintf('%6.2f', $data->status->innodb_rows_updated / $data->status->uptime)));

  return $output ;
}

/*
 * Provide tuning advice for high key buffer usage.
 */
function dba_mysql_report_help_buffer_used() {
  return t("You have used a significant amount of the allocated key buffer.  The key buffer is used only by the MyISAM storage engine (not the InnoDB storage engine), however note that MySQL uses the MyISAM storage engine for all temporary tables even if the InnoDB storage engine is being used for everything else.\n\nTo improve MyISAM performance, consider raising the key_buffer_size tunable to allocate more memory to the MySQL key buffer.");
}

/*
 * Provide tuning advice for low key buffer write rate.
 */
function dba_mysql_report_help_write_hit() {
  return t("This is the ratio of key writes to the hard disk to key writes to RAM expressed as a percentage. You have a low hit rate, suggesting that your key buffer is not performing optimally. You should review what is causing this.");
}

/*
 * Provide tuning advice for low key buffer read rate.
 */
function dba_mysql_report_help_read_hit() {
  return t("This is the ratio of key reads from the hard disk to key read from RAM expressed as a percentage. You have a low hit rate, suggesting that your key buffer is not performing optimally. You should review what is causing this.\n\nIf MySQL has only been running for 1 or 2 hours, give this value more time to improve.");
}

/*
 * Provide tuning advice for low percentage of QC Hits queries.
 */
function dba_mysql_report_help_qc_hits() {
  return t("QC stands for Query Cache. Ideally, this should account for the majority of your questions.  Tuning your MySQL query cache could result in better performance.");
}

/*
 * Provide tuning advice for high percentage of Com_ queries.
 */
function dba_mysql_report_help_com() {
  return t("_Com accounts for all MySQL commands, primarily those that are protocol related. This should be very small. Something is causing an abnormally high percentage of MySQL commands on your server.\n\nLook at the end of the Questions section of this report to see which MySQL commands are being executed so frequently.");
}

/*
 * Provide tuning advice for high percentage of scans.
 */
function dba_mysql_report_help_scan() {
  return t("A SCAN is a SELECT query that required scanning the entire table instead of just a subset of it. These types of queries will often show up in your slow query log, and should be optimized by adding a LIMIT to the queries.");
}

/**
 * Provide tuning advice for high percentage of full joins.
 */
function dba_mysql_report_help_full_join() {
  return t("A FULL JOIN is a SELECT query that joins two or more tables together, and then scans the entirety of the joined tables. These types of queries will often show up in your slow query log, and should be optimized by adding a LIMIT to the queries.\n\nAs you are seeing a significant number of joins, you may benefit from increasing the join_buffer_size tunable. Note that this is a per-connection memory allocation, so don't increase the setting too quickly and monitor the change closely to be sure you've not over allocated memory.");
}

function dba_mysql_report_help_merge_passes() {
  return t("When executing your sort queries, the database is frequently having to process them in multiple passes.  You may improve database performance by increasing your sort_buffer_size tunable.  Note that this is a per-connection memory allocation, so don't increase the setting too quickly and monitor the change closely to be sure you've not over allocated memory.");
}

function dba_mysql_report_help_qc_usage() {
  return t("Your query cache is getting full, you may want to allocate more memory.  However, the query cache has many limitations so it should not be blindly increased on the assumption that this will help.  Prior to MySQL 5.0.50 there was a locking bug in MySQL which prevented you from being able to increase your query cache beyond 64M in size.  Even in later version of MySQL it is not recommend you increase the cache beyond 256M unless you've found a measurable improvement in performance.\n\nTo adjust the size of your query cache, set the query_cache_size tunable.");
}

function dba_mysql_report_help_qc_fragmented() {
  return t("Your query cache is highly fragmented, reducing its effectiveness.  Sometimes increasing the query_cache_size tunable helps reduce fragmentation. For Drupal websites, you can also try decreasing the query_cache_min_res_unit tunable to 2048 or 1024.");
}

function dba_mysql_report_help_lock_wait() {
  return t("A significant number of your table locks are waiting for other locks to be released. Review your slow query log, this could be caused by poorly indexed or otherwise poorly optimized queries.");
}

function dba_mysql_report_help_table_cache() {
  return t("Your table cache is full, you may want to consider increasing it. However, monitor your total memory consumption carefully, and don't set this value larger than you need. Note that different threads can open the same table, thus on a busy database you can frequently have more tables open than actually exist in your database.\n\nAdjust the table_cache tunable.");
}

function dba_mysql_report_help_tables() {
  return t("You are opening new tables quickly and may benefit from increasing the table_cache tunable. Monitor total memory consumption carefully if increasing this tunable.");
}

function dba_mysql_report_help_connections() {
  return t("At some time since MySQL was last started, you were using all available MySQL connections. By default, MySQL allows 100 simultaneous connections, however on a well tuned server most queries last less than a second so even on a busy web server you rarely have more than a couple dozen simultaneous connections. It is generally not advisable to increase the connection limit beyond 100, unless you already have a well tuned server and the database is supporting multiple web servers. Before you consider raising this limit, look into first optimizing your queries and perform other tuning.\n\nTo allow additional MySQL connections, adjust the max_connections tunable.");
}

function dba_mysql_report_help_tmp_disk_table() {
  return t("Temporary tables are frequently being created on the filesystem, reducing performance. One cause of this is when temporary tables have TEXT or BLOB fields, as then they can't be built in memory. To allocate more memory for temporary tables, adjust both the tmp_table_size and max_heap_table_size to the same value.  Note that this memory is allocated per-connection, so be careful to not increase it too quickly. It is rarely beneficial to increase these tunables above 32M on a Drupal website.");
}

function dba_mysql_report_help_thread_cache() {
  return t("There is minimal overhead in creating threads, however if you see a large number of threads being created per second that aren't using the thread cache, you can reduce your CPU load by increasing your thread cache by adjusting the thread_cache_size tunable.");
}

function dba_mysql_report_help_slow() {
  return t("This database is processing a large number of slow queries. You should review the slow query log, adding indexes were necessary and otherwise optimizing the slow queries.");
}

function dba_mysql_report_help_abort_client() {
  return t("Investigate why there are so many aborted clients.");
}

function dba_mysql_report_help_abort_connect() {
  return t("Investigate why there are so many aborted connections.");
}

function dba_mysql_report_help_innodb_buffer() {
  return t("As a general rule, if you are using InnoDB, approximately 70% of your available RAM should be given to MySQL's InnoDB Buffer Pool.  Most of your buffer pool is being used, you should look into making more RAM available to InnoDB. You do not ever want your Buffer Pool to be 100% full.  However, you also don't ever want MySQL to cause your operating system to swap.\n\nTo allocate more memory for the InnoDB buffer pool, adjust the innodb_buffer_pool_size tunable.");
}

function dba_mysql_report_help_innodb_read_hit() {
  return t("Your InnoDB buffer pool hit rate is too low, suggesting you may want to make more RAM available to InnoDB. However, you also don't ever want MySQL to cause your operating system to swap.\n\nTo allocate more memory for the InnoDB buffer pool, adjust the innodb_buffer_pool_size tunable.");
}

function dba_mysql_report_help_innodb_flush() {
  return t("By default, InnoDB is ACID compliant. This means that each transaction needs to be immediately flushed to disk. If you can afford to loose one second of transactions in the event of a database or server failure, you should tune InnoDB to only flush to disk once every second.\n\nSet the innodb_flush_log_at_trx_commit tunable to 0 to have InnoDB only flush to disk once every second.");
}
