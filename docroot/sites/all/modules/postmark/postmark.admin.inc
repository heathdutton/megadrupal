<?php
/**
 * @file
 *
 * Administration include for Postmark.
 */

/**
 * Setting form for Postmark
 */
function postmark_settings_form() {
  // The user's Postmark API key
  $form['postmark_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Postmark API key'),
    '#default_value' => variable_get('postmark_api_key', ''),
    '#description' => t('You Postmark API key similar to : ed742D75-5a45-49b6-a0a1-5b9ec3dc9e5d (taken from the API page)'),
    '#required' => TRUE,
  );

  // Debug settings fieldset
  $form['debug'] = array(
    '#type' => 'fieldset',
    '#title' => t('Debugging'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  
  $form['debug']['postmark_debug_mode'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Postmark debugging'),
    '#default_value' => variable_get('postmark_debug_mode', 0),
    '#description' => t('Use Postmark debugging, just to display what is contained in the Drupal $message variable (this will be extended to include more debugging options)'),
  );
  
  $form['debug']['postmark_debug_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Enable Postmark debugging email'),
    '#default_value' => variable_get('postmark_debug_email', variable_get('site_mail', '')),
    '#description' => t('Use a debugging email, so all system emails will go to this address. Debugging mode must be on for this to work'),
  );
  
  $form['debug']['postmark_debug_no_send'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Postmark debugging to not send an email and therefore not use a credit'),
    '#default_value' => variable_get('postmark_debug_no_send', 0),
    '#description' => t('Disable credit usage when debugging'),
  );
 
  $form['test'] = array(
    '#type' => 'fieldset',
    '#title' => t('Test configuration'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  
  $form['test']['test_address'] = array(
    '#type' => 'textfield',
    '#title' => t('Recipient'),
    '#default_value' => '',
    '#description' => t('Enter a valid email address to send a test email.'),
  );
  
  // Add our own submit to the form
  $form['#submit'][] = 'postmark_settings_form_submit';
  
  return system_settings_form($form);
}

/**
 * Submit callback; send a test email and show message about smtp status
 */
function postmark_settings_form_submit($form, &$form_state) {  
  // If an address is submitted, send a test email message.
  $test_address = $form_state['values']['test_address'];
  if ($test_address != '') {
    drupal_set_message(t('A test e-mail has been sent to %email.  You may want to <a href="!watchdog">check the logs</a> for any error messages.', array('%email' => $test_address, '!watchdog' => url('admin/reports/dblog'))), 'warning');
    drupal_mail('postmark', 'test', $test_address, NULL);
    unset($form_state['values']['test_address']);
  }
}

/**
 * Implementation of hook_mail().
 */
function postmark_mail($key, &$message, $params) {
  $message['subject'] = t('Postmark Test Run Email');
  $message['body'][] = t('Your site is properly configured to send emails using the Postmark library.');
}
