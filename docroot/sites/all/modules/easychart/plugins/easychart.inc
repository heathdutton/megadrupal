<?php

/**
 * @file
 * Provides the Easychart Wysiwyg plugin.
 */

/**
 * Implements hook_wysiwyg_plugin().
 */
function easychart_easychart_plugin() {

  // add necessary jQuery ui libs
  drupal_add_library('system', 'ui');
  drupal_add_library('system', 'ui.dialog');

  // Make path to module available to plugin.js
  drupal_add_js(array('easychart' => array('module_path' => drupal_get_path('module', 'easychart'))), 'setting');

  // Make all easychart configs available to plugin.js so that we can render a preview in the WYSIWYG.
  $query = db_select('node', 'n');
  $query->join('field_data_easychart', 'ec', 'ec.entity_id = n.nid');
  $query->fields('n', array('nid'));
  $query->fields('ec', array('easychart_config'));
  $query->condition('n.type', 'easychart');
  $query->condition('n.status', 1);

  $result = $query->execute();

  $existingCharts = array();
  while($record = $result->fetchAssoc()) {
    $existingCharts[$record['nid']] = $record['easychart_config'];
  }
  drupal_add_js(array('easychart' => array('existingCharts' => $existingCharts)), 'setting');

  $plugins['easychart'] = array(
    'title' => t('Easychart'),
    'vendor url' => 'http://drupal.org/project/easychart',
    'icon path' => drupal_get_path('module', 'easychart') . '/plugins/images',
    'icon file' => 'icon.png',
    'icon title' => t('Add a chart'),
    'js path' => drupal_get_path('module', 'easychart') . '/plugins/js',
    'js file' => 'plugin.js',
    'css path' => drupal_get_path('module', 'easychart') . '/plugins/css',
    'css file' => 'plugin.css',
    'settings' => array(),
  );
  return $plugins;
}
