<?php

/**
 * @file
 * Default rule configurations for the Commerce Restrict Area module.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_restrict_area_default_rules_configuration() {
  $rules = array();

  // Add a reaction rule to: Notify to users with bypass permission.
  $rule = rules_reaction_rule();
  $rule->label = t('Restrict Area: Notify to users with bypass permission');
  $rule->tags = array(t('Commerce Restrict Area'));
  $rule->active = TRUE;
  $rule->event('commerce_restrict_area_validation')
       ->condition('data_is', array(
           'data:select' => 'commerce-order:state',
           'op' => 'IN',
           'value' => array(
             'checkout',
             'pending',
           ),
         ))
       ->condition('commerce_restrict_area_user_has_bypass_permission', array('account:select' => 'site:current-user'))
       ->condition(rules_condition('commerce_restrict_area_address_validate', array('commerce_order:select' => 'commerce-order'))->negate())
       ->action('commerce_restrict_area_message', array(
           'commerce_order:select' => 'commerce-order',
           'message' => 'The following products can not be sold at the address specified in this order: ',
           'type' => 'warning',
           'repeat' => 0,
           'products' => 1,
         ));
  $rules['rules_commerce_restrict_area_bypass'] = $rule;

  // Add a reaction rule to: Notify to users and remove products.
  $rule = rules_reaction_rule();
  $rule->label = t('Restrict Area: Notify to users and remove products');
  $rule->tags = array(t('Commerce Restrict Area'));
  $rule->active = TRUE;
  $rule->event('commerce_restrict_area_validation')
       ->condition('data_is', array(
           'data:select' => 'commerce-order:state',
           'op' => 'IN',
           'value' => array('checkout'),
         ))
       ->condition(rules_condition('commerce_restrict_area_user_has_bypass_permission', array('account:select' => 'site:current-user'))->negate())
       ->condition(rules_condition('commerce_restrict_area_address_validate', array('commerce_order:select' => 'commerce-order'))->negate())
       ->action('commerce_restrict_area_remove_products_message', array(
           'commerce_order:select' => 'commerce-order',
           'message' => 'The following products can not be sold at the address specified in this order: ',
           'type' => 'warning',
           'repeat' => 0,
           'products' => 1,
         ))
       ->action('data_set', array(
           'data:select' => 'commerce-order:status',
           'value' => 'cart',
         ))
       ->action('redirect', array('url' => 'cart'));
  $rules['rules_commerce_restrict_area_user'] = $rule;

  return $rules;
}
