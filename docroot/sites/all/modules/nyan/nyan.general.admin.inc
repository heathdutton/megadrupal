<?php
/**
 * @file
 * Code for administrative menu callbacks.
 */

/**
 * Menu callback form for admin/config/system/nyan
 */
function nyan_general_settings($form, &$form_state) {
  // welcome banner fieldset
  $form['audio'] = array(
    '#type' => 'fieldset',
    '#title' => t('Audio'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['audio']['nyan_audio_enabled'] = array(
    '#type' =>'checkbox',
    '#title' => t('Enable audio'),
    '#default_value' => variable_get('nyan_audio_enabled', 1),
  );
  $form['audio']['nyan_audio_show_controls'] = array(
    '#type' =>'checkbox',
    '#title' => t('Show controls'),
    '#default_value' => variable_get('nyan_audio_show_controls', 1),
    '#states' => array(
      'visible' => array(
        ':input[name="nyan_audio_enabled"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['audio']['nyan_audio_initial_volume'] = array(
    '#type' => 'select',
    '#title' => t('Initial volume'),
    '#description' => t('If you prefer the initial volume louder or quieter, you can set it here.'),
    '#default_value' => variable_get('nyan_audio_initial_volume', '.50'),
    '#options' => array(
      '.10' => '10%',
      '.20' => '20%',
      '.30' => '30%',
      '.40' => '40%',
      '.50' => '50%',
      '.60' => '60%',
      '.70' => '70%',
      '.80' => '80%',
      '.90' => '90%',
      '1'   => '100%',
    ),
    '#required' => TRUE,
    '#states' => array(
      'visible' => array(
        ':input[name="nyan_audio_enabled"]' => array('checked' => TRUE),
      ),
    ),
  );

  return system_settings_form($form);
}
