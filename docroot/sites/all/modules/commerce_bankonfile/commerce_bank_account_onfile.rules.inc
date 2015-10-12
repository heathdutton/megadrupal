<?php

/**
 * @file
 * Provides Rules integration
 */

/**
 * Implements hook_rules_data_info().
 */
function commerce_bank_account_onfile_rules_data_info() {
  return array(
    'commerce_bank_account_onfile_charge_bank_account_response' => array(
      'label' => t('charge bank account response'),
      'group' => t('Bank Account on File'),
      'wrap' => TRUE,
      'wrapper class' => 'EntityStructureWrapper',
      'token type' => 'commerce-bank-account-onfile-charge-bank-account-response',
      'property info' => commerce_bank_account_onfile_charge_bank_account_response_property_info_callback(),
    ),
  );
}

/**
 * Implements hook_rules_event_info().
 */
function commerce_bank_account_onfile_rules_event_info() {
  $events = array();
  $events['commerce_bank_account_onfile_charge_failed'] = array(
    'label' => t('After a failed attempt to charge an order'),
    'group' => t('Commerce Bank Account on File'),
    'access callback' => 'commerce_order_rules_access',
    'variables' => array(
      'bank_account_data' => array(
        'type' => 'commerce_bank_account_file',
        'label' => t('Bank Account'),
        'optional' => TRUE,
      ),
      'order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
      ),
      'charge' => array(
        'type' => 'commerce_price',
        'label' => t('Charge'),
        'optional' => TRUE,
      ),
      'response' => array(
        'type' => 'commerce_bank_account_onfile_charge_bank_account_response',
        'label' => t('charge Bank Account Response'),
      ),
    ),
  );
  $events['commerce_bank_account_onfile_charge_success'] = array(
    'label' => t('After a successful chargeing of an order (Bank Account on File)'),
    'group' => t('Commerce Bank Account on File'),
    'access callback' => 'commerce_order_rules_access',
    'variables' => array(
      'bank_account_data' => array(
        'type' => 'commerce_bank_account_file',
        'label' => t('Bank Account'),
      ),
      'order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
      ),
      'charge' => array(
        'type' => 'commerce_price',
        'label' => t('Charge'),
      ),
      'response' => array(
        'type' => 'commerce_bank_account_onfile_charge_bank_account_response',
        'label' => t('charge Bank Account Response'),
      ),
    ),
  );

  return $events;
}

/**
 * Implements hook_rules_action_info().
 */
function commerce_bank_account_onfile_rules_action_info() {
  $actions = array();
  $actions['commerce_bank_account_onfile_order_select_bank_account'] = array(
    'label' => t('Select bank account on file to charge'),
    'group' => t('Commerce Bank Account on File'),
    'parameter' => array(
      'order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
      ),
      'forced_instance_id' => array(
        'type' => 'text',
        'label' => t('Payment method instance ID'),
        'description' => t('If provided, then the instance ID must match that of the bank account data.'),
        'options list' => 'commerce_bank_account_onfile_rules_payment_instance_charge_options_list',
        'optional' => TRUE,
      ),
    ),
    'provides' => array(
      'select_bank_account_response' => array(
        'type' => 'commerce_bank_account_onfile_charge_bank_account_response',
        'label' => t('Select bank account response'),
        'save' => FALSE,
      ),
    ),
    'callbacks' => array(
      'execute' => 'commerce_bank_account_onfile_rules_action_order_select_bank_account',
    ),
  );

  $actions['commerce_bank_account_onfile_order_charge_bank_account'] = array(
    'label' => t('charge an order with a bank account on file'),
    'group' => t('Commerce Bank Account on File'),
    'parameter' => array(
      'order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
      ),
      'charge' => array(
        'type' => 'commerce_price',
        'label' => t('Charge'),
        'description' => t('The charge amount and currency. If not provided, then the order balance will be used.'),
        'optional' => TRUE,
      ),
      'select_bank_account_response' => array(
        'type' => 'commerce_bank_account_onfile_charge_bank_account_response',
        'label' => t('Select Bank Account Response'),
        'description' => t('If provided, Bank Account Data parameter can be omitted.'),
        'optional' => TRUE,
      ),
      'bank_account_data' => array(
        'type' => 'commerce_bank_account_file',
        'label' => t('Bank Account'),
        'description' => t('If provided, Select Bank Account Response will be ignored.'),
        'optional' => TRUE,
      ),
      'forced_instance_id' => array(
        'type' => 'text',
        'label' => t('Payment method instance id'),
        'description' => t('If provided and Bank Account Data is provided, then the instance id must match that of the bank account data.'),
        'options list' => 'commerce_bank_account_onfile_rules_payment_instance_charge_options_list',
        'optional' => TRUE,
      ),
    ),
    'provides' => array(
      'charge_bank_account_response' => array(
        'type' => 'commerce_bank_account_onfile_charge_bank_account_response',
        'label' => t('charge Bank Account Response'),
        'save' => FALSE,
      ),
    ),
    'callbacks' => array(
      'execute' => 'commerce_bank_account_onfile_rules_action_order_charge_bank_account',
    ),
  );


  return $actions;
}

