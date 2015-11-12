<?php

/**
 * @file
 *  This is the SEO Preview admin include which provides an interface to change some of the default settings
 */

/**
 *  Function to generate the form setting array
 */
function seo_preview_settings() {
  $settings = _seo_preview_get_settings();

  $form['settings'] = array(
    '#tree' => TRUE,
  );
  $form['settings']['minimize_by_default'] = array(
    '#type' => 'checkbox',
    '#title' => t('Minimize by default'),
    '#description' => t('If enabled, the SEO Preview box will be minimized by default.'),
    '#default_value' => $settings['minimize_by_default'],
  );
  $form['settings']['expand_on_warning'] = array(
    '#type' => 'checkbox',
    '#title' => t('Expand on warnings'),
    '#description' => t('If enabled, the SEO Preview box will expand on warnings.'),
    '#default_value' => $settings['expand_on_warning'],
  );
  $form['settings']['display_hints'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display SEO hints'),
    '#description' => t('If enabled, the SEO Preview box will display SEO hints in addition to the snippet preview.'),
    '#default_value' => $settings['display_hints'],
  );
  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#submit' => array('seo_preview_settings_submit_save'),
    '#value' => t('Save Configuration'),
  );

  return $form;
}

/**
 * Save submit handler for the seo_preview_settings form.
 * Compares the submitted settings to the defaults and unsets any that are equal. This was we only store overrides.
 */
function seo_preview_settings_submit_save($form_id, &$form_state) {
  // Grab the defaults
  $defaults = _seo_preview_get_settings(TRUE);

  // Copy out the settings
  $settings = $form_state['values']['settings'];

  // Compare each setting to the default. If equal, remove. If not, cast to an int (FormAPI converts keys to string).
  foreach ($settings as $key => $value) {
    if ($value == $defaults[$key]) {
      unset($settings[$key]);
    }
    else {
      $settings[$key] = (int)$value;
    }
  }

  // If we've ended up with an empty settings array, delete the settings variable...
  if (empty($settings)) {
    variable_del('seo_preview_settings');
  }
  // ... otherwise store the settings
  else {
    variable_set('seo_preview_settings', $settings);
  }
  drupal_set_message(t('SEO Preview settings have been saved.'));
}
