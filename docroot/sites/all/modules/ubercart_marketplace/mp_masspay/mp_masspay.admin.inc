<?php

/**
 * @file
 * Marketplace MassPay administration.
 */

/**
 * Display the seller MassPay in Ubercart admin interface
 */
function mp_masspay_seller_form($form) {
  $timezone_offset = variable_get('date_default_timezone', 0);
  $format = variable_get('uc_date_format_default', 'm/d/Y');
  $args = mp_masspay_get_args();
  $order_statuses = array('complete');

  // Get array of all sellers.
  $sellers = array();
  if ($args['rid'] == DRUPAL_AUTHENTICATED_RID) {
    $sellers = db_query('SELECT uid FROM {users} WHERE uid <> :uid', array(':uid' => 0))->fetchCol(0);
  }
  else {
    $sellers = db_query("SELECT u.uid FROM {users_roles} AS u WHERE u.rid = :rid", array(':rid' => $args['rid']))->fetchCol(0);
    //$sellers[] = 1;
  }

  $methods = array('paypal' => t('Paypal'), 'manual' => t('Manual'), 'none' => t('Not selected'));
  if ($args['paid'] == 1) {
    $paid_statuses = array('0', '1', '2', '3', '4', '5');
  }
  else {
    $paid_statuses = array('0', '3', '4');
  }

  $commission_total = 0;
  $revenue_total = 0;
  $product_total = 0;
  $order_total = 0;

  // create form
  $form = array();
  $form['#tree'] = TRUE;
  $form['sellers'] = array();

  // Loop through the sellers and build the report table/form.
  foreach ($sellers as $seller) {
    $product_data = '';

    // Create the seller title and link for each seller.
    $user = user_load($seller);
    $name = $user->name;
    $link = l($name, 'user/' . $seller);
    $can_pay = $user->data['payment_method'] == 'manual' || ($user->data['payment_method'] == 'paypal' && !empty($user->data['paypal_email']));

    // Build the order data for the seller.
    $order_list = db_query('SELECT sos.order_id FROM {uc_orders} AS o 
      LEFT JOIN {mp_seller_order_statuses} AS sos ON o.order_id = sos.order_id 
      WHERE sos.uid = :uid
      AND o.created >= :start 
      AND o.created <= :end
      AND sos.order_status IN (:order_statuses) 
      AND sos.seller_paid_status IN (:paid_statuses)',
      array(
      	':uid' => $seller,
        ':start' => $args['start_date'],
        ':end' => $args['end_date'],
        ':order_statuses' => $order_statuses,
        ':paid_statuses' => $paid_statuses
      ))->fetchCol(0);
    $order_total += count($order_list);

    // Build the product data for the seller.
    if ($args['detail']) {
      // Grab the detailed product breakdown if selected.
      $result = db_query('SELECT SUM(op.qty) as count, n.title, n.nid 
        FROM {uc_order_products} as op 
        LEFT JOIN {uc_orders} as o ON o.order_id = op.order_id 
        LEFT JOIN {mp_seller_order_statuses} AS sos ON o.order_id = sos.order_id 
        LEFT JOIN {node} as n ON n.nid = op.nid 
        WHERE sos.uid = :uid 
        AND n.uid = :uid 
        AND sos.order_status IN (:order_statuses) 
        AND sos.seller_paid_status IN (:paid_statuses) 
        AND o.created >= :start_date
        AND o.created <= :end_date 
        GROUP BY n.nid 
        ORDER BY count DESC, n.title ASC',
        array(
        	':uid' => $seller,
        	':order_statuses' => $order_statuses,
        	':paid_statuses' => $paid_statuses,
        	':start_date' => $args['start_date'],
        	':end_date' => $args['end_date'],
       	)); 
      foreach ($result as $product_breakdown) {
        $product_data .= $product_breakdown->count . ' x ' . l($product_breakdown->title, 'node/' . $product_breakdown->nid) . "<br/>\n";
        $product_total += $product_breakdown->count;
      }
    }
    else {
      // Otherwise just display the total number of products sold.
      $product_data = db_query("SELECT SUM(op.qty) FROM {uc_orders} as o 
        LEFT JOIN {uc_order_products} as op ON o.order_id = op.order_id 
        LEFT JOIN {mp_seller_order_statuses} AS sos ON sos.order_id = o.order_id 
        LEFT JOIN {node} as n ON n.nid = op.nid 
        WHERE sos.uid = :uid
        AND n.uid = :uid 
        AND sos.order_status IN (:order_statuses) 
        AND sos.seller_paid_status IN (:paid_statuses) 
        AND o.created >= :start_date 
        AND o.created <= :end_date", 
        array(
        	':uid' => $seller,
        	':order_statuses' => $order_statuses,
        	':paid_statuses' => $paid_statuses,
        	':start_date' => $args['start_date'],
        	':end_date' => $args['end_date'],
       	))->fetchField(); 
      $product_total += $product_data;
    }
    
    $counts = reset(db_query("SELECT SUM(p.cost * p.qty) AS cost, SUM(p.price * p.qty) AS price 
      FROM {uc_orders} AS o 
      LEFT JOIN {mp_seller_order_statuses} AS sos ON sos.order_id = o.order_id 
      LEFT JOIN {uc_order_products} AS p ON p.order_id = sos.order_id 
      LEFT JOIN {node} as n ON n.nid = p.nid 
      WHERE sos.uid = :uid 
      AND n.uid = :uid
      AND sos.order_status IN (:order_statuses) 
      AND sos.seller_paid_status IN (:paid_statuses) 
      AND o.created >= :start_date
      AND o.created <= :end_date", 
      array(
      	':uid' => $seller,
      	':order_statuses' => $order_statuses,
      	':paid_statuses' => $paid_statuses,
      	':start_date' => $args['start_date'],
      	':end_date' => $args['end_date'],
     	))->fetchAll());

    // add seller shipping
    if (module_exists('mp_quote') && !empty($order_list)) {
      // TODO Please convert this statement to the D7 database API syntax.
      $shipping = db_query('SELECT SUM(rate) FROM {mp_quote_shipping_rates} 
        WHERE uid = :uid AND order_id IN (:order_list)', 
        array(':uid' => $seller, ':order_list' => $order_list))
        ->fetchField();
      $counts->price += $shipping;
      $counts->cost += $shipping;
    }

    // Tally up the revenue from the orders.
    $revenue_count = !empty($counts->price) ? $counts->price : 0;

    // Tally up the commission from the orders.
    $commission_count = !empty($counts->cost) ? $counts->cost : 0;

    // Add the seller's row to the report table.
    $seller_row = array();
    $seller_row['link'] = array('#markup' => $link);
    $seller_row['order_data'] = array('#markup' => count($order_list));
    $seller_row['product_data'] = array('#markup' => empty($product_data) ? '0' : $product_data);
    $seller_row['revenue_count'] = array('#markup' => uc_currency_format($revenue_count));
    $seller_row['commission_count'] = array('#markup' => uc_currency_format($commission_count));
    $seller_row['payment_method'] = array('#markup' => isset($methods[$user->data['payment_method']]) ? $methods[$user->data['payment_method']] : t('Unspecified'));
    if (!$can_pay) {
      $seller_row['payment_method']['#markup'] .= '*';
    }
    $seller_row['checked'] = array('#type' => 'checkbox');
    $seller_row['email'] = array(
      '#type' => 'value',
      '#value' => isset($user->data['paypal_email']) ? $user->data['paypal_email'] : '',
    );
    $seller_row['manual_details'] = array(
      '#type' => 'value',
      '#value' => isset($user->data['manual_pay_details']) ? $user->data['manual_pay_details'] : '',
    );
    $seller_row['commission'] = array(
      '#type' => 'value',
      '#value' => $commission_count,
    );
    $seller_row['order_list'] = array(
      '#type' => 'value',
      '#value' => implode(',', $order_list),
    );
    $seller_row['method'] = array(
      '#type' => 'value',
      '#value' => $user->data['payment_method'],
    );
    if ($seller == 1 || $commission_count == NULL || !$can_pay) {
      $seller_row['checked']['#attributes'] = array('disabled' => 'disabled');
    }

    $form['sellers'][$seller] = $seller_row;

    $commission_total += $commission_count;
    $revenue_total += $revenue_count;
  }

  $form['sellers']['#theme'] = 'mp_masspay_seller_fieldset';
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Make Payment'),
  );

  return $form;
}

