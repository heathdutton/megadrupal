<?php

/**
 * @file
 * paybox menu items.
 *
 */

 /**
 * Customer return after paybox validation
 */
function uc_paybox_effectue() {

  global $user;
  $orderid = $_GET['orderid'];
  $erreur = $_GET['erreur'];

  if (!isset($_GET['orderid'])) {
    drupal_set_message(t('You are not allowed to see this page, please, return to homepage.'));
    drupal_goto('cart');
  }
  // debug
  if (variable_get('uc_paybox_debug', FALSE) == TRUE) {
    foreach ($_GET as $k => $v) {
      drupal_set_message($k . ': ' . $v);
    }
  }
  if (_uc_paybox_verif_sign()) {
    if ($erreur == "00000") {
      $order = uc_order_load($orderid);
      // process order (without payment, status set to Pending -> only auto can validate)
      $output = uc_cart_complete_sale($order, variable_get('uc_new_customer_login', FALSE));
      // add comment and log
      uc_order_comment_save($orderid, $user->uid, t('Customer get back on successful payment, waiting auto notification to validate payment.'), 'order');
      watchdog('uc_paybox', 'Payment back on order !orderid, waiting auto notification.', array('!orderid' => $orderid));
      // check if alternate checkout completion page is set
      $page = variable_get('uc_cart_checkout_complete_page', '');
      if (!empty($page)) {
        drupal_goto($page);
      }
      return $output;
    }
    else {
      $error = _uc_paybox_erreur($erreur);
      drupal_set_message(t('An error has occured during payment, please contact us.') . t('Error: @error', array('@error' => $error)), 'error');
      // add comment and log
      uc_order_comment_save($orderid, $user->uid, t('Error occured during payment: @error', array('@error' => $error)), 'order');
      watchdog('uc_paybox', 'Error on complete : @error', array('@error' => $error), WATCHDOG_WARNING);
      drupal_goto('cart');
    }
  }
  else {
    drupal_set_message(t('An error has occured during payment, please contact us.'), 'error');
    watchdog('uc_paybox', 'Sign error: order @orderid', array('@orderid' => $orderid), WATCHDOG_WARNING);
    drupal_goto('cart');
  }
}

 /**
 * Customer return after paybox refuse payment
 */
function uc_paybox_refuse() {
  drupal_set_message(t('Your purchase has been refused. Please try again or contact us.'));
  drupal_goto('cart');
}

 /**
 * Customer return after paybox cancel payment
 */
function uc_paybox_annule() {
  global $user;
  $orderid = $_GET['orderid'];
  $error = FALSE;

  $order = uc_order_load($orderid);
  if ($order) {
    unset($_SESSION['cart_order']);
    // add comment
    uc_order_comment_save($orderid, $user->uid, t('This order has been explicitly canceled by the user.'), 'order');
    // check status
    if (uc_order_update_status($orderid, uc_order_state_default('canceled'))) {
      drupal_set_message(t('Your order has been canceled.'), 'status');
    }
    else {
      $error = TRUE;
      watchdog('uc_paybox', "Order @order couldn't be marked as 'Canceled'.", array('@order' => $orderid), WATCHDOG_WARNING, NULL);
    }
  }
  else {
    $error = TRUE;
    watchdog('uc_paybox', 'Order cancel error: order @orderid', array('@orderid' => $orderid), WATCHDOG_WARNING);
  }
  if ($error) {
    drupal_set_message(t('An error has occured during payment, please contact us.'), 'error');
  }
  drupal_goto('cart');
}

 /**
 * auto notification from paybox
 */
