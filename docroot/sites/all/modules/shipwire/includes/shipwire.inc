<?php

/**
 * @file
 * Provides classes for Shipwire API integration.
 */

/**
 * Main class used for communication with Shipwire.
 */
class Shipwire {

  protected static $instance;
  protected static $server;
  protected static $endpoint;
  protected static $app_id = 3048610241825397;

  /**
   * Load an instance of the class.
   * 
   * @return Shipwire
   *   The loaded instance of Shipwire.
   */
  public static function getInstance() {
    if (empty(self::$instance)) {
      self::$instance = new Shipwire();
    }
    return self::$instance;
  }

  /**
   * Setup basic variables to determine which server to communicate with.
   */
  protected function __construct() {
    self::$server = variable_get('shipwire_response', 'Test');
    $endpoint = variable_get('shipwire_server', 'Test');
    self::$endpoint = ($endpoint == 'Test') ? 'https://api.beta.shipwire.com/exec/' : 'https://api.shipwire.com/exec/';
  }

  /**
   * Load variable that indicates if Shipwire is configured properly.
   */
  public static function operational() {
    return variable_get('shipwire_operational', FALSE);
  }

  /**
   * Sends API request for order fulfillment.
   * 
   * @param ShipwireFulfillRequest $request
   *   The populated fulfillment request.
   * 
   * @return FALSE|array
   *   FALSE on error or response array on success.
   */
  public function requestFulfillment(ShipwireFulfillRequest $request) {
    $request->validate();
    if (self::operational()) {
      return self::sendRequest('fulfill', self::buildXmlFulfill($request));
    }
  }

  /**
   * Sends API request for order shipping rate.
   * 
   * @param ShipwireRateRequest $rate_request
   *   The populated rate request.
   * 
   * @return FALSE|array
   *   FALSE on error or response array on success.
   */
  public function requestRate(ShipwireRateRequest $rate_request) {
    $rate_request->validate();
    if (self::operational()) {
      return self::sendRequest('rate', self::buildXmlRate($rate_request));
    }
  }

  /**
   * Sends API request for tracking update.
   * 
   * @param ShipwireTrackingRequest $request
   *   The populated tracking request.
   * 
   * @return FALSE|array
   *   FALSE on error or response array on success.
   */
  public function requestTracking(ShipwireTrackingRequest $request) {
    $request->validate();
    if (self::operational()) {
      return self::sendRequest('tracking', self::buildXmlTracking($request));
    }
  }

  /**
   * Sends API request for inventory update.
   * 
   * @param ShipwireInventoryRequest $request
   *   The populated inventory request.
   * 
   * @return FALSE|array
   *   FALSE on error or response array on success.
   */
  public function requestInventory(ShipwireInventoryRequest $request) {
    $request->validate();
    if (self::operational()) {
      return self::sendRequest('inventory', self::buildXmlInventory($request));
    }
  }

  /**
   * Sends API tracking request to test account login.
   * 
   * @return array
   *   The response array.
   */
  public function requestTest() {
    try {
      $request = new ShipwireTrackingRequest();
      $request->setBookmark(2);
      $response = self::sendRequest('test', self::buildXmlTracking($request));
      if ((string) $response['data_xml']->Status == '0') {
        variable_set('shipwire_operational', TRUE);
      }
      else {
        variable_set('shipwire_operational', FALSE);
      }
    }
    catch (Exception $e) {
      $message = 'Error testing login';
      Shipwire::logError($message, array(), WATCHDOG_ERROR, $e);
      variable_set('shipwire_operational', FALSE);
    }
  }

  /**
   * Create the beginning of a Shipwire XML request.
   * 
   * @param string $name_space
   *   The Shipwire name space for the request.
   * 
   * @return string
   *   Raw XML header tags string.
   */
  protected function buildXMLHead($name_space) {
    $head = '<?xml version="1.0" encoding="utf-8"?>';
    $head .= '<!DOCTYPE ' . $name_space . ' SYSTEM "http://www.shipwire.com/exec/download/' . $name_space . '.dtd">';
    $head .= '<' . $name_space . ' />';
    return $head;
  }

  /**
   * Build the AddressInfo XML.
   * 
   * @param Shipwire $xml_element
   *   The request XML element.
   * @param ShipwireAddress $address
   *   A populated Shipwire Address.
   * @param string $method
   *   The type of Shipwire request this address will be used in. Options are:
   *   'rate' or 'fulfill'. 
   */
  protected function buildXmlAddressInfo(ShipwireSimpleXMLElement &$xml_element, ShipwireAddress $address, $method = 'rate') {
    $email_share = variable_get('shipwire_email_share', FALSE);

    $address_info = $xml_element->addChild('AddressInfo');
    $address_info->addAttribute('type', 'ship');
    $name = $address_info->addChild('Name');
    $name->addChildCData('Full', $address->name);

    if (!empty($address->company)) {
      $address_info->addChildCData('Company', $address->company);
    }
    $address_info->addChildCData('Address1', $address->address1);
    if (!empty($address->address2)) {
      $address_info->addChildCData('Address2', $address->address2);
    }
    if (!empty($address->address3)) {
      $address_info->addChildCData('Address3', $address->address3);
    }
    $address_info->addChildCData('City', $address->city);
    if (!empty($address->state)) {
      $address_info->addChildCData('State', $address->state);
    }
    $address_info->addChildCData('Country', $address->country);
    // Is postal required?
    if (!empty($address->postal)) {
      $address_info->addChildCData('Zip', $address->postal);
    }
    if (!empty($address->phone) && $method == 'fulfill') {
      $address_info->addChildCData('Phone', $address->phone);
    }
    if ($email_share && !empty($address->email) && $method == 'fulfill') {
      $address_info->addChildCData('Email', $address->email);
    }
    if (!empty($address->commercial)) {
      $address_info->addChild('Commercial');
    }
    if (!empty($address->pobox)) {
      $address_info->addChild('PoBox');
    }
  }

  /**
   * Builds XML API request for Order API.
   * 
   * @param ShipwireFulfillRequest $request
   *   The populated request object.
   * 
   * @return string
   *   The raw fulfillment request XML.
   */
  protected function buildXmlFulfill(ShipwireFulfillRequest $request) {
    $xml = new ShipwireSimpleXMLElement(self::buildXMLHead('OrderList'));
    $xml->addLogin(self::accountLoad(), 'fulfill');
    // The senders company.
    if (!empty($request->company)) {
      $xml->addChildCData('Company', $request->company);
    }

    $referer = 'Drupal ' . VERSION;
    if (!empty($request->module)) {
      $module_info = system_get_info('module', $request->module);
      if (!empty($module_info)) {
        $referer .= ' ' . $module_info['name'] . ' ' . $module_info['version'];
      }
    }
    $xml->addChildCData('Referer', $referer);

    foreach ($request->orders as $order) {

      $xml_order = $xml->addChild('Order');
      $xml_order->addAttribute('id', $order->order_id);

      if (!empty($order->warehouse)) {
        $xml_order->addChild('Warehouse', $order->warehouse);
      }

      self::buildXmlAddressInfo($xml_order, $order->address, 'fulfill');

      if (!empty($order->carrier)) {
        $xml_order->addChildCData('Carrier', $order->carrier);
      }
      elseif (!empty($order->shipping)) {
        $xml_order->addChild('Shipping', $order->shipping);
      }

      $count = 0;
      foreach ($order->products as $product) {
        $item = $xml_order->addChild('Item');
        $item->addAttribute('num', $count);
        $item->addChildCData('Code', $product->sku);
        $item->addChild('Quantity', $product->quantity);
        if (!empty($product->currency) && !empty($product->cost)) {
          $unit_value = $item->addChildCData('UnitValue', $product->cost);
          $unit_value->addAttribute('currency', $product->currency);
        }
        $count++;
      }
    }
    return $xml->asXML();
  }

  /**
   * Build XML for an inventory update request.
   * 
   * @return string
   *   The raw inventory request XML.
   */
  protected function buildXmlInventory(ShipwireInventoryRequest $request) {
    $xml = new ShipwireSimpleXMLElement(self::buildXMLHead('InventoryUpdate'));
    $xml->addLogin(self::accountLoad(), 'inventory');

    if (!empty($request->warehouse)) {
      $xml->addChild('Warehouse', $request->warehouse);
    }

    if (!empty($request->warehouse_continents) && is_array($request->warehouse_continents)) {
      $warehouse_continents = $xml->addChild('WarehouseContinents');
      foreach ($request->warehouse_continents as $continent) {
        $warehouse_continents->addChild('Continent', $continent);
      }
    }

    if (!empty($request->warehouse_country)) {
      $xml->addChild('warehouse_country', $request->warehouse_country);
    }

    if (!empty($request->products) && is_array($request->products)) {
      foreach ($request->products as $product) {
        $xml->addChildCData('ProductCode', $product);
      }
    }

    if (!empty($request->include_empty)) {
      $xml->addChild('IncludeEmpty');
    }

    return $xml->asXML();
  }