/**
 * Submit handler for mp_masspay_seller_form().
 */
function mp_masspay_seller_form_submit($form, &$form_state) {
  $sellers = array();
  $manuals = array();
  if (arg(9) == 1) {
    drupal_set_message(t('Please uncheck "include sales that sellers have already received payment for" before making a payment.'), 'error');
    return;
  }
  $payment_id = db_query("SELECT MAX(payment_id) FROM {mp_seller_payments}")->fetchField();
  foreach ($form_state['values']['sellers'] as $sid => $seller) {
    if ($seller['checked']) {
      $payment_id++;
      if ($seller['method'] == 'paypal') {
        if (strlen($seller['email']) > 4) {
          $sellers[$sid] = array(
            'email' => $seller['email'],
            'commission' => $seller['commission'],
            'uid' => $sid,
            'order_list' => $seller['order_list'],
            'payment_id' => $payment_id,
          );
        }
      }
      else {
        $manuals[$sid] = array(
          'manual_details' => $seller['manual_details'],
          'uid' => $sid,
          'commission' => $seller['commission'],
          'order_list' => $seller['order_list'],
          'payment_id' => $payment_id,
        );
      }
    }
  }
  if (sizeof($sellers) != 0) {
    if (module_exists('uc_paypal')) {
      mp_masspay_make_masspay($sellers);
    }
    else {
      drupal_set_message(t('Please enable uc_paypal module and configure API credentials.'), 'error');
    }
  }
  // carry over variables to overview page
  $_SESSION['payment_manuals'] = $manuals;
  $_SESSION['payment_sellers'] = $sellers;
  $form_state['redirect'] = 'admin/store/reports/seller/payments';
}

