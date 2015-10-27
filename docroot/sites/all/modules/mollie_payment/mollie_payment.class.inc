<?php

/**
 * This file contains classes for the Mollie Payment module.
 */

/**
 * Mollie payment method controller.
 */
class MolliePaymentMethodController extends PaymentMethodController {
  public $payment_method_configuration_form_elements_callback = 'mollie_payment_method_configuration';

  /**
   * Class constructor.
   */
  public function __construct() {
    $this->title = t('Mollie');
  }

  /**
   * Implements PaymentMethodController::execute().
   */
  public function execute(Payment $payment) {
    $controller_data = $payment->method->controller_data;
    $library = libraries_load('mollie_api');

    if ($library['loaded'] !== FALSE) {
      $mollie = new Mollie_API_Client();
      $api_key = $controller_data['mollie_id'];
      $mollie->setApiKey($api_key);

      $amount = 0;
      foreach ($payment->line_items as $line_item) {
        $amount += (1 + $line_item->tax_rate) * $line_item->amount
                * $line_item->quantity;
      }

      $payment_data = array(
        'amount' => $amount,
        'description' => $payment->description,
        'redirectUrl' => url(MOLLIE_PAYMENT_RETURN_PATH
                . '/' . $payment->pid, array('absolute' => TRUE)),
        'webhookUrl' => url(MOLLIE_PAYMENT_LISTENER_PATH
                . '/' . $payment->pid, array('absolute' => TRUE)),
      );

      $mollie_payment = $mollie->payments->create($payment_data);

      $redirect_url = $mollie_payment->getPaymentUrl();
      drupal_goto($redirect_url);
    }
    else {
      drupal_set_message(t('Could not load Mollie API client'), 'error');
    }
  }
}
