<?php
/**
 * @file
 * Commerce hook implementations for the Commerce NMI Direct Post - eCheck
 * module.
 */

/**
 * Implements hook_commerce_payment_method_info().
 */
function commerce_nmi_direct_post_echeck_commerce_payment_method_info() {
  return array(
    'nmi_dp_echeck' => array(
      'base' => 'commerce_nmi_direct_post_echeck',
      'title' => t('NMI Direct Post - eCheck'),
      'short_title' => t('NMI Direct Post eCheck'),
      'display_title' => t('eCheck'),
      'description' => t('Integrates Network Merchants, Inc. direct post for echeck transactions.'),
      'file' => 'includes/commerce_nmi_direct_post_echeck.forms.inc',
    ),
  );
}
