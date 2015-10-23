<?php

/**
 * @file
 * Rules integration
 * Main logic that allows rules integration
 *
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_invoices_default_rules_configuration() {

  $rules = array();
  $rule = rules_reaction_rule();

  $rule->label = t('Sets the status of the invoice when payment is completed');
  $rule->active = TRUE;

  $rule->event('commerce_checkout_complete')
          ->action('commerce_invoices_update_status', array(
              'commerce_order:select' => 'commerce-order',
  ));

  $rules['commerce_invoices_update_invoice_status'] = $rule;

  return $rules;
}

/**
 * Implements hook_rules_action_info().
 */
function commerce_invoices_rules_action_info() {
  $actions = array();

  $actions['commerce_invoices_update_status'] = array(
      'label' => t('Update invoice status'),
      'parameter' => array(
          'commerce_order' => array(
              'type' => 'commerce_order',
              'label' => t('Invoice Order'),
          ),
      ),
      'group' => t('Commerce Invoices'),
      'callbacks' => array(
          'execute' => 'commerce_invoices_update_invoice_status',
      ),
  );

  return $actions;
}

/**
 * Updates the invoice product status to inactive
 *
 * @param $order
 * Invoice order passed by rules
 *
 */
function commerce_invoices_update_invoice_status($order) {
  if (array_key_exists('type', $order->data) && $order->data['type'] == 'commerce_invoice' && array_key_exists('invoice_no', $order->data) && !empty($order->data['invoice_no'])) {
    $invoice = InvoiceDB::GetInvoice($order->data['invoice_no']);
    $invoice->set_invoice_to_paid();
  }
}