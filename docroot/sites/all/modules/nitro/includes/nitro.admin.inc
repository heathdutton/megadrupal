<?php

/**
 * @todo
 */
function nitro_theme_reset_confirm($form, &$form_state, $theme) {
  $themes = list_themes();
  $form_state['theme'] = $theme;

  return confirm_form($form,
    t('Are you sure you want to reset the theme %name?', array('%name' => $themes[$theme]->info['name'])),
    'admin/appearance/settings/' . $theme,
    t('<p>You are about to reset the theme setting to the original defaults in the .info file.</p><p>This cannot be undone.</p>'),
    t('Reset'),
    t('Cancel')
  );
}

/**
 * @todo
 */
function nitro_theme_reset_confirm_submit($form, &$form_state) {
  $themes = list_themes();
  $theme = $form_state['theme'];

  nitro_revert_theme_settings($theme);

  $form_state['redirect'] = 'admin/appearance/settings/' . $theme;
}

/**
 * @todo
 */
function nitro_theme_export($form, &$form_state, $theme) {
  $form_state['theme'] = $theme;
  $info = drupal_parse_info_file(drupal_get_path('theme', $theme) . '/' . $theme . '.info');
  $settings = isset($info['settings']) ? $info['settings'] : array();
  $settings = variable_get('theme_' . $theme . '_settings', $settings);

  $form['settings'] = array(
    '#type' => 'textarea',
    '#title' => t('Theme settings'),
    '#description' => t('You may copy these directly into a .info file and use as theme settings defaults'),
    '#default_value' => nitro_build_info_file($settings, 'settings'),
    '#rows' => 30,
    '#element_validate' => array('_nitro_validate_theme_settings'),
  );

  return $form;
}