<?php

/**
 * Returns HTML for the entire dashboard.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: A render element containing the properties of the dashboard
 *     region element, #dashboard_region and #children.
 *
 * @ingroup themeable
 */
function twbs_dashboard($variables) {
  extract($variables);
  drupal_add_css(drupal_get_path('module', 'dashboard') . '/dashboard.css');
  return '<div id="dashboard" class="clearfix">' . $element['#children'] . '</div>';
}
