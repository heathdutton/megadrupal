<?php

/**
 * @file
 * Views handler override for field that exposes role permissions.
 */


/**
 * @inheritdoc
 */
class architecture_role_permissions_views_handler extends views_handler_field {
  function render($values) {
    $value = $this->get_value($values);

    return $value;
  }
}
