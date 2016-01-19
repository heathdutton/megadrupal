<?php

/**
 * @file
 * This file contains the Rules hooks and functions necessary to make the cart
 * related modifcations, conditions, events, and actions for discount to work.
 */

/**
 * Implements hook_rules_data_info().
 */
function uc_simple_discount_rules_data_info() {
  $types['uc_simple_discount_cart'] = array(
    'label' => t('Ubercart cart array'),
    'wrap' => TRUE,
    'group' => t('Ubercart'),
  );

  return $types;
}

/**
 * Implements hook_rules_event_info().
 */
function uc_simple_discount_rules_event_info() {
  $events['uc_simple_discount_cart_alter_product'] = array(
    'label' => t('Cart\'s product is being altered.'),
    'group' => t('Cart'),
    'variables' => array(
      'cart' => array(
        'type' => 'uc_simple_discount_cart',
        'label' => t('Cart'),
      ),
      'item' => array(
        'type' => 'node',
        'label' => t('Product'),
      ),
    ),
  );

  return $events;
}

/**
 * Implements hook_rules_condition_info().
 */
function uc_simple_discount_rules_condition_info() {
  $conditions['uc_simple_discount_condition_total'] = array(
    'label' => t("Check a cart's subtotal"),
    'group' => t('Cart'),
    'help' => t('Compare the cart subtotal.'),
    'base' => 'uc_simple_discount_condition_total',
    'parameter' => array(
      'cart' => array(
        'type' => 'uc_simple_discount_cart',
        'label' => t('Cart'),
      ),
      'total_value' => array(
        'type' => 'decimal',
        'label' => t('Total value (!currency)', array('!currency' => variable_get('uc_currency_sign', '$'))),
        'description' => t('Enter the amount of the cart\'s subtotal that will be used with the operator below.'),
      ),
      'total_comparison' => array(
        'type' => 'text',
        'label' => t('Operator'),
        'options list' => 'uc_order_condition_value_operator_options',
        'restriction' => 'input',
      ),
    ),
  );

  return $conditions;
}

/**
 * Implements hook_rules_action_info().
 */
function uc_simple_discount_rules_action_info() {
  $actions['uc_simple_discount_apply'] = array(
    'label' => t('Apply a discount to the cart product'),
    'group' => t('Cart'),
    'base' => 'uc_simple_discount_action_apply',
    'parameter' => array(
      'item' => array(
        'type' => 'node',
        'label' => t('Product'),
      ),
      'discount' => array(
        'type' => 'decimal',
        'label' => t('Discount (%)'),
        'description' => t('Enter the percetage of the discount to apply. Discount must be grater than 0 and lower or equal to 100.'),
      ),
    ),
  );

  return $actions;
}

/**
 * Rules condition callback.
 */
function uc_simple_discount_condition_total($items, $total_value, $op) {
  $product_count = count($items);

  $total = 0;

  if ($product_count) {
    $display_items = entity_view('uc_cart_item', $items, 'cart');
    foreach (element_children($display_items['uc_cart_item']) as $key) {
      $display_item = $display_items['uc_cart_item'][$key];

      if (count(element_children($display_item))) {
        $total += $display_item['#total'];
      }
    }
  }

  switch ($op) {
    case 'less':
      return $total < $total_value;
    case 'less_equal':
      return $total <= $total_value;
    case 'equal':
      return $total == $total_value;
    case 'greater_equal':
      return $total >= $total_value;
    case 'greater':
      return $total > $total_value;
  }
}

/**
 * Rules action callback.
 */
function uc_simple_discount_action_apply($item, $value, $settings, $state, $action, $name) {
  $uc_simple_discount['percent'] = $value;
  $uc_simple_discount['rule'] = $action->parentElement()->label;
  $item->uc_simple_discount[] = $uc_simple_discount;
}

/**
 * Rules action validate callback.
 */
function uc_simple_discount_action_apply_validate($element) {
  if ($element->settings['discount'] <= 0 || $element->settings['discount'] > 100) {
    throw new RulesIntegrityException(t('Discount must be grater than 0 and lower or equal to 100.'), array($element, 'parameter', 'discount'));
  }
}
