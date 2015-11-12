<?php
/**
 * @file
 * Provide an roundabout style plugin for Views. This file is autoloaded by views.
 */

/**
 * Implements hook_views_plugin().
 */
function views_roundabout_views_plugins() {
  return array(
    'style' => array(
      'views_roundabout' => array(
        'title' => t('Views Roundabout'),
        'theme' => 'views_view_roundabout',
        'help' => t('Uses the jQuery Roundabout plugin to create a carousel like animation of the view results.'),
        'handler' => 'views_roundabout_style_plugin',
        'uses row plugin' => TRUE,
        'uses row class' => TRUE,
        'uses options' => TRUE,
        'uses fields' => TRUE,
        'type' => 'normal',
      ),
    ),
  );
}