/**
 * Options list for payment method instances that provide a charge callback
 */
function commerce_bank_account_onfile_rules_payment_instance_charge_options_list() {
  $options = array('' => t('- None -'));
  $implements = commerce_bank_account_file_payment_method_implements('charge callback');


  foreach ($implements as $method_id => $method_function) {
    $payment_method_instances = _commerce_bank_account_onfile_payment_method_instances($method_id, TRUE);
    if (empty($payment_method_instances)) {
      continue;
    }

    foreach ($payment_method_instances as $instance_id => $payment_method) {
      list($method_id_part, $rule_name) = explode('|', $instance_id);

      $options[$instance_id] = t('@title (rule: @rule)', array(
        '@title' => $payment_method['title'],
        '@rule' => $rule_name,
      ));
    }
  }

  return $options;
}

/**
 * Rules action callback for commerce_bank_account_onfile_order_select_bank_account
 */
function commerce_bank_account_onfile_rules_action_order_select_bank_account($order, $forced_instance_id = NULL) {

  $response = commerce_bank_account_onfile_order_select_bank_account($order, $forced_instance_id);

  return array('select_bank_account_response' => $response);
}

/**
 * Rules action callback for commerce_bank_account_onfile_order_charge_bank_account
 */
function commerce_bank_account_onfile_rules_action_order_charge_bank_account($order, $charge = NULL, $select_bank_account_response = NULL, $bank_account = NULL, $forced_instance_id = NULL) {
  // This action needs either the bank account data provided or a response returned by
  // the `Select bank account on file to charge` action.
  if (empty($select_bank_account_response) && empty($bank_account)) {
    // Return an error wrapped in a response if none of them are provided.
    $response = array(
      'code' => COMMERCE_BAOF_PROCESS_CODE_INSUFFICIENT_DATA,
      'message' => 'No chargeable bank account on file is provided for the order @order_id.',
    );
    rules_invoke_all('commerce_bank_account_onfile_charge_failed', NULL, $order, $charge, $response);
    return array('charge_bank_account_response' => $response);
  }

  // If provided, bank account data will be used regardless the response returned by
  // the `Select bank account on file to charge` action.
  if (empty($bank_account)) {
    // Bank account data is not provided, so let's use the response by the
    // aforementioned action. Make sure it found a chargeable bank account.
    if ($select_bank_account_response['code'] == COMMERCE_BAOF_PROCESS_CODE_BANK_ACCOUNT_OK) {
      $bank_account = $select_bank_account_response['bank_account_chosen'];
    }
    else {
      rules_invoke_all('commerce_bank_account_onfile_charge_failed', NULL, $order, $charge, $select_bank_account_response);
      return array('charge_bank_account_response' => $select_bank_account_response);
    }
  }

  $response = commerce_bank_account_onfile_order_charge_bank_account($order, $charge, $bank_account, $forced_instance_id);
  return array('charge_bank_account_response' => $response);
}
