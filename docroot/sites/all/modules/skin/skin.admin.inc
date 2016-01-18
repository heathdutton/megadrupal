<?php

/**
 * Settings form.
 */
function skin_settings_form($form, &$form_state) {
  $form['skin_paths'] = array(
    '#title' => t('Skin paths'),
    '#type' => 'textarea',
    '#description' => t("Enter each path you want skinned, one per line. The '*' character is a wildcard. %front is the front page.", array('%front' => '<front>')),
    '#default_value' => variable_get('skin_paths', ''),
  );
  $form['skin_cache'] = array(
    '#type' => 'radios',
    '#title' => t('Cache CSS and Javascript files'),
    '#description' => t('Optimized CSS and Javascript files are frequently deleted, which could break your skin. You can cache copies of these files so they persist until the next time you build your skin. You can also adjust your optimization settings on the <a href="@url">performance page</a>.', array('@url' => url('admin/config/development/performance'))),
    '#options' => array(
      0 => "Don't cache any files",
      1 => "Cache optimized CSS and JS files",
      2 => "Cache all CSS and JS files",
    ),
    '#default_value' => variable_get('skin_cache', 1),
  );
  $form['skin_directory'] = array(
    '#type' => 'textfield',
    '#title' => t('Skin directory'),
    '#description' => t('The directory where copies of CSS and Javascript files are cached.'),
    '#default_value' => variable_get('skin_directory', variable_get('file_public_path', conf_path() . '/files') . '/skin'),
  );
  $form['skin_clear_cache'] = array(
    '#type' => 'checkbox',
    '#title' => t('Clear cached CSS and Javascript skin files.'),
    '#description' => t('Warning: this could cause CSS and Javascript paths to become outdated in your skins.'),
  );
  $form['#submit'][] = 'skin_settings_form_submit';
  return system_settings_form($form);
}

/**
 * Settings form validate.
 */
function skin_settings_form_validate($form, &$form_state) {
  // The directory must be defined if Skin is configured to cache files.
  if ($form_state['values']['skin_cache'] > 0 and empty($form_state['values']['skin_directory'])) {
    form_set_error('skin_directory', t('Please enter a skin directory.'));
  }
}

/**
 * Settings form submit.
 */
function skin_settings_form_submit($form, &$form_state) {
  if ($form_state['values']['skin_clear_cache']) {
    skin_clear_cache();
  }
}
