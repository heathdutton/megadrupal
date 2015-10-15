<?php

/**
 * @file
 * Rules supporting TaxRates Sales Order Processing.
 */

/**
 * Implements hook_rules_action_info().
 */
function commerce_taxrates_rules_action_info() {
  $parameter = array(
    'order' => array(
      'type' => 'commerce_order',
      'label' => t('Commerce Order'),
    ),
  );
  $actions = array(
    'commerce_taxrates_calculate_sales_tax' => array(
      'label' => t('Calculate sales tax for order'),
      'group' => t('Commerce TaxRates'),
      'parameter' => $parameter,
    ),
    'commerce_taxrates_delete_taxrates_line_items' => array(
      'label' => t('Delete TaxRates line items'),
      'group' => t('Commerce TaxRates'),
      'parameter' => $parameter,
    ),
  );

  return $actions;
}
