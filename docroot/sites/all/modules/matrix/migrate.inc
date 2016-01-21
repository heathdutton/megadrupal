<?php
// $Id$

/**
 * @file
 * Hook implementations to assist with the migration from Drupal 6 to 7
 */

/**
 * Implementation of hook_content_migrate_field_alter()
 */
function matrix_content_migrate_field_alter(&$field_value, $instance_value) {
  switch ($instance_value['widget']['module']) {
    case 'matrix':
      $field_value['module'] = 'matrix';
      $field_value['type'] = 'matrix_text';
      break;
  }
}

/**
 * Implementation of hook_content_migrate_data_record_alter().
 *
 * The data in matrix has a one-to-many relationship from node to data
 * This hook does the inserts rather than content_migrate which assumes a one-to-one relationship
 */
function matrix_content_migrate_data_record_alter(&$record, $field) {
  switch($field['type']) {
    case 'matrix_text':
    case 'matrix_custom':
      $new_table = content_migrate_new_table($field);
      $new_revision_table = content_migrate_new_revision($field);
      $result = db_query("SELECT * FROM {node_field_matrix_data} WHERE nid = :nid AND vid = :vid AND field_name = :field_name", array(':nid' => $record['entity_id'], ':vid' => $record['revision_id'], ':field_name' => $field['field_name']));
      foreach ($result as $row) {
        $record[$field['field_name'] . '_row'] = $row->row + 1;
        $record[$field['field_name'] . '_col'] = $row->col + 1;
        $record[$field['field_name'] . '_value'] = $row->value;
        if (!empty($record)) {
          if ($record['revision_id'] == $row->vid) {
            drupal_write_record($new_table, $record);
          }
          drupal_write_record($new_revision_table, $record);
        }
        $record['delta']++;
      }
      $record = array(); //prevent content_migrate from processing anything
      break;
  }
}
