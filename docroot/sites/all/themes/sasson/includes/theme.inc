<?php

/**
 * @file
 * Overide core markup.
 */


/**
 * Implements theme_menu_tree().
 */
function sasson_menu_tree($vars) {
  return '<ul class="menu clearfix">' . $vars['tree'] . '</ul>';
}

/**
 * theme_menu_tree() implementation for main menu.
 */
function sasson_menu_tree__main_menu($vars) {
  return '<ul class="menu inline clearfix">' . $vars['tree'] . '</ul>';
}

/**
 * theme_menu_tree() implementation for secondary menu.
 */
function sasson_menu_tree__secondary_menu($vars) {
  return '<ul class="menu inline clearfix">' . $vars['tree'] . '</ul>';
}


/**
 * Generate the HTML output for a menu link and submenu.
 *
 * @param $vars
 *   An associative array containing:
 *   - element: Structured array data for a menu link.
 *
 * @return
 *   A themed HTML string.
 *
 * @ingroup themeable
 */
function sasson_menu_link(array $vars) {
  $element = $vars['element'];
  $sub_menu = '';

  if ($element['#below']) {
    $sub_menu = drupal_render($element['#below']);
  }

  $output = l($element['#title'], $element['#href'], $element['#localized_options']);
  // Adding a class depending on the TITLE of the link (not constant)
  $element['#attributes']['class'][] = drupal_html_id($element['#title']);
  // Adding a class depending on the ENGLISH title of the link (constant)
  $element['#attributes']['class'][] = drupal_html_id($element['#original_link']['link_title']);
  // Adding a class depending on the ID of the link (constant)
  if (isset($element['#original_link']['mlid']) && !empty($element['#original_link']['mlid'])) {
    $element['#attributes']['class'][] = 'mid-' . $element['#original_link']['mlid'];
  }
  return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";
}


/**
 *  Duplicate of theme_menu_local_tasks() but adds clearfix to tabs.
 */
function sasson_menu_local_tasks(&$vars) {
  $output = '';

  if (!empty($vars['primary'])) {
    $vars['primary']['#prefix'] = '<h2 class="element-invisible">' . t('Primary tabs') . '</h2>';
    $vars['primary']['#prefix'] .= '<ul class="tabs primary clearfix">';
    $vars['primary']['#suffix'] = '</ul>';
    $output .= drupal_render($vars['primary']);
  }

  if (!empty($vars['secondary'])) {
    $vars['secondary']['#prefix'] = '<h2 class="element-invisible">' . t('Secondary tabs') . '</h2>';
    $vars['secondary']['#prefix'] .= '<ul class="tabs secondary clearfix">';
    $vars['secondary']['#suffix'] = '</ul>';
    $output .= drupal_render($vars['secondary']);
  }

  return $output;
}


/**
 * Implements theme_field__field_type().
 */
function sasson_field__taxonomy_term_reference($vars) {
  $output = '';

  // Render the label, if it's not hidden.
  if (!$vars['label_hidden']) {
    $output .= '<h3 class="field-label">' . $vars['label'] . ': </h3>';
  }

  // Render the items.
  $output .= ( $vars['element']['#label_display'] == 'inline') ? '<ul class="links inline">' : '<ul class="links">';
  foreach ($vars['items'] as $delta => $item) {
    $output .= '<li class="taxonomy-term-reference-' . $delta . '"' . $vars['item_attributes'][$delta] . '>' . drupal_render($item) . '</li>';
  }
  $output .= '</ul>';

  // Render the top-level DIV.
  $output = '<div class="' . $vars['classes'] . (!in_array('clearfix', $vars['classes_array']) ? ' clearfix' : '') . '">' . $output . '</div>';

  return $output;
}


/**
 *  Return a themed breadcrumb trail
 */
function sasson_breadcrumb($vars) {

  $breadcrumb = isset($vars['breadcrumb']) ? $vars['breadcrumb'] : array();

  if (theme_get_setting('sasson_breadcrumb_hideonlyfront')) {
    $condition = count($breadcrumb) > 1;
  } else {
    $condition = !empty($breadcrumb);
  }

  if(theme_get_setting('sasson_breadcrumb_showtitle')) {
    $title = drupal_get_title();
    if(!empty($title)) {
      $condition = true;
      $breadcrumb[] = $title;
    }
  }

  $separator = theme_get_setting('sasson_breadcrumb_separator');

  if (!$separator) {
    $separator = '»';
  }

  if ($condition) {
    return implode(" {$separator} ", $breadcrumb);
  }
}
