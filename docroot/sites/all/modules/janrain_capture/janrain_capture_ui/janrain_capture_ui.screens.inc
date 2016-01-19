<?php

/**
 * @file
 * Admin settings form for Capture screens
 */

/**
 * Sets the Capture screens path.
 */
function janrain_capture_ui_screens_settings() {
  $screens_folder = variable_get('janrain_capture_screens_folder', array());

  $form['janrain_capture_screens_folder'] = array(
    '#type' => 'textfield',
    '#title' => t('Screens Folder'),
    '#default_value' => !empty($screens_folder) ? $screens_folder : 'file:///sites/all/themes/janrain-capture-screens/',
    '#description' => t('URL of the Capture screens folder<br/>(examples: file:///sites/all/themes/janrain-capture-screens/, http://example.com/capture-screens/)'),
  );

  $form['#validate'] = array(
    'janrain_capture_ui_screens_settings_validate',
  );
  $form['#submit'] = array(
    'janrain_capture_ui_screens_settings_submit',
  );

  return system_settings_form($form);
}

/**
 * Validate callback for the settings form.
 */
function janrain_capture_ui_screens_settings_validate($form, &$form_state) {
  $v = strtolower($form_state['values']['janrain_capture_screens_folder']);
  if ($v
      && strpos($v, 'file:///', 0) !== 0
      && strpos($v, 'http://', 0) !== 0
      && strpos($v, 'https://', 0) !== 0) {

    form_set_error('janrain_capture_screens_folder', t('Capture screens folder URL: unknown scheme. Examples of valid URLs: file:///sites/all/themes/janrain-capture-screens/, http://example.com/capture-screens/)'));
  }
}

/**
 * Submit callback for the settings form.
 */
function janrain_capture_ui_screens_settings_submit($form, &$form_state) {
  // Make sure there's a trailing slash in the capture screens folder URL.
  $screens_folder = $form_state['values']['janrain_capture_screens_folder'];
  if ($screens_folder) {
    $form_state['values']['janrain_capture_screens_folder'] = rtrim($screens_folder, '/') . '/';
  }
}