  /**
   * Build XML for tracking request.
   * 
   * @param ShipwireTrackingRequest $request
   *   The populated request object.
   * 
   * @return string
   *   The raw tracking request XML.
   */
  protected function buildXmlTracking(ShipwireTrackingRequest $request) {

    $xml = new ShipwireSimpleXMLElement(self::buildXMLHead('TrackingUpdate'));
    $xml->addLogin(self::accountLoad(), 'track');

    switch ($request->method) {
      case 'bookmark':
        $bookmark = '3';
        if (!empty($request->bookmark)) {
          $bookmark = $request->bookmark;
        }
        $xml->addChild('Bookmark', $bookmark);
        break;

      case 'order_number':
        foreach ($request->orders as $order) {
          $xml->addChild('OrderNo', $order);
        }
        break;

      case 'shipwire_id':
        foreach ($request->shipwire_ids as $shipwire_id) {
          $xml->addChild('ShipwireId', $shipwire_id);
        }
        break;
    }

    if (!empty($request->include_shipped_only)) {
      $xml->addChild('IncludeShippedOnly');
    }
    if (!empty($request->include_held_only)) {
      $xml->addChild('IncludeHeldOnly');
    }
    if (!empty($request->include_holds)) {
      $xml->addChild('IncludeHolds');
    }
    if (!empty($request->include_items)) {
      $xml->addChild('IncludeItems');
    }
    if (!empty($request->include_related_orders)) {
      $xml->addChild('IncludeRelatedOrders');
    }

    if (!empty($request->results_limit)) {
      if (!isset($request->results_offset)) {
        self::logError(t('Result offset must be provided when result limit is used'), WATCHDOG_ERROR);
      }
      else {
        $xml->addChild('ResultsLimit', $request->results_limit);
        $xml->addChild('ResultsOffset', $request->results_offset);
      }
    }

    return $xml->asXML();
  }

  /**
   * Build XML for shipping rates request.
   * 
   * @param ShipwireRateRequest $request
   *   The populated request object.
   * 
   * @return string
   *   The raw rate request XML.
   */
  protected function buildXmlRate(ShipwireRateRequest $request) {

    $xml = new ShipwireSimpleXMLElement(self::buildXMLHead('RateRequest'));
    if (!empty($request->rateset)) {
      $xml->addAttribute('rateset', $request->rateset);
    }
    $xml->addAttribute('currency', $request->currency);

    $xml->addLogin(self::accountLoad(), 'rate');

    foreach ($request->orders as $order) {
      $xml_order = $xml->addChild('Order');
      $xml_order->addChild('Warehouse', $order->warehouse);
      if (!empty($order->warehouse_continents)) {
        foreach ($order->warehouse_continents as $continent) {
          $xml_order->addChild('WarehouseContinents', $continent);
        }
      }

      self::buildXmlAddressInfo($xml_order, $order->address, 'rate');

      $count = 0;
      foreach ($order->products as $product) {
        $item = $xml_order->addChild('Item');
        $item->addAttribute('num', $count);
        $item->addChildCData('Code', $product->sku);
        $item->addChild('Quantity', $product->quantity);
        $count++;
      }
    }

    return $xml->asXML();
  }

  /**
   * Get a list of valid shipping methods used by Shipwire.
   * 
   * @return array
   *   Array of translated shipping methods keyed by method code.
   */
  public static function getShippingMethods() {
    return array(
      '1D' => t('One day shipping'),
      '2D' => t('Two day shipping'),
      'GD' => t('Ground shipping'),
      'E-INTL' => t('International economy shipping'),
      'INTL' => t('International standard shipping'),
      'PL-INTL' => t('International plus shipping'),
      'PM-INTL' => t('International premium shipping'),
    );
  }

  /**
   * Get a translated shipping method label for a given shipping method code.
   * 
   * @param string $method_code
   *   See getShippingMethods() for a list of valid method codes.
   * 
   * @return string
   *   Translated shipping method label.
   */
  public static function getShippingMethodLabel($method_code) {
    $methods = self::getShippingMethods();
    $label = '';
    if (!empty($methods[$method_code])) {
      $label = $methods[$method_code];
    }
    return $label;
  }

  /**
   * Get a list of valid status codes used by this module.
   * 
   * @return array
   *   Array of translated shipment status codes keyed by machine name.
   */
  public static function getStatusCodes() {
    $status_other = array(
      'submitted' => t('Submitted'),
      'shipped' => t('Shipped'),
      'delivered' => t('Delivered'),
      'held' => t('Held'),
    );
    $status_pending = self::getStatusCodesPending();

    return array_merge($status_other, $status_pending);
  }

  public static function getStatusCodesPending() {
    $status_all = &drupal_static(__FUNCTION__);
    if (!isset($status_all)) {
      $pending_default = array(
        'pending' => t('Pending'),
      );

      $pending_new = module_invoke_all('shipwire_pending_status');
      $status_all = array_merge($pending_default, $pending_new);
    }
    return $status_all;
  }

  /**
   * Return a translated status label for a given status machine code.
   * 
   * @param string $status_code
   *   The statis machine code.
   * 
   * @return string
   *   Return empty string or status label.
   */
  public static function getStatusLabel($status_code) {
    $statuses = self::getStatusCodes();
    $label = '';
    if (!empty($statuses[$status_code])) {
      $label = $statuses[$status_code];
    }
    return $label;
  }

  /**
   * Get a list of warehouse options.
   * 
   * @param bool $translated
   *   TRUE to return warehouse names are translated.
   * @param bool $include_auto
   *   TRUE to return warehouse code 00 for auto warehouse selection.
   * 
   * @return array
   *   Array of warehouses keyed by warehouse code with value of warehouse name.
   */
  public static function getWarehouses($translated = TRUE, $include_auto = FALSE) {
    $warehouses = array(
      'CHI' => 'Chicago',
      'HKG' => 'Hong Kong',
      'LAX' => 'Los Angeles',
      'PHL' => 'Philadelphia',
      'TOR' => 'Toronto',
      'UK' => 'United Kingdom',
      'VAN' => 'Vancouver',
    );
    if ($include_auto === TRUE) {
      $warehouses = array('00' => 'Auto select warehouse') + $warehouses;
    }
    if ($translated === TRUE) {
      foreach ($warehouses as $code => $name) {
        $warehouses[$code] = t($name);
      }
    }
    return $warehouses;
  }

  /**
   * Return a translated warehouse name given a valid warehouse code.
   * 
   * @param string $warehouse_code
   *   The warehouse code. See getWarehouses() for a list of valid codes.
   * 
   * @return string
   *   The translated warehouse name.
   */
  public static function getWarehouseLabel($warehouse_code) {
    $warehouses = self::getWarehouses();
    $name = '';
    if (!empty($warehouses[$warehouse_code])) {
      $name = $warehouses[$warehouse_code];
    }
    return $name;
  }

  /**
   * Get a warehouse code using warehouse name.
   * 
   * @param string $warehouse_name
   *   The English name of the warehouse.
   * 
   * @return string
   *   The warehouse code.
   */
  public static function getWarehouseCode($warehouse_name) {
    $warehouses = self::getWarehouses(FALSE);
    $code = array_search($warehouse_name, $warehouses);

    // Workaround for confirmed bug in Shipwire. Warehouse is supposed to be
    // full name but in the case of 'Los Angeles' it is returned as 'LA'.
    // To be fixed by Shipwire.
    if ($warehouse_name == 'LA') {
      $code = 'LAX';
    }

    if ($code == FALSE) {
      $code = '';
    }

    return $code;
  }

  /**
   * Return a list of valid warehouse countries. 
   * 
   * @return array
   *   Translated warehouse countries keyed by 2 digit ISO country code.
   */
  public static function getWarehouseCountries() {
    return array(
      'US' => t('United States'),
      'HK' => t('Hong Kong'),
      'CA' => t('Canada'),
      'GB' => t('United Kingdom'),
    );
  }

  /**
   * Return a list of valid warehouse continets used by Shipwire.
   * 
   * @return array
   *   Array of translated warehouse continent keyed by Shipwire continent code.
   */
  public static function getWarehouseContinents() {
    return array(
      'NORTH_AMERICA' => t('North America'),
      'SOUTH_AMERICA' => t('South America'),
      'EUROPE' => t('Europe'),
      'ASIA' => t('Asia'),
      'AFRICA' => t('Africa'),
      'AUSTRALIA' => t('Australia'),
      'ANTARCTICA' => t('Antarctica'),
    );
  }

