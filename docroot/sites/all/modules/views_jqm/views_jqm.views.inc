<?php
/**
 * @file
 * Defines the View Style Plugins for the Views jQM module.
 */

/**
 * Implements hook_views_plugins().
 */
function views_jqm_views_plugins() {
  $plugins = array(
    'style' => array(
      'basic_list' => array(
        'title' => t('jQM Basic List'),
        'help' => t('Applies jQuery effects to views output.'),
        'handler' => 'views_jqm_plugin_style_basic_list',
        'uses options' => TRUE,
        'uses row plugin' => TRUE,
        'uses grouping' => TRUE,
        'type' => 'normal',
        'path' => drupal_get_path('module', 'views_jqm'),
        'theme' => 'views_jqm_basic_list',
        'theme path' => drupal_get_path('module', 'views_jqm') . '/theme',
        'theme file' => 'views_jqm.theme.inc',
      ),
      'basic_grid' => array(
        'title' => t('jQM Basic Grid'),
        'help' => t('Applies jQuery effects to views output.'),
        'handler' => 'views_jqm_plugin_style_basic_grid',
        'uses row plugin' => TRUE,
        'uses options' => TRUE,
        'uses grouping' => FALSE,
        'type' => 'normal',
        'path' => drupal_get_path('module', 'views_jqm'),
        'theme' => 'views_jqm_basic_grid',
        'theme path' => drupal_get_path('module', 'views_jqm') . '/theme',
        'theme file' => 'views_jqm.theme.inc',
      ),
    ),
  );
  
  return $plugins;
}
