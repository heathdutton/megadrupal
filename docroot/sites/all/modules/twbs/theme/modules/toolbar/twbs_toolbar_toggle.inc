<?php

/**
 * Formats an element used to toggle the toolbar drawer's visibility.
 *
 * @param $variables
 *   An associative array containing:
 *   - collapsed: A boolean value representing the toolbar drawer's visibility.
 *   - attributes: An associative array of HTML attributes.
 *
 * @return
 *   An HTML string representing the element for toggling.
 *
 * @ingroup themable
 */
function twbs_toolbar_toggle($variables) {
  if ($variables['collapsed']) {
    $toggle_text = t('Show shortcuts');
  }
  else {
    $toggle_text = t('Hide shortcuts');
    $variables['attributes']['class'][] = 'toggle-active';
  }
  return l($toggle_text, 'toolbar/toggle', array('query' => drupal_get_destination(), 'attributes' => array('title' => $toggle_text) + $variables['attributes']));
}