  /**
   * Return a list of translated carrier names and services.
   * 
   * @return array
   *   Array of translated carrier names and services keyed by Shipwire carrier
   *   and service code.
   */
  public static function getCarriers() {
    return array(
      'CP EP' => t('Canada Post Expedited Parcel'),
      'CP EXUB' => t('Canada Post Expedited US Business'),
      'CP PA' => t('Canada Post Parcel Air'),
      'CP PC' => t('Canada Post Priority Courier'),
      'CP PUR' => t('Canada Post Purolator International'),
      'CP RP' => t('Canada Post Regular Parcel'),
      'CP SPA' => t('Canada Post Small Packet Air'),
      'CP XP' => t('Canada Post Xpresspost'),
      'CP XPI' => t('Canada Post XPressPost International'),
      'CP XPU' => t('Canada Post Xpresspost USA'),
      'CW FT' => t('Merchant Freight Service'),
      'DHL XPW' => t('DHL Express Worldwide'),
      'DHLGM LP' => t('DHL Global Mail Lightweight Parcel'),
      'DHLGM PPG' => t('DHL Global Mail Packet Plus'),
      'FDX 1D' => t('FedEx Standard Overnight'),
      'FDX 1DP' => t('FedEx Priority Overnight'),
      'FDX 2D' => t('FedEx 2Day'),
      'FDX GD' => t('FedEx Ground'),
      'FDX GDH' => t('FedEx Home Delivery'),
      'FDX GDS' => t('FedEx Ground Signature Required'),
      'FDX GHS' => t('FedEx Home Delivery Signature Required'),
      'HKP AM' => t('Hongkong Post International Registered Mail'),
      'HKP AP' => t('Hongkong Post International Air Parcel'),
      'HKP SP' => t('Hongkong Post Speedpost'),
      'HMS GD' => t('Hermes Standard'),
      'PF 1D' => t('Parcelforce Express24'),
      'PF 2D' => t('Parcelforce Express48'),
      'PF 2DL' => t('Parcelforce Express48 Large'),
      'PF DP' => t('Parcelforce Global Express'),
      'PF EU' => t('Parcelforce Euro Priority Business'),
      'PF IEC' => t('Parcelforce Global Value'),
      'PF ISC' => t('Parcelforce Global Priority'),
      'PUR GD' => t('Purolator Ground'),
      'PUR GDUS' => t('Purolator Ground US'),
      'PUR XP' => t('Purolator Express'),
      'PUR XPI' => t('Purolator Express International'),
      'PUR XPUS' => t('Purolator Express US'),
      'RM AM' => t('Royal Mail Airmail'),
      'RM AML' => t('Royal Mail Airmail Letter'),
      'RM AMS' => t('Royal Mail Airsure'),
      'RM AMSL' => t('Royal Mail Airsure Letter'),
      'RM FC' => t('Royal Mail First Class Letter'),
      'RM FCL' => t('Royal Mail First Class Large Letter'),
      'RM FCLR' => t('Royal Mail First Class Large Letter - Recorded Signed For'),
      'RM FCP' => t('Royal Mail First Class Packet'),
      'RM FCPR' => t('Royal Mail First Class Packet - Recorded Signed For'),
      'RM FCR' => t('Royal Mail First Class Letter - Recorded Signed For'),
      'RM ISF' => t('Royal Mail International Signed For'),
      'RM ISFL' => t('Royal Mail International Signed For Letter'),
      'RM SC' => t('Royal Mail Second Class Letter'),
      'RM SCL' => t('Royal Mail Second Class Large Letter'),
      'RM SCLR' => t('Royal Mail Second Class Large Letter - Recorded Signed For'),
      'RM SCP' => t('Royal Mail Second Class Packet'),
      'RM SCPR' => t('Royal Mail Second Class Packet - Recorded Signed For'),
      'RM SCR' => t('Royal Mail Second Class Letter - Recorded Signed For'),
      'RM SDND' => t('Royal Mail Special Delivery Next Day'),
      'TGX GD' => t('TGX'),
      'TNT PTH' => t('TNT Pakket 30kg'),
      'TNT PTW' => t('TNT Pakket 20kg'),
      'TPC FT' => t('Third Party Freight Carrier'),
      'TPC PL' => t('Third Party Parcel Carrier'),
      'UPS 1D' => t('UPS Next Day Air'),
      'UPS 2D' => t('UPS Second Day Air'),
      'UPS 3D' => t('UPS Three-Day Select'),
      'UPS GD' => t('UPS Ground'),
      'UPS GDS' => t('UPS Ground Signature Required'),
      'UPS SP0' => t('UPS SurePost Small'),
      'UPS SP1' => t('UPS SurePost Large'),
      'UPS ST' => t('UPS Standard'),
      'UPS WPS' => t('UPS Expedited Signature Required'),
      'UPS WSV' => t('UPS Saver'),
      'UPS WXP' => t('UPS Expedited'),
      'UPS WXS' => t('UPS Express'),
      'USPS EMI' => t('USPS Express Mail International'),
      'USPS FC' => t('USPS First-Class Mail Parcel + Delivery Confirmation'),
      'USPS FCI' => t('USPS First-Class Mail International'),
      'USPS FCS' => t('USPS First-Class Mail Parcel + Signature Required'),
      'USPS MM' => t('USPS Media Mail'),
      'USPS PM' => t('USPS Priority Mail + Delivery Confirmation'),
      'USPS PMFL' => t('USPS Priority Mail Flat Rate Large Box'),
      'USPS PMFM' => t('USPS Priority Mail Flat Rate Medium Box'),
      'USPS PMFPE' => t('USPS Priority Mail Flat Rate Padded Envelope'),
      'USPS PMFS' => t('USPS Priority Mail Flat Rate Small Box'),
      'USPS PMI' => t('USPS Priority Mail International'),
      'USPS PMIFL' => t('USPS Priority Mail International Flat Rate Large Box'),
      'USPS PMIFM' => t('USPS Priority Mail International Flat Rate Medium Box'),
      'USPS PMIFPE' => t('USPS Priority Mail International Flat Rate Padded Envelope'),
      'USPS PMIFS' => t('USPS Priority Mail International Flat Rate Small Box'),
      'USPS PMS' => t('USPS Priority Mail + Signature Required'),
      'USPS XP' => t('USPS Express Mail'),
    );
  }

  /**
   * Get a translated carrier label for a given carrier code.
   * 
   * @param string $carrier_code
   *   Carrier code used by Shipwire. See getCarriers() for a full list.
   * 
   * @return string
   *   Translated carrier label.
   */
  public static function getCarrierName($carrier_code) {
    $carriers = self::getCarriers();
    $return = '';
    if (!empty($carriers[$carrier_code])) {
      $return = $carriers[$carrier_code];
    }
    return $return;
  }

  /**
   * List of valid currencies used by Shipwire.
   * 
   * @return array
   *   Array of translated currency names keyed by ISO 3 digit currency code.
   */
  public static function getCurrencies() {
    return array(
//      'AED' => t('United Arab Emirates Dirham'),
//      'AFN' => t('Afghanistan Afghani'),
//      'ALL' => t('Albania Lek'),
//      'AMD' => t('Armenia Dram'),
//      'ANG' => t('Netherlands Antilles Guilder'),
//      'AOA' => t('Angola Kwanza'),
//      'ARS' => t('Argentina Peso'),
      'AUD' => t('Australia Dollar'),
//      'AWG' => t('Aruba Guilder'),
//      'AZN' => t('Azerbaijan New Manat'),
//      'BAM' => t('Bosnia and Herzegovina Convertible Marka'),
//      'BBD' => t('Barbados Dollar'),
//      'BDT' => t('Bangladesh Taka'),
//      'BGN' => t('Bulgaria Lev'),
//      'BHD' => t('Bahrain Dinar'),
//      'BIF' => t('Burundi Franc'),
//      'BMD' => t('Bermuda Dollar'),
//      'BND' => t('Brunei Darussalam Dollar'),
//      'BOB' => t('Bolivia Boliviano'),
//      'BRL' => t('Brazil Real'),
//      'BSD' => t('Bahamas Dollar'),
//      'BTN' => t('Bhutan Ngultrum'),
//      'BWP' => t('Botswana Pula'),
//      'BYR' => t('Belarus Ruble'),
//      'BZD' => t('Belize Dollar'),
      'CAD' => t('Canada Dollar'),
//      'CDF' => t('Congo/Kinshasa Franc'),
      'CHF' => t('Switzerland Franc'),
//      'CLP' => t('Chile Peso'),
      'CNY' => t('China Yuan Renminbi'),
//      'COP' => t('Colombia Peso'),
//      'CRC' => t('Costa Rica Colon'),
//      'CUC' => t('Cuba Convertible Peso'),
//      'CUP' => t('Cuba Peso'),
//      'CVE' => t('Cape Verde Escudo'),
//      'CZK' => t('Czech Republic Koruna'),
//      'DJF' => t('Djibouti Franc'),
//      'DKK' => t('Denmark Krone'),
//      'DOP' => t('Dominican Republic Peso'),
//      'DZD' => t('Algeria Dinar'),
//      'EGP' => t('Egypt Pound'),
//      'ERN' => t('Eritrea Nakfa'),
//      'ETB' => t('Ethiopia Birr'),
      'EUR' => t('Euro Member Countries'),
//      'FJD' => t('Fiji Dollar'),
//      'FKP' => t('Falkland Islands (Malvinas) Pound'),
      'GBP' => t('United Kingdom Pound'),
//      'GEL' => t('Georgia Lari'),
//      'GGP' => t('Guernsey Pound'),
//      'GHS' => t('Ghana Cedi'),
//      'GIP' => t('Gibraltar Pound'),
//      'GMD' => t('Gambia Dalasi'),
//      'GNF' => t('Guinea Franc'),
//      'GTQ' => t('Guatemala Quetzal'),
//      'GYD' => t('Guyana Dollar'),
      'HKD' => t('Hong Kong Dollar'),
//      'HNL' => t('Honduras Lempira'),
//      'HRK' => t('Croatia Kuna'),
//      'HTG' => t('Haiti Gourde'),
//      'HUF' => t('Hungary Forint'),
//      'IDR' => t('Indonesia Rupiah'),
//      'ILS' => t('Israel Shekel'),
//      'IMP' => t('Isle of Man Pound'),
//      'INR' => t('India Rupee'),
//      'IQD' => t('Iraq Dinar'),
//      'IRR' => t('Iran Rial'),
//      'ISK' => t('Iceland Krona'),
//      'JEP' => t('Jersey Pound'),
//      'JMD' => t('Jamaica Dollar'),
//      'JOD' => t('Jordan Dinar'),
      'JPY' => t('Japan Yen'),
//      'KES' => t('Kenya Shilling'),
//      'KGS' => t('Kyrgyzstan Som'),
//      'KHR' => t('Cambodia Riel'),
//      'KMF' => t('Comoros Franc'),
//      'KPW' => t('Korea (North) Won'),
//      'KRW' => t('Korea (South) Won'),
//      'KWD' => t('Kuwait Dinar'),
//      'KYD' => t('Cayman Islands Dollar'),
//      'KZT' => t('Kazakhstan Tenge'),
//      'LAK' => t('Laos Kip'),
//      'LBP' => t('Lebanon Pound'),
//      'LKR' => t('Sri Lanka Rupee'),
//      'LRD' => t('Liberia Dollar'),
//      'LSL' => t('Lesotho Loti'),
//      'LTL' => t('Lithuania Litas'),
//      'LVL' => t('Latvia Lat'),
//      'LYD' => t('Libya Dinar'),
//      'MAD' => t('Morocco Dirham'),
//      'MDL' => t('Moldova Leu'),
//      'MGA' => t('Madagascar Ariary'),
//      'MKD' => t('Macedonia Denar'),
//      'MMK' => t('Myanmar (Burma) Kyat'),
//      'MNT' => t('Mongolia Tughrik'),
//      'MOP' => t('Macau Pataca'),
//      'MRO' => t('Mauritania Ouguiya'),
//      'MUR' => t('Mauritius Rupee'),
//      'MVR' => t('Maldives (Maldive Islands) Rufiyaa'),
//      'MWK' => t('Malawi Kwacha'),
//      'MXN' => t('Mexico Peso'),
//      'MYR' => t('Malaysia Ringgit'),
//      'MZN' => t('Mozambique Metical'),
//      'NAD' => t('Namibia Dollar'),
//      'NGN' => t('Nigeria Naira'),
//      'NIO' => t('Nicaragua Cordoba'),
//      'NOK' => t('Norway Krone'),
//      'NPR' => t('Nepal Rupee'),
//      'NZD' => t('New Zealand Dollar'),
//      'OMR' => t('Oman Rial'),
//      'PAB' => t('Panama Balboa'),
//      'PEN' => t('Peru Nuevo Sol'),
//      'PGK' => t('Papua New Guinea Kina'),
//      'PHP' => t('Philippines Peso'),
//      'PKR' => t('Pakistan Rupee'),
//      'PLN' => t('Poland Zloty'),
//      'PYG' => t('Paraguay Guarani'),
//      'QAR' => t('Qatar Riyal'),
//      'RON' => t('Romania New Leu'),
//      'RSD' => t('Serbia Dinar'),
//      'RUB' => t('Russia Ruble'),
//      'RWF' => t('Rwanda Franc'),
//      'SAR' => t('Saudi Arabia Riyal'),
//      'SBD' => t('Solomon Islands Dollar'),
//      'SCR' => t('Seychelles Rupee'),
//      'SDG' => t('Sudan Pound'),
//      'SEK' => t('Sweden Krona'),
//      'SGD' => t('Singapore Dollar'),
//      'SHP' => t('Saint Helena Pound'),
//      'SLL' => t('Sierra Leone Leone'),
//      'SOS' => t('Somalia Shilling'),
//      'SPL' => t('Seborga Luigino'),
//      'SRD' => t('Suriname Dollar'),
//      'STD' => t('São Tomé and Príncipe Dobra'),
//      'SVC' => t('El Salvador Colon'),
//      'SYP' => t('Syria Pound'),
//      'SZL' => t('Swaziland Lilangeni'),
//      'THB' => t('Thailand Baht'),
//      'TJS' => t('Tajikistan Somoni'),
//      'TMT' => t('Turkmenistan Manat'),
//      'TND' => t('Tunisia Dinar'),
//      'TOP' => t("Tonga Pa'anga"),
//      'TRY' => t('Turkey Lira'),
//      'TTD' => t('Trinidad and Tobago Dollar'),
//      'TVD' => t('Tuvalu Dollar'),
//      'TWD' => t('Taiwan New Dollar'),
//      'TZS' => t('Tanzania Shilling'),
//      'UAH' => t('Ukraine Hryvna'),
//      'UGX' => t('Uganda Shilling'),
      'USD' => t('United States Dollar'),
//      'UYU' => t('Uruguay Peso'),
//      'UZS' => t('Uzbekistan Som'),
//      'VEF' => t('Venezuela Bolivar'),
//      'VND' => t('Viet Nam Dong'),
//      'VUV' => t('Vanuatu Vatu'),
//      'WST' => t('Samoa Tala'),
//      'XAF' => t('Communauté Financière Africaine (BEAC) CFA Franc BEAC'),
//      'XCD' => t('East Caribbean Dollar'),
//      'XDR' => t('International Monetary Fund (IMF) Special Drawing Rights'),
//      'XOF' => t('Communauté Financière Africaine (BCEAO) Franc'),
//      'XPF' => t('Comptoirs Français du Pacifique (CFP) Franc'),
//      'YER' => t('Yemen Rial'),
//      'ZAR' => t('South Africa Rand'),
//      'ZMW' => t('Zambia Kwacha'),
//      'ZWD' => t('Zimbabwe Dollar'),
    );
  }

