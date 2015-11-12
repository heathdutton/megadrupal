<?php

/**
 * @file
 * Forms and other UI elements
 *
 */

/**
 * Form to add an invoice
 *
 * @return
 * Generated form array
 */
function commerce_invoices_add_invoice($form, &$form_state) {

  $form['invoice_no'] = array(
      '#type' => 'textfield',
      '#title' => t('Invoice #'),
      '#description' => t('Please enter the ID of the invoice sent to the customer'),
      '#default_value' => '',
      '#size' => 30,
      '#maxlength' => 128,
      '#required' => TRUE,
  );

  $form['invoice_payee'] = array(
      '#type' => 'textfield',
      '#title' => t('Company Name'),
      '#description' => t('Please enter the Name of the company who should pay the invoice'),
      '#default_value' => '',
      '#size' => 30,
      '#maxlength' => 128,
      '#required' => TRUE,
  );

  $form['amount'] = array(
      '#type' => 'textfield',
      '#title' => t('Invoice Amount in ' . commerce_default_currency()),
      '#description' => t('Please enter the amount the customer will pay'),
      '#default_value' => '',
      '#size' => 30,
      '#maxlength' => 128,
      '#required' => TRUE,
  );

  $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Add Invoice'),
  );

  return $form;
}

/**
 * Validation function for  commerce_invoices_add_invoice()
 *
 * @see commerce_invoices_add_invoice()
 */
function commerce_invoices_add_invoice_validate($form, &$form_state) {
  if (!is_numeric($form_state['values']['amount'])) {
    form_set_error('amount', t('Value must be Numeric'));
    return FALSE;
  }
}

/**
 * Submit function for  commerce_invoices_add_invoice()
 * Saves an Invoice as a Commerce product
 *
 * @see commerce_invoices_add_invoice()
 */
function commerce_invoices_add_invoice_submit($form, &$form_state) {
  global $user;

  $invoice = new Invoice($form_state['values']['invoice_no']);
  $invoice->set__invoice_amount($form_state['values']['amount']);
  $invoice->set__invoice_payee($form_state['values']['invoice_payee']);
  $invoice->save_invoice($user);

  drupal_set_message('Invoice added successfully');
}

/**
 * Form to enter invoice number and proceed to payment
 *
 * @return
 * Generated form array
 */
function commerce_invoices_pay_invoice($form, &$form_state) {

  $form['invoice_no'] = array(
      '#type' => 'textfield',
      '#title' => t('Invoice #'),
      '#description' => t('Please enter the ID of the invoice you recieved'),
      '#default_value' => '',
      '#size' => 30,
      '#maxlength' => 128,
      '#required' => TRUE,
  );

  $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Pay Invoice'),
  );

  return $form;
}

/**
 * Validation function for commerce_invoices_pay_invoice()
 *
 * @see commerce_invoices_pay_invoice()
 *
 */
function commerce_invoices_pay_invoice_validate($form, &$form_state) {

  $invoice_no = $form_state['values']['invoice_no'];

  $exists = InvoiceDB::CheckInvoiceExists($invoice_no);

  if (!$exists) {
    form_set_error('invoice_no', t("Invoice doesn't exist, Please make sure you entered the correct Invoice #, or contact our support to help you"));
    return FALSE;
  }

  $paid = InvoiceDB::CheckInvoiceAlreadyPaid($invoice_no);

  if ($paid) {
    form_set_error('invoice_no', t("Invoice #").$invoice_no.t(" was already paid, Please make sure you entered the correct Invoice #, or contact our support to help you"));
    return FALSE;
  }
}

/**
 * Submit function for commerce_invoices_pay_invoice()
 * Adds specific properties to the created order and submits it
 *
 * @see commerce_invoices_pay_invoice()
 *
 */
function commerce_invoices_pay_invoice_submit($form, &$form_state) {
  global $user;
  $invoice = InvoiceDB::GetInvoice($form_state['values']['invoice_no']);
  $invoiceOrder = new CommerceInvoiceOrder($invoice);
  $invoiceOrder->Checkout($user);
}