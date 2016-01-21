<?php

/**
 * @file
 * Define the WYSIWYG browser plugin.
 */

/**
 * Implementation of WYSIWYG's hook_INCLUDE_plugin().
 */
function mm_media_mm_media_plugin() {
  // Include the required browser JS.
  // @todo: wyswiyg should allow libraries and multiple js files
  // to be defined by this hook.
  // @see http://drupal.org/node/1039076
  mm_media_include_browser_js();

  // Plugin definition
  $path = drupal_get_path('module', 'mm_media') . '/wysiwyg';
  $plugins['mm_media'] = array(
    'title' => t('Monster Menus media'),
    'vendor url' => 'https://www.amherst.edu/offices/it/web/monstermenus/mm_media',
    'icon path' => $path,
    'icon file' => 'wysiwyg-media.gif',
    'icon title' => t(variable_get('media__wysiwyg_icon_title')),
    'js path' => $path,
    'js file' => 'wysiwyg-media.js',
    'css file' => NULL,
    'css path' => NULL,
    'settings' => array(
      'global' => array(
        'types' => variable_get('media__wysiwyg_allowed_types'),
        'extended_valid_elements' => array("swf[*],embed[*]"),
        'id' => 'mm_media_wysiwyg',
      ),
    ),
  );

  return $plugins;
}

/**
 * Prepares the page to be able to launch the media browser.
 *
 * Defines default variables.
 */
function mm_media_include_browser_js() {
  static $included;
  if (!empty($included)) {
    return;
  }
  $included = TRUE;
  // Add wysiwyg-specific settings.
  $settings = array('blacklist' => array('src', 'fid', 'view_mode', 'format'), 'browserUrl' => url('mm_media_assist/load', array('absolute' => TRUE, 'query' => array('location' => url($_GET['q'])))));
  drupal_add_js(array('mm_media' => $settings), 'setting');
}
