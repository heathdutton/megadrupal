<?php

/**
 * @file
 * Provides database driver specific report functions.
 */

/**
 * Return current SQLite version.
 */
function dba_sqlite_report_version() {
  $result = db_query("SELECT sqlite_version() AS version");
  foreach ($result as $row) {
    $sqlite_version = explode('.', $row->version);
    break;
  }
  $version = new dba_report_version($sqlite_version[0], $sqlite_version[1], $sqlite_version[2]);
  return $version;
}

/**
 * Return the pretty name for this database type.
 */
function dba_sqlite_report_name() {
  return t('SQLite');
}

/**
 * No way to get the SQLite uptime.
 */
function dba_sqlite_report_uptime() {
  return t('Unknown');
}

/**
 * @todo: Does SQLite maintain useful statistics?
 */
function dba_sqlite_report_status() {
  $status = new stdClass();
  return $status;
}

/**
 * The PRAGMA statement is an SQL extension specific to SQLite and used to
 * modify the operation of the SQLite library or to query the SQLite library
 * for internal (non-table) data. See https://sqlite.org/pragma.html
 */
function dba_sqlite_report_variables() {
  $variables = new stdClass();

  // @todo: Is there some way to confirm that a given PRAGMA is available
  // with the active version of SQLite?
  $pragmas = array('auto_vacuum', 'automatic_index', 'cache_size', 'encoding', 'freelist_count', 'fullfsync', 'ignore_check_constraints', 'journal_mode', 'journal_size_limit', 'legacy_file_format', 'locking_mode', 'max_page_count', 'mmap_size', 'page_count', 'page_size', 'read_uncommitted', 'recursive_triggers', 'reverse_unordered_selects', 'schema_version', 'user_version', 'secure_delete', 'synchronous', 'temp_store', 'wal_autocheckpoint');
  foreach ($pragmas as $pragma) {
    $result = db_query("PRAGMA $pragma");
    $value = $result->fetchObject();
    $variables->$pragma = $value->$pragma;
  }
  return $variables;
}

/**
 * SQLite specific stuff.
 * @todo: Does SQLite maintain useful statistics?
 */
function dba_sqlite_report_driver($data) {
  $driver = new stdClass();
  return $driver;
}

/**
 * SQLite report.
 * @todo: Does SQLite maintain useful statistics?
 */
function dba_sqlite_report_output($data) {
  $output = array();
  $output[] = '';

  $output[] = t('No SQLite reporting functionality has been implemented yet.');
  $output[] = t('Contributers welcome.');
  $output[] = t('Submit patches to !url.', array('!url' => l(t('the DBA module issue queue'), 'http://drupal.org/project/issues/dba?categories=All')));

  $output[] = dba_report_section(t('Variables (PRAGMA)'));
  foreach ($data->variables as $key => $value) {
    $output[] = t('  !key!spaces!value', array('!key' => $key, '!value' => l($value, 'https://sqlite.org/pragma.html', array('external' => TRUE, 'fragment' => "pragma_$key")), '!spaces' => dba_report_spaces(30 - strlen($key))));
  }

  return $output;
};
