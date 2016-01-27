<?php

/**
 * Always returns an empty string.
 */
$plugin = array(
  'title' => t('★ Hidden'),
  'description' => t('Always returns an empty string.'),
  'render region' => 'hap_hidden_style_render_region',
  'render pane' => 'hap_hidden_style_render_pane',
);

/**
 * Theme callback for regions.
 */
function theme_hap_hidden_style_render_region($vars) {
  return '';
}

/**
 * Theme callback for panes.
 */
function theme_hap_hidden_style_render_pane($vars) {
  return '';
}
