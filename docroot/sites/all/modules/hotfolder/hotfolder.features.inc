<?php
/**
 * @file
 * hotfolder.features.inc
 */

/**
 * Implements hook_views_api().
 */
function hotfolder_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function hotfolder_node_info() {
  $items = array(
    'job_record' => array(
      'name' => t('Job Record'),
      'base' => 'node_content',
      'description' => t('Use job records to track jobs processed by hotfolder configurations.'),
      'has_title' => '1',
      'title_label' => t('Job ID'),
      'help' => '',
    ),
    'watch_configuration' => array(
      'name' => t('Watch Configuration'),
      'base' => 'node_content',
      'description' => t('Use watch configurations to create routines to watch specified hotfolders for data, define processes for them, and drop folders for the results.'),
      'has_title' => '1',
      'title_label' => t('Watch ID'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
