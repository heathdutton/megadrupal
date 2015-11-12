<?php

/**
 * @file
 * Theme functions for the Facet API JQuery Menu module.
 */

/**
 * Returns HTML for a JQuery Menu facet menu.
 */
function theme_facetapi_jquerymenu_menu($variables) {
  $tree = $variables['tree'];
  $trail = $variables['trail'];
  $attributes = $variables['attributes'];

  $menu_output = recursive_link_creator($tree, $trail);
  if (!empty($menu_output)) {
    $attributes['class'][] = 'menu';
    $attributes['class'][] = 'jquerymenu';

    $attributes = drupal_attributes($attributes);

    $output = '<ul' . $attributes . '>';
    $output .= $menu_output;
    $output .= '</ul>';

    return $output;
  }
}

/**
 * Returns HTML for an inactive facet item.
 *
 * @param $variables
 *   An associative array containing the keys 'text', 'options', and
 *   'count'. See theme_facetapi_count() functions for information
 *   about these variables.
 *
 * @ingroup themeable
 */
function theme_facetapi_jquerymenu_item_inactive($variables) {
  $options = $variables['options'];

  // Builds accessible markup.
  // @see http://drupal.org/node/1316580
  $accessible_vars = array(
    'text' => $variables['text'],
    'active' => FALSE,
  );
  $accessible_markup = theme('facetapi_accessible_markup', $accessible_vars);

  // Sanitizes the text if necessary.
  $sanitize = empty($options['html']);
  $variables['text'] = ($sanitize) ? check_plain($variables['text']) : $variables['text'];

  // Adds count if one was passed.
  if (!empty($variables['count'])) {
    $variables['text'] .= ' ' . theme('facetapi_count', $variables);
  }

  // Resets text, sets to options to HTML since we already sanitized the
  // text and are providing additional markup for accessibility.
  $variables['text'] .= $accessible_markup;

  $attributes = isset($options['attributes']) ? drupal_attributes($options['attributes']) : '';

  return '<span' . $attributes . '>' . $variables['text'] . '</span>';
}

/**
 * Returns HTML for an inactive facet item.
 *
 * @param $variables
 *   An associative array containing the keys 'text' and 'options'.
 *
 * @ingroup themeable
 */
function theme_facetapi_jquerymenu_item_active($variables) {
  $options = $variables['options'];

  // Sanitizes the text if necessary.
  $sanitize = empty($options['html']);
  $text = ($sanitize) ? check_plain($variables['text']) : $variables['text'];

  // Theme function variables for accessible markup.
  // @see http://drupal.org/node/1316580
  $accessible_vars = array(
    'text' => $variables['text'],
    'active' => TRUE,
  );

  // Builds text, passes through t() which gives us the ability to change the
  // position of the widget on a per-language basis.
  $replacements = array(
    '!facetapi_deactivate_widget' => theme('facetapi_deactivate_widget', $variables),
    '!facetapi_accessible_markup' => theme('facetapi_accessible_markup', $accessible_vars),
  );
  $variables['text'] = t('!facetapi_deactivate_widget !facetapi_accessible_markup', $replacements);

  $attributes = isset($options['attributes']) ? drupal_attributes($options['attributes']) : '';

  return '<span class="deactivate-widget">' . $variables['text'] . '</span>' . $text;
}
