<?php

/**
 * @file
 * Admin page callback file for the Username Reminder module.
 */

/**
 * Form constructor for the Username Reminder settings form.
 *
 * @see system_settings_form()
 *
 * @ingroup forms
 */
function username_reminder_settings_form() {
  $form['email_username_reminder'] = array(
    '#type' => 'fieldset',
    '#title' => t('E-mail'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('Edit the e-mail messages sent to users who request a username reminder.'),
  );
  $form['email_username_reminder']['username_reminder_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => _username_reminder_text('subject', NULL, array(), FALSE),
    '#maxlength' => 180,
  );
  $form['email_username_reminder']['username_reminder_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#default_value' => _username_reminder_text('body', NULL, array(), FALSE),
    '#rows' => 12,
  );
  // Display list of tokens that may be useful. If the Token module is
  // installed, use it to list them. Otherwise, display an abbreviated list of
  // the same tokens the User module displays for its configurable e-mails.
  if (module_exists('token')) {
    $form['email_username_reminder']['tokens'] = array(
      '#type' => 'fieldset',
      '#title' => t('Replacement patterns'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#description' => theme('token_tree', array(
          'token_types' => array('site', 'user'),
          'global_types' => FALSE,
        )),
    );
  }
  else {
    $email_token_help = t('Available variables are: [site:name], [site:url], [user:name], [user:mail], [site:login-url], [site:url-brief], [user:edit-url].');
    $form['email_username_reminder']['#description'] .= ' ' . $email_token_help;
  }

  return system_settings_form($form);
}
