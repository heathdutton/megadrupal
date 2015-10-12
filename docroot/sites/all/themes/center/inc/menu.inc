<?php
/**
 * @file
 * Menu template functions.
 */

/**
 * Overrides theme_menu_tree().
 */
function center_menu_tree($variables) {
  return '<ul class="nav">' . $variables['tree'] . '</ul>';
}

/**
 * Custom implementation of theme_menu_link().
 *
 * This adds a span for including icons before a menu link.
 */
function center_menu_link__icon(array $variables) {
  $element = $variables['element'];
  $sub_menu = '';

  // Prevent the <span> tag from being escaped.
  $element['#localized_options']['html'] = TRUE;
  if ($element['#below']) {
    $sub_menu = drupal_render($element['#below']);
  }
  $icon = '<span' . drupal_attributes($element['#icon_attributes']) . '></span>';
  $output = l($icon . $element['#title'], $element['#href'], $element['#localized_options']);

  return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";
}

/**
 * Overrides theme_menu_local_tasks().
 */
function center_menu_local_tasks(&$variables) {
  $output = '';
  if (!empty($variables['primary'])) {
    $variables['primary']['#prefix'] = '<h2 class="element-invisible">' . t('Primary tabs') . '</h2>';
    $variables['primary']['#prefix'] .= '<ul class="nav nav-tabs">';
    $variables['primary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['primary']);
  }
  if (!empty($variables['secondary'])) {
    $variables['secondary']['#prefix'] = '<h2 class="element-invisible">' . t('Secondary tabs') . '</h2>';
    $variables['secondary']['#prefix'] .= '<ul class="nav nav-pills">';
    $variables['secondary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['secondary']);
  }

  return $output;
}
