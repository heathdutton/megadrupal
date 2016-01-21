<?php

/**
 * @file
 * Rules integration for the commerce sermepa module.
 */

use CommerceRedsys\Payment\Sermepa;

/**
 * Implements hook_rules_data_info().
 */
function commerce_sermepa_rules_data_info() {
  return array(
    'commerce_sermepa_gateway_instance' => array(
      'label' => t('Sermepa/Redsys gateway instance'),
    ),
  );
}

/**
 * Implements hook_rules_event_info().
 */
function commerce_sermepa_rules_event_info() {
  $items = array();

  $items['commerce_sermepa_gateway'] = array(
    'label' => t('Configure Sermepa/Redsys gateway to a given order'),
    'group' => t('Commerce Sermepa'),
    'variables' => array(
      'commerce_sermepa_gateway_instance' => array(
        'type' => 'commerce_sermepa_gateway_instance',
        'label' => t('Sermepa/Redsys gateway'),
      ),
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order', array(), array('context' => 'a drupal commerce order')),
      ),
    ),
    'access callback' => 'commerce_order_rules_access',
  );

  return $items;
}

/**
 * Implements hook_rules_action_info().
 */
function commerce_sermepa_rules_action_info() {
  $actions = array();

  $actions['commerce_sermepa_set_amount'] = array(
    'label' => t('Set the Sermepa gateway Ds_Merchant_Amount parameter'),
    'parameter' => array(
      'commerce_sermepa_gateway_instance' => array(
        'type' => 'commerce_sermepa_gateway_instance',
        'label' => t('Sermepa/Redsys gateway'),
        'description' => t('The Sermepa/Redsys gateway instance to be setted.'),
      ),
      'amount' => array(
        'type' => 'integer',
        'label' => t('Amount'),
        'description' => t('Specify a numeric amount to set the Sermepa gateway amount. To Euros the last two places are considered decimals.'),
      ),
    ),
    'group' => t('Commerce Sermepa'),
  );
  $actions['commerce_sermepa_set_authorisation_code'] = array(
    'label' => t('Set the Sermepa gateway Ds_Merchant_AuthorisationCode parameter'),
    'parameter' => array(
      'commerce_sermepa_gateway_instance' => array(
        'type' => 'commerce_sermepa_gateway_instance',
        'label' => t('Sermepa/Redsys gateway'),
        'description' => t('The Sermepa/Redsys gateway instance to be setted.'),
      ),
      'authorisation_code' => array(
        'type' => 'integer',
        'label' => t('Authorisation code'),
        'description' => t('Represents the authorization code necessary to identify a  recurring transaction returns successively in subsequent recurring transactions. Required in continuing returner operations.'),
      ),
    ),
    'group' => t('Commerce Sermepa'),
  );
  $actions['commerce_sermepa_set_charge_expiry_date'] = array(
    'label' => t('Set the Sermepa gateway Ds_Merchant_ChargeExpiryDate parameter'),
    'parameter' => array(
      'commerce_sermepa_gateway_instance' => array(
        'type' => 'commerce_sermepa_gateway_instance',
        'label' => t('Sermepa/Redsys gateway'),
        'description' => t('The Sermepa/Redsys gateway instance to be setted.'),
      ),
      'charge_expiry_date' => array(
        'type' => 'text',
        'label' => t('Charge expiry date'),
        'description' => t('Format yyyy-MM-dd date for recurring transactions (required for recurring and recurring deferred).'),
      ),
    ),
    'group' => t('Commerce Sermepa'),
  );
  $actions['commerce_sermepa_set_consumer_language'] = array(
    'label' => t('Set the Sermepa gateway Ds_Merchant_ConsumerLanguage parameter'),
    'parameter' => array(
      'commerce_sermepa_gateway_instance' => array(
        'type' => 'commerce_sermepa_gateway_instance',
        'label' => t('Sermepa/Redsys gateway'),
        'description' => t('The Sermepa/Redsys gateway instance to be setted.'),
      ),
      'consumer_language' => array(
        'type' => 'text',
        'label' => t('Consumer language'),
        'description' => t('Sermepa gateway user interface language.'),
        'options list' => 'commerce_sermepa_get_available_consumer_languages',
      ),
    ),
    'group' => t('Commerce Sermepa'),
  );
  $actions['commerce_sermepa_set_currency'] = array(
    'label' => t('Set the Sermepa gateway Ds_Merchant_Currency parameter'),
    'parameter' => array(
      'commerce_sermepa_gateway_instance' => array(
        'type' => 'commerce_sermepa_gateway_instance',
        'label' => t('Sermepa/Redsys gateway'),
        'description' => t('The Sermepa/Redsys gateway instance to be setted.'),
      ),
      'currency' => array(
        'type' => 'text',
        'label' => t('Currency'),
        'description' => t('The numeric currency code.'),
        'options list' => 'commerce_sermepa_get_available_currencies',
      ),
    ),
    'group' => t('Commerce Sermepa'),
  );
  $actions['commerce_sermepa_set_date_frecuency'] = array(
    'label' => t('Set the Sermepa gateway Ds_Merchant_DateFrecuency parameter'),
    'parameter' => array(
      'commerce_sermepa_gateway_instance' => array(
        'type' => 'commerce_sermepa_gateway_instance',
        'label' => t('Sermepa/Redsys gateway'),
        'description' => t('The Sermepa/Redsys gateway instance to be setted.'),
      ),
      'date_frecuency' => array(
        'type' => 'text',
        'label' => t('Date frecuency'),
        'description' => t('Frequency in days for recurring transactions, recurring deferred.'),
      ),
    ),
    'group' => t('Commerce Sermepa'),
  );
  $actions['commerce_sermepa_set_merchant_code'] = array(
    'label' => t('Set the Sermepa gateway Ds_Merchant_MerchantCode parameter'),
    'parameter' => array(
      'commerce_sermepa_gateway_instance' => array(
        'type' => 'commerce_sermepa_gateway_instance',
        'label' => t('Sermepa/Redsys gateway'),
        'description' => t('The Sermepa/Redsys gateway instance to be setted.'),
      ),
      'merchant_code' => array(
        'type' => 'text',
        'label' => t('Merchant code'),
        'description' => t('FUC Code assigned to commerce'),
      ),
    ),
    'group' => t('Commerce Sermepa'),
  );
  $actions['commerce_sermepa_set_merchant_data'] = array(
    'label' => t('Set the Sermepa gateway Ds_Merchant_MerchantData parameter'),
    'parameter' => array(
      'commerce_sermepa_gateway_instance' => array(
        'type' => 'commerce_sermepa_gateway_instance',
        'label' => t('Sermepa/Redsys gateway'),
        'description' => t('The Sermepa/Redsys gateway instance to be setted.'),
      ),
      'merchant_data' => array(
        'type' => 'text',
        'label' => t('Merchant data'),
        'description' => t('Field for commerce to be included in the data sent by the "on-line" answer to trade if you have chosen this option. It is used to recognize the order in the feedback process.'),
      ),
    ),
    'group' => t('Commerce Sermepa'),
  );
  $actions['commerce_sermepa_set_merchant_name'] = array(
    'label' => t('Set the Sermepa gateway Ds_Merchant_MerchantName parameter'),
    'parameter' => array(
      'commerce_sermepa_gateway_instance' => array(
        'type' => 'commerce_sermepa_gateway_instance',
        'label' => t('Sermepa/Redsys gateway'),
        'description' => t('The Sermepa/Redsys gateway instance to be setted.'),
      ),
      'merchant_name' => array(
        'type' => 'text',
        'label' => t('Merchant name'),
        'description' => t('The commerce name that appear on the ticket that the client'),
      ),
    ),
    'group' => t('Commerce Sermepa'),
  );
  $actions['commerce_sermepa_set_merchant_password'] = array(
    'label' => t('Set the Sermepa gateway SHA256 merchant password parameter'),
    'parameter' => array(
      'commerce_sermepa_gateway_instance' => array(
        'type' => 'commerce_sermepa_gateway_instance',
        'label' => t('Sermepa/Redsys gateway'),
        'description' => t('The Sermepa/Redsys gateway instance to be setted.'),
      ),
      'merchant_password' => array(
        'type' => 'text',
        'label' => t('SHA256 merchant password'),
        'description' => t('Commerce SHa256 password.'),
      ),
    ),
    'group' => t('Commerce Sermepa'),
  );
  $actions['commerce_sermepa_set_merchant_url'] = array(
    'label' => t('Set the Sermepa gateway Ds_Merchant_MerchantURL parameter'),
    'parameter' => array(
      'commerce_sermepa_gateway_instance' => array(
        'type' => 'commerce_sermepa_gateway_instance',
        'label' => t('Sermepa/Redsys gateway'),
        'description' => t('The Sermepa/Redsys gateway instance to be setted.'),
      ),
      'merchant_url' => array(
        'type' => 'text',
        'label' => t('Merchant URL'),
        'description' => t('The URL of commerce that receive a post with transaction data.'),
      ),
    ),
    'group' => t('Commerce Sermepa'),
  );
  $actions['commerce_sermepa_set_order'] = array(
    'label' => t('Set the Sermepa gateway Ds_Merchant_Order parameter'),
    'parameter' => array(
      'commerce_sermepa_gateway_instance' => array(
        'type' => 'commerce_sermepa_gateway_instance',
        'label' => t('Sermepa/Redsys gateway'),
        'description' => t('The Sermepa/Redsys gateway instance to be setted.'),
      ),
      'order' => array(
        'type' => 'text',
        'label' => t('Order'),
        'description' => t('The order identifier.'),
      ),
    ),
    'group' => t('Commerce Sermepa'),
  );
  $actions['commerce_sermepa_set_payment_method'] = array(
    'label' => t('Set the Sermepa gateway Ds_Merchant_PayMethod parameter'),
    'parameter' => array(
      'commerce_sermepa_gateway_instance' => array(
        'type' => 'commerce_sermepa_gateway_instance',
        'label' => t('Sermepa/Redsys gateway'),
        'description' => t('The Sermepa/Redsys gateway instance to be setted.'),
      ),
      'payment_method' => array(
        'type' => 'text',
        'label' => t('Payment Method'),
        'description' => t('This variable is used to skip the payment method selection when more than one is enabled for the sermepa account.'),
      ),
    ),
    'group' => t('Commerce Sermepa'),
  );
  $actions['commerce_sermepa_set_product_description'] = array(
    'label' => t('Set the Sermepa gateway Ds_Merchant_ProductDescription parameter'),
    'parameter' => array(
      'commerce_sermepa_gateway_instance' => array(
        'type' => 'commerce_sermepa_gateway_instance',
        'label' => t('Sermepa/Redsys gateway'),
        'description' => t('The Sermepa/Redsys gateway instance to be setted.'),
      ),
      'product_description' => array(
        'type' => 'text',
        'label' => t('Product description'),
        'description' => t('This field are displayed to the holder on the screen confirmation of purchase.'),
      ),
    ),
    'group' => t('Commerce Sermepa'),
  );
  $actions['commerce_sermepa_set_sum_total'] = array(
    'label' => t('Set the Sermepa gateway Ds_Merchant_SumTotal parameter'),
    'parameter' => array(
      'commerce_sermepa_gateway_instance' => array(
        'type' => 'commerce_sermepa_gateway_instance',
        'label' => t('Sermepa/Redsys gateway'),
        'description' => t('The Sermepa/Redsys gateway instance to be setted.'),
      ),
      'sum_total' => array(
        'type' => 'integer',
        'label' => t('Sum total'),
        'description' => t('Represents the sum of the amounts of installments. The latter two are considered decimal positions.'),
      ),
    ),
    'group' => t('Commerce Sermepa'),
  );
  $actions['commerce_sermepa_set_terminal'] = array(
    'label' => t('Set the Sermepa gateway Ds_Merchant_Terminal parameter'),
    'parameter' => array(
      'commerce_sermepa_gateway_instance' => array(
        'type' => 'commerce_sermepa_gateway_instance',
        'label' => t('Sermepa/Redsys gateway'),
        'description' => t('The Sermepa/Redsys gateway instance to be setted.'),
      ),
      'terminal' => array(
        'type' => 'text',
        'label' => t('Terminal'),
        'description' => t('The terminal number assigned by your bank.'),
      ),
    ),
    'group' => t('Commerce Sermepa'),
  );
  $actions['commerce_sermepa_set_titular'] = array(
    'label' => t('Set the Sermepa gateway Ds_Merchant_Titular parameter'),
    'parameter' => array(
      'commerce_sermepa_gateway_instance' => array(
        'type' => 'commerce_sermepa_gateway_instance',
        'label' => t('Sermepa/Redsys gateway'),
        'description' => t('The Sermepa/Redsys gateway instance to be setted.'),
      ),
      'titular' => array(
        'type' => 'text',
        'label' => t('Cardholder'),
        'description' => t('The cardholder and are displayed to the holder on the screen confirmation of purchase.'),
      ),
    ),
    'group' => t('Commerce Sermepa'),
  );
  $actions['commerce_sermepa_set_transaction_date'] = array(
    'label' => t('Set the Sermepa gateway Ds_Merchant_TransactionDate parameter'),
    'parameter' => array(
      'commerce_sermepa_gateway_instance' => array(
        'type' => 'commerce_sermepa_gateway_instance',
        'label' => t('Sermepa/Redsys gateway'),
        'description' => t('The Sermepa/Redsys gateway instance to be setted.'),
      ),
      'transaction_date' => array(
        'type' => 'text',
        'label' => t('Transaction date'),
        'description' => t('Format yyyy-MM-dd. Represents the date of subsequent recurrent operation is required to identify the transaction in successive returns from continuing operations. Required for returns from continuing operations continuing operations and deferred.'),
      ),
    ),
    'group' => t('Commerce Sermepa'),
  );
  $actions['commerce_sermepa_set_transaction_type'] = array(
    'label' => t('Set the Sermepa gateway Ds_Merchant_TransactionType parameter'),
    'parameter' => array(
      'commerce_sermepa_gateway_instance' => array(
        'type' => 'commerce_sermepa_gateway_instance',
        'label' => t('Sermepa/Redsys gateway'),
        'description' => t('The Sermepa/Redsys gateway instance to be setted.'),
      ),
      'transaction_type' => array(
        'type' => 'text',
        'label' => t('Transaction type'),
        'description' => t('Value fot indicate what type of transaction it is.'),
        'options list' => 'commerce_sermepa_get_available_transaction_types',
      ),
    ),
    'group' => t('Commerce Sermepa'),
  );
  $actions['commerce_sermepa_set_url_ko'] = array(
    'label' => t('Set the Sermepa gateway Ds_Merchant_UrlKO parameter'),
    'parameter' => array(
      'commerce_sermepa_gateway_instance' => array(
        'type' => 'commerce_sermepa_gateway_instance',
        'label' => t('Sermepa/Redsys gateway'),
        'description' => t('The Sermepa/Redsys gateway instance to be setted.'),
      ),
      'url_ko' => array(
        'type' => 'text',
        'label' => t('URL KO'),
        'description' => t('The returns URL when the payment process fails.'),
      ),
    ),
    'group' => t('Commerce Sermepa'),
  );
  $actions['commerce_sermepa_set_url_ok'] = array(
    'label' => t('Set the Sermepa gateway Ds_Merchant_UrlOK parameter'),
    'parameter' => array(
      'commerce_sermepa_gateway_instance' => array(
        'type' => 'commerce_sermepa_gateway_instance',
        'label' => t('Sermepa/Redsys gateway'),
        'description' => t('The Sermepa/Redsys gateway instance to be setted.'),
      ),
      'url_ok' => array(
        'type' => 'text',
        'label' => t('URL OK'),
        'description' => t('The return URL when the payment process has been successful.'),
      ),
    ),
    'group' => t('Commerce Sermepa'),
  );

  return $actions;
}

