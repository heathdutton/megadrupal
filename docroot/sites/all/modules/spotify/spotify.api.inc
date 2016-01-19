<?php

/**
 * @file
 * Implementation of Spotify API methods.
 */

/**
 * Spotify web services.
 */
define('SPOTIFY_LOOKUP_WS', 'http://ws.spotify.com/lookup/1/');
define('SPOTIFY_SEARCH_WS', 'http://ws.spotify.com/search/1/');

/**
 * Filter results based on territorial settings.
 *
 * @param $type
 *   What type of results to filter; album, artist or track.
 * @param $results
 *   Results as raw XML (string).
 * @return
 *   Filtered results as raw XML (string).
 */
function _spotify_filter_results($type, $results) {
  $selected_territories = variable_get('spotify_territories', NULL);
  if (is_null($selected_territories) || empty($selected_territories)) {
    return;
  }

  $selected_territories = explode(',', $selected_territories);
  array_walk($selected_territories, 'trim');

  $doc = new DOMDocument();
  $doc->loadXML($results);
  $root = $doc->documentElement;

  $nodes = $root->getElementsByTagName($type);

  // Save references to nodes to remove as removing them in the
  // loop does not work.
  $remove = array();
  foreach ($nodes as $node) {
    $availability = $node->getElementsByTagName('availability');
    if ($availability->length > 0) {
      $availability = $availability->item(0);
    }
    else {
      continue;
    }
    $territories = $availability->getElementsByTagName('territories')->item(0);
    $item_territories = explode(' ', $territories->nodeValue);
    $diff = array_diff($selected_territories, $item_territories);
    if (!empty($diff)) {
      $remove[] = $node;
    }
  }

  // Remove nodes.
  foreach ($remove as $node) {
    $doc->documentElement->removeChild($node);
  }

  return $doc->saveXML();
}

/**
 * Perform request to Spotify Metadata API.
 *
 * @param $url
 *   Web service URL.
 * @return
 *   XML response on success or FALSE on failure.
 */
function _spotify_request($url) {
  // Generate cache key.
  $key = md5($url);
  
  // Check cache for query.
  $response = cache_get($key, 'cache');
  
  // No cache found, perform request.
  if (!$response) {
    // Make request.
    $response = drupal_http_request($url, array(), 'GET', NULL, 3);
    
    // Check response.
    if ($response->code != 200) {
      watchdog('spotify', $response->error);
      return FALSE;
    }
    else {
      // Set cache item.
      cache_set($key, $response, 'cache');
      
      $response = cache_get($key, 'cache');
    }
  }
  
  return $response->data;
}

/**
 * Perform search.
 *
 * @param $what
 *   Type of search: album, artist or track; required.
 * @param $q
 *   Search string encoded in UTF-8; required.
 * @param $page
 *   The page of the result set to return; defaults to 1.
 * @return
 *   XML response on success or FALSE on failure.
 */
function _spotify_search($what, $q, $page = 1) {
  if (strlen($q) < 3) {
    return FALSE;
  }
  $url = SPOTIFY_SEARCH_WS . $what . '?q=' . $q . '&page=' . $page;

  $results = _spotify_request($url);

  // Filter results.
  //$results->data = _spotify_filter_results($what, $results->data);

  return $results;
}

/**
 * Perform lookup.
 *
 * @param $what
 *   Type of lookup: album, artist or track; required.
 * @param $uri
 *   A Spotify URI; required.
 * @param $extras
 *   A comma-separated list of words that defines the detail level expected in
 *   the response. Either album or albumdetail.
 */
function spotify_lookup($uri, $extras = NULL) {
  // Assemble url.
  $url = SPOTIFY_LOOKUP_WS . '?uri=' . $uri;
  if (!is_null($extras)) {
    $url .= '&extras=' . $extras;
  }
  
  return _spotify_request($url);
}

/**
 * Search album.
 *
 * @param $q
 *   Search string encoded in UTF-8; required.
 * @param $page
 *   The page of the result set to return; defaults to 1.
 */
function spotify_search_album($q, $page = 1) {
  return _spotify_search('album', $q, $page);
}

/**
 * Search artist.
 *
 * @param $q
 *   Search string encoded in UTF-8; required.
 * @param $page
 *   The page of the result set to return; defaults to 1.
 */
function spotify_search_artist($q, $page = 1) {
  return _spotify_search('artist', $q, $page);
}

/**
 * Search track.
 *
 * @param $q
 *   Search string encoded in UTF-8; required.
 * @param $page
 *   The page of the result set to return; defaults to 1.
 */
function spotify_search_track($q, $page = 1) {
  return _spotify_search('track', $q, $page);
}