  /**
   * Saves obfuscated account info for better security than plain text.
   * 
   * @param string $username
   *   The account username which is an email address.
   * @param string $password
   *   The account password.
   */
  public function accountSave($username, $password) {
    if (user_access('edit shipwire settings')) {

      $len = strlen($username);
      $key = trim(self::$app_id, 293570648) + $len;
      $value = $username . $password;

      while (strlen($key) < 8) {
        $num = rand(97, 122);
        $letter = chr($num);
        if ($num % 2) {
          $key .= $letter;
        }
        else {
          $key = $letter . $key;
        }
      }
      $value_save = $key . base64_encode($value);
      variable_set('shipwire_account', $value_save);
      self::requestTest();
    }
  }

  /**
   * Load account info.
   * 
   * @return array
   *   An empty array on failure or populated array on success.
   */
  protected function accountLoad() {
    $return = array();
    $value_save = variable_get('shipwire_account', FALSE);

    if (!empty($value_save)) {
      $key = trim(preg_replace('/[^0-9]/', '', self::$app_id), 293570648);
      $len = preg_replace('/[^0-9]/', '', substr($value_save, 0, 8)) - $key;
      $value_decrypt = base64_decode(substr($value_save, 8));

      $return['user'] = substr($value_decrypt, 0, $len);
      $return['pass'] = substr($value_decrypt, $len);
      $return['app_id'] = $key;
      $return['server'] = self::$server;
    }

    return $return;
  }

  /**
   * Send and receive all communication.
   *
   * @param string $method
   *   The API communication method. Possible values are:
   *   rate, fulfill, tracking, inventory
   *   Do not use 'test' value which is there only to test login info is correct.
   * @param string $xml
   *   The raw XML request string that will be sent to Shipwire.
   * 
   * @return array
   *   The response array which contains keys:
   *   request - The raw request XML
   *   data - The raw response XML
   *   data_xml - SimpleXMLElement conversion of raw XML
   *   data_json - JSON conversion of raw XML. Not production ready.
   */
  protected function sendRequest($method, $xml) {
    $log_xml_request = variable_get('shipwire_log_xml_request', 0);
    $log_xml_response = variable_get('shipwire_log_xml_response', 0);

    if ($log_xml_request) {
      $message = t('Sent @method XML request: @xml', array('@method' => $method, '@xml' => $xml));
      watchdog('shipwire', $message, array(), WATCHDOG_DEBUG);
    }

    $error_tag = 'ErrorMessage';
    switch ($method) {
      case 'rate':
        $url = self::$endpoint . 'RateServices.php';
        $data = "RateRequestXML=" . trim($xml);
        $error_tag = 'Error';
        break;

      case 'fulfill':
        $url = self::$endpoint . 'FulfillmentServices.php';
        $data = "OrderListXML=" . trim($xml);
        break;

      case 'test':
      case 'tracking':
        $url = self::$endpoint . 'TrackingServices.php';
        $data = "TrackingUpdateXML=" . trim($xml);
        break;

      case 'inventory':
        $url = self::$endpoint . 'InventoryServices.php';
        $data = "InventoryUpdateXML=" . trim($xml);
        break;
    }

    if (!self::operational() && $method != 'test') {
      self::logError(t('Shipwire is not configured properly.') . ' ' . l(t('Configure Shipwire'), 'admin/shipwire/settings'), WATCHDOG_CRITICAL);
      return FALSE;
    }

    // Setup the cURL request.
    $ch = curl_init();
    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_VERBOSE, 0);
    curl_setopt($ch, CURLOPT_POST, 1);
    curl_setopt($ch, CURLOPT_POSTFIELDS, $data);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 1);
    curl_setopt($ch, CURLOPT_NOPROGRESS, 1);
    curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 0);
    $response = curl_exec($ch);
    $error = curl_error($ch);
    curl_close($ch);

    if (!empty($response) && $log_xml_response) {
      $message = t('Sent @method XML response: @xml', array('@method' => $method, '@xml' => $response));
      watchdog('shipwire', $message, array(), WATCHDOG_DEBUG);
    }

    if ($error) {
      $message = 'Error in @method request communication: @error';
      $message_vars = array('@method' => $method, '@error' => check_plain($error));
      throw new Exception(t($message, $message_vars));
    }

    $data_xml = new SimpleXMLElement($response);
    $data_json = drupal_json_decode(drupal_json_encode($data_xml));

    if ((string) $data_xml->Status == 'Error') {
      $message = 'Error in @method response: @error';
      $message_vars = array('@method' => $method, '@error' => check_plain((string) $data_xml->{$error_tag}));
      throw new Exception(t($message, $message_vars));
    }

    return array(
      'request' => $data,
      'data' => $response,
      'data_xml' => $data_xml,
      'data_json' => $data_json,
    );
  }

  /**
   * Log error to screen and possibly watchdog.
   * 
   * If the user executing requests has sufficient permission then error messages
   * are displayed on screen. All warnings and error are logged using Watchdog.
   * Optionally WATCHDOG_DEBUG and WATCHDOG_INFO messages can be looged depending
   * on user preferences.
   * 
   * @param string $message
   *   Untranslated text message [not passed through t()]. It can contain
   *   standard drupal replacement patterns.  
   * @param array $variables
   *   Array of replacement pattern values keyed by replacement pattern.
   * @param int $watchdog_severity
   *   Watchdog serevity constant such as WATCHDOG_ERROR.
   * @param object $exception
   *   An optional exception.
   */
  public static function logError($message, $variables, $watchdog_severity, $exception = NULL) {
    switch ($watchdog_severity) {
      case WATCHDOG_EMERGENCY:
      case WATCHDOG_ALERT:
      case WATCHDOG_CRITICAL:
      case WATCHDOG_ERROR:
        $drupal_severity = 'error';
        break;

      case WATCHDOG_WARNING:
        $drupal_severity = 'warning';
        break;

      default:
        $drupal_severity = 'status';
    }

    $message = 'Shipwire: ' . $message;

    if (!empty($exception)) {
      require_once DRUPAL_ROOT . '/includes/errors.inc';
      $message .= ' ' . '%type: !message in %function (line %line of %file).';
      $variables += _drupal_decode_exception($exception);
    }

    $log_error = TRUE;

    if ($drupal_severity == 'status' && !variable_get('shipwire_log_errors', 0)) {
      $log_error = FALSE;
    }

    if ($log_error) {
      watchdog('shipwire', $message, $variables, $watchdog_severity);
    }

    if ($drupal_severity != 'status' && user_access('edit shipwire settings')) {
      drupal_set_message(t($message, $variables), $drupal_severity);
    }
  }

}

