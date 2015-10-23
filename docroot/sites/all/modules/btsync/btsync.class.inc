<?php
/**
 * @file
 * Core BitTorrent Sync API method class.
 */
class BTSyncMethod {

  /**
   * Protected variables.
   */
  protected $name = NULL;
  protected $description = NULL;
  protected $method = NULL;

  protected $arguments = array();
  protected $return = array();

  /**
   * Public variables.
   */
  public $response = array();
  public $settings = array();

  /**
   * Class contructor.
   */
  public function __construct($settings = array()) {
    if (empty($settings)) {
      $settings = variable_get('btsync_settings', array());
    }
    $this->settings = $settings;
  }

  /**
   * Class getter.
   */
  public function __get($property) {
    if (property_exists($this, $property)) {
      return $this->$property;
    }
  }

  /**
   * Send callback; Makes the request and returns the data.
   */
  public function send($data = array()) {
    // Build request URL.
    $url = url($this->settings['url'], array(
      'query' => $data + array('method' => $this->method)
    ));

    // Build cURL requiest.
    // Note: drupal_http_request() doesn't seem to work with the BT Sync API.
    $ch = curl_init();
    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_HTTPHEADER, array(
      'Authorization: Basic ' . base64_encode($this->settings['username'] . ':' . $this->settings['password']),
    ));
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);

    // Process request.
    $this->response = json_decode(curl_exec($ch));
    $info = curl_getinfo($ch);
    curl_close($ch);

    // Return the data.
    if ($info['http_code'] == 200 && !isset($this->response->error)) {
      return $this->response;
    }

    // ERROR
    return FALSE;
  }

}
