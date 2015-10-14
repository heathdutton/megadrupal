<?php

/**
 * @file
 * Rules integration for line items.
 *
 */


/**
 * Implements hook_rules_action_info().
 */
function commerce_vat_rules_action_info() {
  $actions = array();

  if (count(commerce_vat_rates()) > 0) {
    $actions['commerce_vat_rate_apply'] = array(
      'label' => t('Apply a vat rate to a line item'),
      'parameter' => array(
        'commerce_line_item' => array(
          'type' => 'commerce_line_item',
          'label' => t('Line item'),
        ),
        'vat_rate_name' => array(
          'type' => 'text',
          'label' => t('vat rate'),
          'options list' => 'commerce_vat_rate_titles',
        ),
      ),
      'provides' => array(
        'applied_vat' => array(
          'type' => 'commerce_price',
          'label' => t('Applied vat'),
        ),
      ),
      'group' => t('Commerce VAT'),
      'callbacks' => array(
        'execute' => 'commerce_vat_rate_rules_apply',
      ),
    );

    $actions['commerce_vat_order_rate'] = array(
      'label' => t('Get the highest VAT rate currently applied to an order'),
      'parameter' => array(
        'commerce_order' => array(
          'type' => 'commerce_order',
          'label' => t('Order'),
        ),
      ),
      'provides' => array(
        'order_vat_rate' => array(
          'type' => 'text',
          'label' => t('Order VAT Rate'),
        ),
      ),
      'group' => t('Commerce VAT'),
      'callbacks' => array(
        'execute' => 'commerce_vat_rules_order_rate',
      ),
    );
  }

  $actions['commerce_vat_calculate'] = array(
    'label' => t('Calculate VAT for a line item'),
    'parameter' => array(
      'commerce_line_item' => array(
        'type' => 'commerce_line_item',
        'label' => t('Line item'),
      ),
      'country' => array(
        'type' => 'text',
        'label' => t('ISO 2 Country Code'),
      ),
    ),
    'group' => t('Commerce VAT'),
    'callbacks' => array(
      'execute' => 'commerce_vat_rules_calculate',
    ),
  );

  $actions['commerce_vat_place_of_supply'] = array(
    'label' => t('Calculate Place of Supply for a line item'),
    'parameter' => array(
      'commerce_line_item' => array(
        'type' => 'commerce_line_item',
        'label' => t('Line item'),
      ),
    ),
    'group' => t('Commerce VAT'),
    'callbacks' => array(
      'execute' => 'commerce_vat_rules_place_of_supply',
    ),
  );

  return $actions;
}

/**
 * Rules action: loads and applies a vat rate to the given line item.
 */
function commerce_vat_rate_rules_apply($line_item, $name) {
  if ($vat_rate = commerce_vat_rate_load($name)) {
    $vat_price = commerce_vat_rate_apply($vat_rate, $line_item);

    // If vat was applied, return the price array as a new variable for use in
    // subsequent actions.
    if ($vat_price) {
      return array('applied_vat' => $vat_price);
    }
  }
}

/**
 * Rules action: loads and applies a vat rate to the given line item.
 */
function commerce_vat_rules_order_rate($commerce_order) {
  $vat_rate_name = commerce_vat_order_rate($commerce_order);

  return array('order_vat_rate' => $vat_rate_name);
}

/**
 * Rules action: checks for the application of each vat rate of a certain type.
 */
function commerce_vat_rules_calculate($line_item, $iso2) {
  commerce_vat_calculate_rates($line_item, $iso2);
}

/**
 * Rules action: checks for the application of each vat country.
 */
function commerce_vat_rules_place_of_supply($line_item) {
  commerce_vat_calculate_place_of_supply($line_item);
}
