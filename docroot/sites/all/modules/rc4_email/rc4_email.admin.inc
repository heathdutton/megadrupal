<?php

/**
 * @file
 * Admin configuration form for RC4 email.
 */

/**
 * Form constructor for RC4 email admin form.
 *
 * @see rc4_email_menu()
 *
 * @ingroup forms
 */
function rc4_email_admin_setting() {
  $form = array();

  $form['rc4_email_nofollow'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add rel=“nofollow”'),
    '#default_value' => variable_get('rc4_email_nofollow', 1),
    '#return_value' => 1,
    '#description' => t('Instruct search engines that they don’t need to follow these email links.'),
  );
  $form['rc4_email_href'] = array(
    '#type' => 'radios',
    '#title' => t('Options for href'),
    '#default_value' => variable_get('rc4_email_href', '_rc4_email_obscure_emails'),
    '#options' => array(
      '_rc4_email_obscure_emails' => t('Apply email obfuscation'),
      'hash' => t('Drop link'),
    ),
    '#description' => t('What to do with email address found in a mailto href.'),
  );
  $form['rc4_email_regex'] = array(
    '#type' => 'textfield',
    '#title' => t('Email matching regular expression'),
    '#maxlength' => 1024,
    '#default_value' => variable_get('rc4_email_regex', '/\S+@\S+/'),
    '#description' => t('The email matching regex we pass straight to preg_match_all(). By default it is simply 1 or more non-space characters, followed by @ symbol, followed by 1 or more non-space characters.'),
  );

  return system_settings_form($form);
}
