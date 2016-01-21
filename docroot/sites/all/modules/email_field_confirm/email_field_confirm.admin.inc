<?php

/**
 * @file
 * Email Field Confirm admin settings.
 */

/**
 * Admin settings form for Email Field Confirm configuration.
 */
function email_field_confirm_admin_settings($form, &$form_state) {
  $form = array();

  // New email addresses.
  $form['email_field_confirm_email'] = array(
    '#type' => 'fieldset',
    '#title' => t('New Email Address Confirmation'),
    '#description' => t('The content below will be used for the email sent to the email address that requires confirmation.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['email_field_confirm_email']['email_field_confirm_confirm_email_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => email_field_confirm_mail_text('confirm_email_subject', NULL, array(), FALSE),
    '#size' => 60,
    '#maxlength' => 256,
    '#required' => TRUE,
  );
  $form['email_field_confirm_email']['email_field_confirm_confirm_email_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#default_value' => email_field_confirm_mail_text('confirm_email_body', NULL, array(), FALSE),
    '#cols' => 80,
    '#rows' => 10,
    '#required' => TRUE,
  );
  // Original email addresses.
  $form['email_field_confirm_email_original'] = array(
    '#type' => 'fieldset',
    '#title' => t('Original Email Address Notification'),
    '#description' => t('The content below will be used for the email sent to the <strong>original</strong> email address (if applicable).'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['email_field_confirm_email_original']['email_field_confirm_original_email_notify_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => email_field_confirm_mail_text('original_email_notify_subject', NULL, array(), FALSE),
    '#size' => 60,
    '#maxlength' => 256,
    '#required' => TRUE,
  );
  $form['email_field_confirm_email_original']['email_field_confirm_original_email_notify_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#default_value' => email_field_confirm_mail_text('original_email_notify_body', NULL, array(), FALSE),
    '#cols' => 80,
    '#rows' => 10,
    '#required' => TRUE,
  );

  return system_settings_form($form);
}
