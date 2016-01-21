<?php

/**
 * @file
 * Administration page callbacks for Mailgun.
 */

/**
 * Menu callback: displays the Mailgun module settings page.
 *
 * @param array $form
 *   Form render array.
 * @param array $form_state
 *   Array containing form state values.
 *
 * @return array
 *   An array containing form items to place on the module settings page.
 */
function mailgun_admin_settings($form, &$form_state) {
  $library = libraries_detect('mailgun');

  if (!$library['installed']) {
    drupal_set_message(t('The Mailgun PHP library is not installed. Please see <a href="@url">documentation</a> for more information.', array('@url' => url('https://www.drupal.org/node/2547591'))), 'error');
  }

  $key = variable_get('mailgun_api_key', '');

  $form['mailgun_api_key'] = array(
    '#title' => t('Mailgun API key'),
    '#type' => 'textfield',
    '#description' => t('Get your Secret API key from the <a href="@url">Mailgun dashboard</a>.', array('@url' => url('https://mailgun.com/app/dashboard'))),
    '#default_value' => $key,
    '#required' => TRUE,
  );

  // Display settings only when a valid API key is present.
  if (!empty($key)) {
    $form['mailgun_queue'] = array(
      '#title' => t('Queue mails'),
      '#type' => 'checkbox',
      '#description' => t('Mails will be queued and sent during cron runs. Useful for sending a large number of emails.'),
      '#default_value' => variable_get('mailgun_queue', FALSE),
    );

    $form['mailgun_log'] = array(
      '#title' => t('Log mails'),
      '#type' => 'checkbox',
      '#description' => t('Log mails sent through Mailgun. Should not be enabled on production sites. Messages fail to send will be logged regardless of this setting.'),
      '#default_value' => variable_get('mailgun_log', FALSE),
    );

    $formats = array('_none' => t('- None -'));
    foreach (filter_formats() as $format) {
      if ($format->format == 'php_code') {
        continue;
      }
      $formats[$format->format] = t($format->name);
    }
    $form['mailgun_format'] = array(
      '#title' => t('Text format'),
      '#type' => 'select',
      '#description' => t('Specify an additional text format to filter the message through before sending the email.'),
      '#options' => $formats,
      '#default_value' => variable_get('mailgun_format', '_none'),
    );

    $form['defaults'] = array(
      '#type' => 'fieldset',
      '#title' => t('Default settings'),
      '#description' => t('These default settings apply to messages sent using Mailgun and may be overriden on a per-message basis.'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
    );

    $form['defaults']['mailgun_tracking'] = array(
      '#title' => t('Enable tracking'),
      '#type' => 'select',
      '#options' => array('default' => t('Use default setting'), 'enabled' => t('Enabled'), 'disabled' => t('Disabled')),
      '#description' => t('Whether to enable event tracking by default or not. See <a href="@url">Tracking Messages</a> for details.', array('@url' => url('https://documentation.mailgun.com/user_manual.html#tracking-messages'))),
      '#default_value' => variable_get('mailgun_tracking', 'default'),
    );

    $form['defaults']['mailgun_tracking_clicks'] = array(
      '#title' => t('Enable click tracking'),
      '#type' => 'select',
      '#options' => array('default' => t('Use default setting'), 'enabled' => t('Enabled'), 'disabled' => t('Disabled')),
      '#description' => t('Whether to enable click tracking by default or not.'),
      '#default_value' => variable_get('mailgun_tracking_clicks', 'default'),
    );

    $form['defaults']['mailgun_tracking_opens'] = array(
      '#title' => t('Enable open tracking'),
      '#type' => 'select',
      '#options' => array('default' => t('Use default setting'), 'enabled' => t('Enabled'), 'disabled' => t('Disabled')),
      '#description' => t('Whether to enable open tracking by default or not.'),
      '#default_value' => variable_get('mailgun_tracking_opens', 'default'),
    );
  }

  $form = system_settings_form($form);
  $form['#validate'][] = 'mailgun_admin_settings_validate';

  return $form;
}

/**
 * Form validation handler for mailgun_admin_settings().
 *
 * Perform additional validation to ensure the API key entered is valid.
 */
function mailgun_admin_settings_validate($form, &$form_state) {
  if ($form['mailgun_api_key']['#default_value'] != $form_state['values']['mailgun_api_key']) {
    // The API key has changed. Perform validation.
    $form_state['values']['mailgun_api_key'] = trim($form_state['values']['mailgun_api_key']);
    $client = mailgun_get_client($form_state['values']['mailgun_api_key']);
    try {
      $result = $client->get('domains');
      drupal_set_message(t('Your API key has been successfully validated.'));
    } catch (Exception $e) {
      form_set_error('mailgun_api_key', t('An exception occurred. @code: @message', array('@code' => $e->getCode(), '@message' => $e->getMessage())));
    }
  }
  $key = variable_get('mailgun_api_key', '');
}

/**
 * Form builder. Display a form for sending a test e-mail.
 */
function mailgun_test_form($form, &$form_state) {
  drupal_set_title(t('Send test mail'));

  $form['to'] = array(
    '#type' => 'textfield',
    '#title' => t('To'),
    '#default_value' => variable_get('site_mail', ''),
    '#description' => t('Type in an address to have the test email sent there.'),
    '#required' => TRUE,
  );

  $message = "Howdy!\n\nIf this e-mail is displayed correctly and delivered sound and safe, congrats! You have successfully configured Mailgun.";
  $message .= ' Visit the <a href="@project">project page</a> to contribute or read <a href="@documentation">documentation</a> to learn more.';
  $message = t($message, array('@project' => url('https://www.drupal.org/project/mailgun'), '@documentation' => url('https://www.drupal.org/node/2547591')));
  $form['message'] = array(
    '#type' => 'textarea',
    '#title' => t('Message'),
    '#default_value' => $message,
    '#required' => TRUE,
  );

  $form['attachment'] = array(
    '#title' => t('Include attachment'),
    '#type' => 'checkbox',
    '#description' => t('If checked, the Drupal icon will be included as an attachment with the test e-mail.'),
    '#default_value' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Send'),
  );
  $form['cancel'] = array(
    '#type' => 'link',
    '#href' => 'admin/config/system/mailgun',
    '#title' => t('Cancel'),
  );

  return $form;
}

/**
 * Form submission handler for mailgun_test_form().
 * Send the test e-mail.
 */
function mailgun_test_form_submit($form, &$form_state) {
  $to = $form_state['values']['to'];
  $params = array(
    'message' => $form_state['values']['message'],
    'attachment' => $form_state['values']['attachment'],
  );
  $site_name = variable_get('site_name', '');
  $default_from = variable_get('site_mail', ini_get('sendmail_from'));
  $from = (!empty($site_name)) ? $site_name . ' <' . $default_from . '>' : $default_from;
  $result = drupal_mail('mailgun', 'test', $to, $GLOBALS['language'], $params, $from);
  drupal_set_message(t('Test email sent from %from to %to. If you have the "Log mails" setting enabled, check the <a href="@url">database log</a> for details.', array('%from' => $result['from'], '%to' => $result['to'], '@url' => url('admin/reports/dblog'))), 'status');
}
