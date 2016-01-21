<?php
/**
 * @file
 * Forms for use in Direct Post Method transactions.
 */

/**
 * Returns an array of form elements common to all Direct Post forms.
 *
 * @param array $payment_method
 *   The current payment method.
 * @param object $order
 *   The current order.
 *
 * @return array
 *   Array of common Direct Post form fields.
 */
function commerce_iats_direct_post_form_base($payment_method, $order) {
  global $base_url;

  $form = array();

  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  $address_data = $order_wrapper->commerce_customer_billing->commerce_customer_address->value();

  commerce_iats_add_extra_address_fields($address_data);

  // Relay URL.
  $form['iats_dpm_relay_url'] = array(
    '#type' => 'hidden',
    '#name' => 'IATS_DPM_RelayURL',
    '#default_value' => $base_url . '/commerce_iats/direct_post_relay/',
  );

  // Process Key.
  $form['iats_dpm_process_key'] = array(
    '#type' => 'hidden',
    '#name' => 'IATS_DPM_ProcessID',
    '#default_value' => $payment_method['settings']['direct_post']['process_key'],
  );

  // Total processed amount.
  $form['iats_dpm_amount'] = array(
    '#type' => 'hidden',
    '#name' => 'IATS_DPM_Amount',
    '#default_value' => commerce_currency_amount_to_decimal($order->commerce_order_total[LANGUAGE_NONE][0]['amount'],
      $order->commerce_order_total[LANGUAGE_NONE][0]['currency_code']),
  );

  // Invoice number.
  $invoice_number = $order->order_number;
  drupal_alter('commerce_iats_invoice_number', $invoice_number, $order);

  $form['iats_dpm_invoice'] = array(
    '#type' => 'hidden',
    '#name' => 'IATS_DPM_Invoice',
    '#default_value' => $payment_method['settings']['invoice_prefix'] . $invoice_number,
  );

  // Store order information in comment for future reference.

  // Get current payment method instance ID.
  $payment_method_instance_id = NULL;
  foreach ($order->payment_methods as $instance_id => $payment_method_instance) {
    if ($payment_method_instance['method_id'] == $payment_method['method_id']) {
      $payment_method_instance_id = $instance_id;
      continue;
    }
  }

  $comment_parts = array(
    $order->order_id,
    $payment_method_instance_id,
    // Process key used to verify response from iATS.
    // Trimmed to avoid IATS_DPM_Comment length limit (110 chars.)
    substr($payment_method['settings']['direct_post']['process_key'], 0, 8),
  );

  $form['iats_dpm_comment'] = array(
    '#type' => 'hidden',
    '#name' => 'IATS_DPM_Comment',
    '#default_value' => COMMERCE_IATS_COMMENT_PREFIX . implode('|', $comment_parts),
  );

  // First name for the end user.
  $form['iats_dpm_first_name'] = array(
    '#type' => 'hidden',
    '#name' => 'IATS_DPM_FirstName',
    '#default_value' => $address_data['first_name'],
  );

  // Last name for the end user.
  $form['iats_dpm_last_name'] = array(
    '#type' => 'hidden',
    '#name' => 'IATS_DPM_LastName',
    '#default_value' => $address_data['last_name'],
  );

  // Address for the end user.
  $form['iats_dpm_address'] = array(
    '#type' => 'hidden',
    '#name' => 'IATS_DPM_Address',
    '#default_value' => $address_data['thoroughfare'],
  );

  // City of the address.
  $form['iats_dpm_city'] = array(
    '#type' => 'hidden',
    '#name' => 'IATS_DPM_City',
    '#default_value' => $address_data['locality'],
  );

  // Province of the address.
  $form['iats_dpm_province'] = array(
    '#type' => 'hidden',
    '#name' => 'IATS_DPM_Province',
    '#default_value' => $address_data['administrative_area'],
  );

  // Country of the address.
  $form['iats_dpm_country'] = array(
    '#type' => 'hidden',
    '#name' => 'IATS_DPM_Country',
    '#default_value' => $address_data['country'],
  );

  // Zip code of the address.
  $form['iats_dpm_zip_code'] = array(
    '#type' => 'hidden',
    '#name' => 'IATS_DPM_ZipCode',
    '#default_value' => $address_data['postal_code'],
  );

  return $form;
}

/**
 * Returns an array of form elements used by the credit card Direct Post form.
 *
 * @param array $payment_method
 *   The current payment method.
 * @param object $order
 *   The current order.
 *
 * @return array
 *   Array of credit card Direct Post form fields.
 */
