<?php

/**
 * @file
 * Webform module paired fields component.
 */

/**
 * Implements _webform_defaults_component().
 */
function _webform_defaults_paired_fields() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'pid' => 0,
    'weight' => 0,
    'extra' => array(
      'title_display' => 0,
      'description' => '',
      'private' => FALSE,
    ),
  );
}

/**
 * Implements _webform_edit_component().
 */
function _webform_edit_paired_fields($component) {
  $form = array();
  return $form;
}

/**
 * Implements _webform_render_component().
 */
function _webform_render_paired_fields($component, $value = NULL, $filter = TRUE) {
  $node = isset($component['nid']) ? node_load($component['nid']) : NULL;

  $element = array(
    '#type' => 'paired_fields',
    '#title' => $filter ? _webform_filter_xss($component['name']) : $component['name'],
    '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : NULL,
    '#weight' => $component['weight'],
    '#description' => $filter ? _webform_filter_descriptions($component['extra']['description'], $node) : $component['extra']['description'],
    '#attributes' => array(
      'class' => array('webform-component-paired-fields'),
      'id' => 'webform-component-' . $component['form_key'],
    ),
    '#pre_render' => array('form_pre_render_paired_fields', 'webform_paired_fields_prerender', 'webform_element_title_display'),
    '#translatable' => array('title', 'description'),
  );

  return $element;
}

/**
 * Pre-render function to set a paired fields ID.
 */
function webform_paired_fields_prerender($element) {
  $element['#attributes']['id'] = 'webform-component-' . str_replace('_', '-', implode('--', array_slice($element['#parents'], 1)));
  return $element;
}

/**
 * Implements _webform_display_component().
 */
function _webform_display_paired_fields($component, $value, $format = 'html') {
  if ($format == 'text') {
    $element = array(
      '#title' => $component['name'],
      '#weight' => $component['weight'],
      '#translatable' => array('title'),
    );
  }
  else {
    $element = _webform_render_paired_fields($component, $value);
  }

  $element['#format'] = $format;

  return $element;
}
