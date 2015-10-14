<?php
namespace devtools;

class Request extends \devtools\Base {
  protected $_endpoint = '';
  protected $_params = array();
  protected $_cert = FALSE;
  protected $_response_class = 'devtools\Response';
  protected $_extra_options = array();

  public $response = FALSE;

  /**
   * Constructor
   */
  public function __construct() {
    if (!function_exists('curl_init')) {
      throw new \Exception(t('CURL library not available.'));
    }
  }

  /*--------------------------------------------------------------------------
      Getters & setters
  --------------------------------------------------------------------------*/
  
  // endpoint
  public function get_endpoint() {
    return $this->_endpoint;
  }

  public function set_endpoint($endpoint) {
    $this->_endpoint = $endpoint;
    return $this->get_endpoint();
  }

  // params
  public function get_params() {
    return $this->_params;
  }

  public function set_params($params) {
    $this->_params = $params;
    return $this->get_params();
  }

  public function add_params($params) {
    $this->_params += $params;
    return $this->get_params();
  }

  // cert
  public function get_cert() {
    return $this->_cert;
  }

  public function set_cert($cert) {
    if (!file_exists($cert)) {
      throw new \Exception(t('Certificate file not found at path: !path', array('!path' => $cert)));
    }

    $this->_cert = $cert;

    return $this->get_cert();
  }

  // response class
  public function get_response_class() {
    return $this->_response_class;
  }

  public function set_response_class($class) {
    $this->_response_class = $class;
    return $this->get_response_class;
  }

  // extra options
  public function get_extra_options() {
    return $this->_extra_options;
  }

  public function set_extra_options($extra_options) {
    $this->_extra_options = $extra_options;
    return $this->get_extra_options();
  }

  /*--------------------------------------------------------------------------
      Request processing
  --------------------------------------------------------------------------*/

  public function curl($verbose = TRUE, $debug = FALSE) {
    // turn parameters into a string of NVPs
    $params = http_build_query($this->get_params());

    // basic options
    $options = array (
       CURLOPT_URL => $this->_endpoint,
       // CURLOPT_VERBOSE => $verbose,
       CURLOPT_RETURNTRANSFER => 1,
       CURLOPT_POST => 1,
       CURLOPT_POSTFIELDS => $params,
       CURLOPT_SSL_VERIFYPEER => 1,
       CURLOPT_SSL_VERIFYHOST => 2,  
    );

    // if we have a certificate, add it
    if ($cert = $this->get_cert()) {
      $options[CURLOPT_CAINFO] = $cert;
    }

    // add extra options
    $options += $this->get_extra_options();

    $handler = curl_init();
    curl_setopt_array($handler, $options);

    $class = $this->get_response_class();
    if (!class_exists($class)) {
      throw new \Exception(t('Response class %class not found.', array('%class' => $class)));
    }

    $this->response = new Response($handler, curl_exec($handler));
    $this->response->display_errors();

    return $this->response;
  }
}