<?php

namespace Drupal\manual_direct_debit_uk;

use \Drupal\payment_forms\FormInterface;

class AccountForm implements FormInterface {
  static protected $id = 0;

  public function getForm(array &$form, array &$form_state, \Payment $payment) {
    $context = $payment->contextObj;
    if ($context && $context->value('donation_interval') != 1) {
      $form['payment_date'] = array(
        '#type' => 'select',
        '#title' => t('Payment date'),
        '#description' => t('On which date would you like the donation to be made each month?'),
        '#options' => array(
          '1st' => '1st of the month',
          '15th' => '15th of the month',
          '28th' => '28th of the month',
        ),
      );
    }
    $form['holder'] = array(
      '#type' => 'textfield',
      '#title' => t('Account holder(s)'),
    );
    $form['account'] = array(
      '#type' => 'textfield',
      '#title' => t('Account Number'),
      '#maxlength' => 10,
    );
    $form['bank_code'] = array(
      '#type' => 'textfield',
      '#title' => t('Branch Sort Code'),
      '#maxlength' => 6,
    );
    return $form;
  }

  public function validateForm(array &$element, array &$form_state, \Payment $payment) {
    $values = &drupal_array_get_nested_value($form_state['values'], $element['#parents']);
    // In case we have a one-off donation.
    $values += array('payment_date' => NULL);

    if (empty($values['holder']) == TRUE) {
      form_error($element['holder'], t('Please enter the name of the account holder(s).'));
    }

    $values['account'] = trim($values['account']);
    if (!$values['account'] || !preg_match('/[0-9]{2,11}/', $values['account'])) {
      form_error($element['account'], t('Please enter valid Account Number.'));
    }

    $values['bank_code'] = trim($values['bank_code']);
    if (!$values['bank_code'] || !preg_match('/[0-9]{2,6}/', $values['bank_code'])) {
      form_error($element['bank_code'], t('Please enter valid Branch Sort Code.'));
    }

    $method_data = &$payment->method_data;
    $method_data['holder'] = $values['holder'];
    $method_data['country'] = 'GB';
    $method_data['account'] = $values['account'];
    $method_data['bank_code'] = $values['bank_code'];
    $method_data['payment_date'] = $values['payment_date'];
  }
}
