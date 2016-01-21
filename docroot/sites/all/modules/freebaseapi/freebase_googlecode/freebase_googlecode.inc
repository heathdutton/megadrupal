<?php
/**
 * @file Freebase API
 *
 * Utility to make requests from the Freebase API.
 * This won't do anything on its own.
 *
 * See the README for theory and usage.
 *
 * NO DRUPALISMs in this file - it can be used as a stand-alone library.
 *
 * @author 'dman' Dan Morrison http://coders.co.nz/
 * @version 2010
 */

/**
 * Initialize the freebase lib, return an object handle.
 *
 * This function is allowed to be called from outside of a Drupal context
 * (for tests) if that's what you need to do.
 *
 * In order, it tries to
 *   use the libraries.module API,
 *   then searches sites/all/libraries,
 *   then searches the local module 'contrib' directory
 * for a copy of the googlecode API library.
 *
 * @param Bool $patched = default is to use the (broken) original library.
 * Set to TRUE to use a localized fork of that that resolves a few
 * unfinished features.
 */
function freebase_googlecode_get_freebase($config = array()) {
  static $freebase;
  if (!empty($freebase)) {
    return $freebase;
  }
  // Configure settings.
  // These get passed through when initializing the GoogleClient object also.
  // If we 'use_objects', then the API library becomes meaningful.
  // Otherwise it's just a cheap data array, and no use using the lib
  // at all.
  $defaults = array(
    // Use the local patched version of the API lib
    'use_patched' => TRUE,
    // Use OO invocations, otherwise we just get big arrays of data.
    'use_objects' => TRUE,
  );
  $config = array_merge($defaults, $config);

  // Do it the preferred Drupal way if possible.
  if (function_exists('libraries_load')) {
    $info = libraries_load('google-api-php-client');
    // Use either the distro or localized version of FreebaseService.
    if ($config['use_patched']) {
      include_once(dirname(__FILE__) . '/Google_FreebaseService.php');
    }
    else {
      include_once($info['library path'] . "/src/contrib/Google_FreebaseService.php");
    }
  }

  if (!empty($info['loaded'])) {
    // Double-check it's got what we need.
    if (!class_exists('Google_FreebaseService')) {
      trigger_error("Unable to find the Freebase client in the google-api-php-client lib, the library may not be installed right.",  E_USER_ERROR);
      return FALSE;
    }
    // Yay. we should be good, carry on.
  }
  else {
    // Find library.
    $searchdir = array(
      'sites/all/libraries',
    );
    if (function_exists('conf_path')) {
      $searchdir[] = conf_path() . '/libraries';
    }
    if (function_exists('drupal_get_path')) {
      $searchdir[] = drupal_get_path('module', 'freebase_googlecode') . '/contrib';
    }
    // If not in Drupal context, climb the directory tree manually and look for
    // sites/all/libraries
    $dir_parts = explode('/', dirname(__FILE__));
    while (!empty($dir_parts) && array_pop($dir_parts) != 'all') {}
    $searchdir[] = implode('/', $dir_parts) . '/all/libraries';

    $libdirname = 'google-api-php-client';
    foreach ($searchdir as $dir) {
      if (is_dir($dir) && is_dir("$dir/$libdirname") && is_file("$dir/$libdirname/src/Google_Client.php")) {
        require_once("$dir/$libdirname/src/Google_Client.php");
          // Use either the distro or localized version of FreebaseService.
        if ($config['use_patched']) {
          include_once(dirname(__FILE__) . '/Google_FreebaseService.php');
        }
        else {
          include_once("$dir/$libdirname/src/contrib/Google_FreebaseService.php");
        }
        break;
      }
    }
    if (!class_exists('Google_FreebaseService')) {
      trigger_error("Unable to find the Freebase client in the google-api-php-client lib, searched in ". join(', ', $searchdir) . " - try https://code.google.com/p/google-api-php-client/",  E_USER_ERROR);
      return FALSE;
    }
  }

  try {
    $client = new Google_Client($config);
    $freebase = new Google_FreebaseService($client);
  } catch (Exception $e) {
    return FALSE;
  }
  return $freebase;
}

/**
 * Run just one query on freebase
 *
 * @param a query, json string representing an array of conditions, or a PHP
 * array, containing condition objects
 * @param $just_result if true, chop down to and return the first result only.
 * @return A query result array, containing a code, a result, and maybe some
 * messages
 */
function freebase_googlecode_run_freebase_query($query, $just_result = FALSE) {
  if (is_string($query)) {
    $query = json_decode($query);
    if (empty($query)) {
      trigger_error("Bad JSON.", E_USER_ERROR);
      return NULL;
    }
  }
  elseif (is_array($query)) {
    // good
  }
  else {
    trigger_error("Wrong type of argument. $query", E_USER_ERROR);
    return NULL;
  }
  // Need to wrap it in an extra envelope for phpfreebase
  $queryarray = array('query' => $query);

  $fb = freebase_googlecode_get_freebase();
  if (! $fb) {
    // Error, library unavailable.
    return FALSE;
  }
  watchdog('freebase_googlecode', 'Making phpfreebase request <pre>%data</pre>', array('%data' => var_export($queryarray, 1)), WATCHDOG_INFO);
  $qnum = $fb->addQuery($queryarray);
  $query_results = $fb->get();
  if ($fb->hasError()) {
    drupal_set_message('fb errors:' . join(', ', $fb->getErrors), 'error');
  }
  $query_result = $query_results['q' . $qnum];
  // freebase.php, this should have been better named, or the key I was returned
  // should have been correct
  if (!empty($query_result['messages'])) {
    foreach ($query_result['messages'] as $message_packet) {
      drupal_set_message('Message from FreeBase Query:<pre>' . print_r($message_packet['message'], 1) . '</pre>', 'error');
    }
  }
  #dpm(get_defined_vars());

  if (empty($query_result['result'])) {
    drupal_set_message('Freebase query ran, but returned an empty resultset', 'error');
  }

  if ( $just_result ) {
    return (!empty($query_result['result'][0])) ? $query_result['result'][0] : NULL;
  }
  return $query_result;
}
