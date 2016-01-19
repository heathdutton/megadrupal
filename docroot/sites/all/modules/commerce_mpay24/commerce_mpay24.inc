<?php

/**
 * Class MPay24Commerce
 *
 * Integrates Drupal Commerce into the MPay24Shop library. The MPay24Shop
 * library has it's own classes for transactions and orders, we use them to map
 * the data to the Drupal commerce entities.
 */
class MPay24Commerce extends MPay24Shop {

  var $commerce_order;
  var $commerce_transaction;
  var $commerce_payment_method;
  var $billing_address_array;

  /**
   * {@inheritdoc}
   */
  function createTransaction() {
    $wrapper = entity_metadata_wrapper('commerce_order', $this->commerce_order);

    $amount = $wrapper->commerce_order_total->amount->value();
    $order_currency_code = $wrapper->commerce_order_total->currency_code->value();
    $rounded_amount = commerce_currency_round($amount, commerce_currency_load($order_currency_code));
    $price = number_format(commerce_currency_amount_to_decimal($rounded_amount, $order_currency_code), 2, '.', '');

    $transaction = new Transaction($this->commerce_order->order_id);
    $transaction->PRICE = $price;
    $transaction->CURRENCY = $order_currency_code;
    $transaction->ORDERDESC = t('Order @order_number at @store', array('@order_number' => $this->commerce_order->order_id, '@store' => variable_get('site_name', url('<front>', array('absolute' => TRUE)))));

    $this->createCommerceTransaction($amount, $order_currency_code);

    return $transaction;
  }

  /**
   * {@inheritdoc}
   */
  function createMDXI($transaction) {
    $mdxi = new ORDER();

    $mdxi->Order->ClientIP = ip_address();
    $mdxi->Order->Tid = $transaction->TID;

    $mdxi->Order->ShoppingCart->Description = $transaction->ORDERDESC;

    $wrapper = entity_metadata_wrapper('commerce_order', $this->commerce_order);
    $i = 1;
    foreach ($wrapper->commerce_line_items as $line_item_wrapper) {
      $mdxi->Order->ShoppingCart->Item($i)->Description = $line_item_wrapper->line_item_label->value();
      $mdxi->Order->ShoppingCart->Item($i)->Quantity = number_format($line_item_wrapper->quantity->value(), 0);

      $unit_price = $line_item_wrapper->commerce_unit_price->value();
      // Try to extract the base price if possible.
      if (isset($unit_price['data']['components'][0]['name']) && $unit_price['data']['components'][0]['name'] == 'base_price') {
        $formatted_unit_price = number_format(commerce_currency_amount_to_decimal($unit_price['data']['components'][0]['price']['amount'], $unit_price['data']['components'][0]['price']['currency_code']), 2, '.', '');
      }
      else {
        $formatted_unit_price = number_format(commerce_currency_amount_to_decimal($unit_price['amount'], $unit_price['currency_code']), 2, '.', '');
      }
      $mdxi->Order->ShoppingCart->Item($i)->ItemPrice = $formatted_unit_price;

      $total_price = $line_item_wrapper->commerce_total->value();
      // Try to extract the base price if possible.
      if (isset($total_price['data']['components'][0]['name']) && $total_price['data']['components'][0]['name'] == 'base_price') {
        $formatted_total_price = number_format(commerce_currency_amount_to_decimal($total_price['data']['components'][0]['price']['amount'], $total_price['data']['components'][0]['price']['currency_code']), 2, '.', '');
      }
      else {
        $formatted_total_price = number_format(commerce_currency_amount_to_decimal($total_price['amount'], $total_price['currency_code']), 2, '.', '');
      }
      $mdxi->Order->ShoppingCart->Item($i)->Price = $formatted_total_price;

      $i++;
    }

    // Set the base price, discount and taxes if available.
    $price = $wrapper->commerce_order_total->value();
    if (isset($price['data']['components'])) {
      foreach ($price['data']['components'] as $price_component) {
        if ($price_component['name'] == 'base_price') {
          $mdxi->Order->ShoppingCart->SubTotal = number_format(commerce_currency_amount_to_decimal($price_component['price']['amount'], $price_component['price']['currency_code']), 2, '.', '');
        }
        elseif (strpos($price_component['name'], 'commerce_coupon_') === 0) {
          $mdxi->Order->ShoppingCart->Discount = number_format(commerce_currency_amount_to_decimal($price_component['price']['amount'], $price_component['price']['currency_code']), 2, '.', '');
        }
        elseif (strpos($price_component['name'], 'tax|') === 0) {
          $mdxi->Order->ShoppingCart->Tax = number_format(commerce_currency_amount_to_decimal($price_component['price']['amount'], $price_component['price']['currency_code']), 2, '.', '');
        }
      }
    }

    $mdxi->Order->Price = $transaction->PRICE;
    $mdxi->Order->Currency = $transaction->CURRENCY;

    // Set billing address if available.
    if (!empty($this->billing_address_array)) {
      $mdxi->Order->BillingAddr->setMode("ReadOnly");
      foreach ($this->billing_address_array as $key => $value) {
        if ($key == 'Country') {
          $mdxi->Order->BillingAddr->Country->setCode($value);
        }
        else {
          $mdxi->Order->BillingAddr->{$key} = $value;
        }
      }
    }

    // URLs
    $mdxi->Order->URL->Success = url('checkout/' . $this->commerce_order->order_id . '/payment/return/' . $this->commerce_order->data['payment_redirect_key'], array('absolute' => TRUE));
    $mdxi->Order->URL->Error = url('checkout/' . $this->commerce_order->order_id . '/payment/back/' . $this->commerce_order->data['payment_redirect_key'], array('absolute' => TRUE));

    // This variable allows the change the return path for confirm calls by
    // mpay24. This could be useful for testing purposes if your site is
    // protected via basic authentication (e.g. then set the variable to
    // 'https://user:password@domain.com').
    $confirm_url_prefix = variable_get('commerce_mpay24_confirm_url_prefix', '');
    if (empty($confirm_url_prefix)) {
      $mdxi->Order->URL->Confirmation = url('mpay24/confirm/' . $this->commerce_order->order_id . '/' . $this->commerce_order->data['payment_redirect_key'], array('absolute' => TRUE));
    }
    else {
      $mdxi->Order->URL->Confirmation = url($confirm_url_prefix . '/mpay24/confirm/' . $this->commerce_order->order_id . '/' . $this->commerce_order->data['payment_redirect_key'], array('absolute' => FALSE));
    }

    return $mdxi;
  }

