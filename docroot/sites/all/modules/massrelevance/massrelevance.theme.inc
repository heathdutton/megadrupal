<?php

/**
 * @file
 * Theme implementation file.
 */

define('MASSRELEVANCE_BASE_URL', 'http://up.massrelevance.com');
define('MASSRELEVANCE_BASE_SECURE_URL', 'https://secure-up.massrelevance.com');

/**
 * Theme function for Mass Relevance widgets.
 */
function theme_massrelevance_widget($variables) {
  $url = _massrelevance_widget_url($variables['widget_type'], $variables['display_type'], $variables['widget_code']);
  switch ($variables['display_type']) {
    case 'iframe':
    case 'facebook':
      $output = '<iframe src="' . $url . '" frameborder="0" width="' . $variables['width'] . 'px" height="' . $variables['height'] . 'px" style="border-top-style: none; border-right-style: none; border-bottom-style: none; border-left-style: none; border-width: initial; border-color: initial; border-image: initial; overflow-x: hidden; overflow-y: hidden;" allowtransparency="true" scrolling="no"></iframe>';
      return $output;
    case 'html5':
      return $url;
  }
}

/**
 * Helper function to get the URL where the widget can be found.
 *
 * @param string $widget_type
 *   The type of widget.
 * @param string $display_type
 *   The type of the output.
 * @param string $widget_type
 *   The code of widget.
 * @return string
 *   The URL for the iframe, html5 or facebook formatter.
 */
function _massrelevance_widget_url($widget_type, $display_type, $widget_code) {
  $widgets_info = massrelevance_massrelevance_widget_info();
  if ($display_type == 'facebook') {
    $url = MASSRELEVANCE_BASE_SECURE_URL;
  }
  else {
    $url = MASSRELEVANCE_BASE_URL;
  }
  $widget_info = $widgets_info[$widget_type];
  $url .= '/massrel-products/' . $widget_info['component'] . '-' . $widget_info['version'] . '/index.html';
  return url($url, array(
    'query' => array('config' => $widget_code),
  ));
}
