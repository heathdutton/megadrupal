<?php

/**
 * @file
 * Shipwire class used for testing that returns dummy responses. Do not use yet.
 */
class ShipwireDummy extends Shipwire {

  protected static $instance;

  /**
   * Load an instance of the class.
   * 
   * @return ShipwireDummy
   */
  public static function getInstance() {
    if (empty(self::$instance)) {
      self::$instance = new ShipwireDummy();
    }
    return self::$instance;
  }

  /**
   * Create the beginning of a Shipwire XML request.
   * 
   * @param string $type
   * 
   * @return string
   */
  protected function buildXMLResponseHead($type) {
    $head = '<?xml version="1.0" encoding="utf-8"?>';
    $head .= '<!DOCTYPE ' . $type . ' SYSTEM "http://www.shipwire.com/exec/download/' . $type . '.dtd">';
    $head .= '<' . $type . ' />';
    return $head;
  }

  /**
   * Sends API request for order fulfillment.
   * 
   * @param object $order
   * @param object $test_params
   * 
   * @return bool|object
   *   FALSE on error or Drupal Http response on success
   */
  public function requestFulfillment(ShipwireFulfillRequest $order, $test_params = NULL) {
    $xml_request = self::buildXmlFulfill($order);
    $xml = new SimpleXMLElement(self::buildXMLResponseHead('SubmitOrderResponse'));

    $status_response = 'OK';
    if (!empty($test_params->status_response)) {
      $status_response = $test_params->status_response;
    }
    $xml->addChild('Status', $status_response);

    $xml->addChild('TotalOrders', 1, 'Status');
    $xml->addChild('TotalItems', count($order->products), 'Status');
    $xml->addChild('TransactionId', '1319266806-257193-1', 'Status');
    $xml->addChild('OrderInformation', NULL, 'Status');
    $xml->addChild('Order', NULL, 'Status');
    $xml->addAttribute('number', $order->id, 'Order');
    $xml->addAttribute('id', '1319266806-257193-1', 'Order');

    $status_order = 'accepted';
    if (!empty($test_params->status_order)) {
      $status_order = $test_params->status_order;
    }
    $xml->addAttribute('status', $status_order, 'Order');
    $xml->addChild('Shipping', NULL, 'Order');

    $warehouse = 'LAX';
    if (!empty($test_params->warehouse)) {
      $warehouse = $test_params->warehouse;
    }
    $xml->addChild('Warehouse', $warehouse, 'Shipping');

    $service = 'LAX';
    if (!empty($test_params->service)) {
      $service = $test_params->service;
    }
    $xml->addChild('Service', $service, 'Shipping');

    $cost = '10.21';
    if (!empty($test_params->cost)) {
      $cost = $test_params->cost;
    }
    $xml->addChild('Cost', $cost, 'Shipping');

    $xml_response = $xml->asXML();
    return self::sendTestRequest('fulfill', $xml_request, $xml_response);
  }

  /**
   * Sends API request for inventory update.
   * 
   * @param object $inventory
   * @param object $test_params
   * 
   * @return FALSE|object
   *   FALSE on error or Drupal Http response on success
   */
  public function requestInventory($inventory, $test_params = NULL) {
    $response = new stdClass();
    $response->request = self::buildXmlInventory($inventory);

    return $response;
  }

  /**
   * Sends API request for order shipping rate.
   * 
   * @param ShipwireRateRequest $request
   *   The rate request.
   * @param array $test_params
   *   Array of test parameters that alter the response.
   * 
   * @return FALSE|array
   *   FALSE on error or response array on success
   */
  public function requestRate(ShipwireRateRequest $request, $test_params = NULL) {
    $xml_request = self::buildXmlRate($request);

    $xml = new SimpleXMLElement(self::buildXMLResponseHead('RateResponse'));

    $status_response = 'OK';
    if (!empty($test_params->status_response)) {
      $status_response = $test_params->status_response;
    }
    $xml->addChild('Status', $status_response);
    $xml_order = $xml->addChild('Order');
    $xml_order->addAttribute('sequence', 1);
    $xml_quotes = $xml_order->addChild('Quotes');

    $currency = 'USD';
    if (!empty($test_params->currency)) {
      $currency = $test_params->currency;
    }
    elseif (!empty($request->currency)) {
      $currency = $request->currency;
    }

    $warehouse = 'LAX';
    if (!empty($test_params->warehouse)) {
      $warehouse = $test_params->warehouse;
    }
    elseif (!empty($request->warehouse) && $request->warehouse != '00') {
      $warehouse = $request->warehouse;
    }

    $methods = self::getShippingMethodsRates($currency, $warehouse);
    if (!empty($test_params->shipping_methods) && is_array($test_params->shipping_methods)) {
      $methods = $test_params->shipping_methods;
    }

    foreach ($methods as $code => $method) {
      $xml_quote = $xml_quotes->addChild('Quote');
      $xml_quote->addAttribute('method', $code);

      $xml_quote->addChild('Warehouse', self::getWarehouseLabel($warehouse));

      $xml_service = $xml_quote->addChild('Service', $method->name);
      $xml_service->addAttribute('deliveryConfirmation', $method->delivery_confirmation);
      $xml_service->addAttribute('trackable', $method->trackable);
      $xml_service->addAttribute('signatureRequired', $method->signature_required);
      $xml_quote->addChild('CarrierCode', $method->carrier_code);

      $xml_cost = $xml_quote->addChild('Cost', $method->total);
      $xml_cost->addAttribute('currency', $method->currency);
      $xml_cost->addAttribute('converted', $method->currency_converted);
      $xml_cost->addAttribute('originalCurrency', $method->currency_original);
      $xml_cost->addAttribute('originalCost', $method->total);

      $xml_subtotals = $xml_quote->addChild('Subtotals');
      $subtotals = array('Freight', 'Insurance', 'Packaging', 'Handling');
      foreach ($subtotals as $name) {
        $key = strtolower($name);
        $xml_subtotal = $xml_subtotals->addChild('Subtotal');
        $xml_subtotal->addAttribute('type', $name);
        $xml_sub_cost = $xml_subtotal->addChild('Cost', $method->{"subtotal_$key"});
        $xml_sub_cost->addAttribute('currency', $method->currency);
        $xml_sub_cost->addAttribute('converted', $method->currency_converted);
        $xml_sub_cost->addAttribute('originalCurrency', $method->currency_original);
        $xml_sub_cost->addAttribute('originalCost', $method->{"subtotal_$key"});
      }

      $xml_delivery = $xml_quote->addChild('DeliveryEstimate');
      $xml_min = $xml_delivery->addChild('Minimum', $method->delivery_estimate_minimum);
      $xml_min->addAttribute('units', $method->delivery_estimate_units);
      $xml_max = $xml_delivery->addChild('Maximum', $method->delivery_estimate_maximum);
      $xml_max->addAttribute('units', $method->delivery_estimate_units);
    }

    return self::sendTestRequest('rate', $xml_request, $xml->asXML());
  }

