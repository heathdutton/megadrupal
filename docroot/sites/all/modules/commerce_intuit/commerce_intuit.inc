<?php

/**
 * @file
 * Class that will handle the Intuit Payment.
 */

define('COMMERCE_INTUIT_PRODUCTION', 'https://webmerchantaccount.quickbooks.com/j/AppGateway');
define('COMMERCE_INTUIT_SANDBOX', 'https://webmerchantaccount.ptc.quickbooks.com/j/AppGateway');

class CommerceIntuit {

  protected $applogin;
  protected $conticket;
  protected $appid;
  protected $pem;
  protected $gatewayurl;
  protected $sessionticket;

  /**
   * Constructor.
   *
   * @param string $mode
   *   The mode for check Live or Test.
   */
  public function __construct($mode) {
    $this->setAppLogin(($mode == 'Live') ? variable_get('commerce_intuit_applogin') : variable_get('commerce_intuit_sand_applogin'));
    $this->setConTicket(($mode == 'Live') ? variable_get('commerce_intuit_connticket') : variable_get('commerce_intuit_sand_connticket'));
    $this->setAppId(($mode == 'Live') ? variable_get('commerce_intuit_appid') : variable_get('commerce_intuit_sand_appid'));
    $this->setPem(($mode == 'Live') ? variable_get('commerce_intuit_pem') : variable_get('commerce_intuit_sand_pem'));
    $this->setGatewayUrl(($mode == 'Live') ? COMMERCE_INTUIT_PRODUCTION : COMMERCE_INTUIT_SANDBOX);
  }

  /**
   * Get applogin.
   */
  public function getAppLogin() {
    return $this->applogin;
  }

  /**
   * Setter Method.
   *
   * @param string $applogin
   *   Set applogin
   */
  public function setAppLogin($applogin) {
    $this->applogin = $applogin;
  }

  /**
   * Get conticket.
   */
  public function getConTicket() {
    return $this->conticket;
  }

  /**
   * Setter Method.
   *
   * @param string $conticket
   *   Set conticket.
   */
  public function setConTicket($conticket) {
    $this->conticket = $conticket;
  }

  /**
   * Get appid.
   */
  public function getAppId() {
    return $this->appid;
  }

  /**
   * Setter Method.
   *
   * @param string $appid
   *   Set appid.
   */
  public function setAppId($appid) {
    $this->appid = $appid;
  }

  /**
   * Get pem.
   */
  public function getPem() {
    return $this->pem;
  }

  /**
   * Setter Method.
   *
   * @param string $pem
   *   Set pem.
   */
  public function setPem($pem) {
    $this->pem = $pem;
  }

  /**
   * Get gatewayurl.
   */
  public function getGatewayUrl() {
    return $this->gatewayurl;
  }

  /**
   * Setter Method.
   *
   * @param string $gatewayurl
   *   Set gatewayurl.
   */
  public function setGatewayUrl($gatewayurl) {
    $this->gatewayurl = $gatewayurl;
  }

  /**
   * Get sessionticket.
   */
  public function getSessionTicket() {
    return $this->sessionticket;
  }

  /**
   * Setter Method.
   *
   * @param string $sessionticket
   *   Set sessionticket.
   */
  public function setSessionTicket($sessionticket) {
    $this->sessionticket = $sessionticket;
  }

  /**
   * Validate User.
   */
  public function checkSignon() {
    $signonxml = '<?xml version="1.0" ?>
        <?qbmsxml version="4.5"?>
        <QBMSXML>
        <SignonMsgsRq>
        <SignonAppCertRq>
        <ClientDateTime>' . date('Y-m-d\TH:i:s') . '</ClientDateTime>
        <ApplicationLogin>' . $this->getAppLogin() . '</ApplicationLogin>
        <ConnectionTicket>' . $this->getConTicket() . '</ConnectionTicket>
        </SignonAppCertRq>
        </SignonMsgsRq>
        </QBMSXML>';

    // Submit to intuit.
    $response = $this->sendToIntuit($signonxml);
    $xml = simplexml_load_string($response);

    $statuscode = (array) $xml->SignonMsgsRs->SignonAppCertRs['statusCode'];
    $statusmessage = (array) $xml->SignonMsgsRs->SignonAppCertRs['statusMessage'];
    $statusmessage = (!empty($statusmessage)) ? $statusmessage[0] : '';
    // Go ahead and get the session ticket.
    // Find the location of the start tag.
    $php_tempstring = strstr($response, "<SessionTicket>");
    $php_endlocation = strpos($php_tempstring, "</SessionTicket>");
    $sessionticket = substr($php_tempstring, 15, $php_endlocation - 15);
    $this->setSessionticket($sessionticket);
    return array($statuscode[0], $statusmessage);
  }

