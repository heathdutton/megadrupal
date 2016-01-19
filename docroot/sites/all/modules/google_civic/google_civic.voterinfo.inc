<?php

/**
 * Renders the Google Civic voterinfo view.
 * endpoint: google_civic/where-do-I-vote
 *
 * @return string
 *   The voterinfo view rendered as HTML.
 */

function google_civic_voterinfo() {
  $address_form = drupal_get_form('google_civic_address_form');

  $address = check_plain(trim($_GET['address']));
  if (!$address) {
    return array(
      '#theme' => 'google_civic_voterinfo',
      '#address_form' => $address_form,
    );
  }

  drupal_add_css(drupal_get_path('module', 'google_civic')
                  . '/css/google_civic.css');

  $election_id = (int) variable_get('google_civic_election_id', '4000');

  $point = geocoder('google', $address);  

  $coord = $point->coords;
  $home_lat = (float) $coord[1];
  $home_long = (float) $coord[0];
  $home_bubble_text = "Your Location: " . $address; 


  $response = google_civic_request_voterinfo($election_id, $address);
  module_invoke_all('google_civic_voterinfo', $response);

  $address = google_civic_translate_address_response(
                                          $response->normalizedInput);

  $election_date = _google_civic_format_election_date();
  if (!empty($response->pollingLocations)) {
    $headline = format_plural(
      count($response->pollingLocations),
      'Your Polling Location for the !election_date Election',
      'Your Polling Locations for the !election_date Election',
      array('!election_date' => $election_date));
    $locations = google_civic_locations_response_massage(
      $response->pollingLocations); 

    $early_locations =!empty($response->earlyVoteSites) ?
      google_civic_locations_response_massage($response->earlyVoteSites) : 
      array();

    $all_polling_locations = array_merge($locations, $early_locations);

    $multi_locations = array();

    $home_obj = new stdClass();
    $home_obj->latitude = $home_lat;
    $home_obj->longitude = $home_long;
    $home_obj->balloon_text = $home_bubble_text;
    $home_obj->marker_color = "blue";
    $multi_locations[] = $home_obj; 

    for ($i = 0; $i < count($locations); $i++) {
      $current_location = $locations[$i];
      $curr_address = $current_location->address;
      $readable_address = ($curr_address->line1) . " " . ($curr_address->city) . " " . ($curr_address->state);

      $point = geocoder('google', $readable_address);
      $coord = $point->coords;

      $object = new stdClass();
      $object->latitude = (float) $coord[1];
      $object->longitude = (float) $coord[0];
      $object->balloon_text = (!empty($curr_address->locationName)) ? $curr_address->locationName : $curr_address->line1;
      $object->marker_color = "green";
      $multi_locations[] = $object;
    }

    for ($i = 0; $i < count($early_locations); $i++) {
      $current_location = $early_locations[$i];
      $curr_address = $current_location->address;
      $readable_address = ($curr_address->line1) . " " . ($curr_address->city) . " " . ($curr_address->state);
      
      $point = geocoder('google', $readable_address);
      $coord = $point->coords;
 
      $object = new stdClass();
      $object->latitude = (float) $coord[1];
      $object->longitude = (float) $coord[0];
      $object->balloon_text = (!empty($curr_address->locationName)) ? $curr_address->locationName : $curr_address->line1;
      $multi_locations[] = $object;        
    }

    if(module_exists('ip_geoloc')) {
      module_load_include('inc', 'ip_geoloc', 'ip_geoloc_api');
      $latTotal = 0;
      $lngTotal = 0;
      foreach ($multi_locations as $location) {
        $latTotal += $location->latitude;
    	$lngTotal += $location->longitude;
      }
      $centerLat = $latTotal/count($multi_locations);
      $centerLng = $lngTotal/count($multi_locations);
      $center_latlng = array();
      $center_latlng[] = $centerLat;
      $center_latlng[] = $centerLng;
      $map = ip_geoloc_output_map_multi_location($multi_locations, "ip-geoloc-map-multi-locations", '{"mapTypeId":"roadmap", "zoom":15}', NULL,  NULL, "TRUE", 3, $center_latlng, FALSE);
    }
    else{
      $map = NULL;
    }

    $locations = google_civic_locations_render($locations, 'poll');
    $early_locations = !empty($early_locations) ?
      google_civic_locations_render($early_locations, 'early-poll') :
      array();

    $election_info_link = google_civic_voterinfo_response_get_link($response);
    $sorry = NULL;
  }
  else {
    $headline = NULL;
    $locations = NULL;
    $early_locations = array();
    $election_info_link = NULL;
    $map = NULL;
    $contests = NULL;
    $sorry = t("Sorry, no polling locations found for: %address",
        array('%address' => $address));
  }

  if (!empty($response->contests)) {
    $contests = variable_get('google_civic_show_contest_info', TRUE) ?
      google_civic_contest_info($response->contests) : NULL;
  }
  else {
    $contests = NULL;
  }
  
  return array(
    '#theme' => 'google_civic_voterinfo',
    '#headline' => $headline,
    '#locations' => $locations,
    '#early_locations' => $early_locations,
    '#map' => $map,
    '#election_info_link' => $election_info_link,
    '#address_form' => $address_form,
    '#contests' => $contests,
    '#sorry' => $sorry,
  );
}

