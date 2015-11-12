<?php

/**
 * Contains iDEALPaymentMethodController.
 */

/**
 * A payment method controller for iDEAL.
 */
class iDEALPaymentMethodController extends PaymentMethodController {

  /**
   * The timeout for requests sent to the iDEAL server.
   */
  const IDEAL_TIMEOUT = 7.6;

  /**
   * Identifies a directory request.
   */
  const REQUEST_DIRECTORY = 'DirectoryReq';

  /**
   * Identifies a transaction request.
   */
  const REQUEST_TRANSACTION = 'AcquirerTrxReq';

  /**
   * Identifies a status request.
   */
  const REQUEST_STATUS = 'AcquirerStatusReq';

  /**
   * Identifies a directory response.
   */
  const RESPONSE_DIRECTORY = 'DirectoryRes';

  /**
   * Identifies a transaction response.
   */
  const RESPONSE_TRANSACTION = 'AcquirerTrxRes';

  /**
   * Identifies a status response.
   */
  const RESPONSE_STATUS = 'AcquirerStatusRes';

  /**
   * The length of the private key, used for validation.
   */
  const IDEAL_PRIVATE_KEY_LENGTH = NULL;

  /**
   * {@inheritdoc}
   */
  public $controller_data_defaults = array(
    'expiration_period' => 900,
    'merchant_id' => '',
    'merchant_sub_id' => '',
    'merchant_private_key_path' => '',
    'merchant_private_key_password' => '',
    'merchant_public_certificate_path' => '',
    'acquirer_url' => '',
    'acquirer_public_certificate_path' => '',
  );

  /**
   * {@inheritdoc}
   */
  public $currencies = array(
    'EUR' => array(),
  );

  /**
   * {@inheritdoc}
   */
  public $payment_method_configuration_form_elements_callback = 'ideal_payment_method_configuration_form_elements';

  /**
   * {@inheritdoc}
   */
  public $payment_configuration_form_elements_callback = 'ideal_payment_configuration_form_elements';

  /**
   * {@inheritdoc}
   */
  function validate(Payment $payment, PaymentMethod $payment_method, $strict) {
    parent::validate($payment, $payment_method, $strict);
    // Check if the payer can be presented with a list of issuers to choose
    // from.
    try {
      $this->issuerOptions($payment_method);
    }
    catch (PaymentException $e) {
      throw new PaymentValidationException($e->getMessage());
    }
  }

  /**
   * {@inheritdoc}
   */
  function execute(Payment $payment) {
    try {
      $dom = $this->transactionRequest($payment);
      $xpath = new DOMXPath($dom);
      $xpath->registerNamespace('iDEAL', $this::IDEAL_XML_NAMESPACE);
      $payment->method_data['transaction_id'] = $xpath->query('/iDEAL:AcquirerTrxRes/iDEAL:Transaction/iDEAL:transactionID/text()')->item(0)->nodeValue;
      $payment->method_data['issuer_authentication_url'] = $xpath->query('/iDEAL:AcquirerTrxRes/iDEAL:Issuer/iDEAL:issuerAuthenticationURL/text()')->item(0)->nodeValue;
      entity_save('payment', $payment);
      drupal_goto($payment->method_data['issuer_authentication_url']);
    }
    catch (PaymentException $e) {
      $payment->setStatus(new PaymentStatusItem(PAYMENT_STATUS_FAILED));
      $payment->finish();
    }
  }

  /**
   * Returns the entrance code for a transaction request.
   *
   * @param Payment $payment
   *
   * @return string
   */
  function entranceCode(Payment $payment) {
    if (!isset($payment->method_data['entrance_code']) || !$payment->method_data['entrance_code']) {
      $payment->method_data['entrance_code'] = '';
      $characters = 'abcdefghijklmnoprstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789';
      for ($i = 0; $i < 40; $i++) {
        $payment->method_data['entrance_code'] .= $characters[mt_rand(0, strlen($characters) - 1)];
      }
    }

    return $payment->method_data['entrance_code'];
  }

