<?php
/**
 * @file
 * This file contains PTVTimetableAPI class.
 */

/**
 * A wrapper class for PTV timetable API.
 */
class PTVTimetableAPI {

  private $developerId;
  private $securityKey;
  private $poiTypes;
  private $transportModes;
  private $useAssocResponse;
  private $hasError;
  private $errorMessage;

  /**
   * Constructor.
   */
  public function __construct() {
    $this->developerId = '';
    $this->securityKey = '';
    $this->useAssocResponse = FALSE;
    $this->hasError = FALSE;
    $this->errorMessage = '';
    $this->getPOITypes();
    $this->getTransportModes();
  }

  /**
   * Re-format API call response data in associative array format.
   */
  public function useAssocResponse() {
    $this->useAssocResponse = TRUE;
  }

  /**
   * Set developer ID.
   *
   * @param int $developer_id
   *   PTV assigned developer ID.
   */
  public function setDeveloperId($developer_id) {
    $this->developerId = $developer_id;
  }

  /**
   * Set security key.
   *
   * @param string $security_key
   *   PTV provided security key.
   */
  public function setSecurityKey($security_key) {
    $this->securityKey = $security_key;
  }

  /**
   * Check that API call response has error.
   *
   * @return bool
   *   True if API call response has error, otherwise FALSE.
   */
  public function hasError() {
    return $this->hasError;
  }

  /**
   * Get API call response error message.
   *
   * @return string
   *   Error message.
   */
  public function getErrorMessage() {
    return $this->errorMessage;
  }

  /**
   * Get defined POI types.
   */
  private function getPoiTypes() {

    $this->poiTypes = array();

    if (defined('PTV_TIMETABLE_API_TYPE_TRAIN')) {
      $this->poiTypes[] = PTV_TIMETABLE_API_TYPE_TRAIN;
    }

    if (defined('PTV_TIMETABLE_API_TYPE_TRAM')) {
      $this->poiTypes[] = PTV_TIMETABLE_API_TYPE_TRAM;
    }

    if (defined('PTV_TIMETABLE_API_TYPE_BUS')) {
      $this->poiTypes[] = PTV_TIMETABLE_API_TYPE_BUS;
    }

    if (defined('PTV_TIMETABLE_API_TYPE_VLINE')) {
      $this->poiTypes[] = PTV_TIMETABLE_API_TYPE_VLINE;
    }

    if (defined('PTV_TIMETABLE_API_TYPE_NIGHTRIDER')) {
      $this->poiTypes[] = PTV_TIMETABLE_API_TYPE_NIGHTRIDER;
    }

    if (defined('PTV_TIMETABLE_API_TYPE_TICKET_OUTLET')) {
      $this->poiTypes[] = PTV_TIMETABLE_API_TYPE_TICKET_OUTLET;
    }

  }

  /**
   * Get defined transport modes.
   */
  private function getTransportModes() {
    $this->transportModes = array();

    if (defined('PTV_TIMETABLE_API_TRANSPORT_GENERAL')) {
      $this->transportModes[] = PTV_TIMETABLE_API_TRANSPORT_GENERAL;
    }

    if (defined('PTV_TIMETABLE_API_TRANSPORT_METRO_BUS')) {
      $this->transportModes[] = PTV_TIMETABLE_API_TRANSPORT_METRO_BUS;
    }

    if (defined('PTV_TIMETABLE_API_TRANSPORT_METRO_TRAIN')) {
      $this->transportModes[] = PTV_TIMETABLE_API_TRANSPORT_METRO_TRAIN;
    }

    if (defined('PTV_TIMETABLE_API_TRANSPORT_METRO_TRAM')) {
      $this->transportModes[] = PTV_TIMETABLE_API_TRANSPORT_METRO_TRAM;
    }

    if (defined('PTV_TIMETABLE_API_TRANSPROT_REGIONAL_BUS')) {
      $this->transportModes[] = PTV_TIMETABLE_API_TRANSPROT_REGIONAL_BUS;
    }

    if (defined('PTV_TIMETABLE_API_TRANSPROT_REGIONAL_COACH')) {
      $this->transportModes[] = PTV_TIMETABLE_API_TRANSPROT_REGIONAL_COACH;
    }

    if (defined('PTV_TIMETABLE_API_TRANSPROT_REGIONAL_TRAIN')) {
      $this->transportModes[] = PTV_TIMETABLE_API_TRANSPROT_REGIONAL_TRAIN;
    }
  }

  /**
   * Generate PTV timetable API timestamp.
   *
   * @param string $now
   *   A data and time string in a format like 2015-05-05 13:30:20.
   */
  private function iso8601Utc($now = NULL) {
    $timestamp = isset($now) ? strtotime($now) : REQUEST_TIME;
    return gmdate('Y-m-d\TH:i:s\Z', $timestamp);
  }

