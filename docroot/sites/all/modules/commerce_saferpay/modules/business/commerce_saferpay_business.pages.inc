<?php

/**
 * @file
 * Page callbacks for Saferpay Business payment gateway.
 */

/**
 * Page callback for processing the Saferpay SCD response.
 *
 * @param $order
 *   The commerce order object.
 */
function commerce_saferpay_business_scd_handle_response($order) {
  $data = simplexml_load_string($_GET['DATA']);
  if ($data['RESULT'] != 0) {
    // @todo Add message.
    drupal_set_message(t('Credit card verification failed: @error.', array('@error' => $data['DESCRIPTION'])), 'error');
    drupal_goto(commerce_checkout_order_uri($order));
  }
  // Store the CVC in the user session.
  $_SESSION['commerce_saferpay_business_cvc'] = $_GET['CVC'];

  if (!empty($_GET['cardonfile_store'])) {
    $order->data['cardonfile'] = 'new';
  }

  // Copied from from Saferpay SCD documentation.
  $card_types = array(
    21699 => 'Lastschrift',
    19265 => 'American Express',
    19269 => 'MasterCard',
    19274 => 'J.C.B.',
    19286 => 'Visa',
    99072 => 'Saferpay Testkarte'
  );

  // Store the card information in order.
  $order->data['commerce_saferpay_card_ref_id'] = (string)$data['CARDREFID'];
  $order->data['commerce_saferpay_card_holder'] = (string)$_GET['CardHolder'];
  $order->data['commerce_saferpay_card_number'] = substr((string)$data['CARDMASK'], -4);
  $order->data['commerce_saferpay_expiry_month'] = (string)$data['EXPIRYMONTH'];
  $order->data['commerce_saferpay_expiry_year'] = '20' . (string)$data['EXPIRYYEAR'];
  $order->data['commerce_saferpay_card_type'] = $card_types[(string)$data['CARDTYPE']];
  commerce_order_save($order);

  $payment_method = commerce_payment_method_instance_load($order->data['payment_method']);
  if (empty($payment_method['settings']['use_mpi'])) {
    // Authorize and optionally settle the order immediatly.
    commerce_saferpay_business_pay_order($order);
  }
  else {
    $mpi_response = commerce_saferpay_business_verify_enrollment($order, $data, $payment_method['settings']);
    if ($mpi_response !== FALSE) {
      // Redirect to 3-D secure, if necessary.
      if ((int)$mpi_response['RESULT'] == 0 && (int)$mpi_response['ECI'] == COMMERCE_SAFERPAY_ECI_3D_AUTHENTICATION) {
        header('Location: ' . $mpi_response['MPI_PA_LINK'], TRUE, 302);
        drupal_exit($mpi_response['MPI_PA_LINK']);
      }
      else {
        // Check if there is no liability shift and if such payments are allowed.
        if (((int) $mpi_response['RESULT'] != 0 || $mpi_response['ECI'] == COMMERCE_SAFERPAY_ECI_NO_LIABILITYSHIFT) && !empty($payment_method['settings']['require_liablityshift'])) {
          drupal_set_message(t('Payments from credit cards without 3-D Secure support are not accepted.'), 'error');
          drupal_goto(commerce_checkout_order_uri($order));
        }
        // Authorize and optionally settle the order immediatly.
        $order->data['commerce_saferpay_mpi_session_id'] = (string)$data['MPI_SESSIONID'];
        commerce_saferpay_business_pay_order($order);
      }
    }
    else {
      drupal_set_message(t('Payment failed: @error.', array('@error' => commerce_saferpay_business_error())), 'error');
      drupal_goto(commerce_checkout_order_uri($order));
    }
  }
}

/**
 * Page callback for processing the Saferpay MPI response.
 *
 * @param $order
 *   The commerce order object.
 */
function commerce_saferpay_business_mpi_handle_response($order) {
  $data = simplexml_load_string($_GET['DATA']);
  if ($data['RESULT'] != 0) {
    drupal_set_message(t('Payment failed: @message', array('@message' => $data['MESSAGE'])));
    drupal_goto(commerce_checkout_order_uri($order));
  }

  // To prevent double execution, check if we already have a payment, also make
  // sure we're not running into this twice.
  if (!isset($order->data['commerce_saferpay_mpi_session_id']) && lock_acquire('commerce_saferpay_' . $order->order_id)) {
    // Authorize and optionally settle the order.
    $order->data['commerce_saferpay_mpi_session_id'] = (string)$data['MPI_SESSIONID'];
    commerce_saferpay_business_pay_order($order);
    commerce_order_save($order);
    lock_release('commerce_saferpay_' . $order->order_id);
  }
  else {
    // Redirect to the current order checkout page, without error. If this
    // happened, then we already have a payment for this order and can ignore
    // the request.
    drupal_goto(commerce_checkout_order_uri($order));
  }
}
