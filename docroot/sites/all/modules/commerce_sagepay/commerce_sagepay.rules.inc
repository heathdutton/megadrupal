<?php

/**
 * @file
 * Implements new rules events actions and conditions.
 */

/**
 * Implements hook_rules_event_info().
 */
function commerce_sagepay_rules_event_info() {
  $events = array();

  $events['commerce_sagepay_prepare_transaction'] = array(
    'label' => t('Prepare the transaction for sending to Sage Pay.'),
    'group' => t('Commerce SagePay'),
    'variables' => array(
      'transaction' => array(
        'label' => t('SagePay Transaction Array'),
        'type' => 'array',
      ),
      'commerce_order' => array(
        'label' => t('Order', array(), array('context' => 'a drupal commerce order')),
        'type' => 'commerce_order',
        'skip save' => TRUE,
      ),
    ),
    'access callback' => 'commerce_order_rules_access',
  );

  return $events;
}

/**
 * Implements hook_rules_action_info().
 */
function commerce_sagepay_rules_action_info() {
  $actions = array();

  $actions['commerce_sagepay_set_transaction_account'] = array(
      'label' => t('Set the account for this transaction.'),
      'parameter' => array(
        'commerce_order' => array(
          'type' => 'commerce_order',
          'label' => t('Order'),
        ),
        'account_type' => array(
          'type' => 'text',
          'label' => t('Account Type'),
          'options list' => 'commerce_sagepay_account_type_list',
        ),
      ),
      'callbacks' => array(
        'execute' => 'commerce_sagepay_update_order_account',
      ),
      'group' => t('Commerce SagePay'),
    );

  $actions['commerce_sagepay_set_transaction_type'] = array(
    'label' => t('Set the transaction type for this transaction.'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
      ),
      'transaction_type' => array(
        'type' => 'text',
        'label' => t('Transaction Type'),
        'options list' => 'commerce_sagepay_transaction_type_list',
      ),
    ),
    'callbacks' => array(
      'execute' => 'commerce_sagepay_update_order_transaction_type',
    ),
    'group' => t('Commerce SagePay'),
  );

  $actions['commerce_sagepay_set_vendor'] = array(
    'label' => t('Set the vendor name for this transaction.'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
      ),
      'vendor_name' => array(
        'type' => 'text',
        'label' => t('Vendor Name'),
      ),
    ),
    'callbacks' => array(
      'execute' => 'commerce_sagepay_update_order_vendor_name',
    ),
    'group' => t('Commerce SagePay'),
  );

  $actions['commerce_sagepay_set_apply_avscv2'] = array(
    'label' => t('Set which AVS / CV2 checks are applied for this transaction.'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
      ),
      'avscv2' => array(
        'type' => 'text',
        'label' => t('AVS CV2 Checks'),
        'options list' => 'commerce_sagepay_avs_flag_list',
      ),
    ),
    'callbacks' => array(
      'execute' => 'commerce_sagepay_update_order_avs_flag',
    ),
    'group' => t('Commerce SagePay'),
  );

  $actions['commerce_sagepay_set_apply_3ds'] = array(
    'label' => t('Set which 3D Secure checks are applied for this transaction.'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
      ),
      'avscv2' => array(
        'type' => 'text',
        'label' => t('3D Secure Checks'),
        'options list' => 'commerce_sagepay_3ds_flag_list',
      ),
    ),
    'callbacks' => array(
      'execute' => 'commerce_sagepay_update_order_3ds_flag',
    ),
    'group' => t('Commerce SagePay'),
  );

  $actions['commerce_sagepay_request_token'] = array(
    'label' => t('Request a token when creating the transaction (if available)
    .'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
      ),
      'token' => array(
        'type' => 'text',
        'label' => t('Request Token'),
        'options list' => 'commerce_sagepay_request_token_list',
      ),
    ),
    'callbacks' => array(
      'execute' => 'commerce_sagepay_update_order_token_flag',
    ),
    'group' => t('Commerce SagePay'),
  );

  $actions['commerce_sagepay_remove_overrides'] = array(
    'label' => t('Removes transaction overrides set by previous rules.'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
      ),
    ),
    'callbacks' => array(
      'execute' => 'commerce_sagepay_remove_overrides',
    ),
    'group' => t('Commerce SagePay'),
  );

  $actions['commerce_sagepay_repeat_charge_card'] = array(
    'label' => t('Charge an order using a repeat payment'),
    'group' => t('Commerce SagePay'),
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
    ),
    'provides' => array(
      'charge_card_response' => array(
        'type' => 'commerce_cardonfile_charge_card_response',
        'label' => t('Charge Card Response'),
        'save' => FALSE,
      ),
    ),
    'callbacks' => array(
      'execute' => 'commerce_sagepay_rules_action_repeat_charge_card',
    ),
  );

  $actions['commerce_sagepay_cancel_order_transaction'] = array(
    'label' => t('Cancel Order Transactions'),
    'parameter' => array(
      'order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
      ),
      'transaction_type' => array(
        'type' => 'text',
        'label' => t('Transaction Type'),
        'options list' => 'commerce_sagepay_transaction_type_list',
      ),
    ),
    'callbacks' => array(
      'execute' => 'commerce_sagepay_cancel_order_transaction',
    ),
    'group' => t('Commerce SagePay'),
  );

  $actions['commerce_sagepay_abort_order_transaction'] = array(
    'label' => t('Abort Order Transactions'),
    'parameter' => array(
      'order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
      ),
      'transaction_type' => array(
        'type' => 'text',
        'label' => t('Transaction Type'),
        'options list' => 'commerce_sagepay_transaction_type_list',
      ),
    ),
    'callbacks' => array(
      'execute' => 'commerce_sagepay_abort_order_transaction',
    ),
    'group' => t('Commerce SagePay'),
  );

  $actions['commerce_sagepay_release_order_transaction'] = array(
    'label' => t('Release Order Transactions'),
    'parameter' => array(
      'order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
      ),
      'transaction_type' => array(
        'type' => 'text',
        'label' => t('Transaction Type'),
        'options list' => 'commerce_sagepay_transaction_type_list',
      ),
    ),
    'callbacks' => array(
      'execute' => 'commerce_sagepay_release_order_transaction',
    ),
    'group' => t('Commerce SagePay'),
  );

  return $actions;
}

