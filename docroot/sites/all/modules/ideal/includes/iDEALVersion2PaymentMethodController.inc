<?php

/**
 * Contains iDEALVersion2PaymentMethodController.
 */

/**
 * A payment method controller for iDEAL version 2.
 */
class iDEALVersion2PaymentMethodController extends iDEALPaymentMethodController {

  const IDEAL_INTERFACE_VERSION = '1.1.0';

  const IDEAL_XML_NAMESPACE = 'http://www.idealdesk.com/Message';

  const IDEAL_XML_SCHEMA_PATH = 'ideal_xml_schema_v2.xsd';

  const IDEAL_PRIVATE_KEY_LENGTH = 1024;

  function __construct() {
    $this->title = t('iDEAL 2');
  }

  /**
   * Implements iDEALPaymentMethodController::createRequest().
   */
  function createRequest(PaymentMethod $payment_method, $request_type) {
    $dom = parent::createRequest($payment_method, $request_type);
    $xpath = new DOMXpath($dom);
    $xpath->registerNamespace('iDEAL', $this::IDEAL_XML_NAMESPACE);

    $root = $xpath->query('/iDEAL:*')->item(0);
    $create_date_time_stamp = $dom->createElementNS($this::IDEAL_XML_NAMESPACE, 'createDateTimeStamp', gmdate('o-m-d\TH:i:s.000\Z', time()));
    $root->insertBefore($create_date_time_stamp, $root->firstChild);

    return $dom;
  }

  /**
   * Sign a request.
   */
  function signRequest(DOMDocument $dom, PaymentMethod $payment_method, array $token_code_values = array()) {
    $xpath = new DOMXpath($dom);
    $xpath->registerNamespace('iDEAL', $this::IDEAL_XML_NAMESPACE);

    $merchant = $xpath->query('/iDEAL:*/iDEAL:Merchant')->item(0);

    $token_code = $dom->createElementNS($this::IDEAL_XML_NAMESPACE, 'tokenCode', $this->tokenCode($token_code_values, $this->merchantPrivateKey($payment_method)));
    // The signature elements are the last children of the Merchant element,
    // except for the transaction request, which has a merchantReturnURL.
    $merchant_return_url = $xpath->query('iDEAL:merchantReturnURL', $merchant)->item(0);
    if ($merchant_return_url) {
      $merchant->insertBefore($token_code, $merchant_return_url);
    }
    else {
      $merchant->appendChild($token_code);
    }
    $token = $dom->createElementNS($this::IDEAL_XML_NAMESPACE, 'token', $this->publicKeyIdentifier($this->publicCertificate($payment_method, 'merchant')));
    $merchant->insertBefore($token, $token_code);
    $authentication = $dom->createElementNS($this::IDEAL_XML_NAMESPACE, 'authentication', 'SHA1_RSA');
    $merchant->insertBefore($authentication, $token);
  }

  /**
   * Implements iDEALPaymentMethodController::directoryRequest().
   */
  function directoryRequest(PaymentMethod $payment_method) {
    $dom = $this->createRequest($payment_method, $this::REQUEST_DIRECTORY);
    $xpath = new DOMXpath($dom);
    $xpath->registerNamespace('iDEAL', $this::IDEAL_XML_NAMESPACE);
    $root = $xpath->query('/iDEAL:*')->item(0);

    $this->alterRequest($this::REQUEST_DIRECTORY, $dom, $payment_method);

    $merchant = $xpath->query('iDEAL:Merchant', $root)->item(0);
    $token_code_values = array(
      $xpath->query('iDEAL:createDateTimeStamp', $root)->item(0)->nodeValue,
      $xpath->query('iDEAL:merchantID', $merchant)->item(0)->nodeValue,
      $xpath->query('iDEAL:subID', $merchant)->item(0)->nodeValue,
    );
    $this->signRequest($dom, $payment_method, $token_code_values);

    return $this->request($payment_method, $this::REQUEST_DIRECTORY, $dom);
  }

