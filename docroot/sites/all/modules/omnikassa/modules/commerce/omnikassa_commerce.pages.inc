<?php
/**
 * @file
 * Page callbacks.
 */

/**
 * Report callback.
 */
function omnikassa_commerce_report($order) {
  $orderlink = l(t('View order'), 'admin/commerce/orders/' . $order->order_id);

  watchdog('omnikassa_commerce', '<p>Incoming Rabobank OmniKassa report message for order %orderid.</p><p>Data:</p><pre>!data</pre>', array('%orderid' => $order->order_id, '!data' => print_r($_REQUEST, TRUE)), WATCHDOG_DEBUG, $orderlink);

  if (omnikassa_commerce_handle_response($_REQUEST, $order)) {
    commerce_payment_redirect_pane_next_page($order, t('Rabobank OmniKassa automatic response'));
    echo 'OK';
  }
  else {
    echo 'Failure';
  }
}