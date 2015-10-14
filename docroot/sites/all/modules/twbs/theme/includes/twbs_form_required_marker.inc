<?php

/**
 * Returns HTML for a marker for required form elements.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: An associative array containing the properties of the element.
 *
 * @ingroup themeable
 */
function twbs_form_required_marker($variables) {
  // This is also used in the installer, pre-database setup.
  $t = get_t();
  $attributes = array(
    'class' => 'form-required',
    'title' => $t('This field is required.'),
  );
  return '<span' . drupal_attributes($attributes) . '>*</span>';
}
