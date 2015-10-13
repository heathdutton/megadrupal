<?php
/**
 * @file
 * Notifier_scc configuration.
 */

/**
 * Settings form.
 */
function notifier_scc_admin_form($form, &$form_state) {
  $form['notifier_scc'] = array(
    '#type' => 'fieldset',
    '#title' => t('Email'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  $form['notifier_scc']['notifier_scc_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email'),
    '#description' => t('Enter an email to be used when the primary feed can not be reached.'),
    '#default_value' => variable_get('notifier_scc_email', ''),
  );

  $form['notifier_scc']['notifier_scc_email_time'] = array(
    '#type' => 'textfield',
    '#title' => t('Time between emails'),
    '#description' => t('How many seconds should there be between emails when the primary feed is down.'),
    '#default_value' => variable_get('notifier_scc_email_time', 3600),
  );

  return system_settings_form($form);
}

/**
 * Settings form Validation.
 */
function notifier_scc_admin_form_validate($form, &$form_state) {
  $values = $form_state['values'];

  if (!valid_email_address($values['notifier_scc_email'])) {
    form_set_error('notifier_scc_email', t('Please enter a valid email address.'));
  }
}