  /**
   * Implements iDEALPaymentMethodController::transactionRequest().
   */
  function transactionRequest(Payment $payment) {
    global $language;

    // DOM and XPath preparations.
    $dom = $this->createRequest($payment->method, $this::REQUEST_TRANSACTION);
    $xpath = new DOMXpath($dom);
    $xpath->registerNamespace('iDEAL', $this::IDEAL_XML_NAMESPACE);
    $root = $xpath->query('/iDEAL:*')->item(0);

    // Preprocess values.
    $issuer_id = str_repeat('0', 4 - strlen($payment->method_data['issuer_id'])) . $payment->method_data['issuer_id'];
    // Convert diacritics to HTML entities and replace illegal characters with
    // underscores.
    $description = substr(trim(preg_replace('#[^a-z0-9=[:space:]%*+,-./&@"\':;?()$]#i', '_', htmlentities($payment->description, ENT_NOQUOTES, 'UTF-8'))), 0, 32);
    $amount = (string) round($payment->totalAmount(TRUE) * 100);
    $expiration_period = $this->secondsToISO8601($payment->method->controller_data['expiration_period']);

    // Build the message.
    $merchant = $xpath->query('iDEAL:Merchant', $root)->item(0);
    $issuer = $dom->createElementNS($this::IDEAL_XML_NAMESPACE, 'Issuer');
    $root->insertBefore($issuer, $merchant);
    $issuer->appendChild($dom->createElementNS($this::IDEAL_XML_NAMESPACE, 'issuerID', $issuer_id));
    $merchant->appendChild($dom->createElementNS($this::IDEAL_XML_NAMESPACE, 'merchantReturnURL', $this->merchantReturnURL()));
    $transaction = $dom->createElementNS($this::IDEAL_XML_NAMESPACE, 'Transaction');
    $root->appendChild($transaction);
    $transaction->appendChild($dom->createElementNS($this::IDEAL_XML_NAMESPACE, 'purchaseID', $payment->pid));
    $transaction->appendChild($dom->createElementNS($this::IDEAL_XML_NAMESPACE, 'amount', $amount));
    $transaction->appendChild($dom->createElementNS($this::IDEAL_XML_NAMESPACE, 'currency', 'EUR'));
    $transaction->appendChild($dom->createElementNS($this::IDEAL_XML_NAMESPACE, 'expirationPeriod', $expiration_period));
    $transaction->appendChild($dom->createElementNS($this::IDEAL_XML_NAMESPACE, 'language', $language->language));
    $transaction->appendChild($dom->createElementNS($this::IDEAL_XML_NAMESPACE, 'description', $description));
    $transaction->appendChild($dom->createElementNS($this::IDEAL_XML_NAMESPACE, 'entranceCode', $this->entranceCode($payment)));
    $this->alterRequest($this::REQUEST_TRANSACTION, $dom, $payment->method);

    // Sign the message.
    $token_code_values = array(
      $xpath->query('iDEAL:createDateTimeStamp', $root)->item(0)->nodeValue,
      $issuer_id,
      $xpath->query('iDEAL:merchantID', $merchant)->item(0)->nodeValue,
      $xpath->query('iDEAL:subID', $merchant)->item(0)->nodeValue,
      $this->merchantReturnURL(),
      $payment->pid,
      $amount,
      'EUR',
      $language->language,
      $description,
      $this->entranceCode($payment),
    );
    $this->signRequest($dom, $payment->method, $token_code_values);

    return $this->request($payment->method, $this::REQUEST_TRANSACTION, $dom);
  }

