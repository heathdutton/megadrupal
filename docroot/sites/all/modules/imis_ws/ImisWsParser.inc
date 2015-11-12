<?php

/**
 * Class ImisWsXmlParser
 */
class ImisWsXmlParser implements ImisWsParserInterface {

  /**
   * @inherit.
   */
  public function parse($data) {
    $xml = simplexml_load_string($data);
    $json = json_encode($xml);
    return json_decode($json, TRUE);
  }

}

/**
 * Interface ImisWsParserInterface
 */
interface ImisWsParserInterface {

  /**
   * Parse raw data from request into usable, structured data.
   *
   * @param string $data
   *   Raw data from the request.
   *
   * @return array
   *   An array of structured data.
   */
  public function parse($data);

}
