<?php

/**
 * Override or insert variables into page templates.
 *
 * @param $vars
 *   A sequential array of variables to pass to the theme template.
 * @param $hook
 *   The name of the theme function being called.
 */
function earthish_preprocess_page(&$vars) {
  if (!empty($vars['node'])) {
    $vars['theme_hook_suggestions'][] = 'page__' . $vars['node']->type;
  }

  //@TODO add variables for the menu for easier printing
}

/**
 * Override or insert variables into the node templates.
 *
 * @param $vars
 *   A sequential array of variables to pass to the theme template.
 * @param $hook
 *   The name of the theme function being called.
 */
function earthish_preprocess_node(&$vars) {
}

/**
 * Override or insert variables into the html template.
 */
function earthish_preprocess_html(&$vars) {
  // Add conditional CSS for IE
  drupal_add_css(path_to_theme() . '/css/ie.css', array('weight' => CSS_THEME, 'browsers' => array('!IE' => FALSE), 'preprocess' => FALSE));

		// Set up variable for view port meta tag
		$meta_viewport = array(
    '#type' => 'html_tag',
    '#tag' => 'meta',
    '#attributes' => array(
      'name' => 'viewport',
      'content' => 'width=device-width'
    )
  );

		// Add meta tag to head
		drupal_add_html_head($meta_viewport, 'viewport');

}

/**
 * Implements theme_breadcrumb.
 */
function earthish_breadcrumb(&$variables) {
  $breadcrumb = $variables['breadcrumb'];
	$title = strip_tags(drupal_get_title());

  if (!empty($breadcrumb)) {
    // Provide a navigational heading to give context for breadcrumb links to
    // screen-reader users. Make the heading invisible with .element-invisible.
    $output = '<h2 class="element-invisible">' . t('You are here') . '</h2>';

    $output .= '<div class="breadcrumb">' . implode(' &raquo; ', $breadcrumb) . ' &raquo; ' . $title . '</div>';
    return $output;
  }
}

/**
 * Implements theme_field__field_type().
 */
function earthish_field__taxonomy_term_reference(&$variables) {
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
