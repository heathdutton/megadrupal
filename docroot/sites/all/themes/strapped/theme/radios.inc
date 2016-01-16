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
      '#element_columns' => $element['#webform_component']['extra']['grid']['element_columns'],
      '#form_type' => $element['#webform_component']['extra']['form_type'],
    );
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

  if (isset($element['#element_columns']) && $element['#form_type'] == 'horizontal') {
    // The radios are going to be come columns so we need a containing row.
    $element['#attributes'] = array('class' => array('row'));

    foreach (element_children($element) as $key) {
        // Copy the element_columns attributes to the children.
        $element[$key]['#element_columns'] =  $element['#element_columns'];
        // Remo the form group wrapper
        $element[$key]['#form_group_wrapper'] = FALSE;
        // remove the radio wrapper as it adds padding.
        $element[$key]['#checkbox_wrapper'] = FALSE;
    }
    // After copying the data to the children, remove it from the parent
    unset($element['#element_columns']);
  }


  return $element;
}