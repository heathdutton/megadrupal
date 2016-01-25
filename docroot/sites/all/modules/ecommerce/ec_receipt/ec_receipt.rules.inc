<?php

/**
 * @file
 * Implement rules and actions for ec_receipt.
 */

/**
 * Implements hook_rules_event_info().
 */
function ec_receipt_rules_event_info() {
  return array(
    'ec_receipt_saved' => array(
      'group' => t('receipt'),
      'label' => t('Receipt has been saved.'),
      'module' => 'ec_receipt',
      'variables' => array(
        'receipt' => array(
          'type' => 'ec_receipt',
          'label' => t('Receipt'),
        ),
        'orig_receipt' => array(
          'type' => 'ec_receipt',
          'label' => t('Original Receipt'),
        ),
      ),
    ),
  );
}

/**
 * Implements hook_rules_condition_info().
 */
function ec_receipt_rules_condition_info() {
  return array(
    'ec_receipt_condition_receipt_status' => array(
      'group' => t('receipt'),
      'label' => t('Receipt status'),
      'parameter' => array(
        'receipt' => array(
          'type' => 'ec_receipt',
          'label' => t('Receipt'),
        ),
        'status' => array(
          'type' => 'list<text>',
          'label' => t('Status'),
          'options list' => 'ec_receipt_get_statuses',
          'description' => t('Select the statuses which the receipt is set to.'),
          'restriction' => 'input',
        ),
      ),
      'module' => 'ec_receipt',
    ),
  );
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_receipt_condition_receipt_status() {
  
}
