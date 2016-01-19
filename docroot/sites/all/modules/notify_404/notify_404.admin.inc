<?php
/**
 * @file
 * Administrative forms for Notify 404 Module.
 */

/**
 * Form for administering the email content.
 *
 * @param array $form
 *   The form array
 *
 * @param array $form_state
 *   The form_state array
 *
 * @return form
 *   Returns the form array
 */
function notify_404_email_form($form, &$form_state) {
  $form['#tree'] = TRUE;
  $notify_404 = variable_get('notify_404');

  $form['email_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Email Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['email_settings']['to'] = array(
    '#type' => 'textfield',
    '#default_value' => (isset($notify_404['to']) ? $notify_404['to'] : variable_get('site_mail', 'admin@example.com')),
    '#title' => t('To'),
    '#maxlength' => 100,
    '#description' => t('If no "To" email is set, the system email
                         address will be used.'),
  );

  $form['email_settings']['subject'] = array(
    '#type' => 'textfield',
    '#default_value' => (isset($notify_404['subject']) ? $notify_404['subject'] : notify_404_mail_text('notify_404_subject')),
    '#title' => t('Subject'),
    '#maxlength' => 255,
  );

  $form['email_settings']['message'] = array(
    '#type' => 'textarea',
    '#default_value' => (isset($notify_404['message']) ? $notify_404['message'] : notify_404_mail_text('notify_404_message')),
    '#title' => t('Message'),
    '#rows' => 8,
  );

  $form['email_settings']['token_set'] = array(
    '#type' => 'fieldset',
    '#title' => t('Available Tokens'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['email_settings']['token_set']['tokens'] = array(
    '#theme' => 'token_tree',
    '#token_types' => array('notify_404'),
    '#global_types' => TRUE,
    '#click_insert' => TRUE,
  );

  $form['notify_404_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Notify 404 Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $volumes = drupal_map_assoc(array('2', '10', '30', '75', '100'));
  $form['notify_404_settings']['volume'] = array(
    '#type' => 'select',
    '#default_value' => (isset($notify_404['volume']) ? $notify_404['volume'] : NOTIFY_404_VOLUME),
    '#title' => t('Volume'),
    '#options' => $volumes,
    '#description' => t('The volume at which the 404s occur.'),
  );

  $frequencies = array(
    2 => '2 minutes',
    10 => '10 minutes',
    30 => '30 minutes',
    75 => '75 minutes',
    100 => '100 minutes',
  );
  $form['notify_404_settings']['frequency'] = array(
    '#type' => 'select',
    '#default_value' => (isset($notify_404['frequency']) ? $notify_404['frequency'] : NOTIFY_404_FREQUENCY),
    '#title' => t('Frequency'),
    '#options' => $frequencies,
    '#description' => t('The frequency to check if 404s have occurred.
                         Cron must be running in order for the check to take
                         place. If cron is running every hour but, frequency is
                         set to every 10 minutes, the frequency check will only
                         occur every hour.'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Configuration'),
  );

  return $form;
}

/**
 * Validates the form values.
 *
 * Validates if the user has entered a valid subject / message for the emails.
 *
 * @param array $form
 *   The form array
 *
 * @param array $form_state
 *   The form_state array
 */
function notify_404_email_form_validate($form, &$form_state) {
  // Determine if subject is set with no message.
  if ($form_state['values']['email_settings']['subject'] != NULL && $form_state['values']['email_settings']['message'] == NULL) {
    form_set_error("notification_email][message", 'You must add an email message if a subject is provided');
  }
  // Determine if message is set with no subject.
  if ($form_state['values']['email_settings']['subject'] == NULL && $form_state['values']['email_settings']['message'] != NULL) {
    form_set_error("notification_email][subject", 'You must add a subject if an email message is provided');
  }
}

/**
 * Submit function saves notify_404 settings.
 *
 * @param array $form
 *   The form array
 *
 * @param array $form_state
 *   The form_state array
 */
function notify_404_email_form_submit($form, &$form_state) {
  variable_set('notify_404',
    array(
      'to' => $form_state['values']['email_settings']['to'],
      'subject' => $form_state['values']['email_settings']['subject'],
      'message' => $form_state['values']['email_settings']['message'],
      'frequency' => $form_state['values']['notify_404_settings']['frequency'],
      'volume' => $form_state['values']['notify_404_settings']['volume'],
    )
  );

  drupal_set_message('Notify 404 settings have been updated.');
}
