<?php

require_once 'CommerceAvangardMethodCallXMLEncoder.php';
require_once 'CommerceAvangardMethodCallXMLDecoder.php';

/**
 * Sends requests to the server encoding request parameters and decoding
 * results with given encoder and decoder. returns error if a timeout occurs
 */
class CommerceAvangardRemoteRequestSender {

  const maxFileReadingStringLength = 4096;

  /**
   * @var string
   *  Well-formed server url to connect with
   */
  private $serverUrl;

  /**
   * @var int
   *  Timeout for server communications in seconds.
   */
  private $timeout;

  /**
   * @param string $server_url
   *  Server url.
   * @param int $timeout
   *  Timeout in seconds.
   */
  public function __construct($server_url, $timeout) {
    $this->serverUrl = $server_url;
    $this->timeout = $timeout;
  }

  /**
   * @param string $url_suffix
   * @param CommerceAvangardMethodCallXMLEncoder $params_encoder
   * @param CommerceAvangardMethodCallXMLDecoder $results_decoder
   * 
   * @return array
   *  Resulted data structure representing decoded xml or containing error information.
   */
  private function sendRequest($url_suffix, CommerceAvangardMethodCallXMLEncoder $params_encoder, CommerceAvangardMethodCallXMLDecoder $results_decoder) {
    try {
      $request_xml = $params_encoder->encode();
      $result = $this->doPostRequest($url_suffix, $request_xml);

      return $this->decodeResult($results_decoder, $result);
    }
    catch (Exception $exception) {
      return array('error' => $exception->getMessage());
    }
  }

  /**
   * @param string $url_suffix
   * @param string $method_name
   * @param array $parameters
   * 
   * @return array call result
   */
  public final function callMethod($url_suffix, $method_name, array $parameters = array()) {
    $encoder = new CommerceAvangardMethodCallXMLEncoder($method_name, $parameters);
    $decoder = new CommerceAvangardMethodCallXMLDecoder($method_name);
    return $this->sendRequest($url_suffix, $encoder, $decoder);
  }

  /**
   * @param CommerceAvangardMethodCallXMLDecoder $results_decoder
   * @param string $result
   *
   * @return array
   * 
   * @throws Exception
   *  When decoder fails to decode results.
   */
  protected function decodeResult(CommerceAvangardMethodCallXMLDecoder $results_decoder, $result) {
    $data = $results_decoder->decode($result);
    if (!$data) {
      throw new Exception(t('Unable to decode results'));
    }

    return $data;
  }

  /**
   * @param int $communication_start_time
   *
   * @return bool
   */
  private function isTimedOut($communication_start_time) {
    return (time() - $communication_start_time) >= $this->timeout;
  }

  /**
   * Sends data compressed by deflate algorithm to the server.
   * 
   * @param string $url_suffix
   *  Url suffix.
   * @param string $data
   *  Xml to be send to server.
   *
   * @return string
   *  Xml response.
   * 
   * @throws Exception
   *  If something went wrong when communication with server.
   */
  protected function doPostRequest($url_suffix, $data) {
    $params = array(
      'http' => array(
        'method' => 'POST',
        'content' => "xml={$data}",
        'timeout' => $this->timeout,
        'header' => 'Content-type: application/x-www-form-urlencoded;charset="utf-8"',
      ),
      'ssl' => array(
        'verify_peer' => FALSE,
        'verify_host' => 1,
      ),
    );
    $context = stream_context_create($params);

    $communication_start_time = time();
    $fp = fopen("{$this->serverUrl}{$url_suffix}", 'rb', NULL, $context);
    if (!$fp) {
      $this->throwConnectionException($communication_start_time, t("Problem occurred while connecting to bank's server"));
    }
    stream_set_blocking($fp, FALSE);

    $response = $this->readResponse($fp);
    fclose($fp);

    if (empty($response)) {
      $this->throwConnectionException($communication_start_time, t("Problem occurred while reading data from bank's server."));
    }

    return $response;
  }

  /**
   * @param int $communication_start_time
   * @param string $message
   * 
   * @throws Exception
   */
  private function throwConnectionException($communication_start_time, $message) {
    $timeout_msg = $this->isTimedOut($communication_start_time) ? t('Bank server response time exceeded (@timeout)', array('@timeout' => $this->timeout)) : '';
    throw new Exception($message . ' ' . $timeout_msg);
  }

  /**
   * @param mixed $fp
   *  File pointer.
   * @return string
   *  Read string.
   */
  private function readResponse($fp) {
    $response = '';

    while (!feof($fp)) {
      $response .= fgets($fp, self::maxFileReadingStringLength);
    }

    return $response;
  }

}
