<?php

/**
 * Form callback for the Steroids settings form.
 */
function steroids_settings($form_state) {
  $form = array();

  $form['help'] = array(
      '#type' => 'fieldset',
      '#title' => t('Steroids'),
  );

  $form['help']['request_cache_clear'] = array(
      '#type' => 'submit',
      '#value' => t('Clear all caches'),
      '#suffix' => t('This requests a cache clear for the entire cache bin for all registered varnish servers'),
      '#submit' => array('steroids_settings_request_cache_clear'),
  );

  $form['steroids'] = array();
  $plugins = steroids_get_all();

  // Provide menu items for each task.
  foreach ($plugins as $plugin_name => $plugin) {
    $handlers = steroids_get($plugin_name);
    if ($function = ctools_plugin_get_function($handlers, 'admin_settings_form')) {
      $form['steroids'] = array_merge($form['steroids'], $function($form_state));
    }
  }

  return system_settings_form($form);
}

/**
 * Form callback settings validation.
 */
function steroids_settings_validate(&$form, $form_state) {
  $plugins = steroids_get_all();
  foreach ($plugins as $plugin_name => $plugin) {
    $handlers = steroids_get($plugin_name);
    if ($function = ctools_plugin_get_function($handlers, 'settings_form_validate')) {
      $function($form, $form_state);
    }
  }
}

/**
  * Send a request to all cache backends that they need to clear their entire bin
  */
function steroids_settings_request_cache_clear(&$form, $form_state) {
  $servers = 0;

  $modules = module_implements('steroids_cache_clear');
  foreach ($modules as $module) {
    module_invoke($module, 'steroids_cache_clear');
    $servers++;
  }

  drupal_set_message(format_plural($servers,
    'Cache clear request has been sent to 1 server',
    'Cache clear request has been sent to @count servers'));
}
