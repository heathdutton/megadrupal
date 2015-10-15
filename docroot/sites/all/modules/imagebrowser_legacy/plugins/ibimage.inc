<?php

/**
 * Implementation of hook_wysiwyg_plugin().
 */
function imagebrowser_legacy_ibimage_plugin() {
//   drupal_add_css(drupal_get_path('module', 'ib_wysiwyg') . "/ib_wysiwyg_init.css");
//   if(module_exists('jquery_ui')){ jquery_ui_add('ui.dialog'); };
  $plugins['ibimage'] = array(
    'title' => t('Image Browser Legacy'),
    'vendor url' => 'http://drupal.org/project/imagebrowser_legacy',
//     'icon file' => 'ib_wysiwyg_icon.gif',
//     'icon path' => drupal_get_path('module', 'ib_wysiwyg'),
//     'icon title' => t('Insert images via Image Browser'),
    'js path' => drupal_get_path('module', 'imagebrowser_legacy') . '/plugins',
    'js file' => 'ibimage_init.js',
//     'css path' => file_directory_path(),
//     'css file' => 'ibstyles.css',
    'settings' => array(),
  );
  return $plugins;
}