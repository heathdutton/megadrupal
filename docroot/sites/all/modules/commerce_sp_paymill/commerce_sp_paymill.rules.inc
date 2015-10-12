<?php
/**
 * @file
 * Rules integration for the Commmerce SP Paymill webhooks invocation.
 */

/**
 * Implements hook_rules_event_info()
 */
function commerce_sp_paymill_rules_event_info() {
  $items = array(
    'commerce_sp_paymill_webhook_event' => array(
      'label' => t('Commerce SP Paymill webhook event'),
      'group' => t('Commerce SP Paymill subscription'),
      'variables' => array(
        'subscription' => array(
          'type' => 'commerce_sp_paymill_sub',
          'label' => t('Commerce SP Paymill subscription entity')
        ),
        'event_type' => array(
          'type' => 'text',
          'label' => t('Paymill webhook event type'),
          'options list' => 'commerce_sp_paymill_webhook_event_types',
          'restriction' => 'input',
        ),
      ),
    ),
  );
  return $items;
}
