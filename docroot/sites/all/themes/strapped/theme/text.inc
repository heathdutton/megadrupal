<?php
/**
 * @file
 * Text related theme functions.
 */

/**
 * Overides theme_text_format_wrapper().
 *
 * @param $variables
 * @return string
 */
function strapped_text_format_wrapper($variables) {
  $element = $variables['element'];
  $output = '<div class="text-format-wrapper">';
  $output .= $element['#children'];
  if (!empty($element['#description'])) {
    $output .= '<div class="description">' . $element['#description'] . '</div>';
  }
  $output .= "</div>\n";

  return $output;
}

/**
 * Overides theme_process_text_format().
 * @param $element
 * @return mixed
 */
function strapped_process_text_format($element) {

  $element['#theme_wrappers'] = array('form_element');
  $element['value']['#title_display'] = 'none';
  $element['value']['#form_type'] = 'basic';
  $element['format']['#title_display'] = 'none';
  $element['format']['#form_type'] = 'basic';

  return $element;
}


/**
 * Overides theme_textarea().
 *
 * Returns HTML for a textarea form element.
 */
function strapped_textarea($variables) {


  $element = $variables['element'];
  // Grippie sucks!
  $element['#resizable'] = FALSE;
  element_set_attributes($element, array('id', 'name', 'cols', 'rows'));
  _form_set_class($element, array('form-textarea'));

  $wrapper_attributes = array(
    'class' => array('form-textarea-wrapper'),
  );

  // Add resizable behavior.
  if (!empty($element['#resizable'])) {
    drupal_add_library('system', 'drupal.textarea');
    $wrapper_attributes['class'][] = 'resizable';
  }

  $output = '<div' . drupal_attributes($wrapper_attributes) . '>';
  $output .= '<textarea' . drupal_attributes($element['#attributes']) . '>' . check_plain($element['#value']) . '</textarea>';
  $output .= '</div>';
  return $output;
}
