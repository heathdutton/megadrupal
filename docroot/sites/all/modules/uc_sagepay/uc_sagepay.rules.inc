<?php

/**
 * Implements hook_rules_data_info().
 */
function uc_sagepay_rules_data_info() {


  return array(
    'uc_sagepay_transaction' => array(
      'label' => t('SagePay transaction data'),
      'wrap' => TRUE,
      'property info' => array(
        'Apply3DSecure' => array(
          'type' => 'integer',
          'label' => t('3D-Secure flag'),
          'description' => t('Whether 3D-Secure checks should be performed'),
          'setter callback' => 'entity_property_verbatim_set',
          'options list' => 'uc_sagepay_transaction_3dsecure_options',
        ),
        'TxType' => array(
          'type' => 'text',
          'label' => t('Transaction Types'),
          'description' => t('Sagepy supported transaction types.'),
          'setter callback' => 'entity_property_verbatim_set',
          'options list' => 'uc_sagepay_transaction_txtype_options',
        ),
        'AccountType' => array(
          'type' => 'text',
          'label' => t('Account type'),
          'description' => t('The merchant account to be used.'),
          'setter callback' => 'entity_property_verbatim_set',
          'options list' => 'uc_sagepay_transaction_account_type_options',
        ),
      ),
    ),
  );
}

/**
 * Implements hook_rules_event_info().
 */
function uc_sagepay_rules_event_info() {
  return array(
    'uc_sagepay_transaction' => array(
      'label' => t('Before sending a Sage Pay Direct transaction'),
      'group' => t('Payment'),
      'variables' => array(
        'uc_sagepay_transaction' => array(
          'label' => t('SagePay transaction data'),
          'type' => 'uc_sagepay_transaction',
        ),
        'order' => array(
          'label' => t('Order'),
          'type' => 'uc_order',
        ),
      ),
    ),
  );
}

/**
 * Implements hook_rules_condition_info().
 */
function uc_sagepay_rules_condition_info() {
  return array(
    'uc_sagepay_condition_path_is_admin' => array(
      'label' => t('Path is admin page'),
      'group' => t('Path'),
      'parameter' => array(
        'path' => array(
          'type' => 'text',
          'label' => t('Path'),
          'description' => t('Specifies the path you wish to check.'),
        ),
      ),
      'base' => 'path_is_admin',
    ),
  );
}

/**
 * Returns the options list for applying 3D-Secure checks.
 */
function uc_sagepay_transaction_3dsecure_options() {
  return array(
    0 => t('Default'),
    1 => t('Force 3D-Secure checks'),
    2 => t('Do not perform 3D-Secure checks'),
    3 => t('Force 3D-Secure checks but ALWAYS obtain an auth code'),
  );
}

/**
 * Returns the options list for Sage Pay merchant account types.
 */
function uc_sagepay_transaction_account_type_options() {
  return array(
    'E' => t('E-commerce account'),
    'M' => t('Mail order/telephone order account'),
    'C' => t('Continuous authority account'),
  );
}

/**
 * Returns the options list for Sage Pay transaction types.
 */
function uc_sagepay_transaction_txtype_options() {
  $types = array(
    'AUTHENTICATE' => t('Authorization only (AUTHENTICATE)'),
    'PAYMENT' => t('Authorize and capture immediately (PAYMENT)'),
    'DEFERRED' => t('Deferred transaction (DEFERRED)'),
  );

  return $types;
}
