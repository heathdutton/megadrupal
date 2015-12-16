<?php

/**
 * Implements hook_rules_condition_info().
 */
function coupon_combine_condition_rules_condition_info() {
  $inline_conditions = inline_conditions_get_info();
  $conditions = array();

  $condition = array(
    'label' => t('Line item product contains specific tag categories'),
    'parameter' => array(
      'entity' => array(
        'type' => 'entity',
        'label' => t('Order or Line Item'),
        'wrapped' => TRUE,
      ),
      'not_applicable_with' => array(
        'type' => 'checkboxes',
        'label' => t('Terms ID'),
        'description' => t('Enter a comma-separated list of term ID to compare against the node display of the passed product line item.'),
      ),
    ),
    'group' => t('Commerce Coupon'),
    'callbacks' => array(
      'execute' => 'coupon_combine_condition_no_other_offer_build',
    ),
  );

  $conditions['coupon_combine_condition_order_no_other_offer'] = $condition;
  $conditions['coupon_combine_condition_line_item_no_other_offer'] = $condition;

  return $conditions;
}