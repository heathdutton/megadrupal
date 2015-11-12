<?php
/**
 * @file
 * Provide an fractionslider style plugin for Views. This file is autoloaded by views.
 */

/**
  * Implementation of hook_views_plugin().
  */
function views_fractionslider_views_plugins() {
  return array(
    'style' => array(
      'views_fractionslider' => array(
        'title' => t('Views Fractionslider'),
        'theme' => 'views_view_fractionslider',
        'help' => t('Display a JQuery Fractionslider with the results. The first field will be used as the header and trigger.'),
        'handler' => 'views_fractionslider_style_plugin',
        'uses row plugin' => TRUE,
        'uses row class' => TRUE,
        'uses options' => TRUE,
        'uses fields' => TRUE,
        'type' => 'normal',
      ),
    ),
  );
}

/**
 * Implements hook_views_data().
 */
function views_fractionslider_views_data_alter(&$data) {
}