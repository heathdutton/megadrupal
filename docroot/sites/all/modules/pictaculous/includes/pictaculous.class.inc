<?php
/**
 * @file
 * Contains Pictaculous API class.
 */

class Pictaculous {

  protected $apiUrl;

  /**
   * Class constructor function.
   */
  public function __construct($api_url) {
    $this->apiUrl = trim($api_url, '/');
  }

  /**
   * Returns information based on the given version of the API.
   *
   * @param array $postfields
   *   The file contents of an uploaded file.
   * @return array
   *   API return value or NULL.
   */
  public function api($postfields) {
    return $this->call('1.0', $postfields);
  }

	/**
	 * Call the API
	 *
	 * @param string $version
	 *   The endpoint to call.
	 * @param array $postfields
	 *   The values to pass into the API
	 * @return array
	 *   API return value or NULL.
	 */
  private function call($version, $postfields = array()) {
    if ($response = drupal_http_request(url($this->apiUrl . '/' . $version . '/', array(
      'absolute' => TRUE
    )), array(
      'headers' => array(
        'Content-Type' => 'application/x-www-form-urlencoded'
      ),
      'method' => 'POST',
      'data' => drupal_http_build_query(array_merge($postfields, array(
        'format' => 'json'
      ))),
    ))) {
      if ($response->code == 200) {
        return json_decode($response->data);
      }
    }
    return NULL;
  }

}
