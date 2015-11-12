<?php

/**
 * Implements hook_views_plugins().
 */
function dan_polaroid_style_views_plugins() {
  return array(
    'style' => array(
      'dan_polaroid' => array(
        'title' => t('Dan Polaroid'),
        'help' => t('Dan Polaroid plugin'),
        'handler' => 'dan_polaroid_style_plugin',
        'theme' => 'views_view_dan_polaroid',
        'uses options' => TRUE,
        'uses fields' => TRUE,
        'uses row plugin' => TRUE,
        'type' => 'normal',
      ),
    ),
    'row' => array(
      'dan_polaroid_row' => array(
        'title' => t('Dan Polaroid Row'),
        'help' => t('Dan Polaroid Row plugin'),
        'handler' => 'dan_polaroid_row_plugin',
        'theme' => 'views_view_dan_polaroid_row',
        'uses options' => TRUE,
        'uses fields' => TRUE,
        'type' => 'normal',
      ),
    ),
  );
}
