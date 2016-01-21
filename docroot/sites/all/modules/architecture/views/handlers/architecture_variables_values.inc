<?php

/**
 * @file
 * Views handler override for field that exposes variable values.
 */


/**
 * @inheritdoc
 */
class architecture_variable_value_views_handler extends views_handler_field {

  /**
  * Render the available menus for this nodetype.
  */
  function render($values) {
    $value = $this->get_value($values);
    return ('<pre>' . print_r(unserialize($value), 1) . '</pre>');
  }
}
