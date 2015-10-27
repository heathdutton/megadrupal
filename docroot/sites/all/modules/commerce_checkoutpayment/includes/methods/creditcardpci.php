<?php

class methods_creditcardpci extends methods_Abstract {

  /**
   * Payment method callback: checkout form.
   */
  public function submitForm($payment_method, $pane_values, $checkout_pane, $order) {
    module_load_include('inc', 'commerce_payment', 'includes/commerce_payment.credit_card');

    // Prepare the fields to include on the credit card form.
    $fields = array(
      'code' => '',
    );

    // Add the credit card types array if necessary.
    if (isset($payment_method['settings']['card_types'])) {
      $card_types = array_diff(array_values($payment_method['settings']['card_types']), array(0));

      if (!empty($card_types)) {
        $fields['type'] = $card_types;
      }
    }

    return commerce_payment_credit_card_form($fields);
  }

  /**
   * Payment method callback: checkout form submission.
   */
  public function submitFormCharge($payment_method, $pane_form, $pane_values, $order, $charge) {
    $config = parent::submitFormCharge($payment_method, $pane_form, $pane_values, $order, $charge);
    $config['postedParam']['card']['number'] = $pane_values['credit_card']['number'];
    $config['postedParam']['card']['expiryMonth'] = $pane_values['credit_card']['exp_month'];
    $config['postedParam']['card']['expiryYear'] = $pane_values['credit_card']['exp_year'];
    $config['postedParam']['card']['cvv'] = $pane_values['credit_card']['code'];

    return $this->placeorder($config, $charge, $order, $payment_method);
  }

}