  /**
   * Send the params to intuit and get the response.
   *
   * @param string $params
   *   This is post fields
   */
  public function sendToIntuit($params) {
    $header[] = "Content-type: application/x-qbmsxml";
    $header[] = "Content-length: " . strlen($params);

    $submit = curl_init();
    curl_setopt($submit, CURLOPT_POST, 1);
    curl_setopt($submit, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($submit, CURLOPT_CUSTOMREQUEST, 'POST');
    curl_setopt($submit, CURLOPT_URL, $this->getGatewayUrl());
    curl_setopt($submit, CURLOPT_TIMEOUT, 10);
    curl_setopt($submit, CURLOPT_HTTPHEADER, $header);
    curl_setopt($submit, CURLOPT_POSTFIELDS, $params);
    curl_setopt($submit, CURLOPT_VERBOSE, 1);
    curl_setopt($submit, CURLOPT_SSL_VERIFYPEER, 1);
    curl_setopt($submit, CURLOPT_SSLCERT, $this->getPem());
    $response = curl_exec($submit);
    curl_close($submit);

    return $response;
  }

  /**
   * Charge on Creadit Card.
   *
   * @param string $txn_array
   *   This is Customer Transaction Array.
   */
  public function charge($txn_array) {

    $creditcardcharge = '<?xml version="1.0" ?>
        <?qbmsxml version="4.5"?>
        <QBMSXML>
        <SignonMsgsRq>
        <SignonTicketRq>
        <ClientDateTime>' . date('Y-m-d\TH:i:s') . '</ClientDateTime>
        <SessionTicket>' . $this->getSessionTicket() . '</SessionTicket>
        </SignonTicketRq>
        </SignonMsgsRq>
        <QBMSXMLMsgsRq>
        <CustomerCreditCardChargeRq>
        <TransRequestID>' . $txn_array['order_id'] . '</TransRequestID>
        <CreditCardNumber>' . $txn_array['card'] . '</CreditCardNumber>
        <ExpirationMonth>' . $txn_array['expmonth'] . '</ExpirationMonth>
        <ExpirationYear>' . $txn_array['expyear'] . '</ExpirationYear>
        <IsECommerce>TRUE</IsECommerce>
        <Amount>' . $txn_array['amount'] . '</Amount>
        <NameOnCard>' . $txn_array['billing_name'] . '</NameOnCard>
        <CreditCardAddress>' . $txn_array['street_address'] . '</CreditCardAddress>
        <CreditCardPostalCode>' . $txn_array['postal_code'] . '</CreditCardPostalCode>
        <CommercialCardCode></CommercialCardCode>
        <SalesTaxAmount>0.00</SalesTaxAmount>
        <CardSecurityCode>' . $txn_array['cvd_value'] . '</CardSecurityCode>
        </CustomerCreditCardChargeRq>
        </QBMSXMLMsgsRq>
        </QBMSXML>';

    // Submit to intuit.
    $response = $this->sendToIntuit($creditcardcharge);
    $xml = simplexml_load_string($response);
    $result = array();
    if ($xml->QBMSXMLMsgsRs->CustomerCreditCardChargeRs['statusCode'] != "0") {
      $result['error_no'] = $xml->QBMSXMLMsgsRs->CustomerCreditCardChargeRs['statusCode'];
      $result['error_message'] = $xml->QBMSXMLMsgsRs->CustomerCreditCardChargeRs['statusMessage'];
      return $result;
    }

    $result['responsecode'] = $xml->QBMSXMLMsgsRs->CustomerCreditCardChargeRs['statusCode'];
    $result['message'] = $xml->QBMSXMLMsgsRs->CustomerCreditCardChargeRs['statusMessage'];
    $result['TransID'] = $xml->QBMSXMLMsgsRs->CustomerCreditCardChargeRs->CreditCardTransID;
    return $result;
  }

}
