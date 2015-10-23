<?php

/**
 * @file
 * Contains theme implementation.
 */

/**
 * Renders table to merge entities.
 */
function theme_entity_merge_ui_merge_table($vars) {
  $table = $vars['table'];
  $header_dirty = $table['entity_id'];
  unset($table['entity_id']);
  $header = array();
  foreach (element_children($header_dirty) as $key) {
    $header[$key]['data'] = $header_dirty[$key];
  }

  $selected_entities = $table['#selected'];
  unset($table['#selected']);
  $rows = array();
  foreach (element_children($table) as $id) {
    $row = array('data' => array());
    $row['data'][] = drupal_render($table[$id]['field_name']);
    foreach ($selected_entities as $cid) {
      $row['data'][] = drupal_render($table[$id][$cid]);
    }
    $rows[] = $row;
  }
  $output = theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array('id' => 'merge-entities-table'),
    'sticky' => FALSE,
  ));

  return $output;
}
