<?php

/**
 * @file
 * YellowPay Menu Callbacks
 * Compatible with ShopBuilder.me eCommerce platform
 * Author Eweev S.A.R.L (www.eweev.com)
 * Sponsor YellowPay (www.yellowpay.co)
 */

/**
 * Process callback triggered by an IPN call from YellowPay server.
 */
function commerce_yellowpay_response_process() {
  $settings = commerce_yellowpay_get_settings();

  $response = file_get_contents("php://input");
  $data = json_decode($response);

  if (!empty($settings['watchdog'])) {
    // Log full transaction data.
    watchdog('commerce_yellowpay', 'YellowPay IPN with response @response', array('@response' => print_r($data, TRUE)));
  }

  $invoice_id = $data->id;
  $status = $data->status;

  // We load the transaction we have in memory to edit its status.
  $transactions = commerce_payment_transaction_load_multiple(array(), array('remote_id' => $invoice_id));
  $transaction = array_shift($transactions);
  if ($transaction->transaction_id != 0) {
    $transaction->remote_status = $status;
    $transaction->payload[REQUEST_TIME] = $data;

    switch ($status) {
      case 'authorizing':
        $order = commerce_order_load($transaction->order_id);
        commerce_payment_redirect_pane_next_page($order);
        break;

      case 'paid':
        $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
        break;

      case 'expired':
        $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
        break;

      default:
        break;
    }
    commerce_payment_transaction_save($transaction);
  }
}

/**
 * Helper function for the integration with ShopBuilder's interface
 */
function commerce_yellowpay_callbackconfig($arg = FALSE) {
  $base_path = "admin/config";
  module_load_include("inc", "rules", "ui/ui.forms");
  $config_name = "commerce_payment_yellowpay";
  $rules_config = rules_config_load($config_name);
  if ($rules_config) {
    $element_id = commerce_yellowpay_get_element_Id_action($rules_config);
    $element = rules_element_load($element_id, $config_name);
    $form = drupal_get_form('rules_ui_edit_element', $rules_config, $element, $base_path);
    unset($form['parameter']['commerce_order']);

    return $form;
  }
  else {
    return t("YellowPay Bitcoin Payments is not installed on your site");
  }
}

/**
 * Helper function to check the status and data of a specific Invoice.
 * Development use only.
 * i.e use _yellowpay_invoice_call("49f8eb4782e937c1ef85272898470b40");
 *
 * @param string $invoice_id
 */
function _commerce_yellowpay_invoice_call($invoice_id) {

  $settings = commerce_yellowpay_get_settings();
  $secret = $settings['apisecret'];
  $key = $settings['apikey'];

  $url = url(COMMERCE_YELLOWPAY_API_URL . 'invoice/' . $invoice_id . '/');
  $nonce = round(microtime(TRUE) * 1000);

  $headers = array(
    "Content-type" => 'application/json',
    "API-Key" => $key,
    "API-Nonce" => $nonce,
    "API-Sign" => hash_hmac("sha256", $nonce . $url, $secret, FALSE),
  );

  $options = array(
    'headers' => $headers,
    'method' => 'GET',
    'timeout' => 15,
  );

  $response = drupal_http_request($url, $options);

  // If we get a valid response with data.
  if ($response->code = 200) {
    $response_data = json_decode($response->data);
    watchdog('commerce_yellowpay', 'YellowPay Invoice Details @invoice', array('@invoice' => print_r($response_data, TRUE)));
    // kpr(array($url, $response, $response_data));
  }
}
