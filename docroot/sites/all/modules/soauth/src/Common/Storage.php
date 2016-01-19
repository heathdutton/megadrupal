<?php

namespace Drupal\soauth\Common;


/**
 * Class Storage
 * Thin wrapper over Drupal's variable_ functions.
 * @author Raman Liubimau <raman@cmstuning.net>
 */
class Storage {
  
  /**
   * Prefix
   * @var string
   */
  private $prefix;
  
  /**
   * Construct new instance
   * @param string $prefix
   */
  public function __construct($prefix='') {
    $this->prefix = $prefix;
  }
  
  /**
   * Get a persistent variable
   * @param string $name
   * @param mixed $default
   * @return mixed
   */
  public function get($name, $default=NULL) {
    return variable_get($this->getIndex($name), $default);
  }
  
  /**
   * Set a persistent variable
   * @param string $name
   * @param mixed $value
   */  
  public function set($name, $value) {
    variable_set($this->getIndex($name), $value);
  }
  
  /**
   * Store several values
   * @param array $values
   */
  public function map($values) {
    foreach ($values as $name => $value) {
      $this->set($name, $value);
    }
  }
  
  /**
   * Unset a persistent variable
   * @param string $name
   */
  public function del($name) {
    variable_del($this->getIndex($name));
  }
  
  /**
   * Get name prefix
   * @return string
   */
  public function getPrefix() {
    return $this->prefix;
  }
  
  /**
   * Get unique index
   * @param string $name
   * @return string
   */  
  protected function getIndex($name) {
    if ($this->prefix) {
      return implode('_', array($this->prefix, $name));
    }
    return $name;
  }
}
