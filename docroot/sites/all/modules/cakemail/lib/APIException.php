<?php
/**
 * @file
 * The CakeMailAPIException class.
 */

namespace Drupal\cakemail_api;

/**
 * Exception class for the CakeMail API method invocation.
 */
class APIException extends \Exception {
  /**
   * @var array
   *  Method call's arguments.
   */
  protected $post_data;

  /**
   * @var string
   *  Called service's name.
   */
  protected $service;

  /**
   * @var string
   *  Called method's name.
   */
  protected $method;

  /**
   * Create a new CakeMail API exception.
   *
   * @param string $message
   *   The Exception message to throw.
   * @param string $service
   *   Called service's name.
   * @param string $method
   *   Called method's name.
   * @param array $post_data
   *   Method call's arguments.
   * @param int $code
   *   The exception code (optional).
   * @param Exception $previous
   *   The previous exception used for the exception chaining (optional).
   */
  public function __construct($message, $service, $method, array $post_data = array(), $code = 0, Exception $previous = NULL) {
    parent::__construct($message, $code, $previous);
    $this->post_data = $post_data;
    $this->service = $service;
    $this->method = $method;
  }

  /**
   * Returns the called method's name.
   * @return string
   *   The called method's name.
   */
  public function getMethod() {
    return $this->method;
  }

  /**
   * Returns the called service's name.
   *
   * @return string
   *   The called service's name.
   */
  public function getService() {
    return $this->service;
  }

  /**
   * Returns method call's arguments.
   *
   * If a proper JSON encoded error response has been retrieved from the REST
   * endpoint the arguments will be provided as received (and re-sent) by the
   * API. Otherwise, they are provided as received by the exception throwing
   * method.
   *
   * @return array
   *   The method call's arguments.
   */
  public function getPostData() {
    return $this->post_data;
  }
}