<?php
/**
 * @file
 * Checkout.fi gateway PaymentMethodController.
 */

/**
 * Payment method controller for Checkout.fi
 */
class CheckoutFiMethodController extends PaymentMethodController {

  /**
   * API constants for the payment gateway.
   */
  const SERVER_URL = 'https://payment.checkout.fi/';
  const VERSION = '0001';
  const ALGORITHM = '3';
  const COUNTRY = 'FIN';
  const TYPE = '0';

  /**
   * API constants for status polling.
   */
  const POLLING_URL = 'https://rpcapi.checkout.fi/poll';
  const POLLING_ALGORITHM = '1';
  const POLLING_FORMAT = '1';
  const POLLING_VERSION = '0001';

  /**
   * Contains type of request.
   *
   * @see setDevice()
   */
  protected $device = 1;

  /**
   * Default test account settings.
   */
  // @ignore sniffer_namingconventions_validvariablename_lowercamelname
  public $controller_data_defaults = array(
    'merchant_id' => '375917',
    'passphrase' => 'SAIPPUAKAUPPIAS',
    'message' => '',
    'content' => '1',
    'date_offset' => '+1 week',
    'onsite_gateway' => 1,
  );

  // @ignore sniffer_namingconventions_validvariablename_lowercamelname
  public $payment_method_configuration_form_elements_callback = 'payment_checkoutfi_payment_method_configuration_form_elements';
  public $title = 'Checkout.fi';

  /**
   * Controller constructor.
   */
  public function __construct() {
    // Only EUR is supported by Checkout.fi. However, if they
    // decide to allow other currencies, adding the new ones
    // to this array should suffice for this module
    // to support them.
    $this->currencies = array(
      'EUR' => array(
        'minimum' => '1',
      ),
    );
  }

  /**
   * Implements PaymentMethodController::execute().
   */
  public function execute(Payment $payment) {
    // Before executing the payment, the status
    // is set to this module's payment status, for
    // the module to recognize payments that should
    // be polled for status updates from Checkout.fi.
    $payment->setStatus(new PaymentStatusItem(PAYMENT_CHECKOUTFI_STATUS_PENDING));

    // Save stamp and reference to payment data. These
    // values are used when polling for successful payments
    // that did not return to the site through the browser.
    $payment->context_data['checkoutfi_stamp'] = $this->getStamp();
    $payment->context_data['checkoutfi_reference'] = $this->getReference($payment);

    // Save payment to persist changes.
    entity_save('payment', $payment);

    // Redirect to payment gateway.
    drupal_goto('checkoutfi/pay/' . $payment->pid);
  }

  /**
   * Set up POST data for polling unfinished payments.
   *
   * @param Payment $payment
   *   An unfinished payment about to be
   *   polled for updates.
   *
   * @return array
   *   Array of key-value pairs required by the API.
   *
   * @todo merge duplicate code in this and formData() to methods.
   */
  public function getPollingData(Payment $payment) {
    // Get payment amount.
    $currency = currency_load($payment->currency_code);
    $amount = intval(bcmul($currency->roundAmount($payment->totalAmount(TRUE)), pow(10, $currency->getDecimals())));

    $data = array(
      'VERSION' => self::POLLING_VERSION,
      'STAMP' => $payment->context_data['checkoutfi_stamp'],
      'REFERENCE' => $payment->context_data['checkoutfi_reference'],
      'MERCHANT' => $payment->method->controller_data['merchant_id'],
      'AMOUNT' => $amount,
      'CURRENCY' => $payment->currency_code,
      'FORMAT' => self::POLLING_FORMAT,
      'ALGORITHM' => self::POLLING_ALGORITHM,
    );
    $mac = md5(implode('+', $data) . '+' . $payment->method->controller_data['passphrase']);
    $data['MAC'] = $mac;
    return $data;
  }

  /**
   * Process feedback data, finalize payment.
   */
  public function processFeedback(array $data, Payment $payment) {
    $payment->setStatus(new PaymentStatusItem($this->getPaymentStatus($data['STATUS'], $payment->pid)));
    entity_save('payment', $payment);
    $payment->finish();
  }

  /**
   * Get feedback data.
   *
   * @return array
   *   Array of merged GET and POST data.
   */
  public function getFeedbackData() {
    // $data = &drupal_static(__FUNCTION__);
    $data = array_merge($_GET, $_POST);
    unset($data['q']);
    return $data;
  }

