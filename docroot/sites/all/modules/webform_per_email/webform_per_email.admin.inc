<?php

/**
 * @file
 * Admin for Webform per Email module.
 */

/**
 * Form for creating new permits and sending emails in bulk.
 */
function webform_per_email_bulk_token() {
  $form['webform_per_email_list'] = array(
    '#type' => 'textarea',
    '#title' => t('Enter emails'),
    '#rows' => 5,
    '#required' => TRUE,
  );

  $form['webform_per_email_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Webform Path'),
    '#required' => TRUE,
    '#default_value' => "",
    '#description' => t("Please enter path of the webform. E.g. for 'http://example.com/webform/myform', please enter 'webform/myform' or 'node/x'"),
    '#size' => 250,
    '#maxlength' => 250,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  return $form;
}

/**
 * Validator for Form webform_per_email_bulk_token.
 */
function webform_per_email_bulk_token_validate($form, &$form_state) {
  $values = $form_state['values'];
  $path = drupal_get_normal_path($values['webform_per_email_path']);
  if (url_is_external($path)) {
    form_set_error('webform_per_email_path', t("Please enter an internal drupal path without http://website.com. E.g. for 'http://example.com/webform/myform', please enter 'webform/myform' or 'node/x'"));
  }
  elseif (!drupal_valid_path($path)) {
    form_set_error('webform_per_email_path', t('Enter a valid path alias'));
  }
}

/**
 * Submit handler for Form webform_per_email_bulk_token.
 */
function webform_per_email_bulk_token_submit($form, &$form_state) {
  $values = $form_state['values'];
  $path = drupal_get_normal_path($values['webform_per_email_path']);

  $emails = $values['webform_per_email_list'];

  // Replace \n with space to unify the delimiter.
  $emails = str_replace("\n", " ", $emails);

  // Remove multiple spaces with one space to avoid blank emails
  // and spaces at the end and beginning.
  $emails = preg_replace('/\s+/', ' ', $emails);
  $emails = trim($emails);

  $emails = explode(" ", $emails);
  $nid = arg(1, $path);

  foreach ($emails as $email) {
    $permit = new WebformPermit(NULL, $email, $nid);
    $permit->save();

    $queue = DrupalQueue::get('webform_permit_outbox_queue');
    $queue_item = array(
      'email' => $email,
      'nid' => $nid,
      'hash' => $permit->getHash(),
    );

    // Calling all modules implementing hook_webform_per_email_mail_alter():
    drupal_alter('webform_per_email_mail', $queue_item);

    $queue->createItem($queue_item);
    drupal_set_message(t('Access to node/%nid for %email added to the webform_permit_outbox_queue', array('%email' => $email, '%nid' => $nid)));
    watchdog("webform_per_email_added", 'Access to node/%nid for %email added to the webform_permit_outbox_queue', array('%email' => $email, '%nid' => $nid), WATCHDOG_INFO);
  }
}

/**
 * Form for Webform per Email settings.
 */
function webform_per_email_settings() {
  $webform_per_email_settings = variable_get('webform_per_email_settings');

  $form['webform_per_email_mail_from_name'] = array(
    '#type' => 'textfield',
    '#title' => t('From Name'),
    '#required' => TRUE,
    '#default_value' => $webform_per_email_settings['mail_from_name'],
    '#description' => t('This name will appear as the from name'),
    '#size' => 100,
    '#maxlength' => 100,
  );

  $form['webform_per_email_mail_from_email'] = array(
    '#type' => 'textfield',
    '#title' => t('From Email Address'),
    '#required' => TRUE,
    '#default_value' => $webform_per_email_settings['mail_from_email'],
    '#description' => t('Emails will be sent from this address'),
    '#size' => 100,
    '#maxlength' => 100,
  );

  $form['webform_per_email_mail_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Mail Subject'),
    '#required' => TRUE,
    '#default_value' => $webform_per_email_settings['mail_subject'],
    '#description' => t('Mail Subject'),
    '#size' => 100,
    '#maxlength' => 100,
  );

  $form['webform_per_email_mail_template'] = array(
    '#type' => 'textarea',
    '#title' => t('Create the mail template'),
    '#rows' => 5,
    '#description' => t('@path = Drupal Path, @link = Drupal Path with permit, @email = email, @nid = Node id, @hash = permit hash'),
    '#default_value' => format_string($webform_per_email_settings['mail_template']),
    '#required' => TRUE,
  );

  $form['webform_per_email_mail_cron_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of emails to send per cron run'),
    '#required' => TRUE,
    '#default_value' => $webform_per_email_settings['cron_limit'],
    '#description' => t('Number of emails to send per cron run'),
    '#size' => 3,
    '#maxlength' => 3,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  return $form;
}

/**
 * Validator for Form webform_per_email_settings_submit.
 */
function webform_per_email_settings_validate($form, &$form_state) {
  $mail_from_email = $form_state['values']['webform_per_email_mail_from_email'];
  if (!valid_email_address($mail_from_email)) {
    form_set_error('webform_per_email_mail_from_email', t('Please enter a valid From Email'));
  }
}

/**
 * Submit Handler for Form webform_per_email_settings.
 */
function webform_per_email_settings_submit($form, &$form_state) {
  $cron_limit = $form_state['values']['webform_per_email_mail_cron_limit'];
  $webform_per_email_settings = array(
    'mail_from_name' => $form_state['values']['webform_per_email_mail_from_name'],
    'mail_from_email' => $form_state['values']['webform_per_email_mail_from_email'],
    'mail_subject' => $form_state['values']['webform_per_email_mail_subject'],
    'mail_template' => $form_state['values']['webform_per_email_mail_template'],
    'cron_limit' => is_numeric($cron_limit) ? $cron_limit : 25,
  );
  variable_set('webform_per_email_settings', $webform_per_email_settings);
}