  /**
   * Generate PTV timetable API signature.
   *
   * @param string $request
   *   The request string without the base URL but including developer ID.
   */
  private function hash($request) {
    return hash_hmac(PTV_TIMETABLE_API_SIGNATURE_HASH_METHOD, $request, $this->securityKey);
  }

  /**
   * Actual PTV API call.
   *
   * @param string $request
   *   Request string without base URL and data.
   * @param array $data
   *   Query string data.
   *
   * @return object
   *   drupal_http_request response.
   */
  private function call($request, $data = array()) {

    $data['devid'] = $this->developerId;

    $signature = $this->hash($request . '?' . http_build_query($data));

    $data['signature'] = $signature;

    $endpoint = PTV_TIMETABLE_API_BASE_URL . $request . '?' . http_build_query($data);

    $response = drupal_http_request($endpoint);

    if ($response->code != '200') {
      $this->hasError = TRUE;
      $this->errorMessage = $response->code . ' ' . $response->status_message;
      return FALSE;
    }

    $data = json_decode($response->data, $this->useAssocResponse);

    return $data;
  }

  /**
   * PTV timetable API - health check.
   *
   * A check on the timely availability, connectivity and reachability of the
   * services that deliver security, caching and data to web clients.
   *
   * @return array
   *   An array of health check reports.
   */
  public function healthCheck() {

    $request = PTV_TIMETABLE_API_HEALTH_CHECK;

    $data = array('timestamp' => $this->iso8601Utc());

    return $this->call($request, $data);
  }

  /**
   * PTV timetable API - stops nearby.
   *
   * Stops Nearby returns up to 30 stops nearest to a specified coordinate.
   *
   * @param float $lat
   *   Prescribed latitude, expressed in decimal degrees.
   * @param float $long
   *   Prescribed longitude, expressed in decimal degrees.
   *
   * @return array
   *   A list of stops nearby provided latitude and longitude.
   */
  public function stopsNearby($lat, $long) {
    $request = format_string(PTV_TIMETABLE_API_STOPS_NEARBY, array(
      '@lat' => $lat,
      '@long' => $long,
    ));

    return $this->call($request);
  }

  /**
   * PTV timetable API - transport POIs by map.
   *
   * Transport POIs by Map returns a set of locations consisting of stops
   * and/or myki ticket outlets (collectively known as points of interest –
   * i.e. POIs) within a region demarcated on a map through a set of latitude
   * and longitude coordinates.
   *
   * @param float $lat1
   *   Latitude at the top left corner of a region depicted on a map.
   * @param float $long1
   *   Longitude at the top left corner of a region depicted on a map.
   * @param float $lat2
   *   Latitude at the bottom right corner of a region depicted on a map.
   * @param float $long2
   *   Longitude at the bottom right corner of a region depicted on a map.
   * @param int $grid_depth
   *   The number of cells per block of cluster grid (between 0-20 inclusive).
   * @param int $limit
   *   The minimum number of POIs required to create a cluster, as well
   *   as the maximum number of POIs returned as part of a cluster in the JSON
   *   response.
   * @param array $poi_types
   *   An array of numbers representing the types of POIs you want returned.
   *
   * @return array
   *   Points of interest.
   */
  public function pointsOfInterest($lat1, $long1, $lat2, $long2, $grid_depth, $limit, array $poi_types) {

    $poi_types = array_intersect($this->poiTypes, $poi_types);

    if (empty($poi_types)) {
      $poi_types = $this->poiTypes;
    }

    // Adjust grid depth.
    $grid_depth = (int) $grid_depth;

    if ($grid_depth < PTV_TIMETABLE_API_MIN_POI_GRID_DEPTH) {
      $grid_depth = PTV_TIMETABLE_API_MIN_POI_GRID_DEPTH;
    }

    if ($grid_depth > PTV_TIMETABLE_API_MAX_POI_GRID_DEPTH) {
      $grid_depth = PTV_TIMETABLE_API_MAX_POI_GRID_DEPTH;
    }

    $request = format_string(PTV_TIMETABLE_API_POI, array(
      '@lat1' => (float) $lat1,
      '@long1' => (float) $long1,
      '@lat2' => (float) $lat2,
      '@long2' => (float) $long2,
      '@griddepth' => $grid_depth,
      '@limit' => (int) $limit,
      '@poi' => implode(',', $poi_types),
    ));

    return $this->call($request);
  }

  /**
   * PTV timetable API - Search.
   *
   * The Search API returns all stops and lines that match the input search
   * text.
   *
   * @param string $text
   *   Search text.
   *
   * @return array
   *   An array of search results.
   */
  public function search($text) {
    $request = format_string(PTV_TIMETABLE_API_SEARCH, array(
      '@search' => check_plain($text),
    ));

    return $this->call($request);
  }

