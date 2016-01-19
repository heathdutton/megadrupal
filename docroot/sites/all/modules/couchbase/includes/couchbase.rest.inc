<?php

/**
 * @file
 * A highly optimized Couchbase wrapping class.
 * 
 * Copyright (c) 2011-2012 DOCOMO Innovations, Inc.
 * 
 */

/**
 * A Couchbase wrapping class.
 */
class CouchbaseRest {
  
  /**
   * Couchbase endpoint constructed with
   * basic authentication scheme
   */
  protected $endpoint;

  /**
   * The Rest API port
   */
  protected $port = 8092;  

  /**
   * Default Constructor
   * @param 
   * 	$endpoint Couchbase Endpoint, ex: localhost
   * @param 
   *   $port Couchbase port
   * @param 
   * 	$username Username used for authentication
   * @param 
   * 	$password Password used for authentication
   */
  public function __construct($endpoint) {
    $this->endpoint = "http://{$endpoint}:{$this->port}";
  }

  /**
   * Helper method that makes the actual REST call.
   * All communication to CouchBase should be done
   * with ths function.  In the future, we can abstract
   * this function and swap out drupal_http_request
   * with another http client such as lib curl
   * @param
   *   $path Endpoint
   * @param
   *   $data The POST data accepted by drupal_http_request
   * @return
   *   Return the results from drupal_http_request.
   *   The response body is json decoded and stored
   *   in $result->decoded_data
   */
  protected function doPost($path, $data = '', $header = array('Content-Type' => 'application/x-www-form-urlencoded')) {
    $options = array(
        'method' => 'POST',
        'data' => $data,
        'headers' => $header,
    );
    $result = drupal_http_request($this->endpoint . '/' . $path, $options);
    // check for error, any error code above 400 is a bad response
    if ($result->code < 200 || $result->code > 206) {
      throw new CouchbaseRestException(t('Error executing @endpoint command. Error is @error',  array('@endpoint' => $this->endpoint . '/' . $path, '@error' => $result->data)));
    }
    if (isset($result->data)) {
      $result->decoded_data = json_decode($result->data, TRUE);
    }
    return $result;
  }
  
  /**
   * Helper method to do a HTTP Get
   * @param
   * 	 $path Complete URL for the GET
   * @return
   *   Return the results from drupal_http_request.
   *   The response body is json decoded and stored
   *   in $result->decoded_data
   */
  protected function doGet($path) {
    $result = drupal_http_request($this->endpoint . '/' . $path);
    if ($result->code < 200 || $result->code > 206) {
      throw new CouchbaseRestException(t('Error executing @endpoint command. Error - @error. Couchbase response - @cb_response',  array('@endpoint' => $this->endpoint . '/' . $path, '@error' => $result->error, '@cb_response' => $result->data)));
    }
    
    if (isset($result->data)) {
      $result->decoded_data = json_decode($result->data, TRUE);
    }
    return $result;
  }
  
  /**
   * Helper method to do a HTTP PUT
   * @param
   *   $path
   * @param
   *   $data The body of the PUT
   * @return
   *   Returns response from the Put request
   */
  protected function doPut($path, $data) {
    $options = array(
    	'method' => 'PUT',
      'data' => $data,
      'headers' => array(
      	'Content-Type' => 'application/json',
      	'Accept' => '*/*',
      ),
    );
    $result = drupal_http_request($this->endpoint . '/' . $path, $options);
    if ($result->code < 200 || $result->code > 206) {
      throw new CouchbaseRestException(t('Error executing @endpoint command. Error is @error',  array('@endpoint' => $this->endpoint . '/' . $path, '@error' => $result->data)));
    }
    if (isset($result->data)) {
      $result->decoded_data = json_decode($result->data, TRUE);
    }
    return $result;
  }

