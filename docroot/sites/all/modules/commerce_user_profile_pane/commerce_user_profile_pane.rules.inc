<?php

/**
 * @file
 * Rules integration for commerce_user_profile_pane module to allow to update
 * user profile just after the commerce_checkout_new_account Rule create the
 * user account.
 */

/**
 * Implements hook_rules_action_info().
 */
function commerce_user_profile_pane_rules_action_info() {
  $actions = array();

  $actions['commerce_user_profile_pane_user_update'] = array(
    'label' => t('Update the User with input from User Profile Pane.'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order in checkout'),
      ),
    ),
    'group' => t('Commerce User Profile Pane'),
    'base' => 'commerce_user_profile_pane_user_update',
  );

  return $actions;
}

/**
 * Update User Profile Rule callback that handle the update of the user profile
 * data just after the commerce_checkout_new_account Rule had created the user
 * account.
 */
function commerce_user_profile_pane_user_update($order) {
  if (isset($order->data['commerce_user_profile_pane'])) {
    // wrap the order to make easy extract data
    $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
    // process the user profile pane input values and update the user profile
    _commerce_user_profile_pane_update_user_profile($order_wrapper->owner->value(), $order->data['commerce_user_profile_pane']);
  }
}
