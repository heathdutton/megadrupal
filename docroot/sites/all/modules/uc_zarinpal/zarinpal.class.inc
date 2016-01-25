<?php

/**
 * @author "Ahmad Hejazee" <mngafa@gmail.com>
 * @file Zarinpal class handles all transactions related to zarinpal payment service.
 * 
 * @see http://www.hejazee.ir/
 * @see http://www.tolooco.com/
 * 
 * Copyright 2014-2015 - tolooco.com - All rights reserved.
 * For Licence, see README.txt and README.fa.txt
 */

/**
 * Class Zarinpal Handles Zarinpal Gateway transactions
 */
class Zarinpal {

  /**
   * @var bool Whether we should use nusoap library
   */
  protected $nusoap;

  /**
   * @var string Merchand ID
   */
  protected $MerchantID;

  /**
   * @var int Amount in Tomans
   */
  protected $Amount;

  /**
   * @var string Description of the transaction
   */
  protected $Description;

  /**
   * @var string optional Email of the customer
   */
  protected $Email;

  /**
   * @var string optional Mobile number of the customer
   */
  protected $Mobile;

  /**
   * @var Callback URL of the shopping site
   */
  protected $CallbackURL;

  /**
   * @var string Zarinpal webgate WSDL address in Germany
   */
  protected $webgate_auth_de = 'https://de.zarinpal.com/pg/services/WebGate/wsdl';

  /**
   * @var string Zarinpal webgate WSDL address in Iran
   */
  protected $webgate_auth_ir = 'https://ir.zarinpal.com/pg/services/WebGate/wsdl';

  /**
   * @var string Zarinpal Webgate address prefix
   */
  protected $webgate = 'https://www.zarinpal.com/pg/StartPay/';

  /**
   * @var bool Whether we should use iran switch or not (germany otherwise)
   */
  protected $Iran = False;

  /**
   * Class Constructor
   *
   * @param $MerchantID
   *   Merchant ID
   * @param $Amount
   *   Amount In tomans
   * @param $Description
   *   Description of Payment
   * @param $CallbackURL
   *   Callback url of the Shopping site
   * @param string $Email
   *   Optional email of customer
   * @param string $Mobile
   *   Optional mobile number of the customer
   */
  function __construct($MerchantID, $Amount, $Description, $CallbackURL, $Email= '', $Mobile = '') {
    //Check if we should use SoapClient or Nusoap
    $soapmode = variable_get('uc_zarinpal_soapmode', 'nusoap');
    if ($soapmode == 'soapclient' && class_exists('SoapClient')) {
      $this->nusoap = FALSE;
    }
    else {
      //Load the nusoap class.
      require_once dirname(__FILE__) . '/lib/nusoap/nusoap.php';
      $this->nusoap = TRUE;
    }

    //Initialize variables
    $this->MerchantID = $MerchantID;
    $this->Amount = $Amount;
    $this->Description = $Description;
    $this->CallbackURL = $CallbackURL;
    $this->Email = $Email;
    $this->Mobile = $Mobile;
  }

  /**
   * Set the zarinpal switch to Iran or Germany or check it.
   *
   * @param $arg
   *   If True, the switch will be set to Iran
   *   If False, the switch will be set to Germany
   *   Otherwise, the switch status will be returned (True = Iran; False = Germany)
   * @return bool
   */
  public function isIran($arg = NULL) {
    if ($arg === TRUE) {
      $this->Iran = TRUE;
    }
    elseif ($arg === FALSE) {
      $this->Iran = FALSE;
    }
    else {
      return $this->Iran;
    }

    return TRUE;
  }

  /**
   * Request payment.
   *
   * @throws ZarinpalException
   *
   * @return String Authority number
   */
  public function paymentRequest() {
    $wsdl = $this->isIran() ? $this->webgate_auth_ir : $this->webgate_auth_de;
    $authority = NULL;
    $args = array(
      'MerchantID' 	=> $this->MerchantID,
      'Amount' 		  => $this->Amount,
      'Description' => $this->Description,
      'Email' 		  => $this->Email,
      'Mobile' 		  => $this->Mobile,
      'CallbackURL' => $this->CallbackURL
    );

    if ($this->nusoap) {
      $client = new nusoap_client($wsdl, 'wsdl');
      $client->soap_defencoding = 'UTF-8';
      $result = $client->call('PaymentRequest', array($args));

      if (($result['Status'] == 100) && (isset($result['Authority'])) && (strlen($result['Authority']) == 36)) {
        $authority = $result['Authority'];
      }
      else {
        throw new ZarinpalException(t('Can not request payment. !msg', array(
          '!msg' => $this->translateErrorCode($result['Status']),
        )));
      }
    }
    else {
      $client = new SoapClient($wsdl, array('encoding' => 'UTF-8'));

      $result = $client->PaymentRequest($args);

      if (($result->Status == 100) && (isset($result->Authority)) && (strlen($result->Authority) == 36)) {
        $authority = $result->Authority;
      }
      else {
        throw new ZarinpalException(t('Can not request payment. !msg', array(
          '!msg' => $this->translateErrorCode($result->Status),
        )));
      }
    }

    return $authority;
  }

