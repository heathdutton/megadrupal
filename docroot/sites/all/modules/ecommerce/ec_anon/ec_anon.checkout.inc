<?php

/**
 * @file
 * Provide checkout support for anonymous customers.
 */

/**
 * Implements hook_checkout_init().
 */
function ec_anon_checkout_init(&$txn) {
  global $user;

  // User needs to login or create an account before they can checkout.
  // TODO: I am not sure if this should be done, as the default system wide policy maybe to not allow anonymous users to
  // purchase any products, but the individual products may be set to allow purchase.
  if ($user->uid == 0 && variable_get('ec_anon_policy', ECANON_POLICY_DEFAULT) == ECANON_POLICY_NEVER) {
    drupal_set_message(t('Login or <a href="!reg-link">register</a> to continue the checkout process', array('!reg-link' => url('user/register', array('query' => drupal_get_destination())))));
    drupal_goto('user', array('query' => drupal_get_destination()));
  }

  // If this is an anonymous user, check all the products and see if they can
  // be sold to them. If there are products which cannot be sold to an
  // anonymous user then get the user to login or register.
  foreach ($txn->items as $item) {
    if (!$user->uid) {
      if (ec_product_attributes_get($item, 'registered_user') || in_array($item->anon_policy, array(ECANON_POLICY_NEVER, ECANON_POLICY_NEVER_CART))) {
        drupal_set_message(t('Login or <a href="!reg-link">register</a> to continue the checkout process', array('!reg-link' => url('user/register', array('query' => drupal_get_destination())))));
        drupal_goto('user', array('query' => drupal_get_destination()));
      }
    }
  }
}
