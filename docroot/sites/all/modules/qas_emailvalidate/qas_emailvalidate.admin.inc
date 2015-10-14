<?php

/**
 * Builds the administration settings form.
 */
function qas_emailvalidate_admin_settings_form($form, $form_state) {
  $form['qas_emailvalidate_apikey'] = array(
    '#type' => 'textfield',
    '#title' => t('Token'),
    '#required' => TRUE,
    '#default_value' => variable_get('qas_emailvalidate_apikey'),
  );

  $form['qas_emailvalidate_halt'] = array(
    '#type' => 'select',
    '#title' => t('If service unavailable'),
    '#options' => array(
      0 => t('Don\'t halt. Allow form submission.'),
      1 => t('Halt. Prohibit form submission.'),
      2 => t('Validate using Drupal\'s built-in email validation (recommended)'),
    ),
    '#description' => t('In most cases, it would be wiser - if the service is unavailable - to allow Drupal handle the validation.'),
    '#default_value' => variable_get('qas_emailvalidate_halt', 2),
  );

  $form['qas_emailvalidate_user_registration_form'] = array(
    '#type' => 'checkbox',
    '#title' => t('Validate email in user registration form.'),
    '#default_value' => variable_get('qas_emailvalidate_user_registration_form', 1),
  );

  if (module_exists('contact')) {
    $form['qas_emailvalidate_user_contact_form'] = array(
      '#type' => 'checkbox',
      '#title' => t('Validate email in user contact form.'),
      '#default_value' => variable_get('qas_emailvalidate_user_contact_form', 1),
    );

    $form['qas_emailvalidate_site_contact_form'] = array(
      '#type' => 'checkbox',
      '#title' => t('Validate email in site contact form.'),
      '#default_value' => variable_get('qas_emailvalidate_site_contact_form', 1),
    );
  }

  return system_settings_form($form);
}
