<?php

/**
 * @file
 * Contains WYSIWYG plugin hook implementation.
 */

/**
 * Implements hook_PLUGIN_NAME_plugin().
 */
function local_storage_wysiwyg_html5ls_plugin() {
  $path = drupal_get_path('module', 'local_storage_wysiwyg');
  $plugins['html5ls'] = array(
    // The plugin's title; defaulting to its internal name ('awesome').
    'title' => t('Local Storage Info plugin'),
    // The (vendor) homepage of this plugin; defaults to ''.
    'vendor url' => 'http://drupal.org/project/html5ls',
    // The path to the button's icon; defaults to
    // '/[path-to-module]/[plugins-directory]/[plugin-name]/images'.
    // 'icon path' => 'sites/all/libraries/ckeditor/plugins/',
    // The button image filename; defaults to '[plugin-name].png'.
    // 'icon file' => 'icons.png',
    // The button title to display on hover.
    'icon title' => t('Local Storage'),
    // An alternative path to the integration JavaScript; defaults to
    // '[path-to-module]/[plugins-directory]/[plugin-name]'.
    // An alternative filename of the integration JavaScript; defaults to
    // '[plugin-name].js'.
    // 'js file' => 'html5ls.js',
    // An alternative path to the integration stylesheet; defaults to
    // '[path-to-module]/[plugins-directory]/[plugin-name]'.
    'css path' => $path . '/plugins/html5ls',
    // An alternative filename of the integration stylesheet; defaults to
    // '[plugin-name].css'.
    'css file' => 'html5ls.css',
    // An array of settings for this button. Required, but API is still in flux.
    'settings' => array(),
  );
  return $plugins;
}
