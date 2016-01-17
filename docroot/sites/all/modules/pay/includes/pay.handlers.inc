<?php

/**
 * @file
 * Hooks to specify available handlers for pay
 */

/**
 * Implementation of hook_pay_method_handler_info().
 */
function pay_pay_method_handler_info() {
  $path = drupal_get_path('module', 'pay') . '/includes/handlers';
  return array(
    'pay_method_custom' => array(
      'title' => t('Custom payment'),
      'description' => t('Manual payment entry, for COD payments, pledges, or manually incrementing a total.'),
      'parent' => 'pay_method',
      'module' => 'pay',
      'path' => $path,
    ),
    'pay_method_gateway' => array(
      'parent' => 'pay_method',
      'module' => 'pay',
      'path' => $path,
    ),
    'pay_method_direct' => array(
      'parent' => 'pay_method',
      'module' => 'pay',
      'path' => $path,
    ),
  );
}
