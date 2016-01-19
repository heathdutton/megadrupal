<?php

/**
 * Returns HTML for the non-customizable part of the dashboard page.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: A render element containing a #message.
 *
 * @ingroup themeable
 */
function twbs_dashboard_admin($variables) {
  // We only return a simple help message, since the actual content of the page
  // will be populated via the dashboard regions in dashboard_page_build().
  return '<div class="customize-dashboard js-hide">' . $variables['element']['#message'] . '</div>';
}
