<?php

namespace Drupal\soauth\Common\Field;


/**
 * SimpleField
 * @author Raman Liubimau <raman@cmstuning.net>
 */
class SimpleField implements DataField {
  
  /**
   * Path
   * @var array
   */
  private $path;
  
  /**
   * Construct field
   * @param string $path
   */
  public function __construct($path) {
    $this->path = explode('/', $path);
  }
  
  /**
   * Get field value
   * @param array $data
   * @param mixed default
   * @return mixed
   */
  public function get($data, $default=NULL) {
    return $this->extract($data, $this->path, $default);
  }
  
  /**
   * Get field path
   * @return array
   */
  public function getPath() {
    return $this->path;
  }
  
  /**
   * Extract field
   * @param array $data
   * @param array $path
   * @param mixed $default
   * @return mixed
   */
  private function extract($data, $path, $default) {
    // This function recursively walks over array using path passed as function
    // argument. If path is invalid, function returns default value.
    $key = array_shift($path);
    
    if (empty($path) && isset($data[$key])) {
      return $data[$key];
    } 
    elseif (!isset($data[$key])) {
      return $default;
    }
    
    return $this->extract($data[$key], $path, $default);
  }
  
}
