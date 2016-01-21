<?php

/**
 * @file
 * Trello API code.
 *
 * This file contains the code that interacts with the API.
 */

/**
 * Call the trello api.
 *
 * @param string $request
 *   path, or array of key/value for request
 * @param string|array $query
 *   array of field, value pairs
 * @param string $method
 *   GET, PUT, POST, or DELETE
 *
 * @return boolean|mixed
 *   array of json data and meta data
 */
function trello_api($request, $query = array(), $method = 'GET', $include_result_object = FALSE) {
  // Establish path from $request.
  $path = '';
  if (is_array($request)) {
    foreach ($request as $key => $val) {
      $path .= is_string($key) && drupal_strlen($key) ? $key . '/' : '';
      $path .= is_string($val) && drupal_strlen($val) ? $val . '/' : '';
    }
  }
  else {
    $path = $request;
  }
  $path = trim($path, '/ ');

  // Establish $query array.
  if (is_string($query)) {
    parse_str($query, $array);
    $query = $array;
  }

  $url = sprintf('https://api.trello.com/%s/%s', TRELLO_API_VERSION, $path);
  if ($method != 'GET') {
    $args = $query;
    $query = array();
  }
  $query['key'] = trello_get_conf('key');
  $query['token'] = trello_get_conf('token');
  $url = url($url, array('query' => $query, 'absolute' => TRUE, 'https' => TRUE));

  $options = array('method' => $method, 'timeout' => 10);
  if ($method != 'GET') {
    $options['data'] = drupal_http_build_query($args);
    $options['headers'] = array('Content-Type' => 'application/x-www-form-urlencoded');
  }
  $request = drupal_http_request($url, $options);

  $json = json_decode($request->data);
  if (empty($json)) {
    watchdog('trello', 'Empty API result returned');
  }
  // Put it in an array for consistency.
  is_array($json) OR $json = array($json);

  // Compile meta information.
  $meta = array(
    'status' => $request->status_message,
    'count' => count($json), 'url' => $url, 'cached' => FALSE,
  );
  isset($args) AND $meta['args'] = $args;
  isset($request->error) AND $meta['error'] = $request->error;

  if ($include_result_object) {
    $meta['request'] = $request;
  }

  // Return the above results and the meta info.
  return array('data' => $json, 'meta_data' => $meta);
}

/**
 * The trello class for utilizing the trello api.
 */
class Trello {
  public $result = array();

  /**
   * This is called from the get, post, put, and delete methods.
   *
   * @param string $success
   *   callback to handle success
   * @param string $error
   *   callback to handle error
   */
  protected function handleCallbacks($success, $error) {
    if (empty($this->result) || !isset($this->result['data'])) {
      throw new Exception('No result to process');
    }

    // This is incomplete. Need to determine if it is success or not.
    return FALSE;

    if (!empty($success) && function_exists($success)) {
      call_user_func($success, $this->result);
    }
    if (!empty($error) && function_exists($error)) {
      call_user_func($error, $this->result);
    }
  }

  /**
   * Trello client.js style functions starting with GET.
   *
   * @param string $path
   *   API path
   * @param array $params
   *   (optional) key,values of paramaters
   * @param string $success
   *   (optional) callback on success
   * @param string $error
   *   (optional) callback on error
   *
   * @see https://trello.com/docs/gettingstarted/clientjs.html
   */
  public function get($path, $params = NULL, $success = NULL, $error = NULL) {
    $this->result = trello_api($path, $params, 'GET');
    $this->handleCallbacks($success, $error);
  }

  /**
   * Trello client.js style POST function.
   */
  public function post($path, $params = NULL, $success = NULL, $error = NULL) {
    $this->result = trello_api($path, $params, 'POST');
    $this->handleCallbacks($success, $error);
  }

  /**
   * Trello client.js style PUT function.
   */
  public function put($path, $params = NULL, $success = NULL, $error = NULL) {
    $this->result = trello_api($path, $params, 'PUT');
    $this->handleCallbacks($success, $error);
  }

  /**
   * Trello client.js style DELETE function.
   */
  public function delete($path, $params = NULL, $success = NULL, $error = NULL) {
    $this->result = trello_api($path, $params, 'DELETE');
    $this->handleCallbacks($success, $error);
  }
}
