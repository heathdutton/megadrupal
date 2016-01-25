<?php

/**
 * @file
 * Default rules configurations for Shipping.
 */


/**
 * Implements hook_default_rules_configuration().
 */
function commerce_free_shipping_default_rules_configuration() {
  $rules = array();
  // Loop over every shipping method and define a rule to let user
  // add condition for free shipping.
  $shipping_services = commerce_shipping_services();
  foreach ($shipping_services as $service) {
    $base = $service['base'];
    // Check if free shipping is enabled for this services.
    if (variable_get($base . '_free_shipping_additionnal_rules', FALSE)) {
      // Create a new condition set.
      // See rules doc: https://drupal.org/node/905262 for tips.
      $condition_set = rules_and(commerce_free_shipping_service_component_variables());
      $condition_set->label = t('Free shipping conditions for: @title', array('@title' => $service['display_title']));
      $condition_set->active = TRUE;
      $condition_set->tags = array('Commerce free shipping');

      $rules['commerce_free_shipping_service_' . $base] = $condition_set;
    }
  }

  return $rules;
}

/**
 * Returns an array of variables for use in free shipping service components.
 */
function commerce_free_shipping_service_component_variables() {
  return array(
    'commerce_order' => array(
      'type' => 'commerce_order',
      'label' => t('Order'),
    ),
  );
}
