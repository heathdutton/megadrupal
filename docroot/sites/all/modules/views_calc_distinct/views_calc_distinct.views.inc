<?php
/**
 * @file
 * Views Calc Distinct views plugins.
 */

/**
 * Implements hook_views_plugins().
 */
function views_calc_distinct_views_plugins() {
  $path = drupal_get_path('module', 'views_calc');
  
  return array(
    'module' => 'views_calc_distinct',
    'style' => array(
      'views_calc_distinct' => array(
        'title' => t('Views Calc Distinct'),
        'help' => t('Creates a table with column calculations.'),
        'handler' => 'views_calc_distinct_plugin_style_table',
        'uses options' => TRUE,
        'uses fields' => TRUE,
        'uses row plugin' => FALSE,
        'uses grouping' => FALSE,
        'uses row class' => TRUE,
        'type' => 'normal',
        'parent' => 'views_calc_table',
        'theme' => 'views_calc_table',
        'theme file' => 'theme.inc',
        'theme path' => $path, 
      ),
    ),
  );
}
