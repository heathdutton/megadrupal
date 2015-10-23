<?php

/**
 * Implements hook_views_plugins
 *
 *
 */
function gallery_summary_views_plugins() {
  return array(
    'style' => array(
      'gallery_summary' => array(
        'title' => t('Gallery Summary'),
        'help' => t('Displays a summary, with option for attaching a display for each argument.'),
        'handler' => 'views_plugin_style_gallery_summary',
        'parent' => 'default_summary',
        'theme' => 'views_plugin_style_gallery_summary',
        'type' => 'summary', // Only shows up as a summary style.
        'uses options' => TRUE,
        'help topic' => 'style-gallery-summary',
      ),
    ),
  );
}
