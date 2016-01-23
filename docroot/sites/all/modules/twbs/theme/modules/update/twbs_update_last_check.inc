<?php

/**
 * Returns HTML for the last time we checked for update data.
 *
 * In addition to properly formatting the given timestamp, this function also
 * provides a "Check manually" link that refreshes the available update and
 * redirects back to the same page.
 *
 * @param $variables
 *   An associative array containing:
 *   - last: The timestamp when the site last checked for available updates.
 *
 * @see theme_update_report()
 * @see theme_update_available_updates_form()
 * @ingroup themeable
 */
function twbs_update_last_check($variables) {
  $last = $variables['last'];
  $output = '<div class="update checked">';
  $output .= $last ? t('Last checked: @time ago', array('@time' => format_interval(REQUEST_TIME - $last))) : t('Last checked: never');
  $output .= ' <span class="check-manually">(' . l(t('Check manually'), 'admin/reports/updates/check', array('query' => drupal_get_destination())) . ')</span>';
  $output .= "</div>\n";
  return $output;
}