/**
 * Action: Set the Sermepa gateway Ds_Merchant_Amount parameter.
 *
 * @param \CommerceRedsys\Payment\Sermepa $gateway
 *   The Sermepa gateway instance.
 * @param integer $value
 *   The Ds_Merchant_Amount value to being setted.
 */
function commerce_sermepa_set_amount($gateway, $value) {
  $gateway->setAmount($value);
}

/**
 * Action: Set the Sermepa gateway Ds_Merchant_AuthorisationCode parameter.
 *
 * @param \CommerceRedsys\Payment\Sermepa $gateway
 *   The Sermepa gateway instance.
 * @param integer $value
 *   The Ds_Merchant_AuthorisationCode value to being setted.
 */
function commerce_sermepa_set_authorisation_code($gateway, $value) {
  $gateway->setAuthorisationCode($value);
}

/**
 * Action: Set the Sermepa gateway Ds_Merchant_ChargeExpiryDate parameter.
 *
 * @param \CommerceRedsys\Payment\Sermepa $gateway
 *   The Sermepa gateway instance.
 * @param sring $value
 *   The Ds_Merchant_ChargeExpiryDate value to being setted.
 */
function commerce_sermepa_set_charge_expiry_date($gateway, $value) {
  $gateway->setChargeExpiryDate($value);
}

