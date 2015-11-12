<?php
/**
 * @file
 * Rules integration for the Commerce Stock Reserve module.
 */

/**
 * Implements hook_rules_action_info().
 */
function commerce_stock_reserve_rules_action_info() {
  $actions = array();

  $actions['commerce_stock_reserve_reserve'] = array(
    'label' => t('Reserve stock for a line item'),
    'group' => t('Commerce Stock Reserve'),
    'parameter' => array(
      'commerce_line_item' => array(
        'type' => 'commerce_line_item',
        'label' => t('Line item'),
      ),
    ),
  );

  $actions['commerce_stock_reserve_release'] = array(
    'label' => t('Release stock for a line item'),
    'group' => t('Commerce Stock Reserve'),
    'parameter' => array(
      'commerce_line_item' => array(
        'type' => 'commerce_line_item',
        'label' => t('Line item'),
      ),
      'restore_to_product' => array(
        'type' => 'boolean',
        'label' => t('Restore the released stock to the product'),
        'optional' => TRUE,
        'default value' => TRUE,
      ),
    ),
  );

  return $actions;
}

/**
 * Implements hook_rules_condition_info().
 */
function commerce_stock_reserve_rules_condition_info() {
  $conditions = array();

  $conditions['commerce_stock_reserve_is_enough_available'] = array(
    'label' => t('Enough product stock is available for the user'),
    'parameter' => array(
      'commerce_product' => array(
        'type' => 'commerce_product',
        'label' => t('Product'),
      ),
      'stock_already_ordered' => array(
        'type' => 'decimal',
        'label' => t('Stock already ordered'),
      ),
    ),
    'group' => t('Commerce Stock Reserve'),
    'callbacks' => array(
      'execute' => 'commerce_stock_reserve_is_enough_available',
    ),
    'module' => 'commerce_stock_reserve',
  );

  return $conditions;
}

/**
 * Rules condition to check that enough product stock is available for the user.
 */
function commerce_stock_reserve_is_enough_available($product, $already_ordered) {
  return $already_ordered <= commerce_stock_reserve_get_user_available_amount($product);
}

/**
 * Rules action helper to reserve stock for a line item.
 */
function commerce_stock_reserve_reserve($line_item) {
  commerce_stock_reserve($line_item, FALSE);
}

/**
 * Rules action helper to release stock for a line item.
 *
 * @param object $line_item
 *   The Commerce line item object.
 * @param bool $restore_to_product
 *   Whether to restore the stock back to the product when it has been released.
 */
function commerce_stock_reserve_release($line_item, $restore_to_product = TRUE) {
  commerce_stock_reserve($line_item, TRUE, $restore_to_product);
}
