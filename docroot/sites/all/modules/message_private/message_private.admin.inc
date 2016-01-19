<?php
/**
 * Message Private admin config form.
 */

/**
 * Get the admin settings form for message_private.
 *
 * @return mixed
 *   Form array returned from system_settings_form().
 */
function message_private_admin_settings() {

  // Global email notifications on/off checkbox.
  $form[MESSAGE_PRIVATE_EMAIL_NOTIFICATIONS] = array(
    '#type' => 'checkbox',
    '#title' => t('Message Private Email Notifications'),
    '#default_value' => variable_get(MESSAGE_PRIVATE_EMAIL_NOTIFICATIONS, TRUE),
    '#description' => t('Global On / Off checkbox for emails notifying users of a new private message'),
  );

  // Role based message create limit on/off checkbox.
  $form[MESSAGE_PRIVATE_MESSAGE_LIMIT] = array(
    '#type' => 'checkbox',
    '#title' => t('Limit Message Create By Role'),
    '#default_value' => variable_get(MESSAGE_PRIVATE_MESSAGE_LIMIT, FALSE),
    '#description' => t('Impose a message creation limit per interval. Users with multiple roles, get the highest limit from these roles'),
  );

  // Conditional fieldset for all message limitation settings.
  $form['interval_limit'] = array(
    '#title' => t('Message interval limits'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#states' => array(
      'invisible' => array(
        ':input[name="' . MESSAGE_PRIVATE_MESSAGE_LIMIT . '"]' => array('checked' => FALSE),
      ),
    ),
  );

  // Add a default fieldset.
  $form['interval_limit'][MESSAGE_PRIVATE_DEFAULT_INDEX] = array(
    '#type' => 'fieldset',
    '#title' => 'Default limit',
    '#description' => t('Applies to all roles with blank entries below'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['interval_limit'][MESSAGE_PRIVATE_DEFAULT_INDEX][MESSAGE_PRIVATE_DEFAULT_LIMIT] = array(
    '#type' => 'textfield',
    '#title' => t('Limit'),
    '#default_value' => variable_get(MESSAGE_PRIVATE_DEFAULT_LIMIT),
    '#description' => t('Enter a message limit') . ' ' . MESSAGE_PRIVATE_MESSAGE_LIMIT_MIN . ' - ' . MESSAGE_PRIVATE_MESSAGE_LIMIT_MAX,
  );

  $form['interval_limit'][MESSAGE_PRIVATE_DEFAULT_INDEX][MESSAGE_PRIVATE_DEFAULT_INTERVAL] = array(
    '#type' => 'textfield',
    '#title' => t('Interval'),
    '#default_value' => variable_get(MESSAGE_PRIVATE_DEFAULT_INTERVAL),
    '#description' => t('Enter an interval in minutes') . ' ' . MESSAGE_PRIVATE_MESSAGE_INTERVAL_MIN . ' - ' . MESSAGE_PRIVATE_MESSAGE_INTERVAL_MAX,
  );

  // Generate variable names for all roles used with get/set in admin form.
  foreach (user_roles() as $id => $role) {
    $role_name = str_replace(' ', '_', $role);
    $limit_name = 'message_private_' . $role_name . '_limit';
    $interval_name = 'message_private_' . $role_name . '_interval';

    $form['interval_limit'][$id] = array(
      '#type' => 'fieldset',
      '#title' => $role,
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['interval_limit'][$id][$limit_name] = array(
      '#type' => 'textfield',
      '#title' => t('Limit'),
      '#default_value' => variable_get($limit_name),
      '#description' => t('Enter a message limit') . ' ' . MESSAGE_PRIVATE_MESSAGE_LIMIT_MIN . ' - ' . MESSAGE_PRIVATE_MESSAGE_LIMIT_MAX,
    );
    $form['interval_limit'][$id][$interval_name] = array(
      '#type' => 'textfield',
      '#title' => t('Interval'),
      '#default_value' => variable_get($interval_name),
      '#description' => t('Enter an interval in minutes') . ' ' . MESSAGE_PRIVATE_MESSAGE_INTERVAL_MIN . ' - ' . MESSAGE_PRIVATE_MESSAGE_INTERVAL_MAX,
    );
  }

  return system_settings_form($form);
}

/**
 * Validate the admin settings form.
 *
 * @param mixed $form
 *   The admin form array from message_private_admin_settings.
 * @param mixed $form_state
 *   The current state of the admin form array.
 */
function message_private_admin_settings_validate($form, &$form_state) {

  // Validate the default fieldset values.
  _message_private_validate_fieldset(
    $form['interval_limit'][MESSAGE_PRIVATE_DEFAULT_INDEX][MESSAGE_PRIVATE_DEFAULT_LIMIT],
    $form['interval_limit'][MESSAGE_PRIVATE_DEFAULT_INDEX][MESSAGE_PRIVATE_DEFAULT_INTERVAL],
    $form_state['values'][MESSAGE_PRIVATE_DEFAULT_LIMIT],
    $form_state['values'][MESSAGE_PRIVATE_DEFAULT_INTERVAL]
  );

  // Cycle through the settings for each role and validate.
  foreach (user_roles() as $id => $role) {
    $role_name = str_replace(' ', '_', $role);
    $limit_name = 'message_private_' . $role_name . '_limit';
    $interval_name = 'message_private_' . $role_name . '_interval';

    _message_private_validate_fieldset(
      $form['interval_limit'][$id][$limit_name],
      $form['interval_limit'][$id][$interval_name],
      $form_state['values'][$limit_name],
      $form_state['values'][$interval_name]
    );
  }
}

/**
 * Validate limit and interval values and show any errors on the form elements.
 *
 * @param mixed $limit_element
 *   Limit form element reference.
 * @param mixed $interval_element
 *   Interval form element reference.
 * @param string|int $limit
 *   Limit value to validate.
 * @param string|int $interval
 *   Interval value to validate.
 */
function _message_private_validate_fieldset(&$limit_element, &$interval_element, $limit, $interval) {
  // Validate role settings, check both textfields per fieldset are set.
  if (!empty($limit) && !empty($interval)) {
    // Check is numeric and between the boundaries.
    if (!ctype_digit($limit) || $limit > MESSAGE_PRIVATE_MESSAGE_LIMIT_MAX || $limit < MESSAGE_PRIVATE_MESSAGE_LIMIT_MIN) {
      form_error($limit_element,
        t('Enter a numerical message limit between') . ' '
          . MESSAGE_PRIVATE_MESSAGE_LIMIT_MIN . ' - ' . MESSAGE_PRIVATE_MESSAGE_LIMIT_MAX . '.');
    }
    // Check is numeric and between the boundaries.
    if (!ctype_digit($interval) || $interval > MESSAGE_PRIVATE_MESSAGE_INTERVAL_MAX || $interval < MESSAGE_PRIVATE_MESSAGE_INTERVAL_MIN) {
      form_error($interval_element,
        t('Enter a numerical interval in minutes between') . ' '
          . MESSAGE_PRIVATE_MESSAGE_INTERVAL_MIN . ' - ' . MESSAGE_PRIVATE_MESSAGE_INTERVAL_MAX . '.');
    }
  }
  elseif (!empty($limit) || !empty($interval)) {
    // Show error if only 1 textfield is set in each fieldset.
    form_error($limit_element, t('Both a limit and interval value are required.'));
    form_error($interval_element);
  }
}
