<?php

/**
 * @file
 * Administration page callbacks for Commerce Moodle Integration.
 */

/**
 * Callback for commerce_moodle_menu().
 */
function commerce_moodle_admin_settings() {

  // Product types.
  $types = commerce_product_types();
  $options = array();
  foreach ($types as $product_type) {
    $options[$product_type['type']] = $product_type['name'];
  }
  if ($options) {
    $form['commerce_moodle_product_types'] = array(
      '#type'          => 'checkboxes',
      '#title'         => t('Product types to be treated as Moodle courses'),
      '#options'       => $options,
      '#default_value' => variable_get('commerce_moodle_product_types', array()),
      '#description'   => t('All products of these types are expected to have a Moodle course internal number for SKU.'),
    );

    return system_settings_form($form);
  }

  drupal_set_message('No product types found. Please make sure you have Product UI module enabled.', 'warning');
}
