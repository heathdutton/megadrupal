<?php

/**
 * @file
 * Adds operation links as a views field.
 */

/**
 * Implements hook_views_data_alter().
 */
function elearning_ui_views_data_alter(&$data) {

  foreach (elearning_get_elearning_entity_info() as $entity_type => $entity_info) {
    $data[$entity_type]['operations'] = array(
      'field' => array(
        'title' => t('Operations links'),
        'help' => t('Display all operations available for this entity.'),
        'handler' => 'elearning_ui_handler_entity_operations_field',
      ),
    );
  }
  return $data;
}
