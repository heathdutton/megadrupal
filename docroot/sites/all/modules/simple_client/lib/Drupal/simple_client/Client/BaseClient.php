<?php
/**
 * @file
 * Provides the base Client.
 *
 * @copyright Copyright(c) 2012 Previous Next Pty Ltd
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at previousnext dot com dot au
 */

namespace Drupal\simple_client\Client;

use Guzzle\Http\Client;
use Guzzle\Http\Exception\BadResponseException;
use Guzzle\Http\Message\EntityEnclosingRequest;
use Guzzle\Http\Message\Response;

/**
 * Class BaseClient
 *
 * @package Drupal\simple_client\Client
 */
abstract class BaseClient {

  /**
   * Store the client we use to make the request.
   *
   * @var \Guzzle\Http\ClientInterface
   */
  protected $client;

  /**
   * The request.
   *
   * @var \Guzzle\Http\Message\RequestInterface|EntityEnclosingRequest;
   */
  protected $request;

  /**
   * Options for the request.
   *
   * @var array
   */
  protected $requestOptions = array();

  /**
   * Store the last response from the client
   *
   * @var Response
   */
  protected $response;

  /**
   * The decoded result from the response.
   */
  protected $result;

  /**
   * Stores the user session used in this request.
   *
   * @var object
   */
  protected $session;

  /**
   * Stores a cookie value to retain login for authenticated sessions.
   *
   * @var string
   */
  protected $cookieSession;

  /**
   * Track whether we should catch all Exceptions.
   *
   * @var bool
   */
  protected $noExceptions;

  /**
   * Constructor for a SimpleClient.
   *
   * @param string|null $url
   *   A URL.
   * @param array $options
   *   (optional) An array of settings information.
   */
  public function __construct($url = NULL, $options = array()) {

    $this->noExceptions();

    $this->extractOptions($options);

    $this->setUrl($url);

    // Set a default response.
    $this->result = FALSE;
  }

  /**
   * Fetch a remote resource.
   *
   * This takes two arguments, a URL to GET, and an array of options, which
   * might include things like the 'user' and 'pass' to set in the request. Both
   * arguments are optional, as they can be set in the constructor as well.
   *
   * @param string|null $url
   *   The URL to GET.
   * @param array $options
   *   An array of optional parameters. Any options set here will overwrite
   *   individual options set previously, for example when creating the client.
   *
   * @return mixed
   *   The result of the GET request.
   */
  public function get($url = NULL, $options = array()) {

    $this->setUrl($url);

    $this->extractOptions($options);

    // Ensure that the request method is GET, no matter what is set in the
    // $options array.
    $this->setRequestOption('http_method', 'GET');

    $this->prepareRequest('get');
    $this->doRequest();

    return $this->getResult();
  }

  /**
   * Post to a remote resource.
   *
   * @param string|null $url
   *   The URL to post to.
   * @param array $data
   *   An array of data to post.
   * @param array $options
   *   An array of optional parameters.
   *
   * @return mixed
   *   The result of the pull request.
   */
  public function post($url = NULL, $data = array(), $options = array()) {

    $this->setUrl($url);

    $this->extractOptions($options);

    $this->setRequestOption('http_method', 'POST');
    $this->setRequestOption('post_body', $data);

    $this->prepareRequest('post');

    $this->doRequest();

    return $this->getResult();
  }

  /**
   * Invoke a request object to work with.
   *
   * @param string $type
   *   The type of request. Valid values are get, post, put and delete.
   *
   * @return bool|mixed
   *   The result of the request.
   */
  abstract protected function prepareRequest($type);

  /**
   * Process a request.
   *
   * This should make the actual request, set the Response using
   * SimpleClientInterface::setResponse(), and the result in $this->result.
   */
  abstract protected function doRequest();

  /**
   * Return a request result.
   *
   * @return mixed
   *   Result of the request, or FALSE.
   */
  public function getResult() {

    if (isset($this->result)) {
      return $this->result;
    }
  }

  /**
   * Set the request URL.
   *
   * @param string $url
   *   A url.
   */
  public function setUrl($url) {

    if (!empty($url)) {
      $this->setRequestOption('url', $url);
    }
  }

