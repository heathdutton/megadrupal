<?php

/**
 * Implements hook_features_export().
 */
function commerce_custom_offline_payment_features_export($data, &$export, $module_name = '') {
  $pipe = array();

  $export['dependencies']['features'] = 'features';
  $export['dependencies']['commerce_payment'] = 'commerce_payment';
  $export['dependencies']['commerce_cop'] = 'commerce_cop';
  $export['dependencies']['rules'] = 'rules';

  // Get the list of the commerce custom offline payments and export them.
  $payments = commerce_cop_get_payments();
  foreach ($data as $payment_id) {
    $export['features']['commerce_custom_offline_payment'][$payment_id] = $payment_id;
  }
  return $pipe;
}

/**
 * Implements hook_features_export_options().
 */
function commerce_custom_offline_payment_features_export_options() {
  $feature_types = array();
  $payments = commerce_cop_get_payments();
  if (!empty($payments)) {
    foreach($payments as $payment) {
      $feature_types[$payment['id']] = $payment['title'];
    }
  }
  return $feature_types;
}

/**
 * Implements hook_features_export_render().
 */
function commerce_custom_offline_payment_features_export_render($module, $data, $export = NULL) {
  $payments = commerce_cop_get_payments();
  $output = array();
  $output[] = '  $items = array(';
  foreach ($data as $payment_id) {
    if (isset($payments[$payment_id]) && $payment = $payments[$payment_id]) {
      $output[] = "    '{$payment_id}' => " . features_var_export($payment, '    ') . ",";
    }
  }
  $output[] = '  );';
  $output[] = '  return $items;';
  $output = implode("\n", $output);
  return array('commerce_custom_offline_payments' => $output);
}

/**
 * Implements hook_features_revert().
 */
function commerce_custom_offline_payment_features_revert($module = NULL) {
  // Get default custom offline payments.
  if (module_hook($module, 'commerce_custom_offline_payments')) {
    $default_payments = module_invoke($module, 'commerce_custom_offline_payments');
    $existing_payments = commerce_cop_get_payments();
    foreach ($default_payments as $default_payment) {
      // Add / or update
      if (!isset($existing_payments[$default_payment['id']])) {
        $default_payment['is_new'] = TRUE;
      }
      // Save the features payment.
      commerce_cop_payment_save($default_payment);
    }
  }
  else {
    drupal_set_message(t('Could not load default payments.'), 'error');
    return FALSE;
  }

  // Reset the caches.
  entity_defaults_rebuild();
  rules_clear_cache(TRUE);
  // Schedule a menu rebuild.
  variable_set('menu_rebuild_needed', TRUE);

  return TRUE;
}

/**
 * Implements hook_features_rebuild().
 */
function commerce_custom_offline_payment_features_rebuild($module) {
  return commerce_custom_offline_payment_features_revert($module);
}
