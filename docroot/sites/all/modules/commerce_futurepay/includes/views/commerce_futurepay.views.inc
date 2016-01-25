<?php

/**
 * Export the refund functionality to commerce payment view.
 */

/**
 * Implements hook_views_data_alter().
 */
function commerce_futurepay_views_data_alter(&$data) {
  $data['commerce_payment_transaction']['fp_refund'] = array(
    'title' => t('FuturePay refund'),
    'help' => t('Display a refund button to reimburse customers.'),
    'area' => array(
      'handler' => 'commerce_futurepay_handler_area_refund',
    ),
  );
}
