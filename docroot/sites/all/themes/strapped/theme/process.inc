<?php
/**
 * @file
 * Preprocess theme functions.
 */

/**
 * Implements hook_element_info_alter().
 *
 * @param $elements
 */
function strapped_element_info_alter(&$elements) {
  foreach ($elements as $key => &$element) {

    // Process input elements.
    if (!empty($element['#input'])) {
      $element['#process'][] = '_strapped_process_input';
    }

    $elements[$key]['#process'][] = '_strapped_process_element';

    // Change back to fieldset (bootstrap parent theme changed it)
    if (!empty($element['#type']) && $element['#type'] === 'fieldset') {
      $element['#theme_wrappers'] = array('fieldset');
    }

    // Change back to fieldset (bootstrap parent theme changed it)
    if (!empty($element['#theme']) && $element['#theme'] === 'fieldset') {
      $element['#theme'] = 'fieldset';
    }

    // Change back to fieldset (bootstrap parent theme changed it)
    if (!empty($element['#theme_wrappers']) && ($key = array_search('fieldset', $element['#theme_wrappers'])) !== FALSE) {
      $element['#theme_wrappers'][$key] = 'fieldset';
    }

    // Set some defaults for all elements
    $element['#form_group_wrapper'] = TRUE;
    $element['#form_type_wrapper'] = TRUE; // Show the form-type wrapper by default
    $element['#form_field_wrapper'] = TRUE; // Show the standard drupal element wrapper by default



  }

  $elements['vertical_tabs']['#pre_render'][] = 'strapped_process_vertical_tabs';
  $elements['text_format']['#process'][] = 'strapped_process_text_format';
  $elements['checkboxes']['#process'][] = 'strapped_process_checkboxes';
  $elements['radios']['#process'][] = 'strapped_process_radios';

  $elements['date_popup']['#process'][] = 'strapped_process_date_popup';
  $elements['media_element']['#process'][] = 'strapped_process_media_element';


}


/**
 * Run through the child elements of vertical tabs and remove the form wrappers that interfere with verticaltabs.js
 *
 * @param $element
 *   An associative array containing the properties and children of the
 *   fieldset.
 * @param $form_state
 *   The $form_state array for the form this vertical tab widget belongs to.
 *
 * @return
 *   The processed element.
 */
function strapped_process_vertical_tabs($element) {

  foreach (element_children($element) as $key => $value) {
    if (is_array( $element[$value])) {
      $element[$value]['#form_group_wrapper'] = FALSE;
      $element[$value]['#form_type_wrapper'] = FALSE;
      $element[$value]['#form_field_wrapper'] = FALSE;
    }

  }

  return $element;
}


/**
 * Callback for processing all elements.
 *
 * @param $element
 * @return mixed
 */
function _strapped_process_element($element) {
  // If the parents form type is set then propagate it throughout the children
  // This is useful for a checkbox where they render differently when inside a checkboxes element.
  if (isset($element['#form_type'])) {
    foreach (element_children($element) as $key => $value) {
      $element[$value]['#form_parent_type'] = $element['#form_type'];
      $element[$value]['#parent_type'] = $element['#type'];
    }
  }
  return $element;
}

function _strapped_process_input($element) {

  // Fix for elements without form-control
  $elements_needing_form_control = array('webform_number', 'webform_email');
  if (isset($element['#theme']) && in_array($element['#theme'],$elements_needing_form_control)) {
    $element['#attributes']['class'][] = 'form-control';
  }

  $elements_hating_form_control = array('select_as_checkboxes');
  if (isset($element['#theme']) && in_array($element['#theme'],$elements_hating_form_control)) {

    if (!empty($element['#attributes']['class'])
      && FALSE !== ($key = array_search('form-control', $element['#attributes']['class']))) {
      unset($element['#attributes']['class'][$key]);
    }
  }

  return $element;

}