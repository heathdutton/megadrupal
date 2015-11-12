<?php

/**
 * Class ImisWsRequest
 */
class ImisWsRequest implements ImisWsRequestInterface {

  public $method = 'GET';
  public $parser;

  /**
   * @inherit.
   */
  public function __construct() {
  }

  /**
   * @inherit.
   */
  public function query($endpoint, $params = NULL) {

    $host = variable_get('imis_ws_host');

    if (!$host) {
      throw new Exception(t('No iMIS webservice host configured.'));
    }
    $url = "$host/$endpoint";

    // An array of POST variables
//    $data = array(
//      'Username' => $name,
//      'Password' => $pass,
//    );
    // Header for mime type
    $headers = array(
      'Content-Type' => 'application/x-www-form-urlencoded; charset=utf-8',
    );

    // Post to the host
//    $options = array('headers' => $headers, 'method' => 'POST', 'data' => drupal_http_build_query($data));
    $options = array('headers' => $headers, 'method' => $this->method);
    $result = drupal_http_request($url, $options);

    if ($result->code != 200) {
      throw new Exception(t('Error communicating with iMIS webservice.'));
    }

    // If we've got a parser use it, otherwise reture the raw data.
    return ($this->parser) ?$this->parser->parse($result->data) : $result->data;
  }

}

/**
 * Interface ImisWsRequestInterface
 */
interface ImisWsRequestInterface {

  public function __construct();

  /**
   * @param null $params
   * @return mixed
   */
  public function query($endpoint, $params = NULL);

}
