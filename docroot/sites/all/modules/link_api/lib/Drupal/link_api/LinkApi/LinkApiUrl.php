<?php
/**
 * @file
 * Class description for a URL class
 *
 * @copyright Copyright(c) 2012 Christopher Skene
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at xtfer dot com
 */

namespace Drupal\link_api\LinkApi;

/**
 * A class for managing individual URLs.
 */
class LinkApiUrl {

  /**
   * The URL parts, as returned by parse_url
   *
   * @see parse_url
   *
   * @var array()
   */
  protected $parts = array();

  /**
   * The Full URL
   */
  protected $url;

  /**
   * Hash information.
   *
   * @var array
   */
  protected $md5 = array();

  /**
   * Constructor.
   *
   * @param string $url
   *   (optional) A URL to load
   * @param bool|string $flag
   *   (optional) An optional flag to pass to the loadURL function. See
   *   LinkApiUrl::loadURL for options.
   */
  public function __construct($url = NULL, $flag = LINK_API_URL_NORMALISE) {
    if (isset($url) && !empty($url)) {
      return $this->loadURL($url, $flag);
    }

    return FALSE;
  }

  /**
   * Make a URL with different configuration options, and sanitize it.
   *
   * @param string $method
   *   (optional) A constant defining the parts to use to build the link.
   *   Can be either:
   *    - LINK_API_URL_BUILD_FULL (default),
   *    - LINK_API_URL_BUILD_PARTIAL, or
   *    - LINK_API_URL_BUILD_PATH.
   * @param array $keys
   *   (optional) If specified, $method is ignored, and only the keys
   *   specified are used.
   * @param bool $validate
   *   (optional) Whether to validate the URL
   *
   * @return string
   *   A URL string
   */
  public function getUrl($method = LINK_API_URL_BUILD_FULL, $keys = array(), $validate = FALSE) {
    $final_parts = array();

    if (empty($keys)) {
      switch ($method) {
        case LINK_API_URL_BUILD_PARTIAL:
          $keys = array('host', 'path', 'query', 'fragment');
          break;

        case LINK_API_URL_BUILD_ABSOLUTE:
          $keys = array('path', 'query', 'fragment');
          break;

        case LINK_API_URL_BUILD_PATH:
          $keys = array('path');
          break;

        case LINK_API_URL_BUILD_FULL:
          $keys = array(
            'scheme',
            'host',
            'port',
            'user',
            'pass',
            'path',
            'query',
            'fragment',
          );
          break;

      }
    }

    // Pull out keys from $this for use in the build process.
    if (!empty($keys)) {
      foreach ($keys as $key) {
        if ($this->getPart($key)) {
          $final_parts[$key] = $this->getPart($key);
        }
      }

      // Build the URL.
      $url = link_api_helper()->buildURL($final_parts);
      $url = link_api_helper()->sanitizeURL($url);

      // If validation is requested, run it now.
      if ($validate == TRUE) {
        $valid = link_api_helper()->validateURL($url);
        if ($valid == FALSE) {
          return FALSE;
        }
      }

      return $url;
    }

    return FALSE;
  }

  /**
   * Load a URL into this object.
   *
   * @todo: UTF-8 chars may still fail
   * see https://bugs.php.net/bug.php?id=52923
   *
   * @param string $url
   *   A URL
   * @param string $flag
   *   An optional flag to pass to the loadURL function.
   *    LINK_API_URL_NORMALISE  Normalise the link (default)
   *    LINK_API_URL_NORMALISE_NONE Do not normalise the link
   *
   * @return bool
   *   TRUE, if the URL was successfully loaded
   */
  public function loadURL($url, $flag = LINK_API_URL_NORMALISE) {

    // Sanitize the URL before loading it, in case its malicious.
    $url = LinkApiHelper::sanitizeURL($url);
    if ($url == FALSE) {
      return FALSE;
    }

    // @todo This results in a double parse_url() being called
    // Normalise if required.
    if ($flag == LINK_API_URL_NORMALISE) {
      $url = link_api_helper()->normaliseURL($url);
    }

    // Do the replacement.
    $this->url = $url;
    $this->parts = parse_url($url);

    return $this;
  }

