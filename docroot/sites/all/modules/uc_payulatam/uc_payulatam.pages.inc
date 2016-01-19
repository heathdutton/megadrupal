<?php

function _uc_payulatam_response_page() {
  global $language;

  $response = array();
  foreach ($_GET as $key => $value) {
    $response[$key] = html_entity_decode($value);
  }
  unset($response['q']);

  $extra3 = unserialize($response['extra3']);
  $uid = $extra3['uid'];
  $session_id = $extra3['token_id'];
  $order = uc_order_load($extra3['order_id']);

  if (!uc_payulatam_get_txn_type()) {
    watchdog('PayU Latam', 'Response page (' . $extra3['order_id'] . '): ' . serialize($response));
  }

  if (!$order) {
    drupal_set_message(t("Order doesn't exists."), 'error');
    drupal_access_denied();
  }
  if ($session_id != session_id()) {
    drupal_set_message(t("You aren't the owner of this order."), 'error');
    drupal_access_denied();
  }
  $signature = uc_payulatam_get_signature($response['referenceCode'], $response['TX_VALUE'], $response['currency'], $response['transactionState']);
  if (strtoupper($response['signature']) != strtoupper($signature)) {
    drupal_set_message(t("Signature doesn't match."), 'error');
    drupal_access_denied();
  }

  $variables = array(
    'header' => array('', ''),
    'caption' => '',
    'attributes' => array(),
    'empty' => '',
    'colgroups' => array(),
    'sticky' => FALSE,
    );

  if($response['transactionState'] == 6 && $response['polResponseCode'] == 5) {
    $txn_status = uc_payulatam_polresponse_codes($response['polResponseCode']);
  }
  else if($response['transactionState'] == 6 && $response['polResponseCode'] == 4) {
    $txn_status = uc_payulatam_polresponse_codes($response['polResponseCode']);
  }
  else if($response['transactionState'] == 12 && $response['polResponseCode'] == 9994) {
    $txn_status = uc_payulatam_polresponse_codes($response['polResponseCode']);
  }
  else if($response['transactionState'] == 4 && $response['polResponseCode'] == 1) {
    $txn_status = uc_payulatam_polresponse_codes($response['polResponseCode']);
  }
  else {
    $txn_status = t("Unknown. Please contact PayU Latam and/or website administrator for more information.");
  }

  $order_status = uc_order_status_data($order->order_status, 'title');
  if ($order->order_status == 'in_checkout') {
    $order_status .= ". " . t("Don't worry, <strong>PayU Latam</strong> hasn't contacted us with your transaction status, but they will do; it normally take some seconds.") . ' ' . t("!reload_link to check if it is updated.", array('!reload_link' => l(t("Reload this page"), 'cart/uc_payulatam/response', array('query' => $response))));
  }

  $variables['rows'] = array();

  $variables['rows'][] = array('<strong>' . t('Amount') . '</strong>', uc_payulatam_format_price($response['TX_VALUE']) . ' (' . $response['currency'] . ')');
  $variables['rows'][] = array('<strong>' . t('Transaction Status') . '</strong>', $txn_status);
  $variables['rows'][] = array('<strong>' . t('Transaction ID') . '</strong>', check_plain($response['transactionId']));
  $variables['rows'][] = array('<strong>' . t('Sale Reference') . '</strong>', check_plain($response['reference_pol']));
  if (!empty($response['pseBank'])) {
    $variables['rows'][] = array('<strong>' . t('PSE Bank') . '</strong>', check_plain($response['pseBank']));
    $variables['rows'][] = array('<strong>' . t('Tracking Code') . '</strong>', check_plain($response['cus']));
  }
  $variables['rows'][] = array('<strong>' . t('Payment Method') . '</strong>', check_plain($response['lapPaymentMethod']));
  $variables['rows'][] = array('<strong>' . t('Order Status') . '</strong>', $order_status);
  $variables['rows'][] = array('<strong>' . t('Reference Code') . '</strong>', check_plain($response['referenceCode']));

  $build['table'] = array('#markup' => theme_table($variables));

  $build['options'] = array('#markup' => l(t("Continue Shopping"), variable_get('uc_continue_shopping_url')));

  return $build;
}

