<?php

function uc_invoicepay_admin_settings_form() {
  $form = array();

  $form['uc_invoicepay_form_page_title'] = array(
      '#type' => 'textfield',
      '#title' => t('Title of invoice payment form page'),
      '#default_value' => variable_get('uc_invoicepay_form_page_title', t('Pay an Invoice')),
  );

  $form['uc_invoicepay_invoice_product_nid'] = array(
      '#type' => 'textfield',
      '#title' => t('Node ID of invoice product'),
      '#default_value' => variable_get('uc_invoicepay_invoice_product_nid', ''),
  );

  return system_settings_form($form);
}