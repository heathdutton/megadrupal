<?php

/**
 * @file
 * Webform module bt_layout_row component.
 */

/**
 * Implements _webform_edit_component().
 */
function _webform_edit_bt_layout_row($component) {
  $form = array();
  // Hide name, it's never shown
  $form['name']['#type'] = 'value';
  // Hack - name is required by webform but we don't expose it to the user. Instead we'll replace it with the value of form_key.
  $form['name']['#value'] = 'bt_row';
  return $form;
}

/**
 * Implements _webform_render_component().
 */
function _webform_render_bt_layout_row($component, $value = NULL, $filter = TRUE) {
  $element = array(
    '#weight' => $component['weight'],
    '#pre_render' => array('webform_bt_layout_row_prerender'),
    '#webform_component' => $component,
    '#prefix' => '<div>',
    '#suffix' => '</div>',
  );
  return $element;
}

/**
 * Pre-render function to set a bt_layout_row ID and classes.
 */
function webform_bt_layout_row_prerender($element) {
  $attributes = empty($element['#attributes']) ? array('class' => array()) : $element['#attributes'];
  $attributes['class'][] = 'row';
  $attributes['class'][] = 'webform-component--' . str_replace('_', '-', implode('--', array_slice($element['#parents'], 1)));
  // css class for number of children
  $n = empty($element['#webform_component']['children']) ? 0 : count($element['#webform_component']['children']);
  $element['#prefix'] = '<div ' . drupal_attributes($attributes) . '>';
  return $element;
}

/**
 * Implements _webform_display_component().
 */
function _webform_display_bt_layout_row($component, $value, $format = 'html') {
  if ($format == 'text') {
    $element = array(
      '#title' => '',
      '#weight' => $component['weight'],
      '#theme_wrappers' => array(),
    );
  }
  else {
    $element = _webform_render_bt_layout_row($component, $value);
  }

  $element['#format'] = $format;

  return $element;
}
