<?php

/**
 * @file
 * Views handler override for field that exposes available menus.
 */


/**
 * @inheritdoc
 */
class architecture_nodetype_menu_parent_views_handler extends views_handler_field {

  /**
  * Render the available menus for this nodetype.
  */
  function render($values) {
    $value = $this->get_value($values);
    $result = NULL;
    $options = menu_parent_options(menu_get_menus(), array('mlid' => 0));
    if(!empty($value)) {
      $menu_parent = variable_get('menu_parent_' . $value, 'main-menu:0');
      $result = $options[$menu_parent];
    }
    return $this->sanitize_value($result);
  }
}
