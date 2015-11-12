<?php

/**
 * @file
 * PxOrder class for managing orders to PayEx.
 */

class PxOrder {
  protected $account_number;
  protected $client;
  protected $key;

  function __construct($account_number, $key) {
    $this->account_number = $account_number;
    $settings = variable_get('payex_settings', array('live' => 0));
    $url = ($settings['live'] ? 'https://external.payex.com' : 'https://test-external.payex.com') . '/pxorder/pxorder.asmx?wsdl';
    $this->client = new SoapClient($url, array(
      'compression' => SOAP_COMPRESSION_ACCEPT | SOAP_COMPRESSION_GZIP,
      'soap_version' => 'SOAP_1_2',
    ));
    $this->key = $key;
  }

  /**
   * Implementation of PXOrder.Initialize.
   */
  function initialize($params = array()) {
    // Default to the client's stored account number.
    $params += array('accountNumber' => $this->account_number);

    // Correct order of the params when hashing.
    $ordered_keys = array('accountNumber', 'purchaseOperation', 'price', 'priceArgList', 'currency', 'vat', 'orderID', 'productNumber', 'description', 'clientIPAddress', 'clientIdentifier', 'additionalValues', 'externalID', 'returnUrl', 'view', 'agreementRef', 'cancelUrl', 'clientLanguage');

    $values = array(); 
    foreach ($ordered_keys as $key) {
      // Contrary to the documentation, PayEx requires all params to be
      // set, if only as an empty string.
      if (!isset($params[$key])) {
        $params[$key] = '';
      }
      // Add the keys with values to the values array.
      else {
        $values[] = $params[$key];
      }
    }

    // Finally, add the encryption key to the values array, implode and
    // md5hash it.
    $values[] = $this->key;
    $params['hash'] = md5(implode($values));

    // Get the Soap response and turn it into a DOM document.
    $order = $this->client->Initialize7($params);
    $xml = new SimpleXMLElement($order->Initialize7Result);

    // The PayEx XML response is simple enough that we can flatten it
    // by typecasting it to arrays.
    $response = (array) $xml;
    $response['header'] = (array) $response['header'];
    $response['status'] = (array) $response['status'];

    return $response;
  }

  /**
   * Implementation of PXOrder.Complete.
   */
  function complete($params = array()) {
    // Default to the client's stored account number.
    $params += array('accountNumber' => $this->account_number);

    $params['hash'] = md5($params['accountNumber'] . $params['orderRef'] . $this->key);

    // Get the Soap response and turn it into a DOM document.
    $order = $this->client->Complete($params);
    $xml = new SimpleXMLElement($order->CompleteResult);

    // The PayEx XML response is simple enough that we can flatten it
    // by typecasting it to arrays.
    $response = (array) $xml;
    $response['header'] = (array) $response['header'];
    $response['status'] = (array) $response['status'];

    return $response;
  }
}

