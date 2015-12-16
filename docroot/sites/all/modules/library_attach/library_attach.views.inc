<?php

/**
 * @file
 * Views integration for the views_library module.
 */

/**
 * Implements hook_views_api().
 */
function library_attach_views_api() {
  return array('api' => 3);
}

/**
 * Implements hook_views_plugins().
 */
function library_attach_views_plugins() {
  $plugins = array();
  $plugins['display_extender']['libraries'] = array(
    'title' => t('Libraries'),
    'help' => t('Allows libraries to be added to views.'),
    'handler' => 'library_attach_plugin_display_extender_libraries',
    'enabled' => TRUE,
  );

  return $plugins;
}

/**
 * Implements hook_views_pre_render().
 *
 * There appears to be no way to attach libraries to view output using
 * #attached. This makes me sad.
 */
function library_attach_views_pre_render($view) {
  if ($libraries = $view->display_handler->get_option('libraries')) {
    library_attach_add_libraries($libraries);
  }
}
