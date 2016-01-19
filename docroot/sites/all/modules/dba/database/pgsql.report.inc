<?php

define('PGSQL_CONNECTIONS_PERCENT', 85);

/**
 * Return current PostgreSQL version.
 */
function dba_pgsql_report_version() {
  $result = db_query("SELECT version()")->fetch();
  preg_match('/(\d{1,2})\.(\d{1,2})\.(\d{1,2})/', $result->version, $matches) ;
  array_shift($matches);
  $version = new dba_report_version($matches[0], $matches[1], $matches[2]) ;
  return $version;
}

/**
 * Return the pretty name for this database type.
 */
function dba_pgsql_report_name() {
  return t('PostgreSQL');
}

/**
 * No way to get the PostgreSQL uptime.
 */
function dba_pgsql_report_uptime() {
  return t('Unknown');
}

/**
 * Status.
 * TODO:  http://www.postgresql.org/docs/current/static/monitoring-stats.html
 * For example: SELECT * FROM pg_stat_bgwriter;
 */
function dba_pgsql_report_status($data) {
  $status = new stdClass();

  // Active connections
  $result = db_query('SELECT COUNT(*) FROM pg_stat_activity')->fetch();
  $status->connections = $result->count;

  // Locks
  $result = db_query('SELECT COUNT(*) FROM pg_locks WHERE granted = TRUE')->fetch();
  $status->locks_held = $result->count;
  $result = db_query('SELECT COUNT(*) FROM pg_locks WHERE granted = FALSE')->fetch();
  $status->locks_wait = $result->count;
  $status->locks_total = $status->locks_wait + $status->locks_held;

  return $status;
}

/**
 *  Variables.
 */
function dba_pgsql_report_variables($data) {
  $variables = new stdClass();

  $query = db_query('SHOW ALL');
  foreach ($query as $row) {
    $name = $row->name;
    $variables->$name = $row->setting;
  }
  return $variables;
}

/**
 * PostgreSQL specific stuff.
 */
function dba_pgsql_report_driver($data) {
  $driver = new stdClass();
  return $driver;
}

/**
 * PostgreSQL report.
 */
function dba_pgsql_report_output($data) {
  $output = array();

  $output[] = '';

  $output[] = dba_report_section(t('Connections'));

  $percent = dba_report_to_percentage($data->status->connections / $data->variables->max_connections);
  $alert = dba_report_alert(array($percent), array(PGSQL_CONNECTIONS_PERCENT));
  $output[] = t('!openUsed        !count of !total      %Max: !percent!close', array('!count' => sprintf('%9d', $data->status->connections), '!total' => sprintf('%4d', $data->variables->max_connections), '!percent' => sprintf('%6.2f', $percent), '!open' => _dba_alert_open($alert, dba_pgsql_report_help_connections(), $data->drush), '!close' => _dba_alert_close($alert, $data->drush)));

  $output[] = dba_report_section(t('Locks'));

  $output[] = t('Held        !count of !total         %: !percent', array('!count' => sprintf('%9d', $data->status->locks_held), '!total' => sprintf('%4d', $data->status->locks_total), '!percent' => sprintf('%6.2f', dba_report_to_percentage($data->status->locks_held / $data->status->locks_total))));
  $output[] = t('Wait        !count of !total         %: !percent', array('!count' => sprintf('%9d', $data->status->locks_wait), '!total' => sprintf('%4d', $data->status->locks_total), '!percent' => sprintf('%6.2f', dba_report_to_percentage($data->status->locks_wait / $data->status->locks_total))));
	
  $output[] = '';
  $output[] = t('No further PostgreSQL reporting functionality has been implemented yet.');
  $output[] = t('Contributers welcome.');
  $output[] = t('Submit patches to !url.', array('!url' => l(t('the DBA module issue queue'), 'http://drupal.org/project/issues/dba?categories=All')));
  $output[] = '';
  $output[] = t('Variable dump:');
  foreach ($data->variables as $key => $value) {
    $output[] = '  '. $key .'  '. $value;
  }
  return $output;
}

function dba_pgsql_report_help_connections() {
  return t("PostgreSQL is currently using a significant percentage of available connections. By default, PostgreSQL allows 100 simultaneous connections, however on a well tuned server most queries last less than a second so even on a busy web server you rarely have more than a couple dozen simultaneous connections. It is generally not advisable to increase the connection limit beyond 100, unless you already have a well tuned server and the database is supporting multiple web servers. Before you consider raising this limit, look into first optimizing your queries and perform other tuning.\n\nTo allow additional PostgreSQL connections, adjust the max_connections tunable.");
}