/**
 * Set an override parameter in the order for account type.
 *
 * Store an override for the AccountType parameter for the Sage Pay
 * transaction.
 *
 * @param $order
 * @param $account_type
 */
function commerce_sagepay_update_order_account($order,
                                               $account_type) {

  $sagepay_overrides = isset($order->data['sagepay_overrides']) ?
    $order->data['sagepay_overrides'] : array();
  $sagepay_overrides['AccountType'] = $account_type;
  $order->data['sagepay_overrides'] = $sagepay_overrides;
  commerce_order_save($order);
}

/**
 * Set an override parameter in the order for Transaction Type.
 *
 * Store an override for the TxType parameter for the Sage Pay transaction.
 *
 * @param $order
 * @param $transaction_type
 */
function commerce_sagepay_update_order_transaction_type($order,
                                               $transaction_type) {

  $sagepay_overrides = isset($order->data['sagepay_overrides']) ?
    $order->data['sagepay_overrides'] : array();
  $sagepay_overrides['TxType'] = $transaction_type;
  $order->data['sagepay_overrides'] = $sagepay_overrides;
  commerce_order_save($order);
}

/**
 * Set an override parameter in the order for Vendor Name.
 *
 * Store an override for the Name parameter for the Sage Pay transaction.
 *
 * @param $order
 * @param $vendor_name
 */
function commerce_sagepay_update_order_vendor_name($order,
                                                        $vendor_name) {

  $sagepay_overrides = isset($order->data['sagepay_overrides']) ?
    $order->data['sagepay_overrides'] : array();
  $sagepay_overrides['Vendor'] = $vendor_name;
  $order->data['sagepay_overrides'] = $sagepay_overrides;
  commerce_order_save($order);
}

/**
 * Set an override parameter in the order for ApplyAVSCV2.
 *
 * Store an override for the ApplyAVSCV2 parameter for the Sage Pay transaction.
 *
 * @param $order
 * @param $flag
 */
function commerce_sagepay_update_order_avs_flag($order,
                                                   $flag) {

  $sagepay_overrides = isset($order->data['sagepay_overrides']) ?
    $order->data['sagepay_overrides'] : array();
  $sagepay_overrides['ApplyAVSCV2'] = $flag;
  $order->data['sagepay_overrides'] = $sagepay_overrides;
  commerce_order_save($order);
}

/**
 * Set an override parameter in the order for Apply3DSecure.
 *
 * Store an override for the Apply3DSecure parameter for the Sage Pay transaction.
 *
 * @param $order
 * @param $flag
 */
function commerce_sagepay_update_order_3ds_flag($order,
                                                $flag) {

  $sagepay_overrides = isset($order->data['sagepay_overrides']) ?
    $order->data['sagepay_overrides'] : array();
  $sagepay_overrides['Apply3DSecure'] = $flag;
  $order->data['sagepay_overrides'] = $sagepay_overrides;
  commerce_order_save($order);
}

/**
 * Set an override parameter in the order for Token.
 *
 * Store an override for the Token parameter for the Sage Pay transaction.
 *
 * @param $order
 * @param $flag
 */
function commerce_sagepay_update_order_token_flag($order,
                                                  $flag) {

  $sagepay_overrides = isset($order->data['sagepay_overrides']) ?
    $order->data['sagepay_overrides'] : array();
  $sagepay_overrides['CreateToken'] = $flag;
  $order->data['sagepay_overrides'] = $sagepay_overrides;
  commerce_order_save($order);
}

