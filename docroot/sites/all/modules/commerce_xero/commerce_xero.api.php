<?php
/**
 * @file
 * Describe hooks provided by the Commerce Xero module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Perform alterations before an invoice is created on Xero.
 *
 * @param $invoice
 *   Nested array of elements that comprise the invoice.
 * @param $order
 *   Entity Metadata Wrapper for the Commerce Order for this invoice.
 * @param $bank
 *   The bank account used to create this invoice.
 *
 * @see commerce_xero_get_invoice().
 * @see https://github.com/mradcliffe/PHP-Xero
 * @see http://developer.xero.com/documentation/api/Invoices/
 */
function hook_commerce_xero_invoice_alter(&$invoice, $order, $bank) {
  $invoice['Invoice']['Reference'] = t('@site Order #@num', array('@site' => variable_get('site_name', 'Drupal'), '@num' =>  $order->order_number->value()));
}

/**
 * Perform alterations before a payment is created on Xero.
 *
 * @param $payment
 *   Nested array of elements that comprise the invoice payment.
 * @param $order
 *   Entity Metadata Wrapper for the Commerce Order.
 * @param $bank
 *   The bank account used to create this invoice.
 * @param $invoice
 *   Nested array of elements that comprise the invoice.
 *
 * @see hook_commerce_xero_invoice_alter().
 */
function hook_commerce_xero_payment_alter(&$payment, $order, $bank, $invoice) {
  $payment['Payment']['Reference'] = 'some other reference';
}


/**
 * Perform alterations before a bank transaction is created on Xero.
 *
 * @param $transaction
 *   Nested array of elements that comprise the bank transaction.
 * @param $order
 *   Entity Metadata Wrapper for the Commerce Order for this invoice.
 * @param $bank
 *   The bank account used to create this invoice.
 *
 * @see hook_commerce_xero_invoice_alter().
 */
function hook_commerce_xero_banktransaction_alter(&$transaction, $order, $bank) {
  $transaction['BankTransaction']['Url'] = 'http://example.com';
}

/**
 * @} End of "addtogroup hooks".
 */
