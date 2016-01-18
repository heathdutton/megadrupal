<?php

/**
 * @file
 * Providing extra functionality to the Party Activity UI via views
 */

/**
 * Implements hook_views_data().
 */
function party_activity_views_data_alter(&$data) {
  $data['party_activity']['link_party_activity'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a link to the activity'),
      'handler' => 'party_activity_handler_link_field',
    ),
  );
  $data['party_activity']['edit_party_activity'] = array(
    'field' => array(
      'title' => t('Edit Link'),
      'help' => t('Provide a link to the edit form for an activity'),
      'handler' => 'party_activity_handler_edit_link_field',
    ),
  );
  $data['party_activity']['delete_party_activity'] = array(
    'field' => array(
      'title' => t('Delete Link'),
      'help' => t('Provide a link to the delete form for an activity'),
      'handler' => 'party_activity_handler_delete_link_field',
    ),
  );

  $data['party_activity']['operations'] = array(
    'field' => array(
      'title' => t('Operations links'),
      'help' => t('Display all operations available for this model.'),
      'handler' => 'party_activity_handler_operations_field',
    ),
  );
}