  /**
   * Creates an identifier for a public key based on the key's certificate.
   *
   * @throws PaymentException
   *
   * @param string $certificate
   *
   * @return string
   */
  static function publicKeyIdentifier($certificate) {
    $certificate = str_replace(array('-----BEGIN CERTIFICATE-----', '-----END CERTIFICATE-----'), '', $certificate);
    $certificate = base64_decode($certificate);
    $fingerprint = sha1($certificate);

    return $fingerprint;
  }

  /**
   * Creates a request.
   *
   * @param PaymentMethod $payment_method
   *   The payment method to use for this request.
   * @param string $request_type
   *   One of the iDEALPaymentMethodController::REQUEST_* constants.
   *
   * @return DOMDocument
   */
  function createRequest(PaymentMethod $payment_method, $request_type) {
    $dom = new DOMDocument('1.0', 'UTF-8');
    $root = $dom->createElementNS($this::IDEAL_XML_NAMESPACE, $request_type);
    $dom->appendChild($root);
    $root->setAttribute('version', $this::IDEAL_INTERFACE_VERSION);
    $merchant = $dom->createElementNS($this::IDEAL_XML_NAMESPACE, 'Merchant');
    $root->appendChild($merchant);
    // Prefix the merchant ID with zeroes until it is 9 characters long.
    $merchant_id = str_repeat('0', 9 - strlen($payment_method->controller_data['merchant_id'])) . $payment_method->controller_data['merchant_id'];
    $sub_id = strlen($payment_method->controller_data['merchant_sub_id']) ? $payment_method->controller_data['merchant_sub_id'] : '0';
    $merchant_id = $dom->createElementNS($this::IDEAL_XML_NAMESPACE, 'merchantID', $merchant_id);
    $merchant->appendChild($merchant_id);
    $sub_id = $dom->createElementNS($this::IDEAL_XML_NAMESPACE, 'subID', $sub_id);
    $merchant->appendChild($sub_id);

    return $dom;
  }

  /**
   * Executes a request.
   *
   * @param PaymentMethod $payment_method
   *   The payment method to use for this request.
   * @param string $request_type
   *   One of the iDEALPaymentMethodController::REQUEST_* constants.
   * @param DOMDocument $request_dom
   *   The request message as a DOM.
   *
   * @return DOMDocument
   *   The response message as a DOM.
   */
  function request(PaymentMethod $payment_method, $request_type, DOMDocument $request_dom) {
    $this->DOMVerifySchema($request_dom);
    $response = drupal_http_request($payment_method->controller_data['acquirer_url'], array(
      'headers' => array(
        'content-type' => 'text/xml; charset="UTF-8"',
      ),
      'method' => 'POST',
      'data' => $request_dom->saveXML(),
      'timeout' => $this::IDEAL_TIMEOUT,
    ));
    $this->requestVerifySuccess($payment_method, $request_type, $response);
    $response_dom = $this->XMLVerifyValidity($response->data);
    $this->DOMVerifySchema($response_dom);
    $this->responseVerifyAuthenticity($payment_method, $request_type, $response_dom);
    $this->responseVerifySuccess($payment_method, $request_type, $response_dom);
    $this->alterResponse($request_type, $response_dom, $payment_method);

    return $response_dom;
  }

  /**
   * Checks if a HTTP request was successful.
   *
   * @throws PaymentException
   *
   * @param PaymentMethod $payment_method
   * @param string $request_type
   *   One of the static::REQUEST_* constants.
   * @param stdClass $response
   *   A drupal_http_request() response.
   */
  function requestVerifySuccess(PaymentMethod $payment_method, $request_type, stdClass $response) {
    if (isset($response->error)) {
      throw new PaymentException(t('@error_message (@error_code)', array(
        '@error_message' => $response->error,
        '@error_code' => $response->code,
      )));
    }
  }

  /**
   * Verifies whether an XML string is valid.
   *
   * @throws PaymentException
   *
   * @param string $xml
   *
   * @return DOMDocument
   */
  function XMLVerifyValidity($xml) {
    $dom = new DOMDocument();
    if (!@$dom->loadXML($xml)) {
      throw new PaymentException(t('The message is not valid XML.'));
    }

    return $dom;
  }

