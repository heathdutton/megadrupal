<?php

/**
 * Returns HTML for a file upload form element.
 *
 * For assistance with handling the uploaded file correctly, see the API
 * provided by file.inc.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: An associative array containing the properties of the element.
 *     Properties used: #title, #name, #size, #description, #required,
 *     #attributes.
 *
 * @ingroup themeable
 */
function twbs_file($variables) {
  $element = $variables['element'];
  $element['#attributes']['type'] = 'file';
  element_set_attributes($element, array('id', 'name', 'size'));
  _form_set_class($element, array('form-file'));

  return '<input' . drupal_attributes($element['#attributes']) . ' />';
}
