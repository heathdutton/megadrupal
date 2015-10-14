<?php

/**
 * @file
 * Declares the rules for commerce billy mail.
 */

/**
 * Implements hook_rules_action_info().
 *
 * Declares any meta-data about actions for Rules in a big, associative, nested
 * array. See also hook_rules_action_info in the rules.api.php file, and the
 * online documentation at http://drupal.org/node/878928.
 */
function commerce_billy_mail_rules_action_info() {
  $actions = array(
    // The base-level keys in the array are the machine names for the actions,
    // and by default also the function name for the action callback. Prefix
    // with your module name to avoid name conflicts.
    'commerce_billy_mail_action_send_invoice' => array(
      'label' => t('Send commerce billy invoice mail.'), // Name displayed to admins
      'group' => t('Commerce Billy'), // Used for grouping actions in select lists
      'parameter' => array(
        'order' => array(
          'type' => 'commerce_order',
          'label' => t('Order')),
      ),
    ),
  );

  // Don't forget to return the array of action information!
  return $actions;
}

/**
 * The action function for 'commerce_billy_mail_action_hello_world'.
 */
function commerce_billy_mail_action_send_invoice($order) {
  return _commerce_billy_mail_send_order_invoice($order);
}