<?php

/**
 * @class
 * Encodes request to bank's API server.
 */
class CommerceAvangardMethodCallXMLEncoder {

  /**
   * @var DOMDocument
   */
  protected $dom;

  /**
   * @var string
   */
  private $methodName;

  /**
   * @var array
   *  Associative array of attributes to be set in method call Params tag .
   */
  protected $parameters;

  /**
   * @param string $method_name
   * @param array $params
   */
  public function __construct($method_name, array $params = array()) {
    $this->methodName = $method_name;
    $this->parameters = $params;
  }

  /**
   * @return string
   */
  public function encode() {
    $envelope = $this->createEnvelope();
    $method_call = $this->dom->createElement($this->methodName);
    $envelope->appendChild($method_call);
    foreach ($this->parameters as $name => $value) {
      $param = $this->dom->createElement($name, $value);
      $method_call->appendChild($param);
    }
    return $this->dom->saveXML();
  }

  /**
   * @return string
   */
  public function getMethodName() {
    return $this->methodName;
  }

  /**
   * @return DOMElement
   */
  protected function createEnvelope() {
    $this->dom = new DOMDocument('1.0', 'UTF-8');
    return $this->dom;
  }

}
