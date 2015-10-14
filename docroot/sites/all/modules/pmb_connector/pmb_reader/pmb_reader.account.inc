<?php

/**
 * @file
 * PMB reader account.
 */

function pmb_reader_information($user = NULL) {
  if (!$user || !$user->uid) {
    global $user;

    // Check if user is not anonymous.
    if ($user->uid) {
      drupal_goto(_pmb_p('reader/') . $user->uid);
    }
    else {
      drupal_set_message(t('You are not an authenticated reader.'), 'warning');
      return drupal_access_denied();
    }
  }

  drupal_set_title(t('My reader account: Summary'));
  $breadcrumb = array();
  $breadcrumb[] = l(t('Home'), NULL);
  $breadcrumb[] = t('My reader account: @name', array('@name' => $user->name));
  drupal_set_breadcrumb($breadcrumb);

  require_once(drupal_get_path('module', 'pmb') . '/pmb.class_data.inc');
  $pmb_data = new pmb_data();
  $pmb_data->set_reader($user);
  $reader_info = $pmb_data->reader_get_account_info();

  return theme('pmb_reader_account', array(
    'reader' => $reader_info,
    'parameters' => array(),
  ));
}

function pmb_reader_loans($user) {
  drupal_set_title(t('My reader account: Loans'));
  $breadcrumb = array();
  $breadcrumb[] = l(t('Home'), NULL);
  $breadcrumb[] = l(t('My reader account: @name', array('@name' => $user->name)), _pmb_p('reader/') . $user->uid);
  $breadcrumb[] = t('Loans');
  drupal_set_breadcrumb($breadcrumb);

  require_once(drupal_get_path('module', 'pmb') . '/pmb.class_data.inc');
  $pmb_data = new pmb_data();
  $pmb_data->set_reader($user);
  $loans = $pmb_data->reader_get_loans($user);

  return theme('pmb_reader_loans', array(
    'reader' => $user,
    'loans' => $loans,
    'parameters' => array(),
  ));
}
