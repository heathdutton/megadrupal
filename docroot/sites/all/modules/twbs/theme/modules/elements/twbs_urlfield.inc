<?php

/**
 * Returns HTML for an urlfield form element.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: An associative array containing the properties of the element.
 *     Properties used: #title, #value, #description, #size, #maxlength,
 *     #placeholder, #required, #attributes, #autocomplete_path.
 *
 * @ingroup themeable
 */
function twbs_urlfield($variables) {
  $element = $variables['element'];
  $element['#attributes']['type'] = 'url';
  element_set_attributes($element, array('id', 'name', 'value', 'size', 'maxlength', 'placeholder'));
  _form_set_class($element, array('form-text', 'form-url'));

  $extra = elements_add_autocomplete($element);
  $output = '<input' . drupal_attributes($element['#attributes']) . ' />';

  return $output . $extra;
}