  /**
   * Helper method that performs a HTTP Delete
   * using drupal_http_request(). 
   * @param 
   *   $path
   * @return 
   *   Returns response from the Delete request
   */
  protected function doDelete($path) {
    $result = drupal_http_request($this->endpoint . '/' . $path, array('method' => 'DELETE'));
    if ($result->code < 200 || $result->code > 206) {
      throw new CouchbaseRestException(t('Error executing @endpoint command. Error is @error',  array('@endpoint' => $this->endpoint . '/' . $path, '@error' => $result->data)));
    }
    if (isset($result->data)) {
      $result->decoded_data = json_decode($result->data, TRUE);
    }
    return $result;
  }

  /**
   * List active tasks in a couchbase 
   * @return
   *   Return all active tasks that are running
   */
  public function getActiveTasks() {
    return $this->doGet('_active_tasks')->decoded_data;
  }

  /**
   * Get UUIDs from Couchbase 
   * @param $count
   *   The number of ids to return.  Defaults to 1 UUID
   * @return
   *   Returns the requested UUIDS
   */
  public function getUUIDS($count = 1) {
    $path = "_uuids" . (($count > 1) ? '?count=' . $count : '');
    return $this->doGet($path)->decoded_data;
  }

  /**
   * Create a design document
   * @param
   *   $bucket
   * @param
   *   $designDoc
   * @param
   *   $function Array reprensenting design document
   *
   */
  public function createDesignDoc($bucket, $designDoc, $function) {
    return $this->doPut("{$bucket}/_design/{$designDoc}", json_encode($function))->decoded_data;
  }
  
  /**
   * Get a design document by name
   * @param $bucket
   * @param $designDoc
   */
  public function getDesignDoc($bucket, $designDoc) {
    return $this->doGet("{$bucket}/_design/{$designDoc}", json_encode($function))->decoded_data;
  }
  
  /**
  * Delete a design document
  * @param $bucket
  * @param $designDoc
  */
  public function deleteDesignDoc($bucket, $designDoc) {
    try {
      $path = "_design/{$designDoc}";
      $design_doc = $this->getDesignDoc($bucket, $designDoc);
      $rev = $design_doc['_rev'];  
      $url .= "?rev={$rev}";
      $this->doDelete($path);
    }
    catch (CouchbaseRestException $e) {
      throw new CouchbaseRestException(t('Error deleting design doc @error', array('@error' => $e)));
    }
  }
  
  /**
   * Method accesses views using REST Api.  This is a temporary
   * method until the couchbase php lib supports sending keys as post.
   * This method follows the REST API spec, which is based on Couchdb
   * @param $bucket
   * @param $designDoc
   * @param $view
   * @param $keys
   * @param $descending
   * @param $startkey
   * @param $endkey
   * @param $limit
   * @param $additional_args
   */
  public function getView($bucket, $designDoc, $view, $keys = array(), $descending = FALSE, $startkey = '', $endkey = '', $limit = '', $additional_args = array()) {
    $query = array();
    if ($descending) {
      $query[] = 'descending=true';
    }
    if (!empty($startkey)) {
      $query[] = 'startkey="' . urlencode($startkey) . '"';
    }
    if (!empty($endkey)) {
      $query[] = 'endkey="' . urlencode($endkey) . '"';
    }
    if (!empty($limit)) {
      $query[] = 'limit=' . $limit;
    }
    if (count($additional_args)) {
      $query = array_merge($query, $additional_args);
    }
              
    // The presence of $keys means this is a post
    if (count($keys)) {
      $key_array = array(
        'keys' => $keys,
      );
      return $this->doPost($bucket . '/_design/' . $designDoc . '/_view/' . $view . (count($query) ? "?" . implode('&', $query) : ''), json_encode($key_array), array('Content-type' => 'application/json'))->decoded_data;
    }
    else {
      return $this->doGet($bucket . '/_design/' . $designDoc . '/_view/' . $view . (count($query) ? "?" . implode('&', $query) : ''))->decoded_data;
    }
  }
}


/**
 * A simple Couchbase-specific exception.
 */
class CouchbaseRestException extends Exception {}