function _uc_payulatam_confirmation_page() {
  $response = array();
  foreach ($_POST as $key => $value) {
    $response[$key] = html_entity_decode($value);
  }

  $extra3 = unserialize($response['extra3']);
  $uid = $extra3['uid'];
  $order = uc_order_load($extra3['order_id']);

  if (!uc_payulatam_get_txn_type()) {
    watchdog('PayU Latam', 'Confirmation Page (' . $extra3['order_id'] . '): ' . serialize($response));
  }

  $watchdog_variables = array(
    '!order_id' => $extra3['order_id'],
    '!current_order_status' => $order->order_status,
    '!reference_pol' => check_plain($response['reference_pol']),
    );

  if ($order) {
    $signature = uc_payulatam_get_signature($response['reference_sale'], $response['value'], $response['currency'], $response['state_pol']);
    if (strtoupper($response['sign']) == strtoupper($signature)) {
      if (!in_array($order->order_status, array('in_checkout'))) {
        watchdog('PayU Latam', "Confirming order !order_id but it isn't in checkout. Its current status is !current_order_status.", $watchdog_variables, WATCHDOG_WARNING);
        uc_order_comment_save($extra3['order_id'], 0, t("Confirming order !order_id but it isn't in checkout. Its current status is !current_order_status.", $watchdog_variables));
      }
      if (in_array($order->order_status, array('payment_received', 'completed'))) {
        watchdog('PayU Latam', "Confirming order !order_id but it is already completed. Its current status is !current_order_status.", $watchdog_variables, WATCHDOG_WARNING);
        uc_order_comment_save($extra3['order_id'], 0, t("Confirming order !order_id but it is already completed. Its current status is !current_order_status.", $watchdog_variables));
      }

      uc_order_comment_save($extra3['order_id'], 0, _uc_payulatam_format_response($response));

      switch ($response['state_pol']) {

        case 4: // Payment Received
        $comment = t("Order Completed.") . ' ' . t("PayU Latam # !reference_pol", $watchdog_variables);
        uc_order_comment_save($extra3['order_id'], 0, $comment, 'order');
        uc_payment_enter($extra3['order_id'], 'payulatam', $response['value'], 0, NULL, $comment);
        uc_cart_empty($extra3['cart_id']);
        uc_cart_complete_sale($order, FALSE);
        watchdog('PayU Latam', "Order !order_id completed. PayU Latam # !reference_pol", $watchdog_variables);
        break;

        case 7: // Pending/Processing
        $comment = t("Processing Order.") . ' ' . t("PayU Latam # !reference_pol", $watchdog_variables);
        uc_order_update_status($extra3['order_id'], 'processing');
        uc_order_comment_save($extra3['order_id'], 0, $comment, 'order', 'processing');
        uc_cart_empty($extra3['cart_id']);
        watchdog('PayU Latam', "Processing order !order_id. PayU Latam # !reference_pol", $watchdog_variables);
        break;

        case 5: // Rejected
        $comment = t("Order Rejected.") . ' ' . t("PayU Latam # !reference_pol", $watchdog_variables);
        uc_order_update_status($extra3['order_id'], 'canceled');
        uc_order_comment_save($extra3['order_id'], 0, $comment, 'order', 'canceled');
        watchdog('PayU Latam', "Order !order_id rejected. PayU Latam # !reference_pol", $watchdog_variables);
        break;

        case 6: // Canceled
        $comment = t("Order Canceled.") . ' ' . t("PayU Latam # !reference_pol", $watchdog_variables);
        uc_order_update_status($extra3['order_id'], 'canceled');
        uc_order_comment_save($extra3['order_id'], 0, $comment, 'order', 'canceled');
        watchdog('PayU Latam', "Order !order_id canceled. PayU Latam # !reference_pol", $watchdog_variables);
        break;

        default:
        $comment = t("Order Error.") . ' ' . t("PayU Latam # !reference_pol", $watchdog_variables);
        uc_order_comment_save($extra3['order_id'], 0, $comment, 'order', $order->order_status);
        watchdog('PayU Latam', "Error with order !order_id. PayU Latam # !reference_pol", $watchdog_variables);

      }
    }
    else {
      watchdog('PayU Latam', "Confirming order !order_id but signature doesn't match.", $watchdog_variables, WATCHDOG_ERROR);
    }
  }
  else {
    watchdog('PayU Latam', "Confirming order !order_id but it doesn't exists.", $watchdog_variables, WATCHDOG_ERROR);
  }
}

function _uc_payulatam_format_response($response) {
  $output = t('PayU Latam response:');
  $output .= '<ul>';
  $output .= '<li>' . t('Sale Reference (!key): !value', array('!key' => 'reference_sale', '!value' => $response['reference_sale'])) . '</li>';
  $output .= '<li>' . t('Transaction status (!key): !value !info', array('!key' => 'state_pol', '!value' => $response['state_pol'], '!info' => uc_payulatam_txn_states($response['state_pol']))) . '</li>';
  $output .= '<li>' . t('Response code (!key): !value !info', array('!key' => 'response_code_pol', '!value' => $response['response_code_pol'], '!info' => uc_payulatam_polresponse_codes($response['response_code_pol']))) . '</li>';
  $output .= '</ul>';

  return $output;
}
