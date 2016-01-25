<?php
/**
 * @file
 * Provides an interface for SimpleClient clients.
 *
 * @copyright Copyright(c) 2012 Previous Next Pty Ltd
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at previousnext dot com dot au
 */

namespace Drupal\simple_client\Client;

/**
 * Class SimpleClientInterface
 * @package Drupal\simple_client\Client
 */
interface SimpleClientInterface {

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
  public function get($url = NULL, $options = array());

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
  public function post($url = NULL, $data = array(), $options = array());

  /**
   * Load a client to make http requests with.
   *
   * @param array $client_options
   *   (optional) Settings to pass to the Client. For Guzzle, this should
   *   correspond to the Guzzle client options documented at
   *   http://guzzlephp.org/tour/http.html.
   *
   * @return \Guzzle\Http\Client
   *   A Guzzle client.
   */
  public function getClient($client_options = array());

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
  public function prepareRequest($type);

  /**
   * Process a request.
   */
  public function doRequest();

  /**
   * Log in a user to work with the remote repository.
   *
   * @return bool
   *   TRUE, if authentication succeeds, or FALSE.
   */
  public function authenticate();

}
