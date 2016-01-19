<?php

namespace Drupal\soauth\Common\Field;

/**
 * DataField
 * @author Raman Liubimau <raman@cmstuning.net>
 */
interface DataField {
  
  /**
   * Get field value
   * @param array $data
   * @param mixed default
   * @return mixed
   */
  public function get($data, $default='');
  
}
