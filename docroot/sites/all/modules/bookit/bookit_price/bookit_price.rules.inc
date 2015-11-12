<?php

/**
 * Implements hook_rules_action_info().
 */
function bookit_price_rules_action_info() {
  $actions = array();

  $actions['bookit_price_calculate_amount'] = array(
    'label' => t('Calculate Booking Price'),
    'parameter' => array(
      'commerce_line_item' => array(
        'type' => 'commerce_line_item',
        'label' => t('Line item'),
      ),
    ),
    'group' => t('Booking'),
    'callbacks' => array(
      'execute' => 'bookit_price_calculate_amount',
    ),
  );

  return $actions;
}

/**
 * Calculates line item's price by date.
 */
function bookit_price_calculate_amount($line_item) {
  $wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);

  $bookit_item = $wrapper->bookit_item->value();
  $date = $wrapper->bookit_date->value();
  $min = _bookit_calendar_create_date_from_datetime($date['value']);
  $max = _bookit_calendar_create_date_from_datetime($date['value2']);

  $query = db_select('bookit_price', 'p')
    ->condition('p.item_id', $bookit_item->item_id)
    ->condition('p.date', $min, '>=')
    ->condition('p.date', $max, '<');
  $query->addExpression('SUM(p.amount)');
  $price = $query->execute()->fetchField();

  $unit_price = $wrapper->commerce_unit_price->value();
  $unit_price['amount'] += $price;

  // Reset base price component.
  $unit_price['data'] = array();
  $unit_price['data'] = commerce_price_component_add($unit_price, 'base_price', $unit_price, TRUE);

  $wrapper->commerce_unit_price = $unit_price;
}
