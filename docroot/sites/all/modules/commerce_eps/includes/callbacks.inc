<?php
/**
 * @callbacks.inc
 *
 * Callback URLs used by EPS scheme operator.
 */

/**
 * Confirmation URL callback marking payment process in checkout workflow
 * and returning vitality checks (VitalityCheckDetails) or validating payment confirmations (ShopResponseDetails)
 *
 * @param auth_token
 *   hash authorizing access
 */
function commerce_eps_confirm($auth_token) {
  // load XML from post data
  $xml = commerce_eps_get_xml_from_http_post();

  $handler = new EPSXMLHandler($xml);
  if ($handler->validate() &&
     ($remit_id = $handler->getElementValue('epi:RemittanceIdentifier')) ||
      $un_remit_id = $handler->getElementValue('epi:UnstructuredRemittanceIdentifier')) {

    // fall back to unstructured remittance id if remittance not set
    if (!isset($remit_id)) {
      $remit_id = $un_remit_id;
    }
    // ignore string appended by GIROPAY
    if ($end = strpos($remit_id, 'GIROPAY') > 0) {
      $remit_id = substr($remit_id, 0, $end-1);
    }

    // retrieve order from remittance identifier
    $order = commerce_order_load($remit_id);

    // check if authentification token matches payment redirect key from URL.
    // apart from order id provided as remittance id this is an additional security measure.
    if (empty($order) || !isset($order->data['payment_redirect_key']) ||
        $order->data['payment_redirect_key'] !== $auth_token) {
      $vars = array(
        'error_msg' => t('Sorry, you do not have privileges to access this end point.'),
        'session_id' => time(),
      );
      commerce_eps_print_xml_response('commerce_eps_shop_response_details_not_validated', $vars);
    }
    else { // process incoming XML requests
      switch ($handler->document->documentElement->firstChild->localName) {
        // return the same message on a vitality check
        case 'VitalityCheckDetails':
          echo $xml;
          break;
        // validate payment confirmation
        case 'BankConfirmationDetails':
          commerce_eps_process_transaction($handler, $order);
          break;
      }
    }
  }
}

/**
 * Displays a landing page when payment was successful.
 */
function commerce_eps_success() {
  $vars =  array(
    'title' => t('Thank you'),
    'msg' => t('Your payment completed successfully. Thank you very much for your purchase!'),
  );
  return theme('commerce_eps_landing_page', $vars);
}

/**
 * Displays a page when an error occurred during payment.
 */
function commerce_eps_error() {
  switch (strtoupper($_REQUEST['epserrorcode'])) {
    case "ERROR1":
      $msg = t('The web shop was not available during transfer of the payment confirmation.') . '<br>' . t('Please try again later.');
      break;
    case "ERROR2":
      $msg = t('The XML stream or the signature is invalid. Please contact the <a href="mailto:!mail">site administrator</a>.', array('!mail' => variable_get('site_mail', '')));
      break;
    case "ERROR3":
      $msg = t('You aborted the payment process. Therefore the order could not be completed.');
      break;
    case "ERROR4":
      $cart_url = url('cart');
      $msg = l(t('Proceed to your shopping cart'), $cart_url) . '<META HTTP-EQUIV=Refresh CONTENT="0; URL=' . $cart_url . '">';
      break;
    case "ERROR5":
      $co_url = url('checkout');
      $msg = l(t('Proceed to the checkout page'), $co_url) . '<META HTTP-EQUIV=Refresh CONTENT="0; URL=' . $co_url . '">' . '<META HTTP-EQUIV=Refresh CONTENT="0; URL=' . $co_url . '">';
      break;
    default:
      $msg = t('The cause of the error is unknown. There may be a new error code (!code) which is yet unsupported. Please contact the <a href="mailto:!mail">site administrator</a>.', array('!code' => $_REQUEST['epserrorcode'], '!mail' => variable_get('site_mail', '')));
  }
  $vars = array(
    'title' => t('An error occured'),
    'msg' => $msg,
  );
  return theme('commerce_eps_landing_page', $vars);
}

/**
 * Prints a XML ShopResponseDetails response.
 *
 * @param \EPSXMLHandler $handler
 *   wrapper object for XML handling
 * @param $order
 *   commerce order
 */
function commerce_eps_process_transaction(EPSXMLHandler $handler, $order) {
  $status_code = $handler->getElementValue('eps:StatusCode'); // get transaction status code

  // If same confirmation is sent multiple times, update existing transaction
  if (!empty($trans = commerce_payment_transaction_load_multiple(NULL, array('order_id' => $order->order_id)))) {
    $transaction = current($trans);
  }
  else {
    // Create a new payment transaction for the order.
    $transaction = commerce_payment_transaction_new('commerce_eps', $order->order_id);
    $transaction->uid = $order->uid;
    $transaction->instance_id = 'commerce_eps';
  }

  $transaction->remote_id = $handler->getElementValue('eps:PaymentReferenceIdentifier');
  $transaction->amount = commerce_currency_decimal_to_amount($handler->getElementValue('epi:InstructedAmount'), $handler->getElementValue('epi:InstructedAmount', 0, 'AmountCurrencyIdentifier'));
  $transaction->currency_code = $handler->getElementValue('epi:InstructedAmount', 0, 'AmountCurrencyIdentifier');
  $transaction->payload[REQUEST_TIME] = $handler->document->saveXML();

  // Set the transaction's statuses based on the IPN's payment_status.
  $transaction->remote_status = $status_code;

  switch ($status_code) {
    case 'VOK': // treat scheduled transfers as normal ones
    case 'OK': // immediate payment
      // complete payment transaction
      $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
      $transaction->message = t('The payment has completed.');

      // tell Drupal commerce that payment was successful, finish checkout workflow and log payment details
      $log = 'BIC: ' . $handler->getElementValue('eps:ApprovingUnitBankIdentifier') . "\r\n" .
        'PayConApprovalTime: ' . $handler->getElementValue('eps:PayConApprovalTime') . "\r\n" .
        'PaymentReferenceIdentifier: ' . $handler->getElementValue('eps:PaymentReferenceIdentifier');
      commerce_payment_redirect_pane_next_page($order, $log);

      // return valid shop response
      $vars = array(
        'session_id' => $handler->getElementValue('epsp:SessionId'),
        'status_code' => $status_code,
        'pay_ref_id' => $handler->getElementValue('eps:PaymentReferenceIdentifier'),
      );
      commerce_eps_print_xml_response('commerce_eps_shop_response_details_validated', $vars);
      break;

    case 'NOK': // marking failed payment
    case 'ERROR':
      // tag transaction as unsuccessful
      $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
      $transaction->message = t('The payment has failed, please contact your bank or site administrator.');

      // tell Drupal commerce that payment failed, go back to previous checkout workflow
      commerce_payment_redirect_pane_previous_page($order);
      $vars = array(
        'session_id' => $handler->getElementValue('epsp:SessionId'),
        'error_msg' => $transaction->message,
      );
      commerce_eps_print_xml_response('commerce_eps_shop_response_details_not_validated', $vars);
      break;

    default: // Unknown error
      // tag transaction as unsuccessful
      $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
      $transaction->message = t('Unknown status code, please contact your bank or site administrator for an update.');

      // log and print response
      $vars = array(
        'session_id' => $handler->getElementValue('epsp:SessionId'),
        'error_msg' => $transaction->message,
      );
      commerce_eps_print_xml_response('commerce_eps_shop_response_details_not_validated', $vars);
  }

  // Save the transaction information.
  commerce_payment_transaction_save($transaction);
}
