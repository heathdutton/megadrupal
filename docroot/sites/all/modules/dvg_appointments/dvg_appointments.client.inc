<?php

abstract class AppointmentsClientApi {

  protected $soap_client;
  protected $timezone;

  abstract function __construct();

  protected function call($function, $args = array(), $reset = FALSE) {
    try {
      $start = microtime(TRUE);

      ksort($args);
      $cid = 'dvg_appointments__' . $function . '__' . substr(md5(serialize($args)), 0, 8);
      if (!$reset && $cache = cache_get($cid)) {

        // Make sure the cache isn't expired.
        if ($cache->expire > REQUEST_TIME) {
          return $cache->data;
        }
      }
      if ($this->api_method == 'soap') {
        $return = $this->soap_client->{$function}($args);
      }
      elseif ($this->api_method == 'rest'){
        $rest_url = $this->url . $args['rest_url'];
        $method = $args['rest_method'];
        $rest_url_parameters = (isset($args['rest_url_parameters'])) ? $args['rest_url_parameters'] : array();

        // Remove REST information from the data.
        unset(
          $args['rest_method'],
          $args['rest_url'],
          $args['rest_url_parameters']
        );

        $data = drupal_json_encode($args);
        $options = array(
          'headers' => array(
            'Content-Type' => 'application/json',
          ),
          'method' => $method,
          'data' => $data
        );
        $response = drupal_http_request(url($rest_url, array('query' => $rest_url_parameters)), $options);
        $return = json_decode($response->data);
        if (isset($response->code) && isset($response->error)) {
          $return->code = $response->code;
          $return->error = $response->error;
        }
      }

      $duration = microtime(TRUE) - $start;
      if ($duration >= variable_get('dvg_appointments_api_duration_threshold', 2)) {
        watchdog('dvg_appointments', '@method duration: @durations (@info)', array(
          '@method' => $function,
          '@duration' => round($duration, 2),
          '@info' => var_export($args, TRUE),
        ), WATCHDOG_WARNING);
      }

      // Get the expiration times of this function.
      $expires = $this->get_cache_expire();
      $function_expire = isset($expires[$function]) ? $expires[$function] : variable_get('dvg_appointments__cache_default', 3600);

      $custom_function_expire = variable_get('dvg_appointments__cache__' . $function, FALSE);

      // Store the results in cache.
      if ($custom_function_expire || $custom_function_expire === FALSE) {
        $expire = REQUEST_TIME + ($custom_function_expire ? $custom_function_expire : $function_expire);
        cache_set($cid, $return, 'cache', $expire);
      }

      return $return;
    }
    catch (SoapFault $f) {
      watchdog('appointment_api', 'Soap Fault: ' . var_export($f, TRUE), array(), WATCHDOG_ERROR);
      return FALSE;
    }
  }

  public function get_timezone() {
    return $this->timezone;
  }

  protected function sort_available_products($a, $b) {
    return strcmp($a['name'], $b['name']);
  }

  abstract public function get_book_fields();

  public function set_booking_value($field, $value) {
    return $value;
  }

  abstract public function get_available_products();

  abstract public function get_customer_appointments($vars);

  abstract public function delete_appointment(DvgAppointment $appointment);

  /**
   * @param array $appointment_data
   *
   * @param array $book_data
   *
   * @return DvGAppointment
   */
  abstract public function book_appointment($appointment_data, $book_data);

  abstract public function get_dates_times($product_ids, $durations, $persons);

  protected function get_cache_expire() {
    return array();
  }
}