  /**
   * {@inheritdoc}
   */
  function updateTransaction($tid, $args, $shippingConfirmed) {
    // Every time we receive calls from mpay24 via
    // commerce_mpay24_confirm_callback(), this method is called. We try to
    // store all information in the Drupal Commerce transaction and change the
    // state if necessary.
    if (!empty($this->commerce_transaction)) {
      if ($this->commerce_transaction->status == COMMERCE_PAYMENT_STATUS_PENDING &&
        isset($args['OPERATION']) && $args['OPERATION'] == 'CONFIRMATION' &&
        isset($args['TSTATUS']) && in_array($args['TSTATUS'], array('RESERVED', 'BILLED'))
      ) {
        $this->commerce_transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
      }

      if (isset($args['TSTATUS'])) {
        $this->commerce_transaction->message = t('mPAY24: Status @status', array('@status' => $args['TSTATUS']));
      }

      $this->commerce_transaction->remote_id = $args['MPAYTID'];
      $this->commerce_transaction->remote_status = $args['TSTATUS'];
      $this->commerce_transaction->payload[REQUEST_TIME . '-log'] = $args;

      commerce_payment_transaction_save($this->commerce_transaction);
    }
  }

  /**
   * {@inheritdoc}
   */
  function getTransaction($tid){
    // This method is called when using the confirm() method (in
    // commerce_mpay24_confirm_callback()). We do not need the full mpay24
    // transaction object, as we store everything in the Drupal Commerce
    // transaction, but it needs the transaction id (order id) and the price.
    $transaction = new Transaction($tid);
    $transaction->TID = $tid;

    if (!empty($this->commerce_transaction)) {
      $rounded_amount = commerce_currency_round($this->commerce_transaction->amount, commerce_currency_load($this->commerce_transaction->currency_code));
      $price = number_format(commerce_currency_amount_to_decimal($rounded_amount, $this->commerce_transaction->currency_code), 2, '.', '');
      $transaction->PRICE = $price;
    }

    return $transaction;
  }

  /**
   * {@inheritdoc}
   */
  function createSecret($tid, $amount, $currency, $timeStamp) {
    // We already use Drupal Commerce's payment_redirect_key token, no need for
    // further tokens.
    // @see commerce_mpay24_confirm_callback()
    return $tid;
  }

  /**
   * {@inheritdoc}
   */
  function getSecret($tid) {
    return $tid;
  }

  // Unimplemented methods from parent class.
  function write_log($operation, $info_to_log) {}
  function createProfileOrder($tid) {}
  function createExpressCheckoutOrder($tid) {}
  function createFinishExpressCheckoutOrder($tid, $shippingCosts, $amount, $cancel) {}


  /*****************************
    Drupal Commerce Integration
   *****************************/

  /**
   * Sets the commerce order variable.
   *
   * @param $commerce_order
   */
  function setCommerceOrder($commerce_order) {
    $this->commerce_order = $commerce_order;
  }

  /**
   * Sets the commerce payment method variable.
   *
   * @param $commerce_payment_method
   */
  function setCommercePaymentMethod($commerce_payment_method) {
    $this->commerce_payment_method = $commerce_payment_method;
  }

  /**
   * Sets the commerce transaction variable.
   *
   * @param $commerce_transaction
   */
  function setCommerceTransaction($commerce_transaction) {
    $this->commerce_transaction = $commerce_transaction;
  }

  /**
   * Creates and saves a new commerce transaction.
   *
   * @param $amount
   *   The amount without comma (123.00 -> 12300), as the commerce module
   *   formats it.
   * @param $currency_code
   *   The currency code.
   */
  function createCommerceTransaction($amount, $currency_code) {
    $this->commerce_transaction = commerce_payment_transaction_new('commerce_mpay24', $this->commerce_order->order_id);
    $this->commerce_transaction->instance_id = $this->commerce_payment_method['instance_id'];
    $this->commerce_transaction->amount = $amount;
    $this->commerce_transaction->currency_code = $currency_code;
    $this->commerce_transaction->status = COMMERCE_PAYMENT_STATUS_PENDING;
    $this->commerce_transaction->message = 'mPAY24: Waiting for payment';

    commerce_payment_transaction_save($this->commerce_transaction);
  }

  /**
   * An array containing following data (in this order):
   *  - Name
   *  - Street
   *  - Street2
   *  - Zip
   *  - City
   *  - Country (code)
   *  - Email
   */
  function setBillingAddressArray($billing_address_array) {
    $this->billing_address_array = $billing_address_array;
  }
}
