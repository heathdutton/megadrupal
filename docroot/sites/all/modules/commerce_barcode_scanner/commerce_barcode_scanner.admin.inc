<?php

/**
 * @file
 * Generate the admin form used to configure the module.
 */


/**
 * Create an admin form for configuring the commerce barcode scanner.
 */
function commerce_barcode_scanner_admin_form($form, $form_state) {
  $form = array();
  $form['basic_config'] = array(
    '#type' => 'fieldset',
    '#title' => t('Basic Configuration'),
    'children' => array(
      'commerce_barcode_scanner_min_sku_length' => array(
        '#title' => t('Minimum SKU Length'),
        '#type' => 'textfield',
        '#description' => t('The minimum number of characters to make up a valid SKU.'),
        '#default_value' => commerce_barcode_scanner_variable_get('min_sku_length'),
        '#element_validate' => array('element_validate_integer_positive'),
      ),
      'commerce_barcode_scanner_key_interval' => array(
        '#title' => t('Keypress Interval'),
        '#type' => 'textfield',
        '#description' => t('This may need to be tweaked for different scanners. The number of milliseconds between each character in a barcode scan to be valid.'),
        '#default_value' => commerce_barcode_scanner_variable_get('key_interval'),
        '#element_validate' => array('element_validate_integer_positive'),
      ),
    ),
  );
  return system_settings_form($form);
}