  /**
   * PTV timetable API - broad next departures.
   *
   * Broad Next Departures returns the next departure times at a prescribed
   * stop irrespective of the line and direction of the service.
   *
   * @param int $mode
   *   A number representing the transport type of the stop.
   * @param string $stop
   *   The stop ID of the stop.
   * @param int $limit
   *   The number of next departure times to be returned.
   *
   * @return array
   *   An array of next departures times at a stop.
   */
  public function broadNextDepartures($mode, $stop, $limit = 0) {

    $request = format_string(PTV_TIMETABLE_API_BROAD_NEXT_DEPARTURES, array(
      '@mode' => $mode,
      '@stop' => $stop,
      '@limit' => $limit,
    ));

    return $this->call($request);
  }

  /**
   * PTV Timetable API - Specific next departures.
   *
   * Specific Next Departures returns the times for the next departures at a
   * prescribed stop for a specific mode, line and direction.
   *
   * @param int $mode
   *   A number representing the transport_type of the stop.
   * @param int $line
   *   The line ID of the requested services.
   * @param int $stop
   *   The stop ID of the stop.
   * @param int $direction
   *   The direction ID of the requested services.
   * @param int $limit
   *   The number of next departure times to be returned.
   * @param string $datetime
   *   The date and time of the request. e.g. 2015-05-05 13:30:20.
   *
   * @return array
   *   An array of times for the next departures at a stop.
   */
  public function specificNextDepartures($mode, $line, $stop, $direction, $limit = 5, $datetime = NULL) {

    $request = format_string(PTV_TIMETABLE_API_SPECIFIC_NEXT_DEPARTURES, array(
      '@mode' => $mode,
      '@line' => $line,
      '@stop' => $stop,
      '@direction' => $direction,
      '@limit' => $limit,
    ));

    $data = array('for_utc' => $this->iso8601Utc($datetime));

    return $this->call($request, $data);
  }

  /**
   * PTV Timetable API - Stopping pattern.
   *
   * The Stopping Pattern API returns the stopping pattern for a specific run
   * (i.e. transport service) from a prescribed stop at a prescribed time.
   *
   * @param int $mode
   *   A number representing the transport_type of the stop.
   * @param int $run
   *   The run ID of the requested run.
   * @param int $stop
   *   The stop ID of the stop.
   * @param string $datetime
   *   The date and time of the request. e.g. 2015-05-05 13:30:20.
   *
   * @return array
   *   An array of stopping pattern for a specific run.
   */
  public function stoppingPattern($mode, $run, $stop, $datetime = NULL) {
    $request = format_string(PTV_TIMETABLE_API_STOPPING_PATTERN, array(
      '@mode' => $mode,
      '@run' => $run,
      '@stop' => $stop,
    ));

    $data = array('for_utc' => $this->iso8601Utc($datetime));

    return $this->call($request, $data);
  }

  /**
   * PTV Timetable API - Stops on a line.
   *
   * The Stops on a Line API returns a list of all the stops for a requested
   * line, ordered by location name.
   *
   * @param int $mode
   *   A number representing the transport_type of the stop.
   * @param int $line
   *   The line ID of the requested line.
   *
   * @return array
   *   An array of stops on a line.
   */
  public function lineStops($mode, $line) {

    $request = format_string(PTV_TIMETABLE_API_LINE_STOPS, array(
      '@mode' => $mode,
      '@line' => $line,
    ));

    return $this->call($request);
  }

  /**
   * PTV Timetable API - Lines by mode.
   *
   * Lines by Mode returns the lines for a selected mode of transport.
   *
   * @param int $mode
   *   A number representing the transport type of the line.
   * @param string $name
   *   Optional: part of a line's name.
   *
   * @return array
   *   An array of lines in a specific transport type.
   */
  public function lines($mode, $name = NULL) {

    $request = format_string(PTV_TIMETABLE_API_LINES, array(
      '@mode' => $mode,
    ));

    $data = isset($name) ? array('name' => $name) : array();

    return $this->call($request, $data);
  }

  /**
   * PTV Timetable API - Disruptions.
   *
   * The Disruptions API returns information on planned and unplanned
   * disruptions for selected modes of transport.
   *
   * @param array $modes
   *   A comma separated list of modes of transport for which disruption
   *   information is returned.
   *
   * @return array
   *   An array of disruptions.
   */
  public function disruptions(array $modes = array()) {

    if (empty($modes)) {
      $modes = $this->transportModes;
    }

    $request = format_string(PTV_TIMETABLE_API_DISRUPTIONS, array(
      '@modes' => implode(',', $modes),
    ));

    return $this->call($request);
  }

}
