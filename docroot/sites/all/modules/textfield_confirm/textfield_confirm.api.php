<?php

/**
 * @file
 * Form options provided by the Textfield confirm module.
 */

/**
 * Sample form that demonstrates element settings.
 *
 * The same applies to the emailfield_confirm element.
 */
function example_form($form, &$form_state) {
  $form['testfield'] = array(
    '#type' => 'textfield_confirm', // Or 'emailfield_confirm'.

    // Message text.
    '#error_message' => t('The specified fields do not match.'),
    '#success_help' => t('Fields match: <span class="ok">yes</span>'),
    '#error_help' => t('Fields match: <span class="error">no</span>'),

    // Apply to the wrapper element.
    '#title' => t('The wrapper title'),
    '#description' => t('The wrapper description.'),

    // Individual properties.
    '#primary_title' => t('Field one'),
    '#primary_description' => t('This is the first field.'),
    '#secondary_title' => t('Field two'),
    '#secondary_description' => t('This is the second field. The value must match the first field.'),

    '#primary_attributes' => array('class' => array('class-applied-individually')),
    '#primary_autocomplete_path' => 'beep',
    '#primary_placeholder' => t('Field one placeholder'),
    // Etc.

    // Common properties. These will be set on each field and removed from the
    // wrapper.
    '#size' => 60,
    '#maxlength' => 100,
    '#required' => TRUE,
    '#autocomplete_path' => 'boop',
    '#placeholder' => t('I am a placeholder'),
  );

  return $form;
}
