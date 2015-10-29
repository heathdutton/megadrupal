<?php
/**
 * @file
 * pager-last.theme.inc
 */


/**
 * Returns HTML for the "next page" link in a query pager.
 *
 * @param array $variables
 *   An associative array containing:
 *   - text: The name (or image) of the link.
 *   - element: An optional integer to distinguish between multiple pagers on
 *     one page.
 *   - interval: The number of pages to move forward when the link is clicked.
 *   - parameters: An associative array of query string parameters to append to
 *     the pager links.
 *
 * @ingroup themeable
 */
function wetkit_omega_pager_last($variables) {
  $text = $variables['text'];
  $element = $variables['element'];
  $parameters = $variables['parameters'];
  if (!isset($variables['attributes'])) {
    $variables['attributes'] = array();
  }
  $attributes = $variables['attributes'];
  global $pager_page_array, $pager_total;
  $output = '';

  // If we are anywhere but the last page.
  if ($pager_page_array[$element] < ($pager_total[$element] - 1)) {
    $output = theme('pager_link', array('text' => $text, 'page_new' => pager_load_array($pager_total[$element] - 1, $element, $pager_page_array), 'element' => $element, 'parameters' => $parameters, 'attributes' => $attributes));
  }
  return $output;
}
