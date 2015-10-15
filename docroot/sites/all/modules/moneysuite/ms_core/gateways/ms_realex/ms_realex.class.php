<?php

/**
 * @file
 * Class definition for RealexRemote.
 */
class RealexRemote
{
  // Initialise arrays
  var $parser;
  var $record;
  var $timestamp;
  var $current_field = '';
  var $field_type;
  var $ends_record;
  var $records;
  var $answers;

  function RealexRemote($url, $xml) {
    $ch = curl_init();
    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_POST, 1);
    curl_setopt($ch, CURLOPT_USERAGENT, 'payandshop.com php version 0.9');
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($ch, CURLOPT_POSTFIELDS, $xml);
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);
    $response = curl_exec($ch);
    curl_close($ch);

    $this->parseXML($response);
  }

  function parseXML($response) {
    // Clean up the response.
    $response = preg_replace('/[[:space:]]+/i', ' ', $response);
    $response = preg_replace("/[\n\r]/i", '', $response);

    // Create and initialise XML parser.
    $this->parser = xml_parser_create();
    xml_set_object($this->parser, $this);
    xml_set_element_handler($this->parser, 'startElement', 'endElement');
    xml_set_character_data_handler($this->parser, 'cDataHandler');

    // 1 = single field, 2 = array field, 3 = record container.
    $this->field_type = array(
      'response' => 1,
      'orderid' => 1,
      'authcode' => 1,
      'result' => 2,
      'message' => 1,
      'pasref' => 1,
      'batchid' => 1,
      'md5hash' => 1,
      'sha1hash' => 1,
      'cvnresult' => 1,
      'dccinfo' => 2,
      'cardholdercurrency' => 1,
      'cardholderamount' => 1,
      'cardholderrate' => 1,
      'merchantcurrency' => 1,
      'merchantamount' => 1,
    );

    xml_parse($this->parser, $response);
    xml_parser_free($this->parser);
  }

  /**
   * Creates a variable on the fly contructed of all the parent elements joined together with an underscore.
   *
   * Called when an open element tag is found.
   *
   * So the following xml:
   * <response><something>Owen</something></response>
   *
   * would create two variables:
   *
   * $RESPONSE and $RESPONSE_SOMETHING
   */
  function startElement($parser, $element, &$attrs) {
    if ($element = strtolower($element)) {
      if (!isset($this->field_type[$element])) {
        $this->field_type[$element] = 1;
      }
      if ($this->field_type[$element] != 0) {
        $this->current_field = $element;
      } else {
        $this->current_field = '';
      }
      if ($element == 'response' && $attrs['TIMESTAMP']) {
        $this->record['timestamp'] = $attrs['TIMESTAMP'];
      }
    }
  }

  function endElement($parser, $element) {
    $this->current_field = '';
  }

  /**
   * Called when the parser encounters any text that's not an element.
   *
   * Simply places the text found in the variable that was last created. So
   * using the XML example above the text 'Owen' would be placed in the
   * variable $RESPONSE_SOMETHING
   */
  function cDataHandler($parser, $text) {
    if ($field = $this->current_field) {
      if (!isset($this->field_type[$field])) {
        $this->field_type[$field] = 1;
      }
      if ($this->field_type[$field] == 2) {
        $this->record[$field][] = $text;
      } elseif ($this->field_type[$field] === 1) {
        if (empty($this->record[$field])) {
          $this->record[$field] = "";
        }
        $this->record[$field] .= $text;
      }
    }
  }

}