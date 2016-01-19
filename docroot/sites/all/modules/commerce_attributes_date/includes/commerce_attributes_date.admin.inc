<?php

/**
 * @file
 * Defines admin settings for extra attributes module.
 */

/**
 * Callback function for extra attributes admin settings.
 *
 * @return array
 *   $form
 */
function commerce_attributes_date_admin_settings_form() {
  $form = array();

  $options = array(
    'date' => t('Date'),
  );

  $all_format_types = system_get_date_types();
  foreach ($all_format_types as $type => $value) {
    $format_types[$type] = $all_format_types[$type]['title'];
  }

  $form['commerce_attributes_date_date_format'] = array(
    '#type' => 'select',
    '#title' => t('Date format'),
    '#options' => $format_types,
    '#description' => t('Enter the date format to use for formatting date'),
    '#default_value' => variable_get('commerce_attributes_date_date_format', 'medium'),
    '#weight' => 1,
  );

  $form['commerce_attributes_date_allowed_attributes'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Allow attributes to'),
    '#options' => $options,
    '#default_value' => variable_get('commerce_attributes_date_allowed_attributes', array()),
    '#description' => t('Allow these types of fields to be used as attributes.'),
    '#weight' => 0,
  );
  $form['commerce_attributes_date_product_reference_field_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Product Reference Field Name'),
    '#default_value' => variable_get('commerce_attributes_date_product_reference_field_name', 'field_product'),
    '#description' => t("Enter the product variation reference field you've used in your content type."),
    '#weight' => 2,
  );

  return system_settings_form($form);
}