function commerce_iats_direct_post_credit_card_form($payment_method, $order) {
  module_load_include('inc', 'commerce_payment', 'includes/commerce_payment.credit_card');

  $default_form = commerce_payment_credit_card_form();

  $form = commerce_iats_direct_post_form_base($payment_method, $order);

  $form['#attached'] = $default_form['credit_card']['#attached'];
  $form['#attached']['js'] = array(
    drupal_get_path('module', 'commerce_iats') . '/js/commerce_iats.direct_post.credit_card.js',
  );

  $credit_card_types = commerce_iats_credit_card_types();

  // Limit to the enabled card types
  $enabled_cards = commerce_iats_enabled_credit_card_types($payment_method);

  $credit_card_mop = array();
  foreach ($credit_card_types as $type => $name) {
    if (in_array($type, $enabled_cards)) {
      $credit_card_mop[commerce_iats_credit_card_mop($type)] = $name;
    }
  }

  // Credit card type. #Validated prevents illegal choice error on AJAX call.
  $form['iats_dpm_mop'] = array(
    '#type' => 'select',
    '#title' => t('Card type'),
    '#name' => 'IATS_DPM_MOP',
    '#options' => $credit_card_mop,
    '#required' => TRUE,
    '#validated' => TRUE,
  );

  // Credit card number.
  $form['iats_dpm_account_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Card number'),
    '#name' => 'IATS_DPM_AccountNumber',
    '#attributes' => array('autocomplete' => 'off'),
    '#required' => TRUE,
    '#maxlength' => 19,
    '#size' => 20,
  );

  // Expiry date elements shown to the user.
  $form['internal_iats_dpm_exp_month'] = $default_form['credit_card']['exp_month'];
  $form['internal_iats_dpm_exp_month']['#name'] = 'internal_iats_dpm_exp_month';
  $form['internal_iats_dpm_exp_year'] = $default_form['credit_card']['exp_year'];
  $form['internal_iats_dpm_exp_year']['#name'] = 'internal_iats_dpm_exp_year';

  // Expiry date string populated using JS.
  $form['iats_dpm_exp'] = array(
    '#type' => 'hidden',
    '#name' => 'IATS_DPM_ExpiryDate',
  );

  // CVV2.
  $form['iats_dpm_cvv2'] = array(
    '#type' => 'textfield',
    '#title' => t('Security code'),
    '#name' => 'IATS_DPM_CVV2',
    '#attributes' => array('autocomplete' => 'off'),
    '#required' => TRUE,
    '#maxlength' => 4,
    '#size' => 4,
  );

  return $form;
}

/**
 * Adds customer code capability to the Direct Post credit card form.
 *
 * @param array $payment_method
 *   The current payment method.
 * @param object $order
 *   The current order.
 *
 * @return array
 *   Array of credit card Direct Post form fields.
 */
function commerce_iats_direct_post_credit_card_customer_code_form($payment_method, $order) {
  $form = commerce_iats_direct_post_credit_card_form($payment_method, $order);

  // Creating token set to TRUE.
  $form['iats_dpm_creating_token'] = array(
    '#type' => 'hidden',
    '#name' => 'IATS_DPM_CreatingToken',
    '#default_value' => 'TRUE',
  );

  return $form;
}

/**
 * Returns an array of form elements used by the ACH/EFT Direct Post form.
 *
 * @param array $payment_method
 *   The current payment method.
 * @param object  $order
 *   The current order.
 *
 * @return array
 *   Array of ACH/EFT Direct Post form fields.
 */
function commerce_iats_direct_post_ach_eft_form($payment_method, $order) {
  module_load_include('inc', 'commerce_iats', 'includes/commerce_iats.ach_eft.forms');

  $form = commerce_iats_direct_post_form_base($payment_method, $order);

  $form['#attached']['js'] = array(
    drupal_get_path('module', 'commerce_iats') . '/js/commerce_iats.direct_post.ach_eft.js',
  );

  $currency_code = $order->commerce_order_total[LANGUAGE_NONE][0]['currency_code'];

  $fields = array(
    'number' => '',
  );

  switch ($currency_code) {
    case 'USD':
      $fields['routing_number'] = '';
      $fields['type'] = '';
      $default_form = commerce_iats_ach_eft_payment_form($fields);
      $default_form['bank_account']['routing_number']['#name'] = 'internal_iats_dpm_routing_number';
      break;

    case 'CAD':
      $fields['bank_number'] = '';
      $fields['transit_number'] = '';
      $fields['type'] = '';
      $default_form = commerce_iats_ach_eft_payment_form($fields);
      $default_form['bank_account']['bank_number']['#name'] = 'internal_iats_dpm_bank_number';
      $default_form['bank_account']['transit_number']['#name'] = 'internal_iats_dpm_transit_number';
      break;

    case 'GBP':
      $fields['sort_code'] = '';
      $default_form = commerce_iats_ach_eft_payment_form($fields);
      $default_form['bank_account']['sort_code']['#name'] = 'internal_iats_dpm_sort_code';
      break;

    default:
      $default_form = commerce_iats_ach_eft_payment_form($fields);
  }

  if (isset($default_form['bank_account']['number'])) {
    $default_form['bank_account']['number']['#name'] = 'internal_iats_dpm_number';
  }

  if (isset($default_form['bank_account']['type'])) {
    $default_form['bank_account']['type']['#name'] = 'internal_iats_dpm_type';
  }

  $form = array_merge($default_form, $form);

  // ACH/EFT payment type.
  $form['iats_dpm_mop'] = array(
    '#type' => 'hidden',
    '#name' => 'IATS_DPM_MOP',
    '#default_value' => 'ACHEFT',
  );

  // Account number.
  $form['iats_dpm_account_number'] = array(
    '#type' => 'hidden',
    '#name' => 'IATS_DPM_AccountNumber',
  );

  // Account type.
  $form['iats_dpm_account_type'] = array(
    '#type' => 'hidden',
    '#name' => 'IATS_DPM_AccountType',
  );

  return $form;
}

/**
 * Adds customer code capability to the Direct Post ACH/EFT form.
 *
 * @param array $payment_method
 *   The current payment method.
 * @param object $order
 *   The current order.
 *
 * @return array
 *   Array of ACH/EFT Direct Post form fields.
 */
function commerce_iats_direct_post_ach_eft_customer_code_form($payment_method, $order) {
  $form = commerce_iats_direct_post_ach_eft_form($payment_method, $order);

  // Creating token set to TRUE.
  $form['iats_dpm_creating_token'] = array(
    '#type' => 'hidden',
    '#name' => 'IATS_DPM_CreatingToken',
    '#default_value' => 'TRUE',
  );

  return $form;
}
