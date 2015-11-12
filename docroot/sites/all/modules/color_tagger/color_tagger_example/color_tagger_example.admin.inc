<?php

/**
 * @file
 * Admin page callbacks for Color Tagger Example module.
 *
 * Provides a page to test color tagger matches for specific hex codes.
 */

/**
 * Debugging function for testing what term is matched to a given hexadecimal value.
 */
function _color_tagger_test_color_match($hex) {
  if ($rgb = _color_tagger_hex_to_rgb($hex)) {
    $use_lab = color_tagger_get_record('use_lab', 1);
    if ($use_lab) {
      $pixel_values = _color_tagger_rgb_to_lab($rgb);
    }
    else {
      $pixel_values = $rgb;
    }

    if ($color_palette = _color_tagger_get_color_palette('field_hex_code', 'colors')) {
      module_load_include('inc', 'color_tagger', 'ColorTaggerImage');
      $debug_image = new ColorTaggerImage(NULL, $color_palette);
      $match = $debug_image->findColorMatch($pixel_values);
      $term = taxonomy_term_load($match);
      drupal_set_message($term->name);
    }
  }
  else {
    drupal_set_message(t('"@hex" is an invalid hexadecimal value and could not be processed.', array('@hex' => $hex)), 'error');
  }
}

/**
 * Form callback for Color Tagger Test form.
 */
function color_tagger_test_colors_form($form, &$form_state) {
  $form['color'] = array(
    '#type' => 'textfield',
    '#title' => t('Hexadecimal Color'),
    '#default_value' => (isset($form_state['values']['color'])) ? $form_state['values']['color'] : '',
    '#size' => 10,
    '#maxlength' => 10,
    '#required' => TRUE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

/**
 * Form Submit callback for Color Tagger Test form.
 */
function color_tagger_test_colors_form_submit($form, &$form_state) {
  _color_tagger_test_color_match($form_state['values']['color']);
  $form_state['rebuild'] = TRUE;
}
