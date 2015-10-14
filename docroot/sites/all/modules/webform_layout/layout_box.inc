<?php

/**
 * @file
 * Webform module layout_box component.
 */

/**
 * Implements _webform_defaults_component().
 */
function _webform_defaults_layout_box() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'pid' => 0,
    'weight' => 0,
    'extra' => array(
      'private' => FALSE,
      'align' => 'horiz',
    ),
  );
}

/**
 * Implements _webform_edit_component().
 */
function _webform_edit_layout_box($component) {
  $form = array();
  $form['display']['align'] = array(
    '#type' => 'select',
    '#title' => t('Alignment'),
    '#default_value' => $component['extra']['align'],
    '#description' => t('Controls how elements are arranged in this container. Choose "horizontal" to create a row, or "equal width" to create a row with equal column widths. The "vertical" option can be used to make columns within a horizontal box.'),
    '#parents' => array('extra', 'align'),
    '#options' => array(
      'horiz' => t('Horizontal'),
      'equal' => t('Equal Width'),
      'vert' => t('Vertical'),
    ),
  );
  // Hide name, it's never shown
  $form['name']['#type'] = 'value';
  // Hack - name is required by webform but we don't expose it to the user. Instead we'll replace it with the value of form_key.
  $form['name']['#value'] = 'box';
  return $form;
}

/**
 * Implements _webform_render_component().
 */
function _webform_render_layout_box($component, $value = NULL, $filter = TRUE) {
  $element = array(
    '#weight' => $component['weight'],
    '#pre_render' => array('webform_layout_box_prerender'),
    '#webform_component' => $component,
    '#prefix' => '<div>',
    '#suffix' => '</div>',
    '#attached' => array('css' => array(drupal_get_path('module', 'webform_layout') . '/layout_box.css')),
  );
  return $element;
}

/**
 * Pre-render function to set a layout_box ID and classes.
 */
function webform_layout_box_prerender($element) {
  $attributes = empty($element['#attributes']) ? array('class' => array()) : $element['#attributes'];
  $attributes['class'][] = 'webform-layout-box';
  $attributes['class'][] = $element['#webform_component']['extra']['align'];
  $attributes['class'][] = 'webform-component--' . str_replace('_', '-', implode('--', array_slice($element['#parents'], 1)));
  // css class for number of children
  $n = empty($element['#webform_component']['children']) ? 0 : count($element['#webform_component']['children']);
  if ($element['#webform_component']['extra']['align'] == 'equal' && $n > 1) {
    $attributes['class'][] = 'child-width-' . $n;
  }
  $element['#prefix'] = '<div ' . drupal_attributes($attributes) . '>';
  return $element;
}

/**
 * Implements _webform_display_component().
 */
function _webform_display_layout_box($component, $value, $format = 'html') {
  if ($format == 'text') {
    $element = array(
      '#title' => '',
      '#weight' => $component['weight'],
      '#theme_wrappers' => array(),
    );
  }
  else {
    $element = _webform_render_layout_box($component, $value);
  }

  $element['#format'] = $format;

  return $element;
}
