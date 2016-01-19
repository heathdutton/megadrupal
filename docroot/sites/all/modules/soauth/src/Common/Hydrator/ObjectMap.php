<?php

namespace Drupal\soauth\Common\Hydrator;


/**
 * Class ObjectMap
 * @author Raman Liubimau <raman@cmstuning.net>
 */
class ObjectMap {
  
  /**
   * Hydration map
   * @var array
   */
  private $map;
  
  /**
   * Construct ObjectMap
   * @param array $map
   */
  public function __construct($map) {
    $this->map = $map;
  }
  
  /**
   * Hydrate object
   * @param object|array $obj
   * @param object|array $data
   * @return mixed
   */
  public function hydrate($obj, $data) {
    foreach ($this->map as $key => $field) {
      // Key may be compound
      $value = array();
      
      foreach (explode('+', $key) as $subkey) {
        if (isset($data[$subkey])) {
          $value[] = $data[$subkey];
        }
      }
      
      $obj->{$field} = array(
        LANGUAGE_NONE => array(
          array('value' => implode(' ', $value))));
    }
  }
}
