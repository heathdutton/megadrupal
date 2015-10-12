<?php

/**
 * @file
 * Webform module bt_layout_cols component.
 */

/**
 * Implements _webform_defaults_component().
 */
function _webform_defaults_bt_layout_cols() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'pid' => 0,
    'weight' => 0,
    'extra' => array(
      'private' => FALSE,
      'bt_cols' => 'col-sm-12',
      'bt_breakpoint' => 'sm',
    ),
  );
}

/**
 * Implements _webform_edit_component().
 */
function _webform_edit_bt_layout_cols($component) {
  $form = array();
  $form['display']['bt_cols'] = array(
    '#type' => 'select',
    '#title' => t('Bootstrap rows'),
    '#default_value' => $component['extra']['bt_cols'],
    '#description' => t('Choose number of rows.'),
    '#parents' => array('extra', 'bt_cols'),
    '#options' => array(
      '12' => t('12 / 12'),
      '11' => t('11 / 12'),
      '10' => t('10 / 12'),
      '9' => t('9 / 12'),
      '8' => t('8 / 12'),
      '7' => t('7 / 12'),
      '6' => t('6 / 12'),
      '5' => t('5 / 12'),
      '4' => t('4 / 12'),
      '3' => t('3 / 12'),
      '2' => t('2 / 12'),
      '1' => t('1 / 12'),
    ),
  );
  $form['display']['bt_breakpoint'] = array(
    '#type' => 'select',
    '#title' => t('Bootstrap breakpoint'),
    '#default_value' => $component['extra']['bt_breakpoint'],
    '#description' => t('Choose bootstrap breakpoint.'),
    '#parents' => array('extra', 'bt_breakpoint'),
    '#options' => array(
      'xs' => t('xs'),
      'sm' => t('sm'),
      'md' => t('md'),
      'lg' => t('lg'),
    ),
  );
  // Hide name, it's never shown
  $form['name']['#type'] = 'value';
  // Hack - name is required by webform but we don't expose it to the user. Instead we'll replace it with the value of form_key.
  $form['name']['#value'] = 'bt_cols';
  return $form;
}

/**
 * Implements _webform_render_component().
 */
function _webform_render_bt_layout_cols($component, $value = NULL, $filter = TRUE) {
  $element = array(
    '#weight' => $component['weight'],
    '#pre_render' => array('webform_bt_layout_cols_prerender'),
    '#webform_component' => $component,
    '#prefix' => '<div>',
    '#suffix' => '</div>',
  );
  return $element;
}

/**
 * Pre-render function to set a bt_layout_cols ID and classes.
 */
function webform_bt_layout_cols_prerender($element) {
  $attributes = empty($element['#attributes']) ? array('class' => array()) : $element['#attributes'];
  $attributes['class'][] = 'col-' . $element['#webform_component']['extra']['bt_breakpoint'] . '-' . $element['#webform_component']['extra']['bt_cols'];
  $attributes['class'][] = 'webform-component--' . str_replace('_', '-', implode('--', array_slice($element['#parents'], 1)));
  // css class for number of children
  $n = empty($element['#webform_component']['children']) ? 0 : count($element['#webform_component']['children']);
  $element['#prefix'] = '<div ' . drupal_attributes($attributes) . '>';
  return $element;
}

/**
 * Implements _webform_display_component().
 */
function _webform_display_bt_layout_cols($component, $value, $format = 'html') {
  if ($format == 'text') {
    $element = array(
      '#title' => '',
      '#weight' => $component['weight'],
      '#theme_wrappers' => array(),
    );
  }
  else {
    $element = _webform_render_bt_layout_cols($component, $value);
  }

  $element['#format'] = $format;

  return $element;
}