  /**
   * Implements iDEALPaymentMethodController::statusRequest().
   */
  function statusRequest(Payment $payment) {
    // DOM and XPath preparation.
    $dom = $this->createRequest($payment->method, $this::REQUEST_STATUS);
    $xpath = new DOMXpath($dom);
    $xpath->registerNamespace('iDEAL', $this::IDEAL_XML_NAMESPACE);
    $root = $xpath->query('/iDEAL:*')->item(0);

    $transaction = $dom->createElementNS($this::IDEAL_XML_NAMESPACE, 'Transaction');
    $root->appendChild($transaction);
    $transaction->appendChild($dom->createElementNS($this::IDEAL_XML_NAMESPACE, 'transactionID', $payment->method_data['transaction_id']));
    $this->alterRequest($this::REQUEST_STATUS, $dom, $payment->method);

    $merchant = $xpath->query('iDEAL:Merchant', $root)->item(0);
    $token_code_values = array(
      $xpath->query('iDEAL:createDateTimeStamp', $root)->item(0)->nodeValue,
      $xpath->query('iDEAL:merchantID', $merchant)->item(0)->nodeValue,
      $xpath->query('iDEAL:subID', $merchant)->item(0)->nodeValue,
      $payment->method_data['transaction_id'],
    );
    $this->signRequest($dom, $payment->method, $token_code_values);

    return $this->request($payment->method, $this::REQUEST_STATUS, $dom);
  }

  /**
   * Implements iDEALPaymentMethodController::responseVerifyAuthenticity().
   */
  function responseVerifyAuthenticity(PaymentMethod $payment_method, $request_type, DOMDocument $dom) {
    if ($request_type == $this::REQUEST_STATUS) {
      // Prepare XPath.
      $xpath = new DOMXPath($dom);
      $xpath->registerNamespace('iDEAL', $this::IDEAL_XML_NAMESPACE);
      $root = $xpath->query('/iDEAL:*')->item(0);

      // Extract authentication information.
      $token_code =  $xpath->query('iDEAL:Signature/iDEAL:signatureValue', $root)->item(0)->nodeValue;
      $token =  $xpath->query('iDEAL:Signature/iDEAL:fingerprint', $root)->item(0)->nodeValue;

      // Verify the public key name.
      if (strtolower($token) != strtolower($this->publicKeyIdentifier($this->publicCertificate($payment_method, 'acquirer')))) {
        throw new PaymentException(t("The iDEAL response's authenticity could not be verified, because the response was signed using an unknown acquirer public certificate."));
      }

      // Verify the signature value.
      $transaction = $xpath->query('/iDEAL:AcquirerStatusRes/iDEAL:Transaction')->item(0);
      $status = $xpath->query('iDEAL:status/text()', $transaction)->item(0)->nodeValue;
      $token_code_values = array(
        $xpath->query('iDEAL:createDateTimeStamp', $root)->item(0)->nodeValue,
        $xpath->query('iDEAL:transactionID', $transaction)->item(0)->nodeValue,
        $status,
      );
      // The account number is only added in case of success.
      if ($status == 'Success') {
        $token_code_values[] = $xpath->query('iDEAL:consumerAccountNumber', $transaction)->item(0)->nodeValue;
      }
      $this->verifyTokenCode($payment_method, $token_code_values, $token_code);
    }

    return $dom;
  }

  /**
   * Create a tokenCode (signature) for a request.
   *
   * @throws PaymentException
   *
   * @param array $values
   *   The values to create a tokenCode from.
   * @param string $private_key
   *
   * @return string
   */
  function tokenCode(array $values, $private_key) {
    $data = preg_replace('#\s#', '', implode($values));
    $token_code = NULL;
    openssl_sign($data, $token_code, $private_key);
    openssl_free_key($private_key);

    return base64_encode($token_code);
  }

  /**
   * Verify a received tokenCode (signature) for a response.
   *
   * @throws PaymentException
   *
   * @param PaymentMethod $payment_method
   * @param array $values
   *   The values the tokenCode was created from.
   * @param string $token_code
   *   The tokenCode to check.
   *
   * @return NULL
   */
  function verifyTokenCode(PaymentMethod $payment_method, $values, $token_code) {
    $certificate = $this->publicCertificate($payment_method, 'acquirer');
    $data = preg_replace('#\s#', '', implode($values));
    if (!openssl_verify($data, base64_decode($token_code), $certificate)) {
      throw new PaymentException(t('The iDEAL response is not authentic.'));
    }
  }

