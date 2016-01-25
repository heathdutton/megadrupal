<?php

/**
 * @file
 * Wrapper class for the Todo Pago gateway.
 */

/**
 * Status codes from Todo Pago
 */
define('TODOPAGO_STATUSCODE_APPROVED', -1);
define('TODOPAGO_STATUSCODE_NO_INFORMATION', 0);
define('TODOPAGO_STATUSCODE_REJECTED', 5);
define('TODOPAGO_STATUSCODE_ACCREDITED', 6);
define('TODOPAGO_STATUSCODE_CANCELLED', 7);
define('TODOPAGO_STATUSCODE_REFUNDED', 9);
define('TODOPAGO_STATUSCODE_CONFIRMED_REFUND', 10);
define('TODOPAGO_STATUSCODE_PRE_AUTHORIZED', 11);
define('TODOPAGO_STATUSCODE_OVERDUE', 12);
define('TODOPAGO_STATUSCODE_PENDING_ACCREDITATION', 13);
define('TODOPAGO_STATUSCODE_PENDING_REFUND', 15);
define('TODOPAGO_STATUSCODE_REJECTED_NO_AUTHENTICATION', 24);
define('TODOPAGO_STATUSCODE_REJECTED_INVALID_DATA', 25);
define('TODOPAGO_STATUSCODE_REJECTED_NO_VALIDATED', 32);
define('TODOPAGO_STATUSCODE_TIMEOUT', 38);
define('TODOPAGO_STATUSCODE_INVALID_TRANSACTION', 404);
define('TODOPAGO_STATUSCODE_INVALID_ACCOUNT', 702);
define('TODOPAGO_STATUSCODE_PARAMETRIZATION_ERROR', 720);
define('TODOPAGO_STATUSCODE_SYSTEM_ERROR', 999);

/**
 * Todo Pago connector using the drupal stored credentials.
 */
class TodoPagoConnector {
  private $settings;
  private $todopago;

  /**
   * Constructor
   */
  public function __construct($settings) {
    if (
      (empty($settings)) ||
      (!is_array($settings)) ||
      (empty($settings['merchant'])) ||
      (empty($settings['authorization'])) ||
      (empty($settings['security'])) ||
      (empty($settings['currency_code'])) ||
      (!in_array($settings['mode'], array('test', 'prod')))
    ) {
      throw new Exception(t('Todo Pago is not configured for use.'));
    }
    $this->settings = $settings;

    $http_header = array(
      'Authorization' => $this->settings['authorization'],
      'user_agent' => 'Commerce_TodoPago',
    );

    $this->todopago = new TodoPago\Sdk($http_header, $this->settings['mode']);
  }

  /**
   * Returns an array with the accepted currencies
   *
   * @return
   *   An array with the ISO codes from the accepted currencies
   */
  public static function get_currencies() {
    return array(
      'ARS',
    );
  }

  /**
   * Returns a currency numeric code, as used by Todo Pago
   *
   * @param $currency_code
   *   Currency code to convert
   *
   * @return
   *   The numeric code of the currency
   */
  public static function currency_code($currency_code) {
    if ($currency = commerce_currency_load($currency_code)) {
      return $currency['numeric_code'];
    }
    return FALSE;
  }

  /**
   * Formats a price amount into a decimal value as expected by Todo Pago.
   *
   * @param $amount
   *   An integer price amount.
   * @param $currency_code
   *   The currency code of the price.
   *
   * @return
   *   The decimal price amount as expected by Todo Pago's API servers.
   */
  public static function format_price_amount($amount, $currency_code) {
    $rounded_amount = commerce_currency_round($amount, commerce_currency_load($currency_code));
    return number_format(commerce_currency_amount_to_decimal($rounded_amount, $currency_code), 2, '.', '');
  }

  /**
   * Returns the Commerce Status equivalent for a Todo Pago status code
   *
   * @param $status_code
   *   Todo Pago status code
   *
   * @return
   *   Commerce payment status code
   */
  private function payment_status_code($status_code) {
    switch ($status_code) {
      case TODOPAGO_STATUSCODE_INVALID_ACCOUNT:
      case TODOPAGO_STATUSCODE_INVALID_TRANSACTION:
      case TODOPAGO_STATUSCODE_PARAMETRIZATION_ERROR:
        $payment_status = '';
        break;

      case TODOPAGO_STATUSCODE_NO_INFORMATION:
      case TODOPAGO_STATUSCODE_PENDING_ACCREDITATION:
      case TODOPAGO_STATUSCODE_PENDING_REFUND:
      case TODOPAGO_STATUSCODE_PRE_AUTHORIZED:
        $payment_status = COMMERCE_PAYMENT_STATUS_PENDING;
        break;

      case TODOPAGO_STATUSCODE_ACCREDITED:
      case TODOPAGO_STATUSCODE_APPROVED:
        $payment_status = COMMERCE_PAYMENT_STATUS_SUCCESS;
        break;

      case TODOPAGO_STATUSCODE_CANCELLED:
      case TODOPAGO_STATUSCODE_CONFIRMED_REFUND:
      case TODOPAGO_STATUSCODE_OVERDUE:
      case TODOPAGO_STATUSCODE_REFUNDED:
      case TODOPAGO_STATUSCODE_REJECTED:
      case TODOPAGO_STATUSCODE_REJECTED_INVALID_DATA:
      case TODOPAGO_STATUSCODE_REJECTED_NO_AUTHENTICATION:
      case TODOPAGO_STATUSCODE_REJECTED_NO_VALIDATED:
      case TODOPAGO_STATUSCODE_SYSTEM_ERROR:
      case TODOPAGO_STATUSCODE_TIMEOUT:
      default:
        $payment_status = COMMERCE_PAYMENT_STATUS_FAILURE;
        break;

    }

    return $payment_status;
  }