function uc_paybox_auto() {

  $orderid = $_GET['orderid'];
  $montant = $_GET['montant'];
  $uniqueid = $_GET['uniqueid'];
  $erreur = $_GET['erreur'];
  $sign = $_GET['sign'];
  $ip_paybox = ip_address();

  // check ip
  if (!_uc_paybox_ip_check($ip_paybox)) {
    $errors[] = t('Invalid IP @ip.', array('@ip' => $ip_paybox));
  }
  // check sign
  if (!_uc_paybox_verif_sign()) {
    $errors[] = t('Invalid sign.');
  }

  // check order
  $order = uc_order_load($orderid);
  if (!$order) {
    $errors[] = t('Invalid order @id.', array('@id' => $order->order_id));
  }
  else {
    // Check status
    if ($order->order_status != 'in_checkout' && $order->order_status != 'pending') {
      $errors[] = t('Order not in checkout or pending:') . ' ' . $order->order_status;
    }
    if ($order->order_status == 'payment_received') {
      $errors[] = t('Try to revalide a payment.');
    }
    // Check price
    //$total = number_format($order->order_total * 100, 2, variable_get('uc_currency_dec', '.'), '');
    $total = uc_currency_format($order->order_total, FALSE, FALSE, '.');
    if ($total != $montant/100) {
      $errors[] = t('Incorrect price: @price1 / @price2.', array('@price1' => $total, '@price2' => $montant));
    }
    // Check error code
    if ($erreur != "00000") {
      $errors[] = _uc_paybox_erreur($erreur);
    }
  }
  if (!empty($errors)) {
    // Log errors to order and watchdog.
    $error_data = array('@id' => $orderid, '@error' => implode(" - ", $errors));
    uc_order_comment_save($order->order_id, 0, t('Autoresponse error on order @id: @error', $error_data), 'admin');
    watchdog('uc_paybox', 'Order @id caused error: @error', $error_data, WATCHDOG_ERROR);
    // debug
    if (variable_get('uc_paybox_debug', FALSE) == TRUE) {
      $output = t('Order @id caused error: @error', $error_data);
      print $output;
    }
    exit();
  }

  // if everything ok, process
  // Sale complete
  uc_order_update_status($order->order_id, 'payment_received');
  $comment = t('Paid by paybox reference @uniqueid.', array('@uniqueid' => $uniqueid));
  uc_payment_enter($order->order_id, 'paybox', $montant / 100, 0, NULL, $comment);
  //uc_order_comment_save($order->order_id, 0, t('Order created through website. Paybox payment transaction : @uniqueid', array('@uniqueid' => $uniqueid)), 'admin');
  uc_order_comment_save($order->order_id, 0, t('Payment accepted. Paybox transaction id: @uniqueid', array('@uniqueid' => $uniqueid)), 'order');
  // debug
  if (variable_get('uc_paybox_debug', FALSE) == TRUE) {
    print $comment;
  }
  exit();
}

/**
 * Define paybox error status, see paybox doc
 */
function _uc_paybox_erreur($erreur) {
  switch ($erreur) {
    case "00001":
      $output['infos'] = t('Connexion to autorise center failed.');
      break;
    case substr($erreur, 0, 3) == "001":
      $output['infos'] = t('Payment refused by autorisation center.');
      break;
    case "00003":
      $output['infos'] = t('Paybox error.');
      break;
    case "00004":
      $output['infos'] = t('Owner number or cryptogram invalid.');
      break;
    case "00006":
      $output['infos'] = t('Access refused or rank/site/is incorrect.');
      break;
    case "00008":
      $output['infos'] = t('Error on expiry date');
      break;
    case "00009":
      $output['infos'] = t('Error creating subscription.');
      break;
    case "00010":
      $output['infos'] = t('Unknow currency.');
      break;
    case "00011":
      $output['infos'] = t('Wrong order total.');
      break;
    case "00015":
      $output['infos'] = t('Payment already validate.');
      break;
    case "00016":
      $output['infos'] = t('User exist.');
      break;
    case "00021":
      $output['infos'] = t('Unauthorize card.');
      break;
    case "00029":
      $output['infos'] = t('Card non-compliant.');
      break;
    case "00030":
      $output['infos'] = t('Timeout on checkout page (> 15 mn).');
      break;
    case "00031":
      $output['infos'] = t('Reserved.');
      break;
    default:
      $output['infos'] = t('Unknown error');
  }
  return t('ERROR @error:', array('@error' => $erreur)) . ' ' . $output['infos'];
}

/**
 * Check sign on paybox back url
 */
function _uc_paybox_verif_sign() {
  $q = preg_replace("/q=[^&]+&/", "", $_SERVER['QUERY_STRING']);
  $pos = strrpos($q, '&');
  $data = substr($q, 0, $pos);
  $pos = strpos($q, '=', $pos) + 1;
  $sig = substr($q, $pos);
  $sig = base64_decode(urldecode($sig));
  $key = openssl_pkey_get_public(file_get_contents(drupal_get_path('module', 'uc_paybox') . '/pubkey.pem'));
  return openssl_verify($data, $sig, $key);
}