/**
 * Remove any existing override data stored in an order.
 *
 * @param $order
 */
function commerce_sagepay_remove_overrides($order) {
  unset($order->data['sagepay_overrides']);
  commerce_order_save($order);
}

/**
 * Return a list of supported account types.
 *
 * @return array
 */
function commerce_sagepay_account_type_list() {
  return array(
    'E' => t('e-commerce merchant account (default).'),
    'C' => t('Continuous authority merchant account (if present).'),
    'M' => t('Mail order, telephone order account (if present).'),
  );
}

/**
 * Return a list of supported Transaction types.
 *
 * @return array
 */
function commerce_sagepay_transaction_type_list() {
  return array(
    'PAYMENT' => t('Payment'),
    'DEFERRED' => t('Deferred'),
    'AUTHENTICATE' => t('Authenticate'),
  );
}

/**
 * Return a list of supported AVS Flag options.
 *
 * @return array
 */
function commerce_sagepay_avs_flag_list() {
  return array(
    '0' => t('If AVS/CV2 enabled then check them. If rules apply, use rules. (default)'),
    '1' => t('Force AVS/CV2 checks even if not enabled for the account. If rules apply, use rules.'),
    '2' => t('Force NO AVS/CV2 checks even if enabled on account.'),
    '3' => t('Force AVS/CV2 checks even if not enabled for the account but DO NOT apply any rules.'),
  );
}

/**
 * Return a list of supported 3d secure flag options.
 *
 * @return array
 */
function commerce_sagepay_3ds_flag_list() {
  return array(
    '0' => t('If 3D-Secure checks are possible and rules allow, perform the checks and apply the authorisation rules. (default)'),
    '1' => t('Force 3D-Secure checks for this transaction if possible and apply rules for authorisation.'),
    '2' => t('Do not perform 3D-Secure checks for this transaction and always authorise.'),
    '3' => t('Force 3D-Secure checks for this transaction if possible but ALWAYS obtain an auth code, irrespective of rule base.'),
  );
}

/**
 * Return a list of supported Token options.
 *
 * @return array
 */
function commerce_sagepay_request_token_list() {
  return array(
    '0' => t('No token is required (default)'),
    '1' => t('Request a token if the account supports it and card on file is
    active.'),
  );
}

/**
 * Rules action callback for commerce_sagepay_repeat_charge_card
 */
function commerce_sagepay_rules_action_repeat_charge_card($order,
                                                        $charge = NULL) {

  module_load_include('inc', 'commerce_sagepay',
    'includes/commerce_sagepay_repeat');
  $response = commerce_sagepay_repeat_process_transaction($order, $charge);
  return array('charge_card_response' => $response);
}

/**
 * Rules action callback for commerce_sagepay_cancel_transaction action
 */
function commerce_sagepay_cancel_order_transaction($order, $type=FALSE) {
  module_load_include('inc', 'commerce_sagepay', 'includes/commerce_sagepay_cancel');

  $conditions = array('order_id' => $order->order_id);
  $transactions = commerce_payment_transaction_load_multiple(array(), $conditions);
  if (!empty($transactions)) {
    foreach ($transactions as $transaction) {
      if (!empty($type)) {
        if ($type == $transaction->remote_status) {
          commerce_sagepay_cancel_transaction($transaction);
        }
      }
    }
  }
}

/**
 * Rules action callback for commerce_sagepay_abort_transaction action
 */
function commerce_sagepay_abort_order_transaction($order, $type=FALSE) {
  module_load_include('inc', 'commerce_sagepay', 'includes/commerce_sagepay_abort');

  $conditions = array('order_id' => $order->order_id);
  $transactions = commerce_payment_transaction_load_multiple(array(), $conditions);
  if (!empty($transactions)) {
    foreach ($transactions as $transaction) {
      if (!empty($type)) {
        if ($type == $transaction->remote_status) {
          commerce_sagepay_abort_transaction($transaction);
        }
      }
    }
  }
}

/**
 * Rules action callback for commerce_sagepay_release_transaction action
 */
function commerce_sagepay_release_order_transaction($order, $type=FALSE) {
  module_load_include('inc', 'commerce_sagepay', 'includes/commerce_sagepay_release');

  $conditions = array('order_id' => $order->order_id);
  $transactions = commerce_payment_transaction_load_multiple(array(), $conditions);
  if (!empty($transactions)) {
    foreach ($transactions as $transaction) {
      if (!empty($type)) {
        if ($type == $transaction->remote_status) {
          $amount = commerce_currency_amount_to_decimal($transaction->amount, $transaction->currency_code);
          commerce_sagepay_release_transaction($transaction, $amount);
        }
      }
    }
  }
}
