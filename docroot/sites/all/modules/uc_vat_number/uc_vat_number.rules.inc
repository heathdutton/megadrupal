<?php
/**
 * @file
 * Rules integration
 */

/**
 * Implements hook_rules_condition_info().
 */
function uc_vat_number_rules_condition_info() {
  return array(
    'uc_vat_number_condition_has_number' => array(
      'label' => t("Check for a VAT number"),
      'group' => t('Order'),
      'parameter' => array(
        'order' => array(
          'type' => 'uc_order',
          'label' => t('Order'),
        )
      )
    )
  );
}

/**
 * Checks the current order for a VAT number.
 */
function uc_vat_number_condition_has_number($order) {
  return (isset($order->data) && !empty($order->data['vat_number']));
}