  /**
   * Set a request option.
   *
   * @param string $key
   *   The key to use.
   * @param mixed $value
   *   The value.
   * @param bool $append
   *   (optional) If the current value is set and is an array, if this is set
   *   to TRUE then the value will be appended, rather than overwriting the
   *   existing contents.
   */
  public function setRequestOption($key, $value, $append = FALSE) {

    if ($append == TRUE) {
      if (isset($this->requestOptions[$key]) && is_array($this->requestOptions[$key])) {
        $this->requestOptions[$key][] = $value;
      }
      else {
        $this->requestOptions[$key] = array();
        $this->requestOptions[$key][] = $value;
      }
    }
    else {
      $this->requestOptions[$key] = $value;
    }
  }

  /**
   * Catch all exceptions during processing.
   *
   * This is the default. An implementer may call SimpleClient::throwExceptions
   * and catch its own errors.
   *
   * Not that this only controls Exceptions which SimpleClient knows about. It's
   * possible that other Exceptions may cause errors.
   */
  public function noExceptions() {

    $this->noExceptions = TRUE;
  }

  /**
   * Throw exceptions during processing.
   *
   * Not that this only controls Exceptions which SimpleClient knows about. It's
   * possible that other Exceptions may cause errors.
   */
  public function throwExceptions() {

    $this->noExceptions = FALSE;
  }

  /**
   * Get the value for No Exceptions.
   *
   * @return boolean
   *   TRUE, if no exceptions should be thrown.
   */
  public function getNoExceptions() {

    return $this->noExceptions;
  }

  /**
   * Get the session object.
   *
   * @return object
   *   An object.
   */
  public function getSession() {

    return $this->session;
  }

  /**
   * Set the session object.
   *
   * @param object $session
   *   An object with session data.
   */
  public function setSession($session) {

    $this->session = $session;
  }

  /**
   * Get the Request.
   *
   * @return \Guzzle\Http\Message\RequestInterface
   *   A request object
   */
  public function getRequest() {

    return $this->request;
  }

  /**
   * Set the Request.
   *
   * @param \Guzzle\Http\Message\RequestInterface $request
   *   A request object.
   */
  public function setRequest($request) {

    $this->request = $request;
  }

  /**
   * Get the cookie session value.
   *
   * @return string
   *   A cookie value.
   */
  public function getCookieSession() {

    return $this->cookieSession;
  }

  /**
   * Set the cookie session value.
   *
   * @param string $cookie_session
   *   A cookie value.
   */
  public function setCookieSession($cookie_session) {

    $this->cookieSession = $cookie_session;
  }

  /**
   * Get the Response value.
   *
   * @return Response
   *   A Guzzle response object, or FALSE.
   */
  public function getResponse() {

    if (isset($this->response)) {

      return $this->response;
    }

    return FALSE;
  }

  /**
   * Set the Response value.
   *
   * @param Response $response
   *   A valid Guzzle response object.
   */
  public function setResponse($response) {

    $this->response = $response;
  }

  /**
   * Retrieve a request option.
   *
   * @param string $key
   *   The key to request.
   * @param mixed $default
   *   (optional) A default value to use.
   *
   * @return mixed|null
   *   The value of the requested key, or a default value.
   */
  public function getRequestOption($key, $default = NULL) {

    if (isset($this->requestOptions[$key])) {
      return $this->requestOptions[$key];
    }

    return $default;
  }

  /**
   * Get the set URL.
   *
   * @return bool|string
   *   The URL, or FALSE.
   */
  public function getUrl() {

    $full_url = $this->getRequestOption('url');
    if (isset($full_url)) {
      return $full_url;
    }
    else {
      $url = $this->prepareUrl();
      if (!empty($url)) {
        return $url;
      }
    }

    return FALSE;
  }

  /**
   * Get header information.
   *
   * @return array
   *   An array of headers
   */
  protected function getHeaders() {

    return $this->getRequestOption('headers', array());
  }

  /**
   * Get the result of the request.
   *
   * This should be overridden to decode the request as required.
   *
   * @param mixed $result
   *   The result of the request.
   *
   * @return mixed
   *   The contents of the request.
   */
  protected function decodeResponse($result) {

    return $result;
  }

  /**
   * Prepare the URL from settings.
   *
   * @return string
   *   A URL path.
   */
  protected function prepareUrl() {

    $endpoint = '';

    // Add the target path.
    $url = $this->getRequestOption('base_url');
    if (!empty($url)) {
      $endpoint = $url;
    }

    // Add the target path.
    $path = $this->getRequestOption('path');
    if (!empty($path)) {
      $endpoint .= $path;
    }

    // Respond to $options['request args'].
    // @todo this should handle query strings better
    $args = $this->getRequestOption('request args', array());
    if (!empty($args)) {
      $endpoint = $endpoint . '/' . $args;
    }

    return $endpoint;
  }

