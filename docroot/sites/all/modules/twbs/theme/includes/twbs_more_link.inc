<?php

/**
 * Returns HTML for a "more" link, like those used in blocks.
 *
 * @param $variables
 *   An associative array containing:
 *   - url: The URL of the main page.
 *   - title: A descriptive verb for the link, like 'Read more'.
 */
function twbs_more_link($variables) {
  return '<div class="more-link">' . l(t('More'), $variables['url'], array('attributes' => array('title' => $variables['title']))) . '</div>';
}