  /**
   * Validate returned data.
   *
   * @param array $data
   *   Array of data returned by the request.
   * @param Payment $payment
   *   The payment related to this request.
   *
   * @return bool
   *   Whether or not the returned auth-key can
   *   be recreated from the payment data.
   */
  public function validateMac(array $data, Payment $payment) {
    $hash_data = array(
      $data['VERSION'],
      $data['STAMP'],
      $data['REFERENCE'],
      $data['PAYMENT'],
      $data['STATUS'],
      $data['ALGORITHM'],
    );
    $hash = strtoupper(hash_hmac('sha256', implode('&', $hash_data), $payment->method->controller_data['passphrase']));
    return ($data['MAC'] === $hash);
  }

  /**
   * Set request type.
   *
   * Set whether buttons should be integrated in the system (XML) or
   * if the customer should be redirected to Checkout.fi (HTML)
   *
   * @param string $device
   *   Request type: HTML or XML.
   *
   * @return bool
   *   Whether device was updated or not.
   */
  public function setDevice($device = 'HTML') {
    switch ($device) {
      case 'HTML':
        $this->device = '1';
        return TRUE;

      case 'XML':
        $this->device = '10';
        return TRUE;
    }
    return FALSE;
  }

  /**
   * Set up POST data for the Checkout.fi payment gateway.
   *
   * @param Payment $payment
   *   The payment in question.
   *
   * @return array
   *   Array of key-value pairs required by the API.
   */
  public function getRequestData(Payment $payment) {

    $return_url = url('checkoutfi/return/' . $payment->pid, array('absolute' => TRUE));
    $currency = currency_load($payment->currency_code);

    // For (potential) future currency support.
    $amount = intval(bcmul($currency->roundAmount($payment->totalAmount(TRUE)), pow(10, $currency->getDecimals())));

    // Load required POST variables and corresponding
    // values into an array.
    $data = array(
      'AMOUNT' => $amount,
      'CURRENCY' => $payment->currency_code,
      'LANGUAGE' => $this->getLocale(),
      'STAMP' => $payment->context_data['checkoutfi_stamp'],
      'REFERENCE' => $payment->context_data['checkoutfi_reference'],
      'MERCHANT' => $payment->method->controller_data['merchant_id'],
      'COUNTRY' => self::COUNTRY,
      'DEVICE' => $this->device,
      'CONTENT' => $payment->method->controller_data['content'],
      'ALGORITHM' => self::ALGORITHM,
      'VERSION' => self::VERSION,
      'TYPE' => self::TYPE,
      'RETURN' => $return_url,
      'CANCEL' => $return_url,
      'REJECT' => $return_url,
      'DELAYED' => $return_url,
      'EMAIL' => '',
      'DELIVERY_DATE' => empty($payment->method->controller_data['date_offset']) ? date('Ymd') : date('Ymd', strtotime($payment->method->controller_data['date_offset'])),
      'MESSAGE' => $payment->method->controller_data['message'],
      'FIRSTNAME' => '',
      'FAMILYNAME' => '',
      'ADDRESS' => '',
      'POSTCODE' => '',
      'POSTOFFICE' => '',
    );

    // Allow other modules to change POST data.
    drupal_alter('payment_checkoutfi_form_data', $data, $payment);

    // Add authentication code for this Payment
    // to the POST data array.
    $data['MAC'] = $this->getAuthenticationKey($payment, $data);

    return $data;
  }

  /**
   * Generate authentication key for a gateway request.
   *
   * @param Payment $payment
   *   The payment object to be paid.
   * @param array $data
   *   Array of payment data as returned by getRequestData.
   *
   * @return string
   *   An md5-hash for authenticating this request.
   *
   * @see getRequestData()
   */
  protected function getAuthenticationKey(Payment $payment, array $data) {
    $hash_data = array(
      $data['VERSION'],
      $data['STAMP'],
      $data['AMOUNT'],
      $data['REFERENCE'],
      $data['MESSAGE'],
      $data['LANGUAGE'],
      $data['MERCHANT'],
      $data['RETURN'],
      $data['CANCEL'],
      $data['REJECT'],
      $data['DELAYED'],
      $data['COUNTRY'],
      $data['CURRENCY'],
      $data['DEVICE'],
      $data['CONTENT'],
      $data['TYPE'],
      $data['ALGORITHM'],
      $data['DELIVERY_DATE'],
      $data['FIRSTNAME'],
      $data['FAMILYNAME'],
      $data['ADDRESS'],
      $data['POSTCODE'],
      $data['POSTOFFICE'],
      $payment->method->controller_data['passphrase'],
    );
    return strtoupper(md5(implode('+', $hash_data)));
  }

