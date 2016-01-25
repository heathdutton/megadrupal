<?php

/**
 * @file
 * Add support for rules in the checkout process.
 */

/**
 * Implements hook_rules_event_info().
 */
function ec_checkout_rules_event_info() {
  return array(
    'ec_checkout_after_process' => array(
      'group' => t('checkout'),
      'label' => t('After checkout'),
      'module' => 'ec_checkout',
      'variables' => array(
        'txn' => array(
          'type' => 'ec_transaction',
          'label' => t('Final transaction'),
        ),
      ),
    ),
  );
}
