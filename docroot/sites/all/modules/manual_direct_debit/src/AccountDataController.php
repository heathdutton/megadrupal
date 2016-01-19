<?php

/**
 * @file
 * Defines the payment method controller for manual direct debit payments.
 */

namespace Drupal\manual_direct_debit;

use \Drupal\webform_paymethod_select\PaymentRecurrentController;

/**
 * Payment controller for manual direct debit payments.
 */
class AccountDataController extends \PaymentMethodController implements PaymentRecurrentController {
  public $payment_method_configuration_form_elements_callback;
  public $payment_configuration_form_elements_callback;
  /**
   * Define callbacks and classes.
   */
  public function __construct() {
    $this->payment_method_configuration_form_elements_callback = '\Drupal\manual_direct_debit\configuration_form';
    $this->payment_configuration_form_elements_callback        = 'payment_forms_method_form';
    $this->title = t('Collect account data (SEPA)');
    $this->form = new \Drupal\payment_forms\AccountForm();
  }

  /**
   * Validate the payment data.
   *
   * We heavily rely on the form validations of the AccountForm.
   * @see \Drupal\payment_forms\AccountForm
   */
  public function validate(\Payment $payment, \PaymentMethod $payment_method, $strict) {
    if (!$strict) {
      return parent::validate($payment, $payment_method, $strict);
    }

    $data = &$payment->method_data;
    if (isset($data['iban'])) {
      $data['country'] = substr($data['iban'], 0, 2);
    }
  }

  /**
   * Finish the payment.
   *
   * Our payments are always successful (if the validation succceeds) because
   * we only need to save the data.
   */
  public function execute(\Payment $payment) {
    $payment->setStatus(new \PaymentStatusItem(PAYMENT_STATUS_SUCCESS));
  }

  /**
   * Column headers for webform data.
   */
  public function webformDataInfo() {
    $info['account_holder'] = t('Account holder');
    $info['account_iban'] = t('IBAN');
    $info['account_bic'] = t('BIC');
    $info['account_country'] = t('Account country');
    $info['account_number'] = t('Account number');
    $info['account_bank_code'] = t('Bank code');
    $info['account_payment_date'] = t('Payment date');
    return $info;
  }

  /**
   * Data for webform results.
   */
  public function webformData(\Payment $payment) {
    $d = $payment->method_data;
    $data['account_holder'] = $d['holder'];
    $data['account_country'] = $d['country'];
    $data['account_iban'] = $d['iban'];
    $data['account_bic'] = $d['bic'];
    $data['account_number'] = $d['account'];
    $data['account_bank_code'] = $d['bank_code'];
    $data['account_payment_date'] = $d['payment_date'];
    return $data;
  }
}
