<?php

/**
 * @file
 * Get directions API
 */

/**
 * API Function to generate a url path for use by other modules/themes.
 *
 * Example Usage:
 * $path = getdirections_location_path('to', $nid);
 * $url = l(t('Get directions'), $path);
 *
 * @param string $direction
 *   Required. The direction the supplied input applies to. Must be either 'to' or 'from'
 * @param int $nid
 *   Required. The nid of the location node
 * @return
 *   Returns a path relative to the drupal root.
 */
function getdirections_location_path($direction, $nid) {
  if (getdirections_get_current_supported_module() && is_numeric($nid) && ($direction == 'to' || $direction == 'from')) {
    return "getdirections/location/$direction/$nid";
  }
}

/**
 * API Function to generate a url path for use by other modules/themes.
 *
 * Example Usage:
 * $path = getdirections_location_user_path('to', $uid);
 * $url = l(t('Get directions'), $path);
 *
 * @param string $direction
 *   Required. The direction the supplied input applies to. Must be either 'to' or 'from'
 * @param int $uid
 *   Required. The uid of the location owner
 * @return
 *   Returns a path relative to the drupal root.
 */
function getdirections_location_user_path($direction, $uid) {
  if (getdirections_get_current_supported_module() && is_numeric($uid) && ($direction == 'to' || $direction == 'from')) {
    return "getdirections/location_user/$direction/$uid";
  }
}

/**
 * API Function to generate a url path for use by other modules/themes.
 *
 * Example Usage:
 * $path = getdirections_locations_path($fromnid, $tonid);
 * $url = l(t('Get directions'), $path);
 *
 * @param int $fromnid
 *   Required. The nid  of the location node of the starting point
 * @param int $tonid
 *   Required. The nid of the location node of the destination
 * @return
 *   Returns a path relative to the drupal root.
 */
function getdirections_locations_path($fromnid, $tonid) {
  if (getdirections_get_current_supported_module() && is_numeric($fromnid) && is_numeric($tonid)) {
    return "getdirections/locations/$fromnid/$tonid";
  }
}

/**
 * API Function to generate a url path for use by other modules/themes.
 *
 * Example Usage:
 * $path = getdirections_locations_user_path($fromuid, $touid);
 * $url = l(t('Get directions'), $path);
 *
 * @param int $fromuid
 *   Required. The uid  of the location user of the starting point
 * @param int $touid
 *   Required. The uid of the location user of the destination
 * @return
 *   Returns a path relative to the drupal root.
 */
function getdirections_locations_user_path($fromuid, $touid) {
  if (getdirections_get_current_supported_module() && is_numeric($fromuid) && is_numeric($touid)) {
    return "getdirections/locations_user/$fromuid/$touid";
  }
}

/**
 * API Function to generate a url path for use by other modules/themes.
 *
 * Example Usage:
 * $uids = "1,2,3,4";
 * where '1' is the uid of the starting point
 * '4' is the uid of the end point
 * and the numbers in between are the uids of the waypoints
 * $path = getdirections_locations_user_via_path($uids);
 * $url = l(t('Get directions'), $path);
 *
 * @param string $uids
 *   Required. A comma delimited list of uid numbers
 * @return
 *   Returns a path relative to the drupal root.
 */
function getdirections_locations_user_via_path($uids) {
  if (getdirections_get_current_supported_module()) {
    return "getdirections/locations_user_via/$uids";
  }
}

/**
 * API Function to generate a url path for use by other modules/themes.
 *
 * Example Usage:
 * $nids = "1,2,3,4";
 * where '1' is the nid of the starting point
 * '4' is the nid of the end point
 * and the numbers in between are the nids of the waypoints
 * $path = getdirections_locations_via_path($nids);
 * $url = l(t('Get directions'), $path);
 *
 * @param string $nids
 *   Required. A comma delimited list of nid numbers
 * @return
 *   Returns a path relative to the drupal root.
 */
function getdirections_locations_via_path($nids) {
  if (getdirections_get_current_supported_module()) {
    return "getdirections/locations_via/$nids";
  }
}


/**
 * API Function to generate a url path for use by other modules/themes.
 *
 * Example Usage:
 * $path = getdirections_location_u2n_path($fromuid, $tonid);
 * $url = l(t('Get directions'), $path);
 *
 *
 *
 * @param int $fromuid
 *   Required. The uid  of the location user of the starting point
 * @param int $tonid
 *   Required. The nid of the location node of the destination
 * @return
 *   Returns a path relative to the drupal root.
 */
function getdirections_location_u2n_path($fromuid, $tonid) {
  if (getdirections_get_current_supported_module() && is_numeric($fromuid) && is_numeric($tonid)) {
    return "getdirections/location_u2n/$fromuid/$tonid";
  }
}

/**
 * API Function to generate a url path for use by other modules/themes.
 *
 * Example Usage:
 * $path = getdirections_location_n2u_path($fromnid, $touid);
 * $url = l(t('Get directions'), $path);
 *
 *
 *
 * @param int $fromnid
 *   Required. The vid  of the location node of the starting point
 * @param int $touid
 *   Required. The uid of the location user of the destination
 * @return
 *   Returns a path relative to the drupal root.
 */
function getdirections_location_n2u_path($fromnid, $touid) {
  if (getdirections_get_current_supported_module() && is_numeric($fromnid) && is_numeric($touid)) {
    return "getdirections/location_u2n/$fromnid/$touid";
  }
}
/**
 * API Function to generate a url path for use by other modules/themes.
 *
 * Example Usage:
 * $path = getdirections_location_latlon_path('to', '1.1234,1.234', 'my place');
 * $url = l(t('Get directions'), $path);
 *
 *
 * @param string $direction
 *   Required. The direction the supplied input applies to. Must be either 'to' or 'from'
 * @param string $latlon
 *   Required. The location point to be used. In decimal latitude,longitude
 * @param string $locs
 *   Optional. A description of the point.
 * @return
 *   Returns a path relative to the drupal root.
 */
function getdirections_location_latlon_path($direction, $latlon, $locs='') {
  if (($direction == 'to' || $direction == 'from') && preg_match("/[0-9.\-],[0-9.\-]/", $latlon)) {
    $out = "getdirections/latlon/$direction/$latlon";
    if ($locs) {
      $out .= "/$locs";
    }
    return $out;
  }
}

/**
 * API Function to generate a url path for use by other modules/themes.
 *
 * Example Usage:
 * $path = getdirections_location_id_path('to', $lid);
 * $url = l(t('Get directions'), $path);
 *
 * @param string $direction
 *   Required. The direction the supplied input applies to. Must be either 'to' or 'from'
 * @param int $lid
 *   Required. The lid (location id) of the location, as found in the location table
 * @return
 *   Returns a path relative to the drupal root.
 */
function getdirections_location_id_path($direction, $lid) {
  if (getdirections_get_current_supported_module() && is_numeric($lid) && ($direction == 'to' || $direction == 'from')) {
    return "getdirections/location_id/$direction/$lid";
  }
}

/**
 * API Function to setup a map with two points
 *
 * @param string $fromlocs
 *   Required. The string to display for From.
 * @param string $fromlatlon
 *   Required. The location point for From. In decimal latitude,longitude
 * @param string $tolocs
 *   Required. The string to display for To.
 * @param string $tolatlon
 *   Required. The location point for To. In decimal latitude,longitude
 * @return
 *   Returns the themed map.
 */
function getdirections_locations_bylatlon($fromlocs, $fromlatlon, $tolocs, $tolatlon, $width='', $height='') {
  return getdirections_locations($fromlocs, $fromlatlon, $tolocs, $tolatlon, $width, $height);
}