  /**
   * Implements iDEALPaymentMethodController::issuerOptions().
   */
  function issuerOptions(PaymentMethod $payment_method) {
    $cid = 'ideal_issuers_' . $payment_method->pmid;
    $cache = cache_get($cid);

    // Make a directory request if there is no previously cached data or if the
    // last request failed and its cache item has expired.
    if (!$cache || empty($cache->data) && $cache->expire < time()) {
      try {
        $dom = $this->directoryRequest($payment_method);
        $xpath = new DOMXPath($dom);
        $xpath->registerNamespace('iDEAL', $this::IDEAL_XML_NAMESPACE);
        $issuers = $xpath->query('/iDEAL:DirectoryRes/iDEAL:Directory/iDEAL:Issuer');
        $list_options = array();
        for ($i = 0; $i < $issuers->length; $i++) {
          $issuer = $issuers->item($i);
          $list = $xpath->query('iDEAL:issuerList/text()', $issuer)->item(0)->nodeValue;
          $issuer_id = $xpath->query('iDEAL:issuerID/text()', $issuer)->item(0)->nodeValue;
          $issuer_name = $xpath->query('iDEAL:issuerName/text()', $issuer)->item(0)->nodeValue;
          $list_options[$list][$issuer_id] = $issuer_name;
        }
        $options = array();
        if (!empty($list_options['Short'])) {
          foreach ($list_options['Short'] as $issuer_id => $issuer_name) {
            $options[$issuer_id] = $issuer_name;
          }
        }
        if (!empty($list_options['Long'])) {
          $options['ideal_choose_other'] = '---' . t('Other banks') . '---';
          foreach ($list_options['Long'] as $issuer_id => $issuer_name) {
            $options[$issuer_id] = $issuer_name;
          }
        }
        // Cache issuers for 24h in case of succes. If the request fails, cache
        // the result for ten minutes to prevent errors from repeatedly popping
        // up.
        cache_set($cid, $options, 'cache', time() + 86400);
        return $options;
      }
      catch (PaymentException $e) {
        // Cache failed requests for ten minutes so we do not bombard the server
        // with requests.
        $options = array();
        cache_set($cid, $options, 'cache', time() + 600);
      }
    }
    else {
      $options = $cache->data;
    }
    if ($options) {
      return $options;
    }
    throw new PaymentValidationException(t('iDEAL could not retrieve the list of issuing banks and will try again in ten minutes. Clear the cache to force a retry immediately.'));
  }

  /**
   * Implements iDEALPaymentMethodController::statusUpdate().
   */
  function statusUpdate(Payment $payment) {
    if (payment_status_is_or_has_ancestor($payment->getStatus()->status, PAYMENT_STATUS_PENDING)) {
      try {
        $dom = $this->statusRequest($payment);
        $payment->method_data['updated'] = time();
        $xpath = new DOMXPath($dom);
        $xpath->registerNamespace('iDEAL', $this::IDEAL_XML_NAMESPACE);
        $transaction = $xpath->query('/iDEAL:AcquirerStatusRes/iDEAL:Transaction')->item(0);
        $status_map = $this->statusMap();
        $status = $status_map[$xpath->query('iDEAL:status', $transaction)->item(0)->nodeValue];
        if ($status != $payment->getStatus()->status) {
          $payment->setStatus(new PaymentStatusItem($status));
          if (payment_status_is_or_has_ancestor($payment->getStatus()->status, PAYMENT_STATUS_SUCCESS)) {
            $payment->method_data['consumer_name'] = $xpath->query('iDEAL:consumerName', $transaction)->item(0)->nodeValue;
            $payment->method_data['consumer_city'] = $xpath->query('iDEAL:consumerCity', $transaction)->item(0)->nodeValue;
            $payment->method_data['consumer_account_number'] = $xpath->query('iDEAL:consumerAccountNumber', $transaction)->item(0)->nodeValue;
          }
          entity_save('payment', $payment);
          return TRUE;
        }
      }
      catch (Exception $e) {
        // Silently fail if something went wrong, as we'll try to update the
        // status later again.
      }
    }
    return FALSE;
  }
}