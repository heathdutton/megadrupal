<?php

//=============================================================================
//= MENU CALLBACKS
//=============================================================================

/**
 * Successful payment callback.
 */
function uc_gopay_payment_success() {

  _uc_gopay_check_response_access();

  if (uc_gopay_setting('debug')) {
    watchdog('uc_gopay', 'Access to payment success page. $_REQUEST = %request', array('%request' => print_r($_REQUEST, TRUE)), WATCHDOG_DEBUG);
  }

  try {
    $lock_name =  'uc_gopay_' .  $_GET['paymentSessionId'];

    if (!lock_acquire($lock_name, 5)) {
      lock_wait($lock_name);
      lock_acquire($lock_name, 5);
    }

    _uc_gopay_complete();

    lock_release($lock_name);

    // This lets us know it's a legitimate access of the complete page.
    $_SESSION['do_complete'] = TRUE;
    drupal_goto('cart/checkout/complete');
  }
  catch (GopayPaymentException $e) {
    drupal_set_message(t('Error during processing the payment: %error', array('%error' => $e->getMessage())), 'error');
    drupal_goto('cart/checkout/review');
  }
}

/**
 * Failed payment callback.
 */
function uc_gopay_payment_failed() {
  // Available in $_GET: paymentSessionId, variableSymbol, encryptedSignature, eshopGoId
  if (empty($_GET['eshopGoId'])) {
    drupal_access_denied();
    exit;
  }
  if (uc_gopay_setting('debug')) {
    watchdog('uc_gopay', 'Access to payment failed page. $_REQUEST = %request', array('%request' => print_r($_REQUEST, TRUE)), WATCHDOG_DEBUG);
  }
  unset($_SESSION['cart_order']);
  drupal_set_message(t('Your GoPay payment was canceled. Please feel free to continue shopping or contact us for assistance.'));
  drupal_goto('cart');
}

/**
 * Notify callback.
 */
function uc_gopay_soap_notify() {

  _uc_gopay_check_response_access();

  if (uc_gopay_setting('debug')) {
    watchdog('uc_gopay', 'Access to payment notify page. $_REQUEST: <pre>@request</pre>', array('@request' => print_r($_REQUEST, TRUE)), WATCHDOG_DEBUG);
  }

  try {
    $lock_name =  'uc_gopay_' .  $_GET['paymentSessionId'];

    if (!lock_acquire($lock_name, 5)) {
      lock_wait($lock_name);
      lock_acquire($lock_name, 5);
    }

    _uc_gopay_complete();

    lock_release($lock_name);
  }
  catch (GopayPaymentException $e) {
    _uc_gopay_server_error();
  }
}

/**
 * Admin form of the GoPay methods.
 */
function uc_gopay_payment_methods_list() {

  $form = $options = array();

  $form['uc_gopay_back_top'] = array(
    '#prefix' => '<p>',
    '#markup' => l(t('Back to GoPay settings'), 'admin/store/settings/payment/method/gopay_wps/'),
    '#suffix' => '</p>',
  );

  $header = array(t('Logo'), t('Payment method'), t('Code'), t('Offline'));
  $payment_method_list = uc_gopay_channel_list_all();
  foreach ($payment_method_list as &$payment_method) {
    $row = array();
    $row[] = '<img src="' . $payment_method->logo . '">';
    $row[] = $payment_method->paymentMethod;
    $row[] = $payment_method->code;
    $row[] = ($payment_method->offline == 1) ? t('yes') : t('no');
    $options[$payment_method->code] = $row;
  }

  $form['uc_gopay_enabled_channels'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' =>  t('Payment channels available'),
    '#default_value' => uc_gopay_setting('channels'),
  );

  $form['uc_gopay_back_bottom'] = array(
    '#prefix' => '<p>',
    '#markup' => l(t('Back to GoPay settings'), 'admin/store/settings/payment/method/gopay_wps/'),
    '#suffix' => '</p>',
  );

  return system_settings_form($form);
}


/**
 * Optional registration form.
 */
