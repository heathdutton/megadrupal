<?php

/**
 * @file
 * Creates an action for the batch rule.
 */

/**
 * Implements hook_rules_action_info().
 */
function commerce_vantiv_rules_action_info() {
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
    'group' => t('Commerce vantiv'),
    'callbacks' => array(
      'execute' => 'commerce_vantiv_batch_rule',
    ),
  );

  return $actions;
}

/**
 * Implements hook_rules_condition_info().
 */
function commerce_vantiv_rules_condition_info() {
  $condition = array();

  $condition['commerce_vantiv_affluence'] = array(
    'label' => t('Commerce vantiv affluence'),
    'parameter' => array(
      'order' => array(
        'type' => 'commerce_order',
        'label' => 'Order of the current user',
        'description' => t('Order to be checked whether or not have a credit card issuer country.'),
      ),

      'order_affluence' => array(
        'type' => 'text',
        'label' => t('Operator'),
        'options list' => 'commerce_vantiv_list_affluence',
      ),

    ),
    'group' => t('Commerce Vantiv'),
    'callbacks' => array(
      'execute' => 'commerce_vantiv_compare_affluence',
    ),
  );

  $condition['commerce_vantiv_card_issuer_country'] = array(
    'label' => t('Commerce vantiv card issuer country'),
    'parameter' => array(
      'order' => array(
        'type' => 'commerce_order',
        'label' => 'Orders',
        'description' => t('The order that will be checked whether or not it’s owner is affluent.'),
      ),
    ),
    'group' => t('Commerce Vantiv'),
    'callbacks' => array(
      'execute' => 'commerce_vantiv_card_issuer_country',
    ),
  );

  return $condition;
}

/**
 * Creates option list.
 */
function commerce_vantiv_list_affluence() {
  return array(
    'affluent' => t('Affluent'),
    'mass_affluent' => t('Mass affluent'),
    'both' => t('Both'),
  );
}