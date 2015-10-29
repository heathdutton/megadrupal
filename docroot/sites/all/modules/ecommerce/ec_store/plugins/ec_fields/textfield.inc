<?php
/**
 * @file
 */

$plugin = array(
  'title' => t('Textfield'),
  'settings' => array(
    'form' => 'ec_charge_field_settings_textfield',
  ),
  'render' => 'ec_charge_field_render_textfield',
);

function ec_charge_field_settings_textfield(&$form_state) {
  $form = array();
  
  $form['size'] = array(
    '#type' => 'textfield',
    '#title' => t('Length'),
    '#size' => 4,
    '#default_value' => $form_state['item']->field_settings['size'],
    '#description' => t('Length of the text field'),
  );
  
  return $form;
}

function ec_charge_field_render_textfield(&$form, &$form_state, $field) {
  $element = array(
    '#type' => 'textfield',
    '#title' => $field->field_title,
    '#description' => $field->field_description,
  );
  
  if ($field->field_settings['size']) {
    $element['#size'] = $field->field_settings['size'];
  }
  
  return $element;
}