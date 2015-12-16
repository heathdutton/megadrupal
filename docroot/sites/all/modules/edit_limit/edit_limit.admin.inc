<?php
/**
 * @file edit_limit.admin.inc
 * include file containing admin-related functions, likethe system settings form.
 */

/**
 * A sform function to handle admin-level settings for the Edit Limit module
 */
function edit_limit_settings($form, $form_state) {
  $form = array();

  $form['enabled'] = array(
    '#type' => 'fieldset',
    '#title' => t('Enabled options'),
    '#collapsible' => FALSE,
  );
  $form['enabled']['edit_limit_node_time_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable node time limits'),
    '#description' => t('If enabled, nodes can only be edited within the given time frame.'),
    '#default_value' => variable_get('edit_limit_node_time_enabled', FALSE),
  );
  $form['enabled']['edit_limit_comment_time_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable comment time limits'),
    '#description' => t('If enabled, comments can only be edited within the given time frame.'),
    '#default_value' => variable_get('edit_limit_comment_time_enabled', FALSE),
  );
  $form['enabled']['edit_limit_node_count_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable node count limits'),
    '#description' => t('If enabled, nodes can only be edited a certain number of times.'),
    '#default_value' => variable_get('edit_limit_node_count_enabled', FALSE),
  );
  /* TODO: Create the functionality that this enables.
  $form['enabled']['edit_limit_comment_count_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable comment count limits'),
    '#description' => t('If enabled, comments can only be edited a certain number of times.'),
    '#default_value' => variable_get('edit_limit_comment_count_enabled', FALSE),
  );
  */

  // TODO: Make the time unit changeable. Currently seconds
  $form['node_limits'] = array(
    '#type' => 'fieldset',
    '#title' => t('Node limits'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $time_units = array();
  foreach (explode(',', EDIT_LIMIT_TIME_UNITS) as $unit) {
    $time_units[$unit] = $unit;
  }
  $form['node_limits']['edit_limit_node_time_unit'] = array(
    '#type' => 'select',
    '#title' => t('Node time unit'),
    '#options' => $time_units,
    '#default_value' => variable_get('edit_limit_node_time_unit', EDIT_LIMIT_TIME_UNIT_DEFAULT),
  );
  $form['node_limits']['edit_limit_node_time_default'] = array(
    '#type' => 'textfield',
    '#title' => t('Node time limit'),
    '#description' => t('The amount of time after a node has been saved that it can be edited (in unit selected above).'),
    '#size' => 4,
    '#default_value' => variable_get('edit_limit_node_time_default', EDIT_LIMIT_NODE_TIME_DEFAULT),
    '#maxlength' => 10,
  );
  $form['node_limits']['edit_limit_node_count_default'] = array(
    '#type' => 'textfield',
    '#title' => t('Edit count'),
    '#description' => t('The number of times a node can be edited, after the original submission.'),
    '#size' => 4,
    '#default_value' => variable_get('edit_limit_node_count_default', EDIT_LIMIT_NODE_COUNT_DEFAULT),
    '#maxlength' => 4,
  );
  module_load_include('module', 'node');
  $types = array();
  foreach (node_type_get_types() as $type_data) {
    $types[$type_data->type] = $type_data->name;
  }
  asort($types);
  $form['node_limits']['edit_limit_node_types'] = array(
    '#type' => 'select',
    '#title' => t('Content types'),
    '#description' => t('Select the content types to apply these node limits to. Only content types selected here will have any limits applied.'),
    '#multiple' => TRUE,
    '#options' => $types,
    '#default_value' => variable_get('edit_limit_node_types', array()),
  );

  // TODO: Make the time unit changeable. Currently seconds
  $form['comment_limits'] = array(
    '#type' => 'fieldset',
    '#title' => t('Comment limits'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['comment_limits']['edit_limit_comment_time_unit'] = array(
    '#type' => 'select',
    '#title' => t('Node time unit'),
    '#options' => $time_units,
    '#default_value' => variable_get('edit_limit_comment_time_unit', EDIT_LIMIT_TIME_UNIT_DEFAULT),
  );
  $form['comment_limits']['edit_limit_comment_time'] = array(
    '#type' => 'textfield',
    '#title' => t('Comment time limit'),
    '#description' => t('The amount of time after a comment has been saved that it can be edited (in unit selected above).'),
    '#size' => 4,
    '#default_value' => variable_get('edit_limit_comment_time', EDIT_LIMIT_COMMENT_TIME),
    '#maxlength' => 10,
  );
  $form['comment_limits']['edit_limit_comments'] = array(
    '#type' => 'select',
    '#title' => t('Content types'),
    '#description' => t('Select the content types to apply these comment limits to. Only comments related to the content types selected here will have any limits applied.'),
    '#multiple' => TRUE,
    '#options' => $types,
    '#default_value' => variable_get('edit_limit_comments', array()),
  );
  
  /* TODO: Need to add support for this option in edit_limit.module: edit_limit_link_alter().
   $form['comment_limits']['edit_limit_comment_count'] = array(
   '#type' => 'textfield',
   '#title' => t('Edit count'),
   '#description' => t('The number of times a comment can be edited, after the original submission.'),
   '#size' => 4,
   '#default_value' => variable_get('edit_limit_comment_count', EDIT_LIMIT_COMMENT_COUNT),
   '#maxlength' => 4,
   );
   */

  return system_settings_form($form);
}
