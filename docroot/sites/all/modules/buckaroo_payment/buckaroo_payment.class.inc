<?php

/**
 * This file contains classes for the Buckaroo Payment module.
 */

/**
 * Buckaroo payment method controller.
 */
class BuckarooPaymentMethodController extends PaymentMethodController {
  public $payment_method_configuration_form_elements_callback = 'buckaroo_payment_method_configuration';

  /**
   * Class constructor.
   */
  function __construct() {
    $this->title = t('Buckaroo');
  }

  /**
   * Implements PaymentMethodController::execute().
   */
  public function execute(Payment $payment) {
    $controller_data = $payment->method->controller_data;
    $secret_key = $controller_data['buckaroo_secret'];

    $hash = md5($payment->pid . $secret_key);
    drupal_goto('payment/buckaroo/form/' . $payment->pid . '/' . $hash);
  }

  /**
   * @todo Write function documentation.
   */
  public function form(Payment $payment, $form, &$form_state) {
    global $base_url;

    $controller_data = $payment->method->controller_data;

    // Get settings from controller data.
    $merchant_id = $controller_data['buckaroo_id'];
    $secret_key = $controller_data['buckaroo_secret'];
    $url = $controller_data['buckaroo_mode'];
    $encryption = $controller_data['buckaroo_encryption'];

    /**
     * Calculate amount to pay.
     */
    $amount = 0;
    foreach ($payment->line_items as $line_item) {
      $amount += (1 + $line_item->tax_rate) * $line_item->amount
              * $line_item->quantity;
    }

    $payment_data = array(
      'amount' => $amount,
      'description' => $payment->description,
    );

    /*
     * @todo Make some options configurable.
     */
    $data = array(
      'brq_amount' => number_format($payment_data['amount'], 2, '.', ''),
      'brq_culture' => 'nl-NL',
      'brq_currency' => 'EUR',
      'brq_invoicenumber' => $payment->pid,
      'brq_websitekey' => $merchant_id,
      'brq_return' => url(BUCKAROO_PAYMENT_RETURN_PATH
              . '/' . $payment->pid, array('absolute' => TRUE)),
    );

    ksort($data);

    $data['brq_signature'] = $this->_generate_signature($data, $secret_key,
            $encryption);

    $form['#action'] = $url;
    foreach($data as $name => $value) {
      if (!empty($value)) {
        $form[$name] = array('#type' => 'hidden', '#value' => $value);
      }
    }
    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Pay with Buckaroo'),
    );

    return $form;
  }

  /**
   * @todo Write function documentation.
   */
  public function generate_signature($data, $secret, $encryption) {
    return $this->_generate_signature($data, $secret, $encryption);
  }

  /**
   * @todo Write function documentation.
   */
  private function _generate_signature($data, $secret, $encryption) {
    $hasbable = '';

    /*
     * @todo Find out why resorting is needed.
     */
    $sort = array();
    $orig = array();
    foreach ($data as $key => $value) {
      $sort[strtolower($key)] = filter_xss($value);
      $orig[strtolower($key)] = $key;
    }

    ksort($sort);

    $sorted = array();
    foreach($sort as $key => $value) {
      $key = $orig[$key];
      $sorted[$key] = $value;
    }

    $a = $sorted;
    foreach ($a as $k => $v) {
      $hasbable .= $k . '=' . $v;
    }

    return hash($encryption, $hasbable . $secret);
  }

  /**
   * @todo Write function documentation.
   */
  public function status($buckaroo_status) {
    $payment_status = PAYMENT_STATUS_PENDING;
    switch ($buckaroo_status) {
      case 100:
      case 121:
      case 190:
      case 301:
  //    case 400:
      case 401:
      case 701:
      case 801:
        $payment_status = PAYMENT_STATUS_SUCCESS;
        break;
      case 101:
      case 104:
      case 203:
      case 302:
      case 411:
      case 490:
      case 491:
      case 702:
        $payment_status = PAYMENT_STATUS_CANCELLED;
        break;
      case 402:
      case 690:
        $payment_status = PAYMENT_STATUS_CANCELLED;
        break;
      case 122:
      case 309:
      case 409:
      case 790:
      case 792:
      case 793:
      case 802:
      case 890:
      case 891:
        $payment_status = PAYMENT_STATUS_CANCELLED;
        break;
      case 001:
      case 102:
      case 103:
      case 120:
      case 123:
      case 124:
      case 125:
      case 126:
      case 135:
      case 136:
      case 138:
      case 139:
      case 140:
      case 201:
      case 303:
      case 414:
      case 492:
      case 705:
      case 711:
      case 712:
      case 720:
      case 721:
      case 800:
      case 803:
      case 804:
      case 810:
      case 811:
      case 812:
      case 813:
      case 814:
      case 815:
      case 816:
      case 900:
      case 901:
      case 931:
      case 932:
      case 933:
      case 934:
      case 935:
      case 940:
      case 941:
      case 942:
      case 943:
      case 944:
      case 945:
      case 946:
      case 947:
      case 948:
      case 949:
      case 950:
      case 951:
      case 955:
      case 956:
      case 960:
      case 961:
      case 962:
      case 992:
      case 993:
      case 999:
        $payment_status = PAYMENT_STATUS_CANCELLED;
        break;
      case 791:
        break;
    }

    return $payment_status;
  }
}