function uc_gopay_registry_form($form, &$form_state) {

  $form['uc_gopay_back_top'] = array(
    '#prefix' => '<p>',
    '#markup' => l(t('Back to GoPay settings'), 'admin/store/settings/payment/method/gopay_wps/'),
    '#suffix' => '</p>',
  );

  $registered = uc_gopay_setting('registration');
  $defaults = uc_gopay_setting('registration_data');
  if (isset($form_state['values'])) {
    $defaults = $form_state['values'] + $defaults;
  }

  if (!$registered) {
    $form['appeal_title'] = array(
      '#type' => 'markup',
      '#markup' => t("Please, support the development and register this module!"),
      '#prefix' => '<h3>',
      '#suffix' => '</h3>',
    );
    $form['appeal_body'] = array(
      '#type' => 'markup',
      '#markup' => t(<<<TXT
This module isn't officially supported by the GoPay s.r.o., but GoPay will pay
us for every site which is using this module. Unfortunately GoPay provider has
no tool to detect what plugin each e-shop is using and the whole evidence of
these sites is upon to us. The registration is optional and its done by an
email. You can use the form below or you can send an email to
<a href="mailto:@email">@email</a> with the following data: e-shop owner name,
owner e-mail address, e-shop URL and GoPay ID (if you've already obtained it).
TXT
, array('@email' => UC_GOPAY_REGISTRATION_EMAIL)),
      '#prefix' => '<p>',
      '#suffix' => '</p>',
    );
  }
  else {
    $form['thankyou'] = array(
      '#type' => 'markup',
      '#markup' => t("Thank you for your support!"),
      '#prefix' => '<h3>',
      '#suffix' => '</h3>',
    );
    $form['last_change'] = array(
      '#type' => 'markup',
      '#markup' => t("The registration was sent @date", array('@date' => format_date($registered))),
      '#prefix' => '<p>',
      '#suffix' => '</p>',
    );
  }
  $form['disclaimer_title'] = array(
    '#type' => 'markup',
    '#markup' => t("Disclaimer"),
    '#prefix' => '<h3>',
    '#suffix' => '</h3>',
  );
  $form['disclaimer_body'] = array(
    '#type' => 'markup',
    '#markup' => t("We guarantee you that the following information will be used
 only for evidence of the sites which are using this module and will be provided to GoPay s.r.o.
 This information will not be shared to any other subject."),
    '#prefix' => '<p>',
    '#suffix' => '</p>',
  );
  $form['first_name'] = array(
    '#type' => 'textfield',
    '#title' => t('First name'),
    '#required' => TRUE,
    '#default_value' => $defaults['first_name'],
    '#description' => t('First name of the e-shop owner.') . ' ' .
                      t('Should be same as the one used in GoPay official registration process.'),
  );
  $form['second_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Second name'),
    '#required' => TRUE,
    '#default_value' => $defaults['second_name'],
    '#description' => t('Second name of the e-shop owner.') . ' ' .
                      t('Should be same as the one used in GoPay official registration process.'),
  );
  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => t('E-mail'),
    '#required' => TRUE,
    '#default_value' => $defaults['email'] ? $defaults['email'] : variable_get('site_mail', ''),
    '#description' => t('E-mail of the e-shop owner.') . ' ' .
                      t('Should be same as the one used in GoPay official registration process.'),
  );
  $form['eshop_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Eshop URL'),
    '#required' => TRUE,
    '#default_value' => $defaults['eshop_url'] ? $defaults['eshop_url'] : url('<front>', array('absolute' => TRUE)),
    '#description' => t('URL of your e-shop including http:// or https://.'),
  );
  $form['goid'] = array(
    '#type' => 'textfield',
    '#title' => t('GoPay ID'),
    '#required' => FALSE,
    '#default_value' => uc_gopay_setting('goid'),
    '#description' => t('Your GoPay ID, if you have pass the official registration.'),
  );
  $form['send_copy'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send copy'),
    '#description' => t('Send copy to the provided e-mail address. (Includes the registrar email in the blind carbon copy.)'),
    '#default_value' => '',
  );
  $form['warning'] = array(
    '#type' => 'markup',
    '#markup' => t("Please note, that this is not official e-shop registration, you still need to contact the official GoPay provider to use the GoPay payment gateway."),
    '#prefix' => '<p><strong>',
    '#suffix' => '</strong></p>',
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['preview'] = array(
    '#type' => 'submit',
    '#value' => t('Preview'),
  );
  $form['actions']['send'] = array(
    '#type' => 'submit',
    '#value' => t('Send'),
  );

  $form['actions']['uc_gopay_back_bottom'] = array(
    '#prefix' => '<p>',
    '#markup' => l(t('Back to GoPay settings'), 'admin/store/settings/payment/method/gopay_wps/'),
    '#suffix' => '</p>',
  );

  return $form;
}