/**
 * Menu callback for admin/store/reports/seller/payments
 */
function mp_masspay_seller_payments() {
  $sellers = $_SESSION['payment_sellers'];
  $manuals = $_SESSION['payment_manuals'];

  $output = '<h2>' . t('Paypal Masspay Response') . '</h2>';
  if ($sellers != NULL && variable_get('mp_masspay_enable_paypal', TRUE)) {

    $output .= '<p>' . t('You may need to refresh this page so that it reflects Paypal\'s most recent response.') . '</p>' .
      '<h3>' . t('Payment Status Key (for table below):') . '</h3>' .
      '<ul>' .
        '<li>' . t('Completed: The payment has been processed.') . '</li>' .
        '<li>' . t('Failed: Insufficient Paypal balance.') . '</li>' .
        '<li>' . t('Returned: Funds have been returned to you') . '</li>' .
        '<li>' . t('Unclaimed: Receiver\'s email address is either not a Paypal account or receiver\'s Paypal account is blocked.') . '</li>' .
        '<li>' . t('Pending: Waiting for feedback from Paypal\'s Instant Payment Notification (IPN) system.') . '</li>' .
      '</ul><br />';

    // Masspay payment table
    $header = array(t('uid'), t('email'), t('amount'), t('payment status'));
    $rows = array();
    foreach ($sellers as $seller) {
      $status = db_query("SELECT status FROM {mp_seller_payments} WHERE payment_id = :payment_id", array(':payment_id' => $seller['payment_id']))->fetchField();
      $status = $status ? $status : 'Pending';
      $rows[] = array(l($seller['uid'], 'user/' . $seller['uid']), $seller['email'], $seller['commission'], $status);
    }
    $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('width' => '100%', 'class' => array('mp-reports-payments-table'))));
    $output .= '<br />';
  }
  else {
    $output .= 'Not applicable';
  }

  $output .= '<h2>' . t('Manual Payment Summary') . '</h2>';
  if ($manuals != NULL && variable_get('mp_masspay_enable_check', FALSE)) {
    $output .= '<p>' . t('Here are the supplied details from the selected sellers for manual payment.') . '</p>';
    $output .= drupal_render(drupal_get_form('mp_masspay_manuals_form', $manuals));
  }
  else {
    $output .= 'Not applicable';
  }

  return $output;
}

/**
 * Form builder form mp_masspay_manuals_form
 */