/**
 * Action: Set the Sermepa gateway Ds_Merchant_ConsumerLanguage parameter.
 *
 * @param \CommerceRedsys\Payment\Sermepa $gateway
 *   The Sermepa gateway instance.
 * @param sting $value
 *   The Ds_Merchant_ConsumerLanguage value to being setted.
 */
function commerce_sermepa_set_consumer_language($gateway, $value) {
  $gateway->setConsumerLanguage($value);
}

/**
 * Action: Set the Sermepa gateway Ds_Merchant_Currency parameter.
 *
 * @param \CommerceRedsys\Payment\Sermepa $gateway
 *   The Sermepa gateway instance.
 * @param string $value
 *   The Ds_Merchant_Currency value to being setted.
 */
function commerce_sermepa_set_currency($gateway, $value) {
  $gateway->setCurrency($value);
}

/**
 * Action: Set the Sermepa gateway Ds_Merchant_DateFrecuency parameter.
 *
 * @param \CommerceRedsys\Payment\Sermepa $gateway
 *   The Sermepa gateway instance.
 * @param string $value
 *   The Ds_Merchant_DateFrecuency value to being setted.
 */
function commerce_sermepa_set_date_frecuency($gateway, $value) {
  $gateway->setDateFrecuency($value);
}

/**
 * Action: Set the Sermepa gateway Ds_Merchant_MerchantCode parameter.
 *
 * @param \CommerceRedsys\Payment\Sermepa $gateway
 *   The Sermepa gateway instance.
 * @param string $value
 *   The Ds_Merchant_MerchantCode value to being setted.
 */
