<?php

class methods_creditcard extends methods_Abstract {

  /**
   * Payment method callback: checkout form submission.
   */
  public function submitFormCharge($payment_method, $pane_form, $pane_values, $order, $charge) {

    $config = parent::submitFormCharge($payment_method, $pane_form, $pane_values, $order, $charge);
    if(isset($pane_values['cko-cc-paymenToken'])) {
      $config['postedParam']['paymentToken'] = $pane_values['cko-cc-paymenToken'];
    }

    if (!empty($pane_values['cko-cc-redirectUrl'])) {
      drupal_goto($pane_values['cko-cc-redirectUrl'] . '&trackId=' . $order->order_id);
    }
    else {
      return $this->placeorder($config, $charge, $order, $payment_method);
    }
  }

  /**
   * Payment method callback: checkout form.
   */
  public function submitForm($payment_method, $pane_values, $checkout_pane, $order) {

    $data = $this->getExtraInit($order, $payment_method);
    $form['pay_method_container'] = array(
      '#type' => 'container',
      '#attributes' => array(
        'class' => array('widget-container'),
      ),
    );

    $form['credit_card']['cko-cc-paymenToken'] = array(
      '#type' => 'hidden',
      '#value' => !empty($data['paymentToken']['token']) ? $data['paymentToken']['token'] : '',
      '#attributes' => array(
        'id' => array('cko-cc-paymenToken'),
      ),
    );

    $form['#attached']['js'] = array(
      drupal_get_path('module', 'commerce_checkoutpayment') . '/includes/methods/js/checkoutapi.js' => array(
        'type' => 'file',
      ),
    );

    $form['#attached']['js'][] = array(
      'data' => array('commerce_checkoutpayment' => $data['script']),
      'type' => 'setting',
    );

    return $form;
  }

  /**
   * Payment method settings form.
   *
   * @param $order
   * The order transaction
   * @param $payment_method
   * The payment method used
   *
   * @return array
   *   Settings form array
   */
  public function getExtraInit($order, $payment_method) {

    $array = array();
    module_load_include('inc', 'commerce_payment', 'includes/commerce_payment.credit_card');

    $payment_token = $this->generatePaymentToken($order, $payment_method);

    if ($order) {
      $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
      $billing_address = $order_wrapper->commerce_customer_billing->commerce_customer_address->value();
      $order_array = $order_wrapper->commerce_order_total->value();
      $default_currency = commerce_default_currency();
      $amount_cents = commerce_currency_convert($order_array['amount'], $order_array['currency_code'], $default_currency);
      $config = array();

      $config['publicKey'] = $payment_method['settings']['public_key'];
      $config['mode'] = $payment_method['settings']['mode'];
      $config['iconcolor'] = $payment_method['settings']['iconcolor'];
      $config['logourl'] = $payment_method['settings']['logourl'];
      $config['buttoncolor'] = $payment_method['settings']['buttoncolor'];
      $config['themecolor'] = $payment_method['settings']['themecolor'];
      $config['currencycode'] = $payment_method['settings']['currencycode'];
      $config['email'] = $order->mail;
      $config['name'] = "{$billing_address['first_name']} {$billing_address['last_name']}";
      $config['amount'] = $amount_cents;
      $config['currency'] = $default_currency;
      $config['localpayment'] = ($payment_method['settings']['localpayment'] == 'false') ? 'card' : 'mixed';
      $config['paymentToken'] = $payment_token['token'];

      $array['script'] = $config;
      $array['paymentToken'] = $payment_token;
    }

    return $array;
  }

