<?php

/**
 * @file
 *
 */

/**
 * Form constructor for the admin settings form.
 *
 * @see nomensa_amp_menu()
 */
function nomensa_amp_admin_settings_form($form, $form_state) {
  $form['nomensa_amp_settings']['#tree'] = TRUE;

  $form['nomensa_amp_settings']['players'] = array(
    '#title' => t('Enabled Players'),
    '#type' => 'fieldset',
  );

  $players = array(
    'youtube' => 'YouTube',
    'vimeo' => 'Vimeo',
    'jwplayer' => 'jwPlayer',
  );

  // Load the default settings.
  $defaults = variable_get('nomensa_amp_settings');

  // Create a checkbox for each player.
  foreach ($players as $key => $name) {
    $form['nomensa_amp_settings']['players'][$key] = array(
      '#title' => $name,
      '#type' => 'checkbox',
      '#default_value' => isset($defaults['players'][$key]) ? $defaults['players'][$key] : 0,
    );
  }

  // Advanced settings.
  $form['nomensa_amp_settings']['advanced'] = array(
    '#title' => t('Advanced settings'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['nomensa_amp_settings']['advanced']['minimised'] = array(
    '#title' => t('Use minified CSS and javascript'),
    '#type' => 'checkbox',
    '#default_value' => isset($defaults['advanced']['minimised']) ? $defaults['advanced']['minimised'] : 1,
  );

  $form = system_settings_form($form);

  return $form;
}
