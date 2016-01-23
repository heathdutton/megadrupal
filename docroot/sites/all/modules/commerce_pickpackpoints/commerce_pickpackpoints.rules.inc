<?php

/**
 * @file
 * Rules integration for Pick-Pack Pont. 
 */

/**
 * Implements hook_rules_condition_info().
 */
function commerce_pickpackpoints_rules_condition_info() {
  $conditions = array();

  $conditions['commerce_pickpackpoints_calculate_order_weight'] = array(
    'label' => t('Compare order weight'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
        'description' => t('The order what has weight.'),
      ),
      'weight_min' => array(
        'type' => 'text',
        'label' => t('Min order weight'),
        'description' => t('Input shipping min order weight.'),
      ),
      'weight_max' => array(
        'type' => 'text',
        'label' => t('Max order weight'),
        'description' => t('Input shipping max order weight.'),
      ),
      'unit' => array(
        'type' => 'text',
        'label' => t('Weight unit'),
        'options list' => 'physical_weight_unit_options',
        'description' => t('Choose weight unit.'),
      ),
    ),
    'group' => t('Commerce Order'),
    'callbacks' => array(
      'execute' => 'commerce_pickpackpoints_rules_order_weight',
    ),
  );

  return $conditions;
}

/**
 * Checks if order weight is less or equal than max weight.
 *
 * @param $order
 *   The order to check for a shipping line item.
 * @param $weight
 *   The max order weight.
 * @param $unit
 *   The weight unit.
 *   the condition returns TRUE if order weight is less than max weight.
 */
function commerce_pickpackpoints_rules_order_weight($order, $weight_min, $weight_max, $unit) {
  $order_weight = commerce_physical_order_weight($order, $unit);
  return ($order_weight['weight'] > $weight_min && $order_weight['weight'] <= $weight_max);
}
