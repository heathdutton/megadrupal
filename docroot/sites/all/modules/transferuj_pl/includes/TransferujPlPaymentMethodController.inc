<?php

/**
 * @file
 * Contains TransferujPlPaymentMethodController.
 */

class TransferujPlPaymentMethodController extends PaymentMethodController {

  public $title = 'transferuj.pl';

  // Currencies allowed by transferuj.pl
  public $currencies = array("PLN" => array());

  // The array of IPs that are allowed to send to us verification requests.
  // TODO: Expose it somewhere?
  static public $returnIPs = array("195.149.229.109");

  // The url to which we are sending payers with payment data, to allow them
  // to make a transaction.
  public $redirectUrl = "https://secure.transferuj.pl";

  public $payment_method_configuration_form_elements_callback = 'transferuj_pl_payment_method_configuration_form_elements';

  // Language codes allowed by transferuj.pl
  public $languageCodes = array('PL', 'EN', 'DE');

  public $controllerDataDefaults = array(
    'sid' => '',
    'sdesc' => '',
    'channel' => '',
    'block' => 0,
    'online' => 0,
    'info_email' => '',
    'security_code' => '',
  );

  /**
   * Implements PaymentMethodController::execute().
   */
  public function execute(Payment $payment) {
    entity_save('payment', $payment);
    $_SESSION['transferuj_pl_pid'] = $payment->pid;
    drupal_goto('transferuj/redirect/' . $payment->pid);
  }

  /**
   * This method creates an array of parameters that will be sent to payment gateway.
   *
   * @param Payment $payment
   *   The payment we are sending information about to gateway.
   *
   * @return array
   *   Array of variables that will be sent to payment gateway.
   */
  public function redirectData(Payment $payment) {

    $security_code = $this->getControllerData($payment, 'security_code');

    $data = array(
      'id' => $this->getControllerData($payment, 'sid'),
      'kwota' => $payment->totalAmount(TRUE),
      'opis' => $payment->description,
      'online' => $this->getControllerData($payment, 'online'),
      'kanal' => $this->getControllerData($payment, 'channel'),
      'zablokuj' => $this->getControllerData($payment, 'block'),
      'wyn_url' => url('transferuj/verify', array('absolute' => TRUE)),
      'wyn_email' => $this->getControllerData($payment, 'info_email'),
      'opis_sprzed' => $this->getControllerData($payment, 'sdesc'),
      'pow_url' => url('transferuj/return/' . $payment->pid, array('absolute' => TRUE)),
      'jezyk' => $this->language(),
      'crc' => $payment->pid . ":" . variable_get('site_name', url('<front>', array('absolute' => TRUE))),
      'email' => user_load($payment->uid)->mail,
      /*
      // Those are other possible parameters. You can add them in
         hook_transferuj_pl_redirect_data(&$vars) from the source of choice.
      'nazwisko' => '',
      'adres' => '',
      'miasto' => '',
      'kod' => '',
      'kraj' => '',
      'telefon' => '',
      'akceptuje_regulamin' => '',
      */
    );

    drupal_alter('transferuj_pl_redirect_data', $data, $payment);

    $data['md5sum'] = md5($data['id'] . $data['kwota'] . $data['crc'] . $security_code);

    // Remove empty variables.
    foreach ($data as $key => $value) {
      if ($value == '') {
        unset($data[$key]);
      }
    }

    return $data;
  }

  /**
   * The method processing feedback from the payment gateway.
   *
   * @param array $data
   *   Array of variables.
   * @param Payment $payment
   *   Pending payment, the feedback relates to.
   */
  public function processFeedback(array $data, Payment $payment) {

    $payment->method_data = array(
      'pid' => $payment->pid,
      'id' => $data['id'],
      'tr_id' => $data['tr_id'],
      'tr_crc' => $data['tr_crc'],
      'tr_desc' => $data['tr_desc'],
      'tr_email' => $data['tr_email'],
    );

    if ($data['tr_status'] == "FALSE") {
      switch ($data['tr_error']) {
        case 'overpay':
          $payment->setStatus(new PaymentStatusItem(TRANSFERUJ_PL_STATUS_REJECTED_OVERPAYMENT));
          break;

        case 'surcharge':
          $payment->setStatus(new PaymentStatusItem(TRANSFERUJ_PL_STATUS_REJECTED_UNDERPAYMENT));
          break;

        case 'none':
          $payment->setStatus(new PaymentStatusItem(PAYMENT_STATUS_FAILED));
          break;

      }
    }
    elseif ($data['tr_status'] == "TRUE") {
      switch ($data['tr_error']) {
        case 'overpay':
          $payment->setStatus(new PaymentStatusItem(TRANSFERUJ_PL_STATUS_ACCEPTED_OVERPAYMENT));
          break;

        case 'surcharge':
          $payment->setStatus(new PaymentStatusItem(TRANSFERUJ_PL_STATUS_ACCEPTED_UNDERPAYMENT));
          break;

        case 'none':
          $payment->setStatus(new PaymentStatusItem(PAYMENT_STATUS_SUCCESS));
          break;

      }
    }

    $diffpay = $data['tr_paid'] - $data['tr_amount'];

    if ($diffpay > 0) {
      $payment->setLineItem(new PaymentLineItem(array(
        'amount' => $diffpay,
        'name' => 'transferuj_pl_diff',
        'description' => 'Overpay',
        'quantity' => 1,
      )));
    }
    elseif ($diffpay < 0) {
      $payment->setLineItem(new PaymentLineItem(array(
        'amount' => $diffpay,
        'name' => 'transferuj_pl_diff',
        'description' => 'Underpay',
        'quantity' => 1,
      )));
    }

    entity_save('payment', $payment);
    module_invoke_all('transferuj_pl_feedback', $data, $payment);
  }

  /**
   * Determine the language code, that will be used on redirection.
   *
   * @return string
   *   The language code in the form of string.
   */
  private function language() {
    global $language;

    foreach ($this->languageCodes as $language_code) {
      if (strtolower($language_code) == $language->language) {
        return $language_code;
      }
    }
    return 'PL';
  }

  /**
   * Helper function.
   *
   * @param Payment $payment
   *   The payment that has a controller, we are searching variables in.
   * @param string $key
   *   The name of the variable.
   *
   * @return string
   *   Returns a variable specific to controller.
   */
  private function getControllerData(Payment $payment, $key) {
    return isset($payment->method->controller_data[$key]) ? $payment->method->controller_data[$key] : '';
  }
}
