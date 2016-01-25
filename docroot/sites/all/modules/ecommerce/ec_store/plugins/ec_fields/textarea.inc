<?php

/**
 * @file
 *  Implementation of textarea fields
 */

$plugin = array(
  'title' => t('Textarea'),
  'settings' => array(
    'form' => 'ec_charge_field_settings_textarea',
  ),
  'render' => 'ec_charge_field_render_textarea',
);

function ec_charge_field_settings_textarea(&$form_state) {
  $form = array();
  
  $form['rows'] = array(
    '#type' => 'textfield',
    '#title' => t('Rows'),
    '#size' => 4,
    '#default_value' => $form_state['item']->field_settings['rows'],
    '#description' => t('Length of the text field'),
  );
  
  return $form;
}

function ec_charge_field_render_textarea(&$form, &$form_state, $field) {
  $element = array(
    '#type' => 'textarea',
    '#title' => $field->field_title,
    '#description' => $field->field_description,
  );
  
  if ($field->field_settings['rows']) {
    $element['#rows'] = $field->field_settings['rows'];
  }
  
  return $element;
}