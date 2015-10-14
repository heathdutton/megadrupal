<?php
/**
 * @file
 * Redhen Donation rules file.
 */

/**
 * Implements hook_rules_event_info().
 */
function redhen_donation_rules_event_info() {
  $info = array();

  $info['redhen_donation_donation_completed'] = array(
    'group' => t('Donation'),
    'label' => t('After a Successful Donation'),
    'help' => t('Triggers when a new donation is processed successfully.'),
    'variables' => array(
      'redhen_donation' => array(
        'type' => 'redhen_donation',
        'label' => t('Donation'),
      ),
      'commerce_transaction' => array(
        'type' => 'commerce_payment_transaction',
        'label' => t('Transaction'),
      ),
    ),
  );

  return $info;
}
