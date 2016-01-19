<?php

/**
 * @file
 * Providing extra functionality for the Model UI via views.
 */


/**
 * Implements hook_views_data().
 */
function studyroom_closed_views_data_alter(&$data) {
  $data['studyroom_closed']['edit_studyroom_closed'] = array(
    'field' => array(
      'title' => t('Edit link'),
      'help' => t('Provide a link to the edit form for the studyroom_closed.'),
      'handler' => 'studyroom_closed_handler_edit_link_field',
    ),
  );
  $data['studyroom_closed']['delete_studyroom_closed'] = array(
    'field' => array(
      'title' => t('Delete link'),
      'help' => t('Provide a link to delete the studyroom_closed.'),
      'handler' => 'studyroom_closed_handler_delete_link_field',
    ),
  );
  $data['studyroom_closed']['operations'] = array(
    'field' => array(
      'title' => t('Operations'),
      'help' => t('Display all operations available for this studyroom_closed.'),
      'handler' => 'studyroom_closed_handler_operations_field',
    ),
  );
  $data['studyroom_closed']['spaces'] = array(
    'field' => array(
      'title' => t('Spaces'),
      'help' => t('Provide list of spaces.'),
      'handler' => 'studyroom_closed_handler_spaces_field',
    ),
  );
  $data['studyroom_closed']['conflict_studyroom_closed'] = array(
    'field' => array(
      'title' => t('Conflicts'),
      'help' => t('List if any conflict exist for this reservation.'),
      'handler' => 'studyroom_closed_handler_conflict_field',
    ),
  );
}
