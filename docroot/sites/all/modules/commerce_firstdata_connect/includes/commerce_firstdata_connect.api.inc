<?php
/**
 * @file
 * The API for Connect payment gateway.
 */

class CommerceFirstdataConnectConnectApi {
  /**
   * Set merchant credentials.
   *
   * @param array $settings
   *   Setting for the payment rules action.
   */
  public function __construct(array $settings) {
    $this->storename = !empty($settings['storename']) ? trim($settings['storename']) : '';
    $this->sharedsecret = !empty($settings['sharedsecret']) ? trim($settings['sharedsecret']) : '';
    $this->transaction_type_process = !empty($settings['transaction_type_process']) ? trim($settings['transaction_type_process']) : '';
    $this->mode = !empty($settings['payment_mode']) ? trim($settings['payment_mode']) : '';
    $this->language = !empty($settings['language']) ? trim($settings['language']) : '';
    $this->extended_hash = !empty($settings['extended_hash']) ? trim($settings['extended_hash']) : '';
  }

  /**
   * Process direct payments.
   *
   * @param object $customer_profile
   *   Contains customer profile, billing and shipping address information.
   * @param object $order
   *   Contains customer’s order information.
   * @param array $pane_values
   *   Contains the pane values for the customer from the checkout,
   *   like credit card information: card number, card code, expiration date,
   *   card holder name, and card on file information.
   * @param object $amount_object
   *   Order’s amount and currency.
   * @param string $card_on_file_status
   *   The card on file status, can be ‘new’ representing a new card.
   * @param object $card_details
   *   Card on file details.
   * @param bool $bypass_3d_secure
   *   TRUE or FALSE to indicate whether or not to bypass 3d
   *   secure authentication.
   * @param string $transaction_type
   *   Sale or preauth.
   *
   * @return array
   *   Response from firstdata connect.
   */
  public function directPayments($customer_profile, $order, array $pane_values, $amount_object = '', $card_on_file_status = '', $card_details = '', $bypass_3d_secure = '', $transaction_type = '') {
    global $base_root;
    $payment_methods = commerce_payment_method_instance_load('commerce_firstdata_connect_full_payment|commerce_payment_commerce_firstdata_connect_full_payment');
    if ($this->transaction_type_process != 'sale') {
      $this->transaction_type_process = 'preauth';
    }
    if (empty($amount_object)) {
      $amount = $order->commerce_order_total[LANGUAGE_NONE][0]['amount'];
      $currency = $order->commerce_order_total[LANGUAGE_NONE][0]['currency_code'];
      $currency_new_code = commerce_currency_load($currency);
      $currency_code = $currency_new_code['numeric_code'];
    }
    else {
      $amount = $amount_object->amount;
      $currency = $amount_object->currency;
      $currency_new_code = commerce_currency_load($currency);
      $currency_code = $currency_new_code['numeric_code'];
    }
    if ($this->commerceFirstdataConnectDevice() == 'mobile') {
      $mobile_mode = TRUE;
    }

    // Get the timezone.
    $money = commerce_currency_amount_to_decimal($amount, $currency);
    // All of the billing data needed for the request.
    $billing_data = array(
      // Mandatory fields.
      'txntype' => !empty($this->transaction_type_process) ? $this->transaction_type_process : $this->commerceFirstdataConnectErrors($payment_methods, 'transaction_type_process'),
      'timezone' => commerce_firstdata_connect_get_default_time_zone(),
      'txndatetime' => date('Y:m:d-H:i:s'),
      'hash' => commerce_firstdata_connect_createHash($money, $currency_code, date('Y:m:d-H:i:s')),
      'storename' => !empty($this->storename) ? $this->storename : $this->commerceFirstdataConnectErrors($payment_methods, 'storename'),
      'mode' => !empty($this->mode) ? $this->mode : $this->commerceFirstdataConnectErrors($payment_methods, 'mode'),
      'chargetotal' => commerce_currency_amount_to_decimal($amount, $currency),
      'currency' => $currency_code,
      'oid' => $order->order_id . '-' . time(),
      // Optional fields.
      'paymentMethod' => $this->commerceFirstdataConnectCardMode($pane_values['credit_card']['type']),
      'customerid' => $order->uid,
      'invoicenumber' => $order->order_number,
      'comments' => 'commerce_firstdata_connect',
      'responseSuccessURL' => $base_root . '/checkout/%commerce_order/complete',
      'responseFailURL' => $base_root . '/checkout/%commerce_order/complete',
      'language' => $this->language,
      'trxOrigin' => 'ECI',
      'full_bypass' => 'true',
      'transactionNotificationURL' => $base_root . '/checkout/%commerce_order/complete',
    );

    // Set txndatetime which will be used to verify the transaction.
    $order->data['txndatetime'] = $billing_data['txndatetime'];
    commerce_order_save($order);

    if ($card_on_file_status == 'new') {
      $billing_data['hosteddataid'] = $card_details->card_id;
    }
    if (!empty($transaction_type)) {
      $billing_data['txntype'] = $transaction_type;
    }
    if (!empty($bypass_3d_secure) and $bypass_3d_secure == TRUE) {
      $billing_data['authenticateTransaction'] = 'false';
    }
    else {
      $billing_data['authenticateTransaction'] = 'true';
    }
    $billing_data['responseSuccessURL'] = $base_root . '/firstdata_connect/3ds/callback';
    $billing_data['responseFailURL'] = $base_root . '/firstdata_connect/3ds/callback';
    $billing_data['transactionNotificationURL'] = $base_root . '/firstdata_connect/3ds/callback';

    if (!empty($mobile_mode) and $mobile_mode == TRUE) {
      $billing_data['mobileMode'] = 'true';
    }

    $billing_data['cardnumber'] = trim($pane_values['credit_card']['number']);
    $billing_data['expmonth'] = trim($pane_values['credit_card']['exp_month']);
    $billing_data['expyear'] = trim($pane_values['credit_card']['exp_year']);
    $billing_data['cvm'] = trim($pane_values['credit_card']['code']);

    if ($this->mode == 'payplus' or $this->mode == 'fullpay') {
      $billing_data['bcompany'] = !empty($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['organisation_name']) ? trim($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['organisation_name']) : '';
      $billing_data['bname'] = !empty($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['name_line']) ? trim(substr($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['name_line'], 0, 30)) : '';
      $billing_data['baddr1'] = !empty($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['thoroughfare']) ? trim(substr($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['thoroughfare'], 0, 30)) : '';
      $billing_data['baddr2'] = !empty($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['premise']) ? trim(substr($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['premise'], 0, 30)) : '';
      $billing_data['bcity'] = !empty($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['locality']) ? trim(substr($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['locality'], 0, 30)) : '';
      $billing_data['bstate'] = !empty($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['administrative_area']) ? trim(substr($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['administrative_area'], 0, 30)) : '';
      $billing_data['bcountry'] = !empty($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['country']) ? trim(substr($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['country'], 0, 2)) : '';
      $billing_data['bzip'] = !empty($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['postal_code']) ? trim($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['postal_code']) : '';
      $billing_data['email'] = $order->mail;
    }

    if ($this->mode == 'fullpay') {
      $billing_data['sname'] = !empty($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['name_line']) ? trim($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['name_line']) : '';
      $billing_data['saddr1'] = !empty($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['thoroughfare']) ? trim(substr($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['thoroughfare'], 0, 30)) : '';
      $billing_data['saddr2'] = !empty($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['premise']) ? trim(substr($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['premise'], 0, 30)) : '';
      $billing_data['scity'] = !empty($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['locality']) ? trim(substr($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['locality'], 0, 30)) : '';
      $billing_data['sstate'] = !empty($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['administrative_area']) ? trim(substr($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['administrative_area'], 0, 30)) : '';
      $billing_data['scountry'] = !empty($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['country']) ? trim(substr($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['country'], 0, 2)) : '';
      $billing_data['szip'] = !empty($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['postal_code']) ? trim($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['postal_code']) : '';
    }

    ksort($billing_data);
    if (!empty($this->extended_hash) and $this->extended_hash == 1) {
      $billing_data['hashExtended'] = commerce_firstdata_connect_createHash($money, $currency_code, date('Y:m:d-H:i:s'), $billing_data);
      foreach ($billing_data as $key => $value) {
        if ($key != 'hash') {
          $billing_data_new[$key] = $value;
        }
      }
      $billing_data = array();
      $billing_data = $billing_data_new;
      ksort($billing_data);

    }
    foreach ($billing_data as $key => $value) {
      $data[$key] = urlencode($value);
    }
    return $this->request($payment_methods, $data);

  }

