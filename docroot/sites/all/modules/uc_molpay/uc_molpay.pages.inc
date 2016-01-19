<?php

/**
 * @file
 * molpay menu items.
 */

/**
 * Finalizes molpay transaction.
 */
function uc_molpay_complete($cart_id = 0) {
  watchdog('Molpay', 'Receiving order payment notification for order !order_id.', array('!order_id' => check_plain($_REQUEST['orderid'])));

  $order = uc_order_load($_REQUEST['orderid']);

  if ($order === FALSE || uc_order_status_data($order->order_status, 'state') != 'in_checkout') {
    return t('An error has occurred during payment.  Please contact us to ensure your order has submitted.');
  }

  $vkey = variable_get('uc_molpay_secret_word');

  $order->amount = str_replace('.', '', str_replace(',', '', $_REQUEST['amount']));
  $order->orderid = $_REQUEST['orderid'];
  $order->appcode = $_REQUEST['appcode'];
  $order->tranID = $_REQUEST['tranID'];
  $order->domain = $_REQUEST['domain'];
  $order->currency = $_REQUEST['currency'];
  $order->status = $_REQUEST['status'];
  $order->paydate = $_REQUEST['paydate'];
  $order->channel = $_REQUEST['channel'];
  $order->skey = $_REQUEST['skey'];

	$key0 = md5( $order->tranID.$order->orderid.$order->status.$order->domain.$order->amount.$order->currency );
    $key1 = md5( $order->paydate.$order->domain.$key0.$order->appcode.$vkey );
    

  // Save changes to order without it's completion.
  uc_order_save($order);


    if ($_REQUEST['status'] == '00' && is_numeric($_REQUEST['amount'])) {
    $comment = t('Paid by !type, MolPay order #!order.', array('!type' => $_REQUEST['channel'] == 'Credit' ? t('credit card') : t('MolPay'), '!order' => check_plain($_REQUEST['orderid'])));
    uc_payment_enter($order->orderid, 'uc_molpay',$_REQUEST['amount'], 0, NULL, $comment);
  	
	/*
	}
  	else {
    drupal_set_message(t('Your order will be processed as soon as your payment clears at molpay.com.'));
    uc_order_comment_save($order->orderid, 0, t('!type payment is pending approval at molpay.com.', array('!type' => $_REQUEST['channel'] == 'CC' ? t('Credit card') : t('eCheck'))), 'admin');
  }
  */
  
    // Empty that cart...
  uc_cart_empty($cart_id);

  // Add a comment to let sales team know this came in through the site.
  uc_order_comment_save($order->order_id, 0, t('Order created through website.'), 'admin');

  $build = uc_cart_complete_sale($order, variable_get('uc_new_customer_login', FALSE));

  $page = variable_get('uc_cart_checkout_complete_page', '');

  if (!empty($page)) {
    drupal_goto($page);
  }

	}
	// payment failed, redirect to cart
	else {
    drupal_set_message(t('Your payment was not successful. Please retry your order'),'error');
    uc_order_comment_save($order->orderid, 0, t('!type payment failed or pending approval at Molpay', array('!type' => $_REQUEST['channel'] == 'CC' ? t('Credit card') : t('eCheck'))), 'admin');
	drupal_goto('cart');
  }


  return $build;
}
