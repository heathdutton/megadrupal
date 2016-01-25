<?php
/**
 * @file
 * Contains a custom exception class for SimpleClient
 *
 * @copyright Copyright(c) 2012 Previous Next Pty Ltd
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at previousnext dot com dot au
 */

namespace Drupal\simple_client\Client;

/**
 * Class Exception
 *
 * @package Drupal\simple_client\Client
 */
class SimpleClientException
  extends \Exception {

  protected $httpCode;

  protected $url;

  /**
   * Set the HTTP code.
   *
   * @param string $http_code
   *   A valid HTTP code.
   */
  public function setHttpCode($http_code) {

    $this->httpCode = $http_code;
  }

  /**
   * Get the HTTP code.
   *
   * @return string
   *   An HTTP code.
   */
  public function getHttpCode() {

    return $this->httpCode;
  }

  /**
   * Set the requested URL.
   *
   * @param string $url
   *   A URL.
   */
  public function setUrl($url) {

    $this->url = $url;
  }

  /**
   * Get the requested URL.
   *
   * @return string
   *   A URL
   */
  public function getUrl() {

    return $this->url;
  }


}
