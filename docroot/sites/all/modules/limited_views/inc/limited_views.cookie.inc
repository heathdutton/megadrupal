<?php

/**
 * @file
 *  Contains Cookie tracking method
 */

/**
 * Adds the javascript to the page.
 * 
 * @param integer $nid Node ID
 */
function limited_views_add_js($nid) {
  // Add some basic parameters
  drupal_add_js(array(
    'limited_views' => array(
      'cookie_name_count' => LIMITED_VIEWS_COOKIE_NAME_COUNT,
      'nid' => $nid,
      'expiry' => LIMITED_VIEWS_EXPIRY_TIME,
      'threshold' => LIMITED_VIEWS_VIEW_THRESHOLD,
    )
  ), 'setting');
  
  drupal_add_js(drupal_get_path('module', 'limited_views') . '/inc/jquery.cookie.js');
  drupal_add_js(drupal_get_path('module', 'limited_views') . '/inc/json2.js');
  drupal_add_js(drupal_get_path('module', 'limited_views') . '/inc/limited_views.cookie.js');
  
  drupal_add_js('jQuery(document).ready(function () { Drupal.limited_views.default_trigger(); });',
    array('type' => 'inline', 'scope' => 'footer', 'weight' => 5)
  );  
}

/**
 * Returns the number of views a user has accrued on their cookie
 *
 * This should always be used to access the view count, as it sanitizes the user
 * provided data.
 *
 * To use this in a test, you will have to use === in your statement to force
 * type checking.
 *
 * @return int
 *  number of content views
 */
function limited_views_get_view_count() {
  static $view_count;
  
  if(isset($view_count)) {
    return $view_count;
  }
  
  $view_count = 0;
  arg(0) == 'node' ? $current_nid = arg(1) : $current_nid = NULL;
  $expiry_timestamp = time() - LIMITED_VIEWS_EXPIRY_TIME;
  if(array_key_exists(LIMITED_VIEWS_COOKIE_NAME_COUNT, $_COOKIE)) {
    $cookie = is_array($_COOKIE[LIMITED_VIEWS_COOKIE_NAME_COUNT]) ?  $_COOKIE[LIMITED_VIEWS_COOKIE_NAME_COUNT] : json_decode($_COOKIE[LIMITED_VIEWS_COOKIE_NAME_COUNT]);
    $count_data = (is_array($cookie)) ? $cookie : array();
    foreach($count_data as $data) {
      $data =  (object) $data;
      // ignore the currently viewed nid if it's in their history
      // every x timestamp they should get a new one to view
      if( isset($data->nid) && $data->timestamp) {
        if ($data->nid != $current_nid && $data->timestamp > $expiry_timestamp) {
          // referrer hits arent counted (this one is free)
          if(!limited_views_is_search_engine_referrer() ) {      
            $view_count ++;
          }
        }
      } 
    }
  } else {
    $view_count = 0;
  }
  
  return $view_count;  
}

/**
 * See if this nid is in their history as provided by the cookie info
 * @param node-id $nid
 */
function limited_views_nid_in_history($nid) {
  $in_history = false;
  if(array_key_exists(LIMITED_VIEWS_COOKIE_NAME_COUNT, $_COOKIE)) {
    $cookie = is_array($_COOKIE[LIMITED_VIEWS_COOKIE_NAME_COUNT]) ?  $_COOKIE[LIMITED_VIEWS_COOKIE_NAME_COUNT] : json_decode($_COOKIE[LIMITED_VIEWS_COOKIE_NAME_COUNT]);
    $count_data = (is_array($cookie)) ? $cookie : array();
    // check their history only up to the number of free views allocated
    // as we store +1 which is the current one over the view
    $i=0;
    $limit = variable_get('limited_views_threshold', 5);    
    foreach($count_data as $data) {
      $i++;
      if($data->nid == $nid && $i <= $limit ) {
        $in_history = true;
      }
    }
  }
  return $in_history;
}

/**
 * Tests if the user has cookies enabled and has a limited_views cookie
 *
 * Beware, this causes a full page refresh if the user doesn't have the cookie yet
 */
function limited_views_check_method() {
  if (is_array($_COOKIE[LIMITED_VIEWS_COOKIE_NAME_COUNT])) {
    // user already has a count cookie
    return TRUE;
    
  }
  else {
    $cookies_enabled = (bool)$_COOKIE[LIMITED_VIEWS_COOKIE_NAME_ENABLED];
    if (empty($cookies_enabled) && !$_GET['cookie_check']) {
      // set a test cookie if the cookie doesnt exist, and the test isn't already running
      setcookie(LIMITED_VIEWS_COOKIE_NAME_ENABLED, 1, 0, '/');
      $query = array('cookie_check' => 1);
      $url = explode('?', trim($_SERVER['REQUEST_URI'], '/'));
      $other_queries = explode('&', $url[1]);
      foreach ($other_queries as $string) {
        $parts = explode('=', $string);
        $query[$parts[0]] = $parts[1];
      }
      drupal_goto($url[0], $query);
    }
    if (empty($cookies_enabled) && $_GET['cookie_check']) {
      // the test is running and the cookie doesn't exist, user is not
      // accepting our cookie
      return FALSE;
    }
    if ($cookies_enabled) {
      // client is accepting our cookie
      return TRUE;
    }
  }
}
