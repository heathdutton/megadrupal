<?php
/**
 * @file uc_alpha_deltapay.pages.inc
 * alpha deltapay menu items.
 *
 */

/**
 * Actions to take when order gets completed.
 */
function uc_alpha_deltapay_complete() {
  $order_id = check_plain($_POST['Param1']);
  watchdog('uc_alpha_deltapay', 'Receiving new order notification for order !order_id.', array(
    '!order_id' => $order_id
  ));

  $order = uc_order_load($order_id);
  if ($order === FALSE
      || uc_order_status_data($order->order_status, 'state') != 'in_checkout') {
    drupal_set_message( t('An error has occurred during payment. Please contact us to ensure your order has submitted.'));
    drupal_goto(variable_get('uc_alpha_deltapay_cancel_return_url', 'cart'));
  }

  $payment_status = check_plain($_POST['Result']);
  $delta_pay_id = check_plain($_POST['DeltaPayId']);
  $payment_amount = check_plain($_POST['Charge']);
  $payment_amount = str_replace(',', '.', $payment_amount);
  $price = number_format($payment_amount, 2, '.', '');
  $payment_currency = check_plain($_POST['Currency'])
                     . check_plain($_POST['CurrencyCode']);
  $error_message = check_plain($_POST['ErrorMessage']);
  $installments = check_plain($_POST['Param2']);

  switch ($payment_status) {
      case 1: // successful transaction
        $comment = t('Deltapay transaction ID: @DeltaPayId', array(
          '@DeltaPayId' => $delta_pay_id));
        uc_payment_enter($order->order_id, 'alpha_deltapay', $payment_amount, $order->uid, NULL, $comment);
        uc_cart_complete_sale($order);
        uc_order_comment_save($order->order_id, 0, t('Payment of @amount @currency submitted through Alpha DeltaPay.', array('@amount' =>   $price , '@currency' => $payment_currency)), 'order', 'payment_received');
        uc_order_comment_save($order->order_id, 0, t('Alpha DeltaPay reported a payment of @amount @currency in @installments installments.', array('@amount' =>   $payment_amount , '@currency' => $payment_currency, '@installments' => $installments)));
        drupal_set_message(variable_get('uc_alpha_deltapay_complete_msg', ''));
        // This lets us know it's a legitimate access of the complete page.
        $_SESSION['uc_checkout'][$_SESSION['cart_order']]['do_complete'] = TRUE;
        drupal_goto('cart/checkout/complete');
      break;

      case 2: //error
        $message = variable_get('uc_alpha_deltapay_error_msg', '')
          . t('Alpha DeltaPay payment failed with following error message: @Error', array(
          '@Error' => $error_message
        ));
        uc_order_comment_save($order->order_id, 0, $message, 'admin');
        drupal_set_message($message . t('Please try again in a few moments.'));
        drupal_goto(variable_get('uc_alpha_deltapay_cancel_return_url', 'cart'));
      break;

      case 3: //user cancelled
        uc_order_comment_save($order->order_id, 0, t("The customer cancelled payment."), 'order', 'canceled' );
        drupal_set_message(t('Your Alpha DeltaPay payment was cancelled. Please feel free to continue shopping or contact us for assistance.'));
        unset($_SESSION['cart_order']);
        drupal_goto(variable_get('uc_alpha_deltapay_cancel_return_url', 'cart'));
      break;
  }
}

/**
 * Menu callback for the demo page.
 */
function uc_alpha_deltapay_demo($form, $form_state) {
  $form['Param1'] = array(
    '#type' => 'textfield',
    '#title' => t('Order ID'),
    '#description' => t('Make sure you use one of your test orders here.'),
    '#default_value' => 0,
  );
  $form['Param2'] = array(
    '#type' => 'select',
    '#title' => t('Installments'),
    '#options' => array(0, 1, 2, 3, 4, 5, 6),
  );
  $form['DeltaPayId'] = array(
    '#type' => 'textfield',
    '#title' => t('DeltaPayId'),
    '#default_value' => md5(time()),
  );
  $form['Result'] = array(
    '#type' => 'select',
    '#title' => t('Result'),
    '#options' => array(
      1 => t('Successful'),
      2 => t('Error'),
      3 => t('Cancelled'), 
    ),
  );
  $form['ErrorMessage'] = array(
    '#type' => 'textfield',
    '#title' => t('ErrorMessage'),
    '#default_value' => t('This is just a demo'),
  );
  $form['Charge'] = array(
    '#type' => 'textfield',
    '#title' => t('Charge'),
    '#description' => t('How much money to charge. Use comma(,) instead of point(.) for decimals'),
    '#default_value' => '25,50',
  );
  $form['CurrencyCode'] = array(
    '#type' => 'select',
    '#title' => t('Currency Code'),
    '#options' => array(
      '978' => 'EUR',
      '840' => 'USD',
    ),
  );
  $form['Currency'] = array(
    '#type' => 'select',
    '#title' => t('Currency'),
    '#options' => array(
      'EUR' => 'EUR',
      'USD' => 'USD',
    ),
  );
  $form['Guid2'] = array(
    '#type' => 'textfield',
    '#title' => t('Guid2'),
    '#default_value' => md5(time()),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  $form['#action'] = url('cart/alphadeltapay/complete');
  return $form;
}