  /**
   * Generate payment token.
   *
   * @param $order
   * The order transaction
   * @param $payment_method
   * The payment method used
   *
   * @return array
   * Payment token form array
   */
  public function generatePaymentToken($order, $payment_method) {

    $config = array();
    $shipping_address_config = NULL;

    $order_wrapper = entity_metadata_wrapper('commerce_order', $order);

    $billing_address = $order_wrapper->commerce_customer_billing->commerce_customer_address->value();
    $order_array = $order_wrapper->commerce_order_total->value();
    $product_line_items = $order->commerce_line_items[LANGUAGE_NONE];

    if (isset($order)) {

      $order_id = $order->order_id;
      $default_currency = commerce_default_currency();
      $amount_cents = number_format(commerce_currency_convert($order->commerce_order_total[LANGUAGE_NONE][0]['amount'], $order_array['currency_code'], $default_currency), 0, '', '');

      $secret_key = $payment_method['settings']['private_key'];
      $mode = $payment_method['settings']['mode'];
      $timeout = $payment_method['settings']['timeout'];

      $config['authorization'] = $secret_key;
      $config['mode'] = $mode;
      $config['timeout'] = $timeout;

      if ($payment_method['settings']['payment_action'] == 'authorize') {

        $config = array_merge($config, $this->authorizeConfig());
      }
      else {

        $config = array_merge($config, $this->captureConfig($payment_method));
      }

      $products = array();
      if (!empty($product_line_items)) {
        foreach ($product_line_items as $key => $item) {

          $line_item[$key] = commerce_line_item_load($item['line_item_id']);

          $product_id = $line_item[$key]->commerce_product[LANGUAGE_NONE][0]['product_id'];
          $product = commerce_product_load($product_id);
          $price = commerce_product_calculate_sell_price($product);
          $sell_price = number_format(commerce_currency_amount_to_decimal($price['amount'], $price['currency_code']), 2, '.', '');

          $products[$key] = array(
            'name' => commerce_line_item_title($line_item[$key]),
            'sku' => $line_item[$key]->line_item_label,
            'price' => $sell_price,
            'quantity' => (int) $line_item[$key]->quantity,
          );
        }
      }

      $billing_address_config = array(
        'addressLine1' => $billing_address['thoroughfare'],
        'addressLine2' => $billing_address['premise'],
        'postcode' => $billing_address['postal_code'],
        'country' => $billing_address['country'],
        'city' => $billing_address['locality'],
      );

      if (module_exists('commerce_shipping') && !empty($order_wrapper->commerce_customer_shipping->commerce_customer_address)) {
        $shipping_address = $order_wrapper->commerce_customer_shipping->commerce_customer_address->value();

        // Add the shipping address parameters to the request.
        $shipping_address_config = array(
          'addressLine1' => $shipping_address['thoroughfare'],
          'addressLine2' => $shipping_address['premise'],
          'postcode' => $shipping_address['postal_code'],
          'country' => $shipping_address['country'],
          'city' => $shipping_address['locality'],
        );
      }

      $config['postedParam'] = array_merge($config['postedParam'], array(
        'email' => $order->mail,
        'value' => $amount_cents,
        'trackId' => $order_id,
        'currency' => $default_currency,
        'description' => 'Order number::' . $order_id,
        'shippingDetails' => $shipping_address_config,
        'products' => $products,
        'card' => array(
          'billingDetails' => $billing_address_config,
        ),
      ));

      $api = CheckoutApi_Api::getApi(array('mode' => $mode));

      $payment_token_charge = $api->getPaymentToken($config);

      $payment_token_array = array(
        'message' => '',
        'success' => '',
        'eventId' => '',
        'token' => '',
      );

      if ($payment_token_charge->isValid()) {
        $payment_token_array['token'] = $payment_token_charge->getId();
        $payment_token_array['success'] = TRUE;
      }
      else {

        $payment_token_array['message'] = $payment_token_charge->getExceptionState()->getErrorMessage();
        $payment_token_array['success'] = FALSE;
        $payment_token_array['eventId'] = $payment_token_charge->getEventId();
      }
    }
    return $payment_token_array;
  }

  /**
   * Create charge settings.
   *
   * @param $config
   * Charge settings
   *
   * @return array
   * Settings form array
   */
  protected function createCharge($config) {

    $config = array();

    $payment_method = commerce_payment_method_instance_load('commerce_checkoutpayment|commerce_payment_commerce_checkoutpayment');
    $secret_key = $payment_method['settings']['private_key'];
    $mode = $payment_method['settings']['mode'];
    $timeout = $payment_method['settings']['timeout'];

    $config['authorization'] = $secret_key;
    $config['timeout'] = $timeout;
    $config['paymentToken'] = $_POST['cko-cc-paymenToken'];

    $api = CheckoutApi_Api::getApi(array('mode' => $mode));
    return $api->verifyChargePaymentToken($config);
  }

  /**
   * Capture config settings.
   *
   * @param $action
   * Capture charge settings
   *
   * @return array
   * Settings form array
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
   * Settings form array
   */
  protected function authorizeConfig() {
    $to_return['postedParam'] = array(
      'autoCapture' => CheckoutApi_Client_Constant::AUTOCAPUTURE_AUTH,
      'autoCapTime' => 0,
    );
    return $to_return;
  }

}
