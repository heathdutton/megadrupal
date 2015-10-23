<?php

/**
 * The admin settings form
 */
function email_required_admin_settings($form) {
  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings'),
  );
  $form['settings']['email_required_paths'] = array(
    '#type' => 'textarea',
    '#title' => t('Paths that require a verified email address'),
    '#description' => t('Supply a list of page paths on different lines which will require a user have a verified email addess in order to access.'),
    '#default_value' => variable_get('email_required_paths', NULL),
  );
  
  // Email text defaults
  $text = _email_required_email_text();
  
  $form['email'] = array(
    '#type' => 'fieldset',
    '#title' => t('Email template'),
    '#description' => t('Create a template for email verification messages.'),
  );
  $form['email']['email_required_email_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => $text['subject'],
  );
  $form['email']['email_required_email_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#default_value' => $text['body'],
    '#description' => t('Use !link to place the verification link.'),
  );
  
  $form['email']['email_required_token_tree'] = array(
    '#theme' => 'token_tree',
    '#token_types' => array('user'),
  );
  
  return system_settings_form($form);
}
