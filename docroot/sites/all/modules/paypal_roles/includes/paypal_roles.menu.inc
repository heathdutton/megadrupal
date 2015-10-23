<?php
/**
 * @file
 * Menu callbacks for the paypal_roles module.
 */

/**
 * Callback function to handle PayPal IPN requests.
 */
function paypal_roles_ipn_listener_callback() {
  // STEP 1: read POST data.
  // Reading POSTed data directly from $_POST causes serialization issues with array data in the POST.
  // Instead, read raw POST data from the input stream.
  $raw_post_data = file_get_contents('php://input');
  $raw_post_array = explode('&', $raw_post_data);

  $myPost = array();
  foreach ($raw_post_array as $keyval) {
    $keyval = explode('=', $keyval);
    if (count($keyval) == 2) {
      $myPost[$keyval[0]] = urldecode($keyval[1]);
    }
  }

  // Read the IPN message sent from PayPal and prepend 'cmd=_notify-validate'.
  $req = 'cmd=_notify-validate';
  if (function_exists('get_magic_quotes_gpc')) {
    $get_magic_quotes_exists = TRUE;
  }
  else {
    $get_magic_quotes_exists = FALSE;
  }

  foreach ($myPost as $key => $value) {
    if ($get_magic_quotes_exists == TRUE && get_magic_quotes_gpc() == 1) {
      $value = urlencode(stripslashes($value));
    }
    else {
      $value = urlencode($value);
    }
    $req .= "&$key=$value";
  }

  // STEP 2: POST IPN data back to PayPal to validate.

  if (variable_get('paypal_roles_sandbox', 1) == "1") {
    $url = 'https://www.sandbox.paypal.com/cgi-bin/webscr';
  }
  else {
    $url = 'https://www.paypal.com/cgi-bin/webscr';
  }

  $ch = curl_init($url);
  curl_setopt($ch, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1);
  curl_setopt($ch, CURLOPT_POST, 1);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
  curl_setopt($ch, CURLOPT_POSTFIELDS, $req);
  curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 1);
  curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 2);
  curl_setopt($ch, CURLOPT_FORBID_REUSE, 1);
  curl_setopt($ch, CURLOPT_HTTPHEADER, array('Connection: Close'));

  // In wamp-like environments that do not come bundled with root authority
  // certificates, please download 'cacert.pem' from
  // "http://curl.haxx.se/docs/caextract.html" and set the directory path of
  // the certificate as shown below:
  // curl_setopt($ch, CURLOPT_CAINFO, dirname(__FILE__) . '/cacert.pem');
  if (!($res = curl_exec($ch))) {
    // error_log("Got " . curl_error($ch) . " when processing IPN data");
    curl_close($ch);
    drupal_exit();
  }
  curl_close($ch);

  $log = variable_get('paypal_roles_log_failed_ipn', 1);

  // STEP 3: Inspect IPN validation result and act accordingly.
  if (strcmp($res, "VERIFIED") == 0) {
    $txn_type = strtoupper($_POST['txn_type']);

    switch ($txn_type) {
      // A subscriber has signed up for the service.
      case 'SUBSCR_SIGNUP':
        paypal_roles_ipn_listener_subscr_signup_callback($log, $res);
        break;

      // A subscriber cancelled a subscription.
      case 'SUBSCR_CANCEL':
        paypal_roles_ipn_listener_subscr_cancel_callback($log, $res);
        break;

      // A subscriber profile has been modified.
      case 'SUBSCR_MODIFY':
        paypal_roles_ipn_listener_subscr_modify_callback($log, $res);
        break;

      // A subscriber has paid for the service.
      case 'SUBSCR_PAYMENT':
      case 'WEB_ACCEPT':
        paypal_roles_ipn_listener_subscr_payment_callback($log, $res);
        break;

      // A subscriber tried to pay for the service but things didn't work out.
      case 'SUBSCR_FAILED':
        paypal_roles_ipn_listener_subscr_failed_callback($log, $res);
        break;

      // A subscriber has reached the end of the subscription term.
      case 'SUBSCR_EOT':
        paypal_roles_ipn_listener_subscr_eot_callback($log, $res);
        break;
    }
  }
  else {
    if (strcmp($res, "INVALID") == 0) {
      if ((int) $log === 1) {
        watchdog('PAYPAL_ROLES_IPN', var_export($_POST, TRUE));
      }
      drupal_exit();
    }
  }
}

/**
 * Callback function to handle IPN messages (Signup).
 * A subscriber has signed up for the service.
 *
 * @param bool $log
 * @param string $res
 */
