<?php
/**
 * @file
 * Implements available providers on BIG FISH Paymentgateway. 
 */

/**
 * Available providers on BIG FISH Paymentgateway.
 */
function commerce_bigfish_paymentgateway_providers() {
  _commerce_bigfish_paymentgateway_load_library();

  return array(
    'commerce_bigfish_paymentgateway_abaqoos' => array(
      'title' => t('BIG FISH Payment Gateway to Abaqoos payment service'),
      'description' => t('BIG FISH Payment Gateway to Abaqoos payment service (Hungary)'),
      'provider_id' => \BigFish\PaymentGateway::PROVIDER_ABAQOOS,
    ),
    'commerce_bigfish_paymentgateway_barion' => array(
      'title' => t('BIG FISH Payment Gateway to Barion payment service'),
      'description' => t('BIG FISH Payment Gateway to Barion payment service (Hungary)'),
      'provider_id' => \BigFish\PaymentGateway::PROVIDER_BARION,
    ),
    'commerce_bigfish_paymentgateway_borgun' => array(
      'title' => t('BIG FISH Payment Gateway to Borgun payment service'),
      'description' => t('BIG FISH Payment Gateway to Borgun payment service (Hungary)'),
      'provider_id' => \BigFish\PaymentGateway::PROVIDER_BORGUN,
    ),
    'commerce_bigfish_paymentgateway_cib' => array(
      'title' => t('BIG FISH Payment Gateway to CIB bank'),
      'description' => t('BIG FISH Payment Gateway to CIB bank (Hungary)'),
      'provider_id' => \BigFish\PaymentGateway::PROVIDER_CIB,
    ),
    'commerce_bigfish_paymentgateway_escalion' => array(
      'title' => t('BIG FISH Payment Gateway to Escalion'),
      'description' => t('BIG FISH Payment Gateway to Escalion'),
      'provider_id' => \BigFish\PaymentGateway::PROVIDER_ESCALION,
    ),
    'commerce_bigfish_paymentgateway_fhb' => array(
      'title' => t('BIG FISH Payment Gateway to FHB bank'),
      'description' => t('BIG FISH Payment Gateway to FHB bank (Hungary)'),
      'provider_id' => \BigFish\PaymentGateway::PROVIDER_FHB,
    ),
    'commerce_bigfish_paymentgateway_khb' => array(
      'title' => t('BIG FISH Payment Gateway to KHB bank'),
      'description' => t('BIG FISH Payment Gateway to KHB bank (Hungary)'),
      'provider_id' => \BigFish\PaymentGateway::PROVIDER_KHB,
    ),
    /*
    'commerce_bigfish_paymentgateway_mpp' => array(
      'title' => t('BIG FISH Payment Gateway to Mobile Payment service'),
      'description' => t('BIG FISH Payment Gateway to Mobile Payment service'),
      'provider_id' => 'MPP',
    ),
    */
    'commerce_bigfish_paymentgateway_mpp2' => array(
      'title' => t('BIG FISH Payment Gateway to MasterCard Mobile Payment service'),
      'description' => t('BIG FISH Payment Gateway to MasterCard Mobile Payment service'),
      'provider_id' => \BigFish\PaymentGateway::PROVIDER_MASTERCARD_MOBILE,
    ),
    'commerce_bigfish_paymentgateway_otp' => array(
      'title' => t('BIG FISH Payment Gateway to OTP bank'),
      'description' => t('BIG FISH Payment Gateway to OTP bank (Hungary)'),
      'provider_id' => \BigFish\PaymentGateway::PROVIDER_OTP,
    ),
    'commerce_bigfish_paymentgateway_otp2' => array(
      'title' => t('BIG FISH Payment Gateway to OTP bank (Two-party)'),
      'description' => t('BIG FISH Payment Gateway to OTP bank (Two-party) (Hungary)'),
      'provider_id' => \BigFish\PaymentGateway::PROVIDER_OTP_TWO_PARTY,
    ),
    'commerce_bigfish_paymentgateway_otpay' => array(
      'title' => t('BIG FISH Payment Gateway to OTPay payment service'),
      'description' => t('BIG FISH Payment Gateway to OTPay payment service (Hungary)'),
      'provider_id' => \BigFish\PaymentGateway::PROVIDER_OTPAY,
    ),
    'commerce_bigfish_paymentgateway_otpaymp' => array(
      'title' => t('BIG FISH Payment Gateway to OTPay MasterPass payment service'),
      'description' => t('BIG FISH Payment Gateway to OTPay MasterPass payment service (Hungary)'),
      'provider_id' => \BigFish\PaymentGateway::PROVIDER_OTPAY_MASTERPASS,
    ),
    'commerce_bigfish_paymentgateway_otpmultipont' => array(
      'title' => t('BIG FISH Payment Gateway to OTP Multipont payment service'),
      'description' => t('BIG FISH Payment Gateway to OTP Multipont payment service (Hungary)'),
      'provider_id' => \BigFish\PaymentGateway::PROVIDER_OTP_MULTIPONT,
    ),
    'commerce_bigfish_paymentgateway_paypal' => array(
      'title' => t('BIG FISH Payment Gateway to PayPal'),
      'description' => t('BIG FISH Payment Gateway to PayPal'),
      'provider_id' => \BigFish\PaymentGateway::PROVIDER_PAYPAL,
    ),
    'commerce_bigfish_paymentgateway_payu' => array(
      'title' => t('BIG FISH Payment Gateway to PayU'),
      'description' => t('BIG FISH Payment Gateway to PayU'),
      'provider_id' => \BigFish\PaymentGateway::PROVIDER_PAYU,
    ),
    'commerce_bigfish_paymentgateway_payucash' => array(
      'title' => t('BIG FISH Payment Gateway to PayU Cash payment service'),
      'description' => t('BIG FISH Payment Gateway to PayU Cash payment service (Hungary)'),
      'provider_id' => \BigFish\PaymentGateway::PROVIDER_PAYU_CASH,
    ),
    'commerce_bigfish_paymentgateway_payuwire' => array(
      'title' => t('BIG FISH Payment Gateway to PayU Wire payment service'),
      'description' => t('BIG FISH Payment Gateway to PayU Wire payment service (Hungary)'),
      'provider_id' => \BigFish\PaymentGateway::PROVIDER_PAYU_WIRE,
    ),
    'commerce_bigfish_paymentgateway_qpay' => array(
      'title' => t('BIG FISH Payment Gateway to Wirecard QPAY'),
      'description' => t('BIG FISH Payment Gateway to Wirecard QPAY'),
      'provider_id' => \BigFish\PaymentGateway::PROVIDER_WIRECARD_QPAY,
    ),
    'commerce_bigfish_paymentgateway_sms' => array(
      'title' => t('BIG FISH Payment Gateway to SMS'),
      'description' => t('BIG FISH Payment Gateway to SMS'),
      'provider_id' => \BigFish\PaymentGateway::PROVIDER_SMS,
    ),
    'commerce_bigfish_paymentgateway_sofort' => array(
      'title' => t('BIG FISH Payment Gateway to Sofort payment service'),
      'description' => t('BIG FISH Payment Gateway to Sofort payment service (Hungary)'),
      'provider_id' => \BigFish\PaymentGateway::PROVIDER_SOFORT,
    ),
    'commerce_bigfish_paymentgateway_unicredit' => array(
      'title' => t('BIG FISH Payment Gateway to Unicredit bank'),
      'description' => t('BIG FISH Payment Gateway to Unicredit bank (Hungary)'),
      'provider_id' => \BigFish\PaymentGateway::PROVIDER_UNICREDIT,
    ),
  );
}
