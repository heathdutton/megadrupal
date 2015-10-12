<?php

/**
 * @file
 * Provides the Dreamhost PHP API.
 */

/**
 * The Dreamhost PHP API.
 *
 * @package    Dreamhost
 * @author     Rob Loach (http://www.robloach.net)
 * @version    0.1
 * @access     public
 * @license    GPL (http://www.opensource.org/licenses/gpl-2.0.php)
 */
class Dreamhost {
  public $key = NULL;
  public $api_url = 'https://api.dreamhost.com';

  /**
   * Creates a new interface to the Disqus API.
   *
   * @param $key
   *   (optional) The API key to use.
   * @param $api_url
   *   (optional) The prefix URL to use when calling the Dreamhost API.
   */
  function __construct($key = NULL, $api_url = 'https://api.dreamhost.com') {
    $this->key = $key;
    $this->api_url = $api_url;
  }

  /**
   * Call API functions via $dramhost->somecommand();
   */
  function __call($cmd, array $args = array()) {
    return $this->call($cmd, $args);
  }

  /**
   * Makes a call to a Disqus API method.
   *
   * @return 
   *   The Dreamhost values.
   * @param $cmd
   *   The API method to call.
   * @param $arguments
   *   An associative array of arguments to be passed.
   */
  function call($cmd, array $args = array()) {
    // Construct the arguments.
    if (!isset($args['key'])) {
      $args['key'] = $this->key;
    }
    if (!isset($arguments['cmd'])) {
      $args['cmd'] = $cmd;
    }
    $arguments = 'format=php';
    foreach ($args as $name => $value) {
      $arguments .= '&' . $name . '=' . urlencode($value);
    }

    // Call the Disqus API.
    $ch = curl_init($this->api_url);
    curl_setopt($ch, CURLOPT_POSTFIELDS, $arguments);
    curl_setopt($ch, CURLOPT_POST, 1);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($ch, CURLOPT_TIMEOUT, 5);
    $result = curl_exec($ch);
    if (!$result) {
			$error = curl_error($ch);
			$errno = curl_errno($ch);
			curl_close($ch);
			throw new Exception($error, $errno);
    }
		curl_close($ch);
    $data = unserialize($result);
    if (!$data) {
      throw new Exception('Unserialization error on: '. $data);
    }
    if ($data['result'] === 'error') {
      throw new Exception($data['data']);
    }
    return $data['data'];
  }
}

