<?php

/**
 * @file
 * Views handler override for field that exposes raw data.
 */


/**
 * @inheritdoc
 */
class architecture_php_views_handler extends views_handler_field {

  /**
  * Find any PHP in the body of this field.
  */
  function render($values) {
    if ($value = $this->get_value($values)) {
    $phpcode = '';
    while (strpos($value, '?>')) {
      $phpcode .= substr($value, strpos($value, '<?php')+5, (strpos($value, '?>') - (strpos($value, '<?php')+5)));
      $value = substr($value, strpos($value, '?>'));
      $value = substr($value, strpos($value, '<?php'));
      $phpcode .='<br />';
    }
    return $phpcode ? $phpcode : NULL;
  }
  return NULL;
  }
}