/**
 * Object used to build a valid address in the format used by Shipwire.
 */
class ShipwireAddress {

  protected $name;
  protected $address1;
  protected $address2;
  protected $address3;
  protected $city;
  protected $state;
  protected $country;
  protected $postal;
  protected $email;
  protected $phone;
  protected $commercial = FALSE;
  protected $pobox = FALSE;

  public function __get($property) {
    if (isset($this->$property)) {
      $val = $this->$property;
      return $val;
    }
    else {
      throw new Exception(t('Property "@property" does not exist.', array('@property' => $property)));
    }
  }

  public function __isset($property) {
    return isset($this->$property);
  }

  /**
   * Set shipping address receiver name. Required.
   * 
   * @param string $full_name
   *   The receivers full name limited to 25 chracters.
   * 
   * @throws Exception
   *   Exception when name is empty.
   */
  public function setName($full_name) {
    $full_name = substr(trim($full_name), 0, 25);
    if (!empty($full_name)) {
      $this->name = $full_name;
    }
    else {
      throw new Exception(t('Address name cannot be empty'));
    }
  }

  /**
   * Set the first line of the shipping address. Required.
   * 
   * @param string $address1
   *   The first line of the address limited to 25 characters.
   * 
   * @throws Exception
   *   Exception when empty.
   */
  public function setAddress1($address1) {
    $address1 = substr(trim($address1), 0, 25);
    if (!empty($address1)) {
      $this->address1 = $address1;
    }
    else {
      throw new Exception(t('Address street line 1 cannot be empty'));
    }
  }

  /**
   * Set the second line of the shipping address. Optional.
   * 
   * @param string $address2
   *   The second line of the address limited to 25 characters.
   */
  public function setAddress2($address2) {
    $address2 = substr(trim($address2), 0, 25);
    if (!empty($address2)) {
      $this->address2 = $address2;
    }
  }

  /**
   * Set the third line of the shipping address. Optional.
   * 
   * @param string $address3
   *   The third line of the address limited to 25 characters.
   */
  public function setAddress3($address3) {
    $address3 = substr(trim($address3), 0, 25);
    if (!empty($address3)) {
      $this->address3 = $address3;
    }
  }

  /**
   * Set the shipping address city. Required.
   * 
   * @param string $city
   *   The address city text limited to 25 characters.
   * 
   * @throws Exception
   *   Exception when empty.
   */
  public function setCity($city) {
    $city = substr(trim($city), 0, 25);
    if (!empty($city)) {
      $this->city = $city;
    }
    else {
      throw new Exception(t('Address city cannot be empty'));
    }
  }

  /**
   * Set the shipping address state/province and country.
   * 
   * State format varies depending on country. If country is USA (US) or Canada
   * (CA) then the state must be a valid two letter state code. For all other
   * countries the state/province is the full text limited to 25 characters.
   * 
   * @param string $state
   *   Two letter state code for US and CA countries. Otherwise full text
   *   limited to 25 characters.
   * @param string $country
   *   Two letter country code as defined in ISO 3166 standard.
   * 
   * @throws Exception
   *   If invalid country or state codes are used.
   */
  public function setStateCountry($state, $country) {
    include_once DRUPAL_ROOT . '/includes/iso.inc';
    $countries = _country_get_predefined_list();
    if (isset($countries[$country])) {
      $this->country = $country;
      if (in_array($country, array('US', 'CA'))) {
        if (strlen($state) > 2) {
          throw new Exception(t('State must be a 2 letter code for Canada and USA.'));
        }
        else {
          $this->state = $state;
        }
      }
      else {
        $this->state = substr(trim($state), 0, 25);
      }
    }
    else {
      throw new Exception(t('Country code "@country" is not a valid.', array('@country' => $country)));
    }
  }

  /**
   * Set the shipping adress postal/zip code.
   * 
   * @param string $postal_code
   *   Postal/Zip code limited to 25 characters.
   */
  public function setPostal($postal_code) {
    $postal_code = substr(trim($postal_code), 0, 25);
    if (!empty($postal_code)) {
      $this->postal = $postal_code;
    }
  }

  /**
   * Set commercial address flag.
   * 
   * @param bool $boolean
   *   TRUE if the shipping address is to a commercial location.
   * 
   * @throws Exception
   */
  public function setCommercial($boolean) {
    if (is_bool($boolean)) {
      $this->commercial = $boolean;
    }
    else {
      throw new Exception(t('Commercial address option value must be a boolean.'));
    }
  }

  /**
   * Set PO Box shipping address flag.
   * 
   * @param bool $boolean
   *   TRUE if the ship[ping address is to a PO box.
   * 
   * @throws Exception
   */
  public function setPoBox($boolean) {
    if (is_bool($boolean)) {
      $this->pobox = $boolean;
    }
    else {
      throw new Exception(t('PoBox option value must be a boolean.'));
    }
  }

  /**
   * Set customer's email address.
   * 
   * Email address is used by Shipwire to provide 'Delight' tools which can be
   * used to notify customer of shipment status.
   * 
   * @param string $email
   *   A valid email address. 
   */
  public function setEmail($email) {
    if (valid_email_address($email)) {
      $this->email = $email;
    }
  }

  /**
   * Set customer's telephone number.
   * 
   * Used if shipment carrier needs to get in contact with the customer.
   * 
   * @param string $phone
   *   The telephone number limited to 50 characters.
   */
  public function setPhone($phone) {
    $phone = substr(trim($phone), 0, 50);
    if (!empty($phone)) {
      $this->phone = $phone;
    }
  }

  /**
   * Get the address properties as a keyed array.
   * 
   * @return array
   */
  public function toArray() {
    return (array) get_class_vars(__CLASS__);
  }

  /**
   * Validate the address ensuring all required properties are present.
   * 
   * Required properties include: name, address1, city, country.
   * 
   * @throws Exception
   */
  public function validate() {
    $required = array('name', 'address1', 'city', 'country');
    foreach ($required as $property) {
      if (empty($this->$property)) {
        throw new Exception(t('Address property "@property" is required.', array('@property' => $property)));
      }
    }
  }

}

/**
 * Object used to build a Rate request order item.
 * 
 * Used as a sub element of ShipwireRateRequest. Requires order_id, address, and
 * products array. Optionally set warehouse filters to control where the
 * shipment will be sent from. Warehouse '00' indicates the default auto
 * selection of warehouse.  
 */
class ShipwireRateRequestOrder {

  protected $order_id;
  protected $warehouse = '00';
  protected $warehouse_continents = array();
  protected $warehouse_country;
  protected $address;
  protected $products;

  public function __get($property) {
    if (isset($this->$property)) {
      return $this->$property;
    }
    else {
      throw new Exception(t('Property "@property" does not exist.', array('@property' => $property)));
    }
  }

  public function __isset($property) {
    return isset($this->$property);
  }

  /**
   * Set the order id used in the local store to reference the original order.
   * 
   * If using multiple online stores with one Shipwire account then the order id
   * must ensured to be unique preferably by adding a prefix code.
   *  
   * @param string $order_id
   *   Unique order id used by the local software to idenitify and order.
   * 
   * @throws Exception
   *   If order id is empty.
   */
  public function setOrderId($order_id) {
    if (empty($order_id)) {
      throw new Exception(t('Shipwire order id cannot be empty.'));
    }
    else {
      $this->order_id = $order_id;
    }
  }

  /**
   * Set the warehouse code to restrict shipment to this warehouse.
   * 
   * @param string $warehouse_code
   *   The Shipwire warehouse code. See getWarehouseCodes() for valid codes.
   * 
   * @throws Exception
   *   If a warehouse code is not valid.
   */
  public function setWarehouse($warehouse_code) {
    $warehouses = Shipwire::getWarehouses(TRUE, TRUE);
    if (!empty($warehouses[$warehouse_code])) {
      $this->warehouse = $warehouse_code;
    }
    else {
      throw new Exception(t('Warehouse code "@warehouse" is not a valid.', array('@warehouse' => $warehouse_code)));
    }
  }

  /**
   * Set the shipping address for the order.
   * 
   * @param ShipwireAddress $address
   *   The populated shipping address in shipwire format.
   */
  public function setAddress(ShipwireAddress $address) {
    $this->address = $address;
  }

  /**
   * Add a product line item to the order.
   * 
   * @param ShipwireProduct $product
   *   The product information in Shipwire format. 
   */
  public function addProduct(ShipwireProduct $product) {
    if (!is_array($this->products)) {
      $this->products = array();
    }
    $this->products[] = $product;
  }

  /**
   * Add an array of products to an order.
   * 
   * @param array $products
   *   Array of ShipwireProduct objects.
   * @param bool $reset
   *   TRUE to delete current products, FALSE to append products.
   * 
   * @throws Exception
   */
  public function addProducts($products, $reset = FALSE) {
    if ($reset === TRUE) {
      $this->products = NULL;
    }
    if (empty($products)) {
      throw new Exception(t('Shipwire products array cannot be empty.'));
    }
    else {
      foreach ($products as $product) {
        $this->addProduct($product);
      }
    }
  }

