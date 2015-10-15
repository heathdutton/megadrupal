<?php

/**
 * @file
 * Webform module wrapper component.
 */

/**
 * Implements _webform_defaults_component().
 */
function _webform_defaults_wrapper() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'pid' => 0,
    'weight' => 0,
    'extra' => array(
      'element' => 'div',
      'classes' => '',
      'attributes' => '',
    ),
  );
}

/**
 * Implements _webform_edit_component().
 */
function _webform_edit_wrapper($component) {
  $form = array();

  $form['extra']['element'] = array(
    '#type' => 'textfield',
    '#title' => t('Element'),
    '#default_value' => $component['extra']['element'],
    '#description' => t('E.g. div, section, aside etc.'),
  );

  $form['extra']['classes'] = array(
    '#type' => 'textfield',
    '#title' => t('Classes'),
    '#default_value' => $component['extra']['classes'],
    '#description' => t('E.g. left'),
  );

  $form['extra']['attributes'] = array(
    '#type' => 'textfield',
    '#title' => t('Attributes'),
    '#default_value' => $component['extra']['attributes'],
    '#description' => t('E.g. name="anchor"'),
  );

  return $form;
}

/**
 * Implements _webform_render_component().
 */
function _webform_render_wrapper($component, $value = NULL, $filter = TRUE) {
  $classes = trim($component['extra']['classes']);
  $attributes = trim($component['extra']['attributes']);

  // This regex split the attributes string so that we can pass that
  // later to drupal_attributes().
  preg_match_all('/([^\s=]+)="([^"]+)"/', $attributes, $matches);

  $element_attributes = array();
  // Put the attribute and the value together.
  foreach ($matches[1] as $key => $attribute) {
    $element_attributes[$attribute] = $matches[2][$key];
  }

  // Add the classes to the attributes array.
  if (!isset($element_attributes['class']) && $classes) {
    $element_attributes['class'] = $classes;
  }
  elseif (isset($element_attributes['class']) && $classes) {
    $element_attributes['class'] .= ' ' . $classes;
  }

  $attributes = drupal_attributes($element_attributes);

  $element = array(
    '#type' => 'markup',
    '#weight' => $component['weight'],
    '#prefix' => '<' . $component['extra']['element'] . $attributes . '>',
    '#suffix' => '</' . $component['extra']['element'] . '> ',
    '#webform_component' => $component,
  );

  return $element;
}
