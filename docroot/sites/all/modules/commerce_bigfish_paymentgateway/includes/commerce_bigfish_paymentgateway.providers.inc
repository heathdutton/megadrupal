<?php
/**
 * @file
 * Implements available providers on BIG FISH Paymentgateway. 
 */

/**
 * Available providers on BIG FISH Paymentgateway.
 */
function commerce_bigfish_paymentgateway_providers() {
  return array(
    'commerce_bigfish_paymentgateway_cib' => array(
      'title' => t('BIG FISH Payment Gateway to CIB bank'),
      'description' => t('BIG FISH Payment Gateway to CIB bank (Hungary)'),
      'provider_id' => 'CIB',
    ),
    'commerce_bigfish_paymentgateway_escalion' => array(
      'title' => t('BIG FISH Payment Gateway to Escalation'),
      'description' => t('BIG FISH Payment Gateway to Escalation'),
      'provider_id' => 'Escalion',
    ),
    'commerce_bigfish_paymentgateway_fhb' => array(
      'title' => t('BIG FISH Payment Gateway to FHB bank'),
      'description' => t('BIG FISH Payment Gateway to FHB bank (Hungary)'),
      'provider_id' => 'FHB',
    ),
    'commerce_bigfish_paymentgateway_khb' => array(
      'title' => t('BIG FISH Payment Gateway to KHB bank'),
      'description' => t('BIG FISH Payment Gateway to KHB bank (Hungary)'),
      'provider_id' => 'KHB',
    ),
    'commerce_bigfish_paymentgateway_mpp' => array(
      'title' => t('BIG FISH Payment Gateway to MPP (Mobil pay)'),
      'description' => t('BIG FISH Payment Gateway to MPP (Mobil pay)'),
      'provider_id' => 'MPP',
    ),
    'commerce_bigfish_paymentgateway_mpp2' => array(
      'title' => t('BIG FISH Payment Gateway to MPP (Mobil pay)'),
      'description' => t('BIG FISH Payment Gateway to MPP (Mobil pay)'),
      'provider_id' => 'MPP2',
    ),
    'commerce_bigfish_paymentgateway_otp' => array(
      'title' => t('BIG FISH Payment Gateway to OTP bank'),
      'description' => t('BIG FISH Payment Gateway to OTP bank (Hungary)'),
      'provider_id' => 'OTP',
    ),
    'commerce_bigfish_paymentgateway_paypal' => array(
      'title' => t('BIG FISH Payment Gateway to PayPal'),
      'description' => t('BIG FISH Payment Gateway to PayPal'),
      'provider_id' => 'PayPal',
    ),
    'commerce_bigfish_paymentgateway_payu' => array(
      'title' => t('BIG FISH Payment Gateway to PayU'),
      'description' => t('BIG FISH Payment Gateway to PayU'),
      'provider_id' => 'PayU',
    ),
    'commerce_bigfish_paymentgateway_sms' => array(
      'title' => t('BIG FISH Payment Gateway to SMS'),
      'description' => t('BIG FISH Payment Gateway to SMS'),
      'provider_id' => 'SMS',
    ),
    'commerce_bigfish_paymentgateway_unicredit' => array(
      'title' => t('BIG FISH Payment Gateway to Unicredit bank'),
      'description' => t('BIG FISH Payment Gateway to Unicredit bank (Hungary)'),
      'provider_id' => 'UniCredit',
    ),
    'commerce_bigfish_paymentgateway_qpay' => array(
      'title' => t('BIG FISH Payment Gateway to Wirecard QPAY'),
      'description' => t('BIG FISH Payment Gateway to Wirecard QPAY'),
      'provider_id' => 'QPAY',
    ),
  );
}
