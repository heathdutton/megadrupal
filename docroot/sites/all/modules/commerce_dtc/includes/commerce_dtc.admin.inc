<?php

/**
 * @file
 * Admin page callbacks for the Commerce Direct To Cart module.
 */

/**
 * Form: Commerce DTC Settings Form
 */
function commerce_dtc_settings_form($form, &$form_state) {
  $form = array();

  $attribute_fields = _commerce_dtc_get_cart_attribute_fields();

  $help_text = '<p>Provide mappings for each of the following product attribute fields to be used in querystrings on product display pages. <strong>These must be URL friendly (alphanumerical, dashes, and underscores).</strong></p>';
  $help_text .= '<p>e.g. Map "color" to "field_colors" to allow for querystring http://example.com/product/abc/dtc?color=red</p>';

  $form['help_text'] = array(
    '#markup' => $help_text,
  );

  $form_state['build_info']['attribute_fields'] = $attribute_fields;

  // Provide option to pull configuration from commerce_pdas.
  if (module_exists('commerce_pdas')) {
    $form['commerce_dtc_use_pdas_config'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use Commerce PDAS Configuration'),
      '#description' => t('We detected that you are currently using Commerce Product Display Attribute Selection. The configuration for Commerce Direct To Cart mimics Commerce PDAS. Check this box to use that configuration. !commerce_pdas_config', array('!commerce_pdas_config' => l(t('Configure Commerce PDAS'), 'admin/commerce/config/commerce_pdas'))),
      '#default_value' => variable_get('commerce_dtc_use_pdas_config', 1),
    );

    $form['commerce_dtc_fields'] = array(
      '#type' => 'fieldset',
      '#title' => t('Attribute Fields'),
      '#description' => t('Uncheck the checkbox above to edit these fields.'),
      '#collapsible' => TRUE,
      '#collapsed' => variable_get('commerce_dtc_use_pdas_config', 1),
      '#states' => array(
        'disabled' => array(
          ':input[name="commerce_dtc_use_pdas_config"]' => array('checked' => TRUE),
        ),
        'required' => array(
          ':input[name="commerce_dtc_use_pdas_config"]' => array('checked' => FALSE),
        ),
        'collapsed' => array(
          ':input[name="commerce_dtc_use_pdas_config"]' => array('checked' => TRUE),
        ),
      ),
    );
  }

  foreach ($attribute_fields as $field_name => $attribute_field) {
    $form['commerce_dtc_fields'][$field_name] = array(
      '#type' => 'textfield',
      '#title' => t($attribute_field['label']) . " ({$field_name})",
      '#required' => TRUE,
      '#default_value' => _commerce_dtc_field_shortname($field_name),
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save configuration',
  );

  return $form;
}

/**
 * Form Validation: Commerce DTC Settings Form
 */
function commerce_dtc_settings_form_validate($form, &$form_state) {
  foreach ($form_state['build_info']['attribute_fields'] as $field_name => $attribute_field) {
    if (preg_match('/[^a-zA-Z0-9_-]/s', $form_state['values'][$field_name]) !== 0) {
      form_set_error($field_name, t('@field_label must be alphanumerical, dashes, and underscores.', array('@field_label' => $attribute_field['label'])));
    }
  }
}

/**
 * Form Submit: Commerce DTC Settings Form
 */
function commerce_dtc_settings_form_submit($form, &$form_state) {
  variable_set('commerce_dtc_use_pdas_config', $form_state['values']['commerce_dtc_use_pdas_config']);

  if (!empty($form_state['values']['commerce_dtc_use_pdas_config'])) {
    variable_del('commerce_dtc_short_names');
  }
  else {
    $commerce_dtc_short_names = array();

    foreach ($form_state['build_info']['attribute_fields'] as $field_name => $attribute_field) {
      $commerce_dtc_short_names[$field_name] = $form_state['values'][$field_name];
    }
    variable_set('commerce_dtc_short_names', $commerce_dtc_short_names);
  }
}