function paypal_roles_ipn_listener_subscr_signup_callback($log = FALSE, $res = '') {
  // Check that receiver_email is your Primary PayPal email.
  if (variable_get('paypal_roles_sandbox', 1) == "1") {
    $remail = variable_get('paypal_roles_s_email', '');
  }
  else {
    $remail = variable_get('paypal_roles_l_email', '');
  }
  if (!isset($_POST['receiver_email']) || $_POST['receiver_email'] != $remail) {
    if ((int) variable_get('paypal_roles_validate_primary_email', 1) === 1) {
      if ((int) $log === 1) {
        watchdog('PAYPAL_ROLES', t('The response from IPN was: Invalid "receiver_email".'));
        watchdog('PAYPAL_ROLES_IPN', var_export($_POST, TRUE));
      }
      drupal_exit();
    }
  }

  // Check that Membership Package subscribed for is valid.
  if (!isset($_POST['item_number'])) {
    if ((int) $log === 1) {
      watchdog('PAYPAL_ROLES', t('The response from IPN was: Invalid "item_number".'));
      watchdog('PAYPAL_ROLES_IPN', var_export($_POST, TRUE));
    }
    drupal_exit();
  }
  $ml = paypal_roles_memberships_load($_POST['item_number']);
  if (!$ml) {
    if ((int) $log === 1) {
      watchdog('PAYPAL_ROLES', t('The response from IPN was: Invalid "item_number".'));
      watchdog('PAYPAL_ROLES_IPN', var_export($_POST, TRUE));
    }
    drupal_exit();
  }

  // If membership has a free trial period: we create a custom payment for
  // trial period, because PayPal does not send IPN message about the payment
  // when no charge is being made.
  if ((int) $ml->trial_length > 0 && (float) $ml->trial_amount == 0) {
    // Prepare variable for user account. If left FALSE, user is new user,
    // needs registration.
    $user = FALSE;

    $steps = variable_get('paypal_roles_account_selection', array(0, 1, 2));
    foreach ($steps as $step) {
      if ($user === FALSE) {
        _paypal_roles_try_to_load_account($user, $step);
      }
    }

    // Create Trial period as a custom payment with the specified user.
    $ipn = paypal_roles_save_ipn_message_trial($user);
    if ($ipn) {
      paypal_roles_manage_roles_by_users($ipn->uid);

      if ((int) $log === 1) {
        watchdog('PAYPAL_ROLES', t('Successful Payment Recorded. The response from IPN was') . ': ' . $res);
        watchdog('PAYPAL_ROLES_IPN', var_export($_POST, TRUE));
      }
    }
  }
}

/**
 * Callback function to handle IPN messages (Cancel).
 * A subscriber cancelled a subscription.
 *
 * @param bool $log
 * @param string $res
 */
function paypal_roles_ipn_listener_subscr_cancel_callback($log = FALSE, $res = '') {
  // TODO: If a subscriber cancelled a subscription, we need to cancel the 
  // active payments. User options: Cancel payments immediately, or wait until
  // expiration time.
}

/**
 * Callback function to handle IPN messages (Modify).
 * A subscriber profile has been modified.
 *
 * @param bool $log
 * @param string $res
 */
function paypal_roles_ipn_listener_subscr_modify_callback($log = FALSE, $res = '') {
  // No action is required. (?)
}

/**
 * Callback function to handle IPN messages (Payment (USD), Payment (MC)).
 * A subscriber has paid for the service.
 *
 * @param bool $log
 * @param string $res
 */
