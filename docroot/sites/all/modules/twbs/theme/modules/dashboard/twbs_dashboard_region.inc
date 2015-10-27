<?php

/**
 * Returns HTML for a generic dashboard region.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: A render element containing the properties of the dashboard
 *     region element, #dashboard_region and #children.
 *
 * @ingroup themeable
 */
function twbs_dashboard_region($variables) {
  extract($variables);
  $output = '<div id="' . $element['#dashboard_region'] . '" class="dashboard-region">';
  $output .= '<div class="region clearfix">';
  $output .= $element['#children'];
  // Closing div.region
  $output .= '</div>';
  // Closing div.dashboard-region
  $output .= '</div>';
  return $output;
}
