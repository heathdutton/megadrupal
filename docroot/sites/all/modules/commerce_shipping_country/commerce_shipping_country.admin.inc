<?php

/**
 * @file
 * Commerce Payment Shipping Country admin file.
 */

/**
 * Custom form commerce_shipping_country_limit_form.
 */
function commerce_shipping_country_limit_form($form, &$form_state) {
  $instance = _commerce_shipping_country_get_addressfield_instance();

  if (empty($instance)) {
    return t('There was an error accesing the country list. Please contact the administrator.');
  }

  // Pass the instance in form, so we can update it in submit.
  $form['#instance'] = $instance;
  $form['available_countries'] = array(
    '#type' => 'select',
    '#title' => t('Active countries'),
    '#description' => t('Select the available country list for shipping or leave empty to enable all.'),
    '#options' => _addressfield_country_options_list(),
    '#default_value' => $instance['widget']['settings']['available_countries'],
    '#multiple' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Submit function for commerce_shipping_country_limit_form.
 */
function commerce_shipping_country_limit_form_submit($form, $form_state) {
  // Save the field instance settings.
  $instance = $form['#instance'];
  $instance['widget']['settings']['available_countries'] = $form_state['values']['available_countries'];
  field_update_instance($instance);

  drupal_set_message(t('The configuration options have been saved.'));
}

/**
 * Helper function to get the addressfield instance.
 */
function _commerce_shipping_country_get_addressfield_instance() {
  $entity_type = 'commerce_customer_profile';
  $bundle_name = variable_get('commerce_shipping_country_customer_bundle', 'shipping');
  $field_name = variable_get('commerce_shipping_country_customer_addressfield', 'commerce_customer_address');

  return field_info_instance($entity_type, $field_name, $bundle_name);
}

/**
 * Custom form commerce_shipping_country_settings_form.
 *
 * Settings form allowing to change the customer profile bundle and addressfield
 * used in the UI allowing users to change the available countries.
 */
function commerce_shipping_country_settings_form($form, &$form_state) {
  // Make sure that we have customer profiles.
  $bundles = _commerce_shipping_country_get_bundles();
  if (empty($bundles)) {
    return t('Unable to get any customer profile types.');
  }

  $form['commerce_shipping_country_customer_bundle'] = array(
    '#type' => 'select',
    '#title' => t('Customer profile type'),
    '#description' => t('Choose the Commerce Customer profile type.'),
    '#options' => _commerce_shipping_country_get_bundles(),
    '#required' => TRUE,
    '#default_value' => variable_get('commerce_shipping_country_customer_bundle', 'shipping'),
    '#ajax' => array(
      'callback' => 'commerce_shipping_country_addressfield_ajax_callback',
      'wrapper' => 'csc-customer-addressfield',
      'method' => 'replace',
    ),
  );

  if (isset($form_state['values']['commerce_shipping_country_customer_bundle'])) {
    $bundle = $form_state['values']['commerce_shipping_country_customer_bundle'];
  }
  else {
    $bundle = variable_get('commerce_shipping_country_customer_bundle', 'shipping');
  }

  // Make sure the selected profile type has an address field.
  $addressfields = _commerce_shipping_country_get_addressfields($bundle);
  if (empty($addressfields)) {
    $form['commerce_shipping_country_customer_addressfield']['#markup'] = t('There are no addressfields in this customer profile type.');
    return system_settings_form($form);
  }

  $form['commerce_shipping_country_customer_addressfield'] = array(
    '#type' => 'select',
    '#title' => t('Shipping Addressfield'),
    '#description' => t('Choose the address field.'),
    '#options' => $addressfields,
    '#required' => TRUE,
    '#default_value' => variable_get('commerce_shipping_country_customer_addressfield', 'commerce_customer_address'),
    '#prefix' => '<div id="csc-customer-addressfield">',
    '#suffix' => '</div>',
  );

  return system_settings_form($form);
}

/**
 * Helper function to build a list of customer profile bundles.
 */
function _commerce_shipping_country_get_bundles() {
  $options = array();
  $info = entity_get_info('commerce_customer_profile');
  if (!$info || !isset($info['bundles']) || empty($info['bundles'])) {
    return $options;
  }

  foreach ($info['bundles'] as $key => $bundle) {
    $options[$key] = $bundle['label'];
  }

  return $options;
}

/**
 * Helper function to build a list of addressfields by customer profile bundle.
 * 
 * @param string $bundle
 *   The 'commerce_customer_profile' entity bundle.
 */
function _commerce_shipping_country_get_addressfields($bundle = NULL) {
  $fields = array();
  if ($bundle == NULL) {
    return $fields;
  }

  $all_fields = field_info_instances('commerce_customer_profile', $bundle);
  if (empty($all_fields)) {
    return $fields;
  }

  foreach ($all_fields as $name => $info) {
    $field_info = field_info_field($name);
    if ($field_info['type'] == 'addressfield') {
      $fields[$name] = $info['label'];
    }
  }

  return $fields;
}

/**
 * Ajax callback for commerce_shipping_country_settings_form.
 */
function commerce_shipping_country_addressfield_ajax_callback($form, $form_state) {
  return $form['commerce_shipping_country_customer_addressfield'];
}