/**
 * @see GoogleCivicAPI::request_voterinfo (google_civic.google.php)
 */
function google_civic_request_voterinfo($election_id, $address) {
  module_load_include('inc', 'google_civic', 'google_civic.google');
  $api = google_civic_get_api();
  $response = $api->request_voterinfo($election_id, $address);
  return $response;
}

/**
 * Massages the pollingLocations of the voterinfo response to include
 * a Google Map as part of the location and extracts opening / closing
 * times.
 *
 * @param Array $locations_response
 *   A pollingLocations array from a voterinfo response.
 *
 * @return Array
 *   The pollingLocations--massaged to include a Google Map of the
 *   polling location.
 */
function google_civic_locations_response_massage($locations_response) {
  $locations = array();

  if (!$locations_response) {
    return $locations;
  }

  foreach ($locations_response as &$location) {
    list($start_time, $end_time) = google_civic_location_poll_time($location);
    $location->opening_time = $start_time;
    $location->closing_time = $end_time;

    // Don't know why Google returns kind of an odd zip code, but we want to
    // fix it.
    $zip = $location->address->zip;
    if (strlen($zip) == 9) {
      $location->address->zip = substr($zip, 0, 5) . "-" . substr($zip, 5, 9);
    }

    $locations[] = $location;
  }
  return $locations;
}

/**
 * Gets the electionInfoUrl from the voterinfo_response
 *
 * @param Array $voterinfo_response
 *   A voterinfo response.
 *
 * @return string
 *   The electionInfoUrl--rendered as an HTML anchor.
 */
function google_civic_voterinfo_response_get_link($voterinfo_response) {
  if (!count($voterinfo_response)) {
    return NULL;
  }

  if (!empty($voterinfo_response->state) && !empty($voterinfo_response->state[0]->local_jurisdiction->
      electionAdministrationBody->electionInfoUrl)) {
    $election_info_url = $voterinfo_response->state[0]->local_jurisdiction->
      electionAdministrationBody->electionInfoUrl;
  }

  if (empty($election_info_url)) {
    return NULL;
  }

  $election_info_link = l($election_info_url, $election_info_url);
  return $election_info_link;
}


/**
 * Gets the opening / closing times for a Google Civic polling location.
 *
 * @param Object $location
 *   A Google Civic polling location (from a voterinfo response).
 *
 * @return Array
 *   Two strings--the opening / closing times.
 */
function google_civic_location_poll_time($location) {
  $polling_hours = explode('-', $location->pollingHours);

  if (count($polling_hours) != 2) {
    return NULL;
  }

  $poll_times = array();
  $poll_times[] = trim($polling_hours[0]);
  $poll_times[] = trim($polling_hours[1]);

  if (!$poll_times[0] || !$poll_times[1]) {
    return NULL;
  }

  return $poll_times;
}

/**
 * Gets the admin-set election date as a formatted string for display.
 *
 * @return string
 *   The admin-set election date--formatted for display.
 */
function _google_civic_format_election_date() {
  $election_date = variable_get('google_civic_election_date',
                                '2012-11-06 00:00:00');
  $election_time = strtotime($election_date);
  $election_date = strftime('%B %e', $election_time);
  return $election_date;
}

/**
 * Renders a list of Google Civic locations as HTML.
 *
 * @param Array $locations
 *   A list of Google Civic locations.
 * @param string $class_name
 *   An identifier for the type of Google Civic location being rendered.
 *
 * @return string
 *   The $locations--rendered as HTML.
 */
function google_civic_locations_render($locations, $class_name) {
  $class_prefix = 'google-civic-voterinfo-' . $class_name;
  return theme('google_civic_locations', array('locations' => $locations, 'class_prefix' => $class_prefix));
}

/**
 * Renders a series of fieldsets with the contest information.
 */
function google_civic_contest_info($contests) {
  $candidates = $referendums = "";
  foreach ($contests as $contest) {
    switch($contest->type) {
      case 'Referendum':
        $variables = array(
          'title' => $contest->referendumTitle,
          'subtitle' => $contest->referendumSubtitle,
          'details' => l('More Details', $contest->referendumUrl),
        );
        $referendums .= theme('google_civic_contest_referendum', $variables);
        break;
      default:
        $variables = array(
          'title' => $contest->office,
          'candidates' => $contest->candidates,
        );
        $candidates .= theme('google_civic_contest_candidate', $variables);
        break;
    }
  }

  drupal_add_js('misc/form.js');
  drupal_add_js('misc/collapse.js');

  return $candidates . $referendums;
}
