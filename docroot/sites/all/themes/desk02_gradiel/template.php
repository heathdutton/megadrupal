<?php
// $Id: template.php,v 1.2 2011/02/04 14:35:58 jax Exp $

/**
 * @file
 * Contains theme specific function overrides.
 */

/**
 * Override or insert variables into the html template.
 */
function desk02_gradiel_preprocess_html(&$vars) {
  // Adds a conditional CSS for IE6.
  drupal_add_css(path_to_theme() . '/css/ie6.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'lt IE 7', '!IE' => FALSE), 'preprocess' => FALSE));
}

/**
 * Override or insert variables into the page template.
 */
function desk02_gradiel_preprocess_page(&$variables) {
  // Adds a span tag to the primary menu loinks to allow for sliding doors.
  foreach ($variables['main_menu'] as $key => $menu_item) {
    $menu_item['title'] = '<span>' . $menu_item['title'] . '</span>';
    $menu_item['html'] = TRUE;
    $variables['main_menu'][$key] = $menu_item;
  }
}

/**
 * Implements hook_preprocess_maintenance_page().
 */
function desk02_gradiel_preprocess_maintenance_page(&$variables) {
  drupal_add_css(path_to_theme() . '/css/maintenance-page.css', array('group' => CSS_THEME));
}

/**
 * Override or insert variables into the block template.
 */
function desk02_gradiel_preprocess_block(&$variables) {
  $block_count = count(block_list($variables['block']->region));
  $variables['classes_array'][] = 'block-count-' . $block_count;

  $variables['classes_array'][] = ($variables['block_id'] == 1) ? 'block-first' : '';
  $variables['classes_array'][] = ($variables['block_id'] == $block_count) ? 'block-last' : '';
}

/**
 * Override or insert variables into the page template for HTML output.
 */
function desk02_gradiel_process_html(&$variables) {
  // Hook into color.module.
  if (module_exists('color')) {
    _color_html_alter($variables);
  }
}

/**
 * Override or insert variables into the page template.
 */
function desk02_gradiel_process_page(&$variables) {
  // Hook into color.module.
  if (module_exists('color')) {
    _color_page_alter($variables);
  }
}

/**
 * Override the core breadcrumb functionality.
 */
function desk02_gradiel_breadcrumb($variables) {
  global $language;
  $breadcrumb = $variables['breadcrumb'];

  if (!empty($breadcrumb)) {
    // Provide a navigational heading to give context for breadcrumb links to
    // screen-reader users. Make the heading invisible with .element-invisible.
    $output = '<h2 class="element-invisible">' . t('You are here') . '</h2>';

    if ($language->direction == LANGUAGE_LTR) {
      $output .= '<div class="breadcrumb">' . implode(' » ', $breadcrumb) . '</div>';
    }
    elseif ($language->direction == LANGUAGE_RTL) {
      $output .= '<div class="breadcrumb">' . implode(' » ', array_reverse($breadcrumb)) . '</div>';
    }
    return $output;
  }
}