function mp_masspay_manuals_form($form, &$form_state, $manuals) {
  // create form
  $form = array();
  $form['#tree'] = TRUE;
  $form['manuals'] = array();

  foreach ($manuals as $manual) {
    // Add the manual's row to the report table.
    $manual_row = array();
    $manual_row['uid_print'] = array('#markup' => l($manual['uid'], 'user/' . $manual['uid']));
    $manual_row['order_ids_print'] = array('#markup' => $manual['order_list']);
    $manual_row['manual_details'] = array('#markup' => $manual['manual_details']);
    $manual_row['commission_print'] = array('#markup' => uc_currency_format($manual['commission']));
    $manual_row['checked'] = array('#type' => 'checkbox');
    $manual_row['uid'] = array(
      '#type' => 'value',
      '#value' => $manual['uid'],
    );
    $manual_row['order_ids'] = array(
      '#type' => 'value',
      '#value' => $manual['order_list'],
    );
    $manual_row['commission'] = array(
      '#type' => 'value',
      '#value' => $manual['commission'],
    );
    $manual_row['payment_id'] = array(
      '#type' => 'value',
      '#value' => $manual['payment_id'],
    );

    $form['manuals'][$manual['uid']] = $manual_row;
  }

  $form['manuals']['#theme'] = 'mp_masspay_manuals_fieldset';
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Complete Manual Payments'),
  );

  return $form;
}

/**
 * Submit handler for mp_masspay_manuals_form().
 */
function mp_masspay_manuals_form_submit($form, &$form_state) {
  foreach ($form_state['values']['manuals'] as $sid => $manual) {
    if ($manual['checked']) {
      db_insert('mp_seller_payments')
      ->fields(array(
        'uid' => $sid,
        'method' => t('Manual'),
        'status' => 'Completed',
        'gross' => $manual['commission'],
      ))
      ->execute();
      foreach (explode(',', $manual['order_ids']) as $order_id) {
        db_update('mp_seller_order_statuses')
        ->fields(array(
          'seller_paid_status' =>  5,
          'payment_id' => $manual['payment_id'],
        ))
        ->condition('uid', $sid)
        ->condition('order_id', $order_id)
        ->execute();
      }
    }
  }
  drupal_set_message('Seller order statuses have been changed to reflect manual payment(s). Please do not resubmit.');
}

/**
 * Parse URL to build report parameters.
 */
function mp_masspay_get_args() {
  // Use default report parameters if we don't detect values in the URL.
  if (arg(5) == '') {
    $timezone_offset = variable_get('date_default_timezone', 0);
    $roles_array = array_keys(user_roles(true, 'act as seller'));
    $args = array(
      'start_date' => gmmktime(0, 0, 0, format_date(REQUEST_TIME, 'custom', 'n'), 1, format_date(REQUEST_TIME, 'custom', 'Y')) - $timezone_offset,
      'end_date' => REQUEST_TIME - $timezone_offset,
      'rid' => $roles_array[0],
      'detail' => FALSE,
      'paid' => FALSE,
    );
  }
  else {
    $args = array(
      'start_date' => arg(5),
      'end_date' => arg(6),
      'rid' => arg(7),
      'detail' => arg(8),
      'paid' => arg(9),
    );
  }
  return $args;
}

/**
 * Display the seller reports in Ubercart admin interface
 */
function mp_masspay_seller() {
  drupal_set_title(t('Seller Reports'));

  $args = mp_masspay_get_args();

  // Build the page output holding the form, table, and txt export link.
  $build = array();
  $build['custom'] = drupal_get_form('mp_masspay_seller_custom_form', $args);
  $build['seller'] = drupal_get_form('mp_masspay_seller_form');

  return $build;
}

/**
 * Returns a form for custom seller sales viewing
 */
