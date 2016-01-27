<?php

/**
 * @file
 * Hooks provided by the kkb_epay module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Receive payment, mark orders as payed, provide access to services.
 *
 * This hook is called only when the processing center sends a correct
 * confirmation about completed payment.
 *
 * @warning
 *   Under certain conditions, site can receive more than one notification
 *   about successful payment of the same order. It is responsibility of your
 *   code to check that this is the first notification about an order.
 *   If this is NOT the first notification, and the order is already marked as
 *   payed, ignore the notification and do not perform any actions.
 */
function hook_kkb_epay_payment(KkbEpay_PaymentNotification $notification) {
  $order = my_module_load_order($notification->getOrderId());
  if (!$order) {
    // Order not found. This is a very exceptional situation, but is still
    // worth checking.
    return;
  }
  if ($order->payed) {
    // Order is already payed, notification was repeated. Return early because
    // we do not want to send two confirmation emails.
    return;
  }

  db_update('my_orders')
    ->fields(array('payed' => 1))
    ->condition('order_id', $notification->getOrderId())
    ->execute();

  my_module_send_payment_confirmation($order);
}

/**
 * Process raw response from the processing center.
 *
 * This hook is called together with hook_kkb_epay_payment(), but receives
 * as an arguments a raw response document, as it was sent by the processing
 * center, as a string.
 */
function hook_kkb_epay_raw_success_response($document) {
  db_insert('my_response_log')
    ->fields(array(
      'response_document' => $document,
      'timestamp' => REQUEST_TIME,
    ))
    ->execute();
}

/**
 * Build success landing page for returning to site after payment.
 *
 * Modules should only display status messages or redirect users in this hook.
 * It is very important not to modify order statuses in this hook; it should
 * be done in hook_kkb_epay_payment().
 *
 * Just because this page is displayed, it does not mean that the payment
 * was actually done. User can access this page by simply entering its
 * URL in a browser.
 *
 * @param string $id
 *   When user is redirected to this page by the processing center, it
 *   will contain ID of the payed order. But this, is fact, can be an arbitrary
 *   string set by a user. Be careful with it!
 */
function hook_kkb_epay_success_page($id) {
  $page = array();

  $page['text'] = array(
    '#markup' => '<p>Welcome back! Here, some cookies for you.</p>',
  );

  return $page;
}

/**
 * Build failure landing page for returning to site after payment.
 *
 * Modules should only display status messages or redirect users in this hook.
 * It is very important not to modify order statuses in this hook.
 *
 * Just because this page is displayed, it does not mean that the payment
 * has actually failed. User can access this page by simply entering its
 * URL in a browser.
 *
 * @param string $id
 *   When user is redirected to this page by the processing center, it
 *   will contain ID of the failed order. But this, is fact, can be an
 *   arbitrary string set by a user. Be careful with it!
 */
function hook_kkb_epay_failure_page($id) {
  $page = array();

  $page['text'] = array(
    '#markup' => '<p>Payment is not completed. No cookies for you.</p>',
  );

  return $page;
}

/**
 * Modify billing statement form that will be sent to the processing center.
 *
 * The main reason to modify this form is to change title or style of the
 * submit button, or to set custom landing pages.
 *
 * When altering the form, it is *very* important not to change any values
 * of the existing fields and the #action parameter.
 *
 * You can access original KkbEpay_Order object in the $form['#order']
 * parameter, and the user's account in the $form['#account'] parameter.
 *
 * If you change landing pages, make sure to generate absolute URLs.
 */
function hook_form_kkb_epay_billing_statement_alter(&$form, &$form_state) {
  $order = $form['#order'];
  $order_id_hash = my_module_hash_order_id($order->getOrderId());

  $form['BackLink'] = url(
    'my-module/epay-customized-success/' . $order_id_hash,
    array('absolute' => TRUE)
  );

  if (empty($form['actions']['submit']['#attributes'])) {
    $form['actions']['submit']['#attributes'] = array();
  }
  $form['actions']['submit']['#attributes']['class'] = array('special-style');
}

/**
 * @} End of "addtogroup hooks".
 */
