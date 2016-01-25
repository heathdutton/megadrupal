<?php
/**
 * @file
 * Class for accessing Emediate backend
 */

class Emediate {

  private $username;
  private $password;
  private $url;
  public $format = 'xml';

  /**
   * Constructor for Emediate object.
   *
   * @param string $url
   *   URL for API backend.
   * @param string $username
   *   Username for API backend.
   * @param string $password
   *   Password for API backend.
   *
   * @return object
   *   This.
   */
  public function __construct($url = NULL, $username = NULL, $password = NULL) {
    if (isset($url)) {
      $this->setEndpoint($url);
    }

    if (isset($username)) {
      $this->setCredentials($username, $password);
    }

    return $this;
  }

  /**
   * Convert from UTF-8 to Latin1 if applicable.
   */
  static public function UTF8ToLatin1($item) {
    return mb_detect_encoding($item) == 'UTF-8' ? utf8_decode($item) : $item;
  }

  /**
   * Callback function for URL construct.
   */
  private function UTF8ToLatin1Callback(&$item, $key = NULL) {
    $item = self::UTF8ToLatin1($item);
  }

  /**
   * Construct url with parameters.
   *
   * @param string $url
   *   URL to construct.
   * @param string $options
   *   Options for http_build_query().
   *
   * @return string
   *   Constructed URL.
   */
  private function constructUrl($url, $options) {
    array_walk_recursive($options, array(__CLASS__, 'UTF8ToLatin1Callback'));
    return $url . '?' . http_build_query($options, NULL, '&');
  }

  /**
   * Set service end point.
   *
   * @param string $url
   *   URL of API endpoint.
   */
  public function setEndpoint($url) {
    $this->url = $url;
  }

  /**
   * Set username and password.
   *
   * @param string $username
   *   API username.
   * @param string $password
   *   API password.
   */
  public function setCredentials($username, $password) {
    $this->username = $username;
    $this->password = $password;
  }

  /**
   * Call the Emediate service.
   *
   * @param string $cmd
   *   Emediate API command.
   * @param array $options
   *   Options for the command
   */
  public function callService($cmd, $options = array()) {
    $options['cmd'] = $cmd;
    if (!isset($options['format'])) {
      $options['format'] = $this->format;
    }
    $options['username'] = $this->username;
    $options['password'] = $this->password;

    $url = $this->constructUrl($this->url, $options);
    $response = drupal_http_request($url);

    if (isset($response)) {
      $xml = simplexml_load_string($response->data);
      return $xml;
    }
  }

  /**
   * Add a content unit.
   */
  public function AddContentUnit($options) {
    $options['what'] = 'cu';
    return $this->callService('add', $options);
  }

  /**
   * Add a section.
   */
  public function addSection($options) {
    $options['what'] = 'section';
    return $this->callService('add', $options);
  }

  /**
   * Add a category group.
   */
  public function addCategoryGroup($options) {
    $options['what'] = 'categoryGroup';
    return $this->callService('add', $options);
  }

  /**
   * Add a category.
   */
  public function addCategory($options) {
    $options['what'] = 'category';
    return $this->callService('add', $options);
  }

  /**
   * Search for a section.
   */
  public function searchSection($options) {
    $options['what'] = 'section';
    return $this->callService('search', $options);
  }

  /**
   * Retrieve categories.
   */
  public function retrieveCategories($options) {
    $options['what'] = 'category';
    return $this->callService('retrieve', $options);
  }

  /**
   * Search for content units.
   */
  public function searchContentUnits($options) {
    $options['what'] = 'cu';
    return $this->callService('search', $options);
  }

  /**
   * Delete a category.
   */
  public function deleteCategory($options) {
    $options['what'] = 'category';
    return $this->callService('delete', $options);
  }

};