  /**
   * Remove a product from the order array.
   * 
   * @param int $key
   *   The product array key.
   * 
   * @throws Exception
   */
  public function deleteProduct($key) {
    if (isset($this->products[$key])) {
      unset($this->products[$key]);
      if (empty($this->products)) {
        $this->products = NULL;
      }
    }
    else {
      throw new Exception(t('Products key "@key" is not a valid.', array('@key' => $key)));
    }
  }

  /**
   * Validate the shipment order checking required properties are set.
   * 
   * Required properties include: order_id, address, products.
   * Address validation is also triggered.
   *  
   * @throws Exception
   */
  public function validate() {
    $required = array('order_id', 'address', 'products');
    foreach ($required as $property) {
      if (is_null($this->$property)) {
        throw new Exception(t('Property "@property" is required.', array('@property' => $property)));
      }
      elseif ($property == 'address') {
        $this->address->validate();
      }
    }
  }

}

/**
 * Object used to build a properly formatted fulfillment request order.
 */
class ShipwireFulfillRequestOrder {

  protected $order_id;
  protected $hold = FALSE;
  protected $force_duplicate = FALSE;
  protected $can_split = FALSE;
  protected $same_day;
  protected $do_not_ship_before;
  protected $company_name;
  protected $warehouse = '00';
  protected $warehouse_continents;
  protected $warehouse_country;
  protected $address;
  protected $shipping;
  protected $carrier;
  protected $products;
  protected $invoice = FALSE;
  protected $message = FALSE;

  public function __get($property) {
    if (isset($this->$property)) {
      return $this->$property;
    }
    else {
      throw new Exception(t('Property "@property" does not exist.', array('@property' => $property)));
    }
  }

  public function __isset($property) {
    return isset($this->$property);
  }

  /**
   * Set the order id used in the local store to reference the original order.
   * 
   * If using multiple online stores with one Shipwire account then the order id
   * must ensured to be unique preferably by adding a prefix code.
   *  
   * @param string $order_id
   *   Unique order id used by the local software to idenitify and order.
   * 
   * @throws Exception
   *   If order id is empty.
   */
  public function setOrderId($order_id) {
    if (empty($order_id)) {
      throw new Exception(t('Shipwire order id cannot be empty.'));
    }
    else {
      $this->order_id = $order_id;
    }
  }

  /**
   * Set the warehouse code to restrict shipment to this warehouse.
   * 
   * @param string $warehouse_code
   *   The Shipwire warehouse code. See getWarehouseCodes() for valid codes.
   * 
   * @throws Exception
   *   If a warehouse code is not valid.
   */
  public function setWarehouse($warehouse_code) {
    $warehouses = Shipwire::getWarehouses(TRUE, TRUE);
    if (!empty($warehouses[$warehouse_code])) {
      $this->warehouse = $warehouse_code;
    }
    else {
      throw new Exception(t('Warehouse code "@warehouse" is not a valid.', array('@warehouse' => $warehouse_code)));
    }
  }

  /**
   * Set warehouse country code to restrict shipment to originate from country.
   * 
   * @param string $country_code
   *   ISO 3166 two digit country code.
   * 
   * @throws Exception
   */
  public function setWarehouseCountry($country_code) {
    $countries = Shipwire::getWarehouseCountries();
    if (!empty($countries[$country_code])) {
      $this->warehouse_country = $country_code;
    }
    else {
      throw new Exception(t('Warehouse country code "@country" is not a valid.', array('@country' => $country_code)));
    }
  }

  /**
   * Set warehouse continent to force shipment to originate from continent.
   * 
   * @param string $continent_code
   *   Valid Shipwire warehouse continent code. See getWarehouseContinents() for
   *   list of valid codes.
   * 
   * @throws Exception
   */
  public function addWarehouseContinent($continent_code) {
    $continents = Shipwire::getWarehouseContinents();
    if (!empty($continents[$continent_code])) {
      if (!is_array($this->warehouse_continents)) {
        $this->warehouse_continents = array();
      }
      $this->warehouse_continents[] = $continent_code;
    }
    else {
      throw new Exception(t('Warehouse continent code "@continent" is not a valid.', array('@continent' => $continent_code)));
    }
  }

  /**
   * Set array of warehouse continents to force shipment to originate from them.
   * 
   * @param array $continents
   *   Array of valid Shipwire warehouse continent code.
   *   See getWarehouseContinents() for list of valid codes.
   * @param bool $reset
   *   TRUE to delete current continents, FALSE to append continents.
   * 
   * @throws Exception
   */
  public function setWarehouseCountinents(array $continents, $reset = FALSE) {
    if ($reset === TRUE) {
      $this->warehouse_continents = array();
    }
    if (empty($continents)) {
      throw new Exception(t('Warehouse continents array cannot be empty.'));
    }
    else {
      if (!is_array($this->warehouse_continents)) {
        $this->warehouse_continents = array();
      }
      foreach ($continents as $continent) {
        $this->addWarehouseContinent($continent);
      }
    }
  }

  /**
   * Set the shipping address for the order.
   * 
   * @param ShipwireAddress $address
   *   The populated shipping address in shipwire format.
   */
  public function setAddress(ShipwireAddress $address) {
    $this->address = $address;
  }

  /**
   * Add a product line item to the order.
   * 
   * @param ShipwireProduct $product
   *   The product information in Shipwire format. 
   */
  public function addProduct(ShipwireProduct $product) {
    if (!is_array($this->products)) {
      $this->products = array();
    }
    $this->products[] = $product;
  }

  /**
   * Add an array of products to an order.
   * 
   * @param array $products
   *   Array of ShipwireProduct objects.
   * @param bool $reset
   *   TRUE to delete current products, FALSE to append products.
   * 
   * @throws Exception
   */
  public function addProducts($products, $reset = FALSE) {
    if ($reset === TRUE) {
      $this->products = NULL;
    }
    if (empty($products)) {
      throw new Exception(t('Products array cannot be empty.'));
    }
    else {
      foreach ($products as $product) {
        $this->addProduct($product);
      }
    }
  }

  /**
   * Remove a product from the order array.
   * 
   * @param int $key
   *   The product array key.
   * 
   * @throws Exception
   */
  public function deleteProduct($key) {
    if (isset($this->products[$key])) {
      unset($this->products[$key]);
      if (empty($this->products)) {
        $this->products = NULL;
      }
    }
    else {
      throw new Exception(t('Products key "@key" is not a valid.', array('@key' => $key)));
    }
  }

  /**
   * Set 'hold' flag for shipment indicating it will not ship until cleared.
   * 
   * @param bool $boolean
   *   TRUE to hold shipment.
   * 
   * @throws Exception
   */
  public function setHold($boolean) {
    if (is_bool($boolean)) {
      $this->hold = $boolean;
    }
    else {
      throw new Exception(t('Order hold must be a boolean.'));
    }
  }

  /**
   * Set 'force duplicate' flag to ship orders that appear to be the same.
   * 
   * The default setting of FALSE will cause Shipwire to check shipments. If a
   * duplicate order is found then the shipment will be held and will require
   * manual clearance.
   * 
   * @param bool $boolean
   *   TRUE to allow shipments that appear to be duplicates. Default is FALSE.
   * 
   * @throws Exception
   */
  public function setForceDuplicate($boolean) {
    if (is_bool($boolean)) {
      $this->force_duplicate = $boolean;
    }
    else {
      throw new Exception(t('Order force duplicate must be a boolean.'));
    }
  }

  /**
   * Set 'can split' tag allowing backordered products to be shipped later.
   * 
   * By default Shipwire will hold an order that contains out of stock products.
   * If this option is set to TRUE then available products will be shipped
   * immediately and another shipment will eb sent later when backordered
   * products are restocked.
   *   
   * @param bool $boolean
   *   Default FALSE. TRUE to allow order to be split into multiple shipments.
   * 
   * @throws Exception
   */
  public function setCanSplit($boolean) {
    if (is_bool($boolean)) {
      $this->can_split = $boolean;
    }
    else {
      throw new Exception(t('Order can split must be a boolean.'));
    }
  }

  /**
   * Set 'same day' flag indicating level of same day service requested.
   * 
   * Default setting is NOT REQUESTED. Other setting will incurr additional
   * shipping costs.
   * 
   * @param string $same_day
   *   One of the following values:
   *   'NOT REQUESTED' - default
   *   'REQUESTED' - if easily possible to ship same day
   *   'GUARANTEED' - force priority shipping
   * 
   * @throws Exception
   */
  public function setSameDay($same_day) {
    if (in_array($same_day, array('NOT REQUESTED', 'REQUESTED', 'GUARANTEED'))) {
      $this->same_day = $same_day;
    }
    else {
      throw new Exception(t('Same day code @same_day is not valid', array('@same_day' => $same_day)));
    }
  }

  /**
   * Set hold shipment on shipment until specified date.
   * 
   * @param date $date
   *   A unix timestamp indicating when the shipment should be sent.
   *   Ignored if in the past.
   * 
   * @throws Exception
   */
  public function setDoNotShipBefore($date) {
    $dp = date_parse($date);
    if (empty($dp['year']) || empty($dp['month']) || empty($dp['day'])) {
      throw new Exception(t('Do not ship before date requires year, month, and day.'));
    }
    elseif ($dp['error_count']) {
      throw new Exception(t('Do not ship before date could not be converted.'));
    }
    else {
      $datetime = strtotime($date);
      $this->do_not_ship_before = date_iso8601($datetime);
    }
  }

  /**
   * Set sender's comapny name used for international commercial invoices.
   *
   * @param string $company
   *   Senders company name limited to 25 characters.
   */
  public function setSenderCompanyName($company) {
    $this->company_name = substr(trim($company), 0, 25);
  }

