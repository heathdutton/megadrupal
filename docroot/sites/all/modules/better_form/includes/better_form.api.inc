<?php

/**
 * @file
 * Adding an attributes to the form fields.
 */

/**
 * Checks fields and process these to add 'required' attribute.
 */
function better_form_add_required_attribute_fields(&$form_elements) {
  // Check variable name.
  $exception_fields = array('checkboxes', 'radios');
  foreach (element_children($form_elements) as $element) {
    $current_element = &$form_elements[$element];
    if (isset($current_element['#type'])) {
      if (isset($current_element['#required'])) {
        if ($current_element['#required'] == TRUE) {
          if (in_array($current_element['#type'], $exception_fields)) {
            _better_form_add_required_attribute_class_field($current_element);
            continue;
          }
          // Add required attributes.
          _better_form_add_required_attribute_field($current_element);
          continue;
        }
        else {
          // Check next form element.
          continue;
        }
      }
    }
    // Check on next nesting level.
    better_form_add_required_attribute_fields($current_element);
  }
}

/**
 * Adds 'required' attribute to the form element.
 */
function _better_form_add_required_attribute_field(&$form_element) {
  $form_element['#attributes']['required'] = 'required';
  $form_element['#attributes']['aria-required'] = 'true';
  $form_element['#attributes']['title'] = t('This field is required!');
}

/**
 * Adds 'required-form-field' class to the form element.
 */
function _better_form_add_required_attribute_class_field(&$form_element) {
  $form_element['#attributes']['class'][] = 'required-form-field';
}

/**
 * Checks fields and process these to add 'placeholder' attribute.
 */
function better_form_add_placeholder_attribute_fields(&$form_elements) {
  foreach (element_children($form_elements) as $element) {
    $current_element = &$form_elements[$element];
    if (isset($current_element['#title'])) {
      if ($current_element['#title']) {
        // Add placeholder attributes.
        _better_form_add_placeholder_attribute_field($current_element);
        continue;
      }
      else {
        // Check next form element.
        continue;
      }
    }
    // Check on next nesting level.
    better_form_add_placeholder_attribute_fields($current_element);
  }
}

/**
 * Puts the field title to value of the 'placeholder' attribute.
 */
function _better_form_add_placeholder_attribute_field(&$form_elements) {
  if (empty($form_elements['#title'])) {
    return FALSE;
  }
  $form_elements['#attributes']['placeholder'] = $form_elements['#title'];
}
