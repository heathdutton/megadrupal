<?php

/**
 * Returns HTML for a "more help" link.
 *
 * @param $variables
 *   An associative array containing:
 *   - url: The URL for the link.
 */
function twbs_more_help_link($variables) {
  return '<div class="more-help-link">' . l(t('More help'), $variables['url']) . '</div>';
}
