<?php

/**
 * @file
 * Rules implementation for ec_product module.
 */

/**
 * Implements hook_rules_event_info().
 */
function ec_product_rules_event_info() {
  return array(
    'transaction_product_save' => array(
      'group' => t('product'),
      'label' => t('Transaction Product Save'),
      'module' => 'ec_product',
      'variables' => array(
        'txn' => array(
          'type' => 'ec_transaction',
          'label' => t('Altered Transaction'),
        ),
        'original' => array(
          'type' => 'ec_transaction',
          'label' => t('Original Transaction'),
        ),
        'item' => array(
          'type' => 'node',
          'label' => t('Transaction Item'),
        ),
      ),
    ),
  );
}

