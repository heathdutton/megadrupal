<?php

/**
 * @file
 * Admin settings for Email Auto Login module.
 */

/**
 * Admin settings page.
 */
function email_auto_login_settings_form() {
  $timeout = variable_get('user_password_reset_timeout', 24 * 60 * 60);

  $form = array();

  $form['email_auto_login_expiration_time'] = array(
    '#type' => 'select',
    '#title' => t('How long should tokens remain valid for?'),
    '#options' => array(
      60 * 60 => t('1 hour'),
      60 * 60 * 24 => t('1 day'),
      60 * 60 * 24 * 7 => t('1 week'),
      60 * 60 * 24 * 30 => t('30 days'),
    ),
    '#default_value' => variable_get('email_auto_login_expiration_time', $timeout),
  );

  $form['email_auto_login_ignore_paths'] = array(
    '#title' => t('Ignore paths'),
    '#type' => 'textarea',
    '#default_value' => variable_get('email_auto_login_ignore_paths', 'user/reset/*'),
    '#description' => t("Enter one path per line. The '*' character is a wildcard.
Example paths are blog for the blog page and blog/* for every personal blog.
%front is the front page.", array('%front' => '<front>')),
  );

  $form['status'] = array(
    '#type' => 'fieldset',
    '#title' => t('Status'),
  );
  $form['status']['status'] = array(
    '#markup' => t('There are %token_count tokens in the database', array(
      '%token_count' => _email_auto_login_token_count(),
    )),
  );
  $form['status']['flush'] = array(
    '#type' => 'submit',
    '#value' => t('Clear Tokens'),
    '#submit' => array('_email_auto_login_token_flush'),
  );

  return system_settings_form($form);
}

/**
 * Return the number of tokens in the database.
 */
function _email_auto_login_token_count() {
  return db_select('email_auto_login_tokens')->countQuery()->execute()->fetchField();
}

/**
 * Flush all the tokens from the database.
 */
function _email_auto_login_token_flush($form, &$form_state) {
  db_truncate('email_auto_login_tokens')->execute();
}
