<?php

/**
 * @file
 * Creates an action for the batch rule.
 */

/**
 * Implements hook_rules_action_info().
 */
function commerce_firstdata_connect_rules_action_info() {
  $actions = array();

  // The batch rule for capturing transactions.
  $actions['batch_capture'] = array(
    'label' => t('Automated capturing process.'),
    'parameter' => array(
      'date' => array(
        'type' => 'date',
        'label' => t('Enter date from which to capture.'),
        'description' => t('You can enter date from which to start capturing transactions or enter negative number of days to start capturing from the days before today (ex. -1 day, -2 days, -3 days …) or enter "now" to capture all transactions from today.'),
      ),
    ),
    'group' => t('Ogone'),
    'callbacks' => array(
      'execute' => 'commerce_firstdata_connect_batch_rule',
    ),
  );

  return $actions;
}
