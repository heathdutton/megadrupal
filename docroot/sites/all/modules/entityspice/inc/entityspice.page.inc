<?php

/**
 * Menu callback. See entityspice_menu() for more information.
 *
 * Purpose: List available entity types created using ES to add a new entity.
 */
function _entityspice_default_page() {
  $item = menu_get_item();
  $content = system_admin_menu_block($item);

  // Bypass the entity/add listing if only one entity bundle is available.
  // if (count($content) == 1) {
  //  $item = array_shift($content);
  //  drupal_goto($item['href']);
  // }

  $types = _entityspice_getinfo_all();
  $vars = array('content' => $content, 'types' => array_keys($types));
  $output = theme('entityspice_default_page', $vars);

  return $output;
}