function mp_masspay_seller_custom_form($form, $form_state, $values) {
  $form['search'] = array(
    '#type' => 'fieldset',
    '#title' => t('Customize seller report parameters'),
    '#description' => t('Adjust these values and update the report to build your custom seller summary. Once submitted, the report may be bookmarked for easy reference in the future.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['search']['start_date'] = array(
    '#type' => 'date',
    '#title' => t('Start date'),
    '#default_value' => array(
      'month' => format_date($values['start_date'], 'custom', 'n'),
      'day' => format_date($values['start_date'], 'custom', 'j'),
      'year' => format_date($values['start_date'], 'custom', 'Y'),
    ),
  );
  $form['search']['end_date'] = array(
    '#type' => 'date',
    '#title' => t('End date'),
    '#default_value' => array(
      'month' => format_date($values['end_date'], 'custom', 'n'),
      'day' => format_date($values['end_date'], 'custom', 'j'),
      'year' => format_date($values['end_date'], 'custom', 'Y'),
    ),
  );
  $form['search']['rid'] = array(
    '#type' => 'select',
    '#title' => t('Sellers to show'),
    '#description' => t('Only sellers with selected role will be shown in report.'),
    '#options' => user_roles(true, 'act as seller'),
    '#default_value' => $values['rid'],
  );
  $form['search']['detail'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show a detailed list of products sold by seller.'),
    '#default_value' => $values['detail'],
  );
  $form['search']['paid'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include sales that sellers have already received payment for.'),
    '#default_value' => $values['paid'],
  );
  $form['search']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update report'),
  );
  return $form;
}

/**
 * Submit handler for custom seller report.
 */
function mp_masspay_seller_custom_form_submit($form, &$form_state) {
  $timezone_offset = variable_get('date_default_timezone', 0);

  // Build the start and end dates from the form.
  $start_date = gmmktime(0, 0, 0, $form_state['values']['start_date']['month'], $form_state['values']['start_date']['day'], $form_state['values']['start_date']['year']) - $timezone_offset;
  $end_date = gmmktime(23, 59, 59, $form_state['values']['end_date']['month'], $form_state['values']['end_date']['day'], $form_state['values']['end_date']['year']) - $timezone_offset;

  $args = array(
    $start_date,
    $end_date,
    $form_state['values']['rid'],
    $form_state['values']['detail'],
    $form_state['values']['paid'],
  );

  drupal_goto('admin/store/reports/seller/custom/' . implode('/', $args));
}

/**
 * Process PayPal IPN for masspay.
 */
function mp_masspay_ipn() {
  watchdog('mp_masspay', 'Masspay IPN received');

  if (variable_get('mp_masspay_ipn_debug', FALSE)) {
    $details = array();
    foreach ($_POST as $key => $value) {
      $details[] = $key . ' = ' . check_plain($value);
    }
    watchdog('mp_masspay', 'Masspay IPN details: %details', array('%details' => implode(', ', $details)), WATCHDOG_DEBUG);
  }

  if (check_plain($_POST['payment_status']) == 'Denied') {
    watchdog('mp_masspay', 'Masspay IPN denied', WATCHDOG_ERROR);
  }

  $req = 'cmd=_notify-validate';

  foreach ($_POST as $key => $value) {
    $value = urlencode(stripslashes($value));
    $req .= '&' . $key . '=' . $value;
  }

  if (variable_get('uc_paypal_wpp_server', '') == 'https://api-3t.paypal.com/nvp') {
    $host = 'https://www.paypal.com/cgi-bin/webscr';
  }
  else {
    $host = variable_get('uc_paypal_wps_server', 'https://www.sandbox.paypal.com/cgi-bin/webscr');
  }

  $host = explode('/', substr($host, 8));

  // Post back to PayPal to validate
  $header = "POST /cgi-bin/webscr HTTP/1.0\r\n";
  $header .= 'Host: ' . $host[0] . "\r\n";
  $header .= "Content-Type: application/x-www-form-urlencoded\r\n";
  $header .= 'Content-Length: ' . strlen($req) . "\r\n\r\n";

  // Address a screw-up on PayPal's Sandbox that prevents normal validation.
  if (strpos($host[0], 'sandbox') !== FALSE && function_exists('openssl_open')) {
    $fp = fsockopen('ssl://www.sandbox.paypal.com', 443, $errno, $errstr, 30);
  }
  else {
    // The old "normal" way of validating an IPN.
    $fp = fsockopen($host[0], 80, $errno, $errstr, 30);
  }

  if (!$fp) {
    watchdog('mp_masspay', 'Masspay IPN failed with HTTP error.', WATCHDOG_ERROR);
    return;
  }

  fputs($fp, $header . $req);
  while (!feof($fp)) {
    $res = fgets($fp, 1024);
    if (strcmp($res, 'VERIFIED') == 0) {
      watchdog('mp_masspay', 'Masspay IPN transaction verified.');

      $number = 1;
      while (isset($_POST['masspay_txn_id_' . $number])) {
        $txn_id = check_plain($_POST['masspay_txn_id_' . $number]);
        $payment_status = check_plain($_POST['status_' . $number]);
        $receiver_email = check_plain($_POST['receiver_email_' . $number]);
        $payment_gross = check_plain($_POST['mc_gross_' . $number]);
        $payment_fee = check_plain($_POST['mc_fee_' . $number]);
        $payment_id = check_plain($_POST['unique_id_' . $number]);

        $result = db_query("SELECT COUNT(*) FROM {mp_seller_payments} WHERE payment_id = :payment_id", array(':payment_id' => $payment_id))->fetchField();
        if ($result == 0) {
          watchdog('mp_masspay', 'Masspay IPN contains invalid unique_id.', WATCHDOG_ERROR);
          fclose($fp);
          return;
        }

        db_update('mp_seller_payments')
        ->fields(array(
          'txn_id' => $txn_id,
          'fee' => $payment_fee,
          'gross' => $payment_gross,
          'status' => $payment_status,
        ))
        ->condition('payment_id', $payment_id)
        ->execute();

        switch ($payment_status) {
          case 'Completed':
            $status_code = 5;
            break;
          case 'Failed':
            $status_code = 4;
            break;
          case 'Reversed':
            $status_code = 3;
            break;
          case 'Unclaimed':
            $status_code = 2;
            break;
        }
        db_update('mp_seller_order_statuses')
        ->fields(array(
          'seller_paid_status' => $status_code,
        ))
        ->condition('payment_id', $payment_id)
        ->execute();

        $number++;
      }
    }
    elseif (strcmp($res, 'INVALID') == 0) {
      watchdog('mp_masspay', 'Masspay IPN transaction failed verification.', WATCHDOG_ERROR);
    }
  }
  fclose($fp);
}

