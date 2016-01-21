<?php

/**
 * @file
 *
 * Main class for making call to google weather service.
 */

class googleWeather {
  
  // Set up public variables so they can be accessed from outside.
  protected $location, $language, $data, $cache;

  function __construct($location, $language) {
    $this->location = $location;
    $this->language = $language;
    // Get the cache.
    $this->cache_get();

    return $this;
  }
  
  /**
	 * Main function to make call to google weather service using drupal_http_request().
	 */
  protected function request_data() {

    if ($this->location->days == NULL || !is_numeric($this->location->days)) {
      $this->location->days = 1;
    }

    $timeout = variable_get("google_weather_timeout", 20);
    $api = "http://www.google.com/ig/api?hl=" . $this->language->language . "&weather=";
    $url = $api . urlencode($this->location->location);

    $request = drupal_http_request($url);
    $data = isset($request->data) ? $request->data : NULL;

    /*if (mb_detect_encoding($data, 'UTF-8', TRUE) !== 'UTF-8') {
      $data = utf8_encode($data);
    }*/

    $xml = simplexml_load_string($data);

    if ($xml) {
      if ($xml->weather->problem_cause) {
        $blad = $xml->weather->problem_cause->attributes()->data;
        return $blad;
      }

      $handler = $xml->weather;

      $content = array();
      $content['google_weather_info'] = array(
        'location' => (string) $handler->forecast_information->postal_code->attributes()->data,
        'date' => (string) $handler->forecast_information->forecast_date->attributes()->data,
      );

      $today = &$handler->current_conditions;
      $condition = (string) $today->condition->attributes()->data;
      $convert_to = variable_get('google_weather_units', 'SI');
      $unit = $this->weather_get_unit($convert_to);
      $icon_src = (string)$today->icon->attributes()->data;

      $image_vars = array(
        'path' => 'http://www.google.com' . $icon_src,
        'title' => $condition,
        'alt' => $condition,
      );

      $content['google_weather_current'] = array(
        'temp' => (string) $this->weather_convert($today->temp_f->attributes()->data, $convert_to) . $unit,
        'humidity' => (string) $today->humidity->attributes()->data,
        'icon' => theme('image', $image_vars),
        'condition' => $condition,
        'wind_condition' => (string) $today->wind_condition->attributes()->data,
      );

      if (empty($icon_src)) {
        $content['google_weather_current']['icon'] = '<div class="spacer"></div>';
      }

      $i = 0;
      $content['google_weather_forecast'] = array();

      foreach ($handler->forecast_conditions as $nextdays) {
        $i++;
        if ($i >= $this->location->days) continue;
        $condition = (string) $nextdays->condition->attributes()->data;
        $image_vars = array(
          'path' => 'http://www.google.com' . (string) $nextdays->icon->attributes()->data,
          'title' => $condition,
          'alt' => $condition,
        );
        $content['google_weather_forecast'][$i] = array(
          'day' => ($i == 1) ? t("Today") : ucwords((string) $nextdays->day_of_week->attributes()->data),
          'low_temp' => (string) $this->weather_convert($nextdays->low->attributes()->data, $convert_to) . $unit,
          'high_temp' => (string) $this->weather_convert($nextdays->high->attributes()->data, $convert_to) . $unit,
          'icon' => theme('image', $image_vars),
          'condition' => $condition,
        );
      }
      $content['google_weather_forecast_header'] = t("Weather forecast");
    }
    else {
      return t("Google weather information could not be requested.");
    }

    $this->data = $content;

    return $this->data;
  }

  /**
   * Helper function for convert units.
   */
  protected function weather_convert($value, $convert_to) {
    if (is_null($convert_to)) {
      return $value;
    } 
    elseif ($convert_to == "SI") {
      return round(($value-32)*5/9);
    } 
    else {
      // Already in Farenheit, so no need to convert.
      // (could be better, looks like nextdays is in Farenheit)
      return $value; //return round($value*9/5+32);
    }
  }

  /**
  * Helper function to return global unit setting.
  */
  protected function weather_get_unit($convert_to = NULL) {
    if (is_null($convert_to)) {
      $convert_to = variable_get('google_weather_units', 'SI');
    }
    if ($convert_to == "SI") {
      return '°C';
    }
    else {
      return '°F';
    }
  }

  /**
   * Wrapper to return weather data.
   */
  public function get_weather_data() {
    if (!isset($this->location) && !is_object($this->location)) {
      return;
    }

    return $this->cache_get() ? $this->cache->data : $this->weather_data(); 
  }

  /**
  * Generate cache id based on $this->location.
  */
  private function get_cache_id() {
    $location = $this->location;
    $id = isset($location->bid) ? "bid_$location->bid" : "uid_$location->uid";
    
    return 'google_weather_' . $id;
  }

  /**
  * Set cache in db and store in $cache variable.
  */
  private function cache_set() {
    // Cache for x time (settings).
    $this->cache = cache_set($this->get_cache_id(), $this->data, 'cache_block',
      time() + variable_get('google_weather_block_cache', 3600));
  }

  /**
  * Get cache and return it if it can, otherwise return FALSE.
  */
  private function cache_get() {
    $this->cache = cache_get($this->get_cache_id(), 'cache_block');
    // Return cache if possible, and if expiration time is still more than current time.
    if (!empty($this->cache) && isset($this->cache->data) && ($this->cache->expire > time())) {
      return $this->cache->data;
    }
    return FALSE;
  }

  /**
  * Call request_data to get weather from google and store in cache.
  */
  private function weather_data() {
    // Request google weather data again and save in object cache.
    $this->request_data();
    $this->cache_set();

    return $this->data;
  }

} // Google weather class.
