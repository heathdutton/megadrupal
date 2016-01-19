<?php

namespace Drupal\soauth\Common;


/**
 * Class Session
 * @author Raman Liubimau <raman@cmstuning.net>
 */
class Session {
  
  /**
   * Prefix
   * @var string
   */
  private $prefix;
  
  /**
   * Create new instance
   * @param string $prefix
   */
  public function __construct($prefix='') {
    $this->prefix = $prefix;
  }
  
  /**
   * Get a session variable
   * @param string $name
   * @param mixed $default
   * @return mixed
   */
  public function get($name, $default=NULL) {
    if ($this->has($name)) {
      return $_SESSION[$this->prefix][$name];
    }
    return $default;
  }
  
  /**
   * Set a session variable
   * @param string $name
   * @param mixed $value
   */
  public function set($name, $value) {
    $_SESSION[$this->prefix][$name] = $value;
    return $this;
  }
  
  /**
   * Unset a session variable
   * @param string $name
   */
  public function del($name) {
    unset($_SESSION[$this->prefix][$name]);
  }
  
  /**
   * Check if a session has variable
   * @param string $name
   * @return bool
   */
  public function has($name) {
    return isset($_SESSION[$this->prefix][$name]);
  }
  
  /**
   * Get name prefix
   * @return string
   */
  public function getPrefix() {
    return $this->prefix;
  }
  
  /**
   * Get default session.
   * Uses global namespace.
   * @return Session
   */
  static public function getDefault() {
    return new self();
  }
}