  /**
   * Verifies whether a DOM is built according the iDEAL XML schema.
   *
   * @throws PaymentException
   *
   * @param DOMDocument $dom
   *   An iDEAL response.
   *
   * @return DOMDocument
   */
  function DOMVerifySchema(DOMDocument $dom) {
    if (!$dom->schemaValidate(drupal_get_path('module', 'ideal') . '/' .$this::IDEAL_XML_SCHEMA_PATH)) {
      throw new PaymentException(t('The XML message is not formatted according the iDEAL XML schema.'));
    }

    return $dom;
  }

  /**
   * Verifies an iDEAL server's response.
   *
   * @throws PaymentException
   *
   * @param PaymentMethod $payment_method
   * @param string $request_type
   *   One of the iDEALPaymentMethodController::REQUEST_* constants.
   * @param DOMDocument $dom
   *   An iDEAL response.
   *
   * @return DOMDocument
   */
  function responseVerifyAuthenticity(PaymentMethod $payment_method, $request_type, DOMDocument $dom) {
    throw new iDEALMethodNotImplementedException();
  }

  /**
   * Verifies whether the response was successful.
   *
   * @throws PaymentException
   *
   * @param PaymentMethod $payment_method
   * @param string $request_type
   *   One of the static::REQUEST_* constants.
   * @param DOMDocument $dom
   *   An iDEAL response.
   *
   * @return NULL
   */
  function responseVerifySuccess(PaymentMethod $payment_method, $request_type, DOMDocument $dom) {
    $xpath = new DOMXPath($dom);
    $xpath->registerNamespace('iDEAL', $this::IDEAL_XML_NAMESPACE);
    $error = $xpath->query('/iDEAL:*/iDEAL:Error')->item(0);
    if ($error) {
      throw new PaymentException(t('An error response was received. Error code: !error_code. Error detail: @error_detail. Error message: @error_message. Consumer message: @consumer_message.', array(
        '@consumer_message' => $xpath->query('iDEAL:consumerMessage/text()', $error)->item(0)->nodeValue,
        '@error_message' => $xpath->query('iDEAL:errorMessage/text()', $error)->item(0)->nodeValue,
        '!error_code' => $xpath->query('iDEAL:errorCode/text()', $error)->item(0)->nodeValue,
        '@error_detail' => $xpath->query('iDEAL:errorDetail/text()', $error)->item(0)->nodeValue,
      )));
    }
  }

  /**
   * Execute a directory request.
   *
   * @see iDEALPaymentMethodController::issuerOptions()
   *
   * @throws iDEALMethodNotImplementedException
   *
   * @param PaymentMethod $payment_method
   *   The payment method to use for this request.
   *
   * @return DOMDocument
   */
  function directoryRequest(PaymentMethod $payment_method) {
    throw new iDEALMethodNotImplementedException();
  }

  /**
   * Returns form element options for selecting an issuer.
   *
   * @see iDEALPaymentMethodController::directoryRequest()
   *
   * @throws iDEALMethodNotImplementedException
   *
   * @param PaymentMethod $payment_method
   *   The payment method with the iDEAL account to get the issuers for.
   *
   * @return array
   */
  function issuerOptions(PaymentMethod $payment_method) {
    throw new iDEALMethodNotImplementedException();
  }

  /**
   * Executes a transaction request.
   *
   * @see iDEALPaymentMethodController::execute()
   *
   * @throws iDEALMethodNotImplementedException
   *
   * @param Payment $payment
   *   The payment to execute a request for.
   *
   * @return string
   */
  function transactionRequest(Payment $payment) {
    throw new iDEALMethodNotImplementedException();
  }

  /**
   * Maps iDEAL statuses to Payment statuses.
   */
  static function statusMap() {
    return array(
      'Success' => PAYMENT_STATUS_SUCCESS,
      'Open' => PAYMENT_STATUS_PENDING,
      'Cancelled' => PAYMENT_STATUS_CANCELLED,
      'Expired' => PAYMENT_STATUS_EXPIRED,
      'Failure' => PAYMENT_STATUS_FAILED,
      'Unknown' => PAYMENT_STATUS_UNKNOWN,
    );
  }