  /**
   * Compute the URL hash for this link.
   */
  public function hash() {
    $this->md5['full'] = LinkApiHelper::hash($this->getUrl(LINK_API_URL_BUILD_FULL));
    $this->md5['partial'] = LinkApiHelper::hash($this->getUrl(LINK_API_URL_BUILD_PARTIAL));
    $this->md5['uri'] = LinkApiHelper::hash($this->getUrl(LINK_API_URL_BUILD_PATH));
  }

  /**
   * Get the URI's hash value.
   *
   * @param string $type
   *   Type of hash. Possible values are full, partial and URI
   *
   * @return string|bool
   *   The hash value, or false.
   */
  public function getHash($type = 'full') {
    if (empty($this->md5)) {
      $this->hash();
    }

    if (isset($this->md5[$type])) {
      return $this->md5[$type];
    }

    return FALSE;
  }

  /**
   * Set a property.
   */
  public function setPart($key, $value = NULL) {

    if (is_integer($key)) {
      $key = $this->constantToString($key);
    }

    if (empty($key)) {
      return FALSE;
    }

    $this->parts[$key] = $value;

    return $this;
  }

  /**
   * Get a property.
   *
   * Specify one of PHP_URL_SCHEME, PHP_URL_HOST, PHP_URL_PORT, PHP_URL_USER,
   * PHP_URL_PASS, PHP_URL_PATH, PHP_URL_QUERY or PHP_URL_FRAGMENT to retrieve
   * just a specific URL component as a string (except when PHP_URL_PORT is
   * given, in which case the return value will be an integer).
   *
   * @see parse_url()
   *
   * @param string|int $key
   *   The property to return.
   *
   * @return mixed
   *   The property or FALSE.
   */
  public function getPart($key) {

    if (is_integer($key)) {
      $key = $this->constantToString($key);
    }

    if (empty($key)) {
      return FALSE;
    }

    if (array_key_exists($key, $this->parts) && !empty($this->parts[$key])) {
      return $this->parts[$key];
    }

    return FALSE;
  }

  /**
   * Get all parts.
   *
   * An associative array is returned. At least one element will be present
   * within the array. Potential keys within this array are:
   *  - scheme - e.g. http
   *  - host
   *  - port
   *  - user
   *  - pass
   *  - path
   *  - query - after the question mark ?
   *  - fragment - after the hashmark #
   *
   * If the component parameter is specified, this returns a string (or an
   * integer, in the case of PHP_URL_PORT) instead of an array. If the requested
   * component doesn't exist within the given URL, NULL will be returned.
   *
   * @see parse_url()
   *
   * @param string|int $parameter
   *   An optional part value, either as a string (see below for values, or
   *   a PHP URL constant (e.g. PHP_URL_SCHEME).
   *
   * @return array|string|int
   *   The values requested.
   */
  public function getParts($parameter = NULL) {

    if (!empty($parts)) {
      return $this->getPart($parameter);
    }

    return $this->parts;
  }

  /**
   * Get the current link's path as an array.
   *
   * @return array|bool
   *   An array of path parts, if a path is set, or FALSE.
   */
  public function getPathArray() {
    if ($this->getPart('path')) {
      return LinkApiHelper::pathArray($this->getPart('path'));
    }

    return FALSE;
  }

  /**
   * Convert a PHP URL constant into our local part key.
   *
   * @param int $key
   *   The key to look for.
   *
   * @return string|bool
   *   The local key, or FALSE.
   */
  protected function constantToString($key) {
    $map = array(
      PHP_URL_SCHEME => 'scheme',
      PHP_URL_HOST => 'host',
      PHP_URL_PORT => 'port',
      PHP_URL_USER => 'user',
      PHP_URL_PASS => 'pass',
      PHP_URL_PATH => 'path',
      PHP_URL_QUERY => 'query',
      PHP_URL_FRAGMENT => 'fragment',
    );

    if (array_key_exists($key, $map)) {
      return $map[$key];
    }

    return FALSE;
  }
}