  /**
   * Verify transaction
   *
   * @param String $Authority
   *   The Authority number which is passed by Zarinpal to the callback page as QueryString
   *   If Null provided, it will be get from $_GET
   *
   * @throws ZarinpalException
   * @throws ZarinpalExceptionNok
   * @throws ZarinpalExceptionVerification
   *
   * @return int Reference ID of transaction if it is successful.
   */
  public function verifyTransaction($Authority = NULL) {
    // Validate $Authority and $Status (passed by zarinpal via QueryString in callback url)
    if (is_null($Authority)) {
      $status = isset($_GET['Status']) ? $_GET['Status'] : NULL;
      if ($status == 'OK') {
        if (isset($_GET['Authority'])) {
          $Authority = $_GET['Authority'];
        }
        else {
          throw new ZarinpalException(t('Error: $_GET[Authority] not available'));
        }
      }
      elseif ($status == 'NOK') {
        // In Sample PHP Code: Transaction canceled by user.
        throw new ZarinpalExceptionNok();
      }
      else {
        throw new ZarinpalException(t('Error: $_GET[Status] invalid'));
      }
    }

    // Validated. Now Process verification
    $wsdl = $this->isIran() ? $this->webgate_auth_ir : $this->webgate_auth_de;
    $refid = NULL;
    $args = array(
      'MerchantID' => $this->MerchantID,
      'Amount' 		 => $this->Amount,
      'Authority'  => $Authority,
    );

    if ($this->nusoap) {
      $client = new nusoap_client($wsdl, 'wsdl');
      $client->soap_defencoding = 'UTF-8';
      $res = $client->call('PaymentVerification', array($args));

      if (isset($res['Status']) && ($res['Status'] == 100)) {
        $refid = $res['RefID'];
      }
      else {
        throw new ZarinpalExceptionVerification(t('Transaction verification failed. RefID: %refid, Message: !message', array(
          '%refid'   => $res['RefID'],
          '!message' => $this->translateErrorCode($res['Status']),
        )));
      }

    }
    else {
      $client = new SoapClient($wsdl, array('encoding' => 'UTF-8'));
      $res = $client->PaymentVerification($args);

      if (isset($res->Status) && ($res->Status == 100)) {
        $refid = $res->RefID;
      }
      else {
        throw new ZarinpalExceptionVerification(t('Transaction verification failed. RefID: %refid, Message: !message', array(
          '%refid'   => $res->RefID,
          '!message' => $this->translateErrorCode($res->Status),
        )));
      }
    }

    return $refid;
  }

  /**
   * Build WebGate or ZarinGate URL. The user should be redirected to this address for payment
   *
   * @param String $Authority The authority code returned by paymentRequest() method
   * @param string $type Type of Gate. Wither 'webgate' or 'zaringate'. default is 'webgate'
   *
   * @return string Gate url
   *
   * @throws Exception
   */
  public function buildGateURL($Authority, $type = 'webgate') {
    switch ($type) {
      case 'webgate':
        $url = $this->webgate . $Authority;
        break;
      case 'zaringate':
        $url = $this->webgate . $Authority . '/ZarinGate';
        break;
      default:
        throw new Exception('Invalid gate type: should be either "webgate" or "zaringate"');
    }

    return $url;
  }

  /**
   * Translates zarinpal error codes
   *
   * @param int $code The error code
   *
   * @return string Translated message
   */
  public function translateErrorCode($code) {
    switch ($code) {
      case -1:
        $msg = t('Incomplete data.');
        break;
      case -2:
        $msg = t('Invalid IP or MerchantID.');
        break;
      case -3:
        $msg = t('Cannot pay this amount due to restrictions of the Shapark system.');
        break;
      case -4:
        $msg = t('Merchant\'s verify-level is below the accepted silver level.');
        break;
      case -11:
        $msg = t('Specified request was not found.');
        break;
      case -12:
        $msg = t('Can\'t edit request.');
        break;
      case -21:
        $msg = t('No financial actions found for this transaction.');
        break;
      case -22:
        $msg = t('Transaction failed.');
        break;
      case -33:
        $msg = t('Transaction\'s amount does not match with the paid amount.');
        break;
      case -34:
        $msg = t('Maximum transaction division exceeded (by quantity or amount).');
        break;
      case -40:
        $msg = t('Access to requested method is denied.');
        break;
      case -41:
        $msg = t('Invalid data sent to AdditionalData() method.');
        break;
      case -42:
        $msg = t('Paymeny authority lifetime should be between 30 minutes and 45 days.');
        break;
      case -54:
        $msg = t('The specified request is archived.');
        break;
      case 100:
        $msg = t('Action completed successfully');
        break;
      case 101:
        $msg = t('Payment was successfull but PaymentVerification() method is already done.');
        break;
      default:
        $msg = t('Unknown error code.');
    }

    $message = t("Error code: %code : %msg", array(
      '%code' => $code,
      '%msg' => $msg,
    ));

    return $message;
  }
}

/**
 * Custom Exception class for Zarinpal general errors.
 */
class ZarinpalException extends Exception {
}

/**
 * Custom Exception class
 * When Zarinpal sends Status=NOK to callback URL as query string.
 * In Sample PHP Code: Transaction canceled by user.
 */
class ZarinpalExceptionNok extends Exception {
}

/**
 * Custom Exception class
 * When Zarinpal verification transaction fails. (aka PaymentVerification() method
 * does not return 100)
 */
class ZarinpalExceptionVerification extends Exception {
}
