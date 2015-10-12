<?php

/**
 * @file
 * Contains the theme function overrides for menu.
 */

/**
 * Adding menu name class to menu items, adding menu item depth ass class.
 */
function circle_preprocess_menu_link(&$variables) {
  $element = $variables['element'];
  $original_link = $element['#original_link'];
  // Add menu depth to item class.
  $depth = 'depth-' . $original_link['depth'];
  // Add menu name to item class.
  $menu_name = 'item-' . $original_link['menu_name'];
  $link_menu_name = 'link-' . $original_link['menu_name'];

  $variables['element']['#localized_options']['attributes']['class'][] = $link_menu_name;

  $variables['element']['#attributes']['class'][] = $depth;
  $variables['element']['#attributes']['class'][] = $menu_name;
}

/**
 * Custom function overriding Drupal default menu tree preprocess.
 */
function _circle_preprocess_menu_tree(&$variables) {
  $variables['menu_name'] = '';
  if (is_array($variables['tree'])) {
    foreach ($variables['tree'] as $item) {
      if (isset($item['#original_link'])) {
        if (isset($item['#original_link']['menu_name'])) {
          $variables['menu_name'] = $item['#original_link']['menu_name'];
          break;

        }
      }
    }
    $variables['tree'] = $variables['tree']['#children'];
  }
}

/**
 * Implements hook_menu_tree().
 */
function circle_menu_tree($variables) {
  $menu_class = 'menu';
  if (isset($variables['menu_name']) && $variables['menu_name']) {
    $menu_class .= ' menu-' . $variables['menu_name'];
  }
  return '<ul class="' . $menu_class . '">' . $variables['tree'] . '</ul>';
}
