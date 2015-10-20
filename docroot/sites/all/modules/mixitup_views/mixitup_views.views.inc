<?php
/**
 * @file
 * Provide views data and handlers for mixitup_views.module.
 */

/**
 * Implements hook_views_plugins().
 */
function mixitup_views_views_plugins() {
  return array(
    'style' => array(
      'mixitup_views' => array(
        'title' => t('MixItUp'),
        'handler' => 'views_plugin_style_mixitup_views',
        'uses options' => TRUE,
        'help' => t("Display content with MixItUp."),
        'theme' => 'views_view_mixitup',
        'base' => array('node'),
        'type' => 'normal',
        'uses row plugin' => TRUE,
        'uses row class' => TRUE,
        'uses grouping' => FALSE,
      ),
    ),
  );
}