  /**
   * Checks a payment's current status with the bank.
   *
   * @see iDEALPaymentMethodController::statusRequest()
   *
   * @throws iDEALMethodNotImplementedException
   *
   * @param Payment $payment
   *
   * @return boolean
   *   Whether the payment's status has changed.
   */
  function statusUpdate(Payment $payment) {
    throw new iDEALMethodNotImplementedException();
  }

  /**
   * Executes a status request.
   *
   * @see iDEALPaymentMethodController::statusUpdate()
   *
   * @throws PaymentException
   *
   * @param Payment $payment
   *   The payment to check the status for.
   *
   * @return DOMDocument
   */
  function statusRequest(Payment $payment) {
    throw new iDEALMethodNotImplementedException();
  }

  /**
   * Returns the merchant return URL
   *
   * @return string
   */
  function merchantReturnURL() {
    return url('ideal/return', array(
      'absolute' => TRUE,
    ));
  }

  /**
   * Converts a period in seconds to an ISO 8601 formatted period.
   *
   * @param mixed $seconds
   *   The period in seconds as an integer or a string.
   *
   * @return string
   */
  function secondsToISO8601($seconds) {
    return 'PT' . $seconds . 'S';
  }

  /**
   * Update the status for pending payments that expired more than 24 hours
   * ago.
   */
  static function updateExpiredPayments() {
    $query = db_select('payment', 'p')
      ->fields('p', array('pid'))
      ->condition('psil.status', array_merge(array(PAYMENT_STATUS_PENDING), payment_status_info(PAYMENT_STATUS_PENDING)->descendants()))
      ->condition('controller_class_name', array_merge(array('iDEALPaymentMethodController'), iDEALPaymentMethodController::descendants()))
      ->where('psif.created + expiration_period < :time', array(
        ':time' => REQUEST_TIME - 86400,
      ));
    $query->innerJoin('payment_status_item', 'psif', 'p.psiid_first = psif.psiid');
    $query->innerJoin('payment_status_item', 'psil', 'p.psiid_last = psil.psiid');
    $query->innerJoin('payment_method', 'pm', 'p.pmid = pm.pmid');
    $query->innerJoin('ideal_payment_method', 'ipm', 'pm.pmid = ipm.pmid');
    $pids = $query->execute()->fetchCol();
    foreach (entity_load('payment', $pids) as $payment) {
      /** @var Payment $payment */
      $payment->setStatus(new PaymentStatusItem(PAYMENT_STATUS_UNKNOWN));
      entity_save('payment', $payment);
    }
  }

  /**
   * Check the status for open payments from the last 24 hours.
   */
  static function checkPendingPayments() {
    $query = db_select('payment', 'p')
      ->fields('p', array('pid'))
      ->condition('psil.status', array_merge(array(PAYMENT_STATUS_PENDING), payment_status_info(PAYMENT_STATUS_PENDING)->descendants()))
      ->condition('controller_class_name', array_merge(array('iDEALPaymentMethodController'), iDEALPaymentMethodController::descendants()))
      // // Statuses should never be requested after 24 hours after expiration.
      ->where('(psif.created + :expiration_period) > :time1', array(
        ':expiration_period' => REQUEST_TIME - 86400,
        ':time1' => REQUEST_TIME - 86400,
      ))
      // // The transaction must have expired for at least an hour.
      ->where('psif.created + expiration_period < :time2', array(
        ':time2' => REQUEST_TIME - 3600,
      ))
      // // A transaction's status may only be requested five times. The first time
      // // usually happens directly after the user has come back from his bank's
      // // iDEAL environment, so we only check every five hours here (which is four
      // // times in 23 hours).
      ->where('ip.updated < :time3', array(
        ':time3' => REQUEST_TIME - 18000,
      ));
    $query->innerJoin('payment_status_item', 'psif', 'p.psiid_first = psif.psiid');
    $query->innerJoin('payment_status_item', 'psil', 'p.psiid_last = psil.psiid');
    $query->innerJoin('payment_method', 'pm', 'p.pmid = pm.pmid');
    $query->innerJoin('ideal_payment_method', 'ipm', 'pm.pmid = ipm.pmid');
    $query->innerJoin('ideal_payment', 'ip', 'p.pid = ip.pid');
    $pids = $query->execute()->fetchCol();
    foreach (entity_load('payment', $pids) as $payment) {
      if ($payment->method->controller->statusUpdate($payment)) {
        entity_save('payment', $payment);
      }
    }
  }

