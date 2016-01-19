<?php
/**
 * @file
 * Commerce hook implementations for the Commerce NMI Direct Post - Credit Card
 * module.
 */

/**
 * Implements hook_commerce_payment_method_info().
 */
function commerce_nmi_direct_post_cc_commerce_payment_method_info() {
  return array(
    'nmi_dp_cc' => array(
      'base' => 'commerce_nmi_direct_post_cc',
      'title' => t('NMI Direct Post - Credit card'),
      'short_title' => t('NMI Direct Post CC'),
      'display_title' => t('Credit card'),
      'description' => t('Integrates Network Merchants, Inc. direct post for credit card transactions.'),
      'file' => 'includes/commerce_nmi_direct_post_cc.forms.inc',
      'cardonfile' => array(
        'update callback' => 'commerce_nmi_direct_post_cc_cardonfile_update',
        'delete callback' => 'commerce_nmi_direct_post_cc_cardonfile_delete',
        'charge callback' => 'commerce_nmi_direct_post_cc_cardonfile_charge',
      ),
    ),
  );
}
