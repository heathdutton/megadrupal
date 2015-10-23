<?php
/**
 * @file
 * Email Field Confirm module hook implementations
 */


/**
 * Implements hook_form_FORM_ID_alter().
 *
 * Adds the email field confirm settings to the email field instance.
 * Settings include:
 * - confirm: checkbox to enable confirming the email address(es).
 * - save_value: save new value with entity or keep original until verified.
 * - notify_original: notify the original email address.
 * - confirming_user: choose which user is the confirming user. Options include
 *     acting user or entity owner.
 */
function email_field_confirm_form_field_ui_field_edit_form_alter(&$form, &$form_state) {
  // Only worry about 'email' fields.
  if ($form['#field']['type'] != 'email') {
    return;
  }

  $instance = $form['#instance'];

  // If the field allows for multiple values then we force the new emails to be
  // saved to the field. This is to due to not being able to accurately track
  // what an original value may have been and replace when a new value is
  // confirmed.
  $single_value_field = $form['#field']['cardinality'] == 1;
  if ($single_value_field) {
    $save_value_default = isset($instance['settings']['email_field_confirm']['save_value']) ? $instance['settings']['email_field_confirm']['save_value'] : 0;
    $notify_original_default = isset($instance['settings']['email_field_confirm']['notify_original']) ? $instance['settings']['email_field_confirm']['notify_original'] : 0;
  }
  else {
    $save_value_default = 1;
    $notify_original_default = 0;
  }

  $form['instance']['settings']['email_field_confirm'] = array(
    '#type' => 'fieldset',
    '#title' => t('Email Field Confirm Settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['instance']['settings']['email_field_confirm']['confirm'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send email to confirm this address?'),
    '#default_value' => isset($instance['settings']['email_field_confirm']['confirm']) ? $instance['settings']['email_field_confirm']['confirm'] : 0,
  );
  $form['instance']['settings']['email_field_confirm']['save_value'] = array(
    '#type' => 'checkbox',
    '#title' => t('Save the new email address?'),
    '#description' => t('Uncheck this option to retain the original value until the email address is confirmed.<br/><strong>Note:</strong> multi-value fields always save the new value.'),
    '#default_value' => $save_value_default,
    '#disabled' => !$single_value_field,
    '#states' => array(
      'visible' => array(
        ':input[name="instance[settings][email_field_confirm][confirm]"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['instance']['settings']['email_field_confirm']['notify_original'] = array(
    '#type' => 'checkbox',
    '#title' => t('Notify original email address (if applicable)?'),
    '#description' => t('If an email address is changing, send a notification email to the original email address.<br/><strong>Note:</strong> not accurately able to determine original email for multi-value fields.'),
    '#default_value' => $notify_original_default,
    '#disabled' => !$single_value_field,
    '#states' => array(
      'visible' => array(
        ':input[name="instance[settings][email_field_confirm][confirm]"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['instance']['settings']['email_field_confirm']['confirming_user'] = array(
    '#type' => 'radios',
    '#title' => t('Who may confirm the email addresses?'),
    '#options' => array(
      'acting_user' => t('Acting User'),
      'entity_owner' => t('Entity Owner'),
    ),
    '#default_value' => isset($instance['settings']['email_field_confirm']['confirming_user']) ? $instance['settings']['email_field_confirm']['confirming_user'] : 'acting_user',
    '#states' => array(
      'visible' => array(
        ':input[name="instance[settings][email_field_confirm][confirm]"]' => array('checked' => TRUE),
      ),
    ),
  );
  // Add a validate handler to make sure some settings are configured based on
  // the cardinality of the field.
  $form['#validate'][] = 'email_field_confirm_widget_settings_validate';
  // Add a submit handler to clean up any pending emails if the field does not
  // require confirmation.
  $form['#submit'][] = 'email_field_confirm_widget_settings_submit';
}

/**
 * Field UI Settings Validate handler.
 *
 * If a field allows for multiple values then some of the email field confirm
 * settings must be adjusted.
 *
 * @see email_field_confirm_form_field_ui_field_edit_form_alter
 */
function email_field_confirm_widget_settings_validate($form, &$form_state) {
  $instance = $form_state['values']['instance'];
  // If a multi-value field then we possibly need to override some settings.
  if ($form_state['values']['field']['cardinality'] != '1') {
    $efc_settings = $instance['settings']['email_field_confirm'];

    // Save new value
    if (!$efc_settings['save_value']) {
      form_error($form['instance']['settings']['email_field_confirm'], 'This is a multi-value field. Must configure the email field confirm settings to save the new value.');
    }

    // Notify original email
    if ($efc_settings['notify_original']) {
      form_error($form['instance']['settings']['email_field_confirm'], 'This is a multi-value field. Unable to accurately maintain and notify the original email address when changed.');
    }
  }
}

/**
 * Field UI Settings Submit handler.
 *
 * If a field does not require confirmation then remove any pending emails.
 *
 * @see email_field_confirm_form_field_ui_field_edit_form_alter
 */
function email_field_confirm_widget_settings_submit($form, &$form_state) {
  $instance = $form_state['values']['instance'];
  // If we are not confirming emails in this field then clean up any pending
  // emails.
  if (empty($instance['settings']) || empty($instance['settings']['email_field_confirm']) || !$instance['settings']['email_field_confirm']['confirm']) {
    email_field_confirm_delete_pending_by_field($instance['entity_type'], $instance['bundle'], $instance['field_name']);
  }
}

/**
 * Implements hook_field_delete_instance().
 */
function email_field_confirm_field_delete_instance($instance) {
  email_field_confirm_delete_pending_by_field($instance['entity_type'], $instance['bundle'], $instance['field_name']);
}

/**
 * Implements hook_field_purge_instance().
 */
function email_field_confirm_field_purge_instance($instance) {
  email_field_confirm_delete_pending_by_field($instance['entity_type'], $instance['bundle'], $instance['field_name']);
}

/**
 * Delete all pending emails for a given entity / bundle / field.
 *
 * @param  string $entity_type
 *   The entity type to filter on.
 * @param  string $bundle
 *   The bundle to filter on.
 * @param  string $field_name
 *   The field_name to filter on.
 */
function email_field_confirm_delete_pending_by_field($entity_type, $bundle, $field_name) {
  // Build the conditions to determine which records should be deleted.
  $deleted = db_delete('email_field_confirm')
    ->condition('entity_type', $entity_type)
    ->condition('bundle', $bundle)
    ->condition('field_name', $field_name)
    ->condition('status', EMAIL_FIELD_CONFIRM_AWAITING_CONFIRMATION);
  $deleted_count = $deleted->execute();

  // Perform any messaging / logging.
  if ($deleted_count) {
    $message = t('!count pending emails removed. (!entity_type / !bundle / !field_name)', array('!count' => $deleted_count, '!entity_type' => $entity_type, '!bundle' => $bundle, '!field_name' => $field_name));
    if (user_access('view any pending email field addresses')) {
      drupal_set_message($message);
    }
    watchdog('email_field_confirm', $message);
  }
}
