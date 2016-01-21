<?php
/**
 * @file
 * A simple HTTP client wrapping Guzzle.
 *
 * @copyright Copyright(c) 2012 Previous Next Pty Ltd
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at previousnext dot com dot au
 */

namespace Drupal\simple_client\Client;

use \Guzzle\Http\Client;
use \Guzzle\Http\Exception\BadResponseException;
use \Guzzle\Http\Message\Request;
use \Guzzle\Http\Message\EntityEnclosingRequest;
use \Guzzle\Http\Message\Response;

/**
 * Class Client
 *
 * @package Drupal\simple_client\Client
 */
class GuzzleClient
  extends BaseClient
  implements SimpleClientInterface {

  /**
   * Load a Guzzle client to make http requests with.
   *
   * @param array $guzzle_options
   *   (optional) Guzzle settings to pass to the Guzzle Client. This should
   *   correspond to the Guzzle client options documented at
   *   http://guzzlephp.org/tour/http.html.
   *
   * @return \Guzzle\Http\Client
   *   A Guzzle client.
   */
  public function getClient($guzzle_options = array()) {

    // Allow the settings to provide client options for the Guzzle client.
    if (empty($guzzle_options)) {
      $guzzle_options = $this->getRequestOption('client_options', array());
    }

    if (empty($this->client)) {
      $this->client = new Client($this->getUrl(), $guzzle_options);
    }

    return $this->client;
  }

  /**
   * Invoke a request object to work with.
   *
   * Factory function to load a specialised Guzzle wrapper for the request. The
   * request is stored in $this->request.
   *
   * If the request cannot be created, for example if an incorrect Guzzle
   * method is called, this will throw an Exception, which must be handle by the
   * requesting process.
   *
   * @param string $type
   *   The type of request. Valid values are get, post, put and delete.
   */
  public function prepareRequest($type) {

    // Ensure a Guzzle Client is actually loaded.
    $this->getClient();

    // Process authentication, if relevant.
    $this->authenticate();

    // Prepare the endpoint path from set variables.
    $endpoint = $this->prepareUrl();

    // Add any headers.
    $headers = $this->getHeaders();

    // Attempt to create a request.
    try {
      $request = $this->getClient()
        ->createRequest(strtolower($type), $endpoint, $headers);
    }
    catch (\Exception $e) {
      $this->logException($e, 'Unable to create request for type "%method".', array('%method' => $type));

      // If the logException method doesn't throw an exception, we should
      // return FALSE;
      return FALSE;
    }

    // Request succeeded. Assign it.
    $this->setRequest($request);

    if ($type == 'post') {

      // Add post fields.
      $post_body = $this->getRequestOption('post_body', FALSE);
      if (isset($post_body)) {
        if (is_array($post_body)) {
          foreach ($post_body as $post_key => $post_data) {
            $this->getRequest()->addPostFields(array($post_key => $post_data));
          }
        }
        else {
          $this->getRequest()->addPostFields(array('data' => $post_body));
        }
      }
    }

    // Session auth support.
    // Guzzle has cookie support, but Drupal session auth works with just this
    // cookie set, so we're setting it explicitly.
    // @todo: better auth support.
    if ($this->shouldAuthenticate()) {
      $this->getRequest()
        ->getCurlOptions()
        ->set(CURLOPT_COOKIE, $this->getCookieSession());
    }

    // Respond to $options['request params'].
    $request_params = $this->getRequestOption('request params', array());
    if (!empty($request_params)) {
      $query_string = drupal_http_build_query($request_params);
      $this->getRequest()
        ->setUrl($this->request->getUrl() . '?' . $query_string);
    }
  }

  /**
   * Process a request.
   */
  public function doRequest() {

    try {
      // Store the response so we can access it elsewhere.
      $this->setResponse($this->getRequest()->send());

      if ($this->getResponse() != FALSE && $this->getResponse()->isSuccessful()) {

        $this->result = $this->decodeResponse($this->getResponse()->getBody(TRUE));
      }
      else {
        // Probably an invalid request.
        $this->logHttpError($this->getResponse()->getStatusCode(), 'Request failed');
        $this->result = FALSE;
      }
    }
    catch (BadResponseException $e) {
      $this->logHttpExceptionError($e);

      $this->result = FALSE;
    }
  }

  /**
   * Log in a user to work with the remote repository.
   *
   * @todo This really only works for Drupal sites, and should probably be
   *   removed.
   *
   * @return bool
   *   TRUE, if authentication succeeds, or FALSE.
   */
  public function authenticate() {

    $cookie = $this->getCookieSession();

    if ($this->shouldAuthenticate() && empty($cookie)) {
      try {

        $post_data = array(
          'username' => $this->getRequestOption('user'),
          'password' => $this->getRequestOption('pass'),
        );

        // Get Header information.
        $headers = $this->getHeaders();

        // Prepare the request.
        $request = $this->getClient()->post('user/login', $headers, $post_data);

        // Store the response so we can access it elsewhere.
        $this->setResponse($request->send());

        if ($this->getResponse()->isSuccessful()) {
          $session = $this->decodeResponse($this->getResponse()->getBody(TRUE));
          $this->setSession($session);
          $this->setCookieSession($this->getSession()->session_name . '=' . $this->getSession()->sessid);
        }
        else {
          return FALSE;
        }
      } catch (BadResponseException $e) {
        $this->logHttpExceptionError($e);

        return FALSE;
      }
    }

    return TRUE;
  }

}
