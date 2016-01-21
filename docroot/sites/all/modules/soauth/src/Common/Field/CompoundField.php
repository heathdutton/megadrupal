<?php

namespace Drupal\soauth\Common\Field;

/**
 * CompoundField
 * @author Raman Liubimau <raman@cmstuning.net>
 */
class CompoundField implements DataField {
  
  /**
   * Format string
   * @see http://php.net/manual/ru/function.sprintf.php
   * @var string
   */
  private $format;
  
  /**
   * Array of fields
   * @var array
   */
  private $fields;

  /**
   * Construct
   * @param string $format
   * @param array $fields
   */
  public function __construct($format, $fields) {
    $this->format = $format;
    $this->fields = $fields;
  }
  
  public function get($data, $default='') {
    // Collect data from fields
    $args = array();
    
    foreach ($this->fields as $field) {
      $args[] = $field->get($data);
    }
    
    return vsprintf($this->format, $args);
  }
  
}
