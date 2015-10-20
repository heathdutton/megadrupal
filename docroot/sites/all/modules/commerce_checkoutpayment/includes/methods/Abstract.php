<?php

abstract class methods_Abstract {

  /**
   * Payment method callback: checkout form.
   */
  abstract public function submitForm($payment_method, $pane_values, $checkout_pane, $order);

  /**
   * Payment method callback: checkout form submission.
   */
  public function submitFormCharge($payment_method, $pane_form, $pane_values, $order, $charge) {

    $config = array();
    $shipping_array = array();
    $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
    $billing_address = $order_wrapper->commerce_customer_billing->commerce_customer_address->value();
    $order_array = $order_wrapper->commerce_order_total->value();
    $default_currency = commerce_default_currency();
    $amount_cents = number_format(commerce_currency_convert($charge['amount'], $order_array['currency_code'], $default_currency), 0, '', '');
    $config['authorization'] = $payment_method['settings']['private_key'];
    $config['mode'] = $payment_method['settings']['mode'];
    $config['postedParam'] = array(
      'email' => $order->mail,
      'value' => $amount_cents,
      'currency' => $default_currency,
      'trackId' => $order->order_id,
      'card' => array(
        'name' => "{$billing_address['first_name']} {$billing_address['last_name']}",
        'billingDetails' => array(
          'addressLine1' => $billing_address['thoroughfare'],
          'addressLine2' => $billing_address['premise'],
          'postcode' => $billing_address['postal_code'],
          'country' => $billing_address['country'],
          'city' => $billing_address['locality'],
        ),
      ),
    );
    $products = NULL;

    foreach ($order_wrapper->commerce_line_items as $delta => $line_item_wrapper) {
      $product_id = $line_item_wrapper->commerce_product->raw();
      $product = commerce_product_load($product_id);
      $price = commerce_product_calculate_sell_price($product);
      $sell_price = number_format(commerce_currency_amount_to_decimal($price['amount'], $price['currency_code']), 2, '.', '');

      // Add the line item to the return array.
      $products[$delta] = array(
        'productName' => commerce_line_item_title($line_item_wrapper->value()),
        'price' => $sell_price,
        'quantity' => round($line_item_wrapper->quantity->value()),
        'sku' => '',
      );

      // If it was a product line item, add the SKU.
      if (in_array($line_item_wrapper->type->value(), commerce_product_line_item_types())) {
        $products[$delta]['sku'] = $line_item_wrapper->line_item_label->value();
      }
    }
    if ($products && !empty($products)) {
      $config['postedParam']['products'] = $products;
    }
    if (module_exists('commerce_shipping') && !empty($order_wrapper->commerce_customer_shipping->commerce_customer_address)) {
      $shipping_address = $order_wrapper->commerce_customer_shipping->commerce_customer_address->value();

      // Add the shipping address parameters to the request.
      $shipping_array = array(
        'addressLine1' => $shipping_address['thoroughfare'],
        'addressLine2' => $shipping_address['premise'],
        'postcode' => $shipping_address['postal_code'],
        'country' => $shipping_address['country'],
        'city' => $shipping_address['locality'],
      );

      $config['postedParam']['shippingDetails'] = $shipping_array;
    }

    if ($payment_method['settings']['payment_action'] == COMMERCE_CREDIT_AUTH_CAPTURE) {
      $config = array_merge_recursive($this->captureConfig($payment_method), $config);
    }
    else {
      $config = array_merge_recursive($this->authorizeConfig($payment_method), $config);
    }

    return $config;
  }

  /**
   * Payment method callback: checkout form submission.
   */
  protected function placeorder($config, $charge, $order, $payment_method) {

    $respond_charge = $this->createCharge($config);
    $transaction = commerce_payment_transaction_new('commerce_checkoutpayment', $order->order_id);
    $transaction->instance_id = $payment_method['instance_id'];
    $transaction->amount = $charge['amount'];
    $transaction->currency_code = $charge['currency_code'];
    $transaction->payload[REQUEST_TIME] = $respond_charge->getCreated();

    $default_currency = commerce_default_currency();
    $amount_cents = number_format(commerce_currency_convert($charge['amount'], $charge['currency_code'], $default_currency), 0, '', '');
    $to_validate = array(
      'currency' => $default_currency,
      'value' => $amount_cents,
      'trackId' => $order->order_id,
    );
    $api = CheckoutApi_Api::getApi(array('mode' => $config['mode']));
    $validate_request = $api::validateRequest($to_validate, $respond_charge);

    if ($respond_charge->isValid()) {
      if (preg_match('/^1[0-9]+$/', $respond_charge->getResponseCode())) {
        $transaction->message = 'Your transaction has been successfully authorized with transaction id : ' . $respond_charge->getId();

        if(!$validate_request['status']){
          foreach($validate_request['message'] as $errormessage){
            $transaction->message .= $errormessage . '. ';
          }
        }

        $transaction->status = COMMERCE_PAYMENT_STATUS_PENDING;
        commerce_payment_transaction_save($transaction);
        return TRUE;
      }
      $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
      drupal_set_message(t('We could not process your card. Please verify your information again or try a different card.'), 'error');
      drupal_set_message(check_plain($respond_charge->getMessage()), 'error');
      $transaction->message = $respond_charge->getRawRespond();
      commerce_payment_transaction_save($transaction);
      return FALSE;
    }
    else {
      $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
      $transaction->message = $respond_charge->getRawRespond();

      drupal_set_message(t('We received the following error processing your card. Please verify your information again or try a different card.'), 'error');
      drupal_set_message(check_plain($respond_charge->getExceptionState()->getErrorMessage()), 'error');
      commerce_payment_transaction_save($transaction);
      return FALSE;
    }
  }

  /**
   * Create charge settings.
   *
   * @param $config
   * Charge configuration
   *
   * @return array
   *   Settings form array
   */
  protected function createCharge($config) {
    $api = CheckoutApi_Api::getApi(array('mode' => $config['mode']));
    return $api->createCharge($config);
  }

  /**
   * Capture config settings.
   *
   * @param $action
   * Payment method settings
   *
   * @return array
   *   Settings form array
   */
  protected function captureConfig($action) {
    $to_return['postedParam'] = array(
      'autoCapture' => CheckoutApi_Client_Constant::AUTOCAPUTURE_CAPTURE,
      'autoCapTime' => $action['settings']['autocaptime'],
    );

    return $to_return;
  }

  /**
   * Authorize config settings.
   * 
   * @return array
   *   Settings form array
   */
  protected function authorizeConfig() {
    $to_return['postedParam'] = array(
      'autoCapture' => CheckoutApi_Client_Constant::AUTOCAPUTURE_AUTH,
      'autoCapTime' => 0,
    );
    return $to_return;
  }

  /**
   * Config settings.
   */
  public function getExtraInit($order,$payment_method) {
    return NULL;
  }

}
