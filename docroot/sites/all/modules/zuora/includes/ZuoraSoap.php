<?php


class ZuoraSoap extends Zuora {

  /**
   *
   */
  const ZUORA_SOAP_PRODUCTION = 'https://www.zuora.com/apps/services/a/68.0';

  /**
   *
   */
  const ZUORA_SOAP_SANDBOX = 'https://apisandbox.zuora.com/apps/services/a/68.0';

  protected $wsdl;
  protected $client;
  protected $headers = array();

  /**
   * Singleton class instance.
   *
   * @var ZuoraSoap
   */
  protected static $instance;

  /**
   * Singleton constructor for Zuora API.
   *
   * @return \ZuoraSoap
   */
  public static function instance() {
    if (self::$instance === NULL) {
      self::$instance = new self();
    }

    return self::$instance;
  }

  /**
   * Sets the API credential information.
   *
   * @throws ZuoraException
   *
   */
  public function __construct() {
    parent::__construct();

    $this->wsdl = drupal_get_path('module', 'zuora') . '/includes/zuora.68.0.wsdl';

    $this->client = new SoapClient($this->wsdl, array(
      'soap_version' => SOAP_1_1,
      'trace' => 1,
    ));

    $this->createSession();
  }

  protected function setLocation() {
    $location = ($this->is_sandbox) ? self::ZUORA_SOAP_SANDBOX : self::ZUORA_SOAP_PRODUCTION;
    $this->client->__setLocation($location);
  }

  protected function createSession() {
    $this->setLocation();
    try {
      $result = $this->client->login(array(
        'username' => $this->tenant_user_id,
        'password' => $this->tenant_password,
      ));
    }
    catch (SoapFault $e) {
      throw new ZuoraException('Error authenticating to remote service');
    }

    $this->addHeader(new SoapHeader(
      'http://api.zuora.com/',
      'SessionHeader',
      array(
        'session'=>$result->result->Session
      )
    ));
  }

  public function addHeader(SoapHeader $soap_hader) {
    $this->headers[] = $soap_hader;
  }

  public function query($query) {
    $api_query = array(
      'query' => array(
        'queryString' => $query,
      )
    );

    try {
      $result = $this->client->__soapCall('query', $api_query, null, $this->headers);
    }
    catch (SoapFault $e) {
      throw new ZuoraException('Error executing remote API call: ' . $e->getMessage());
    }

    return $result;
  }
}
