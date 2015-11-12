<?php

/**
 * @file
 * Theme functions and display logic
 *
 */

/**
 * Displays the table for all invoices including all invoice details
 *
 */
function commerce_invoices_manage_invoices() {
  global $base_url;
  $invoices = db_query("SELECT * FROM {commerce_product} WHERE type='invoice'");

  $header = array('Invoice #', 'Company Name', 'Amount', 'Status', 'Details');

  $rows = array();

  foreach ($invoices as $invoice) {
    $invoice_no = substr($invoice->title, 9, strpos($invoice->title, '(') - 9);
    $company_name = substr($invoice->title, strpos($invoice->title, '(') + 1, strpos($invoice->title, ')') - strpos($invoice->title, '(') - 1);
    $price = db_query("SELECT commerce_price_amount FROM {field_data_commerce_price} WHERE entity_id=" . $invoice->product_id)->fetchAssoc();
    $status = $invoice->status ? t('Pending') : t('Paid');
    $rows[] = array(
        $invoice_no,
        $company_name,
        commerce_currency_format($price['commerce_price_amount'], commerce_default_currency()),
        $status,
        'Copy payment link below:<br />'.$base_url.'/commerce-invoices/'.$invoice->product_id.'/'.drupal_hmac_base64($invoice->product_id, drupal_get_private_key().drupal_get_hash_salt())
        );
  }

  return theme('table', array('header' => $header, 'rows' => $rows));
}
