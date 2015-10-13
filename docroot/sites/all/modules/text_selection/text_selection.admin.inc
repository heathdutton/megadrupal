<?php
/**
 * @file
 * Administration UI functions for the Text Selection module.
 */

/**
 * Form constructor for the Text Selection Configuration form.
 *
 * @see text_selection_settings_form_validate()
 * @see text_selection_settings_form_submit()
 * @ingroup forms
 */
function text_selection_settings_form() {
  $form['text_selection_bg_color'] = array(
    '#type' => 'textfield',
    '#size' => 7,
    '#maxlength' => 7,
    '#title' => t('Background Color'),
    '#default_value' => variable_get('text_selection_bg_color', '#000000'),
    '#description' => t('Determines the background color for text selection. Format: #000000.'),
  );
  $form['text_selection_font_color'] = array(
    '#type' => 'textfield',
    '#size' => 7,
    '#maxlength' => 7,
    '#title' => t('Font Color'),
    '#default_value' => variable_get('text_selection_font_color', '#FFFFFF'),
    '#description' => t('Determines the font color for text selection. Format: #FFFFFF.'),
  );

  return system_settings_form($form);
}

/**
 * Form validation handler for text_selection_settings_form().
 *
 * @see text_selection_settings_form()
 * @see text_selection_settings_form_submit()
 */
function text_selection_settings_form_validate($form, &$form_state) {
  if (!empty($form_state['values']['text_selection_bg_color'])) {
    $bg_color = $form_state['values']['text_selection_bg_color'];
    if (!preg_match("/^(#)([0-9a-fA-F]{3})([0-9a-fA-F]{3})?$/", $bg_color)) {
      form_set_error('text_selection_bg_color', t('The background color must fit the hexadecimal HTML color default format.'));
    }
  }
  else {
    form_set_error('text_selection_bg_color', t('The background color value is missing.'));
  }
  if (!empty($form_state['values']['text_selection_font_color'])) {
    $font_color = $form_state['values']['text_selection_font_color'];
    if (!preg_match("/^(#)([0-9a-fA-F]{3})([0-9a-fA-F]{3})?$/", $font_color)) {
      form_set_error('text_selection_font_color', t('The font color must fit the hexadecimal HTML color default format.'));
    }
  }
  else {
    form_set_error('text_selection_font_color', t('The font color value is missing.'));
  }
}

/**
 * Form submission handler for text_selection_settings_form().
 *
 * Saves the text selection color values.
 *
 * @see text_selection_settings_form()
 * @see text_selection_settings_form_validate()
 */
function text_selection_settings_form_submit($form, &$form_state) {
  variable_set('text_selection_bg_color', $form_state['values']['text_selection_bg_color']);
  variable_set('text_selection_font_color', $form_state['values']['text_selection_font_color']);
}
