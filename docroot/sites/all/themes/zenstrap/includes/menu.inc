<?php

/**
 * Menu related themes
 */

/**
 * Overrides theme_menu_tree().
 */
function zenstrap_menu_tree(&$variables) {
  return '<ul class="nav">' . $variables['tree'] . '</ul>';
}

function zenstrap_menu_link(array $variables) {
  $element = $variables['element'];
  $main_menu = isset($element['#main_menu']);
  $sub_menu = '';
  if ($element['#below']) {
    if ($main_menu) {
      $element['#below']['#attributes']['class'][] = 'dropdown-menu';
    }
    $sub_menu = drupal_render($element['#below']);
    if ($main_menu) {
      $sub_menu = str_replace('nav', 'dropdown-menu', $sub_menu);
      $element['#attributes']['class'][] = 'dropdown';
      $element['#localized_options']['attributes']['class'][] = 'dropdown-toggle';
      $element['#localized_options']['attributes']['data-toggle'][] = 'dropdown';
      $element['#localized_options']['attributes']['data-target'][] = '#';
      $element['#title'] .= '<b class="caret"></b>';
      $element['#localized_options']['html'] = TRUE;
    }
  }
  if (!$main_menu && in_array('active-trail', $element['#attributes']['class']) &&
    strpos($sub_menu, 'active') === FALSE) {
    $element['#attributes']['class'][] = 'active';
  }

  $output = l($element['#title'], $element['#href'], $element['#localized_options']);
  return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";
}

function zenstrap_links__system_main_menu($links) {
  $output = drupal_render($links['links']);
  return $output;
}

function zenstrap_menu_local_tasks(&$variables) {
  $output = '';

  if (!empty($variables['primary'])) {
    $variables['primary']['#prefix'] = '<h2 class="element-invisible">' . t('Primary tabs') . '</h2>';
    $variables['primary']['#prefix'] .= '<ul class="nav nav-tabs tabs">';
    $variables['primary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['primary']);
  }
  if (!empty($variables['secondary'])) {
    $variables['secondary']['#prefix'] = '<h2 class="element-invisible">' . t('Secondary tabs') . '</h2>';
    $variables['secondary']['#prefix'] .= '<ul class="nav nav-tabs tabs">';
    $variables['secondary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['secondary']);
  }

  return $output;
}