  /**
   * Set the Shipwire shipping method.
   * 
   * @param string $shipping_method
   *   Shipwire shipping method code. See getShippingMethods() for valid codes.
   * 
   * @throws Exception
   */
  public function setShipping($shipping_method) {
    $shipping_methods = Shipwire::getShippingMethods();
    if (!empty($shipping_methods[$shipping_method])) {
      $this->shipping = $shipping_method;
    }
    else {
      throw new Exception(t('Shipping method @shipping_method is not valid.', array('@shipping_method' => $shipping_method)));
    }
  }

  /**
   * Validate the fulfillment request checking required properties are set.
   * 
   * Required properties include: order_id, address, products.
   * Address validation is also triggered.
   *  
   * @throws Exception
   */
  public function validate() {
    $required = array('order_id', 'shipping', 'address', 'products');
    foreach ($required as $property) {
      if (empty($this->$property)) {
        throw new Exception(t('ShipwireFulfillRequestOrder requires property "@property".', array('@property' => $property)));
      }
      elseif ($property == 'address') {
        foreach ($this->address as $item) {
          $item->validate();
        }
      }
    }
  }

}

/**
 * Object used to build product information used by Shipwire requests.
 */
class ShipwireProduct {

  protected $sku;
  protected $quantity;
  protected $currency;
  protected $cost;

  public function __construct($sku, $quantity, $currency = '', $cost = 0) {
    $this->setSku($sku);
    $this->setQuantity($quantity);
    if (!empty($currency) && !empty($cost)) {
      $this->setCost($cost, $currency);
    }
  }

  public function __get($property) {
    if (isset($this->$property)) {
      return $this->$property;
    }
    else {
      throw new Exception(t('Property "@property" does not exist.', array('@property' => $property)));
    }
  }

  public function __isset($property) {
    return isset($this->$property);
  }

  /**
   * Set the unique product SKU/model indentifier.
   * 
   * @param string $sku
   *   A string used to identify the product.
   *
   * @throws Exception
   */
  public function setSku($sku) {
    if (empty($sku)) {
      throw new Exception(t('Product SKU cannot be empty.'));
    }
    else {
      $this->sku = $sku;
    }
  }

  /**
   * Set product quantity requested.
   * 
   * @param int $quantity
   *   The number of this product in the shipment.
   * 
   * @throws Exception
   */
  public function setQuantity($quantity) {
    $quantity = (int) $quantity;
    if (empty($quantity)) {
      throw new Exception(t('Product quantity cannot be zero.'));
    }
    else {
      $this->quantity = $quantity;
    }
  }

  /**
   * Set product cost used for shipment insurance and customs calculation.
   * 
   * @param float $cost
   *   The cost of the product.
   * @param string $currency
   *   The three digit currency code as in ISO 4217.
   * 
   * @throws Exception
   */
  public function setCost($cost, $currency) {
    $currencies = Shipwire::getCurrencies();
    if (!empty($currencies[$currency])) {
      if ((float) $cost < 0) {
        throw new Exception(t('Product cost must be posistive.'));
      }
      else {
        $this->cost = $cost;
        $this->currency = $currency;
      }
    }
    else {
      throw new Exception(t('Product currency code "@currency" is not valid.', array('@currency' => $currency)));
    }
  }

  /**
   * Get the product properties as a keyed array.
   * 
   * @return array
   *   Array of product properties.
   */
  public function toArray() {
    return array(
      'sku' => $this->sku,
      'quantity' => $this->quantity,
      'currency' => $this->currency,
      'cost' => $this->cost,
    );
  }

}

/**
 * Object used to build a properly formatted Shipwire rate request.
 */
Class ShipwireRateRequest {

  protected $currency;
  protected $rateset;
  protected $orders;

  public function __get($property) {
    if (isset($this->$property)) {
      return $this->$property;
    }
    else {
      throw new Exception(t('Property "@property" does not exist.', array('@property' => $property)));
    }
  }

  public function __isset($property) {
    return isset($this->$property);
  }

  /**
   * Set the currency used to format the rate response costs.
   * 
   * Shipwire currently supports a limited number of currencies. 
   * See getCurrencies() for valid codes.
   * 
   * @param string $currency
   *   ISO 4217 three character currency code.
   * 
   * @throws Exception
   */
  public function setCurrency($currency) {
    $currencies = Shipwire::getCurrencies();
    if (!empty($currencies[$currency])) {
      $this->currency = $currency;
    }
    else {
      throw new Exception(t('Currency code "@currency" is not a valid.', array('@currency' => $currency)));
    }
  }

  /**
   * Not document in Shipwire RateRequest API.
   * 
   * @param string $rateset
   *   Either 'all' or 'shipwire'.
   * 
   * @throws Exception
   */
  public function setRateset($rateset) {
    if (in_array($rateset, array('all', 'shipwire'))) {
      $this->rateset = $rateset;
    }
    else {
      throw new Exception(t('Rateset code "@rateset" is not a valid.', array('@rateset' => $rateset)));
    }
  }

  /**
   * Add a ShipwireRateRequestOrder to the request.
   * 
   * @param ShipwireRateRequestOrder $order
   *   Fully populated Shipwire rate request order.
   */
  public function addOrder(ShipwireRateRequestOrder $order) {
    $this->orders[] = $order;
  }

  /**
   * Remove a ShipwireRateRequestOrder from the request.
   * 
   * @param int $key
   *   The array key of the order in the $this->orders array.
   * 
   * @throws Exception
   */
  public function deleteOrder($key) {
    if (isset($this->orders[$key])) {
      unset($this->orders[$key]);
      if (empty($this->orders)) {
        $this->orders = NULL;
      }
    }
    else {
      throw new Exception(t('Order key "@key" is not a valid.', array('@key' => $key)));
    }
  }

  /**
   * Validate the current request checking all required properties are set.
   * 
   * Request requires a currency and an array of valid orders.
   * 
   * @throws Exception
   */
  public function validate() {
    if (is_null($this->currency)) {
      throw new Exception(t('Request currency is required'));
    }
    if (is_null($this->orders)) {
      throw new Exception(t('Request order array cannot be empty.'));
    }
    else {
      foreach ($this->orders as $order) {
        $order->validate();
      }
    }
  }

}

/**
 * Object used to build a properly formatted Shipwire fulfillment request.
 */
class ShipwireFulfillRequest {

  protected $store_name;
  protected $processing_mode; // sync|async
  protected $orders;
  protected $module;

  public function __get($property) {
    if (isset($this->$property)) {
      return $this->$property;
    }
    else {
      throw new Exception(t('Property "@property" does not exist.', array('@property' => $property)));
    }
  }

  public function __isset($property) {
    return isset($this->$property);
  }

  /**
   * Add a ShipwireRateRequestOrder to the request.
   * 
   * @param ShipwireRateRequestOrder $order
   *   Fully populated Shipwire rate request order.
   */
  public function addOrder(ShipwireFulfillRequestOrder $order) {
    if (!is_array($this->orders)) {
      $this->orders = array();
    }
    $this->orders[] = $order;
  }

  /**
   * Remove a ShipwireRateRequestOrder from the request.
   * 
   * @param int $key
   *   The array key of the order in the $this->orders array.
   * 
   * @throws Exception
   */
  public function deleteOrder($key) {
    if (isset($this->orders[$key])) {
      unset($this->orders[$key]);
      if (empty($this->orders)) {
        $this->orders = NULL;
      }
    }
    else {
      throw new Exception(t('Order key "@key" is not a valid.', array('@key' => $key)));
    }
  }

  /**
   * Set the name of the drupal module that is initiating the request.
   * 
   * @param string $store_module
   *   A valid and currently active drupal module name.
   * 
   * @throws Exception
   */
  public function setModule($store_module) {
    if (!empty($store_module)) {
      if (module_exists($store_module)) {
        $this->module = $store_module;
      }
      else {
        throw new Exception(t('Module @module is not valid', array('@module' => $store_module)));
      }
    }
    else {
      throw new Exception(t('Module cannot be empty'));
    }
  }

  /**
   * Validate the fulfillment request ensuring all required properties are set.
   * 
   * The request requires an array of orders.
   * 
   * @throws Exception
   */
  public function validate() {
    if (empty($this->orders)) {
      throw new Exception(t('Request orders array cannot be empty.'));
    }
    else {
      foreach ($this->orders as $order) {
        $order->validate();
      }
    }
    if (is_null($this->module)) {
      throw new Exception(t('Store module cannot be empty.'));
    }
  }

}

/**
 * Object used to build a properly formatted Shipwire tracking request.
 */
class ShipwireTrackingRequest {

  protected $method;
  protected $bookmark;
  protected $orders;
  protected $shipwire_ids;
  protected $include_shipped_only = FALSE;
  protected $include_held_only = FALSE;
  protected $include_holds = FALSE;
  protected $include_items = FALSE;
  protected $include_related_orders = FALSE;
  protected $unique_properties = array('bookmark', 'orders', 'shipwire_ids');

  public function __get($property) {
    if (isset($this->$property)) {
      return $this->$property;
    }
    else {
      throw new Exception(t('Property "@property" does not exist.', array('@property' => $property)));
    }
  }

  public function __isset($property) {
    return isset($this->$property);
  }

