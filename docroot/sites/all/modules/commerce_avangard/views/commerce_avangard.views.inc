<?php

/**
 * Implements hook_views_data().
 */
function commerce_avangard_views_data() {
  $data = array();

  $data['commerce_payment_transaction']['refund_avangard_transaction'] = array(
    'field' => array(
      'title' => t('Refund link'),
      'help' => t('Provide a simple link to refund Avangard the payment transaction.'),
      'handler' => 'commerce_avangard_handler_field_avangard_transaction_link_refund',
    ),
  );

  return $data;
}
