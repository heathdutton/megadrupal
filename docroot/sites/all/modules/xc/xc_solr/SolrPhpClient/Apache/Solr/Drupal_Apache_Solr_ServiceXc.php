<?php
require_once 'ServiceXc.php';

class Drupal_Apache_Solr_ServiceXc extends Apache_Solr_ServiceXc {

  private $protocol = 'HTTP/1.1';

  /**
   * Call the /admin/ping servlet, to test the connection to the server.
   *
   * @param $timeout
   *   maximum time to wait for ping in seconds, -1 for unlimited (default 2).
   * @return
   *   (float) seconds taken to ping the server, FALSE if timeout occurs.
   */
  public function ping($timeout = 2) {
    $start = microtime(TRUE);

    if ($timeout <= 0.0) {
      $timeout = -1;
    }
    // Attempt a HEAD request to the solr ping url.
    list($data, $headers) = $this->_makeHttpRequest($this->_pingUrl, 'HEAD', array(), NULL, $timeout);
    $response = new Apache_Solr_ResponseXc($data, $headers);

    if ($response->getHttpStatus() == 200) {
      // Add 0.1 ms to the ping time so we never return 0.0.
      return microtime(TRUE) - $start + 0.0001;
    }
    else {
      return FALSE;
    }
  }

  /**
   * @see Apache_Solr_Service::__construct()
   */
  public function __construct($host = 'localhost', $port = 8180, $path = '/solr/') {
    parent::__construct($host, $port, $path);
  }

  /**
   * Central method for making a get operation against this Solr Server
   *
   * @see Apache_Solr_Service::_sendRawGet()
   */
  protected function _sendRawGet($url, $timeout = FALSE, $verbose = FALSE) {
    if ($verbose) {
      xc_log_info('xc search', 'Search Solr URL: ' . $url);
    }

    list($rawResponse, $headers) = $this->_makeHttpRequest($url, 'GET', array(), '', $timeout);
    $response = new Apache_Solr_ResponseXc($rawResponse, $headers, $this->_createDocuments, $this->_collapseSingleValueArrays);
    $code = (int) $response->getHttpStatus();
    if ($code != 200) {
      $message = $response->getHttpStatusMessage();
      if ($code >= 400 && $code != 403 && $code != 404) {
        // Add details, like Solr's exception message.
        $message .= $response->getRawResponse();
      }
      throw new Exception('"' . $code . '" Status: ' . $message);
    }
    return $response;
  }

  /**
   * Central method for making a post operation against this Solr Server
   *
   * @see Apache_Solr_Service::_sendRawPost()
   */
  protected function _sendRawPost($url, $rawPost, $timeout = FALSE, $contentType = 'text/xml; charset=UTF-8', $type) {
    $request_headers = array('Content-Type' => $contentType);
    list($data, $headers) = $this->_makeHttpRequest($url, 'POST', $request_headers, $rawPost, $timeout);
    $response = new Apache_Solr_ResponseXc($data, $headers, $this->_createDocuments, $this->_collapseSingleValueArrays);
    $code = (int) $response->getHttpStatus();
    if ($code != 200) {
      $message = $response->getHttpStatusMessage();
      if ($code >= 400 && $code != 403 && $code != 404) {
        // Add details, like Solr's exception message.
        $message .= $response->getRawResponse();
      }
      throw new Exception('"' . $code . '" Status: ' . $message . ' (' . $url . ')');
    }
    return $response;
  }

  protected function _makeHttpRequest($url, $method = 'GET', $headers = array(), $data = '', $timeout = FALSE) {
    // Set a response timeout
    if (!$timeout) {
      $timeout = 3600.0;
    }

    //D7
    $options = array(
      'method' => $method,
      'data' => $data,
      'timeout' => 3600.0,
      'headers' => $headers,
    );

    $result = drupal_http_request($url, $options);

    if (!isset($result->code) || $result->code < 0) {
      $result->code = 0;
    }

    if (!isset($result->data)) {
      $result->data = '';
    }

    $headers[] = $this->protocol . ' ' . $result->code . ' ' . $result->status_message;
    if (isset($result->headers)) {
      foreach ($result->headers as $name => $value) {
        $headers[] = "$name: $value";
      }
    }

    $result_data = $result->data;
    unset($result);

    return array($result_data, $headers);
  }
}