  /**
   * Returns an array with the Drupal form for the redirection to Todo Pago
   *
   * @param $order
   *   Commerce Order
   *
   * @return
   *   An array with the form
   */
  public function create_order_form($order) {
    $form = array();

    $order_wrapper = entity_metadata_wrapper('commerce_order', $order);

    $currency_code = $this->settings['currency_code'];
    $order_currency_code = $order_wrapper->commerce_order_total->currency_code->value();

    if (
      (!empty($settings['allow_supported_currencies'])) &&
      (in_array($order_currency_code, self::get_currencies()))
    ) {
      $currency_code = $order_currency_code;
    }

    $hash = uniqid();

    $operation_id = commerce_todopago_remote_id($order, $hash);
    $amount = $this->format_price_amount(commerce_currency_convert($order_wrapper->commerce_order_total->amount->value(), $order_currency_code, $currency_code), $currency_code);
    $customer_email = $order_wrapper->mail->value();
    $return_url = url('checkout/' . $order->order_id . '/payment/return/' . $order->data['payment_redirect_key'] . '/' . $hash, array('absolute' => TRUE));

    $todopago_info = array(
      'commerce' => array(
        'Security' => $this->settings['security'],
        'EncodingMethod' => 'XML',
        'Merchant' => $this->settings['merchant'],
        'URL_OK' => $return_url,
        'URL_ERROR' => $return_url,
      ),
      'order' => array(
        'MERCHANT' => $this->settings['merchant'],
        'OPERATIONID' => $operation_id,
        'CURRENCYCODE' => self::currency_code($currency_code),
        'AMOUNT' => $amount,
        'EMAILCLIENTE' => $customer_email,
        /**
         * Fraud prevention
         */
        'CSBTCITY' => '',
        'CSBTCOUNTRY' => '',
        'CSBTCUSTOMERID' => $order_wrapper->uid->value(),
        'CSBTIPADDRESS' => ip_address(),
        'CSBTEMAIL' => $customer_email,
        'CSBTFIRSTNAME' => '',
        'CSBTLASTNAME' => '',
        'CSBTPHONENUMBER' => '',
        'CSBTPOSTALCODE' => '',
        'CSBTSTATE' => '',
        'CSBTSTREET1' => '',
        'CSPTCURRENCY' => $currency_code,
        'CSPTGRANDTOTALAMOUNT' => $amount,
      ),
    );

    /**
     * Fraud prevention: Generic fields
     */
    if (isset($order_wrapper->commerce_customer_billing->commerce_customer_address->locality)) {
      $todopago_info['order']['CSBTCITY'] = $order_wrapper->commerce_customer_billing->commerce_customer_address->locality->value();
    }
    if (isset($order_wrapper->commerce_customer_billing->commerce_customer_address->country)) {
      $todopago_info['order']['CSBTCOUNTRY'] = $order_wrapper->commerce_customer_billing->commerce_customer_address->country->value();
    }
    if (isset($order_wrapper->commerce_customer_billing->commerce_customer_address->first_name)) {
      $todopago_info['order']['CSBTFIRSTNAME'] = $order_wrapper->commerce_customer_billing->commerce_customer_address->first_name->value();
    }
    if (isset($order_wrapper->commerce_customer_billing->commerce_customer_address->last_name)) {
      $todopago_info['order']['CSBTLASTNAME'] = $order_wrapper->commerce_customer_billing->commerce_customer_address->last_name->value();
    }
    if (isset($order_wrapper->commerce_customer_billing->commerce_customer_address->phone_number)) {
      $phone_number = $order_wrapper->commerce_customer_billing->commerce_customer_address->phone_number->value();
      $todopago_info['order']['CSBTPHONENUMBER'] = $this->formatPhoneNumber($phone_number);
    }
    if (isset($order_wrapper->commerce_customer_billing->commerce_customer_address->postal_code)) {
      $todopago_info['order']['CSBTPOSTALCODE'] = $order_wrapper->commerce_customer_billing->commerce_customer_address->postal_code->value();
    }
    if (isset($order_wrapper->commerce_customer_billing->commerce_customer_address->administrative_area)) {
      $todopago_info['order']['CSBTSTATE'] = $order_wrapper->commerce_customer_billing->commerce_customer_address->administrative_area->value();
    }
    if (isset($order_wrapper->commerce_customer_billing->commerce_customer_address->thoroughfare)) {
      $todopago_info['order']['CSBTSTREET1'] = $order_wrapper->commerce_customer_billing->commerce_customer_address->thoroughfare->value();
    }

    /**
     * Fraud prevention: line item fields
     */
    $product_code = array();
    $product_description = array();
    $product_name = array();
    $product_sku = array();
    $unit_price = array();
    $quantity = array();
    $total_amount = array();
    foreach ($order_wrapper->commerce_line_items as $delta => $line_item_wrapper) {
      if (in_array($line_item_wrapper->type->value(), commerce_product_line_item_types())) {
        $product_code[] = $line_item_wrapper->commerce_product->type->value();
        $product_description[] = $line_item_wrapper->commerce_product->title->value();
        $product_name[] = $line_item_wrapper->commerce_product->title->value();
        $product_sku[] = $line_item_wrapper->commerce_product->sku->value();

        $item_price = $line_item_wrapper->commerce_product->commerce_price->value();
        $item_price_amount = commerce_currency_convert($item_price['amount'], $item_price['currency_code'], $currency_code);
        $unit_price[] = $this->format_price_amount($item_price_amount, $currency_code);
        $item_quantity = $line_item_wrapper->quantity->value();
        $quantity[] = intval($item_quantity);
        $total_amount[] = $this->format_price_amount($item_price_amount * $item_quantity, $currency_code);
      }
    }
    $todopago_info['order']['CSITPRODUCTCODE'] = substr(implode('#', $product_code), 0, 255);
    $todopago_info['order']['CSITPRODUCTDESCRIPTION'] = substr(implode('#', $product_description), 0, 255);
    $todopago_info['order']['CSITPRODUCTNAME'] = substr(implode('#', $product_name), 0, 255);
    $todopago_info['order']['CSITPRODUCTSKU'] = substr(implode('#', $product_sku), 0, 255);
    $todopago_info['order']['CSITUNITPRICE'] = substr(implode('#', $unit_price), 0, 255);
    $todopago_info['order']['CSITQUANTITY'] = substr(implode('#', $quantity), 0, 255);
    $todopago_info['order']['CSITTOTALAMOUNT'] = substr(implode('#', $total_amount), 0, 255);

    /**
     * Fraud prevention: Specific fields
     */
    if ($this->settings['category'] == 'retail') {
      $todopago_info['order']['CSSTEMAIL'] = $customer_email;

      if (isset($order_wrapper->commerce_customer_shipping->commerce_customer_address->locality)) {
        $todopago_info['order']['CSSTCITY'] = $order_wrapper->commerce_customer_shipping->commerce_customer_address->locality->value();
      }
      if (isset($order_wrapper->commerce_customer_shipping->commerce_customer_address->country)) {
        $todopago_info['order']['CSSTCOUNTRY'] = $order_wrapper->commerce_customer_shipping->commerce_customer_address->country->value();
      }
      if (isset($order_wrapper->commerce_customer_shipping->commerce_customer_address->first_name)) {
        $todopago_info['order']['CSSTFIRSTNAME'] = $order_wrapper->commerce_customer_shipping->commerce_customer_address->first_name->value();
      }
      if (isset($order_wrapper->commerce_customer_shipping->commerce_customer_address->last_name)) {
        $todopago_info['order']['CSSTLASTNAME'] = $order_wrapper->commerce_customer_shipping->commerce_customer_address->last_name->value();
      }
      if (isset($order_wrapper->commerce_customer_shipping->commerce_customer_address->phone_number)) {
        $phone_number = $order_wrapper->commerce_customer_shipping->commerce_customer_address->phone_number->value();
        $todopago_info['order']['CSSTPHONENUMBER'] = $this->formatPhoneNumber($phone_number);
      }
      if (isset($order_wrapper->commerce_customer_shipping->commerce_customer_address->postal_code)) {
        $todopago_info['order']['CSSTPOSTALCODE'] = $order_wrapper->commerce_customer_shipping->commerce_customer_address->postal_code->value();
      }
      if (isset($order_wrapper->commerce_customer_shipping->commerce_customer_address->administrative_area)) {
        $todopago_info['order']['CSSTSTATE'] = $order_wrapper->commerce_customer_shipping->commerce_customer_address->administrative_area->value();
      }
      if (isset($order_wrapper->commerce_customer_shipping->commerce_customer_address->thoroughfare)) {
        $todopago_info['order']['CSSTSTREET1'] = $order_wrapper->commerce_customer_shipping->commerce_customer_address->thoroughfare->value();
      }
    } elseif ($this->settings['category'] == 'travel') {
      // TODO
    } elseif ($this->settings['category'] == 'services') {
      // TODO
    } elseif ($this->settings['category'] == 'digital_goods') {
      // TODO
    } elseif ($this->settings['category'] == 'ticketing') {
      // TODO
    }

    $authorize_request = $this->todopago->sendAuthorizeRequest($todopago_info['commerce'], $todopago_info['order']);

    if (
      (empty($authorize_request)) ||
      (empty($authorize_request['StatusCode'])) ||
      (empty($authorize_request['URL_Request'])) ||
      (empty($authorize_request['RequestKey'])) ||
      ($authorize_request['StatusCode'] != TODOPAGO_STATUSCODE_APPROVED)
    ) {
      throw new Exception(t('Could not get authorize request: !response', array('!response' => print_r($authorize_request, TRUE))));
    }

    $transaction = commerce_payment_transaction_new('commerce_todopago', $order_wrapper->order_number->value());
    $transaction->instance_id = ((!empty($this->settings['instance_id'])) ? ($this->settings['instance_id']) : (''));
    $transaction->remote_id = $operation_id;
    $transaction->amount = commerce_currency_decimal_to_amount($todopago_info['order']['AMOUNT'], $currency_code);
    $transaction->currency_code = $currency_code;
    $transaction->status = COMMERCE_PAYMENT_STATUS_PENDING;
    $transaction->data = array(
      'order' => $todopago_info['order'],
      'authorize_request' => $authorize_request,
    );

    commerce_payment_transaction_save($transaction);

    $form['#action'] = $authorize_request['URL_Request'];
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Proceed to Todo Pago'),
    );