  /**
   * Returns a list of iDEAL encryption file types.
   *
   * @return array
   */
  function fileTypes() {
    return array(
      'merchant_private_key' => t('merchant private key'),
      'merchant_public_certificate' => t('merchant public certificate'),
      'acquirer_public_certificate' => t('acquirer public certificate'),
    );
  }

  /**
   * Load the contents of one of a payment method's encryption files.
   *
   * @throws PaymentException
   *
   * @param PaymentMethod $payment_method
   * @param string $file_type
   *   Either "merchant_private_key", "merchant_public_certificate", or
   *   "acquirer_public_certificate".
   *
   * @return string
   *   THe file's contents.
   */
  function fileContents(PaymentMethod $payment_method, $file_type) {
    $file_types = $this->fileTypes();
    $key = $file_type . '_path';
    if (!empty($payment_method->controller_data[$key])) {
      $file = $payment_method->controller_data[$key];
      if (is_readable($file)) {
        return file_get_contents($file);
      }
      throw new PaymentException(t('The %file_type file is not readable or does not exist.', array(
        '%file_type' => $file_types[$file_type],
      )));
    }
    throw new PaymentException(t('The %file_type is not configured.', array(
      '%file_type' => $file_types[$file_type],
    )));
  }

  /**
   * Returns a payment method's merchant private key.
   *
   * @throws PaymentException
   *
   * @param PaymentMethod $payment_method
   *
   * @return string
   */
  function merchantPrivateKey(PaymentMethod $payment_method) {
    if ($key = @openssl_pkey_get_private($this->fileContents($payment_method, 'merchant_private_key'), $payment_method->controller_data['merchant_private_key_password'])) {
      $details = openssl_pkey_get_details($key);
      if (!is_null($this::IDEAL_PRIVATE_KEY_LENGTH)) {
        if ($details['bits'] != $this::IDEAL_PRIVATE_KEY_LENGTH) {
          throw new PaymentException(t('The merchant private key should be !bits bits.', array(
            '!bits' => $this::IDEAL_PRIVATE_KEY_LENGTH,
          )));
        }
      }
      if ($details['type'] != OPENSSL_KEYTYPE_RSA) {
        throw new PaymentException(t('The merchant private key should be generated using RSA.'));
      }
      return $key;
    }
    throw new PaymentException(t('The merchant private key file does not contain a valid private key or the password is incorrect.'));
  }

  /**
   * Returns a payment method's merchant or acquirer public certificate.
   *
   * @throws PaymentException
   *
   * @param PaymentMethod $payment_method
   * @param string $owner
   *  Either "merchant" or "acquirer".
   *
   * @return string
   */
  function publicCertificate(PaymentMethod $payment_method, $owner) {
    $certificate = NULL;
    $file_type = $owner . '_public_certificate';
    @openssl_x509_export(@openssl_x509_read($this->fileContents($payment_method, $file_type)), $certificate);
    if ($certificate) {
      return $certificate;
    }
    $file_types = $this->fileTypes();
    throw new PaymentException(t('The %certificate_type file does not contain a valid public certificate.', array(
      '%certificate_type' => $file_types[$file_type],
    )));
  }

  /**
   * Alters a request.
   *
   * @param string $type
   *   One of the iDEALPaymentMethodController::REQUEST_* constants.
   * @param DOMDocument $dom
   *   The DOM that represents the request, without the signature.
   * @param PaymentMethod $payment_method
   */
  public function alterRequest($type, DOMDocument $dom, PaymentMethod $payment_method) {
    drupal_alter('ideal_request', $type, $dom, $payment_method);
  }

  /**
   * Alters a response.
   *
   * @param string $type
   *   One of the iDEALPaymentMethodController::RESPONSE_* constants.
   * @param DOMDocument $dom
   *   The DOM that represents the response..
   * @param PaymentMethod $payment_method
   */
  public function alterResponse($type, DOMDocument $dom, PaymentMethod $payment_method) {
    drupal_alter('ideal_response', $type, $dom, $payment_method);
  }
}