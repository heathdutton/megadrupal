<?php
/**
 * @file
 * pager-first.theme.inc
 */


/**
 * Returns HTML for the "first page" link in a query pager.
 *
 * @param array $variables
 *   An associative array containing:
 *   - text: The name (or image) of the link.
 *   - element: An optional integer to distinguish between multiple pagers on
 *     one page.
 *   - parameters: An associative array of query string parameters to append to
 *     the pager links.
 *
 * @ingroup themeable
 */
function wetkit_omega_pager_first($variables) {
  $text = $variables['text'];
  $element = $variables['element'];
  $parameters = $variables['parameters'];
  if (!isset($variables['attributes'])) {
    $variables['attributes'] = array();
  }
  $attributes = $variables['attributes'];
  global $pager_page_array;
  $output = '';

  // If we are anywhere but the first page.
  if ($pager_page_array[$element] > 0) {
    $output = theme('pager_link', array('text' => $text, 'page_new' => pager_load_array(0, $element, $pager_page_array), 'element' => $element, 'parameters' => $parameters, 'attributes' => $attributes));
  }

  return $output;
}
