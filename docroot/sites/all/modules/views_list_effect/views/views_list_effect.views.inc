<?php
/**
 * Implements hook_views_plugins().
 */
function views_list_effect_views_plugins() {
  $theme_path = drupal_get_path('module', 'views_list_effect') . '/views/theme';
  return array(
    'style' => array(
      'views_effects' => array(
        'title' => t('List effect'),
        'help' => t('Display the results using the list effect plugin'),
        'handler' => 'views_plugin_style_list_effect',
        'theme path' => $theme_path,
        'theme' => 'views_list_effect_views',
        'theme file' => 'views_list_effect_views.theme.inc',
        'uses options' => TRUE,
        'uses row plugin' => TRUE,
        'uses fields' => TRUE,
        'uses grouping' => FALSE,
        'type' => 'normal',
      ),
    ),
  );
}
