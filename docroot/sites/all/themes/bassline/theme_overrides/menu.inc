<?php
/**
 * @file
 * Menu overrides.
 */

/**
 * Override theme_menu_tree().
 *
 * Remove class menu.
 */
function bassline_menu_tree($variables) {
  return '<ul>' . $variables['tree'] . '</ul>';
}

/**
 * Override theme_menu_link().
 *
 * Remove the leaf class.
 */
function bassline_menu_link(array $vars) {
  $element = $vars['element'];
  $sub_menu = '';

  // Remove the leaf class/
  $element['#attributes']['class'] = array_diff($element['#attributes']['class'], array('leaf'));
  // Remove the class array if empty.
  if (empty($element['#attributes']['class'])) {
    unset($element['#attributes']['class']);
  }

  if ($element['#below']) {
    $sub_menu = drupal_render($element['#below']);
  }

  // Use our menu title if localized_options title is empty.
  if (empty($element['#localized_options']['attributes']['title'])) {
    $element['#localized_options']['attributes']['title'] = $element['#title'];
  }

  $output = l($element['#title'], $element['#href'], $element['#localized_options']);
  return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";
}

/**
 * Override theme_menu_local_tasks().
 *
 * Remove the leaf class.
 */
function bassline_menu_local_tasks(&$vars) {
  $output = '';

  if (!empty($vars['primary'])) {
    $vars['primary']['#prefix'] = '<p class="element-invisible">' . t('Primary tabs') . '</p>';
    $vars['primary']['#prefix'] .= '<ul class="tabs primary">';
    $vars['primary']['#suffix'] = '</ul>';
    $output .= drupal_render($vars['primary']);
  }
  if (!empty($vars['secondary'])) {
    $vars['secondary']['#prefix'] = '<p class="element-invisible">' . t('Secondary tabs') . '</p>';
    $vars['secondary']['#prefix'] .= '<ul class="tabs secondary">';
    $vars['secondary']['#suffix'] = '</ul>';
    $output .= drupal_render($vars['secondary']);
  }

  return $output;
}
