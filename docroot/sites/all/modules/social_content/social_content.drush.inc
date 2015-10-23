<?php
/**
 * @file
 * Social Content module drush integration file.
 */

/**
 * Implements hook_drush_command().
 */
function social_content_drush_command() {
  $items = array();
  $items['social-content-import'] = array(
    'description' => dt('Run the import process for a given list of instances.'),
    'arguments' => array(
      'instances' => dt('comma separated list of instance ids.'),
    ),
    'examples' => array(
      'Single argument example' => 'drush social-content-import 1',
      'Multiple argument example' => 'drush social-content-import 1,2,3',
    ),
    'aliases' => array('sci'),
  );
  $items['social-content-list'] = array(
    'description' => dt('List all social content instances.'),
    'arguments' => array(),
    'aliases' => array('scl'),
  );
  $items['social-content-purge'] = array(
    'description' => dt('Delete old nodes imported by Social Content.'),
    'arguments' => array(),
    'aliases' => array('scp'),
  );
  return $items;
}

/**
 * Callback function for drush social-content-import command.
 *
 * @param string $instances
 *   List of comma separated instance ids.
 */
function drush_social_content_import($instances = NULL) {

  if (!$instances) {
    drush_log('Please provide list of instances', 'error');
  }
  else {
    $instances = explode(',', $instances);
    foreach ($instances as $id) {
      $instance = social_content_load($id);
      drush_log('Running social content import for: ' . $instance->getInstanceTitle() . '...', 'ok');
      $stats = social_content_run_import($instance);
      $args = array(
        '%title' => $instance->getInstanceTitle(),
        '!processed' => $stats['processed'],
        '!imported' => $stats['imported'],
        '!skipped' => $stats['skipped'],
      );
      $message = dt('processed !processed, imported !imported, skipped !skipped nodes', $args);
      drush_log($message, 'ok');
    }
  }
}

/**
 * Callback function for drush social-content-list command.
 */
function drush_social_content_list() {

  $instances = SocialContent::getAllInstances();
  $rows = array(array('ID', 'Title', 'Network', 'Count', 'Cron'));
  foreach ($instances as $instance) {
    $rows[]= array($instance->id, $instance->settings['title'], $instance->global, $instance->settings['count'], $instance->enabled ? 'Yes' : 'No');
  }
  drush_print_table($rows, TRUE);
}

/**
 * Callback function for drush social-content-purge command.
 */
function drush_social_content_purge() {

  $count = social_content_delete_old_nodes();
  $message = dt('!count nodes have been deleted.', array(
    '!count' => $count,
  ));
  drush_log($message, 'ok');
}
