<?php

/**
 * Defines a handler area that display a refund button. If user press on that
 * button, a refund terminal will be displayed in order to fully/partially
 * reimburse the order.
 *
 * Only works with an order payed through FuturePay services.
 */
class commerce_futurepay_handler_area_refund extends views_handler_area {
  /**
   * @param bool $empty
   *   Boolean indicating if the view results are empty or not.
   *
   * @return string
   *   Return a rendered form to display.
   */
  function render($empty = FALSE) {
    // Load an order object for the View if a single order argument is present.
    if (in_array('order_id', array_keys($this->view->argument)) &&
      !in_array('order_id_1', array_keys($this->view->argument)) &&
      !empty($this->view->args[$this->view->argument['order_id']->position])
    ) {

      // Load the specified order.
      $order = commerce_order_load($this->view->args[$this->view->argument['order_id']->position]);

      // Don't render the refund button if the operation is not available.
      $total_refund = commerce_futurepay_order_refund_balance($order->order_id);
      if ($total_refund['amount'] >= entity_metadata_wrapper('commerce_order', $order)->commerce_order_total->amount->value()) {
        return FALSE;
      }
    }
    else {
      // Otherwise indicate a valid order is not present.
      $order = FALSE;
    }

    // Prevent display of this handler if the FuturePay payment method is not
    // attached to given order.
    if (
      (isset($order->data['commerce_payment_order_paid_in_full_invoked']) &&
        $order->data['commerce_payment_order_paid_in_full_invoked'] !== TRUE) ||
      (isset($order->data['payment_method']) &&
        $order->data['payment_method'] !== 'futurepay|commerce_payment_futurepay')
    ) {
      return FALSE;
    }

    // Build and render the form to add a payment if the View contains a valid.
    module_load_include('inc', 'commerce_futurepay', 'includes/commerce_futurepay.forms');

    return ($order) ? drupal_render(drupal_get_form('commerce_futurepay_add_refund_form', $order)) : NULL;
  }
}