/**
 * An form validation handler.
 *
 * @see uc_gopay_registry_form()
 */
function uc_gopay_registry_form_validate(&$form, &$form_state) {
  $values =& $form_state['values'];

  // Validate the e-mail address.
  $error = user_validate_mail($values['email']);
  if ($error) {
    form_set_error('email', $error);
  }
  // Validate eshop URL. See http://mathiasbynens.be/demo/url-regex.
  if (!empty($values['eshop_url']) && !preg_match('_^(?:(?:https?|ftp)://)(?:\S+(?::\S*)?@)?(?:(?!10(?:\.\d{1,3}){3})(?!127(?:\.\d{1,3}){3})(?!169\.254(?:\.\d{1,3}){2})(?!192\.168(?:\.\d{1,3}){2})(?!172\.(?:1[6-9]|2\d|3[0-1])(?:\.\d{1,3}){2})(?:[1-9]\d?|1\d\d|2[01]\d|22[0-3])(?:\.(?:1?\d{1,2}|2[0-4]\d|25[0-5])){2}(?:\.(?:[1-9]\d?|1\d\d|2[0-4]\d|25[0-4]))|(?:(?:[a-z\x{00a1}-\x{ffff}0-9]+-?)*[a-z\x{00a1}-\x{ffff}0-9]+)(?:\.(?:[a-z\x{00a1}-\x{ffff}0-9]+-?)*[a-z\x{00a1}-\x{ffff}0-9]+)*(?:\.(?:[a-z\x{00a1}-\x{ffff}]{2,})))(?::\d{2,5})?(?:/[^\s]*)?$_iuS', $values['eshop_url'])) {
    form_set_error('eshop_url', t('The e-shop URL %url is not valid.', array('%url' => $values['eshop_url'])));
  }
}

/**
 * An form submit handler.
 *
 * @see uc_gopay_registry_form()
 */
function uc_gopay_registry_form_submit(&$form, &$form_state) {
  $values =& $form_state['values'];

  $to = UC_GOPAY_REGISTRATION_EMAIL;
  $from = variable_get('site_mail', '');
  $language = language_default();
  $params = $values;
  $params['registered'] = REQUEST_TIME;
  $send = $form['actions']['send']['#value'] === $values['op'];
  $preview = $form['actions']['preview']['#value'] === $values['op'];

  $message = drupal_mail('uc_gopay','register', $to, $language, $params, $from, $send);

  if ($send && $message['result']) {
    drupal_set_message('Thank you for the registration! We really appreciate it!');
    variable_set('uc_gopay_registration', REQUEST_TIME);
    // Store registration data
    $data = array(
      'first_name' => $values['first_name'],
      'second_name' => $values['second_name'],
      'eshop_url' => $values['eshop_url'],
      'email' => $values['email'],
      'goid' => $values['goid'],
      'last_update' => REQUEST_TIME,
    );
    variable_set('uc_gopay_registration_data', $data);
  }
  elseif ($preview) {
    drupal_set_message(t('<b>Message preview</b><br><pre><b>From:</b> @from<br><b>To:</b> @to<br><b>Subject:</b> @subject<br><b>Body:</b><br>@body</pre>', array('@from' => $message['from'], '@to' => $message['to'], '@subject' => $message['subject'], '@body' => $message['body'])));
    $form_state['rebuild'] = TRUE;
  }
  else {
    $form_state['rebuild'] = TRUE;
  }
}

//=============================================================================
//= HELPERS
//=============================================================================

class GopayPaymentException extends Exception {}

/**
 * Returns 200 OK HTTP header.
 */
function _uc_gopay_server_success() {
  if (uc_gopay_setting('debug')) {
    watchdog('uc_gopay', 'Server SUCCESS.');
  }
  header('HTTP/1.1 200 OK');
  exit(0);
}

/**
 * Returns 500 Server error HTTP header.
 */
function _uc_gopay_server_error() {
  if (uc_gopay_setting('debug')) {
    watchdog('uc_gopay', 'Server ERROR.');
  }
  header('HTTP/1.1 500 Internal Server Error');
  exit(0);
}