/**
 * Make a MassPay payment.
 *
 * @param $sellers
 *   Array of sellers to be paid.
 */
function mp_masspay_make_masspay($sellers) {
  $nvp_request = array(
    'METHOD' => 'MassPay',
    'RECEIVERTYPE' => 'EmailAddress',
  );
  $number = 0;
  foreach ($sellers as $seller) {
    $nvp_request += array('L_EMAIL' . $number => $seller['email']);
    $nvp_request += array('L_AMT' . $number => uc_currency_format($seller['commission'], FALSE, FALSE, '.'));
    $nvp_request += array('L_UNIQUEID' . $number => $seller['payment_id']);
    $number++;
  }
  $nvp_request += array('CURRENCYCODE' => variable_get('uc_paypal_wpp_currency', 'USD'));
  $nvp_request += array('EMAILSUBJECT' => variable_get('mp_masspay_subject', 'Payment for your sales.'));

  $nvp_response = uc_paypal_api_request($nvp_request, variable_get('uc_paypal_wpp_server', 'https://api-3t.sandbox.paypal.com/nvp'));
  //dpm($nvp_response);
  if ($nvp_response['ACK'] == 'Success') {
    watchdog('mp_masspay', 'Masspay responds success.');
    foreach ($sellers as $seller) {
      db_insert('mp_seller_payments')
      ->fields(array(
        'uid' => $seller['uid'],
        'email' => $seller['email'],
        'method' => t('Paypal'),
      ))
      ->execute();
      foreach (explode(',', $seller['order_list']) as $order_id) {
        db_update('mp_seller_order_statuses')
        ->fields(array(
          'payment_id' => $seller['payment_id'],
          'seller_paid_status' =>  1,
        ))
        ->condition('uid', $seller['uid'])
        ->condition('order_id', $order_id)
        ->execute();
      }
    }
  }
  else {
    watchdog('mp_masspay', 'Failed to make MassPay NVP request.'
      . 'Error Code: ' . $nvp_response['L_ERRORCODE0'] . ' '
      . 'Error Message: ' . $nvp_response['L_LONGMESSAGE0']);
  }
}
