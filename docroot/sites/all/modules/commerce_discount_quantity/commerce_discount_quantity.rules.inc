<?php

function commerce_discount_quantity_rules_condition_info() {
  $inline_conditions = inline_conditions_get_info();

  // Line item: order has quantity product
  $conditions['commerce_discount_quantity_line_item_has_quantity'] = array(
    'label' => t('Product has specific quantity'),
    'parameter' => array(
      'commerce_line_item' => array(
        'type' => 'commerce_line_item',
        'label' => t('Line item'),
        'wrapped' => TRUE,
      ),
      /*'products' => array(
        'type' => 'text',
        'label' => t('Product SKU(s)'),
        'description' => t('Products SKU to look for on the order. Enter a comma-separated list of product SKU(s).'),
      ),*/
      'operator' => array(
        'type' => 'text',
        'label' => t('Operator'),
        'description' => t('The operator used with the product quantity value below.'),
        'default value' => '>=',
        'options list' => '_inline_conditions_order_operator_options',
      ),
      'quantity' => array(
        'type' => 'integer',
        'label' => t('Quantity'),
        'description' => t('Quantity value to be compared against each selected product(s).'),
      ),
    ),
  );

  // Add the build callback defined in commerce_discount_quantity_inline_conditions_info().
  foreach ($conditions as $name => $condition) {
    $conditions[$name]['callbacks'] = array(
      'execute' => $inline_conditions[$name]['callbacks']['build'],
    );
  }

  return $conditions;
}
