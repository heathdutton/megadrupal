<?php

/**
 * @file
 * Commerce API integration
 *
 */

/**
 * Implements hook_commerce_product_type_info().
 */
function commerce_invoices_commerce_product_type_info() {

  return array(
      'invoice' => array(
          'type' => 'invoice',
          'name' => 'Invoice',
          'description' => 'Stores an Invoice as a product',
          'help' => '',
          'revision' => 1
      )
  );
}

/**
 * Redirects user to checkout page for a specific invoice
 *
 * @param $invoice_id
 * The Invoice no.
 *
 * @param $token
 * Drupal hash token generated for the invoice_id
 *
 */
function commerce_invoices_create_order($invoice_id, $token) {
  if (drupal_hmac_base64($invoice_id, drupal_get_private_key() . drupal_get_hash_salt()) == $token) {
    global $user;
    $invoice = InvoiceDB::LoadInvoice($invoice_id);
    $invoice_no = $invoice->get__invoice_no();
    $paid = InvoiceDB::CheckInvoiceAlreadyPaid($invoice_no);
    if (!$paid) {
      $invoiceOrder = new CommerceInvoiceOrder($invoice);
      $invoiceOrder->Checkout($user);
    }
    else{
      drupal_set_message(t("Invoice #").$invoice_no.t(" was already paid, Please make sure you entered the correct Invoice #, or contact our support to help you"), "error");
      return drupal_not_found();
    }
  }
}