/**
 * Splits variable symbol which came back from GoPay to order_id and cart_id.
 *
 * @param string $variable_symbol
 * @return array of order_id [0] and cart_id [1]
 */
function _uc_gopay_parse_variable_symbol($variable_symbol) {
  // Parse Order ID and Cart ID
  @list($order_id, $cart_id) = explode('-', $variable_symbol);
  // Sanitize order ID and cart ID
  $order_id = intval($order_id);
  $cart_id  = $cart_id ? check_plain($cart_id) : 0;
  return array($order_id, $cart_id);
}

/**
 * Checks if paymentSessionId, variableSymbol, encryptedSignature, eshopGoId are
 * available in $_GET.
 */
function _uc_gopay_check_response_access() {
  // Available in $_GET: paymentSessionId, variableSymbol, encryptedSignature, eshopGoId
  if (empty($_GET['eshopGoId']) || empty($_GET['paymentSessionId']) || empty($_GET['variableSymbol']) || empty($_GET['encryptedSignature'])) {
    drupal_access_denied();
    exit(1);
  }
}

/**
 * Process the notification returned from the GoPay module to finish the payment
 * process.
 *
 * @throws GopayPaymentException
 */
function _uc_gopay_complete() {
  // Assing the values from the GoPay request
  $returned_variable_symbol = $_GET['variableSymbol'];
  $returned_payment_session_id = $_GET['paymentSessionId'];
  $returned_goid = $_GET['eshopGoId'];
  $returned_encrypted_signature = $_GET['encryptedSignature'];

  // Parse Order ID and Cart ID
  list($order_id, $cart_id) =  _uc_gopay_parse_variable_symbol($returned_variable_symbol);

  if (!empty($cart_id)) {
    // Needed later by uc_complete_sale to empty the correct cart
    $_SESSION['uc_cart_id'] = $cart_id;
  }

  watchdog('uc_gopay', 'Receiving PN at URL for order @order_id. <pre>@debug</pre>', array('@order_id' => $order_id, '@debug' => uc_gopay_setting('debug') ? print_r($_GET, TRUE) : ''), WATCHDOG_NOTICE, l(t('view'), 'admin/store/orders/' . $order_id));

  $payment = uc_gopay_record_find_by_session_id($returned_payment_session_id);
  if (uc_gopay_setting('debug')) {
    watchdog('uc_gopay', 'Payment session record. <pre>@debug</pre>', array('@debug' => print_r($payment, TRUE)), WATCHDOG_DEBUG, l(t('view'), 'admin/store/orders/' . $order_id));
  }

  // TODO: load using PaymentSessionId?
  $order = uc_order_load($order_id);
  if ($order == FALSE) {
    watchdog('uc_gopay', 'GoPay response attempted for non-existent order @order_id.', array('@order_id' => $order_id), WATCHDOG_ERROR, l(t('view'), 'admin/store/orders/' . $order_id));
    throw new GopayPaymentException('GoPay transaction failed verification');
  }
  elseif ($order->payment_method != UC_GOPAY_STANDARD_PAYMENT_METHOD) {
    watchdog('uc_gopay', 'Attempt to pay order @order_id using GoPay, but pament method %payment_method was selected.', array('@order_id' => $order_id, '%payment_method' => $order->payment_method), WATCHDOG_ERROR, l(t('view'), 'admin/store/orders/' . $order_id));
    throw new GopayPaymentException("Missing GoPay payment method in the order");
  }
  elseif ($order->currency != UC_GOPAY_CURRENCY) {
    watchdog('uc_gopay', 'GoPay transaction failed verification.', array(), WATCHDOG_ERROR, l(t('view'), 'admin/store/orders/' . $order_id));
    throw new GopayPaymentException('Order has wrong currency');
  }

  $eshop_goid = uc_gopay_setting('goid');
  $eshop_secret = uc_gopay_setting('secret');
  $payment_currency = 'CZK';

  // Make sure that the notification was send by the GoPay and signed.
  if (!GopayHelper::checkPaymentIdentity((float) $returned_goid, (float) $returned_payment_session_id, $returned_variable_symbol, $returned_encrypted_signature, (float) $eshop_goid, $payment['variable_symbol'], $eshop_secret)) {
    // Not valid notification verification - probably malicius attempt to set order as paid.
    watchdog('uc_gopay', 'GoPay transaction failed verification.', array(), WATCHDOG_ERROR, l(t('view'), 'admin/store/orders/' . $order_id));
    uc_order_comment_save($order_id, 0, t('An GoPay transaction failed verification for this order.'), 'admin');
    throw new GopayPaymentException('GoPay transaction failed verification.');
  }

  watchdog('uc_gopay', 'GoPay: transaction @id for @order_id verified.', array('@id' => $returned_payment_session_id, '@order_id' => $order_id), WATCHDOG_NOTICE,l(t('view'), 'admin/store/orders/' . $order_id));

  // Check if the order is payed
  $result = GopaySoap::isEshopPaymentDone((float) $returned_payment_session_id, (float) $eshop_goid, $payment['variable_symbol'], $payment['total_price'], $payment['product_name'], $eshop_secret);
  if (empty($result['code'])) {
    throw new GopayPaymentException('Empty or invalid GoPay payment result.');
  }
  if (uc_gopay_setting('debug')) {
    watchdog('uc_gopay', 'GoPay: transaction @id for @order_id. Saved session: <pre>@session</pre> result: <pre>@result</pre>', array('@id' => $returned_payment_session_id, '@order_id' => $order_id, '@session' =>  print_r($payment, TRUE), '@result' =>  print_r($result, TRUE)), WATCHDOG_DEBUG, l(t('view'), 'admin/store/orders/' . $order_id));
  }

  $original_state = $payment['session_state'];

  switch ($result['code']) {
    case GopayHelper::WAITING:
      if ($payment['session_state'] != GopayHelper::WAITING) {
        $payment['session_state'] = $result['code'];
        uc_order_update_status($order_id, 'gopay_pending');
        uc_order_comment_save($order_id, 0, t('Payment is pending at GoPay.'), 'admin');
      }
      break;

    case GopayHelper::PAYMENT_DONE:
      if (empty($payment['session_state']) || $payment['session_state'] == GopayHelper::WAITING) {
        $payment['session_state'] = $result['code'];
        $amount = $payment['total_price'] / 100.00;
        $comment = t('GoPay payment session ID: @sess_id', array('@sess_id' => $returned_payment_session_id));
        uc_payment_enter($order_id, UC_GOPAY_STANDARD_PAYMENT_METHOD, $amount, $order->uid, $payment, $comment);
        uc_cart_complete_sale($order);
        uc_order_comment_save($order_id, 0, t('Payment of @amount @currency submitted through GoPay.', array('@amount' => uc_currency_format($amount, FALSE), '@currency' => $payment_currency)), 'order', 'payment_received');
      }
      break;

    case GopayHelper::FAILED:
      if ($payment['session_state'] !== GopayHelper::FAILED) {
        $payment['session_state'] = $result['code'];
        uc_order_comment_save($order_id, 0, t("The customer's attempted payment from a GoPay account failed."), 'admin');
      }
      break;

    case GopayHelper::CANCELED:
      if ($payment['session_state'] !== GopayHelper::CANCELED) {
        $payment['session_state'] = $result['code'];
        uc_order_comment_save($order_id, 0, t('GoPay has canceled the reversal and returned !amount !currency to your account.', array('!amount' => uc_currency_format($payment['total_price'], FALSE), '!currency' => $payment_currency)), 'admin');
      }
      break;

    case GopayHelper::TIMEOUTED:
      if ($payment['session_state'] !== GopayHelper::TIMEOUTED) {
        $payment['session_state'] = $result['code'];
        uc_order_comment_save($order_id, 0, t('The authorization of GoPay payment has failed and cannot be captured.'), 'admin');
      }
      break;

    default:
      watchdog('uc_gopay', 'GoPay: Unknown payment state.', array(), WATCHDOG_ERROR, l(t('view'), 'admin/store/orders/' . $order_id));
      throw new GopayPaymentException('Unknown GoPay payment state.');
  }

  // Save payment status info if changed.
  if ($original_state != $payment['session_state']) {
    watchdog('uc_gopay', 'GoPay: transaction @id for @order_id state changed from %from to %to.', array('@id' => $returned_payment_session_id, '@order_id' => $order_id, '%from' => $original_state ? $original_state : '--', '%to' => $payment['session_state']), WATCHDOG_NOTICE, l(t('view'), 'admin/store/orders/' . $order_id));
    uc_gopay_record_save($payment);
  }
}
