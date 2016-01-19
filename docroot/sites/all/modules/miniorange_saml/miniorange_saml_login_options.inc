<?php
/**
 * @file
 * Contains Login Settings for miniOrange SAML Login Module.
 */

 /**
 * Showing Settings form.
 */
 function miniorange_saml_login_options($form, &$form_state) {

  $form['markup_1'] = array(
    '#markup' => '<h2>Sign in Settings</h3>',
  );

  $form['miniorange_saml_force_auth'] = array(
    '#type' => 'checkbox',
    '#title' => t('Force authentication with your IdP on each login attempt: <b>[PREMIUM]</b>'),
    '#disabled' => TRUE,
  );

  $form['markup_2'] = array(
    '#markup' => '<b>Note: </b>Users will be redirected to your IdP for login in case user is not logged in and tries to access website.<br><br>',
  );

  $form['miniorange_saml_auto_redirect'] = array(
    '#type' => 'checkbox',
    '#title' => t('Check this option if you want to <b>auto redirect the user to IdP</b>: <b>[PREMIUM]</b>'),
    '#disabled' => TRUE,
  );

  /*$form['miniorange_saml_gateway_config_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Configuration'),
    '#submit' => array('miniorange_saml_save_signin_settings'),
  );*/

  return $form;

 }

 function miniorange_saml_save_signin_settings($form, &$form_state) {

 	drupal_set_message(t('Signin Settings successfully saved'));

 }
