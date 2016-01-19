<?php

/**
 * @file
 * MessageBird configuration page.
 */

/**
 * Page callback for the MessageBird configuration form.
 */
function messagebird_settings_form($form, $form_state) {
  $form['messagebird_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('API key'),
    '#required' => TRUE,
    '#default_value' => variable_get('messagebird_api_key', ''),
  );

  $form['messagebird_default_originator'] = array(
    '#type' => 'textfield',
    '#title' => t('Default originator'),
    '#required' => TRUE,
    '#maxlength' => 11,
    '#default_value' => variable_get('messagebird_default_originator', ''),
  );

  $form['messagebird_credit_balance_warning'] = array(
    '#type' => 'textfield',
    '#title' => t('Credit balance warning'),
    '#description' => t('Display a warning on the status report if the credit balance is below this amount. Leave empty to disable the warning.'),
    '#element_validate' => array('element_validate_integer_positive'),
    '#default_value' => variable_get('messagebird_credit_balance_warning', 100),
  );

  $form['messagebird_debug_mode'] = array(
    '#type' => 'checkbox',
    '#title' => t('Debug mode'),
    '#description' => t('Debug mode displays the message data every time a message is sent.'),
    '#default_value' => variable_get('messagebird_debug_mode', 0),
  );

  return system_settings_form($form);
}

/**
 * Page callback for the MessageBird API testing form.
 */
function messagebird_send_form($form, $form_state) {
  $form['originator'] = array(
    '#type' => 'textfield',
    '#title' => t('Originator'),
  );

  $form['recipient'] = array(
    '#type' => 'textfield',
    '#title' => t('Recipient'),
    '#required' => TRUE,
  );

  $form['body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Send message'),
  );

  return $form;
}

/**
 * Submit handler for the MessageBird API testing form.
 */
function messagebird_send_form_submit($form, &$form_state) {
  $message_sent = messagebird_send($form_state['values']['body'], $form_state['values']['recipient'], $form_state['values']['originator']);

  if ($message_sent) {
    drupal_set_message(t('The message was successfully sent.'));
  }
  else {
    drupal_set_message(t('An error occured while sending the message. Check the logs for details.'), 'error');
  }
}
