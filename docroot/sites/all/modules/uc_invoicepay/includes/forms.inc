<?php

function uc_invoicepay_invoice_form($form, $form_state) {
  $form = array();

  $form['invoice_num'] = array(
      '#type' => 'textfield',
      '#title' => t('Invoice Number'),
      '#required' => TRUE,
      '#description' => t('This can be found on your bill.'),
      '#default_value' => $form_state['values']['invoice_num'],
      '#size' => 10,
  );

  $form['amount'] = array(
      '#type' => 'textfield',
      '#title' => t('Payment Amount'),
      '#field_prefix' => '$',
      '#required' => TRUE,
      '#default_value' => $form_state['values']['amount'],
      '#size' => 10,
  );

  $form['memo'] = array(
      '#type' => 'textarea',
      '#title' => t('Memo or Comments'),
      '#default_value' => $form_state['values']['memo'],
  );

  $form['submit'] = array('#type' => 'submit', '#value' => t('Proceed to Payment'));

  return $form;
}

function uc_invoicepay_invoice_form_submit($form, $form_state) {
  $data = array();

  $data['invoice'] = array(
      'invoice_num' => $form_state['values']['invoice_num'],
      'memo' => $form_state['values']['memo'],
      'amount' => $form_state['values']['amount'],
  );

  uc_cart_add_item(variable_get('uc_invoicepay_invoice_product_nid', ''), $qty = 1, $data);

  drupal_goto('cart/checkout');
}