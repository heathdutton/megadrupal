<?php

/**
 * @file
 * Definition of the Versatile default panel style.
 */

// Plugin definition
$plugin = array(
  'title' => t('Versatile Default'),
  'description' => t('A modification of the default No style without panels separators.'),
  'render region' => 'versatile_default_style_render_region',
);

/**
 * Render callback.
 */
function theme_versatile_default_style_render_region($vars) {
  $output = '';
  $output .= implode("\n", $vars['panes']);
  return $output;
}

