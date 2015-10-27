<?php

/**
 * @file
 * Administration form.
 */

/**
 * Page callback for settings form.
 *
 * @see: panels_cache_bully_menu().
 */
function panels_cache_bully_settings_form($form, &$form_state) {

  $form['panels_cache_bully_cache_plugin'] = array(
    '#type' => 'radios',
    '#title' => t('Panels cache plugin to use'),
    '#default_value' => variable_get('panels_cache_bully_cache_plugin', 'simple'),
    '#description' => t('The default settings of the selected cache plugin will be used by Panels cache bully.'),
    '#options' => panels_cache_bully_plugins_options_list(),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}

/**
 * Returns options list with Panels cache plugins.
 */
function panels_cache_bully_plugins_options_list() {
  $panels_cache_plugins = ctools_get_plugins('panels', 'cache');

  $options = array();
  foreach ($panels_cache_plugins as $plugin => $settings) {
    $options[$plugin] = $settings['title'] . " " . json_encode($settings['defaults']);
  }
  return $options;
}
