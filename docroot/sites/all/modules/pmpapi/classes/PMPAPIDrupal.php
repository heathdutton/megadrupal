<?php

/**
 * @file
 *
 * Defines a class for PMP creation/transmission and retreival/parsing
 */

class PMPAPIDrupal {
  
  /**
   * Initializes a PMPAPIDrupal object.
   */
  function __construct($cache = TRUE) {
    $this->query = new stdClass();
    $this->base = variable_get('pmpapi_base_url');
    $this->errors = array();

    // Cache

    // cache set at __construct() trumps all
    if ($cache === FALSE) {
      $this->cache = FALSE;
    }
    // see if cache is globally set
    elseif (variable_get('pmpapi_cache') !== NULL) {
      $this->cache = variable_get('pmpapi_cache');
    }
    // default = use cache
    else {
      $this->cache = TRUE;
    }
    $this->from_cache = FALSE;
    $this->cache_bin = variable_get('pmpapi_cache_bin', 'cache');
    
    // Auth
    $this->sdkInclude('AuthClient');
    $this->client_id = variable_get('pmpapi_auth_client_id');
    $this->client_secret = variable_get('pmpapi_auth_client_secret');
    $this->auth_client = NULL;
    $this->auth();
  }

  /**
   * Gets an AuthClient object (either fresh, or from cache).
   *
   * @return
   *   The AuthClient of this object
   */
  function auth() {
    // See if this is already stored in cache
    // Note: auth stuff caching is not configurable
    $cached_auth_client = cache_get('pmpapi_auth_client');
    if ($cached_auth_client) {
      $this->auth_client = $cached_auth_client->data;
    }
    else {
      try {
        $auth_client = new \Pmp\Sdk\AuthClient($this->base, $this->client_id, $this->client_secret);
        $this->auth_client = $auth_client;
        cache_set('pmpapi_auth_client', $auth_client);
      }
      catch (Exception $e) {
        $message = t('Error getting authentication for PMP, query aborted. Message: @exception', array('@exception' => $e->getMessage()));
        drupal_set_message($message, 'warning');
        $this->errors['auth'][] = $e->getMessage();
      }
    }
    return $this->auth_client;
  }

  /**
   * Generates a basic report of PMP object.
   *
   * @return array
   *   Various messages (strings) .
   */
  function report() {
    if (empty($this->errors)) {
      $options = array('tag' => 'samplecontent', 'profile' => 'story');
      $this->pull($options);
      return '<pre>' . print_r($this, 1) . '</pre>';
    }
    return '<pre>' . print_r($this->errors, 1) . '</pre>';
  }

  /**
   * Generates a GUID
   * 
   * @return
   *   A UUID v4 GUID
   */
  function guid() {
    $doc = $this->getDoc();
    if ($doc) {
      return $doc->createGuid();
    }
  }

  /**
   * Takes node, creates Hypermedia doc, sends it to PMP API.
   *
   * @param object $data
   *   A PHP stdClass, representing a PMP document.
   */
  function push($data) {
    try {
      $doc = $this->getDoc();
      if ($doc) {
        $doc->setDocument($data);
        return $doc->save();
      }
    }
    catch (Exception $e) {
      $message = t('Error pushing to PMP. Message: @exception', array('@exception' => $e->getMessage()));
      drupal_set_message($message, 'warning');
    }
  }

  /**
   * Pulls from the PMP.
   *
   * @param object $node
   *   PMP query parameters.
   * @param string $type
   *   Query type (docs, profiles, users, etc.)
   */
  function pull($options, $type = 'docs') {
    $this->sdkInclude('CollectionDocJson');
    $key = $this->cacheKey('docs', $options);
    $cache = $this->cacheGet($key);
    if ($cache) {
      $this->query = $cache;
      $this->from_cache = TRUE;
    }
    else {
      try {
        $doc = $this->getDoc();
        if ($doc) {
          $URN = "urn:collectiondoc:query:$type";
          $results = $doc->query($URN)->submit($options);
          $this->query->results = new stdClass();
          $this->query->results->json = $results;
          if (!empty($options['guid'])) {
            $this->query->results->docs[] = $results;
          }
          else {
            $this->query->results->docs = $results->items;
            $this->query->links = new stdClass();
            $this->query->links->navigation = $this->flattenNavLinks($results->links->navigation);
          }
          $this->cacheSet($key, $this->query);
        }
      }
      catch (Exception $e) {
        $message = t('Error querying the PMP. Message: @exception', array('@exception' => $e->getMessage()));
        drupal_set_message($message, 'warning');
        $this->errors['query'][] = $e->getMessage();
      }
    }
  }

  /**
   * Gets a doc from the PMP.
   *
   * @param string $path
   *   PMP query path.
   */
  function getDoc($path = '') {
    $this->sdkInclude('CollectionDocJson');
    $url = $this->base .'/'. $path;
    $key = $this->cacheKey($path);
    $cache = $this->cacheGet($key);
    if ($cache) {
      $this->from_cache = TRUE;
      return $cache;
    }
    elseif (!$this->errors){
      try {
        $doc = new \Pmp\Sdk\CollectionDocJson($url, $this->auth_client);
        $this->cacheSet($key, $doc);
        return $doc;
      }
      catch (Exception $e) {
        $message = t('Error fetching a doc from the PMP. Message: @exception', array('@exception' => $e->getMessage()));
        drupal_set_message($message, 'warning');
      }
    }
    else {
      drupal_set_message('Errors already present in object before doc could be fetched.');
    }
  }

