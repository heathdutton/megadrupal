<?php
/**
 * @file
 *   Commerce Xero views integration.
 */


/**
 * Implements hook_views_data().
 */
function commerce_xero_views_data() {

  $data['commerce_xero_bankaccount']['table']['group'] = t('Xero bank account');
  $data['commerce_xero_bankaccount']['table']['base'] = array(
    'field' => 'id',
    'title' => t('Xero bank account'),
    'help' => t('Display information regarding xero bank accounts and commerce payment methods.'),
    'weight' => 10,
  );

  $data['commerce_xero_bankaccount']['id'] = array(
    'title' => t('Bank Account Id'),
    'help' => t('The primary identifier for a Drupal Xero Bank Account.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
  );

  $data['commerce_xero_bankaccount']['bank_account'] = array(
    'title' => t('Bank Account Code'),
    'help' => t('The bank account code.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_many_to_one',
      'options callback' => 'commerce_xero_bank_account_options',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
  );

  $data['commerce_xero_bankaccount']['revenue_code'] = array(
    'title' => t('Account Code'),
    'help' => t('The account code to post the transaction to.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_many_to_one',
      'options callback' => 'commerce_xero_revenue_account_options',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
  );

  $data['commerce_xero_bankaccount']['payment_method'] = array(
    'title' => t('Payment method'),
    'help' => t('The Drupal commerce payment method.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_many_to_one',
      'options callback' => '',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
  );

  $data['commerce_xero_bankaccount']['transaction_type'] = array(
    'title' => t('Transaction type'),
    'help' => t('The Xero transaction type associated with this account.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
  );

  $data['commerce_xero_bankaccount']['tracking_category'] = array(
    'title' => t('Tracking category'),
    'help' => t('The Xero tracking category associated with this account.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
  );


  return $data;
}
