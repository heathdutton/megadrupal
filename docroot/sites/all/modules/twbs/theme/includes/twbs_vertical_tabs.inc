<?php

/**
 * Returns HTML for an element's children fieldsets as vertical tabs.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: An associative array containing the properties and children of
 *     the fieldset. Properties used: #children.
 *
 * @ingroup themeable
 */
function twbs_vertical_tabs($variables) {
  $element = $variables['element'];

  $attributes['id'] = $element['#parents'][0];
  $attributes['class'] = array('vertical-tabs-panes', 'panel-group', 'form-group');

  $output = '<h2 class="sr-only">' . t('Collapse') . '</h2>';
  $output .= '<div' . drupal_attributes($attributes) . '>';
  $output .= $element['#children'];
  $output .= '</div>';

  return $output;
}
