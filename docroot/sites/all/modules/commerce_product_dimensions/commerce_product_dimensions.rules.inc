<?php

/**
 * Implements hook_rules_action_info().
 */
function commerce_product_dimensions_rules_action_info() {
  return array(
    'commerce_product_dimensions_get_factor' => array(
      'label' => t('Determine multiplication factor from dimensions'),
      'parameter' => array(
        'commerce_line_item' => array(
          'type' => 'commerce_line_item',
          'label' => t('Line Item'),
          'wrapped' => TRUE,
        ),
      ),
      'provides' => array(
        'dimension_factor' => array(
          'type' => 'decimal',
          'label' => t('Multiplication Factor'),
        ),
        'dimension_base_price' => array(
          'type' => 'decimal',
          'label' => t('Adjusted Base Price'),
        ),
      ),
      'group' => t('Commerce Line Item'),
    ),
  );
}

/**
 * Rule action callback.
 */
function commerce_product_dimensions_get_factor($line_item_wrapper) {
  $line_item = $line_item_wrapper->value();
  $base_price = $line_item->commerce_unit_price[LANGUAGE_NONE][0]['amount'];
  $factor = _commerce_product_dimensions_get_factor($line_item);
  $base_price = _commerce_product_dimensions_adjust_base_price($line_item, $base_price, $factor);
  return array(
    'dimension_factor' => $factor,
    'dimension_base_price' => $base_price,
  );
}