  /**
   * Checkout.fi status to PaymentStatusInfo map.
   *
   * @param int $status
   *   The status code returned from a Checkout.fi request.
   * @param int $pid
   *   The id of the payment (for debugging purposes only).
   *
   * @return string
   *   The corresponding PaymentStatusItem identifier.
   */
  public function getPaymentStatus($status, $pid) {
    $statuses = self::getStatusMap();
    if (isset($statuses[$status])) {
      return $statuses[$status];
    }
    watchdog('Checkout.fi', 'Unknown payment error in payment %pid: Response code %status', array('%pid' => $pid, '%status' => $status), WATCHDOG_ERROR);
    return PAYMENT_STATUS_UNKNOWN;
  }

  /**
   * Return best match locale for current user.
   *
   * @return string
   *   One of the languages supported by
   *   Checkout.fi (SE, FI, or EN).
   */
  protected function getLocale() {
    global $language;
    switch ($language->language) {
      case 'sv':
        return 'SE';

      case 'fi':
        return 'FI';
    }
    return 'EN';
  }

  /**
   * Generate stamp value for checkout form.
   *
   * @return string
   *   The stamp identifier for this payment.
   */
  protected function getStamp() {
    // The stamp (AN20) needs to be unique
    // for each payment. For now, the request
    // time is used. This is subject to change.
    return REQUEST_TIME;
  }

  /**
   * Calculate Finnish reference number.
   *
   * @param object $payment
   *   A Payment object.
   *
   * @return int
   *   Finnish reference number.
   */
  protected function getReference(Payment $payment) {
    // Add 100 to id to force 3-digit minimum. This is
    // the default base for reference numbers. If you need
    // another base format, see API documentation for
    // hook information.
    //
    // @see hook_payment_checkoutfi_reference_base()
    $base = $payment->pid + 100;

    // Allow other modules to set reference number base.
    drupal_alter('payment_checkoutfi_reference_base', $base, $payment);

    // Reference base still needs to be at least 3 digits.
    if ($base < 100) {
      drupal_set_message(t('Reference number base is, per definition, required to be an at least 3 digits long number (Currently: @base).', array('@base' => $base)), 'error');
      return;
    }

    // Calculate checksum.
    $check_multiples = array(7, 3, 1);
    $sum = 0;
    foreach (str_split(strrev($base)) as $index => $char) {
      $sum += $check_multiples[$index % 3] * intval($char);
    }
    $sum = ceil($sum / 10) * 10 - $sum;

    // Return entire reference number (base + checksum).
    return intval($base) . $sum;
  }

  /**
   * Checkout.fi status code to PaymentStatusItem string map.
   *
   * @return array
   *   Array of PaymentStatusItem strings, keyed by their
   *   corresponding Checkout.fi status code.
   */
  public static function getStatusMap($status = FALSE) {
    $status_map = array(
      -10 => PAYMENT_CHECKOUTFI_STATUS_REFUNDED,
      -4 => PAYMENT_CHECKOUTFI_STATUS_NOT_FOUND,
      -3 => PAYMENT_CHECKOUTFI_STATUS_TIMEOUT,
      -2 => PAYMENT_CHECKOUTFI_STATUS_CANCELED_BY_SYSTEM,
      -1 => PAYMENT_CHECKOUTFI_STATUS_CANCELED_BY_USER,
      1 => PAYMENT_CHECKOUTFI_STATUS_INTERRUPTED,
      2 => PAYMENT_CHECKOUTFI_STATUS_COMPLETE,
      3 => PAYMENT_CHECKOUTFI_STATUS_PENDING,
      4 => PAYMENT_CHECKOUTFI_STATUS_PENDING,
      5 => PAYMENT_CHECKOUTFI_STATUS_COMPLETE,
      6 => PAYMENT_CHECKOUTFI_STATUS_HELD,
      7 => PAYMENT_CHECKOUTFI_STATUS_THIRD_PARTY_NEEDS_CONFIRMATION,
      8 => PAYMENT_CHECKOUTFI_STATUS_THIRD_PARTY_CONFIRMED,
      9 => PAYMENT_CHECKOUTFI_STATUS_COMPLETE,
      10 => PAYMENT_CHECKOUTFI_STATUS_COMPLETE,
    );
    if (!$status) {
      return $status_map;
    }
    return isset($status_map[$status]) ? $status_map[$status] : FALSE;
  }
}
