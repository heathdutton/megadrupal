<?php

/**
 * @file
 * Provides tokens for menu link.
 */

/**
 * Implements hook_tokens().
 */
function breadcrumb2_tokens($type, $tokens, array $data = array(), array $options = array()) {
  if ($type == 'menu_link') {
    if(isset($data['menu_link'])) {
      $replacements = array();
      $menu_link = $data['menu_link'];
      foreach ($tokens as $name => $original) {
        switch ($name) {
          case 'title':
            $replacements[$original] = $menu_link['title'];
            break;
          case 'href':
            $replacements[$original] = $menu_link['href'];
            break;
          case 'link_path':
            $replacements[$original] = $menu_link['link_path'];
            break;
          case 'localized_options':
            $replacements[$original] = $menu_link['localized_options'];
            break;
          case 'type':
            $replacements[$original] = $menu_link['type'];
            break;
        }
      }
      return $replacements;
    }
  }
}
