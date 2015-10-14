<?php

/**
 * Theme preprocess function for theme_field() and field.tpl.php.
 *
 * @see theme_field()
 * @see field.tpl.php
 */
function twbs_preprocess_field(&$variables, $hook) {
  $element = $variables['element'];

  // There's some overhead in calling check_plain() so only call it if the label
  // variable is being displayed. Otherwise, set it to NULL to avoid PHP
  // warnings if a theme implementation accesses the variable even when it's
  // supposed to be hidden. If a theme implementation needs to print a hidden
  // label, it needs to supply a preprocess function that sets it to the
  // sanitized element title or whatever else is wanted in its place.
  $variables['label_hidden'] = ($element['#label_display'] == 'hidden');
  $variables['label'] = $variables['label_hidden'] ? NULL : check_plain($element['#title']);

  // We want other preprocess functions and the theme implementation to have
  // fast access to the field item render arrays. The item render array keys
  // (deltas) should always be a subset of the keys in #items, and looping on
  // those keys is faster than calling element_children() or looping on all keys
  // within $element, since that requires traversal of all element properties.
  $variables['items'] = array();
  foreach ($element['#items'] as $delta => $item) {
    if (!empty($element[$delta])) {
      $variables['items'][$delta] = $element[$delta];
    }
  }

  // Add default CSS classes. Since there can be many fields rendered on a page,
  // save some overhead by calling strtr() directly instead of
  // drupal_html_class().
  $variables['field_name_css'] = strtr($element['#field_name'], '_', '-');
  $variables['field_type_css'] = strtr($element['#field_type'], '_', '-');
  $variables['classes_array'] = array(
    'field',
    'field-name-' . $variables['field_name_css'],
    'field-type-' . $variables['field_type_css'],
    'field-label-' . $element['#label_display'],
  );
  // Add a "clearfix" class to the wrapper since we float the label and the
  // field items in field.css if the label is inline.
  if ($element['#label_display'] == 'inline') {
    $variables['classes_array'][] = 'clearfix';
  }

  // Add specific suggestions that can override the default implementation.
  $variables['theme_hook_suggestions'] = array(
    'field__' . $element['#field_type'],
    'field__' . $element['#field_name'],
    'field__' . $element['#bundle'],
    'field__' . $element['#field_name'] . '__' . $element['#bundle'],
  );
}
