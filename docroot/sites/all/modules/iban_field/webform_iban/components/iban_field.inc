<?php
/**
 * @file
 * Defines the IBAN field component for webform.
 */

/**
 * Implements _webform_defaults_component().
 */
function _webform_defaults_iban_field() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'pid' => 0,
    'weight' => 0,
    'value' => '',
    'mandatory' => 0,
    'extra' => array(
      'private' => FALSE,
      'field_prefix' => '',
      'field_suffix' => '',
    ),
  );
}

/**
 * Implements _webform_theme_component().
 */
function _webform_theme_iban_field() {
  return array(
    'webform_display_iban_field' => array(
      'render element' => 'element',
    ),
  );
}

/**
 * Implements _webform_edit_component().
 */
function _webform_edit_iban_field($component) {
  $enabled_countries = iban_field_enabled_countries();

  $default_value = array();
  $default_set = FALSE;
  if (isset($component['extra']['iban_field_country_settings']['iban_field_enabled_countries'])) {
    $default_value = $component['extra']['iban_field_country_settings']['iban_field_enabled_countries'];
    $default_set = TRUE;
  }

  $options = array();
  foreach ($enabled_countries as $option) {
    $options[$option->country_code] = $option->country_name;
    if (!$default_set) {
      $default_value[$option->country_code] = $option->country_code;
    }
  }

  $form = array();
  $form['extra']['iban_field_country_settings'] = array(
    '#type' => 'fieldset',
    '#description' => t('Select which countries IBAN numbers should be validated as valid.'),
    '#title' => t('IBAN country settings'),
    '#collapsed' => FALSE,
    '#collapsible' => TRUE,
  );

  $form['extra']['iban_field_country_settings']['iban_field_enabled_countries'] = array(
    '#type' => 'checkboxes',
    '#title' => t('IBAN Banks by country'),
    '#options' => $options,
    '#description' => t("Select which countries' IBAN numbers should be allowed."),
    '#default_value' => $default_value,
  );

  $form['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Default value'),
    '#default_value' => $component['value'],
    '#description' => t('The default value of the field.') . ' ' . theme('webform_token_help'),
    '#size' => 60,
    '#maxlength' => 22,
    '#weight' => 0,
  );

  return $form;
}

/**
 * Implements _webform_render_component().
 */
function _webform_render_iban_field($component, $value = NULL, $filter = TRUE) {
  $form_item = array(
    '#type' => 'textfield',
    '#title' => $filter ? _webform_filter_xss($component['name']) : $component['name'],
    '#required' => $component['mandatory'],
    '#weight' => $component['weight'],
    '#description' => $filter ? _webform_filter_descriptions($component['extra']['description']) : $component['extra']['description'],
    '#default_value' => $filter ? _webform_filter_values($component['value']) : $component['value'],
    '#element_validate'  => array('_webform_validate_iban_field'),
  );

  if (isset($value)) {
    $form_item['#default_value'] = $value[0];
  }

  return $form_item;
}

/**
 * Implements _webform_display_component().
 */
function _webform_display_iban_field($component, $value, $format = 'html') {
  return array(
    '#title' => $component['name'],
    '#weight' => $component['weight'],
    '#theme' => 'webform_display_textfield',
    '#theme_wrappers' => $format == 'html' ? array('webform_element') : array('webform_element_text'),
    '#post_render' => array('webform_element_wrapper'),
    '#field_prefix' => $component['extra']['field_prefix'],
    '#field_suffix' => $component['extra']['field_suffix'],
    '#component' => $component,
    '#format' => $format,
    '#value' => isset($value[0]) ? $value[0] : '',
  );
}

/**
 * Implements _webform_validate_component().
 */
function _webform_validate_iban_field($form_element, &$form_state) {
  $component = $form_element['#webform_component'];
  $enabled_countries = $component['extra']['iban_field_country_settings']['iban_field_enabled_countries'];

  // Strip all whitespaces.
  $iban = preg_replace('/\s+/', '', $form_element['#value']);

  if ($component['required'] && iban_field_field_is_empty($iban)) {
    form_error($form_element, t('IBAN field is required.'));
  }
  elseif (!iban_field_validate_country($iban, $enabled_countries)) {
    form_error($form_element, t('IBAN number is from an not-supported country.'));
  }
  elseif (!iban_field_validate_length($iban)) {
    form_error($form_element, t("IBAN length does not match with the countries' IBAN length."));
  }
  elseif (!iban_field_validate_iban($iban)) {
    form_error($form_element, t('Invalid IBAN number.'));
  }
}
