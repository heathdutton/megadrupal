<?php
/**
 * @file
 * Administrative pages for the Background Audio module.
 */

function background_audio_settings_form($form, &$form_state) {
  $form['vtabs'] = array(
    '#type' => 'vertical_tabs',
  );

  $form['vtabs']['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Options'),
  );
  $form['vtabs']['options']['background_audio_position'] = array(
    '#type' => 'radios',
    '#title' => t('Player position'),
    '#options' => array(
      'topleft' => t('Top left'),
      'topright' => t('Top right'),
      'bottomleft' => t('Bottom left'),
      'bottomright' => t('Bottom right'),
    ),
    '#default_value' => variable_get('background_audio_position', 'bottomright'),
  );
  $form['vtabs']['options']['background_audio_playlist_position'] = array(
    '#type' => 'radios',
    '#title' => t('Playlist position'),
    '#options' => array(
      'top' => t('Top'),
      'bottom' => t('Bottom'),
    ),
    '#default_value' => variable_get('background_audio_playlist_position', 'top'),
  );
  $form['vtabs']['options']['background_audio_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Player width'),
    '#size' => 3,
    '#maxlength' => 4,
    '#element_validate' => array('element_validate_number'),
    '#default_value' => variable_get('background_audio_width', 400),
    '#required' => TRUE,
  );
  $form['vtabs']['options']['background_audio_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Player height'),
    '#size' => 3,
    '#maxlength' => 4,
    '#element_validate' => array('element_validate_number'),
    '#default_value' => variable_get('background_audio_height', 30),
    '#required' => TRUE,
  );
  $form['vtabs']['options']['background_audio_volume'] = array(
    '#type' => 'textfield',
    '#title' => t('Player volume level'),
    '#size' => 3,
    '#maxlength' => 3,
    //'#element_validate' => array('element_validate_number'),
    '#default_value' => variable_get('background_audio_volume', 0.8),
    '#required' => TRUE,
  );
  $form['vtabs']['options']['background_audio_options'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Default player options'),
    '#options' => background_audio_mejs_options(),
    '#default_value' => variable_get('background_audio_options', background_audio_mejs_options_defaults()),
  );
  $form['vtabs']['options']['background_audio_debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Debug mode'),
    '#description' => t('When in debug mode, extra information will be logged into Drupal log.'),
    '#default_value' => variable_get('background_audio_debug', 0),
  );

  $form['vtabs']['controls'] = array(
    '#type' => 'fieldset',
    '#title' => t('Controls'),
    '#theme' => 'background_audio_controls_table',
    '#tree' => TRUE,
  );
  $controls = variable_get('background_audio_controls', background_audio_mejs_controls_defaults());
  $enabled_controls = array_filter($controls);
  $weighted_controls = array_merge($controls, background_audio_mejs_controls());
  $weight = -10;
  foreach ($weighted_controls as $control => $title) {
    $form['vtabs']['controls'][$control][$control . '_enabled'] = array(
      '#type' => 'checkbox',
      '#title' => $title,
      '#default_value' => array_key_exists($control, $enabled_controls) ? 1 : 0,
    );
    $form['vtabs']['controls'][$control][$control . '_weight'] = array(
      '#type' => 'weight',
      '#title' => t('Weight'),
      '#title_display' => 'invisible',
      '#delta' => 10,
      '#default_value' => $weight,
      '#attributes' => array('class' => array('background-audio-control-weight')),
    );
    $weight++;
  }

  // Adding our own submit handler.
  $form['#submit'][] = 'background_audio_settings_form_submit';
  return system_settings_form($form);
}

function background_audio_settings_form_submit($form, &$form_state) {
  $control_weights = array();
  foreach (background_audio_mejs_controls() as $control => $title) {
    $control_weights[$control] = $form_state['values']['controls'][$control][$control . '_weight'];
  }
  asort($control_weights);
  $controls = array();
  foreach ($control_weights as $control => $weight) {
    $controls[$control] = $form_state['values']['controls'][$control][$control . '_enabled'] ? $control : 0;
  }
  variable_set('background_audio_controls', $controls);
  unset($form_state['values']['controls']);
}
