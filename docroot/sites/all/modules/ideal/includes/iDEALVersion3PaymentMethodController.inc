<?php

/**
 * Contains iDEALVersion3PaymentMethodController.
 */

/**
 * A payment method controller for iDEAL version 3.
 */
class iDEALVersion3PaymentMethodController extends iDEALPaymentMethodController {

  const IDEAL_INTERFACE_VERSION = '3.3.1';

  const IDEAL_XML_NAMESPACE = 'http://www.idealdesk.com/ideal/messages/mer-acq/3.3.1';

  const IDEAL_XML_SCHEMA_PATH = 'ideal_xml_schema_v3.xsd';

  const IDEAL_XMLDSIG_XML_NAMESPACE = 'http://www.w3.org/2000/09/xmldsig#';

  /**
   * {@inheritdoc}
   */
  const IDEAL_PRIVATE_KEY_LENGTH = 2048;

  function __construct() {
    $this->title = t('iDEAL 3');
  }

  /**
   * {@inheritdoc}
   */
  function createRequest(PaymentMethod $payment_method, $request_type) {
    $dom = parent::createRequest($payment_method, $request_type);
    $xpath = new DOMXpath($dom);
    $xpath->registerNamespace('iDEAL', $this::IDEAL_XML_NAMESPACE);

    $root = $xpath->query('/iDEAL:*')->item(0);
    $create_date_timestamp = $dom->createElementNS($this::IDEAL_XML_NAMESPACE, 'createDateTimestamp', gmdate('o-m-d\TH:i:s.000\Z', time()));
    $root->insertBefore($create_date_timestamp, $root->firstChild);

    return $dom;
  }

  /**
   * Sign a request.
   */
  function signRequest(DOMDocument $dom, PaymentMethod $payment_method) {
    $xpath = new DOMXpath($dom);
    $xpath->registerNamespace('iDEAL', $this::IDEAL_XML_NAMESPACE);
    $xpath->registerNamespace('XMLDSig', $this::IDEAL_XMLDSIG_XML_NAMESPACE);
    $root = $xpath->query('/iDEAL:*')->item(0);

    // Create the DigestValue element.
    $xml = $dom->C14N(FALSE, TRUE);
    $digest_value = base64_encode(hash('sha256', $xml, TRUE));
    // Create the SignatureValue element.
    $signature_fragment = $this->XMLDSig($dom, $digest_value, NULL, NULL);
    $root->appendChild($signature_fragment);
    $signature = $root->lastChild;
    $signed_info_xml = $xpath->query('XMLDSig:SignedInfo', $signature)->item(0)->C14N(TRUE, FALSE);
    $signature_value = NULL;
    openssl_sign($signed_info_xml, $signature_value, $this->merchantPrivateKey($payment_method), 'SHA256');
    $signature_value = base64_encode($signature_value);
    $key_name = $this->publicKeyIdentifier($this->publicCertificate($payment_method, 'merchant'));
    $signature_fragment = $this->XMLDSig($dom, $digest_value, $signature_value, $key_name);
    $root->replaceChild($signature_fragment, $signature);
  }

  /**
   * {@inheritdoc}
   */
  function directoryRequest(PaymentMethod $payment_method) {
    $dom = $this->createRequest($payment_method, $this::REQUEST_DIRECTORY);
    $this->alterRequest($this::REQUEST_DIRECTORY, $dom, $payment_method);
    $this->signRequest($dom, $payment_method);

    return $this->request($payment_method, $this::REQUEST_DIRECTORY, $dom);
  }

  /**
   * {@inheritdoc}
   */
  function transactionRequest(Payment $payment) {
    global $language;

    // DOM and XPath preparations.
    $dom = $this->createRequest($payment->method, $this::REQUEST_TRANSACTION);
    $xpath = new DOMXpath($dom);
    $xpath->registerNamespace('iDEAL', $this::IDEAL_XML_NAMESPACE);
    $root = $xpath->query('/iDEAL:*')->item(0);

    // Preprocess values.
    $issuer_id = $payment->method_data['issuer_id'];
    // Convert diacritics to HTML entities and replace illegal characters with
    // underscores.
    $description = substr(trim(htmlentities($payment->description, ENT_NOQUOTES, 'UTF-8')), 0, 32);
    $amount = (string) round($payment->totalAmount(TRUE), 2);
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

    $this->signRequest($dom, $payment->method);

    return $this->request($payment->method, $this::REQUEST_TRANSACTION, $dom);
  }

