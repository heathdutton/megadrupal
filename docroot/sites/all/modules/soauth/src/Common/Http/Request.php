<?php

namespace Drupal\soauth\Common\Http;


/**
 * Class Request
 * @author Raman Liubimau <raman@cmstuning.net>
 */
class Request {
  
  /**
   * Fully qualified URI
   * @var string
   */
  private $url;
  
  /**
   * Request method
   * @var string
   */
  private $method;
  
  /**
   * Request headers
   * @var array
   */
  private $headers;
  
  /**
   * Request body
   * @var string
   */
  private $data;
  
  /**
   * 
   * @param string $url
   * @param string $method
   */
  public function __construct($url, $method='GET') {
    $this->url = $url;
    $this->method = $method;
    $this->headers = array();
    $this->data = '';
  }
  
  /**
   * Get URL
   * @return string
   */
  public function getUrl() {
    return $this->url;
  }
  
  /**
   * Get method
   * @return string
   */
  public function getMethod() {
    return $this->method;
  }
  
  /**
   * Get headers
   * @return array
   */
  public function getHeaders() {
    return $this->headers;
  }
  
  /**
   * Get query data
   * @return string
   */
  public function getBody() {
    return $this->data;
  }
  
  /**
   * Set URL
   * @param type $url
   */
  public function setUrl($url) {
    $this->url = $url;
    return $this;
  }
  
  /**
   * Set method
   * @param string $method
   * @return Request
   */
  public function setMethod($method) {
    $this->method = $method;
    return $this;
  }
  
  /**
   * Set headers
   * @param string $name
   * @param string $value
   * @return Request
   */
  public function setHeader($name, $value) {
    $this->headers[$name] = $value;
    return $this;
  }
  
  /**
   * Set query data
   * @param string $data
   * @return Request
   */
  public function setBody($data) {
    $this->data = $data;
    return $this;
  }
  
  /**
   * Send request
   * @param boolean $strict
   * @return stdClass
   */
  public function send() {
    return drupal_http_request($this->url, array(
      'headers' => $this->headers,
      'method' => $this->method,
      'data' => $this->data,
    ));
  }
  
  /**
   * Create request
   * @param string $url
   * @param string $method
   * @return Request
   */
  static public function create($url, $method='GET') {
    return new self($url, $method);
  }
}