function paypal_roles_ipn_listener_subscr_payment_callback($log = FALSE, $res = '') {
  // Check whether the payment_status is Completed.
  if (!isset($_POST['payment_status']) || strtoupper($_POST['payment_status']) != "COMPLETED") {
    if ((int) $log === 1) {
      watchdog('PAYPAL_ROLES', t('The response from IPN was: "payment_status" is not completed.'));
      watchdog('PAYPAL_ROLES_IPN', var_export($_POST, TRUE));
    }
    drupal_exit();
  }

  // Check that txn_id has not been previously processed.
  if (!isset($_POST['txn_id']) || empty($_POST['txn_id'])) {
    if ((int) $log === 1) {
      watchdog('PAYPAL_ROLES', t('The response from IPN was: Invalid "txn_id".'));
      watchdog('PAYPAL_ROLES_IPN', var_export($_POST, TRUE));
    }
    drupal_exit();
  }

  // Check that txn_id exists.
  $txn = paypal_roles_payments_load($_POST['txn_id']);
  if ($txn) {
    if ((int) $log === 1) {
      watchdog('PAYPAL_ROLES', t('The response from IPN was: "txn_id" is duplicated.'));
      watchdog('PAYPAL_ROLES_IPN', var_export($_POST, TRUE));
    }
    drupal_exit();
  }

  // Check that receiver_email is your Primary PayPal email.
  if (variable_get('paypal_roles_sandbox', 1) == "1") {
    $remail = variable_get('paypal_roles_s_email', '');
  }
  else {
    $remail = variable_get('paypal_roles_l_email', '');
  }
  if (!isset($_POST['receiver_email']) || $_POST['receiver_email'] != $remail) {
    if ((int) variable_get('paypal_roles_validate_primary_email', 1) === 1) {
      if ((int) $log === 1) {
        watchdog('PAYPAL_ROLES', t('The response from IPN was: Invalid "receiver_email".'));
        watchdog('PAYPAL_ROLES_IPN', var_export($_POST, TRUE));
      }
      drupal_exit();
    }
  }

  // Check that Membership Package subscribed for is valid.
  if (!isset($_POST['item_number'])) {
    if ((int) $log === 1) {
      watchdog('PAYPAL_ROLES', t('The response from IPN was: Invalid "item_number".'));
      watchdog('PAYPAL_ROLES_IPN', var_export($_POST, TRUE));
    }
    drupal_exit();
  }
  $ml = paypal_roles_memberships_load($_POST['item_number']);
  if (!$ml) {
    if ((int) $log === 1) {
      watchdog('PAYPAL_ROLES', t('The response from IPN was: Invalid "item_number".'));
      watchdog('PAYPAL_ROLES_IPN', var_export($_POST, TRUE));
    }
    drupal_exit();
  }

  // Prepare variable for user account. If left FALSE, user is new user, needs
  // registration.
  $user = FALSE;

  $steps = variable_get('paypal_roles_account_selection', array(0, 1, 2));
  foreach ($steps as $step) {
    if ($user === FALSE) {
      _paypal_roles_try_to_load_account($user, $step);
    }
  }

  $ipn = paypal_roles_save_ipn_message($user);
  if ($ipn) {
    paypal_roles_manage_roles_by_users($ipn->uid);

    if ((int) $log === 1) {
      watchdog('PAYPAL_ROLES', t('Successful Payment Recorded. The response from IPN was') . ': ' . $res);
      watchdog('PAYPAL_ROLES_IPN', var_export($_POST, TRUE));
    }
  }
}

/**
 * Callback function to handle IPN messages (Failed).
 * A subscriber tried to pay for the service but things didn't work out.
 *
 * @param bool $log
 * @param string $res
 */
function paypal_roles_ipn_listener_subscr_failed_callback($log = FALSE, $res = '') {
  // No action is required.
}

/**
 * Callback function to handle IPN messages (EOT).
 * A subscriber has reached the end of the subscription term.
 *
 * @param bool $log
 * @param string $res
 */
function paypal_roles_ipn_listener_subscr_eot_callback($log = FALSE, $res = '') {
  // No action is required. If no payment is renewing membership, PayPal Roles
  // removes role from account.
}

/**
 * Helper function to try load Drupal account.
 */
function _paypal_roles_try_to_load_account(&$user, $step) {
  switch ((int) $step) {
    // Try to load account by using Payer or Subscriber ID.
    case 0:
      $scr = NULL;
      if (isset($_POST['subscr_id'])) {
        $scr = paypal_roles_payer_load($_POST['subscr_id']);
      }

      $pyr = NULL;
      if (isset($_POST['payer_id'])) {
        $pyr = paypal_roles_payer_load($_POST['payer_id']);
      }

      if (isset($scr->uid) && $scr->uid > 0) {
        $user = user_load($scr->uid);
      }
      else {
        if (isset($pyr->uid) && $pyr->uid > 0) {
          $user = user_load($pyr->uid);
        }
      }
      break;

    // Try to load account by using the email of logged in (Drupal) user.
    case 1:
      if (isset($_POST['custom']) && valid_email_address($_POST['custom'])) {
        $user = user_load_by_mail($_POST['custom']);
      }
      break;

    // Try to load account by using the email, which is provided by PayPal
    // account.
    case 2:
      if ($_POST['payer_email']) {
        $user = user_load_by_mail($_POST['payer_email']);
      }
      break;
  }
}
