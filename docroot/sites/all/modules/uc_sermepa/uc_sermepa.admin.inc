<?php

/**
 * @file
 * Admin functions for uc sermepa.
 */

/**
 * Display all the banks with its possible operations.
 */
function uc_sermepa_admin_settings() {
  $banks = uc_sermepa_get_all_banks();
  $rows = array();
  $output = "";
  foreach ($banks as $bank_details) {
    $row = array();
    $row[] = $bank_details['bankname'];
    $row[] = $bank_details['bankmode'];
    if (!$bank_details['enabled']) {
      $row[] = t('Disabled');
    }
    else {
      $row[] = t('Enabled');
    }
    $row[] = l(t('Edit'), 'admin/store/settings/uc_sermepa/edit/' . $bank_details['bankcode']);
    $row[] = l(t('Delete'), 'admin/store/settings/uc_sermepa/delete/' . $bank_details['bankcode']);
    $rows[] = $row;
  }

  if (count($rows)) {
    $header = array(
      t('Bank Name'),
      t('Mode'),
      t('Enabled'),
      array('data' => t('Operations'), 'colspan' => 3));
    $output .= theme('table', array('header' => $header, 'rows' => $rows));
  }
  else {
    $output .= t('There are no banks configured, you should start adding one.');
  }

  return $output;
}

/**
 * Form to manage the banks.
 */
function uc_sermepa_form_bank($form_state) {
  $form = array();
  $bankcode = arg(5);
  $mode = arg(4);
  //Determine the default values
  $bank_defaults = new stdClass;
  $bank_defaults->enabled = 0;
  $bank_defaults->encryption = 'sha1';
  $bank_defaults->bankmode = 'test';
  $bank_defaults->currency = 978;
  $bank_defaults->bankcode = NULL;
  $bank_defaults->bankname = NULL;
  $bank_defaults->ds_merchant_titular = NULL;
  $bank_defaults->ds_merchant_name = NULL;
  $bank_defaults->ds_merchantcode = NULL;
  $bank_defaults->ds_merchant_terminal = NULL;
  $bank_defaults->ds_merchantsignature = NULL;
  $bank_defaults->ds_merchant_productdescription = NULL;
  $bank_defaults->ds_merchant_paymethods = NULL;
  $bank_defaults->url = NULL;

  if ($mode == 'edit' && $bankcode) {
    $bank_defaults = uc_sermepa_get_bank($bankcode);
  }

  $form['enabled'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Enabled'),
    '#default_value' => $bank_defaults->enabled,
    '#required'      => FALSE,
  );

  $form['bankcode'] = array(
    '#type'          => 'textfield',
    '#description'   => t('Codename for the bank, must be unique'),
    '#title'         => t('Bank Code'),
    '#default_value' => $bank_defaults->bankcode ,
    '#size'          => 80,
    '#maxlength'     => 255,
    '#required'      => TRUE,
  );
  if ($bank_defaults->bankcode) {
    $form['bankcode']['#disabled'] = TRUE;
    $form['bankcode']['#value'] = $bank_defaults->bankcode;
  }
  $form['bankname'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Bank Name'),
    '#default_value' => $bank_defaults->bankname,
    '#size'          => 80,
    '#maxlength'     => 255,
    '#required'      => TRUE,
  );
  $form['bankmode'] = array(
    '#type'          => 'radios',
    '#title'         => t('Mode of the bank'),
    '#default_value' => $bank_defaults->bankmode,
    '#options'       => array('test' => t('Test'), 'live' => t('Live')),
    '#size'          => 10,
    '#maxlength'     => 5,
    '#required'      => TRUE,
  );
  $form['ds_merchant_titular'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Ds Merchant Titular'),
    '#default_value' => $bank_defaults->ds_merchant_titular,
    '#size'          => 80,
    '#maxlength'     => 255,
    '#required'      => TRUE,
    '#description' => t('Name shown on the TPV confirmation page'),
  );
  $form['ds_merchant_name'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Ds Merchant Name'),
    '#default_value' => $bank_defaults->ds_merchant_name,
    '#size'          => 80,
    '#maxlength'     => 255,
    '#required'      => TRUE,
    '#description' => t('Name of the commerce shown on customer ticket'),
  );
  $form['ds_merchantcode'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Ds MerchantCode'),
    '#default_value' => $bank_defaults->ds_merchantcode,
    '#size'          => 80,
    '#maxlength'     => 255,
    '#required'      => TRUE,
    '#description' => t('FUC code assigned to commerce'),
  );
  $form['ds_merchantsignature'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Signature Key'),
    '#default_value' => $bank_defaults->ds_merchantsignature,
    '#size'          => 80,
    '#maxlength'     => 255,
    '#required'      => TRUE,
    '#description' => t('SHA256 Commerce Key'),
  );
  $form['ds_merchant_terminal'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Ds Merchant Terminal'),
    '#default_value' => $bank_defaults->ds_merchant_terminal,
    '#size'          => 10,
    '#maxlength'     => 10,
    '#required'      => TRUE,
    '#description' => t('Terminal number that your bank has assigned you (3 digits max)'),
  );
  $form['ds_merchant_productdescription'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Ds Merchant ProductDescription'),
    '#default_value' => $bank_defaults->ds_merchant_productdescription,
    '#size'          => 80,
    '#maxlength'     => 255,
    '#description'   => t('Transaction description sent to payment bridge. Use [uc_order:order-number] to show order_id.'),
  );

  // Add token helper to form
  $form['tokens'] = array(
    '#theme' => 'token_tree',
    '#token_types' => array('uc_order'),
  );

  $form['ds_merchant_paymethods'] = array(
    '#type'          => 'radios',
    '#title'         => t('Payment methods (card, transfer, billing)'),
    '#default_value' => $bank_defaults->ds_merchant_paymethods,
    '#options'       => array('' => t('All'), 'T' => t('Card'), 'R' => t('Transfer'), 'D' => t('Billing')),
    '#size'          => 10,
    '#maxlength'     => 5,
    '#required'      => FALSE,
  );
  $form['currency'] = array(
    '#type'          => 'radios',
    '#title'         => t('Currency'),
    '#default_value' => $bank_defaults->currency,
    '#options'       => array(
      '978' => t('Euro'),
      '840' => t('Dollar'),
      '826' => t('Pound'),
    ),
    '#required'      => TRUE,
  );
  $form['url'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Bank connect url'),
    '#default_value' => $bank_defaults->url,
    '#size'          => 80,
    '#maxlength'     => 255,
    '#required'      => TRUE,
  );
  $form['encryption'] = array(
    '#type'          => 'radios',
    '#title'         => t('Method of encryption'),
    '#default_value' => $bank_defaults->encryption,
    '#options'       => array(
      'sha1' => t('SHA (Deprecated)'),
      'sha1-ampliado' => t('SHA Ampliado (Deprecated)'),
      'hmac_sha256_v1' => t('SHA256'),
    ),
    '#required'      => TRUE,
  );
  $form['submit'] = array(
    '#type'   => 'submit',
    '#value'  => t('Save Bank'),
    '#suffix' => l(t('Cancel'), 'admin/store/settings/uc_sermepa'),
  );

  return $form;
}

