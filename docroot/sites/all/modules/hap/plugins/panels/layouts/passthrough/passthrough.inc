<?php

/**
 * Plugin definition for this "anti-layout" plugin.
 *
 * Panels requires a layout plugin to be configured, however to reduce the size
 * of the DOM, we simply return the direct output of the content within the
 * display. Wrapper style plugins can then take control of how to wrap the page
 * content (@see http://drupal.org/node/1206806).
 */
$plugin = array(
  'title' => t('Pass-through'),
  'category' => t('Highly Attractive'),
  'icon' => 'passthrough.png',
  'theme' => 'hap_layouts_passthrough',
  'regions' => array(
    'page' => t('Page'),
  ),
);

/**
 * Theme callback.
 */
function theme_hap_layouts_passthrough($variables) {
  return $variables['content']['page'];
}
