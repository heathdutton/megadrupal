<?php
/**
 * @file
 * Provides administration functions for the slim_time module.
 *
 * @ingroup slim_time
 * @{
 */

/**
 * Helper function for the slim time widget settings.
 *
 * @see slim_time_field_widget_settings_form().
 */
function _slim_time_field_widget_settings_form($field, $instance) {
  $widget   = $instance['widget'];
  $settings = $widget['settings'] + field_info_widget_settings($widget['type']);
  $form     = array();
  
  switch ($widget['type']) {
    case 'slim_time_textfield':
      $form['size'] = array(
        '#type' => 'textfield',
        '#title' => t('Form width'),
        '#description' => t('Width the of the form input element.'),
        '#default_value' => $settings['size'],
        '#required' => TRUE,
        '#size' => 10,
      );

      $form['slim_time'] = array(
        '#type' => 'fieldset',
        '#title' => t('Slim Time options'),
        '#collapsible' => FALSE,
      );
      $form['slim_time_fuzzy'] = array(
        '#type' => 'checkbox',
        '#title' => t('Fuzzy'),
        '#default_value' => $settings['slim_time_fuzzy'],
        '#description' => t('Recognize time from within a phrase.'),
      );
      $form['slim_time_default'] = array(
        '#type' => 'radios',
        '#title' => t('Format'),
        '#default_value' => $settings['slim_time_default'],
        '#options' => array(
          12 => t('12 hour'),
          24 => t('24 hour'),
        ),
      );
      $form['slim_time_assume'] = array(
        '#type' => 'radios',
        '#title' => t('Assume'),
        '#description' => t('What to assume when missing a suffix.'),
        '#default_value' => $settings['slim_time_assume'],
        '#options' => array(
          'am' => t('am'),
          'pm' => t('pm'),
        ),
      );
      $form['slim_time_colon'] = array(
        '#type' => 'select',
        '#title' => t('Colon handling'),
        '#description' => t('To allow en entry like "615" for 6:15am set this to <em>optional</em>.  To not use a colon, set this to <em>none</em>.'),
        '#default_value' => $settings['slim_time_colon'],
        '#options' => array(
          'none' => t('None'),
          'optional' => t('Optional'),
          'required' => t('Required'),
        ),
      );
      $form['slim_time_seconds'] = array(
        '#type' => 'radios',
        '#title' => t('Granularity'),
        '#default_value' => (int) $settings['slim_time_seconds'],
        '#options' => array(
          0 => t('hours & mins'),
          1 => t('hours, mins & seconds'),
        ),
      );
      break;
  }

  return $form;  
}

/**
 * Form validation handler.
 */
function _slim_time_field_widget_settings_form_validate($form, &$form_state) {
  $slim_time_settings = $form_state['values']['instance']['widget']['settings'];
  $length             = $form_state['values']['field']['settings']['max_length'];

  if ($length < ($required = _slim_time_get_max_length($slim_time_settings))) {
    form_set_error('max_length', t('The Slim Time settings require more characters of field storage than is defined for this field; please increase the <em>Maximum length</em> of this field to at least %count, if possible, or change the Slim Time settings that are requiring more characters, e.g. <em>Granularity</em> support for seconds.', array('%count' => $required)));
  }    
}