<?php
/**
 * @file
 * Contains implementation of StingrayDummyCacheControl
 */

/**
 * Class StingrayDummyCacheControl
 */
class StingrayDummyCacheControl extends StingrayCacheControl {

  /**
   * {@inheritdoc}
   */
  function clearByPath($paths) {
    $return = array();
    // make paths an array.
    if (!is_array($paths)) {
      $paths = array($paths);
    }

    $base_url = $this->getBasePath();
    if (empty($base_url)) {
      throw new Exception('Base path is missing');
    }
    // Allowed schema values are http, https, both.
    $url_schema = isset($base_url['scheme']) ? $base_url['scheme'] : 'both';
    $url_host = isset($base_url['host']) ? $base_url['host'] : '';

    // prepend base path to paths to make URIs
    $uris = array();
    foreach ($paths as $path) {
      $path= rtrim(preg_match("/^\//", $path) ? $path : "/$path");
      array_push($uris, $path);
    }
    foreach ($uris as $uri) {
      $return[] = array($url_schema, $url_host, $uri);
    }
    // Set a message to test admin UI
    drupal_set_message('Cleared cache for given path');
    return $return;
  }

}