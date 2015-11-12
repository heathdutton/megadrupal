<?php
/**
 * @file
 * Defines the Parcel Monkey class.
 */


class CommerceParcelMonkeyAPIConnect {

  public $user_id = '';
  public $api_key = '';
  public $version = '';
  public $url = '';
  public $params = '';

  /**
   * Class construct.
   */
  public function __construct() {
    $this->UserId = variable_get('commerce_parcel_monkey_user_id', '');
    $this->ApiKey = variable_get('commerce_parcel_monkey_api_key', '');
    $this->Version = '2.0';
  }

  /**
   * Connect to web service.
   */
  public function connectToWebService() {
    $connection = curl_init();
    curl_setopt($connection, CURLOPT_URL, $this->url);
    curl_setopt($connection, CURLOPT_POSTFIELDS, $this->params);
    curl_setopt($connection, CURLOPT_HEADER, 0);
    curl_setopt($connection, CURLOPT_RETURNTRANSFER, TRUE);
    curl_setopt($connection, CURLOPT_TIMEOUT, 10);
    curl_setopt($connection, CURLOPT_USERAGENT, "Mozilla/4.0 (compatible;)");
    $response = curl_exec($connection);

    curl_close($connection);

    return $response;
  }
}


/**
 * Connect to getQuote service
 */
class CommerceParcelMonkeyGetQuote extends CommerceParcelMonkeyAPIConnect {
  public $url = 'http://www.parcelmonkey.co.uk/api2/GetQuote';

  /**
   * Set Post parameters.
   */
  public function setPostParams($params) {
    $this->params = $params;
  }
}


/**
 * Connect to createShipment service
 */
class CommerceParcelMonkeyCreateShipment extends CommerceParcelMonkeyAPIConnect {
  public $url = 'http://www.parcelmonkey.co.uk/api2/CreateShipment';

  /**
   * Set Post parameters.
   */
  public function setPostParams($params) {
    $this->params = $params;
  }
}


/**
 * Connect to getShipmentDetails service
 */
class CommerceParcelMonkeyGetShipmentDetails extends CommerceParcelMonkeyAPIConnect {
  public $url = 'http://www.parcelmonkey.co.uk/api2/GetShipmentDetails';

  /**
   * Set Post parameters.
   */
  public function setPostParams($params) {
    $this->params = $params;
  }
}


/**
 * Connect to payForShipment service
 */
class CommerceParcelMonkeyPayForShipment extends CommerceParcelMonkeyAPIConnect {
  public $url = 'http://www.parcelmonkey.co.uk/api2/PayForShipment';

  /**
   * Set Post parameters.
   */
  public function setPostParams($params) {
    $this->params = $params;
  }
}


/**
 * Connect to getLabelUrl service
 */
class CommerceParcelMonkeyGetLabelUrl extends CommerceParcelMonkeyAPIConnect {
  public $url = 'http://www.parcelmonkey.co.uk/api2/GetLabelUrl';

  /**
   * Set Post parameters.
   */
  public function setPostParams($params) {
    $this->params = $params;
  }
}


/**
 * Connect to getTrackingUrl service
 */
class CommerceParcelMonkeyGetTrackingUrl extends CommerceParcelMonkeyAPIConnect {
  public $url = 'http://www.parcelmonkey.co.uk/api2/GetTrackingUrl';

  /**
   * Set Post parameters.
   */
  public function setPostParams($params) {
    $this->params = $params;
  }
}


/**
 * Connect to createCustomsInvoice service
 */
class CommerceParcelMonkeyCreateCustomsInvoice extends CommerceParcelMonkeyAPIConnect {
  public $url = 'http://www.parcelmonkey.co.uk/api2/CreateCustomsInvoice';

  /**
   * Set Post parameters.
   */
  public function setPostParams($params) {
    $this->params = $params;
  }
}
