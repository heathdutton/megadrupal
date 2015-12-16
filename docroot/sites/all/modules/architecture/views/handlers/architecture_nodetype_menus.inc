<?php

/**
 * @file
 * Views handler override for field that exposes available menus.
 */


/**
 * @inheritdoc
 */
class architecture_nodetype_menus_views_handler extends views_handler_field {

  /**
  * Render the available menus for this nodetype.
  */
  function render($values) {
    $value = $this->get_value($values);
    $menu_list = '';
    if(!empty($value)) {
      $menus = variable_get('menu_options_' . $value);
      $count = 0;
      if (!empty($menus)) {
        foreach ($menus as $menu) {
          $menu_list .= $menu;
          $count++;
          if ($count < (count($menus))) {
            $menu_list .= ', ';
          }
        }
      }
    }
    return $this->sanitize_value($menu_list);
  }
}
