<?php

/**
 * @file
 * Page callbacks for menu_tag module.
 */

/**
 * Menu callback; Retrieve a JSON object containing autocomplete suggestions 
 * for existing menu tags.
 */
function menu_tag_autocomplete($string = '') {
  $matches = array();
  if ($string) {
    $result = db_select('menu_tag')
      ->fields('menu_tag', array('menu_tag'))
      ->condition('menu_tag', db_like($string) . '%', 'LIKE')
      ->distinct()
      ->range(0, 10)
      ->execute();
    foreach ($result as $menu_tag) {
      $matches[$menu_tag->menu_tag] = check_plain($menu_tag->menu_tag);
    }
  }

  drupal_json_output($matches);
}