  /**
   * Creates a doc to be pushed to the PMP.
   *
   * @param array $values
   *   Key/value pairs of data for the doc
   *
   * @return object
   *   A PMP doc, built as a PHP object
   */
  function createDoc($values) {
    $values += array(
      'version' => '1.0',
      'lang' => 'en',
      'profile' => 'story',
      'attributes' => array(
        'published' => $this->date(),
        'guid' => $this->guid(),
      ),
      'items' => array(),
    );

    $doc = new stdClass();
    $doc->attributes = new stdClass();
    $doc->version = $values['version'];

    $profile = new stdClass();
    $profile->href = $this->base . '/profiles/' . $values['profile'];
    $doc->links->profile[] = $profile;

    if (!empty($values['alt_url'])) {
      $alt_url = new stdClass();
      $alt_url->href = $values['alt_url'];
      $doc->links->alternate[] = $alt_url;
    }

    foreach($values['attributes'] as $k => $v) {
      $doc->attributes->$k = $v;
    }

    if (!empty($values['items'])) {
      $doc->links->item = $values['items'];
    }

    return $doc;
  }

  /**
   * Sets a cache value.
   *
   * @param string $key
   *   The key of the value to be cached
   *
   * @param string $value
   *   The value to be cached
   *
   * @param string $key
   *   The key of the value to be cached
   *
   * @param integer $lifetime
   *   Lifetime of cache item (in seconds) or CACHE_PERMANENT or
   *   CACHE_TEMPORARY
   *
   * @return
   *   Whatever cache_set might return
   */
  function cacheSet($key, $value, $lifetime = CACHE_PERMANENT) {
    // Always save to cache - unless caching is turned off globally (not
    // recommended).
    if (variable_get('pmpapi_cache')) {
      if ($lifetime > 0) {
        $lifetime = REQUEST_TIME + lifetime;
      }
      return cache_set($key, $value, $this->cache_bin, $lifetime);
    }
  }

  /**
   * Gets a cache value.
   *
   * @param string $key
   *   The key of the cached value
   *
   * @return
   *   The cached data, or FALSE, if $key does not exist, or data has expired
   */
  function cacheGet($key) {
    if ($this->cache) {
      $cache = cache_get($key, $this->cache_bin);
      if (!$cache || REQUEST_TIME > $cache->expire) {
        return FALSE;
      }
      return $cache->data;
    }
  }

  /**
   * Clears a cache value.
   *
   * @param string $key
   *   The key of the cached value
   */
  function cacheClear($key) {
    if ($this->cache) {
      cache_clear_all($key, $this->cache_bin);
    }
  }

  /**
   * Clears all cache values.
   */
  function cacheClearAll() {
    if ($this->cache) {
      cache_clear_all('pmpapi:', $this->cache_bin, TRUE);
    }
    $this->cacheClearAuthClient();
  }

  /**
   * Clears the saved auth client.
   */
  function cacheClearAuthClient() {
    cache_clear_all('pmpapi_auth_client', 'cache');
  }

  /**
   * Generates a unique cache key.
   *
   * @param string $path
   *   The path of the query
   * @param array $options
   *   PMP query parameters
   *
   * @return string
   *   A cache key
   */
  function cacheKey($path, $options = array()) {
    ksort($options);
    $url = url($this->base . '/'. $path, array('https' => TRUE, 'absolute' => TRUE, 'query' => $options));
    return 'pmpapi:' . $url;
  }

  function flattenNavLinks($nav_links) {
    $links = array();
    foreach($nav_links as $nav_link) {
      $index = $nav_link->rels[0];
      $links[$index] = $nav_link;
    }
    return $links;
  }

  /**
   * Deletes a doc from the PMP
   *
   * @param string $guid
   *   A UUID v4 GUID
   */
  function delete($guid) {

    try {
      $path = 'docs/' . $guid;
      $pmp = $this->getDoc($path);
      if ($pmp) {
        $pmp->delete();
      }
      else {
        $message = t('Unable to delete PMP doc with guid = @guid', array('@guid' => $guid));
        drupal_set_message($message, 'warning');
      }
    }
    catch (Exception $e) {
      $message = t('Error deleting from the PMP. Message: @exception', array('@exception' => $e->getMessage()));
      drupal_set_message($message, 'warning');
    }
  }

  /**
   * Includes a file from the SDK
   *
   * @param string $file
   *   The file name - minus the file extension
   */
  function sdkInclude($file) {
    $dir_path = 'phpsdk/lib/Pmp/Sdk/';
    if (module_load_include('php', 'pmpapi', $dir_path . $file)) {
      return TRUE;
    }
    else {
      require_once DRUPAL_ROOT . '/sites/all/libraries/' . $dir_path . $file . '.php';
    }
  }

  /**
   * Generates an ISO 8601 time from a timestamp
   *
   * @param string $timestamp
   *   A Unix timestamp
   *
   * @return string
   *   A ISO 8601 timestamp
   */
  function date($timestamp = '') {
    if ($timestamp == '') {
      $timestamp = REQUEST_TIME;
    }
    return format_date($timestamp, 'custom', DateTime::ISO8601);
  }
}
