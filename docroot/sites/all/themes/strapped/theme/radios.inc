<?php
/**
 * @file
 * Radios related theme functions.
 */


/**
 * Returns HTML for a radio button form element.
 *
 * Note: The input "name" attribute needs to be sanitized before output, which
 *       is currently done by passing all attributes to drupal_attributes().
 *
 * @param $variables
 *   An associative array containing:
 *   - element: An associative array containing the properties of the element.
 *     Properties used: #required, #return_value, #value, #attributes, #title,
 *     #description
 *
 * @ingroup themeable
 */
function strapped_radio($variables) {
  $element = $variables['element'];
  $element['#attributes']['type'] = 'radio';
  element_set_attributes($element, array(
    'id',
    'name',
    '#return_value' => 'value'
  ));

  if (isset($element['#return_value']) && $element['#value'] !== FALSE && $element['#value'] == $element['#return_value']) {
    $element['#attributes']['checked'] = 'checked';
  }
  _form_set_class($element, array('form-radio'));

  return '<input' . drupal_attributes($element['#attributes']) . ' />';
}

/**
 * Returns HTML for a set of radio button form elements.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: An associative array containing the properties of the element.
 *     Properties used: #title, #value, #options, #description, #required,
 *     #attributes, #children.
 *
 * @ingroup themeable
 */
function strapped_radios($variables) {
  $element = $variables['element'];
  $attributes = array();
  if (isset($element['#id'])) {
    $attributes['id'] = $element['#id'];
  }
  $attributes['class'] = 'form-radios';
  if (!empty($element['#attributes']['class'])) {
    $attributes['class'] .= ' ' . implode(' ', $element['#attributes']['class']);
  }
  if (isset($element['#attributes']['title'])) {
    $attributes['title'] = $element['#attributes']['title'];
  }
  return '<div' . drupal_attributes($attributes) . '>' . (!empty($element['#children']) ? $element['#children'] : '') . '</div>';
}


/**
 * @param $element
 * @return array
 */
function strapped_process_radios($element) {


  // If a webform has an element_offset  set via the strapon module then add it to the right place
  if (isset($element['#webform_component']) && isset($element['#webform_component']['extra']['element_columns'])) {
    $element += array(
      '#element_columns' => $element['#webform_component']['extra']['element_columns'],
    );
  }



  if (!isset($element['#form_type']))  {
    $element['#form_group_wrapper'] = FALSE;
    foreach (element_children($element) as $key) {
      if (is_numeric($key)) {
        $element[$key]['#form_group_wrapper'] = FALSE;
        $element[$key]['#form_type_wrapper'] = FALSE;
      }
    }
  }



  // If a set of radios has the element inline flag set then copy it to all the children.
  if (isset($element['#element_inline'])) {
    foreach (element_children($element) as $key) {
      if (is_numeric($key)) {
        $element[$key]['#element_inline'] = TRUE;
        $element[$key]['#form_group_wrapper'] = FALSE;
        $element[$key]['#form_field_wrapper'] = FALSE;
        $element[$key]['#form_type_wrapper'] = FALSE;
      }
    }
  }

  // @todo what happens if someone makes up a column number that is not a postivie divisor of 12.
  if (isset($element['#element_columns']) && isset($element['#form_type']) &&  $element['#form_type'] == 'horizontal') {

    $element['#attributes'] = array('class' => array('row'));
    // If there was a columns attribute set on the parent then apply it to the
    foreach (element_children($element) as $key) {
      if (is_numeric($key) || 1==1) {
        // dpm($element[$key]);
        // Radios in columns shouldnt have the type wrapper
        $element[$key]['#form_type_wrapper'] = FALSE;
        $element[$key]['#form_group_wrapper'] = FALSE;
        $element[$key]['#element_columns_class'] = 'col-md-' . 12 / $element['#element_columns'];
      }
    }

  }


  return $element;
}