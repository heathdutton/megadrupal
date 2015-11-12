<?php

/**
 * @file
 * gratis custom menu functions.
 *
 * If you need to add or modify functions do it in your sub-theme.
 */

/**
 * Theme wrapper function for the primary menu links.
 */
function gratis_menu_tree__primary(&$vars) {
  return '<ul class="menu primary">' . $vars['tree'] . '</ul>';
}

/**
 * Returns HTML for a menu link and submenu.
 */
function gratis_menu_link(array $vars) {
  $element = $vars['element'];
  $sub_menu = '';
  $name_id = strtolower(strip_tags($element['#title']));
  // Remove colons and anything past colons.
  // Preserve alphanumerics, everything else goes away.
  $pattern = '/[^a-z]+/ ';
  $name_id = preg_replace($pattern, '', $name_id);
  // li.
  $element['#attributes']['class'][] = 'menu-' . $element['#original_link']['mlid'] . ' ' . $name_id;
  // li > a.
  $element['#localized_options']['attributes']['class'][] = 'alink menu-' . $vars['element']['#original_link']['mlid'] . ' ' . $name_id;
  // Add levels.
  $element['#attributes']['class'][] = 'level-' . $element['#original_link']['depth'];

  if ($element['#below']) {
    $sub_menu = drupal_render($element['#below']);
  }

  $output = l($element['#title'], $element['#href'], $element['#localized_options']);
  return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";
}
