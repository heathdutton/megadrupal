<?php

namespace Drupal\soauth\Common;

/**
 * Url manage helper class.
 * @author Raman
 */
class Url {
  
  /**
   * Uri.
   * @var string
   */
  private $uri;
  
  /**
   * An associative array of options.
   * @see https://api.drupal.org/api/drupal/includes%21common.inc/function/url/7
   * @var array
   */
  private $options;
  
  /**
   * Constructs a new Url object.
   * @param string $uri
   * @param array $options
   */
  public function __construct($uri, $options=array()) {
    $this->uri = $uri;
    $this->options = $options;
  }
  
  public function addQuery($name, $value) {
    $this->options['query'][$name] = $value;
  }
  
  /**
   * Returns the URI value for this Url object.
   * @return string
   */
  public function getUri() {
    return $this->uri;
  }
  
  /**
   * Gets a specific option.
   * @param string $name
   * @return mixed
   */
  public function getOption($name) {
    return (
      isset($this->options[$name]) ? $this->options[$name] : NULL
    );
  }
  
  /**
   * Returns the URL options.
   * @return array
   */
  public function getOptions() {
    return $this->options;
  }
  
  /**
   * Gets URL query parameters.
   * @return array
   */
  public function getQuery() {
    if (!isset($this->options['query'])) {
      return array();
    }
    return $this->options['query'];
  }
  
  /**
   * Sets a specific option.
   * @param string $name
   * @param mixed $value
   * @return Url
   */
  public function setOption($name, $value) {
    $this->options[$name] = $value;
    return $this;
  }
  
  /**
   * Sets the URL options.
   * @param array $options
   * @return Url
   */
  public function setOptions($options) {
    $this->options = $options;
    return $this;
  }
  
  /**
   * Sets the value of the absolute option for this Url.
   * @param bool $absolute
   * @return Url
   */
  public function setAbsolute($absolute=TRUE) {
    $this->options['absolute'] = $absolute;
    return $this;
  }
  
  /**
   * Set URL query field
   * @param array $query
   * @return Url
   */
  public function setQuery($query=array()) {
    $this->options['query'] = $query;
    return $this;
  }
  
  /**
   * Helper function to properly encode slashes in URLs.
   * It encodes forward-slashes as '%2F', which Drupal intentionally avoids in
   * its own url() function. Encoded slashes are necessary to prevent Service
   * from rejecting the return_uri property.
   * @param string $url
   * @return string
   */
  private function fixSlashes($url) {
    if (($pos = strpos($url, '?'))) {
      $path = substr($url, 0, $pos);
      $query = substr($url, $pos);
      $url = $path.str_replace('/', '%2F', $query);
    }
    return $url;
  }
  
  public function __toString() {
    return $this->fixSlashes(url($this->uri, $this->options));
  }
  
  /**
   * Creates new URL object from uri.
   * @param string $uri
   * @param array $options
   * @return Url
   */
  public static function fromUri($uri, $options=array()) {
    return new self($uri, $options);
  }
  
}
