<?php

/**
 * @file
 * Views integration.
 */

/**
 * Implements hook_views_data().
 */
function payment_commerce_views_data() {
  $data['payment_commerce'] = array(
    'table' => array(
      'base' => array(
        'title' => t('Payments and Commerce transactions'),
      ),
      'group' => t('Payment'),
      'join' => array(
        'payment' => array(
          'left_field' => 'pid',
          'field' => 'pid',
        ),
        'commerce_payment_transaction' => array(
          'left_field' => 'transaction_id',
          'field' => 'transaction_id',
        ),
      ),
    ),
  );

  return $data;
}

/**
 * Implements hook_views_data_alter().
 */
function payment_commerce_views_data_alter(&$data) {
  $data['payment']['table']['join']['commerce_payment_transaction'] = array(
    'left_table' => 'payment_commerce',
    'left_field' => 'pid',
    'field' => 'pid',
  );
  $data['commerce_payment_transaction']['table']['join']['payment'] = array(
    'left_table' => 'payment_commerce',
    'left_field' => 'transaction_id',
    'field' => 'transaction_id',
  );
}