  /**
   * Simulates send and receive of all communications.
   *
   * @param string $method
   *   The request method.
   * @param string $xml_request
   *   The raw request XML.
   * @param string $xml_response
   *   The raw response XML.
   * 
   * @return array
   *   The response array.
   */
  protected function sendTestRequest($method, $xml_request, $xml_response) {
    switch ($method) {
      case 'fulfill':
        $url = self::$endpoint . 'TrackingServices.php';
        $data = "OrderListXML=" . $xml_request;
        break;

      case 'inventory':
        $url = self::$endpoint . 'InventoryServices.php';
        $data = "InventoryUpdateXML=" . $xml_request;
        break;

      case 'rate':
        $url = self::$endpoint . 'RateServices.php';
        $data = "RateRequestXML=" . $xml_request;
        break;

      case 'test':
      case 'tracking':
        $url = self::$endpoint . 'TrackingServices.php';
        $data = "TrackingUpdateXML=" . $xml_request;
        break;
    }

    $data_xml = new SimpleXMLElement($xml_response);
    $data_json = drupal_json_decode(drupal_json_encode($data_xml));
    $response = array(
      'success' => TRUE,
      'request' => $data,
      'data' => $xml_response,
      'data_xml' => $data_xml,
      'data_json' => $data_json,
    );

    return $response;
  }

  /**
   * Simulates a rate response from Shipwire.
   * 
   * @return array
   *   Array of shipping rates keyed by shipping method.
   */
  public static function getShippingMethodsRates($currency = 'USD', $warehouse = 'CHI') {
    $methods = array();
    $service = new stdClass();

    $service->name = 'USPS Priority Mail Flat Rate Small Box';
    $service->method = 'GD';
    $service->delivery_confirmation = 'YES';
    $service->trackable = 'YES';
    $service->signature_required = 'NO';
    $service->carrier_code = 'USPS PMFS';
    $service->currency = $currency;
    $service->currency_original = self::getWarehouseCurrency($warehouse);
    $service->currency_converted = ($service->currency == $service->currency_original) ? 'NO' : 'YES';
    $service->subtotal_freight = 5.90;
    $service->subtotal_insurance = 0.00;
    $service->subtotal_packaging = 0.00;
    $service->subtotal_handling = 0.00;
    $service->total = 5.90;
    $service->delivery_estimate_minimum = 2;
    $service->delivery_estimate_maximum = 3;
    $service->delivery_estimate_units = 'days';
    $methods['GD'] = $service;

    $service2 = clone $service;

    $service2->name = 'UPS Second Day Air';
    $service2->method = '2D';
    $service2->CarrierCode = 'USPS 2D';
    $service2->subtotal_freight = 30.45;
    $service2->total = 30.45;
    $service2->delivery_estimate_minimum = 2;
    $service2->delivery_estimate_maximum = 2;
    $methods['2D'] = $service2;

    $service3 = clone $service;
    $service3->name = 'USPS Express Mail';
    $service3->method = '1D';
    $service3->CarrierCode = 'USPS XP';
    $service3->subtotal_freight = 34.50;
    $service3->total = 34.50;
    $service3->delivery_estimate_minimum = 1;
    $service3->delivery_estimate_maximum = 1;
    $methods['1D'] = $service3;

    return $methods;
  }

  /**
   * Return the currency code for a given warehouse code.
   * 
   * @param string $warehouse_code
   *   A valid Shiwpire warehouse code.
   * 
   * @return string
   */
  public static function getWarehouseCurrency($warehouse_code) {
    $currency = '';
    $currencies = array(
      'CHI' => 'USD',
      'HKG' => 'HKD',
      'LAX' => 'USD',
      'PHL' => 'USD',
      'REN' => 'USD',
      'TOR' => 'CAD',
      'UK' => 'GBP',
      'VAN' => 'CAD',
    );
    if (!empty($currencies[$warehouse_code])) {
      $currency = $currencies[$warehouse_code];
    }
    return $currency;
  }

}
