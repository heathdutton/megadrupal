<?php

/**
 * @file
 * Admin page callbacks for the Commerce Product Display Attribute Selection module.
 */

/**
 * Form: Commerce PDAS Settings Form
 */
function commerce_pdas_settings_form($form, &$form_state) {
  $form = array();

  $attribute_fields = _commerce_pdas_get_cart_attribute_fields();

  $help_text = '<p>Provide mappings for each of the following product attribute fields to be used in querystrings on product display pages. <strong>These must be URL friendly (alphanumerical, dashes, and underscores).</strong></p>';
  $help_text .= '<p>e.g. Map "color" to "field_colors" to allow for querystring http://example.com/product/abc?color=red</p>';

  $form['help_text'] = array(
    '#markup' => $help_text,
  );

  $form_state['build_info']['attribute_fields'] = $attribute_fields;

  foreach ($attribute_fields as $field_name => $attribute_field) {
    $form[$field_name] = array(
      '#type' => 'textfield',
      '#title' => t($attribute_field['label']) . " ({$field_name})",
      '#required' => TRUE,
      '#default_value' => _commerce_pdas_field_shortname($field_name),
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save configuration',
  );

  return $form;
}

/**
 * Form Validation: Commerce PDAS Settings Form
 */
function commerce_pdas_settings_form_validate($form, &$form_state) {
  foreach ($form_state['build_info']['attribute_fields'] as $field_name => $attribute_field) {
    if (preg_match('/[^a-zA-Z0-9_-]/s', $form_state['values'][$field_name]) !== 0) {
      form_set_error($field_name, t('@field_label must be alphanumerical, dashes, and underscores.', array('@field_label' => $attribute_field['label'])));
    }
  }
}

/**
 * Form Submit: Commerce PDAS Settings Form
 */
function commerce_pdas_settings_form_submit($form, &$form_state) {
  $commerce_pdas_short_names = array();

  foreach ($form_state['build_info']['attribute_fields'] as $field_name => $attribute_field) {
    $commerce_pdas_short_names[$field_name] = $form_state['values'][$field_name];
  }

  variable_set('commerce_pdas_short_names', $commerce_pdas_short_names);
}
