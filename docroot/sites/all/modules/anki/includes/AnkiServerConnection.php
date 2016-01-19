<?php
/**
 * @file
 * Contains AnkiServerConnection class.
 */

/**
 * Represents a connection to the AnkiServer.
 */
class AnkiServerConnection {
  private $base_url;

  /**
   * Construct a new AnkiServerConnection.
   *
   * @param string $base_url
   *   Absolute URL to the base of the Anki server's REST endpoint.
   */
  public function __construct($base_url) {
    $this->base_url = $base_url;
  }

  /**
   * Check if we can connect to the AnkiServer.
   *
   * @return bool
   *   Returns TRUE if we are able to connect; FALSE otherwise.
   */
  public function check() {
    try {
      $this->getServerVersion();
    }
    catch (AnkiServerException $e) {
      return FALSE;
    }
    
    return TRUE;
  }

  /**
   * Make a request to the AnkiServer.
   *
   * TODO: Give a link describing all the paths the AnkiServer accepts!
   *
   * @param string $path
   *   The path under the base URL to make the request.
   * @param array $args
   *   An associative array representing the arguments to pass with
   *   the request.
   *
   * @return array
   *   An associative array representing the response.
   */
  public function request($path, $args = array()) {
    $url = $this->base_url;
    if (substr($url, -1) != '/') {
      $url .= '/';
    }
    $url .= $path;

    $options = array(
      'headers' => array('Content-Type', 'application/json'),
      'method' => 'POST',
      'data' => json_encode((object)$args),
    );

    $resp = drupal_http_request($url, $options);

    if ($resp->code != 200) {
      throw new AnkiServerException($resp->code . (isset($resp->data) ? ': ' . $resp->data : ''), $resp);
    }

    return json_decode($resp->data);
  }

  /**
   * Get the Anki server version.
   *
   * @return string
   *   A string representing the Anki server version.
   */
  public function getServerVersion() {
    $resp = drupal_http_request($this->base_url);
    if ($resp->code == 200 && strpos($resp->data, 'AnkiServer') === 0) {
      $parts = explode(' ', $resp->data);
      if (preg_match('/\d+\.\d+\.\d+(?:\S+)?/', $parts[1])) {
        return $parts[1]; 
      }
    }

    throw new AnkiServerException(t("Unable to get server version"), $resp);
  }

  /**
   * Get a list of collections on the Anki server.
   *
   * @return array
   *   An array of names of collections.
   */
  public function listCollections() {
    return $this->request('list_collections');
  }

  /**
   * Get an AnkiServerCollection object.
   *
   * @param string $name
   *   The name of the collection.
   *
   * @return AnkiServerCollection
   *   An object representing the collection on the server.
   */
  public function getCollection($name) {
    return new AnkiServerCollection($this, $name);
  }
}