/**
 * Submit for bank edit/add form.
 */
function uc_sermepa_form_bank_submit($form, &$form_state) {
  $result = uc_sermepa_save_bank($form_state['values']);
  if ( $result ) drupal_set_message(t('Bank saved correctly'));
  $form_state['redirect'] = 'admin/store/settings/uc_sermepa';
}


/**
 * Handle the restore/delete bank confirmation form.
 */
function uc_sermepa_form_restore_delete_bank($form_state, $mode = NULL) {
  $form = array();
  $action = arg(4);
  $bankcode = arg(5);
  $bank = uc_sermepa_get_bank($bankcode);
  $form['bankname'] = array('#type' => 'value', '#value' => $bank->bankname);
  $form['bankcode'] = array('#type' => 'value', '#value' => $bank->bankcode);
  $form['bankmode'] = array('#type' => 'value', '#value' => $bank->bankmode);
  $form['mode'] = array('#type' => 'value', '#value' => $action);
  if ($action == 'restoretodefault') {
    $message = t('Are you sure you want to restore the configuration of %bank?', array('%bank' => $bank->bankname));
    $button = t('Restore settings');
  }
  elseif ($action == 'delete') {
    $message = t('Are you sure you want to delete %bank in %mode mode?', array('%bank' => $bank->bankname, '%mode' => $bank->bankmode));
    $button = t('Delete');
  }
  $caption = '<p>' . t('This action cannot be undone.') . '</p>';
  return confirm_form($form, $message, 'admin/store/settings/uc_sermepa', $caption, $button);
}

function uc_sermepa_form_restore_delete_bank_submit($form, &$form_state) {
  if ($form_state['values']['mode'] == 'delete') {
    uc_sermepa_delete_bank($form_state['values']['bankcode']);
    drupal_set_message(t('%bank in %mode mode deleted', array('%bank' => $form_state['values']['bankname'], '%mode' => $form_state['values']['bankmode'])));
  }
  elseif ($form_state['values']['mode'] == 'restoretodefault') {
    $bank = uc_sermepa_default_banks($form_state['values']['bankcode']);
    uc_sermepa_delete_bank($form_state['values']['bankcode']);
    drupal_write_record('uc_sermepa_settings', $bank);
    drupal_set_message(t('The default settings of %bankname have been restored', array('%bankname' => $form_state['values']['bankname'])));
  }
  $form_state['redirect'] = 'admin/store/settings/uc_sermepa';
}
