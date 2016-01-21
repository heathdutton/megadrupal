<?php

namespace Drupal\moip\Commerce\FormControllers;

class MoipCommerceCheckoutCompleteForm {

  public static function alter(&$form, &$form_state, $order) {

    $order_id = arg(1);
    $order = commerce_order_load($order_id);

    if (isset($order->data['payment_method']) && $order->data['payment_method'] == 'moip_ct|commerce_payment_moip_ct') {
      switch ($order->data['moip_ct_payment_method']) {
        case 'bankbillet': $message = t("If the bank billet didn't opened, <a href='!link' target='_blank'>click here</a> to open it and complete your payment.", array('!link' => $order->data['moip_payment_url']));
          break;
        case 'banktransfer': $message = t("If the internet banking didn't opened, <a href='!link' target='_blank'>click here</a> to open it and complete your payment.", array('!link' => $order->data['moip_payment_url']));
          break;
      }
      if (isset($message)) {
        $form['moip_info'] = array(
          '#markup' => '<div class="moip-info">' . $message . '</div>'
        );
      }
    }
  }

}