  /**
   * Set bookmark type and use bookmark method to get tracking information.
   * 
   * 1 - All Orders: Returns tracking information for all orders in the 
   *  Shipwire account [DEPRECATED]
   * 2 - Orders since last bookmark: Returns tracking information for all orders
   *  updated since the last bookmark was set (see "3" below). 
   * 3 - Update bookmark: Returns tracking information for all orders updated
   *  since the last bookmark was set, and then updates the bookmark to now.
   * 
   * Bookmarks have not proven to be a reliable method of getting information.
   * It is recommended to use the ShipwireID or OrderID methods instead.
   * 
   * @param int $bookmark_type
   *   Integer 1, 2, or 3 indicating the type of bookmark.
   * 
   * @throws Exception
   */
  public function setBookmark($bookmark_type) {
    if (is_null($this->orders) && is_null($this->shipwire_ids)) {
      if (in_array($bookmark_type, array(1, 2, 3))) {
        $this->bookmark = $bookmark_type;
        $this->method = 'bookmark';
      }
      else {
        throw new Exception(t('Bookmark type code "@bookmark_type" is not valid', array('@bookmark_type' => $bookmark_type)));
      }
    }
    else {
      throw new Exception(t('Only one of the following tracking properties can'
        . ' be used: @properties', array(
        '@properties' => implode(', ', $this->unique_properties),
      )));
    }
  }

  /**
   * Add a locally used unique order id to get tracking info for it.
   * 
   * @param string $order_id
   *   The order id previously submitted to shipwire for fulfillment.
   * 
   * @throws Exception
   */
  public function addOrderId($order_id) {
    if (is_null($this->bookmark) && is_null($this->shipwire_ids)) {
      if (!empty($order_id)) {
        if (!is_array($this->orders)) {
          $this->orders = array();
        }
        $this->orders[] = $order_id;
        $this->method = 'order_number';
      }
      else {
        throw new Exception(t('Order ID cannot be empty.'));
      }
    }
    else {
      throw new Exception(t('Only one of the following tracking properties can'
        . ' be used: @properties', array(
        '@properties' => implode(', ', $this->unique_properties)
      )));
    }
  }

  /**
   * Add a shipwire id to get tracking info for it.
   * 
   * @param string $shipwire_id
   *   The unique id used remotely by Shipwire to identify a shipment.
   * 
   * @throws Exception
   */
  public function addShipwireId($shipwire_id) {
    if (is_null($this->bookmark) && is_null($this->orders)) {
      if (!empty($shipwire_id)) {
        if (!is_array($this->shipwire_ids)) {
          $this->shipwire_ids = array();
        }
        $this->shipwire_ids[] = $shipwire_id;
        $this->method = 'shipwire_id';
      }
      else {
        throw new Exception(t('Shipwire ID cannot be empty.'));
      }
    }
    else {
      throw new Exception(t('Only one of the following tracking properties can'
        . ' be used: @properties', array('@properties' => implode(', ', $this->unique_properties))));
    }
  }

  /**
   * Set 'shipped only' flag limiting response to shipments that have shipped.
   * 
   * @param bool $boolean
   *   TRUE to limit response to shipments that have shipped.
   * 
   * @throws Exception
   */
  public function setShippedOnly($boolean) {
    if (is_bool($boolean)) {
      $this->include_shipped_only = $boolean;
    }
    else {
      throw new Exception(t('Include shipped only property requires a boolean value.'));
    }
  }

  /**
   * Set 'held only' flag limiting response to shipments that are held.
   * 
   * @param bool $boolean
   *   TRUE to limit response to shipments that are held.
   * 
   * @throws Exception
   */
  public function setHeldOnly($boolean) {
    if (is_bool($boolean)) {
      $this->include_held_only = $boolean;
    }
    else {
      throw new Exception(t('Include held only property requires a boolean value.'));
    }
  }

  /**
   * Set 'include holds' flag to get additional hold error messages.
   * 
   * @param bool $boolean
   *   TRUE to get shipment hold error information.
   * 
   * @throws Exception
   */
  public function setIncludeHolds($boolean) {
    if (is_bool($boolean)) {
      $this->include_holds = $boolean;
    }
    else {
      throw new Exception(t('Include holds property requires a boolean value.'));
    }
  }

  /**
   * Set 'include items' flag to get additional info on products in shipment.
   * 
   * @param bool $boolean
   *   TRUE to get information on products in shipment.
   * 
   * @throws Exception
   */
  public function setIncludeItems($boolean) {
    if (is_bool($boolean)) {
      $this->include_items = $boolean;
    }
    else {
      throw new Exception(t('Include items property requires a boolean value.'));
    }
  }

  /**
   * Set flag to include information about related orders (return, replacement).
   * 
   * @param bool $boolean
   *   TRUE to get additional related order information.
   * 
   * @throws Exception
   */
  public function setIncludeRelatedOrders($boolean) {
    if (is_bool($boolean)) {
      $this->include_related_orders = $boolean;
    }
    else {
      throw new Exception(t('Include related orders property requires a boolean value.'));
    }
  }

  /**
   * Validate the request ensuring all required properties are set.
   * 
   * The request can use only one method of getting tracking information.
   * Order id - a list of order IDs to check.
   * Shipwire id - a list of Shipwire IDs to check.
   * Bookmark - an integer specifying the bookmark method.
   * 
   * @throws Exception
   */
  public function validate() {
    $unique_properties = $this->unique_properties;
    $unique_count = 0;
    foreach ($unique_properties as $property) {
      if (!is_null($this->$property)) {
        $unique_count++;
      }
    }
    if ($unique_count > 1) {
      throw new Exception(t('Only one of the following tracking properties may'
        . ' be used: @properties', array(
        '@properties' => implode(', ', $this->unique_properties)
      )));
    }
    elseif ($unique_count < 1) {
      throw new Exception(t('One of the following tracking types must be set: @properties', array('@properties' => $unique_properties)));
    }

    switch ($this->method) {
      case 'bookmark':
        if (empty($this->bookmark)) {
          throw new Exception(t('Bookmark tracking methid requested by boomark type is not set.'));
        }
        break;
      case 'order_number':
        if (empty($this->orders)) {
          throw new Exception(t('Order Number tracking method requested but order list is empty.'));
        }
        break;
      case 'shipwire_id':
        if (empty($this->shipwire_ids)) {
          throw new Exception(t('Shipwire ID tracking method requested but Shipwire ID list is empty.'));
        }
        break;
    }
  }

}

/**
 * Object used to build a properly formatted inventory request.
 */
class ShipwireInventoryRequest {

  protected $include_empty;
  protected $products;
  protected $warehouse;
  protected $warehouse_continents;
  protected $warehouse_country;

  public function __get($property) {
    if (isset($this->$property)) {
      return $this->$property;
    }
    else {
      throw new Exception(t('Property "@property" does not exist.', array('@property' => $property)));
    }
  }

  public function __isset($property) {
    return isset($this->$property);
  }

  public function setIncludeEmpty($boolean) {
    if (is_bool($boolean)) {
      $this->include_empty = $boolean;
    }
    else {
      throw new Exception(t('Include empty property value must be a boolean.'));
    }
  }

  /**
   * Set the warehouse code to restrict shipment to this warehouse.
   * 
   * @param string $warehouse_code
   *   The Shipwire warehouse code. See getWarehouseCodes() for valid codes.
   * 
   * @throws Exception
   *   If a warehouse code is not valid.
   */
  public function setWarehouse($warehouse_code) {
    $warehouses = Shipwire::getWarehouses(TRUE, FALSE);
    if (!empty($warehouses[$warehouse_code])) {
      $this->warehouse = $warehouse_code;
    }
    else {
      throw new Exception(t('Warehouse code "@warehouse" is not a valid.', array('@warehouse' => $warehouse_code)));
    }
  }

  /**
   * Set warehouse country code to force shipment to originate from country.
   * 
   * @param string $country_code
   *   ISO 3166 two digit country code.
   * 
   * @throws Exception
   */
  public function setWarehouseCountry($country_code) {
    $countries = Shipwire::getWarehouseCountries();
    if (!empty($countries[$country_code])) {
      $this->warehouse_country = $country_code;
    }
    else {
      throw new Exception(t('Warehouse country code "@country" is not a valid.', array('@country' => $country_code)));
    }
  }

  /**
   * Set warehouse continent to force shipment to originate from continent.
   * 
   * @param string $continent_code
   *   Valid Shipwire warehouse continent code. See getWarehouseContinents() for
   *   list of valid codes.
   * 
   * @throws Exception
   */
  public function addWarehouseContinent($continent_code) {
    $continents = Shipwire::getWarehouseContinents();
    if (!empty($continents[$continent_code])) {
      if (!is_array($this->warehouse_continents)) {
        $this->warehouse_continents = array();
      }
      $this->warehouse_continents[] = $continent_code;
    }
    else {
      throw new Exception(t('Warehouse continent code "@continent" is not a valid.', array('@continent' => $continent_code)));
    }
  }

  /**
   * Set array of warehouse continents to force shipment to originate from them.
   * 
   * @param array $continents
   *   Array of valid Shipwire warehouse continent code.
   *   See getWarehouseContinents() for list of valid codes.
   * @param bool $reset
   *   TRUE to delete current continents, FALSE to append continents.
   * 
   * @throws Exception
   */
  public function setWarehouseCountinents(array $continents, $reset = FALSE) {
    if ($reset === TRUE) {
      $this->warehouse_continents = array();
    }
    if (empty($continents)) {
      throw new Exception(t('Warehouse continents array cannot be empty.'));
    }
    else {
      if (!is_array($this->warehouse_continents)) {
        $this->warehouse_continents = array();
      }
      foreach ($continents as $continent) {
        $this->addWarehouseContinent($continent);
      }
    }
  }

  /**
   * Add product SKU/model to check it's stock level.
   * 
   * @param string $sku
   *   Unique product identifier used in store and Shipwire.
   * 
   * @throws Exception
   */
  public function addProductSku($sku) {
    if (!empty($sku)) {
      if (!is_array($this->products)) {
        $this->products = array();
      }
      $this->products[] = $sku;
    }
    else {
      throw new Exception(t('Product sku cannot be empty.'));
    }
  }

  public function validate() {
    // @todo Add validation on warehouse properties to ensure results.
  }

}
