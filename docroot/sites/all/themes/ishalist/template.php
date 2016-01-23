<?php

/** 
 * This file contains theme override functions and preprocess functions
 * 
 * If you prefer the Drupal default rendering simply comment out the functions.
 * Additionally you can add your own custom functions and variable here
 */

/**
 * Implementing theme_preprocess_html
 * Add browser specific CSS
 */
function ishalist_preprocess_html(&$variables) {
  // Add conditional stylesheets for IE
  drupal_add_css(path_to_theme() . '/css/ie.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'IE', '!IE' => FALSE), 'preprocess' => FALSE));  
}

/**
 * Override or insert variables into the maintenance page template.
 */
function ishalist_preprocess_maintenance_page(&$vars) {
  // While markup for normal pages is split into page.tpl.php and html.tpl.php,
  // the markup for the maintenance page is all in the single
  // maintenance-page.tpl.php template. This happen on the maintenance page,
		// it has to be called here.
  ishalist_preprocess_html($vars);
}


/**
 * Implements theme_field__field_type().
 */
function ishalist_field__taxonomy_term_reference($variables) {
  $output = '';

  // Render the label, if it's not hidden.
  if (!$variables['label_hidden']) {
    $output .= '<h3 class="field-label">' . $variables['label'] . ': </h3>';
  }

  // Render the items.
  $output .= ($variables['element']['#label_display'] == 'inline') ? '<ul class="links inline">' : '<ul class="links">';
  foreach ($variables['items'] as $delta => $item) {
    $output .= '<li class="taxonomy-term-reference-' . $delta . '"' . $variables['item_attributes'][$delta] . '>' . drupal_render($item) . '</li>';
  }
  $output .= '</ul>';

  // Render the top-level DIV.
  $output = '<div class="' . $variables['classes'] . (!in_array('clearfix', $variables['classes_array']) ? ' clearfix' : '') . '">' . $output . '</div>';

  return $output;
}

/**
 * Implements theme_breadcrumb.
 * Add text and modifying default seperator
 */
function ishalist_breadcrumb($variables) {
  $breadcrumb = $variables['breadcrumb'];
		$title = strip_tags(drupal_get_title());

  if (!empty($breadcrumb)) {
    // Provide a navigational heading to give context for breadcrumb links to
    // screen-reader users. Make the heading invisible with .element-invisible.
    $output = '<h2 class="element-invisible">' . t('You are here') . '</h2>';
    $output .= '<div class="breadcrumb"> ' . t('Path: ') . implode(' / ', $breadcrumb) . ' / ' . $title . '</div>';
    return $output;
  }
}

/**
 * Implements theme_preprocess_page
 */
function ishalist_preprocess_page(&$variables) {
		//@TODO Simplify outputting main menu and secondary menus
}
