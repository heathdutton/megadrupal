<?php
/**
 * @file
 * Basic response object which can be extended.
 */

class AdlibBaseResponse {

  /*
   * Todo items here.
   */
  // Members
  /**
   * Boolean that indicates if an error occured
   */
  protected $error;

  /**
   * String for errormessage
   */
  protected $errorString;

  /**
   * HTTP response
   */
  protected $httpStatus;
  protected $httpStatusMessage;
  protected $headers;

  /**
   * The raw response
   */
  protected $raw;

  /**
   * Contructor.
   *
   * @param string $response_with_header
   *   the raw response we get from the adlib server
   * @param array $http_info
   *   the HTTP headers created when the request was made
   */
  public function __construct($response_with_header, $http_info = array()) {
    $this->error = FALSE;
    $this->httpStatus = -1;
    $this->httpStatusMessage = 'No request';
    // If $responseWithHeader is 'false' the request failed completely.
    if (!$response_with_header) {
      $this->error = TRUE;
      $this->errorString = t('No response from adlibserver. Probably the address is wrong.');
    }
    else {
      $this->parse($response_with_header);
    }

    if (is_array($http_info) && count($http_info) > 0) {
      // If we received any other status then 200 OK,
      // we have an error and we assume no valid XML was returned.
      if ($http_info['http_code'] != 200) {
        $this->error = TRUE;
      }
    }
    else {
      // No $httpHeaders, error = true.
      $this->error = TRUE;
    }
  }

  /**
   * Parse the response with headers.
   *
   * @param string $response_with_header
   *   Response from curl including the header.
   */
  protected function parse($response_with_header) {
    // Extract headers from response.
    $pattern = '#HTTP/\d\.\d.*?$.*?\r\n\r\n#ims';
    preg_match_all($pattern, $response_with_header, $matches);
    $headers = explode("\r\n", str_replace("\r\n\r\n", '', array_pop($matches[0])));

    // Extract the version and status from the first header.
    $version_and_status = array_shift($headers);
    preg_match('#HTTP/(\d\.\d)\s(\d\d\d)\s(.*)#', $version_and_status, $matches);
    $this->headers['Http-Version'] = $matches[1];
    $this->headers['Status-Code'] = $matches[2];
    $this->headers['Status-Message'] = $matches[2] . ' ' . $matches[3];
    $this->headers['Error-Message'] = ($this->headers['Status-Code'] != 200) ? $matches[3] : '';

    $this->httpStatus = $this->headers['Status-Code'];
    $this->httpStatusMessage = $this->headers['Status-Message'];

    // Convert headers into an associative array.
    foreach ($headers as $header) {
      preg_match('#(.*?)\:\s(.*)#', $header, $matches);
      $this->headers[$matches[1]] = $matches[2];
    }

    // Remove the headers from the response body.
    $this->raw = preg_replace($pattern, '', $response_with_header);
  }


  /**
   * Get the error.
   *
   * @return bool
   *   Indicating is there was an error.
   */
  public function getError() {
    return $this->error;
  }

  /**
   * Get the errorMessage.
   *
   * @return string
   *   String containing errormessage.
   */
  public function getErrorMessage() {
    return $this->errorString;
  }

  /**
   * Get the HTTP status.
   *
   * @return string
   *   Httpstatus.
   */
  public function getHTTPStatus() {
    return $this->httpStatus;
  }

  /**
   * Get the HTTP status message.
   *
   * @return string
   *   The HTTP status message.
   */
  public function getHTTPStatusMessage() {
    return $this->httpStatusMessage;

  }

  /**
   * Get the raw data the requerst returned.
   *
   * @return string
   *   The raw data
   */
  public function getRaw() {
    return $this->raw;
  }
}
