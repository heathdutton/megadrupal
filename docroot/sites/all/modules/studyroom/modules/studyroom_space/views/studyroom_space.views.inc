<?php

/**
 * @file
 * Providing extra functionality for the studyroom_space UI via views.
 */

/**
 * Implements hook_views_data().
 */
function studyroom_space_views_data_alter(&$data) {
  $data['studyroom_space']['link_studyroom_space'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('A simple link to the studyroom space.'),
      'handler' => 'studyroom_space_handler_field_link',
    ),
  );
  $data['studyroom_space']['delete_studyroom_space'] = array(
    'field' => array(
      'title' => t('Delete link'),
      'help' => t('Provide a link to delete the studyroom space.'),
      'handler' => 'studyroom_space_handler_field_link_delete',
    ),
  );
  $data['studyroom_space']['edit_studyroom_space'] = array(
    'field' => array(
      'title' => t('Edit link'),
      'help' => t('Provide a link to the edit form for the studyroom space.'),
      'handler' => 'studyroom_space_handler_field_link_edit',
    ),
  );
  // This content of this field are decided based on the menu structure that
  // follows admin/structure/space/%space_id/op
  $data['studyroom_space']['operations'] = array(
    'field' => array(
      'title' => t('Operations'),
      'help' => t('Display all operations available for this studyroom_space.'),
      'handler' => 'studyroom_space_handler_field_operations',
    ),
  );

  // Expose the product type.
  $data['studyroom_space']['type'] = array(
    'title' => t('Location'),
    'help' => t('The human-readable name of the location.'),
    'field' => array(
      'handler' => 'studyroom_space_handler_field_type',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'studyroom_space_handler_filter_type',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'studyroom_space_location',
      'field' => 'type',
      'label' => t('Location', array(), array('context' => 'a location')),
    ),
  );
  $data['studyroom_space']['min_duration'] = array(
    'field' => array(
      'title' => t('Minimum duration'),
      'help' => t('The minimum length of time (in seconds) of a reservation for this space.'),
      'handler' => 'views_handler_field_time_interval',
    ),
  );
  $data['studyroom_space']['max_duration'] = array(
    'field' => array(
      'title' => t('Maximum duration'),
      'help' => t('The maximum length of time (in seconds) of a reservation for this space.'),
      'handler' => 'views_handler_field_time_interval',
    ),
  );
  $data['studyroom_space']['max_days_advance'] = array(
    'field' => array(
      'title' => t('Maximum Days in Advance'),
      'help' => t('The maximum number of days in advance to allow a reservation for this space.'),
      'handler' => 'studyroom_space_handler_field_max_days_advance',
    ),
  );
}
