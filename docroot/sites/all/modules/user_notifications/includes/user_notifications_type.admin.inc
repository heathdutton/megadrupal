<?php

/**
 * @file
 * Model type editing UI.
 */

/**
 * Generates the model type editing form.
 */
function user_notifications_type_form($form, &$form_state, $user_notifications_type, $op = 'edit') {
  if ($op == 'clone') {
    $user_notifications_type->label .= ' (cloned)';
    $user_notifications_type->name = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $user_notifications_type->label,
    '#description' => t('The human-readable name of this model type.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  // Machine-readable type name.
  $form['name'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($user_nofitications_type->name) ? $user_notifications_type->name : '',
    '#maxlength' => 32,
    '#disabled' => $user_notifications_type->locked && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'user_notifications_get_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this user notification type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save user notification type'),
    '#weight' => 40,
  );

  if (!$user_notifications_type->locked && $op != 'add') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete user notification type'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('user_notifications_type_form_submit_delete'),
    );
  }

  return $form;
}

/**
 * Form API submit callback for the type form.
 */
function user_notifications_type_form_submit(&$form, &$form_state) {
  $user_notifications_type = entity_ui_form_submit_build_entity($form, $form_state);
  $user_notifications_type->save();
  $form_state['redirect'] = 'admin/structure/user-notifications/user-notifications-types';
}

/**
 * Form API submit callback for the delete button.
 */
function user_notifications_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/user-notifications/user-notifications-types/manage/' . $form_state['user_notifications_type']->name . '/delete';
}

/**
 * return the system settings form
 */
function user_notifications_settings() {
  $form = array();

  $form['flush_deleted'] = array(
    '#type'        => 'fieldset',
    '#collapsible' => FALSE,
    '#collapsed'   => FALSE,
    '#title'       => t('Flush deleted messages'),
    '#description' => t('You can optionally use the cron run to flush out old notifications. Notifications are permanently deleted so use with caution.'),
  );

  $form['flush_deleted']['user_notifications_flush_enabled'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Flush old notifications'),
    '#default_value' => variable_get('user_notifications_flush_enabled', FALSE),
    '#description'   => t('Enable the flushing of old notifications. Requires that cron is enabled'),
  );

  $form['flush_deleted']['user_notifications_flush_days'] = array(
    '#type' => 'select',
    '#title' => t('Flush notifications older than this many days'),
    '#default_value' => variable_get('user_notifications_flush_days', 30),
    '#options' => drupal_map_assoc(array(0, 1, 2, 5, 10, 30, 100)),
  );

  $form['flush_deleted']['user_notifications_flush_max'] = array(
    '#type' => 'select',
    '#title' => t('Maximum number of notifications to flush per cron run'),
    '#default_value' => variable_get('user_notifications_flush_max', 200),
    '#options' => drupal_map_assoc(array(50, 100, 200, 500, 1000)),
  );

  return system_settings_form($form);
}