  /**
   * {@inheritdoc}
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

    $this->signRequest($dom, $payment->method);

    return $this->request($payment->method, $this::REQUEST_STATUS, $dom);
  }

  /**
   * Provides a DOM with an XML digital signature according the iDEAL 3.3
   * specification.
   *
   * @param DOMDocument $dom
   * @param string $digest_value
   * @param string $signature_value
   * @param string $key_name
   *
   * @return DOMDocumentFragment
   */
  function XMLDSig(DOMDocument $dom, $digest_value, $signature_value, $key_name) {
    $signature_xml = <<<EOD
<Signature xmlns="http://www.w3.org/2000/09/xmldsig#">
  <SignedInfo>
    <CanonicalizationMethod Algorithm="http://www.w3.org/2001/10/xml-exc-c14n#" />
    <SignatureMethod Algorithm="http://www.w3.org/2001/04/xmldsig-more#rsa-sha256" />
    <Reference URI="">
      <Transforms>
        <Transform Algorithm="http://www.w3.org/2000/09/xmldsig#enveloped-signature" />
      </Transforms>
      <DigestMethod Algorithm="http://www.w3.org/2001/04/xmlenc#sha256" />
      <DigestValue>$digest_value</DigestValue>
    </Reference>
  </SignedInfo>
  <SignatureValue>$signature_value</SignatureValue>
  <KeyInfo>
    <KeyName>$key_name</KeyName>
  </KeyInfo>
</Signature>
EOD;
    $signature = $dom->createDocumentFragment();
    $signature->appendXML($signature_xml);

    return $signature;
  }

  /**
   * {@inheritdoc}
   */
  function responseVerifyAuthenticity(PaymentMethod $payment_method, $request_type, DOMDocument $dom) {
    // Prepare XPath.
    $xpath = new DOMXPath($dom);
    $xpath->registerNamespace('iDEAL', $this::IDEAL_XML_NAMESPACE);
    $xpath->registerNamespace('XMLDSig', $this::IDEAL_XMLDSIG_XML_NAMESPACE);

    // Get signature data for later use and remove the signature.
    $signature = $xpath->query('/iDEAL:*/XMLDSig:Signature')->item(0);
    $signed_info_c14n = $xpath->query('XMLDSig:SignedInfo', $signature)->item(0)->C14N(TRUE, FALSE);
    $signature->parentNode->removeChild($signature);

    // Extract authentication information.
    $key_name = $xpath->query('XMLDSig:KeyInfo/XMLDSig:KeyName/text()', $signature)->item(0)->nodeValue;
    $digest_value = $xpath->query('XMLDSig:SignedInfo/XMLDSig:Reference/XMLDSig:DigestValue/text()', $signature)->item(0)->nodeValue;
    $signature_value = $xpath->query('XMLDSig:SignatureValue/text()', $signature)->item(0)->nodeValue;

    // Verify the public key name.
    if (strtolower($key_name) != strtolower($this->publicKeyIdentifier($this->publicCertificate($payment_method, 'acquirer')))) {
      throw new PaymentException(t("The iDEAL response's authenticity could not be verified, because the response was signed using an unknown acquirer public certificate."));
    }

    // Verify the digest value.
    $digest_xml = $dom->C14N(FALSE, TRUE);
    if ($digest_value != base64_encode(hash('sha256', $digest_xml, TRUE))) {
      throw new PaymentException(t("The iDEAL response's digest value is incorrect."));
    }

    // Verify the signature value.
    $certificate = $this->publicCertificate($payment_method, 'acquirer');
    $authentic = (bool) openssl_verify($signed_info_c14n, base64_decode($signature_value), $certificate, 'SHA256');
    if (!$authentic) {
      throw new PaymentException(t('The iDEAL response is not authentic.'));
    }
  }

  /**
   * {@inheritdoc}
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
        $countries = $xpath->query('/iDEAL:DirectoryRes/iDEAL:Directory/iDEAL:Country');
        $options = array();
        for ($i = 0; $i < $countries->length; $i++) {
          $country = $countries->item($i);
          $country_names = $xpath->query('iDEAL:countryNames/text()', $country)->item(0)->nodeValue;
          $issuers = $xpath->query('iDEAL:Issuer', $country);
          for ($j = 0; $j < $issuers->length; $j++) {
            $issuer = $issuers->item($j);
            $issuer_id = $xpath->query('iDEAL:issuerID/text()', $issuer)->item(0)->nodeValue;
            $issuer_name = $xpath->query('iDEAL:issuerName/text()', $issuer)->item(0)->nodeValue;
            $options[$country_names][$issuer_id] = $issuer_name;
          }
        }
        // Cache issuers for 24 hours.
        cache_set($cid, $options, 'cache', time() + 86400);
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
   * {@inheritdoc}
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
          $payment->setStatus(new PaymentStatusItem($status, strtotime($xpath->query('iDEAL:statusDateTimestamp', $transaction)->item(0)->nodeValue)));
          if (payment_status_is_or_has_ancestor($payment->getStatus()->status, PAYMENT_STATUS_SUCCESS)) {
            $payment->method_data['consumer_name'] = $xpath->query('iDEAL:consumerName', $transaction)->item(0)->nodeValue;
            $payment->method_data['consumer_iban'] = $xpath->query('iDEAL:consumerIBAN', $transaction)->item(0)->nodeValue;
            $payment->method_data['consumer_bic'] = $xpath->query('iDEAL:consumerBIC', $transaction)->item(0)->nodeValue;
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