function commerce_sermepa_set_merchant_code($gateway, $value) {
  $gateway->setMerchantCode($value);
}

/**
 * Action: Set the Sermepa gateway Ds_Merchant_MerchantData parameter.
 *
 * @param \CommerceRedsys\Payment\Sermepa $gateway
 *   The Sermepa gateway instance.
 * @param string $value
 *   The Ds_Merchant_MerchantData value to being setted.
 */
function commerce_sermepa_set_merchant_data($gateway, $value) {
  $gateway->setMerchantData($value);
}

/**
 * Action: Set the Sermepa gateway Ds_Merchant_MerchantName parameter.
 *
 * @param \CommerceRedsys\Payment\Sermepa $gateway
 *   The Sermepa gateway instance.
 * @param string $value
 *   The Ds_Merchant_MerchantName value to being setted.
 */
function commerce_sermepa_set_merchant_name($gateway, $value) {
  $gateway->setMerchantName($value);
}

/**
 * Action: Set the Sermepa gateway SHA256 merchant password parameter.
 *
 * @param \CommerceRedsys\Payment\Sermepa $gateway
 *   The Sermepa gateway instance.
 * @param string $value
 *   The SHA256 merchant password value to being setted.
 */
function commerce_sermepa_set_merchant_password($gateway, $value) {
  $gateway->setMerchantPassword($value);
}