  /**
   * Create the actual http request.
   *
   * @param array $payment_methods
   *   Payment method details.
   * @param array $data
   *   Billing data that will be send to firstdata connect.
   */
  public function request(array $payment_methods, array $data) {
    $build_result = $this->buildUrl($payment_methods['settings']['url'], $data);
    $result = drupal_http_request($build_result['url'], $build_result['parameters']);
    return $result;
  }

  /**
   * Build the url for the http request.
   *
   * @param string $payment_account_type
   *   Test or prod.
   * @param array $data
   *   Billing data that will be send to firstdata connect.
   *
   * @return array
   *   url and parameters that will be send.
   */
  public function buildUrl($payment_account_type, array $data) {
    if ($payment_account_type == 'test') {
      $url = 'https://test.ipg-online.com/connect/gateway/processing';
    }
    else {
      $url = 'https://www.ipg-online.com/connect/gateway/processing';
    }

    // Build the url parameters.
    foreach ($data as $key => $value) {
      $params[] = $key . '=' . $value;
    }
    $options = '';
    foreach ($params as $key => $value) {
      if ($key === count($params) - 1) {
        $options .= $value;
      }
      else {
        $options .= $value . '&';
      }
    }

    $options_array = array(
      'headers' => array('Content-Type' => 'application/x-www-form-urlencoded'),
      'method' => 'POST',
      'data' => $options,
      'timeout' => 100,
    );

    $result = array(
      'url' => $url,
      'parameters' => $options_array,
    );
    return $result;

  }

