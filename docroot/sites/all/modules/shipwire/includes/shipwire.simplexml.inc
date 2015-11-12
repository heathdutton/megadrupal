<?php

/**
 * @file
 * Extended SimpleXMLElement for use in generating XML for Shipwire API.
 */

/**
 * Add functions to encapsulate text in CDATA tags or escape text for safe
 * transmission via XML.
 */
class ShipwireSimpleXMLElement extends SimpleXMLElement {

  /**
   * Add text value wrapped in CDATA tag.
   * 
   * @param string $value
   *   The text to wrap in CDATA tag.
   */
  private function addCData($value) {
    $node = dom_import_simplexml($this);
    $node_own = $node->ownerDocument;
    $node->appendChild($node_own->createCDATASection($value));
  }

  /**
   * Add an child element to XML wrapped in CDATA tag.
   * 
   * @param string $name
   *   The XML element name.
   * @param string $value
   *   The XML element value.
   * 
   * @return object
   *   The XML element object added.
   */
  public function addChildCData($name, $value) {
    $child = $this->addChild($name);
    $child->addCData(trim($value));
    return $child;
  }

  /**
   * Add XML element child with escaped value safe for transmission.
   * 
   * @param string $name
   *   The XML element name.
   * @param string $value
   *   The XML element value.
   * 
   * @return object
   *   The XML element object added.
   */
  public function addChildEsc($name, $value) {
    return $this->addChild($name, check_plain(trim($value)));
  }

  /**
   * Add account information to XML request.
   *  
   * @param array $info
   *   Account info to add.
   * @param string $method
   *   The API request method being used.
   */
  public function addLogin($info, $method) {
    $this->addChildCData('Username', $info['user']);
    $this->addChildCData('Password', $info['pass']);
    $this->addChild('Server', $info['server']);
    if ($method == 'fulfill') {
      $this->addChild('AffiliateId', $info['app_id']);
    }
  }

}
