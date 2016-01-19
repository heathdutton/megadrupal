<?php

function commerce_avangard_checkout_response($order) {
  $result_code = isset($_GET['result_code']) ? $_GET['result_code'] : '';

  $redirect_url = '/cart';
  if (!$order) {
    drupal_set_message(t('The order does not exist.'), 'error');
  }
  elseif (in_array($result_code, $order->data['commerce_avangard']['ok_codes'])) {
    if (isset($order->data['commerce_avangard']['tickets_map'][$result_code])) {
      $save_result = _commerce_avangard_create_debit_transaction($order, $result_code);

      if ($save_result) {
        commerce_payment_redirect_pane_next_page($order);

        drupal_set_message(t('Payment successfull'), 'status');
        $redirect_url = commerce_checkout_order_uri($order);
      }
      else {
        drupal_set_message(t('Something went wrong. Please contact us to check if payment was successful.'), 'warning');
      }
    }
    else {
      watchdog('commerce_avangard', "Ticket not found for correct 'ok_code'. Porbably double-returned from bank terminal: %order_id", array('%order_id' => $order->order_id), WATCHDOG_INFO);
    }
  }
  elseif ($result_code === '' || in_array($result_code, $order->data['commerce_avangard']['failure_codes'])) {
    // Workarounding commerce_payment losing messages on redirect.
    $order->data['commerce_avangard']['last_error_message'] = t('Payment canceled');
    $order->revision = TRUE;
    $order->log = t('Payment cancelled by unknown reason');
    commerce_order_save($order);

    drupal_set_message(t('Payment canceled'), 'error');
    $redirect_url = _commerce_avangard_get_checkout_payment_back_url($order);
  }

  drupal_goto($redirect_url);
}

/**
 * Redirects user after trey returns from payment gate.
 * 
 * @param object $order
 */
function commerce_avangard_payment_back($order) {
  drupal_goto(_commerce_avangard_get_checkout_payment_back_url($order));
}

function commerce_avangard_order_add_payment_form($form, &$form_state, $user, $order) {
  $form_state['order'] = $order;

  $balance = commerce_payment_order_balance($order);
  if ($balance['amount'] <= 0) {
    drupal_set_message(t('Order does not require additional payments'), 'info');
    drupal_goto("/user/{$user->uid}/orders/{$order->order_id}");
    return;
  }

  $back_url = url("/user/{$user->uid}/orders/{$order->order_id}/avangard_payment_added", array('absolute' => TRUE));
  $form = _commerce_avangard_payment_redirect_form($back_url);

  $message = t("Add %amount payment to your order", array(
    '%amount' => commerce_currency_format($balance['amount'], $balance['currency_code']),
  ));
  $form['message'] = array(
    '#markup' => "<div class='avangard-add-payment-message'>{$message}</div>",
    '#weight' => -10,
  );

  return $form;
}

/**
 * Processes prder after payment has been added.
 *
 * Creates order debit transaction and redirects user to the next page.
 * 
 * @param object $user
 * @param object $order
 */
function commerce_avangard_add_payment_response($user, $order) {
  $result_code = isset($_GET['result_code']) ? $_GET['result_code'] : '';

  $redirect_url = "/user/{$user->uid}/orders/{$order->order_id}";
  if (in_array($result_code, $order->data['commerce_avangard']['ok_codes'])) {
    if (isset($order->data['commerce_avangard']['tickets_map'][$result_code])) {
      $save_result = _commerce_avangard_create_debit_transaction($order, $result_code);

      if ($save_result) {
        drupal_set_message(t('Payment successfull'), 'status');
      }
      else {
        drupal_set_message(t('Something went wrong. Please contact us to check if payment was successful.'), 'warning');
      }
    }
    else {
      watchdog('commerce_avangard', "Ticket not found for correct 'ok_code'. Porbably double-returned from bank terminal: %order_id", array('%order_id' => $order->order_id), WATCHDOG_INFO);
    }
  }
  elseif ($result_code === '' || in_array($result_code, $order->data['commerce_avangard']['failure_codes'])) {
    $order->data['commerce_avangard']['last_error_message'] = t('Payment canceled');
    $order->revision = TRUE;
    $order->log = t('Payment cancelled by unknown reason');
    commerce_order_save($order);

    drupal_set_message(t('Payment canceled'), 'error');
  }

  drupal_goto($redirect_url);
}
