<?php

/**
 * @file
 * Defines basic OOP containers for NPRML.
 */

/**
 * Defines a class for NPRML creation/transmission and retreival/parsing, for any PHP-based system.
 */
class NPRAPI {

  // HTTP status code = OK
  const NPRAPI_STATUS_OK = 200;

  // Default URL for pulling stories
  const NPRAPI_PULL_URL = 'http://api.npr.org';

  // NPRML CONSTANTS
  const NPRML_DATA = '<?xml version="1.0" encoding="UTF-8"?><nprml></nprml>';
  const NPRML_NAMESPACE = 'xmlns:nprml=http://api.npr.org/nprml';
  const NPRML_VERSION = '0.92.2';

  /**
   * Initializes an NPRML object.
   */
  function __construct() {
    $this->request = new stdClass;
    $this->request->method = NULL;
    $this->request->params = NULL;
    $this->request->data = NULL;
    $this->request->path = NULL;
    $this->request->base = NULL;


    $this->response = new stdClass;
    $this->response->code = NULL;
  }

  function request() {

  }

  function prepare_request() {

  }

  function send_request() {

  }

  function parse_response() {
    $xml = simplexml_load_string($this->response->data);
    if (!empty($xml->list->story)) {
      $id = $this->get_attribute($xml->list->story, 'id');
    }
    $this->response->id = !empty($id) ? $id : NULL;
  }

  function flatten() {

  }

  function create_NPRML($node) {

  }

  /**
   * Parses object. Turns raw XML(NPRML) into various object properties.
   */
  function parse() {
    if (!empty($this->xml)) {
      $xml = $this->xml;
    }

    else {
      $this->notices[] = 'No XML to parse.';
      return;
    }

    $object = simplexml_load_string($xml);
    $this->add_simplexml_attributes($object, $this);

    if (!empty($object->message)) {
      $this->message->id = $this->get_attribute($object->message, 'id');
      $this->message->level = $this->get_attribute($object->message, 'level');
    }

    if (!empty($object->list->story)) {
      foreach ($object->list->story as $story) {
        $parsed = new NPRMLEntity();
        $this->add_simplexml_attributes($story, $parsed);

        //Iterate trough the XML document and list all the children
        $xml_iterator = new SimpleXMLIterator($story->asXML());
        $key = NULL;
        $current = NULL;
        for($xml_iterator->rewind(); $xml_iterator->valid(); $xml_iterator->next()) {
          $current = $xml_iterator->current();
          $key = $xml_iterator->key();

          if ($key == 'image' || $key == 'audio' || $key == 'link') {
            // images
            if ($key == 'image') {
              $parsed->{$key}[] = $this->parse_simplexml_element($current);
            }

            // audio
            if ($key == 'audio') {
              $parsed->{$key}[] = $this->parse_simplexml_element($current);
            }

            // links
            if ($key == 'link') {
              $type = $this->get_attribute($current, 'type');
              $parsed->{$key}[$type] = $this->parse_simplexml_element($current);
            }
          }
          else {
            if (empty($parsed->{$key})){
              // The $key wasn't parsed already, so just add the current element.
              $parsed->{$key} = $this->parse_simplexml_element($current);
            }
            else {
              // If $parsed->$key exists and it's not an array, create an array
              // out of the existing element
              if (!is_array($parsed->{$key})){
                $parsed->{$key} = array($parsed->{$key});
              }
              // Add the new child
              $parsed->{$key}[] = $this->parse_simplexml_element($current);
            }
          }
        }
        $body ='';
        if (!empty($parsed->textWithHtml->paragraphs)) {
          foreach ($parsed->textWithHtml->paragraphs as $paragraph) {
            $body = $body . $paragraph->value . "\n\n";
          }
        }
        $parsed->body = $body;
        $this->stories[] = $parsed;
      }
    }
  }

  /**
   * Converts SimpleXML element into NPRMLElement.
   *
   * @param object $element
   *   A SimpleXML element.
   *
   * @return object
   *   An NPRML element.
   */
  function parse_simplexml_element($element) {
    $NPRMLElement = new NPRMLElement();
    $this->add_simplexml_attributes($element, $NPRMLElement);
    if (count($element->children())) { // works for PHP5.2
      foreach ($element->children() as $i => $child) {
        if ($i == 'paragraph' || $i == 'mp3') {
          if ($i == 'paragraph') {
            $paragraph = $this->parse_simplexml_element($child);
            $NPRMLElement->paragraphs[$paragraph->num] = $paragraph;
          }
          if ($i == 'mp3') {
            $mp3 = $this->parse_simplexml_element($child);
            $NPRMLElement->mp3[$mp3->type] = $mp3;
          }
        }
        else {
          // If $i wasn't parsed already, so just add the current element.
          if (empty($NPRMLElement->$i)){
            $NPRMLElement->$i = $this->parse_simplexml_element($child);
          }
          else {
            // If $NPRMLElement->$i exists and it's not an array, create an array
            // out of the existing element
            if (!is_array($NPRMLElement->$i)) {
              $NPRMLElement->$i = array($NPRMLElement->$i);
            }
            // Add the new child.
            $NPRMLElement->{$i}[] = $this->parse_simplexml_element($child);
          }
        }
      }
    }
    else {
      $NPRMLElement->value = (string)$element;
    }
    return $NPRMLElement;
  }

  /**
   * Extracts value of a given attribute from a SimpleXML element.
   *
   * @param object $element
   *   A SimpleXML element.
   *
   * @param string $attribute
   *   The name of an attribute of the element.
   *
   * @return string
   *   The value of the attribute (if it exists in element).
   */
  function get_attribute($element, $attribute) {
    foreach ($element->attributes() as $k => $v) {
      if ($k == $attribute) {
        return (string)$v;
      }
    }
  }

  /**
   * Generates basic report of NPRML object.
   *
   * @return array
   *   Various messages (strings) .
   */
  function report() {
    $msg = array();
    $params = '';
    if (isset($this->request->params)) {
      foreach ($this->request->params as $k => $v) {
        $params .= " [$k => $v]";
      }
      $msg[] =  'Request params were: ' . $params;
    }

    else {
      $msg[] = 'Request had no parameters.';
    }

    if ($this->response->code == self::NPRAPI_STATUS_OK) {
      $msg[] = 'Response code was ' . $this->response->code . '.';
      if (isset($this->stories)) {
        $msg[] = ' Request returned ' . count($this->stories) . ' stories.';
      }
    }
    elseif ($this->response->code != self::NPRAPI_STATUS_OK) {
      $msg[] = 'Return code was ' . $this->response->code . '.';
    }
    else {
      $msg[] = 'No info available.';
    }
    return $msg;
  }

  /**
   * Takes attributes of a SimpleXML element and adds them to an object (as properties).
   *
   * @param object $element
   *   A SimpleXML element.
   *
   * @param object $object
   *   Any PHP object.
   */
  function add_simplexml_attributes($element, $object) {
    if (count($element->attributes())) {
      foreach ($element->attributes() as $attr => $value) {
        $object->$attr = (string)$value;
      }
    }
  }
}

/**
 * Basic OOP container for NPR entity (story, author, etc.).
 */
class NPRMLEntity {

}

/**
 * Basic OOP container for NPRML element.
 */
class NPRMLElement {
  function __toString() {
    return $this->value;
  }
}
