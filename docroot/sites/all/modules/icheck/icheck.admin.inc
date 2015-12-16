<?php

/**
 * @file
 * Administraton settings.
 *
 * @see https://github.com/fronteed/iCheck
 */

/**
 * Admin settings form.
 */
function icheck_settings_form($form, &$form_state) {
  $form['icheck_enabled'] = array(
    '#title' => t('iCheck Enabled'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('icheck_enabled', TRUE)
  );
  $form['icheck_admin_theme_enabled'] = array(
    '#title' => t('iCheck Enabled on Admin theme'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('icheck_admin_theme_enabled', FALSE)
  );
  $form['icheck_skin'] = array(
    '#title' => t('Skin'),
    '#type' => 'select',
    '#options' => icheck_skin_options(),
    '#default_value' => variable_get('icheck_skin', 'minimal'),
    '#ajax' => array(
      'callback' => 'icheck_skin_color_ajax',
      'wrapper' => 'icheck-skin-color-wrapper',
    ),
  );
  $form['icheck_skin_color'] = array(
    '#title' => t('Skin Color'),
    '#type' => 'select',
    '#options' => icheck_skin_color_options($form['icheck_skin']['#default_value']),
    '#default_value' => variable_get('icheck_skin_color'),
    '#validated' => TRUE, // Drupal does not rebuild options properly when using ajax so we skip validation
    '#prefix' => '<div id="icheck-skin-color-wrapper">',
    '#suffix' => '</div>',
  );
  $form['#submit'] = array('icheck_settings_form_submit');

  return system_settings_form($form);
}

/**
 * Submit callback.
 */
function icheck_settings_form_submit(&$form, &$form_state) {
  if (empty($form_state['values']['icheck_skin_color'])) {
    $form_state['values']['icheck_skin_color'] = $form_state['values']['icheck_skin'];
  }
}

/**
 * Select callback for updating color options.
 */
function icheck_skin_color_ajax($form, $form_state) {
  $form['icheck_skin_color']['#options'] = icheck_skin_color_options($form_state['values']['icheck_skin']);
  return $form['icheck_skin_color'];
}

/**
 * Build #options list of available skins.
 */
function icheck_skin_options() {
  $options = array();
  $skins_dir = libraries_get_path('iCheck') . '/skins';
  $skins = file_scan_directory($skins_dir, '/.*/', array('recurse' => FALSE));

  foreach ($skins as $skin) {
    if (is_dir($skin->uri)) {
      $options[$skin->name] = $skin->name;
    }
  }

  return $options;
}

/**
 * Build #options list of available colorschemes.
 */
function icheck_skin_color_options($skin) {
  $options = array();
  $skin_css_dir = libraries_get_path('iCheck') . '/skins/' . $skin;
  $colors = file_scan_directory($skin_css_dir, '/^[^.].*\.css$/', array('nomask' => "/($skin|_all)\.css$/"));

  if (empty($colors)) {
    return array('' => '-- colors unsupported --');
  }

  foreach ($colors as $color) {
    $options[$color->name] = $color->name;
  }

  return $options;
}
