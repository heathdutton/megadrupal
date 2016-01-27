<?php

/**
 * Implements hook_views_data()
 */
function dibs_views_data() {
  $data['dibs_transactions']['table']['group'] = t('Dibs Transactions');

  $data['dibs_transactions']['table']['base'] = array(
    'field' => 'tid',
    'title' => t('Dibs Transactions'),
    'help' => t('Exposed Dibs Transactions.'),
  );

  $data['dibs_transactions']['table']['join'] = array(
    'commerce_payment_transaction' => array(
      'left_field' => 'remote_id',
      'field' => 'tid',
    ),
  );

  $data['dibs_transactions']['tid'] = array(
    'title' => t('Dibs Transaction ID'),
    'help' => t('Dibs Transaction ID from Dibs Transactions table'),
    'relationship' => array(
      'base' => 'commerce_payment_transaction', // The name of the table to join with.
      'base field' => 'remote_id', // The name of the field on the joined table.
      'handler' => 'views_handler_relationship',
      'label' => t('Commerce relationship to Dibs Transactions'),
      'title' => t('Commerce relationship to Dibs Transactions'),
    ),
  );

  $data['dibs_transactions']['payment_order_id'] = array(
    'title' => t('Payment Order ID'),
    'help' => t('Payment Order ID from Dibs Transactions table.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['dibs_transactions']['payment_transaction_id'] = array(
    'title' => t('Payment Transaction ID'),
    'help' => t('Payment Transaction ID from Dibs Transactions table.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
     ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  return $data;
}
