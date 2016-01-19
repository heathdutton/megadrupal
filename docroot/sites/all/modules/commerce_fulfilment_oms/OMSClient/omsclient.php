<?php

// Load all of the SOAP datatypes
require 'omsclient.types.php';


/**
 * Provides an interface to the OMS webservices API which doesn't suck.
 */
class OMSClient {

  /**
   * B2B credentials
   */
  private $_credentials;

  /**
   * WSDL endpoints
   */
  private $_endpoints;

  /**
   * Log
   */
  private $_log = array();


  /**
   * Constructor
   *
   * @var $credentials Array of credentials (B2BCustomerName, B2BCustomerEmail, B2BCustomerPassword)
   * @var $endpoints Array of WSDL endpoints (OrderTaker, InventoryManager)
   */
  function __construct($credentials, $endpoints) {
    $this->_credentials = $credentials;
    $this->_endpoints = $endpoints;
  }


  /**
   * Log a message
   */
  function log($type, $message) {
    $this->_log[] = array('type' => $type, 'message' => $message);
  }


  /**
   * Get the log
   */
  function getLog() {
    return $this->_log;
  }


  /**
   * Clean parameters
   *
   * Iterate all of the parameters and make sure they are safe.
   * No idea why the SoapClient doesn't do this automatically.
   */
  function cleanParameters($object) {
    foreach ($object as $key => $value) {
      if (is_string($value)) {
        $object->$key = htmlspecialchars($value);
      } elseif (is_array($value) || is_object($value)) {
        $this->cleanParameters($value);
      }
    }
  }


  /**
   * Wrapper for CreateSalesOrder webservice.
   *
   * @var $order SalesOrder The SalesOrder object.
   * @var $live_mode boolean The live mode setting.
   */
  function create_sales_order(SalesOrder $order, $live_mode = TRUE) {
    try {
      $parameters = new CreateSalesOrder;
      $parameters->LiveMode = $live_mode;
      $parameters->order = $order;
      $parameters->order->B2BCustomerName = $this->_credentials['name'];
      $parameters->order->B2BCustomerEmail = $this->_credentials['email'];
      $parameters->order->B2BCustomerPassword = $this->_credentials['password'];

      $this->cleanParameters($parameters->order);

      $client = new OMSSoapClient($this->_endpoints['OrderTaker']);
      $response = $client->CreateSalesOrder($parameters);
    } catch (SoapFault $e) {
      $this->log('request', $client->getLastRequestXML());
      throw new OMSClientException($e->getMessage());
    }

    $this->log('request', $client->getLastRequestXML());
    $this->log('response', $response);

    $message = (string) $response->CreateSalesOrderResult;
    $message = strip_tags($message); // Strip out the SOAP junk

    // Check for an error
    if (strpos($message, 'OrderID:') !== 0) {
      throw new OMSClientException($message);
    }

    // Order was created successfully
    $oms_order_id = (int) str_replace('OrderID:', '', $message);
    return $oms_order_id;
  }


  /**
   * Wrapper for the ExportOrders webservice.
   *
   * @var $type string The TransactionType
   * @var $order_id integer The OMS Order ID
   * @var $order_number string The Merchant Order Number
   * @return Returns an array of orders matching the criteria.
   */
  function export_orders($type, $order_id = NULL, $order_number = NULL) {
    try {
      $order_number_str = is_array($order_number) ? implode(',', $order_number) : $order_number;

      $parameters = new ExportOrders;
      $parameters->strB2BCustomerName = $this->_credentials['name'];
      $parameters->strB2BCustomerEmail = $this->_credentials['email'];
      $parameters->strB2BCustomerPassword = $this->_credentials['password'];
      $parameters->strTransactionType = $type;
      $parameters->strOrderID = $order_id;
      $parameters->strMerchantOrderID = $order_number_str;

      $client = new OMSSoapClient($this->_endpoints['OrderTaker']);
      $response = $client->ExportOrders($parameters);
    } catch (SoapFault $e) {
      throw new OMSClientException($e->getMessage());
    }

    if (isset($response->ExportOrdersResult->Orders->Order)) {
      $obj = $response->ExportOrdersResult->Orders->Order;
      if (is_array($obj)) {
        $orders = $obj;
      } else {
        $orders = array($obj);
      }
    } else {
      return FALSE;
    }

    return $orders;
  }


  /**
   * Wrapper for the ViewStockLevels API service.
   *
   * @var $sku string|array The SKU or SKUs
   * @return FALSE if not found, or an array of stock numbers.
   */
  function get_stock_level($sku) {
    try {
      $skucriteria = new stdClass;
      $skucriteria->string = $sku;
      $parameters = new ViewStockLevels;
      $parameters->strSKU = $skucriteria;
      $parameters->strB2BCustomerName = $this->_credentials['name'];
      $parameters->strB2BCustomerEmail = $this->_credentials['email'];
      $parameters->strB2BCustomerPassword = $this->_credentials['password'];

      $client = new OMSSoapClient($this->_endpoints['InventoryManager']);
      $response = $client->ViewStockLevels($parameters);
    } catch (SoapFault $e) {
      throw new OMSClientException($e->getMessage());
    }

    // For some reason SOAP dumps this out as XML, so we need parse it ourselves.
    $rawxml = $response->ViewStockLevelsResult;
    $rawxml = str_replace('utf-16', 'utf-8', $rawxml); // Sometimes OMS tells us it's utf-16, but it's not
    $xml = simplexml_load_string($rawxml);

    // Check that an SKU was found
    if (!isset($xml->Inventories->Inventories->Name)) {
      return FALSE;
    }

    return $xml->Inventories->Inventories;
  }
}


/**
 * Our custom SOAP class to ensure the XML is in the same format that OMS requires.
 */
class OMSSoapClient extends SoapClient {

  /**
   * Provide an accessor for the raw XML request.
   */
  var $_lastRequestXML = NULL;

  /**
   * Overwrite the __doRequest method to format the XML correctly.
   */
  public function __doRequest($request, $location, $action, $version, $one_way = NULL) {
    preg_match('|xmlns:ns1="(.*)"|', $request, $matches);
    $namespace = $matches[1];

    $request = str_replace('SOAP-ENV', 'soap', $request);
    $request = preg_replace('/<ns1:(\w+)/', '<$1 xmlns="'.$namespace.'"', $request, 1);
    $request = preg_replace('/<ns1:(\w+)/', '<$1', $request);
    $request = str_replace(array('/ns1:', 'xmlns:ns1="'.$namespace.'"'), array('/', ''), $request);

    $this->_lastRequestXML = $request;

    return parent::__doRequest($request, $location, $action, $version);
  }

  /**
   * Return the full XML for the last request.
   */
  public function getLastRequestXML() {
    return $this->_lastRequestXML;
  }
}


/**
 * OMSClientException
 */
class OMSClientException extends RuntimeException {}
