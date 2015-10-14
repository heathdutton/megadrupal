<?php
/**
 * Theme overrides.
 */

/**
 * HTML preprocessing.
 */
function fusion_slate_preprocess_html(&$vars) {
  // Add body classes for custom design options.
  $vars['classes_array'][] = theme_get_setting('fusion_slate_banner_style');
}

/**
 * Override theme_button for expanding graphic buttons.
 */
function fusion_slate_button($variables) {
  // Make sure not to overwrite classes.
  $element = $variables['element'];
  if (isset($element['#attributes']['class'])) {
    $element['#attributes']['class'] = 'form-' . $element['#button_type'] . ' ' . $element['#attributes']['class'];
  }
  else {
    $element['#attributes']['class'] = 'form-' . $element['#button_type'];
  }

  // Wrap visible inputs with span tags for button graphics.
  if (isset($element['#attributes']['style']) && (stristr($element['#attributes']['style'], 'display: none;') || stristr($element['#attributes']['class'], 'fivestar-submit'))) {
    return '<input type="submit" ' . (empty($element['#name']) ? '' : 'name="' . $element['#name'] . '" ')  . 'id="' . $element['#id'] . '" value="' . check_plain($element['#value']) . '" ' . drupal_attributes($element['#attributes']) . " />\n";
  }
  else {
    return '<span class="button-wrapper"><span class="button"><span><input type="submit" ' . (empty($element['#name']) ? '' : 'name="' . $element['#name'] . '" ') . 'id="' . $element['#id'] . '" value="' . check_plain($element['#value']) . '" ' . drupal_attributes($element['#attributes']) . " /></span></span></span>\n";
  }
}

/**
 * To add styling for the terms wrapper.
 */
function fusion_slate_field__taxonomy_term_reference($variables) {
  $output = '';
  // Render the items.
  $output .= '<div class=terms terms-inline"><div class="terms-inner">';
  $output .= ($variables['element']['#label_display'] == 'inline') ? '<ul class="links inline">' : '<ul class="links inline">';
  foreach ($variables['items'] as $delta => $item) {
    $output .= '<li class="taxonomy-term-reference-' . $delta . '"' . $variables['item_attributes'][$delta] . '>' . drupal_render($item) . '</li>';
  }
  $output .= '</ul>';

  // Render the top-level DIV.
  $output = '<div class="' . $variables['classes'] . (!in_array('clearfix', $variables['classes_array']) ? ' clearfix' : '') . '">' . $output . '</div></div></div>';

  return $output;
}

/**
 * Override or insert variables into the HTML template for HTML output.
 */
function fusion_slate_process_html(&$vars) {
  // Hook into color.module.
  if (module_exists('color')) {
    _color_html_alter($vars);
  }
}

/**
 * Override or insert variables into the page template for HTML output.
 */
function fusion_slate_process_page(&$vars) {
  // Hook into color.module.
  if (module_exists('color')) {
    _color_page_alter($vars);
  }
}
