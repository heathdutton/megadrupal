<?php

/**
 * @file
 * Provides the Similar Entries Popup administrative interface.
 */

/**
 * Form builder for the admin display defaults page.
 */
function similar_popup_settings() {
  $form = array();

  $form['similar_popup_sound_notification'] = array(
    '#type' => 'checkbox',
    '#title' => t('Sound notification'),
    '#description' => t('Do you want to enable sound notification?'),
    '#default_value' => variable_get('similar_popup_sound_notification', FALSE),
  );
  $form['similar_popup_position'] = array(
    '#title' => t('Position'),
    '#description' => t('Where should the Similar Entries Popup appear?'),
    '#type' => 'select',
    '#options' => array(
      'top_left' => t('Top left'),
      'top_right' => t('Top right'),
      'bottom_right' => t('Bottom right'),
      'bottom_left' => t('Bottom left'),
    ),
    '#default_value' => variable_get('similar_popup_position', 'top_left'),
  );

  return system_settings_form($form);
}
