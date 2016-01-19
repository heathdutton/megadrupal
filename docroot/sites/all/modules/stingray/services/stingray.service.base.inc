<?php
/**
 * @file
 *    Contains StingrayServiceBase class implementation.
 */

/**
 * Default implementation of the StingrayServiceBase class.
 */
abstract class StingrayServiceBase {

  /**
   * @var
   */
  protected $server;

  /**
   * @var
   */
  protected $wsdl_path;

  /**
   * @var
   */
  protected $base_path;

  /**
   * Constructs an instance of the Stingray Cache Control facade.
   *
   * Valid parameters are specified in the options array as key/value pairs with the
   * parameter name being the key and the parameter setting being the value
   *
   * @param array $options
   *   An array of parameter options for the Web Service.
   *   These will override the defaults.
   */
  function __construct($options = array()) {
    $this->wsdl_path = variable_get('stingray_wsdl_dir');
    $this->parameters = $options;
    // Base path
    $base_path = variable_get('stingray_base_path', gethostname());
    $this->base_path = parse_url($base_path);
  }

  /**
   * Request options.
   *
   * @param array $additional_options
   *
   * @return array
   */
  protected function getRequestOptions($additional_options = array()) {
    $server = $this->getServer();
    return array(
      'login' => $server->getUsername(),
      'password' => $server->getPassword(),
      'stream_context' => $server->getContext(),
      'location' => $server->getUrl(),
    ) + $additional_options;
  }

  /**
   * @return null
   */
  protected function getWsdlPath() {
    return $this->wsdl_path;
  }

  /**
   * @param StingrayServer $server
   */
  protected function setServer(StingrayServer $server) {
    $this->server = $server;
  }

  /**
   * @return StingrayServer
   */
  protected function getServer() {
    if (!isset($this->server)) {
      $this->server = new StingrayServer(variable_get('stingray_server_url'), variable_get('stingray_server_username'), variable_get('stingray_server_password'));
    }
    return $this->server;
  }

  /**
   * @return mixed
   */
  protected function getBasePath() {
    return $this->base_path;
  }

}


class StingrayServer {
  /**
   * @var
   */
  protected $url;
  /**
   * @var
   */
  protected $username;
  /**
   * @var
   */
  protected $password;
  /**
   * @var
   */
  protected $context;

  public function __construct($url, $username, $password) {
    // @todo: Make it configurable, if necessary.
    $context = stream_context_create(array(
      'ssl' => array(
        'verify_peer' => false,
        'allow_self_signed' => true
      )
    ));
    $this->url = $url;
    $this->username = $username;
    $this->password = $password;
    $this->context = $context;
  }

  /**
   * @return mixed
   */
  public function getPassword() {
    return $this->password;
  }

  /**
   * @param mixed $password
   */
  public function setPassword($password) {
    $this->password = $password;
  }

  /**
   * @return mixed
   */
  public function getUrl() {
    return $this->url;
  }

  /**
   * @param mixed $url
   */
  public function setUrl($url) {
    $this->url = $url;
  }

  /**
   * @return mixed
   */
  public function getUsername() {
    return $this->username;
  }

  /**
   * @param mixed $username
   */
  public function setUsername($username) {
    $this->username = $username;
  }

  /**
   * @return mixed
   */
  public function getContext() {
    return $this->context;
  }

  /**
   * @param mixed $context
   */
  public function setContext($context) {
    $this->context = $context;
  }

}