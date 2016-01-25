<?php
/**
 * @file
 * Basic library wrapper plugin.
 *
 * @copyright Copyright(c) 2012 Christopher Skene
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at xtfer dot com
 */

namespace Drupal\ldt\Library;

/**
 * Defines a library wrapper for LDT.
 */
class LibraryWrapper {

  /**
   * The stored graph provided by the plugin.
   */
  public $graph;

  /**
   * The loaded resources.
   *
   * @var array
   */
  public $resources = array();

  /**
   * The source data, prior to parsing.
   *
   * @var mixed
   */
  protected $unparsedData;

  /**
   * A default language value.
   *
   * @var string
   */
  protected $lang;

  /**
   * The data format.
   *
   * @var string
   */
  protected $format;

  /**
   * Store a resource locally for reuse.
   *
   * @param string $uri
   *   The URI of the resource.
   * @param mixed $data
   *   The data for the resource.
   * @param bool $cache
   *   (optional) If FALSE, do not cache this resource.
   */
  public function setResource($uri, $data, $cache = TRUE) {

    $this->resources[$uri] = $data;

    // Store this resource in the caching layers in case of reuse.
    if ($cache == TRUE) {
      $this->setCachedResource($uri, $data);
    }
  }

  /**
   * Retrieve a resource from this object.
   *
   * This does not retrieve the resource from the cache, as it is implicit that
   * the resource must already be available.
   *
   * @param string $uri
   *   The resource URI
   *
   * @return mixed
   *   The resource, or FALSE.
   */
  public function getResource($uri) {

    if (isset($this->resources[$uri])) {
      return $this->resources[$uri];
    }

    return FALSE;
  }

  /**
   * Load a resource from the cache into this object.
   *
   * This can be used to check if a resource exists locally before attempting an
   * expensive request to retrieve it.
   *
   * @param string $uri
   *   The URI of the resource.
   *
   * @return bool|array
   *   FALSE if the resource is not successfully loaded.
   */
  public function loadResource($uri) {

    $resource = $this->getCachedResource($uri);
    if (!empty($resource)) {
      $this->resources[$uri] = $resource;
      return $resource;
    }

    return FALSE;
  }

  /**
   * Set a resource in the internal cache.
   *
   * @param string $uri
   *   The resource URI.
   * @param mixed $data
   *   The resource data.
   */
  protected function setCachedResource($uri, $data) {

    // Store in the static cache.
    $resource_cache = &drupal_static('ldt_resources', array());
    $resource_cache[$uri] = $data;

    // Store in the persistent cache.
    cache_set($uri, $data, LDT_DEFAULT_CACHE);
  }

  /**
   * Get a resource from the internal cache.
   *
   * @param string $uri
   *   The resource URI.
   *
   * @return mixed
   *   The result of the request, or FALSE.
   */
  protected function getCachedResource($uri) {

    if (!is_string($uri)) {
      return FALSE;
    }

    // Check the static cache first.
    $resource_cache = &drupal_static('ldt_resources', array());
    if (isset($resource_cache[$uri])) {
      return $resource_cache[$uri];
    }

    // Check the persistent cache.
    $result = cache_get($uri, LDT_DEFAULT_CACHE);
    if (is_object($result) && isset($result->data)) {
      return $result->data;
    }

    return FALSE;
  }

  /**
   * Set the language parameter.
   *
   * @param string $lang
   *   A valid language string, e.g. 'en'.
   */
  public function setLang($lang) {

    $this->lang = $lang;
  }

  /**
   * Get the current language.
   *
   * Defaults to 'en'.
   *
   * @return string
   *   A language string.
   */
  public function getLang() {
    if (isset($this->lang)) {
      return $this->lang;
    }

    return 'en';
  }

  /**
   * Set the format.
   *
   * @param string $format
   *   The format name.
   */
  public function setFormat($format) {

    $this->format = $format;
  }

  /**
   * Get the format.
   *
   * If not set, this will return the default format.
   *
   * @return string
   *   The format name.
   */
  public function getFormat() {
    if (isset($this->format)) {
      return $this->format;
    }

    return LDT_DEFAULT_FORMAT;
  }

  /**
   * Set the source data.
   *
   * @param mixed $unparsed_data
   *   Source data, usually as a string.
   */
  public function setUnparsedData($unparsed_data) {

    $this->unparsedData = $unparsed_data;
  }

  /**
   * Get the source data.
   *
   * @return mixed
   *   The source data, usually a string.
   */
  public function getUnparsedData() {
    if (property_exists($this, 'unparsedData')) {
      return $this->unparsedData;
    }
    return NULL;
  }
}