/**
 * Action: Set the Sermepa gateway Ds_Merchant_MerchantURL parameter.
 *
 * @param \CommerceRedsys\Payment\Sermepa $gateway
 *   The Sermepa gateway instance.
 * @param string $value
 *   The Ds_Merchant_MerchantURL value to being setted.
 */
function commerce_sermepa_set_merchant_url($gateway, $value) {
  $gateway->setMerchantURL($value);
}

/**
 * Action: Set the Sermepa gateway Ds_Merchant_Order parameter.
 *
 * @param \CommerceRedsys\Payment\Sermepa $gateway
 *   The Sermepa gateway instance.
 * @param string $value
 *   The Ds_Merchant_Order value to being setted.
 */
function commerce_sermepa_set_order($gateway, $value) {
  $gateway->setOrder($value);
}

/**
 * Action: Set the Sermepa gateway Ds_Merchant_PayMethod parameter.
 *
 * @param \CommerceRedsys\Payment\Sermepa $gateway
 *   The Sermepa gateway instance.
 * @param string $value
 *   The Ds_Merchant_PayMethod value to being setted.
 */
function commerce_sermepa_set_payment_method($gateway, $value) {
  $gateway->setPaymentMethod($value);
}

/**
 * Action: Set the Sermepa gateway Ds_Merchant_ProductDescription parameter.
 *
 * @param \CommerceRedsys\Payment\Sermepa $gateway
 *   The Sermepa gateway instance.
 * @param string $value
 *   The Ds_Merchant_ProductDescription value to being setted.
 */
