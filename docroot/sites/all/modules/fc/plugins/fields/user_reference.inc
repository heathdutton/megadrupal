<?php
/**
 * @file
 * Field Complete - Provides field-based completeness for any entity - field_collection plugin.
 */

$plugin = array(
  'title' => t('FC: User Reference plugin'),
  'completeness check' => 'fc_user_reference_field_is_complete',
  'incomplete process' => 'fc_user_reference_incomplete_process',
  'can be empty' => TRUE,

  'field form' => array(
    'fc_check_users' => array(
      '#type' => 'checkbox',
      '#title' => t('Check fields in referenced users for completeness'),
      '#description' => t('If checked, any users referenced by this field will also be checked for completeness.'),
    ),
  ),
);

function fc_user_reference_field_is_complete($items, $instance, $field, &$completed = array()) {
  $module = $field['module'];
  $check_users = !empty($instance['settings']['fc']['fc_check_users']);

  foreach ($items as $delta => $item) {
    if ($check_users) {
      // Loads each entity to find out whether it is complete
      $user = user_load($item['uid']);
      $completed[] = $user->fc->complete;
    }
    else {
      // Just do a superficial check of item
      $completed[] = !module_invoke($module, 'field_is_empty', $item, $field);
    }
  }
}

function fc_user_reference_incomplete_process($parent, $function, $cardinality, $items, $instance, $field) {
  $check_users = !empty($instance['settings']['fc']['fc_check_users']);
  $incomplete = new fcIncomplete($instance['field_name'], $parent, 'user');

  $completed = array();
  if ($check_users) {
    foreach ($items as $item) {
      // Loads each entity to find out whether it is complete
      $user = user_load($item['uid']);
      $sub = new fcIncomplete('', $parent, 'user', $user);
      $sub->process();
      $incomplete[] = $sub;
    }
  }

  $incomplete->complete = $cardinality($function, $items, $instance, $field);
  return $incomplete;
}
