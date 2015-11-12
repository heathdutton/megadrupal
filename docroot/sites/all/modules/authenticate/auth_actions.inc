<?php

/**
 * Implements hook_action_info().().
 */
function authenticate_action_info() {
  return array(
    'authenticate_report_notify_action' => array(
      'label' => t("Authenticate: Send notification email"),
      'type' => 'email',
      'batchable' => FALSE,
      'configurable' => TRUE,
      'triggers' => array(
        'nodeapi_presave',
        'nodeapi_insert',
        'nodeapi_update',
      ),
    ),
    'authenticate_report_notify_submitter_action' => array(
      'label' => t("Authenticate: Send notification to submitter"),
      'type' => 'email',
      'batchable' => FALSE,
      'configurable' => TRUE,
      'triggers' => array(
        'nodeapi_presave',
        'nodeapi_insert',
        'nodeapi_update',
      ),
    ),
  );
}


/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function authenticate_report_notify_submitter_action($node, $edit) {
  global $variables;

  $site_name = variable_get('site_name', 'Drupal');
  $from = "$site_name <" . variable_get('site_mail', ini_get('sendmail_from')) . '>';
  $subject = module_invoke('token', 'replace', $edit['subject'], 'node', $node);
  $message = module_invoke('token', 'replace', $edit['message'], 'node', $node);

  $name = $variables['auth_submitter_name'];
  $mail = $variables['auth_submitter_mail'];
  $path = $variables['auth_report_path'];

  $subject = str_replace(array('[auth_submitter_name]', '[auth_report_path]'), array($name, $path), $subject);
  $message = str_replace(array('[auth_submitter_name]', '[auth_report_path]'), array($name, $path), $message);

  $language = language_default();
  $params['subject'] = $subject;
  $params['body'] = $message;

  if (drupal_mail('authenticate', 'auth_report_notify_submitter', $mail, $language, $params, $from)) {
    watchdog('authenticate', t('Sent report complete notice to %user <%email>',  
      array('%user' => $name, '%email' => $mail)), WATCHDOG_NOTICE);
  }
  else {
    watchdog('authenticate', t('Unable to send email to %user <%email>', 
      array('%user' => $name, '%email' => $mail)), WATCHDOG_ERROR);
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function authenticate_report_notify_submitter_action_form($edit) {
  $form = array();
  $form['subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#required' => TRUE,
    '#default_value' => $edit['subject'],
    '#size' => '80',
    '#maxlength' => '254',
    '#description' => t('The subject of the email message. You may include "tokens" in this field which will be replaced with their actual
      values. Useful tokens include: [nid], [body], [teaser], [title], [url], [site-name]. This list is not comprehensive. For a complete
      list of available tokens, see the !token_definition_reference page.'
      , array('!token_definition_reference' => l(t('token definition reference'), 'library/712'))
    ),
  );
  $form['message'] = array(
    '#type' => 'textarea',
    '#title' => t('Message'),
    '#required' => TRUE,
    '#default_value' => $edit['message'],
    '#cols' => '80',
    '#rows' => '20',
    '#description' => t('The body of the email message. You may include "tokens" in this field which will be replaced with their actual
      values. Useful tokens include: [nid], [body], [teaser], [title], [url], [site-name]. This list is not comprehensive. You may 
      also use [auth_submitter_name], [auth_submitter_mail] and [auth_report_path] for variables specific to the Authenticate module.'),
  );

  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function authenticate_report_notify_submitter_action_submit($form, &$form_state) {
  $form_values = $form_state['values'];
  // Process the HTML form to store configuration. The keyed array that
  // we return will be serialized to the database.
  $params = array(
    'recipient' => $form_values['recipient'],
    'subject' => $form_values['subject'],
    'message' => $form_values['message'],
  );
  return $params;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function authenticate_report_notify_action(&$node, $edit) {
  global $variables;

  $site_name = variable_get('site_name', 'Drupal');
  $from = "$site_name <" . variable_get('site_mail', ini_get('sendmail_from')) . '>';
  $subject = module_invoke('token', 'replace', $edit['subject'], 'node', $node);
  $message = module_invoke('token', 'replace', $edit['message'], 'node', $node);
  $recipients = module_invoke('token', 'replace', $edit['recipients'], 'node', $node);

  $name = $variables['auth_submitter_name'];
  $mail = $variables['auth_submitter_mail'];
  $path = $variables['auth_report_path'];

  $subject = str_replace(array('[auth_submitter_name]', '[auth_report_path]'), array($name, $path), $subject);
  $message = str_replace(array('[auth_submitter_name]', '[auth_report_path]'), array($name, $path), $message);

  $language = language_default();
  $params['subject'] = $subject;
  $params['body'] = $message;

  if (drupal_mail('authenticate', 'auth_report_notify', $recipients, $language, $params, $from)) {
    watchdog('authenticate', t('Sent report complete notice to %recipients',  
      array('%recipients' => $recipients)), WATCHDOG_NOTICE);
  }
  else {
    watchdog('authenticate', 'Unable to send email to ' . $recipients, WATCHDOG_ERROR);
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function authenticate_report_notify_action_form($edit) {
  $form = array();
  $form['recipients'] = array(
    '#type' => 'textfield',
    '#title' => t('Recipients'),
    '#required' => TRUE,
    '#default_value' => $edit['recipients'],
    '#size' => '40',
    '#maxlength' => '254',
    '#description' => t('List of recipient email addresses who should be notified when the authentication analysis is complete.
      The formatting of this list must comply with RFC 2822. Some examples are: user@example.com, 
      anotheruser@example.com, User <user@example.com>, Another User <anotheruser@example.com>'),
  );
  $form['subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#required' => TRUE,
    '#default_value' => $edit['subject'] ? $edit['subject'] : "Authentication Analysis has Completed",
    '#size' => '80',
    '#maxlength' => '254',
    '#description' => t('The subject of the email message. You may include "tokens" in this field which will be replaced with their actual
      values. Useful tokens include: [nid], [body], [teaser], [title], [url], [site-name]. This list is not comprehensive. 
      You may also use [auth_submitter_name], [auth_submitter_mail] and [auth_report_path] for variables specific to the Authenticate module.'),
  );
  $form['message'] = array(
    '#type' => 'textarea',
    '#title' => t('Message'),
    '#required' => TRUE,
    '#default_value' => $edit['message'] ? $edit['message'] : "An authentication analysis was submitted and the results are now available at [auth_report_path].\n\rRegards,\n[site-name]",
    '#cols' => '80',
    '#rows' => '20',
    '#description' => t("The body of the email message. You may include 'tokens' in this field which will be replaced with their actual
      values. Useful tokens include: [nid], [body], [teaser], [title], [url], [site-name]. This list is not comprehensive. You may also use [auth_submitter_name], 
      [auth_submitter_mail] and [auth_report_path] for variables specific to the Authenticate module."),
  );

  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function authenticate_report_notify_action_submit($form, &$form_state) {
  $params = array(
    'recipients' => $form_state['values']['recipients'],
    'subject' => $form_state['values']['subject'],
    'message' => $form_state['values']['message'],
  );
  return $params;
}


/**
 * Define email action to send email notification on complettion of report.
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function authenticate_mail($key, &$message, $params) {
  //$language = $message['language'];
  //$variables = user_mail_tokens($params['account'], $language);
  switch ($key) {
    case 'auth_report_notify':
      $message['subject'] = $params['subject'];
      $message['body'][] = $params['body'];
      break;
    case 'auth_report_notify_submitter':
      $message['subject'] = $params['subject'];
      $message['body'][] = $params['body'];
      break;
  }
}

// creates precanned action to send email notification to submitter
function _auth_create_precanned_action() {
  $function = "authenticate_report_notify_submitter_action";
  $type = "email";
  $params = array(
    'subject' => "Authentication Analysis has Completed",
    'message' => "Dear [auth_submitter_name],\n\rThe authentication analysis for your document is complete and can be viewed at [auth_report_path].\n\rRegards,\n[site-name]",
  );
  $desc = "Authenticate: Send notification to submitter";
  actions_save($function, $type, $params, $desc);
}

