<?php

/**
 * @file
 * File containing all the functions that are needed only on administration
 * pages.
 */

/**
 * Menu Callback for admin/reports/mail_tester
 */
function mail_tester_admin_reports_mail_tester() {
  $build['mail_tester_email_data_form'] = drupal_get_form('mail_tester_email_data_form');

  return $build;
}

/**
 * Function to generate the form with the data to be tested in Mail Tester.
 */
function mail_tester_email_data_form($form, &$form_state) {
  $default_from = !empty($_SESSION['mail_tester']['mail_tester_from']) ? $_SESSION['mail_tester']['mail_tester_from'] : variable_get('site_mail', ini_get('sendmail_from'));
  $form['mail_tester_from'] = array(
    '#title' => t("From"),
    '#type' => 'textfield',
    '#default_value' => $default_from,
    '#description' => t("The test message will be sent from this remitent. The default value is the website e-mail address because it is normally used to send all the emails of the website (like new user and password recovery emails), but you can change it in case you want to test other email addresses."),
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t("Send to Mail Tester"),
  );

  return $form;
}

/**
 * Form validator for mail_tester_email_data_form() function.
 */
function mail_tester_email_data_form_validate(&$form, &$form_state) {
  $from = $form_state['values']['mail_tester_from'];
  if (!valid_email_address($from)) {
    form_set_error('mail_tester_from', t("The e-mail address %email is not valid.", array("%email" => $from)));
  }
}

/**
 * Form logic to be executed once the form is validated.
 */
function mail_tester_email_data_form_submit(&$form, &$form_state) {
  global $language;

  $from = $form_state['values']['mail_tester_from'];
  $_SESSION['mail_tester']['mail_tester_from'] = $from;

  $module = 'mail_tester';
  $key = 'test_message';
  $mail_tester_user = "dmt" . REQUEST_TIME . rand(1000000, 9999999);
  $to = $mail_tester_user . "@mail-tester.com";
  $params = array();
  $send = TRUE;

  $drupal_mail = drupal_mail($module, $key, $to, $language, $params, $from, $send);

  if (!empty($drupal_mail['result'])) {
    $url = 'http://www.mail-tester.com/' . $mail_tester_user;
    drupal_goto($url, array('external' => TRUE));
  }
  else {
    drupal_set_message(t("The email was not sent."), 'error');
  }
}
