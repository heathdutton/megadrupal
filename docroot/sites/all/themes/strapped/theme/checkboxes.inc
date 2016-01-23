<?php
/**
 * @file
 * Checkboxes related theme functions.
 */


/**
 * Returns HTML for a set of checkbox form elements.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: An associative array containing the properties of the element.
 *     Properties used: #children, #attributes.
 *
 * @ingroup themeable
 */
function strapped_checkboxes($variables) {
  $element = $variables['element'];
  $attributes = array();
  if (isset($element['#id'])) {
    $attributes['id'] = $element['#id'];
  }
  $attributes['class'][] = 'form-checkboxes';
  if (!empty($element['#attributes']['class'])) {
    $attributes['class'] = array_merge($attributes['class'], $element['#attributes']['class']);
  }
  if (isset($element['#attributes']['title'])) {
    $attributes['title'] = $element['#attributes']['title'];
  }
  return '<div' . drupal_attributes($attributes) . '>' . (!empty($element['#children']) ? $element['#children'] : '') . '</div>';
}


/**
 * Returns HTML for a checkbox form element.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: An associative array containing the properties of the element.
 *     Properties used: #id, #name, #attributes, #checked, #return_value.
 *
 * @ingroup themeable
 */
function strapped_checkbox($variables) {
  $element = $variables['element'];
  $element['#attributes']['type'] = 'checkbox';
  element_set_attributes($element, array(
    'id',
    'name',
    '#return_value' => 'value'
  ));

  // Unchecked checkbox has #value of integer 0.
  if (!empty($element['#checked'])) {
    $element['#attributes']['checked'] = 'checked';
  }
  _form_set_class($element, array('form-checkbox'));

  return '<input' . drupal_attributes($element['#attributes']) . ' />';
}



function strapped_bef_checkbox($variables) {
  $element = $variables['element'];
  $value = check_plain($variables['value']);
  $label = filter_xss_admin($variables['label']);
  $selected = $variables['selected'];
  $id = drupal_html_id($element['#id'] . '-' . $value);
  // Custom ID for each checkbox based on the <select>'s original ID.
  $properties = array(
    '#required' => FALSE,
    '#id' => $id,
    '#type' => 'checkbox',
    '#name' => $id,
    '#description' => isset($element['#bef_term_descriptions'][$value]) ? $element['#bef_term_descriptions'][$value] :
        '',
  );

  // Prevent the select-all-none class from cascading to all checkboxes.
  if (!empty($element['#attributes']['class'])
    && FALSE !== ($key = array_search('bef-select-all-none', $element['#attributes']['class']))) {
    unset($element['#attributes']['class'][$key]);
  }

  // Prevent the form-control class from cascading to all checkboxes.
  if (!empty($element['#attributes']['class'])
    && FALSE !== ($key = array_search('form-control', $element['#attributes']['class']))) {
    unset($element['#attributes']['class'][$key]);
  }

  // Add the checkbox class
  $element['#attributes']['class'][] = 'form-checkbox';

  // Unset the name attribute as we are setting it manually.
  unset($element['#attributes']['name']);

  // Unset the multiple attribute as it doesn't apply for checkboxes.
  unset ($element['#attributes']['multiple']);

  $checkbox = '<input type="checkbox" '
    // Brackets are key -- just like select.
    . 'name="' . $element['#name'] . '[]" '
    . 'id="' . $id . '" '
    . 'value="' . $value . '" '
    . ($selected ? 'checked="checked" ' : '')
    . drupal_attributes($element['#attributes']) . ' />';
  $properties['#title_display'] = 'after';
  $properties['#title'] = $label;
  $properties['#children'] = "$checkbox";
  $output = theme('form_element', array('element' => $properties));
  return $output;
}

/**
 * @param $element
 * @return array
 */
function strapped_process_checkboxes($element) {


  // If a webform has an element_offset  set via the strapon module then add it to the right place
  if (isset($element['#webform_component']) && isset($element['#webform_component']['extra']['grid']['element_columns'])) {
    $element += array(
      '#element_columns' => $element['#webform_component']['extra']['grid']['element_columns'],
      '#form_type' => $element['#webform_component']['extra']['form_type'],
    );
  }

  $element['#element_columns'] = array_filter(array_map('array_filter', $element['#element_columns']));
  if (isset($element['#element_columns']) && !empty($element['#element_columns'])  &&  $element['#form_type'] == 'horizontal') {

    $element['#attributes'] = array('class' => array('row'));
    // If there was a columns attribute set on the parent then apply it toall the children
    foreach (element_children($element) as $key) {
      if (is_numeric($key)) {

        $element[$key]['#element_columns'] =  $element['#element_columns'];
        // Remo the form group wrapper
        $element[$key]['#form_group_wrapper'] = FALSE;
        // remove the radio wrapper as it adds padding.
        $element[$key]['#checkbox_wrapper'] = FALSE;
      }
    }
    // after copying the data to the children, remove it from the parent
    unset($element['#element_columns']);
  }
  return $element;
}

function element_validate_checkboxes($element, &$form_state) {
  $count = count($element['#value']);
  $options_max = $element['#options_selected_max'];
  $options_min = $element['#options_selected_min'];
  $parent = array_shift($element['#parents']);
  if ($count > $options_max) {
    form_set_error($parent, 'Maximum selection limit of ' . $options_max  . ' exceeded.');
  }
  if ($count < $options_min) {
    form_set_error($parent, 'Minimum selection limit of ' . $options_min  . ' not reached.');
  }
}