<?php
/**
 * @file
 * Initiate requests to forWhereiAm API and return results in json.
 */

require_once 'includes/forwhereiam___standard.api.inc';

define("CLIENT_ID", variable_get('forwhereiam___standard_client_id'));
define("CLIENT_SECRET", variable_get('forwhereiam___standard_client_secret'));

/**
 * Call forWhereiAm API's "v1/find.json" endpoint.
 *
 * Request an equivalent postcode for given lon/lat.
 */
function forwhereiam___standard_findme_ajax() {

  $output = array();

  $lat = utf8_decode($_REQUEST['lat']);
  $lat = filter_var($lat, FILTER_SANITIZE_NUMBER_FLOAT, FILTER_FLAG_ALLOW_FRACTION);

  $lon = utf8_decode($_REQUEST['lon']);
  $lon = filter_var($lon, FILTER_SANITIZE_NUMBER_FLOAT, FILTER_FLAG_ALLOW_FRACTION);

  // If no coords specified, do nothing.
  if (empty($lon) || empty($lat)) {
    $output['error'] = t("Missing coordinates. Unable to find equivalent postcode.");
  }
  else {

    $params = array();
    $params['lat'] = $lat;
    $params['lon'] = $lon;

    $forwhereiam = new Forwhereiam(array('clientId' => constant('CLIENT_ID'), 'clientSecret' => constant('CLIENT_SECRET')));
    $output = $forwhereiam->api('/v1/find.json', 'GET', $params);
  }

  drupal_json_output(array('status' => TRUE, 'data' => $output));
}

/**
 * Call forWhereiAm API's "/v1/announcements.json" endpoint.
 *
 * Ask for all relevant announcements for the given user
 * location, and optional keywords and radius.
 */
function forwhereiam___standard_search_ajax() {

  $output = array();

  $location = substr($_REQUEST['location'], 0, 50);
  $location = filter_var($location, FILTER_SANITIZE_STRING, FILTER_FLAG_NO_ENCODE_QUOTES);
  if (empty($location)) {
    $output['error'] = t("Postcode must be given.");
  }
  else {
    $params = array();
    $params['location'] = $location;

    if (isset($_REQUEST['after'])) {
      // Only return the latest announcements which occurred after
      // a given time (i.e. return more newer announcements).
      $after = substr($_REQUEST['after'], 0, 20);
      $after = filter_var($after, FILTER_VALIDATE_INT);

      $params['lastCheckAt'] = $after;
    }
    if (isset($_REQUEST['terms'])) {
      $terms = substr($_REQUEST['terms'], 0, 20);
      $terms = filter_var($terms, FILTER_SANITIZE_STRING, FILTER_FLAG_NO_ENCODE_QUOTES);

      $params['keywords'] = $terms;
    }
    if (isset($_REQUEST['within'])) {
      $within = substr($_REQUEST['within'], 0, 12);
      $within = filter_var($within, FILTER_SANITIZE_STRING, FILTER_FLAG_NO_ENCODE_QUOTES);

      $params['radius'] = $within;
    }

    $forwhereiam = new Forwhereiam(array('clientId' => constant('CLIENT_ID'), 'clientSecret' => constant('CLIENT_SECRET')));
    $output = $forwhereiam->api('/v1/announcements.json', 'GET', $params);
  }

  drupal_json_output(array('status' => TRUE, 'data' => $output));
}

/**
 * Call forWhereiAm API's "/v1/announcements/<id>.json" endpoint.
 *
 * Ask for the full details of the given announcement id.
 */
function forwhereiam___standard_details_ajax() {

  $output = array();

  $id = filter_var($_REQUEST['id'], FILTER_SANITIZE_STRING, FILTER_FLAG_NO_ENCODE_QUOTES);

  if (empty($id)) {
    $output['error'] = t("Invalid announcement ID given.");
  }

  if (empty($output['error'])) {
    $forwhereiam = new Forwhereiam(array('clientId' => constant('CLIENT_ID'), 'clientSecret' => constant('CLIENT_SECRET')));
    $output = $forwhereiam->api('/v1/announcements/' . $id . '.json', 'GET');
  }

  drupal_json_output(array('status' => TRUE, 'data' => $output));
}

/**
 * Call forWhereiAm API's "/v1/rate.json" endpoint.
 *
 * Post the user's rating for the given announcement id.
 */
function forwhereiam___standard_rate_ajax() {

  $output = array();

  $id = filter_var($_REQUEST['id'], FILTER_SANITIZE_STRING, FILTER_FLAG_NO_ENCODE_QUOTES);

  $rating = substr($_REQUEST['vote'], 0, 1);
  $rating = filter_var($rating, FILTER_VALIDATE_INT);

  $ip = ip_address();
  if (empty($ip)) {
    $ip = filter_var($_REQUEST['ip'], FILTER_SANITIZE_STRING, FILTER_FLAG_NO_ENCODE_QUOTES);
  }

  if (empty($id)) {
    $output['error'] = t("Invalid announcement ID given.");
  }
  elseif ($rating < 1 || $rating > 5) {
    $output['error'] = t("Invalid rating given.");
  }

  if (empty($output['error'])) {

    $params = array();
    $params['vote'] = $rating;
    $params['ip'] = $ip;

    $forwhereiam = new Forwhereiam(array('clientId' => constant('CLIENT_ID'), 'clientSecret' => constant('CLIENT_SECRET')));
    $output = $forwhereiam->api('/v1/announcements/' . $id . '/rate.json', 'POST', $params);
  }

  drupal_json_output(array('status' => TRUE, 'data' => $output));
}
     
/**
 * Call forWhereiAm API's "/v1/register.json" endpoint.
 *
 * Post the user's email, password and location to signup.
 * Assumptions: 
 *   - data already validated for this function
 *   - data not returned in json format.
 */
function forwhereiam___standard_register_ajax($params) {     
     
  $output = array();
     
  $forwhereiam = new Forwhereiam(array('clientId' => constant('CLIENT_ID'), 'clientSecret' => constant('CLIENT_SECRET')));
  $output = $forwhereiam->api('/v1/register.json', 'POST', $params);
     
  return $output;
}     
