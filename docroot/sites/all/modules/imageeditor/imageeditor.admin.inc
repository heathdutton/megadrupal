<?php
/**
 * @file
 * Admin settings for the Image Editor module.
 */

/**
 * Image Editor module API keys and other settings form.
 */
function imageeditor_settings_form() {
  $form = array();

  $options = array();
  foreach (imageeditor_info('overlay') as $codename => $overlay) {
    $options[$codename] = $overlay['name'];
  }
  $form['imageeditor_overlay_type'] = array(
    '#type' => 'radios',
    '#title' => t('Image Editor overlay type'),
    '#description' => t('Choose Image Editor modal window type.'),
    '#options' => $options,
    '#default_value' => variable_get('imageeditor_overlay_type', 'custom'),
  );

  foreach (array('editor', 'uploader') as $type) {
    foreach (imageeditor_info($type) as $codename => $plugin) {
      if ($function = ctools_plugin_get_function($plugin, 'settings_form_callback')) {
        $form[$codename] = $function();
      }
    }
  }

  return system_settings_form($form);
}
