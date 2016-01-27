<?php

/**
 * @file
 * Admin page callbacks.
 *
 * Copyright (C) 2012, Victor Nikulshin
 *
 * This file is part of KKB Epay.
 *
 * KKB Epay is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * KKB Epay is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with KKB Epay.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * Form builder. Debugging mode controll form.
 */
function kkb_epay_admin_debug_form($form, &$form_state) {
  $form['#theme'] = 'system_settings_form';

  $form['kkb_epay_debug_status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Put KKB Epay module into debugging mode'),
    '#default_value' => kkb_epay_debug_status(),
    '#description' => t('When debugging mode is turned on, all payments are processed by a sandbox gateway. Payments can be done only with special testing bank cards. Make sure to turn debugging off as soon as you finish testing. You risk loosing money if real clients make payments while it is on.'),
  );
  $form['password_confirmation'] = array(
    '#type' => 'password',
    '#title' => t('Provide your password to save changes:'),
    '#required' => TRUE,
  );

  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Save configuration'),
    ),
  );

  $form['#validate'] = array('kkb_epay_admin_debug_form_validate');
  $form['#submit'] = array('kkb_epay_admin_debug_form_submit');
  return $form;
}

/**
 * Form handler validates kkb_epay_admin_debug_form.
 */
function kkb_epay_admin_debug_form_validate($form, &$form_state) {
  global $user;
  require_once DRUPAL_ROOT . '/' . variable_get('password_inc', 'includes/password.inc');

  if (!user_check_password($form_state['values']['password_confirmation'], $user)) {
    form_set_error('password_confirmation', t('Incorrect password.'));
  }
}

/**
 * Form handler submits kkb_epay_admin_debug_form.
 */
function kkb_epay_admin_debug_form_submit($form, &$form_state) {
  $new_status = (bool) $form_state['values']['kkb_epay_debug_status'];
  variable_set('kkb_epay_debug_status', $new_status);

  if ($new_status) {
    drupal_set_message(t('Debugging mode has been turned on. Turn it off as soon as possible.'), 'warning');
  }
  else {
    drupal_set_message(t('Debugging mode has been turned off.'));
  }
}

/**
 * Form builder. Private key configuration form.
 */
function kkb_epay_admin_private_key_form($form, &$form_state) {
  $form['key_data'] = array(
    '#tree' => TRUE,
  );

  $form['key_data']['certificate_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Certificate ID:'),
    '#size' => 20,
    '#maxlegnth' => 10,
    '#required' => TRUE,
  );

  $form['key_data']['merchant_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Merchant name:'),
    '#size' => 60,
    '#maxlegnth' => 255,
    '#required' => TRUE,
  );

  $form['key_data']['merchant_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Merchant ID:'),
    '#size' => 20,
    '#maxlegnth' => 8,
    '#required' => TRUE,
  );

  $form['key_data']['password'] = array(
    '#type' => 'textfield',
    '#title' => t('Private key password:'),
    '#size' => 60,
    '#maxlegnth' => 255,
    '#required' => FALSE,
  );

  $form['key_data']['key'] = array(
    '#type' => 'file',
    '#title' => t('Private key:'),
    '#description' => t('Name of the file with the private key is <em>&lt;client name&gt;.prv</em>.'),
    '#size' => 60,
  );

  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Import new private key'),
    ),
  );

  $form['#validate'] = array('kkb_epay_admin_private_key_form_validate');
  $form['#submit'] = array('kkb_epay_admin_private_key_form_submit');

  return $form;
}

/**
 * Form handler validates kkb_epay_admin_private_key_form.
 *
 * The KkbEpay_Key model is capable of self-validation and since its
 * constraints can be quite complicated, it would be inefficient to repeat all
 * of them here. That is why we will create an instance of KkbEpay_Key class,
 * set it with inputed values and catch any validation errors.
 */
function kkb_epay_admin_private_key_form_validate($form, &$form_state) {
  $key = new KkbEpay_Key();

  if (!empty($_FILES['files']['tmp_name']['key_data'])) {
    $form_state['values']['key_data']['key'] = trim(file_get_contents(
      $_FILES['files']['tmp_name']['key_data']
    ));
  }
  else {
    form_set_error('key_data][key', t('You must upload file with the private key.'));
    $form_state['values']['key_data']['key'] = NULL;
  }

  $fields = array(
    'key', 'password', 'merchant_id', 'merchant_name', 'certificate_id',
  );
  foreach ($fields as $f) {
    $method = 'set' . str_replace('_', '', $f);

    try {
      $key->$method($form_state['values']['key_data'][$f]);
    }
    catch (KkbEpay_KeyException $ex) {
      form_set_error("key_data][$f", t($ex->getMessage()));
    }
  }

  if (!$key->isValid()) {
    form_set_error('key_data', t('There is something wrong with this key. Please check that the provided information is correct.'));
  }

  try {
    $key->open();
  }
  catch (KkbEpay_KeyException $ex) {
    form_set_error("key_data][$f", t($ex->getMessage()));
  }
}

/**
 * Form handler submits kkb_epay_admin_private_key_form.
 *
 * @todo
 *   It would be much better to write packed private key directly to the
 *   settings.php file and not ask user to do it manually.
 */
function kkb_epay_admin_private_key_form_submit($form, &$form_state) {
  $packed = _kkb_epay_pack_private_key($form_state['values']['key_data']);

  $_SESSION['kkb_epay']['packed_private_key'] = $packed;
  $form_state['redirect'] = 'admin/config/services/epay/save-private-key';
}

/**
 * Menu page callback.
 *
 * Displays previously imported private key in a packaged format and gives
 * further instructions to the user.
 */
function kkb_epay_admin_private_key_save_page() {
  if (empty($_SESSION['kkb_epay']['packed_private_key'])) {
    drupal_goto('admin/config/services/epay/private-key');
  }

  $conf = "\$conf['kkb_epay_private_key_live'] = <<<ENDPRIVATEKEY\n";
  $conf .= $_SESSION['kkb_epay']['packed_private_key'];
  $conf .= "\nENDPRIVATEKEY;\n\n";

  $page = array();
  $page['help'] = array(
    array(
      '#prefix' => '<p>',
      '#markup' => t('<strong>Your private key has been imported, but it was not yet saved.</strong> These instructions will tell you how to do it.'),
      '#suffix' => '</p>',
    ),
    array(
      '#prefix' => '<p>',
      '#markup' => t('The private key must be kept <strong>in secret</strong> because anyone who has it can perform financial operations on your behalf; for example, debit any credit cards, cancel payments, transfer money from your account, etc.'),
      '#suffix' => '</p>',
    ),
    array(
      '#prefix' => '<p>',
      '#markup' => t(
        'Copy the code below. Do not modify it! Place it at the end of the <strong>@file</strong> file and save the file.',
        array('@file' => conf_path() . '/settings.php')
      ),
      '#suffix' => '</p>',
    ),
  );

  $page['configuration'] = array(
    '#type' => 'textarea',
    '#title' => t('Place this code into your settings.php file:'),
    '#value' => $conf,
    '#rows' => 30,
    '#cols' => 80,
    '#attributes' => array(
      'readonly' => 'readonly',
    ),
  );

  return $page;
}
