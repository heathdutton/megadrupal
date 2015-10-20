<?php

/**
 * Implements hook_rules_action_info_alter().
 */
function commerce_invoice_receipt_rules_action_info_alter(&$actions) {
	if ($actions['commerce_invoice_receipt_action_mail']['callbacks']['execute'] === 'commerce_invoice_receipt_mail_admin_form') {
    $actions['commerce_invoice_receipt_action_mail']['callbacks']['execute'] = 'commerce_invoice_receipt_action_mail';
  }
}