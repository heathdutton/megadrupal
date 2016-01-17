<?php

/**
 * Provides a form to send a test email through Sendgrid.
 */
function sendgrid_integration_test() {
  $form = array();

  $defaults = variable_get(
    'sendgrid_integration_test_defaults',
    array(
      'to' => variable_get('site_mail', 'user@example.com'),
      'subject' => 'Test Email from SendGrid Module',
      'body' => array(
        'value' => 'Test Message for SendGrid.',
      ),
    )
  );

  $defaults['body']['format'] = filter_fallback_format();
  $form['to'] = array(
    '#type' => 'textfield',
    '#title' => t('To'),
    '#default_value' => $defaults['to'],
    '#maxlength' => 128,
    '#required' => TRUE,
  );
  $form['subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => $defaults['subject'],
    '#maxlength' => 128,
    '#required' => TRUE,
  );
  $form['body'] = array(
    '#type' => 'text_format',
    '#title' => t('Body'),
    '#rows' => 20,
    '#default_value' => $defaults['body']['value'],
    '#format' => $defaults['body']['format'],
    '#required' => TRUE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Send test message'),
  );
  return $form;
}

/**
 * Implements hook_submit().
 */
function sendgrid_integration_test_submit($form, &$form_state) {
  // Create defaults array.
  $defaults = array(
    'to' => $form_state['values']['to'],
    'subject' => $form_state['values']['subject'],
    'body' => $form_state['values']['body'],
  );
  // Set the defaults for reuse.
  variable_set('sendgrid_integration_test_defaults', $defaults);

  $params = array(
    'subject' => $form_state['values']['subject'],
    'body' => check_markup(
      $form_state['values']['body']['value'],
      $form_state['values']['body']['format']
    ),
  );
  // Setting a specific mail system for the SendGrid Integration Module.
  mailsystem_set(array('sendgrid_integration' => 'SendGridMailSystem'));
  // Attempt to send the email and post a message if it was successful.
  $result = drupal_mail('sendgrid_integration', 'test', $form_state['values']['to'], language_default(), $params);
  if (isset($result['result']) && $result['result'] == 'true') {
    drupal_set_message(t('SendGrid test email sent from %from to %to.', array(
      '%from' => $result['from'],
      '%to' => $result['to']
    )), 'status');
  }
}