  /**
   * Logs errors.
   *
   * @param array $payment_methods
   *   Payment method details.
   * @param array $data
   *   Billing data that will be send to firstdata connect.
   */
  public function commerceFirstdataConnectErrors(array $payment_methods, array $data = NULL) {
    if (!empty($data)) {
      switch ($data) {
        case 'transaction_type_process':
          if ($payment_methods['settings']['logs'] == 0) {
            watchdog('commerce_firstdata_connect', 'Transaction type was not configured! Go back to admin and set it up.', array(), WATCHDOG_ERROR);
          }
          drupal_set_message(t('Transaction type was not configured! Go back to admin and set it up.'), 'error');
          break;

        case 'storename':
          if ($payment_methods['settings']['logs'] == 0) {
            watchdog('commerce_firstdata_connect', 'Storename type was not configured! Go back to admin and set it up.', array(), WATCHDOG_ERROR);
          }
          drupal_set_message(t('Storename type was not configured! Go back to admin and set it up.'), 'error');
          break;
      }
    }
  }

  /**
   * Gets user's electronic device.
   *
   * @return string
   *   Client’s device.
   */
  public function commerceFirstdataConnectDevice() {
    // Get the device.
    if (strstr($_SERVER['HTTP_USER_AGENT'], 'Android') != FALSE) {
      $device = 'mobile';
    }
    elseif (strstr($_SERVER['HTTP_USER_AGENT'], 'Macintosh') != FALSE) {
      $device = 'Macintosh';
    }
    elseif (strstr($_SERVER['HTTP_USER_AGENT'], 'Linux') != FALSE) {
      $device = 'Linux';
    }
    elseif (strstr($_SERVER['HTTP_USER_AGENT'], 'Windows') != FALSE) {
      $device = 'Windows';
    }
    else {
      $device = 'mobile';
    }
    return $device;
  }

  /**
   * Provide acronym for credit card type.
   *
   * @param string $card_type
   *   Credit card type.
   *
   * @return string
   *   Short credit card name.
   */
  public function commerceFirstdataConnectCardMode($card_type) {
    switch ($card_type) {
      case 'visa':
        $type = 'V';
        break;

      case 'mastercard':
        $type = 'M';
        break;

      case 'amex':
        $type = 'A';
        break;

      case 'discover':
        $type = 'C';
        break;

      case 'dc':
        $type = 'C';
        break;

      case 'maestro':
        $type = 'MA';
        break;
    }
    return $type;
  }

