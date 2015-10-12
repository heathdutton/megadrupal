<?php

/**
 * @file
 * Coupon general settings.
 */

/**
 * Commerce coupon settings form.
 */
function commerce_coupon_ui_settings_form($form, &$form_state) {
  $form['commerce_coupon_default_code_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Coupon Code Length'),
    '#default_value' => variable_get('commerce_coupon_default_code_size', 8),
    '#description' => t('Set the coupon code length for auto generated codes.'),
  );

  $form['commerce_coupon_coupon_upgrade_types'] = array(
    '#type' => 'fieldset',
    '#title' => t('Upgrade basic coupons'),
    '#description' => t('If you\'re still using basic coupon type, you need to upgrade them either to percentage coupons or fixed amount coupons. This automated process will convert them based in the values of the percentage and amount fields. <br/><strong>Important note:</strong> If you have extra fields in the basic coupon type, add them to the percentage and/or fixed types, otherwhise the data will be lost'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  if (!module_exists('commerce_coupon_pct')) {
    $form['commerce_coupon_coupon_upgrade_types']['warning_text'] = array(
      '#markup' => t('Commerce coupon percentage is not installed thus percentage coupons won\'t be used as target for the basic coupon upgrade'),
    );
  }
  if (!module_exists('commerce_coupon_fixed_amount')) {
    $form['commerce_coupon_coupon_upgrade_types']['warning_text'] = array(
      '#markup' => t('Commerce coupon fixed amount is not installed thus fixed amount coupons won\'t be used as target for the basic coupon upgrade'),
    );
  }

  if (module_exists('commerce_coupon_pct') || module_exists('commerce_coupon_fixed_amount')) {
    $form['commerce_coupon_coupon_upgrade_types']['upgrade_coupons'] = array(
      '#type' => 'submit',
      '#value' => t('Upgrade coupons'),
      '#submit' => array('commerce_coupon_ui_settings_upgrade_coupons_submit'),
    );
  }

  return system_settings_form($form);
}

function commerce_coupon_ui_settings_upgrade_coupons_submit($form, &$form_state) {

  $info = entity_get_property_info('commerce_coupon');
  if (!isset($info['bundles']['commerce_coupon_fixed']) && !isset($info['bundles']['commerce_coupon_pct'])) {
    drupal_set_message('Coupon types for upgrading basic don\'t exist');
    return;
  }

  $operations = array();
  $basic_coupons = commerce_coupon_load_multiple(array(), array('type' => 'basic_coupon'));
  foreach ($basic_coupons as $basic_coupon) {
    $operations[] = array('commerce_coupon_ui_settings_upgrade_coupons_batch_worker', array($basic_coupon, $info));
  }

  $batch = array(
    'title' => t('Upgrading coupons'),
    'operations' => $operations,
    'init_message' => t('Starting to upgrade coupons'),
    'error_message' => t('An error occurred during processing'),
    'finished' => 'commerce_coupon_ui_settings_upgrade_coupons_batch_finished',
    'progressive' => FALSE,
    'file' => drupal_get_path('module', 'commerce_coupon_ui') . '/includes/commerce_coupon_ui.settings.inc',
  );

  batch_set($batch);
}

function commerce_coupon_ui_settings_upgrade_coupons_batch_worker($basic_coupon, $info, &$context) {
  $coupon_wrapper = entity_metadata_wrapper('commerce_coupon', $basic_coupon);
  $fixed_amount = $coupon_wrapper->commerce_coupon_fixed_amount->amount->value();
  $pct_amount = $coupon_wrapper->commerce_coupon_percent_amount->value();

  // If the fixed amount coupon module is installed, we use it in case that the
  // fixed amount field has any value or the percentage one is empty.
  // For the rest of the cases, if the pct coupon module is installed, we use
  // it.
  $new_type = 'commerce_coupon_fixed';
  if (!empty($fixed_amount)) {
    $new_type = 'commerce_coupon_fixed';
  }
  elseif (!empty($pct_amount)) {
    $new_type = 'commerce_coupon_pct';
  }
  $basic_coupon = _commerce_coupon_ui_settings_upgrade_coupon($basic_coupon, $info, $new_type);

  if ($basic_coupon->type <> 'basic_coupon') {
    commerce_coupon_save($basic_coupon);
    $context['results']['processed']++;
  }
}

function commerce_coupon_ui_settings_upgrade_coupons_batch_finished($success, $results, $operations) {
  if ($success) {
    $message = format_plural($results['processed'], 'One coupon processed.', '@count coupons processed.');
  }
  else {
    $message = t('There were some errors with the upgrade.');
  }
  drupal_set_message($message);
}

function _commerce_coupon_ui_settings_upgrade_coupon($basic_coupon, $info, $new_type = 'commerce_coupon_fixed') {
  if (isset($info['bundles'][$new_type])) {
    field_attach_delete('commerce_coupon', $basic_coupon);
    $basic_coupon->type = $new_type;
    foreach ($info['bundles']['basic_coupon']['properties'] as $property_name => $property) {
      if (!isset($info['bundles'][$new_type]['properties'][$property_name])) {
        unset($basic_coupon->$property_name);
      }
    }
  }
  return $basic_coupon;
}