  /**
   * Extract an array of options into our request options variable.
   *
   * @param array $options
   *   An array of option information.
   */
  protected function extractOptions($options = NULL) {

    if (!empty($options) && is_array($options)) {
      foreach ($options as $key => $option) {
        $this->setRequestOption($key, $option);
      }
    }
  }

  /**
   * Check if authentication is actually required.
   *
   * @return bool
   *   TRUE if the settings say we should authenticate, or FALSE.
   */
  protected function shouldAuthenticate() {

    return $this->getRequestOption('authenticate', FALSE);
  }

  /**
   * Log an error.
   *
   * @param string|null $message
   *   (optional) A message.
   * @param array $args
   *   (optional) Placeholder arguments for the $message value.
   *
   * @internal param $type
   */
  protected function logError($message = NULL, $args = array()) {

    watchdog('simple_client', $message, $args, WATCHDOG_ERROR);
  }

  /**
   * Log an HTTP error.
   *
   * This is just an advanced form of logError to deal with status codes.
   *
   * @param string|int $status
   *   An HTTP status code, or 0.
   * @param string $message
   *   Some descriptive text to help the user work out what happened.
   */
  protected function logHttpError($status, $message = '') {

    // Build an error message for logging.
    if (!empty($message)) {
      $error = $status . ' (' . $message . ')';
    }
    else {
      $error = $status;
    }

    // Log.
    $message = "Request failed with error: %error.";
    $args = array(
      '%error' => $error,
    );

    $this->logError($message, $args);
  }

  /**
   * Log an Exception.
   *
   * @param \Exception $e
   *   The exception.
   * @param string|null $message
   *   (optional) A message.
   * @param array $args
   *   (optional) Placeholder arguments for the $message value.
   *
   * @internal param $type
   */
  protected function logException($e, $message = NULL, $args = array()) {

    watchdog_exception('simple_client', $e, $message, $args, WATCHDOG_ERROR);

    $this->throwConditionalException($e);
  }

  /**
   * Call a Guzzle Exception HTTP error.
   *
   * Wraps $this::logHttpError, with handlers for Guzzle exceptions.
   *
   * @param \Guzzle\Http\Exception\BadResponseException $e
   *   The Guzzle HTTP error.
   */
  protected function logHttpExceptionError(BadResponseException $e) {

    // Get the response and build a status message so we know what happened.
    $response = $e->getResponse();
    if (is_object($response) && $response instanceof Response) {
      $message = '%error. Error code: %code';
      $args = array(
        '%error' => $response->getReasonPhrase(),
        '%code' => $response->getStatusCode(),
      );
    }
    else {
      $message = 'Unknown error (this may mean the endpoint is missing or not responding)  ';
      $args = array();
    }

    watchdog_exception('simple_client', $e, $message, $args, WATCHDOG_ERROR);

    $this->throwConditionalHttpException($e);
  }

  /**
   * Throw an Exception.
   *
   * @param \Exception $e
   *   The exception to throw.
   *
   * @throws SimpleClientException|\Exception
   */
  protected function throwConditionalException(\Exception $e) {

    if ($this->getNoExceptions() == FALSE) {

      if (!$e instanceof SimpleClientException) {
        throw new SimpleClientException($e->getMessage(), 0, $e);
      }

      throw $e;
    }
  }

  /**
   * Throw a conditional Exception for an HTTP request.
   *
   * @param \Guzzle\Http\Exception\BadResponseException $e
   *   The Exception.
   */
  protected function throwConditionalHttpException(BadResponseException $e) {

    $response = $e->getResponse();
    if (is_object($response) && $response instanceof Response) {

      if ($this->getNoExceptions() == FALSE) {
        $exception = new SimpleClientException($e->getMessage(), 0, $e);
        $exception->setHttpCode($response->getStatusCode());
        $exception->setUrl($this->getUrl());

        $message = '%error. Error code: %code';
        $args = array(
          '%error' => $response->getReasonPhrase(),
          '%code' => $response->getStatusCode(),
        );

        watchdog_exception('simple_client', $e, $message, $args, WATCHDOG_ERROR);

        $this->throwConditionalException($exception);
      }
    }
    else {
      $message = 'Unknown error thrown in throwConditionalHttpException()';
      $args = array();

      watchdog_exception('simple_client', $e, $message, $args, WATCHDOG_ERROR);

      $this->throwConditionalException($e);
    }

  }
}
