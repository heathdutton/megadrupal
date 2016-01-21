<?php

/**
 * @file
 * Kred API PHP implementation.
 *
 * @version 1.0
 */

class Kred {

  protected $kredUrl;
  protected $kredAppId;
  protected $kredAppKey;
  protected $kredSource;

  /**
   * Constructor for Kred class.
   *
   * @param string $app_id
   *   Kred's app id.
   *
   * @param string $app_key
   *   Kred's app key.
   *
   * @param string $source
   *   Kred's source to fetch information.
   */
  public function __construct($app_id, $app_key, $source) {
    $this->kredUrl = 'http://api.kred.com/';
    $this->kredAppId = $app_id;
    $this->kredAppKey = $app_key;
    $this->kredSource = $source;
  }

  /**
   * Create an URL to connect to the Kred's API.
   *
   * @param string $action
   *   Action to execute on Kred API.
   *
   * @param array $users
   *   An array of account IDs.
   *
   * @return string
   *   URL to connect to the Kred's API.
   */
  protected function createUrl($action, $users) {

    if (empty($action) || !is_array($users) || empty($users)) {
      return '';
    }

    $term = implode(',', $users);

    // Create query string.
    $query_string  = "term=" . urlencode($term);
    $query_string .= "&source=" . urlencode($this->kredSource);
    $query_string .= "&app_id=" . $this->kredAppId;
    $query_string .= "&app_key=" . $this->kredAppKey;

    return $this->kredUrl . $action . '?' . $query_string;
  }

  /**
   * Connects to Kred API and fetch information.
   *
   * @param string $url
   *   Kred's URL.
   *
   * @return array
   *   Kred's API response.
   */
  protected function connect($url) {
    $result = array();

    try {
      if (empty($url)) {
        watchdog('kred', 'Cannot connect or authentice with the Kred API', WATCHDOG_ERROR);
        return $result;
      }

      $curl_handle = curl_init();
      curl_setopt($curl_handle, CURLOPT_URL, $url);
      curl_setopt($curl_handle, CURLOPT_RETURNTRANSFER, 1);
      curl_setopt($curl_handle, CURLOPT_CONNECTTIMEOUT, 5);
      curl_setopt($curl_handle, CURLOPT_TIMEOUT, 10);
      $response = curl_exec($curl_handle);
      curl_close($curl_handle);

      if (!empty($response)) {
        $response = json_decode($response);

        if (isset($response) && isset($response->data)) {
          return $response->data;
        }
      }

      return $result;
    }
    catch (Exception $e) {
      watchdog('kred', '!message', array('!message' => $e->__toString()), WATCHDOG_ERROR);
    }
  }

  /**
   * Returns Kred Influence + Outreach by community for any Twitter @name.
   *
   * @param array $users
   *   An array of account IDs.
   *
   * @return array
   *   Array with data from Kred API.
   *
   * @see https://developer.peoplebrowsr.com/kred/kredscore
   */
  public function getKredScore($users) {
    $url = $this->createUrl('kredscore', $users);
    return $this->connect($url);
  }

  /**
   * Returns Kred Influence Score + Outreach Level for all communities.
   *
   * @param array $users
   *   An array of account IDs.
   *
   * @return array
   *   Array with data from Kred API.
   *
   * @see https://developer.peoplebrowsr.com/kred/kredcommunity
   */
  public function getKredCommunityScore($users) {
    $url = $this->createUrl('kred', $users);
    return $this->connect($url);
  }

  /**
   * Kredentials presents the online history of a user.
   *
   * @param array $users
   *   An array of account IDs.
   *
   * @return array
   *   Array with data from Kred API.
   *
   * @see https://developer.peoplebrowsr.com/kred/kredentials
   */
  public function getKredentials($users) {
    $url = $this->createUrl('kredentials', $users);
    return $this->connect($url);
  }
}
