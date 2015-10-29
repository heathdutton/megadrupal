<?php
/**
 * @file
 * Views hooks and callback registries.
 */

/**
 * Implements hook_views_data().
 */
function uc_worldpay_views_data() {
  // Describe WorldPay table.
  $data['uc_payment_worldpay']['table']['group'] = t('Order payments');
  $data['uc_payment_worldpay']['table']['join'] = array(
    'uc_orders' => array(
      'left_field' => 'order_id',
      'field' => 'order_id',
    ),
  );

  $data['uc_payment_worldpay']['transid'] = array(
    'title' => t('transid'),
    'help' => t('The transaction ID from WorldPay.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
  );

  $data['uc_payment_worldpay']['cardtype'] = array(
    'title' => t('cardtype'),
    'help' => t('The type of payment method used by the shopper.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
  );

  /*
  // @todo Look for a nice way to convert it to a regular PHP timestamp.
  $data['uc_payment_worldpay']['transtime'] = array(
    'title' => t('transtime'),
    'help' => t('Time of the transaction at WorldPay.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
  );
  */

  return $data;
}