    return($form);
  }

  /**
   * Update the status of a transaction
   *
   * @param $transaction
   *   The transaction who's payment status will be updated
   *
   * @return
   *   void
   */
  public function update_status(&$transaction) {
    if (
      (!empty($transaction->data['authorize_request']['RequestKey'])) &&
      (!empty($transaction->data['answer_key']))
    ) {
      $get_authorize_answer_info = array(
        'Security' => $this->settings['security'],
        'Merchant'  => $this->settings['merchant'],
        'RequestKey' => $transaction->data['authorize_request']['RequestKey'],
        'AnswerKey' => $transaction->data['answer_key'],
      );

      $get_authorize_answer = $this->todopago->getAuthorizeAnswer($get_authorize_answer_info);

      if (
        (empty($get_authorize_answer)) ||
        (empty($get_authorize_answer['StatusCode'])) ||
        (!$payment_status = $this->payment_status_code($get_authorize_answer['StatusCode']))
      ) {
        throw new Exception(t('Could not get authorize answer: !response.', array('!response' => print_r($get_authorize_answer, TRUE))));
      }

      $transaction->status = $payment_status;
      $transaction->remote_status = $get_authorize_answer['StatusCode'];
      $transaction->message = $get_authorize_answer['StatusMessage'];

      if (empty($transaction->payload['get_authorize_answer'])) {
        $transaction->payload['get_authorize_answer'] = array();
      }
      $transaction->payload['get_authorize_answer'][REQUEST_TIME] = $get_authorize_answer;
    }

    $get_status_info = array(
      'MERCHANT' => $this->settings['merchant'],
      'OPERATIONID' => $transaction->remote_id,
    );

    $get_status = $this->todopago->getStatus($get_status_info);

    if (
      (empty($get_status)) ||
      (empty($get_status['Operations'])) ||
      (empty($get_status['Operations']['RESULTCODE'])) ||
      (!$payment_status = $this->payment_status_code($get_status['Operations']['RESULTCODE']))
    ) {
      throw new Exception(t('Could not get transaction status from Todo Pago: !response.', array('!response' => print_r($get_status, TRUE))));
    }

    $transaction->status = $payment_status;
    $transaction->remote_status = $get_status['Operations']['RESULTCODE'];
    $transaction->message = $get_status['Operations']['RESULTMESSAGE'];

    if (empty($transaction->payload['get_status'])) {
      $transaction->payload['get_status'] = array();
    }
    $transaction->payload['get_status'][REQUEST_TIME] = $get_status;

    commerce_payment_transaction_save($transaction);
  }

  /**
   * Returns the phone number in the format expected by TodoPago.
   *
   * @param $phone_number
   *   The phone number to format
   */
  public function formatPhoneNumber($phone_number) {
    // TodoPago complains if the phone contains non numeric characters.
    return preg_replace('/[^0-9]/', '', $phone_number);
  }
}

