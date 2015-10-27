<?php

/**
 * @file epay.admin.inc
 * Admin setting and configuration forms for Epay.
 */

/**
 * List of implementations of the ePay module.
 * Link to edit settings for each implementation
 * @return string
 */
function epay_admin_list_implementations(){
  $header = array(t('Module'), t('Implementation'), '');
  $rows = array();
  foreach ((array)module_implements('epayapi') as $module) {
    foreach ((array)module_invoke($module, 'epayapi') as $delta => $name) {
      $module_info = unserialize(db_query("SELECT info FROM {system} WHERE name = :name;", array(':name' => $module))->fetchField());
      $rows[] = array($module_info['name'], $name['info'], l(t('Edit'), "admin/config/payment/epay/edit/$module/$delta"));
    }
  }
  return theme('table', array('header' => $header, 'rows' => $rows));
}

/**
 * The general settings form, for ePay.
 */
function epay_admin_settings_form($form, $form_state, $module, $delta) {
  $module_info = unserialize(db_query("SELECT info FROM {system} WHERE name = :name;", array(':name' => $module))->fetchField());
  $human_delta = module_invoke($module, 'epayapi', 'info', $delta);
  drupal_set_title(t('Change ePay implementation settings for @name', array('@name' => t($module_info['name']) . ': ' . $human_delta['info'])));

  $implementation = db_query("SELECT * FROM {epay_implementations} WHERE api_module = :api_module AND api_delta = :api_delta LIMIT 1;", array(':api_module' => $module, ':api_delta' => $delta))->fetchAssoc();
  if (empty($implementation)) {
    db_query("INSERT INTO {epay_implementations} (api_module, api_delta) VALUES (:api_module, :api_delta)", array(':api_module' => $module, ':api_delta' => $delta));
    $implementation = array();
  }

  $form['merchant'] = array(
    '#type' => 'textfield',
    '#title' => t('ePay merchantnumber'),
    '#description' => t('your unique merchantnumber from ePay.'),
    '#default_value' => isset($implementation['merchant']) ? $implementation['merchant'] : '',
    '#size' => 16,
    '#required' => TRUE,
  );
  $form['md5_password'] = array(
    '#type' => 'textfield',
    '#title' => t('Secret word for MD5 security'),
    '#description' => t('The secret MD5 password which is to be setup in both the ePay administration and here'),
    '#default_value' => isset($implementation['md5_password']) ? $implementation['md5_password'] : '',
    '#size' => 16,
    '#required' => TRUE,
  );

  $form['remote_password'] = array(
    '#type' => 'textfield',
    '#title' => t('Remote password'),
    '#description' => t('If you have password protected the remote calls in the ePay interface, you need to enter this password here.'),
    '#default_value' => isset($implementation['remote_password']) ? $implementation['remote_password'] : '',
    '#size' => 16,
    '#required' => FALSE,
  );

  $form['instantcapture'] = array(
    '#type' => 'checkbox',
    '#title' => t('Capture the payments as they are made.'),
    '#default_value' => isset($implementation['instantcapture']) ? $implementation['instantcapture'] : '',
  );

  $form['smsreceipt'] = array(
    '#type' => 'textfield',
    '#title' => t('Send an SMS receipt as the payment is made - enter the number here'),
    '#description' => t('Transmits a SMS about payment information as the payment is made. Enter the mobile number here.'),
    '#default_value' => isset($implementation['smsreceipt']) ? $implementation['smsreceipt'] : '',
    '#size' => 16,
  );

  $form['mailreceipt'] = array(
    '#type' => 'textfield',
    '#title' => t('Send an email as the payment is made - enter the email here'),
    '#description' => t('Transmits an email about payment information as the payment is made. Enter the email address here.'),
    '#default_value' => isset($implementation['mailreceipt']) ? $implementation['mailreceipt'] : '',
    '#size' => 16,
  );

  $form['epay_group'] = array(
    '#type' => 'textfield',
    '#title' => t('Move the payments to a specific group'),
    '#description' => t('As the payments are made, they are moved to a specific group within ePay.'),
    '#default_value' => isset($implementation['epay_group']) ? $implementation['epay_group'] : '',
    '#size' => 16,
  );

  $form['windowid'] = array(
    '#type' => 'textfield',
    '#title' => t('Window ID'),
    '#description' => t('If using a payment window other than the default one, enter the ID here.'),
    '#default_value' => isset($implementation['windowid']) ? $implementation['windowid'] : 1,
    '#size' => 4,
  );

  $form['googletracker'] = array(
    '#type' => 'textfield',
    '#title' => t('Google analytics'),
    '#description' => t('Enter any google analytics ID in the form UA-XXXXX-X.'),
    '#default_value' => isset($implementation['googletracker']) ? $implementation['googletracker'] : '',
    '#size' => 16,
  );

  $form['backgroundcolor'] = array(
    '#type' => 'textfield',
    '#title' => t('Background color'),
    '#description' => t('Color code for the payment window background. Hex code'),
    '#default_value' => isset($implementation['backgroundcolor']) ? $implementation['backgroundcolor'] : '',
    '#size' => 16,
  );

  $form['opacity'] = array(
    '#type' => 'textfield',
    '#title' => t('Opacity'),
    '#description' => t('Opacity for the payment window background. 0-100'),
    '#default_value' => isset($implementation['opacity']) ? $implementation['opacity'] : '',
    '#size' => 16,
  );

  $form['iframewidth'] = array(
    '#type' => 'textfield',
    '#title' => t('Iframe height'),
    '#description' => t('Size of the iframe shown if the integrated payment form is chosen.'),
    '#default_value' => isset($implementation['iframewidth']) ? $implementation['iframewidth'] : '',
    '#size' => 4,
  );

  $form['iframeheight'] = array(
    '#type' => 'textfield',
    '#title' => t('Iframe height'),
    '#description' => t('Size of the iframe shown if the integrated payment form is chosen.'),
    '#default_value' => isset($implementation['iframeheight']) ? $implementation['iframeheight'] : '',
    '#size' => 4,
  );

  $form['cssurl'] = array(
    '#type' => 'textfield',
    '#title' => t('CSS URL'),
    '#description' => t('Specify an URL for custom style sheet in the payment window.'),
    '#default_value' => isset($implementation['cssurl']) ? $implementation['cssurl'] : '',
    '#size' => 50,
  );

  $form['mobilecssurl'] = array(
    '#type' => 'textfield',
    '#title' => t('Mobile CSS URL'),
    '#description' => t('Specify an URL for custom style sheet in the payment window.'),
    '#default_value' => isset($implementation['mobilecssurl']) ? $implementation['mobilecssurl'] : '',
    '#size' => 50,
  );

  $form['currency'] = array(
    '#type' => 'select',
    '#title' => t('Currency type'),
    '#description' => t('The currency used for payments.'),
    '#options' => _epay_currency_codes(),
    '#default_value' => (isset($implementation['currency']) && $implementation['currency']) ? $implementation['currency'] : '208',
  );

  $form['windowstate'] = array(
    '#type' => 'select',
    '#title' => t('Window type'),
    '#description' => t('Select the desired type of ePay window'),
    '#options' => array(
      '1' => t('Overlay'),
      '2' => t('Iframe'),
      '3' => t('Full screen (separate page)'),
      '4' => t('Integrated payment form'),
    ),
    '#default_value' => isset($implementation['windowstate']) ? $implementation['windowstate'] : 3,
  );

  $form['mobile'] = array(
    '#type' => 'select',
    '#title' => t('Mobile payment window'),
    '#description' => t('Should mobile clients use a mobile payment window?'),
    '#options' => array(
      '0' => t('Disabled'),
      '1' => t('Auto detect'),
      '2' => t('Force'),
    ),
    '#default_value' => isset($implementation['mobile']) ? $implementation['mobile'] : 1,
  );

  $form['paymentcollection'] = array(
    '#type' => 'select',
    '#title' => t('ePay payment collection type'),
    '#description' => t('Start ePay in a specific collection'),
    '#options' => array(
      '0' => t('Customer choice'),
      '1' => t('Payment cards'),
      '2' => t('Home banking'),
      '3' => t('Invoice'),
      '4' => t('Mobile'),
      '5' => t('Other'),
    ),
    '#default_value' => isset($implementation['paymentcollection']) ? $implementation['paymentcollection'] : '',
  );

  $form['lockpaymentcollection'] = array(
    '#type' => 'checkbox',
    '#title' => t('Lock payment collection.'),
    '#description' => t('If checked, customers cannot choose other methods than the one chosen above.'),
    '#default_value' => isset($implementation['lockpaymentcollection']) ? $implementation['lockpaymentcollection'] : 0,
  );

  $form['splitpayment'] = array(
    '#type' => 'checkbox',
    '#title' => t('Split payment.'),
    '#description' => t('Allows you to capture parts of the amount at different times.'),
    '#default_value' => isset($implementation['splitpayment']) ? $implementation['splitpayment'] : 0,
  );

  $form['ownreceipt'] = array(
    '#type' => 'checkbox',
    '#title' => t('Own reciept page.'),
    '#description' => t('Skips the payment confirm page and goes to the drupal confirm page directly.'),
    '#default_value' => isset($implementation['ownreceipt']) ? $implementation['ownreceipt'] : 0,
  );

  $form['language'] = array(
    '#type' => 'select',
    '#title' => t('ePay window language type'),
    '#description' => t('The language of the payment window.'),
    '#options' => array(
      '0' => t('Auto detect'),
      '1' => t('Danish'),
      '2' => t('English'),
      '3' => t('Swedish'),
      '4' => t('Norwegian'),
      '5' => t('Greenland'),
      '6' => t('Iceland'),
      '7' => t('German'),
      '8' => t('Finnish'),
      '9' => t('Spanish'),
      '10' => t('French'),
      '11' => t('Polish'),
    ),
    '#default_value' => isset($implementation['language']) ? $implementation['language'] : 0,
  );

  $form['api_module'] = array(
    '#type' => 'value',
    '#value' => $module,
  );

  $form['api_delta'] = array(
    '#type' => 'value',
    '#value' => $delta,
  );

  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Submit handler for epay_admin_settings_form
 */
function epay_admin_settings_form_submit($form, &$form_state) {
  drupal_write_record('epay_implementations', $form_state['values'], array('api_delta', 'api_module'));
}

/**
 * Menu callback, displaying a sortable table with pager, displaying the transactions.
 */
function epay_admin_transactions() {
  $header = array(
    array('data' => t('Order id'), 'field' => 'order_id'),
    array('data' => t('Amount'), 'field' => 'amount'),
    array('data' => t('Currency'), 'field' => 'currency'),
    t('Username'),
    t('Order text'),
    array('data' => t('Status'), 'field' => 'status'),
    array('data' => t('Logs'), 'field' => 'log_count'),
    array('data' => t('Transaction time'), 'field' => 'payment_time', 'sort' => 'desc'),
  );
  $sql = "SELECT t.*, COUNT(l.log_id) log_count FROM {epay_transactions} t
          LEFT JOIN {epay_transaction_logs} l ON t.transaction_id = l.transaction_id
          GROUP BY t.transaction_id";
  $rows = array();
  $currency_codes = _epay_currency_codes();
  $status_codes = _epay_transaction_status();
  $result = db_query($sql)->fetchAll();
  if ($result) {
    foreach ($result as $transaction) {
      $username = t('Anonymous');
      if ($transaction->customer_uid > 0) {
        $account = user_load($transaction->customer_uid);
        if (!empty($account)) {
          $username = check_plain($account->name);
        }
      }
      $rows[] = array(
        $transaction->order_id,
        theme('epay_number', $transaction->amount/100),
        $currency_codes[$transaction->currency],
        $username,
        truncate_utf8(check_plain($transaction->order_text), 22, FALSE, TRUE),
        $status_codes[$transaction->status],
        user_access('epay_admin_transaction_logs') ? l($transaction->log_count, "admin/settings/epay/transaction/$transaction->transaction_id/logs") : $transaction->log_count,
        format_date($transaction->payment_time, 'short'),
      );
    }
    return theme('table', $header, $rows);
  }
  return '<p>' . t('No transactions at this time.') . '</p>';
}

/**
 * Menu callback, displaying a table, displaying the logs for a transaction.
 */
function epay_admin_transaction_logs($transaction) {
  $header = array(
    t('Status'),
    t('Message'),
    t('Time'),
  );
  $rows = array();
  $result = db_query("SELECT * FROM {epay_transaction_logs} WHERE transaction_id = %d ORDER BY time", $transaction['transaction_id']);
  foreach ($result as $row) {
    $rows[] = array(
      $log->status,
      $log->log_message,
      format_date($log->time, 'short'),
    );
  }
  return l(t('Return to transaction overview'), 'admin/settings/epay/transactions') . theme('table', $header, $rows);
}
