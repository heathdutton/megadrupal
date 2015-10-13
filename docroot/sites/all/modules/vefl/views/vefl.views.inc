<?php
/**
 * @file
 * Provides views exposed form plugins.
 */

/**
 * Implements hook_views_plugins().
 */
function vefl_views_plugins() {
  $plugins = array('exposed_form' => array());
  $plugins['exposed_form']['bef_basic_layout'] = array(
    'title' => t('Basic (with layout)'),
    'help' => t('Adds layout settings for Exposed form'),
    'handler' => 'vefl_plugin_exposed_form_basic',
    'uses row plugin' => FALSE,
    'uses fields' => TRUE,
    'uses options' => TRUE,
    'help topic' => 'exposed-form-basic',
    'type' => 'normal',
    'parent' => 'views_plugin_exposed_form_basic',
  );

  if (module_exists('better_exposed_filters')) {
    $plugins['exposed_form']['vefl_bef'] = array(
      'title' => t('Better Exposed Filters (with layout)'),
      'help' => t('Adds layout settings to BEF module'),
      'handler' => 'vefl_plugin_exposed_form_bef',
      'uses row plugin' => FALSE,
      'uses fields' => TRUE,
      'uses options' => TRUE,
      'help topic' => 'exposed-form-basic',
      'type' => 'normal',
      'parent' => 'better_exposed_filters_exposed_form_plugin',
    );
  }
  return $plugins;
}
