<?php

/**
 * @file
 * Rules integration for deliveries.
 *
 * @addtogroup rules
 * @{
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_delivery_default_rules_configuration() {
  $rule = rules_reaction_rule();
  $rule->label = 'Add delivery';
  $rule->event('commerce_order_insert')
    ->event('commerce_order_update')
    ->condition('commerce_delivery_order_has_status', array('order:select' => 'commerce-order', 'status' => array('pending' => 'pending', 'processing' => 'processing')))
    ->condition('user_has_role', array('account:select' => 'site:current-user', 'roles' => array(2 => 2)))
    ->condition(rules_condition('commerce_delivery_active_delivery_exists', array('order:select' => 'commerce-order'))->negate())
    ->condition('commerce_delivery_order_has_shippable_item', array('order:select' => 'commerce-order'))
    ->action('entity_save', array('data:select' => 'commerce-order', 'immediate' => 1))
    ->action('entity_create', array('type' => 'commerce_delivery', 'param_order_id:select' => 'commerce-order:order-id', 'param_status' => '0'));
  $configs['rules_add_delivery'] = $rule;

  $rule = rules_reaction_rule();
  $rule->label = 'Assign new line item to delivery';
  $rule->event('commerce_line_item_insert')
    ->condition('commerce_delivery_item_is_shippable', array('line_item:select' => 'commerce-line-item'))
    ->condition('entity_exists', array('type' => 'commerce_delivery', 'property' => 'order_id', 'value:select' => 'commerce-line-item:order-id'))
    ->action('commerce_delivery_assign_delivery', array('line_item:select' => 'commerce-line-item', 'delivery_status' => array('0' , '1')));
  $configs['rules_assign_line_item_to_delivery'] = $rule;

  $rule = rules_reaction_rule();
  $rule->label = 'Add delivery (anonymous user)';
  $rule->event('commerce_checkout_complete')
    ->condition(rules_condition('commerce_delivery_active_delivery_exists', array('order:select' => 'commerce-order'))->negate())
    ->condition('commerce_delivery_order_has_shippable_item', array('order:select' => 'commerce-order'))
    ->action('entity_create', array('type' => 'commerce_delivery', 'param_order_id:select' => 'commerce-order:order-id', 'param_status' => '0'));
  $configs['rules_add_delivery_anonymous'] = $rule;

  $rule = rules_reaction_rule();
  $rule->label = 'Assign orphan item to new delivery';
  $rule->event('commerce_delivery_insert')
    ->action('commerce_delivery_assign_orphans', array('delivery:select' => 'commerce-delivery', ));
  $configs['rules_assign_orphan_to_new_delivery'] = $rule;

/*
 * not needed any more. leaving it as example of adding a rules component by code
 *
  $rule = rule(array('line_item' => array('label' => 'line_item', 'type' => 'commerce_line_item',)));
  $rule->label = 'Item to delivery';
  $rule->condition('commerce_delivery_item_is_shippable', array('line_item:select' => 'line_item'))
       ->condition('commerce_delivery_item_is_orphan', array('line_item:select' => 'line_item'))
       ->action('commerce_delivery_assign_orphans', array('line_item:select' => 'line_item',));
  $configs['rules_item_to_delivery'] = $rule;
 */
  return $configs;
}
