<?php

/**
 * @file
 * Handles counts of taxonomy term views via Ajax with minimal bootstrap.
 */

/**
* Root directory of Drupal installation.
*/
define('HTTP_REFERER_BASE', substr($_SERVER['HTTP_REFERER'], 0, strpos($_SERVER['HTTP_REFERER'], '/taxonomy/term/')));
define('DRUPAL_ROOT', str_replace($_SERVER['REQUEST_SCHEME'] . '://' . $_SERVER['SERVER_NAME'], $_SERVER['DOCUMENT_ROOT'], HTTP_REFERER_BASE));
// Change the directory to the Drupal root.
chdir(DRUPAL_ROOT);

include_once DRUPAL_ROOT . '/includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_VARIABLES);
if (variable_get('taxonomy_term_statistics_count_term_views', 0) && variable_get('taxonomy_term_statistics_count_term_views_ajax', 0)) {
  $tid = $_POST['tid'];
  if (is_numeric($tid)) {
    db_merge('taxonomy_term_counter')
      ->key(array('tid' => $tid))
      ->fields(array(
        'daycount' => 1,
        'totalcount' => 1,
        'timestamp' => REQUEST_TIME,
      ))
      ->expression('daycount', 'daycount + 1')
      ->expression('totalcount', 'totalcount + 1')
      ->execute();
  }
}
