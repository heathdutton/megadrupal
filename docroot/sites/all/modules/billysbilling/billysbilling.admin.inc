<?php

/**
 * @file
 * BillysBilling module admin settings.
 */

use BillysBilling\Accounts\Billy_Account;
use BillysBilling\Accounts\Billy_AccountsRepository;
use BillysBilling\Products\Billy_ProductsRepository;
use BillysBilling\SalesTaxRules\Billy_SalesTaxRulesetsRepository;
use BillysBilling\Exception\Billy_Exception;

/**
 * Return the BillysBilling global settings form.
 */
function billysbilling_admin_settings() {
  $form['billysbilling_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('BillysBilling API Key'),
    '#required' => TRUE,
    '#default_value' => variable_get('billysbilling_api_key', ''),
    '#description' => t('The API key for your BillysBilling account. Get or generate a valid API key at BillysBilling. Please read the !termslink.', array('!termslink' => l(t('BillysBilling API Terms'), 'https://dev.billysbilling.dk/terms'))),
  );

  $form['billysbilling_invoice_message'] = array(
    '#type' => 'textfield',
    '#title' => t('Invoice message'),
    '#required' => TRUE,
    '#default_value' => variable_get('billysbilling_invoice_message', t('Thanks for shopping with us')),
    '#description' => t('Fixed message for the customer on the invoices.'),
  );

  $form['billysbilling_invoice_approved'] = array(
    '#type' => 'checkbox',
    '#title' => t('Mark invoices as approved immediately'),
    '#default_value' => variable_get('billysbilling_invoice_approved', false),
    '#description' => t('Should invoices automatically be marked as approved by synchronized from the website to BillysBilling?')
  );

  // Initiate API Client.
  try {
    $api_request = billysbilling_get_api_request();
  }
  catch (Exception $e) {
    drupal_set_message($e->getMessage(), 'error');
    return system_settings_form($form);
  }

  // Load VAT Models.
  $vat_options = array();
  try {
    $sales_tax_rulesets = new Billy_SalesTaxRulesetsRepository($api_request);
    $vat_options = array(
      '' => t('Choose'),
    );
    foreach ($sales_tax_rulesets->getAll() as $key => $ruleset) {
      $vat_options[$ruleset->getID()] = $ruleset->getName();
    }
  }
  catch (Billy_Exception $e) {
    drupal_set_message(t('Could not retrieve tax rulesets'), 'warning');
  }

  $form['billysbilling_vat_model_id'] = array(
    '#type' => 'select',
    '#title' => t('Default VAT Model'),
    '#options' => $vat_options,
    '#required' => TRUE,
    '#default_value' => variable_get('billysbilling_vat_model_id', ''),
    '#description' => t('Products will use this VAT model. Create the vat model manually in Billys Billing if not available.'),
    '#access' => empty($vat_options) ? false : true,
  );

  // Load accounts.
  $accounts_options = array();
  try {
    $accounts_repository = new Billy_AccountsRepository($api_request);
    $accounts_options = array(
      '' => t('Choose'),
    );
    /**
     * @var string $key
     * @var Billy_Account $model
     */
    foreach ($accounts_repository->getAll() as $key => $model) {
      $accounts_options[$model->getID()] = $model->getName();
    }
  }
  catch (Billy_Exception $e) {
    drupal_set_message(t('Could not retrieve accounts'), 'warning');
  }

  $form['billysbilling_state_account_id'] = array(
    '#type' => 'select',
    '#title' => t('Default State Account'),
    '#options' => $accounts_options,
    '#required' => TRUE,
    '#default_value' => variable_get('billysbilling_state_account_id', ''),
    '#description' => t('Products will be stated on this account. Create the account manually in Billys Billing if not available.'),
    '#access' => empty($accounts_options) ? false : true,
  );

  // Load products.
  $product_options = array();
  try {
    $products = new Billy_ProductsRepository($api_request);
    $product_options = array(
      '' => t('Choose'),
    );
    foreach ($products->getAll() as $key => $product) {
      $product_options[$product->getID()] = $product->getName();
    }
  }
  catch (Billy_Exception $e) {
    drupal_set_message(t('Could not retrieve tax rulesets'), 'warning');
  }

  $form['billysbilling_shipping_product_id'] = array(
    '#type' => 'select',
    '#title' => t('Shipping product'),
    '#options' => $product_options,
    '#required' => TRUE,
    '#default_value' => variable_get('billysbilling_shipping_product_id', ''),
    '#description' => t('Shipping line items will use this product in BillysBilling. Create the product manually in Billys Billing if not available.'),
    '#access' => empty($product_options) ? false : true,
  );

  $form['billysbilling_discount_product_id'] = array(
    '#type' => 'select',
    '#title' => t('Discount product'),
    '#options' => $product_options,
    '#required' => TRUE,
    '#default_value' => variable_get('billysbilling_discount_product_id', ''),
    '#description' => t('Coupon line items will use this product in BillysBilling. Create the product manually in Billys Billing if not available.'),
    '#access' => empty($product_options) ? false : true,
  );

  return system_settings_form($form);
}