  /**
   * Perform cross payment.
   *
   * @param object $order
   *   Order details.
   * @param object $transaction
   *   Transaction details.
   * @param string $type
   *   Sale, preauth or postauth.
   * @param object $amount_object
   *   Order’s amount and currency.
   * @param int $hosteddataid
   *   Card on file id.
   * @param int $card_code
   *   Credit Card CVV code.
   * @param int $exp_month
   *   Credit card expiration month.
   * @param int $exp_year
   *   Credit card expiration year.
   * @param bool $bypass
   *   TRUE to by pass 3d secure authentication.
   *
   * @return array
   *   Response from the firstdata connect.
   */
  public function crossPayment($order, $transaction = '', $type = 'postauth', $amount_object = '', $hosteddataid = '', $card_code = '', $exp_month = '', $exp_year = '', $bypass = '') {
    global $base_root;
    if (!empty($transaction)) {
      $payment_methods = commerce_payment_method_instance_load($transaction->instance_id);
    }
    if (empty($payment_methods)) {
      $payment_methods = commerce_payment_method_instance_load('commerce_firstdata_connect_full_payment|commerce_payment_commerce_firstdata_connect_full_payment');
    }

    // Get the amount.
    if (empty($amount_object)) {
      $amount = $order->commerce_order_total[LANGUAGE_NONE][0]['amount'];
      $currency = $order->commerce_order_total[LANGUAGE_NONE][0]['currency_code'];
      $currency_new_code = commerce_currency_load($currency);
      $currency_code = $currency_new_code['numeric_code'];
    }
    else {
      $amount = $amount_object->amount;
      $currency = $amount_object->currency;
      $currency_new_code = commerce_currency_load($currency);
      $currency_code = $currency_new_code['numeric_code'];
    }

    $money = commerce_currency_amount_to_decimal($amount, $currency);
    if (!empty($transaction)) {
      sleep(2);
      $billing_data = array(
        'txntype' => $type,
        'timezone' => commerce_firstdata_connect_get_default_time_zone(),
        'txndatetime' => date('Y:m:d-H:i:s'),
        'hash' => commerce_firstdata_connect_createHash($money, $currency_code, date('Y:m:d-H:i:s')),
        'storename' => !empty($this->storename) ? $this->storename : $this->commerceFirstdataConnectErrors($payment_methods, 'storename'),
        'chargetotal' => commerce_currency_amount_to_decimal($amount, $currency),
        'currency' => $currency_code,
        'oid' => $transaction->payload['oid'],
      );
    }
    // If we are using saved card.
    elseif (!empty($hosteddataid)) {
      $card_saved = commerce_cardonfile_load($hosteddataid);

      $billing_data = array(
      // Mandatory fields.
        'txntype' => $type,
        'timezone' => commerce_firstdata_connect_get_default_time_zone(),
        'txndatetime' => date('Y:m:d-H:i:s'),
        'hash' => commerce_firstdata_connect_createHash($money, $currency_code, date('Y:m:d-H:i:s')),
        'storename' => !empty($this->storename) ? $this->storename : $this->commerceFirstdataConnectErrors($payment_methods, 'storename'),
        'mode' => !empty($this->mode) ? $this->mode : $this->commerceFirstdataConnectErrors($payment_methods, 'mode'),
        'chargetotal' => commerce_currency_amount_to_decimal($amount, $currency),
        'currency' => $currency_code,
        'oid' => $order->order_id . '-' . time(),
        // Optional fields.
        'paymentMethod' => $this->commerceFirstdataConnectCardMode($card_saved->card_type),
        'customerid' => $order->uid,
        'invoicenumber' => $order->order_number,
        'comments' => 'commerce_firstdata_connect',
        'responseSuccessURL' => $base_root . '/firstdata_connect/3ds/callback',
        'responseFailURL' => $base_root . '/firstdata_connect/3ds/callback',
        'language' => $this->language,
        'trxOrigin' => 'ECI',
        'full_bypass' => 'true',
        'transactionNotificationURL' => $base_root . '/firstdata_connect/3ds/callback',
        'hosteddataid' => $hosteddataid,
        'cvm' => trim($card_code),
      );

      if (!empty($exp_month) and !empty($exp_year)) {
        $billing_data['expmonth'] = trim($exp_month);
        $billing_data['expyear'] = trim($exp_year);
      }
    }
    // Set txndatetime which will be used to verify the transaction.
    $order->data['txndatetime'] = $billing_data['txndatetime'];
    commerce_order_save($order);

    if ($this->mode == 'payplus' or $this->mode == 'fullpay') {
      $customer_profile = commerce_customer_profile_load($order->commerce_customer_billing[LANGUAGE_NONE][0]['profile_id']);
      $billing_data['bcompany'] = !empty($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['organisation_name']) ? trim($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['organisation_name']) : '';
      $billing_data['bname'] = !empty($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['name_line']) ? trim(substr($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['name_line'], 0, 30)) : '';
      $billing_data['baddr1'] = !empty($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['thoroughfare']) ? trim(substr($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['thoroughfare'], 0, 30)) : '';
      $billing_data['baddr2'] = !empty($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['premise']) ? trim(substr($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['premise'], 0, 30)) : '';
      $billing_data['bcity'] = !empty($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['locality']) ? trim(substr($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['locality'], 0, 30)) : '';
      $billing_data['bstate'] = !empty($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['administrative_area']) ? trim(substr($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['administrative_area'], 0, 30)) : '';
      $billing_data['bcountry'] = !empty($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['country']) ? trim(substr($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['country'], 0, 2)) : '';
      $billing_data['bzip'] = !empty($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['postal_code']) ? trim($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['postal_code']) : '';
      $billing_data['email'] = $order->mail;
    }

    if ($this->mode == 'fullpay') {
      $customer_profile = commerce_customer_profile_load($order->commerce_customer_billing[LANGUAGE_NONE][0]['profile_id']);
      $billing_data['sname'] = !empty($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['name_line']) ? trim($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['name_line']) : '';
      $billing_data['saddr1'] = !empty($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['thoroughfare']) ? trim(substr($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['thoroughfare'], 0, 30)) : '';
      $billing_data['saddr2'] = !empty($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['premise']) ? trim(substr($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['premise'], 0, 30)) : '';
      $billing_data['scity'] = !empty($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['locality']) ? trim(substr($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['locality'], 0, 30)) : '';
      $billing_data['sstate'] = !empty($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['administrative_area']) ? trim(substr($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['administrative_area'], 0, 30)) : '';
      $billing_data['scountry'] = !empty($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['country']) ? trim(substr($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['country'], 0, 2)) : '';
      $billing_data['szip'] = !empty($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['postal_code']) ? trim($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['postal_code']) : '';
    }

    if ($type == 'void') {
      $billing_data['tdate'] = $transaction->data['tdate'];
      $billing_data['authenticateTransaction'] = 'false';
      $billing_data['full_bypass'] = 'true';
      ksort($billing_data);
      $billing_data['responseSuccessURL'] = $base_root . '/admin/commerce/orders';
      $billing_data['responseFailURL'] = $base_root . '/admin/commerce/orders';
      $billing_data['transactionNotificationURL'] = $base_root . '/admin/commerce/orders';
    }
    if ($type == 'postauth') {
      $billing_data['responseSuccessURL'] = $base_root . '/admin/commerce/orders';
      $billing_data['responseFailURL'] = $base_root . '/admin/commerce/orders';
      $billing_data['transactionNotificationURL'] = $base_root . '/admin/commerce/orders';
    }
    if ($type != 'void' and $type != 'postauth') {
      $billing_data['authenticateTransaction'] = 'true';
      $billing_data['responseSuccessURL'] = $base_root . '/firstdata_connect/3ds/callback';
      $billing_data['responseFailURL'] = $base_root . '/firstdata_connect/3ds/callback';
      $billing_data['transactionNotificationURL'] = $base_root . '/firstdata_connect/3ds/callback';
    }
    if ($bypass == TRUE) {
      $billing_data['authenticateTransaction'] = 'false';
    }
    ksort($billing_data);

    if (!empty($this->extended_hash) and $this->extended_hash == 1) {
      $billing_data['hashExtended'] = commerce_firstdata_connect_createHash($money, $currency_code, date('Y:m:d-H:i:s'), $billing_data);
    }

    // Url encode.
    foreach ($billing_data as $key => $value) {
      $data[$key] = urlencode($value);
    }
    return $this->request($payment_methods, $data);
  }

  /**
   * Calculates the returned hash string.
   *
   * @param string $response_hash
   *   The returned hashed string.
   * @param string $approval_code
   *   The returned approval codes.
   * @param float $chargetotal
   *   The returned charged total amount.
   * @param int $currency
   *   Currency code in ISO 4217.
   * @param int $order_id
   *   Order id.
   *
   * @return bool
   *   TRUE if the calculated hashed string and the returned hashed
   *   string are equal, returns FALSE otherwise, if txndatetime
   *   parameter was not stored in the order, returns an exception.
   */
  public function calculateResponseHash($response_hash, $approval_code, $chargetotal, $currency, $order_id) {
    $order = commerce_order_load($order_id);
    if (!empty($order->data['txndatetime'])) {
      $str = $this->sharedsecret . $approval_code . $chargetotal . $currency . $order->data['txndatetime'] . $this->storename;
      $ascii = bin2hex($str);
      $hashed_str = sha1($ascii);
      if ($hashed_str == $response_hash) {
        return TRUE;
      }
      else {
        return FALSE;
      }
    }
    else {
      return t('Parameter txndatetime was not stored.');
    }
  }
}
