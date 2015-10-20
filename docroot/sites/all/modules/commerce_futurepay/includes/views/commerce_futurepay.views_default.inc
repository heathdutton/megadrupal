<?php

/**
 * @file
 * Default views for commerce_futurepay module.
 */

/**
 * Implements hook_views_default_views_alter().
 */
function commerce_futurepay_views_default_views_alter(&$views) {
  // Add the refund area on the payment view.
  if (isset($views['commerce_payment_order'])) {
    // Get handler object.
    $handler = $views['commerce_payment_order']->display['default']->handler;

    /* Footer: Commerce Payment Transaction: FuturePay refund */
    $handler->display->display_options['footer']['fp_refund']['id'] = 'fp_refund';
    $handler->display->display_options['footer']['fp_refund']['table'] = 'commerce_payment_transaction';
    $handler->display->display_options['footer']['fp_refund']['field'] = 'fp_refund';
  }
}
