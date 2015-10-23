<?php

/**
 * Form builder; edit a user account or one of their profile categories.
 *
 * @ingroup forms
 * @see gentle_reminder_user_form_submit()
 */
function gentle_reminder_user_settings_form(array $form, &$form_state, $account) {
  $options = array();
  foreach (module_implements('gr_info') as $module) {
    $info = module_invoke($module, 'gr_info');
    $options[$module] = $info['name'];
  }

  $form['reminders'] = array(
    '#title' => t('Which reminders do you want to receive?'),
    '#type' => 'checkboxes',
    '#options' => $options,
    '#default_value' => _gentle_reminder_get_default_values($account),
  );

  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $account->uid
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save')
  );

  return $form;
}

/**
 * Submit function for the user account and profile editing form.
 */
function gentle_reminder_user_settings_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  foreach ($values['reminders'] as $module => $status) {
    db_merge('gentle_reminder_user_settings')
      ->key(array(
        'uid' => $values['uid'],
        'module' => $module,
      ))
      ->fields(array(
        'uid' => $values['uid'],
        'module' => $module,
        'status' => $status ? 1 : 0,
      ))
      ->execute();
  }
}

/**
 * Get user default values.
 *
 * @param $account
 *
 * @return array
 */
function _gentle_reminder_get_default_values($account) {
  $output = array();

  // Get user options.
  foreach (module_implements('gr_info') as $module) {
    $output[$module] = gentle_reminder_allowed_to_remind($account, $module);
  }

  // Delete disabled options.
  $output = array_filter($output);

  // Return keys.
  return array_keys($output);
}