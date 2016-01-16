<?php

/**
 * @todo Does SQLite provide any uptime information?
 */
function dba_sqlite_statpack_uptime($report) {
  return '';
}

/**
 * SQLite report.
 * @todo: Does SQLite maintain useful statistics?
 */
function dba_sqlite_statpack_output($data) {
  $output = array();
  $output[] = '';

  $output[] = t('No SQLite statpack functionality has been implemented yet.');
  $output[] = t('Contributers welcome.');
  $output[] = t('Submit patches to !url.', array('!url' => l(t('the DBA module issue queue'), 'http://drupal.org/project/issues/dba?categories=All')));

  return $output;
};
