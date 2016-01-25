<?php

/**
 * @class
 * Parses and decodes server's response.
 */
class CommerceAvangardMethodCallXMLDecoder {

  /**
   * Decodes xml string and creates array data structure.
   *
   * @param string $xml_string
   *   Xml string to decode.
   *
   * @return array
   *   Decoded data structure.
   */
  public function decode($xml_string) {
    $xml = $this->parseXmlString($xml_string);
    return json_decode(json_encode($xml), TRUE);
  }

  /**
   * Parses xml string and creates xml data structure.
   *
   * @param string $xml_string
   *   Xml string to parse.
   *
   * @return SimpleXMLElement
   *   Xml structure on success NULL otherwise.
   */
  protected final function parseXmlString($xml_string) {
    libxml_use_internal_errors();
    $xml = new SimpleXMLElement($xml_string);

    $errors = libxml_get_errors();
    if (!empty($errors)) {
      $messages = array();
      foreach ($errors as $error) {
        $messages[] = $error->message;
      }
      libxml_clear_errors();
      throw new Exception('Error parsing xml data : ' . implode('\r\n ', $messages));
    }

    return $xml;
  }

}
