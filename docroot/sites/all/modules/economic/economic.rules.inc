<?php
/**
 * @file
 * This files contains rules actions.
 */

/**
 * Implements hook_rules_action_info().
 */
function economic_rules_action_info() {
  $actions = array(
    'economic_order_transfer' => array(
      'label' => t('Order transfer to economic'),
      'group' => t('e-conomic'),
      'module' => 'economic',
      'parameter' => array(
        'commerce_order' => array(
          'type' => 'commerce_order',
          'label' => t('Shop defined order'),
        ),
      ),
    ),
  );

  return $actions;
}

/**
 * Implements hook_rules_event_info().
 */
function economic_rules_event_info() {
  $events = array(
    'economic_order_transferred' => array(
      'label' => t('Order is transferred to e-conomic'),
      'group' => t('e-conomic'),
      'module' => 'economic',
      'variables' => array(
        'commerce_order' => array(
          'type' => 'commerce_order',
          'label' => t('Shop defined order'),
          'description' => t('The order object of the order transferred with the two new variables based on transfer'),
        ),
      ),
    ),
  );

  return $events;
}