function commerce_sermepa_set_product_description($gateway, $value) {
  $gateway->setProductDescription($value);
}

/**
 * Action: Set the Sermepa gateway Ds_Merchant_SumTotal parameter.
 *
 * @param \CommerceRedsys\Payment\Sermepa $gateway
 *   The Sermepa gateway instance.
 * @param integer $value
 *   The Ds_Merchant_SumTotal value to being setted.
 */
function commerce_sermepa_set_sum_total($gateway, $value) {
  $gateway->setSumTotal($value);
}

/**
 * Action: Set the Sermepa gateway Ds_Merchant_Terminal parameter.
 *
 * @param \CommerceRedsys\Payment\Sermepa $gateway
 *   The Sermepa gateway instance.
 * @param string $value
 *   The Ds_Merchant_Terminal value to being setted.
 */
function commerce_sermepa_set_terminal($gateway, $value) {
  $gateway->setTerminal($value);
}

/**
 * Action: Set the Sermepa gateway Ds_Merchant_Titular parameter.
 *
 * @param \CommerceRedsys\Payment\Sermepa $gateway
 *   The Sermepa gateway instance.
 * @param string $value
 *   The Ds_Merchant_Titular value to being setted.
 */
function commerce_sermepa_set_titular($gateway, $value) {
  $gateway->setTitular($value);
}

/**
 * Action: Set the Sermepa gateway Ds_Merchant_TransactionDate parameter.
 *
 * @param \CommerceRedsys\Payment\Sermepa $gateway
 *   The Sermepa gateway instance.
 * @param string $value
 *   The Ds_Merchant_TransactionDate value to being setted.
 */
function commerce_sermepa_set_transaction_date($gateway, $value) {
  $gateway->setTransactionDate($value);
}

/**
 * Action: Set the Sermepa gateway Ds_Merchant_TransactionType parameter.
 *
 * @param \CommerceRedsys\Payment\Sermepa $gateway
 *   The Sermepa gateway instance.
 * @param string $value
 *   The Ds_Merchant_TransactionType value to being setted.
 */
function commerce_sermepa_set_transaction_type($gateway, $value) {
  $gateway->setTransactionType($value);
}

/**
 * Action: Set the Sermepa gateway Ds_Merchant_UrlKO parameter.
 *
 * @param \CommerceRedsys\Payment\Sermepa $gateway
 *   The Sermepa gateway instance.
 * @param string $value
 *   The Ds_Merchant_UrlKO value to being setted.
 */
function commerce_sermepa_set_url_ko($gateway, $value) {
  $gateway->setUrlKO($value);
}

/**
 * Action: Set the Sermepa gateway Ds_Merchant_UrlOK parameter.
 *
 * @param \CommerceRedsys\Payment\Sermepa $gateway
 *   The Sermepa gateway instance.
 * @param string $value
 *   The Ds_Merchant_UrlOK value to being setted.
 */
function commerce_sermepa_set_url_ok($gateway, $value) {
  $gateway->setUrlOK($value);
}

/**
 * Get all available consumer languages.
 *
 * @return array
 *   Return an array with all available languages.
 */
function commerce_sermepa_get_available_consumer_languages() {
  if (!class_exists('Sermepa')) {
    libraries_load('sermepa');
  }

  return Sermepa::getAvailableConsumerLanguages();
}

/**
 * Get all available currencies.
 *
 * @return array
 *   Return an array with all available currencies.
 */
function commerce_sermepa_get_available_currencies() {
  if (!class_exists('Sermepa')) {
    libraries_load('sermepa');
  }

  return Sermepa::getAvailableCurrencies();
}

/**
 * Get all available transaction types.
 *
 * @return array
 *   Return an array with all available transaction types.
 */
function commerce_sermepa_get_available_transaction_types() {
  if (!class_exists('Sermepa')) {
    libraries_load('sermepa');
  }

  return Sermepa::getAvailableTransactionTypes();
}
