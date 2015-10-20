<?php
/**
 * @file
 *
 *
 *
 * @author Kálmán Hosszu - hosszu.kalman@gmail.com
 */

/**
 * Implementing hook_INCLUDE_plugin().
 *
 * @return array
 */
function mg_picker_mg_picker_plugin() {
  $plugins = array();

  $plugins['mg_picker'] = array(
    'title' => t('Media gallery picker'),
    'vendor url' => 'https://github.com/hosszukalman/mg_picker',
    'icon file' => 'mg_picker.gif',
    'icon title' => t('Select the media gallery.'),
    'settings' => array(
      'dialog' => array(
        'url' => base_path() . 'index.php?q=wysiwyg/mg_picker',
        'width' => 620,
        'height' => 600,
      ),
    ),
  );

  return $plugins;
}

/**
 * Implementing hook_wysiwyg_dialog().
 *
 * @param type $instance
 * @return type
 */
function mg_picker_wysiwyg_dialog($instance) {
  $module_path = drupal_get_path('module', 'mg_picker');
  drupal_add_js($module_path . '/plugins/mg_picker/dialog.js', 'file');
  drupal_add_css($module_path . '/plugins/mg_picker/reset.css', array('group' => CSS_THEME));
  drupal_add_css($module_path . '/plugins/mg_picker/dialog.css', array('group' => CSS_THEME));

  drupal_set_title(t('Select a gallery'));

  $form = drupal_get_form('mg_picker_gallery_search_form');

  return drupal_render($form);
}
