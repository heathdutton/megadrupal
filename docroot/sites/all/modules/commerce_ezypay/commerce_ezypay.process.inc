<?php
/*
 * @file commerce_ezypay.process.inc
 * Processing functions for bulk billing on demand
 * @copyright Copyright(c) 2011 Rowlands Group
 * @license GPL v3 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Lee Rowlands leerowlands at rowlandsgroup dot com
 * 
 */

/**
 * Constants
*/
define('COMMERCE_EZYPAY_ORDER_IX', 4);
define('COMMERCE_EZYPAY_DATE_IX', 3);
define('COMMERCE_EZYPAY_AMOUNT_IX', 7);
define('COMMERCE_EZYPAY_TRANSACTION_ID_IX', 5);
define('COMMERCE_EZYPAY_NOTE_IX', 10);
define('COMMERCE_EZYPAY_ERROR_IX', 9);
define('COMMERCE_EZYPAY_EXCEPTION_ORDER_IX', 2);
define('COMMERCE_EZYPAY_EXCEPTION_DATE_IX', 3);
define('COMMERCE_EZYPAY_EXCEPTION_ERROR_IX', 5);

/**
 * Process the queued transactions
 * @return bool
 *   success/failure
*/
function commerce_ezypay_process() {
  // Setup temporary file
  $temporary_file = 'temporary://'. md5(REQUEST_TIME) .'-ezp-up.csv';
  $transaction_file = fopen($temporary_file, 'w');
  // Get our pending transactions
  $orders = commerce_ezypay_fetch_orders_by_state(COMMERCE_EZYPAY_PENDING);
  // Construct our billing file
  foreach ($orders as $order) {
    $wrapper = entity_metadata_wrapper('commerce_order', $order);
    $parent_order = $wrapper->commerce_recurring_parent_order->value();
    $instance_id = $parent_order->data['payment_method'];
    $payment_method = commerce_payment_method_instance_load($instance_id);
    $settings = $payment_method['settings'];
    $account = user_load($order->uid);
    $cards_on_file = commerce_cardonfile_data_load_multiple($order->uid, $payment_method['instance_id']);
    // There could be multiples, we only need one active card
    $card_on_file = (object)array_pop($cards_on_file);
    if (!$card_on_file) {
      watchdog('commerce_ezypay', 'Could not retrieve card on file for order @id, user @name - payment not submitted to EzyPay',
               array('@name' => $user->name, '@id' => $order->order_id), WATCHDOG_CRITICAL);
      continue;
    }
    $billing_pane_id = 'customer_profile_billing';
    if ($field_name = variable_get('commerce_' . $billing_pane_id . '_field', '')) {
      $profile = $wrapper->{$field_name}->value();
    }
    else {
      // Or try the association stored in the order's data array if no field is set.
      if (!empty($order->data['profiles'][$billing_pane_id])) {
        $profile = commerce_customer_profile_load($order->data['profiles'][$billing_pane_id]);
      }
    }
    if (!$profile) {
      watchdog('commerce_ezypay', 'Could not retrieve profile information for order @id, user @name - payment not submitted to EzyPay',
               array('@name' => $account->name, '@id' => $order->order_id), WATCHDOG_CRITICAL);
      continue;
    }
    else {
      $profile_wrapper = entity_metadata_wrapper('commerce_customer_profile', $profile);
      $address = $profile_wrapper->commerce_customer_address->value();
    }
    $total = $wrapper->commerce_order_total->value();
    $transaction = array(
      'principal_id' => $settings['commerce_ezypay_principal_id'],
      'distribution_account_id' => 0,
      // Ezypay have advised no billing type is needed with the hybrid BOD method
      'billing_type' => '',
      'vendor_id' => $order->order_id,
      // The principal ref is the parent order id - we use this instead of uid
      // because we don't have uid available for anoymous at time of checkout
      'principal_ref' => $parent_order->order_id,
      'first_name' => substr($address['first_name'], 0, 30),
      'last_name' => substr($address['last_name'], 0, 30),
      'address_1' => substr($address['thoroughfare'] . $address['premise'], 0, 30),
      'address_2' => '',
      'suburb' => substr($address['locality'], 0, 30),
      'state' => substr($address['administrative_area'], 0, 3),
      'postcode' => substr($address['postal_code'], 0, 4),
      'home_phone' => '',
      'work_phone' => '',
      'mobile' => '',
      'fax' => '',
      'email' => $account->mail,
      'cc_number' => '',
      'cc_exp' => '',
      'bank' => '',
      'direct_debit_acc' => '',
      'amount' => $total['amount'],
      // Ezypay have advised that they don't need title either
      'title' => '',
      // Ezypay have advised that they use the customer reference, not the guid
      'guid' => '',
      'note' => t('Recurring payment against order @order_id', array('@order_id' => $order->order_id))
    );
    // We could use fputcsv here but Ezypay have advised that they don't support
    // quoted strings in their CSV parser so we have to do this
    foreach ($transaction as $key => $value) {
      if (strpos($value, ',') !== FALSE) {
        // We have to escape a comma
        $transaction[$key] = strtr($value, ',', '');
      }
    }
    // We output our row imploded and add a Windows Style line ending
    fputs($transaction_file, implode(",", $transaction) ."\r\n");
  }
  // close file
  fclose($transaction_file);
  if (count($orders)) {
    // Get our file data
    $file_data = file_get_contents($temporary_file);
    $encrypted_file_data = commerce_ezypay_encrypt($file_data, $settings['commerce_ezypay_debug']);
    // Remove our file
    if (!$settings['commerce_ezypay_debug']) {
      // Whilst not in debug - we remove this file
      unlink($temporary_file);
    }
    $client = new SoapClient('https://ssl.ezypay.com.au/webvi/webviservice.asmx?WSDL', array(
        "trace"      => 1,
        "exceptions" => 0));
    try {
      $response = $client->PutBillingFile(array('username' => $settings['commerce_ezypay_username'],
                                          'password' => $settings['commerce_ezypay_password'],
                                          'prinId' => $settings['commerce_ezypay_principal_id'],
                                          'vendorId' => $settings['commerce_ezypay_vendor_id'],
                                          'region' => $settings['commerce_ezypay_region'],
                                          'contents' => $encrypted_file_data));
    }
    catch(SoapFault $error) {
      return (object) array(
        'errors' => array(
          (object) array(
            'message' => $e->faultString
          )
        )
      );
    }
    if ($settings['commerce_ezypay_debug']) {
      watchdog('commerce_ezypay', '<pre>' . $client->__getLastRequest() . '</pre>', array(), WATCHDOG_DEBUG);
      watchdog('commerce_ezypay', '<pre>' . $client->__getLastResponse() . '</pre>', array(), WATCHDOG_DEBUG);
    }
    if (is_soap_fault($response)) {
      watchdog('commerce_ezypay', 'Could not upload transaction file: error was @error',
               array('@error' => $response->faultstring), WATCHDOG_CRITICAL);
      return FALSE;
    }
    if (!empty($response->PutBillingFileResult) || is_numeric($response)) {
      $code = $response->PutBillingFileResult;
      if (!empty($response->PutBillingFileResult)) {
        $code = $response->PutBillingFileResult;
      }
      $message = commerce_ezypay_soap_error($code);
      $fatals = array(1, 2, 3, 4, 5, 6, 7, 9, 11);
      $status = WATCHDOG_NOTICE;
      if (in_array($code, $fatals)) {
        $status = WATCHDOG_CRITICAL;
      }
      watchdog('commerce_ezypay', 'An error occurred whilst posting transaction file: error was @error',
               array('@error' => $message), $status);
      return FALSE;
    }
    else {
      // Update the processing status of the orders
      db_update('commerce_ezypay_processing')
        ->fields(array('status' => COMMERCE_EZYPAY_SENT))
        ->condition('order_id', array_keys($orders), 'IN')
        ->execute();
      return TRUE;
    }
  }
  // Nothing to process - return TRUE
  return TRUE;
}

/**
 * Queue the due transactions
 * 
 * @param $transaction object
 *   The
 *
 * @return int|bool
 *   int queue id or FALSE on fail
*/
function commerce_ezypay_queue_transaction($order, $parent_order) {
  $record = array(
    'order_id' => $order->order_id,
    'status' => COMMERCE_EZYPAY_PENDING
  );
  $exists = db_query("SELECT order_id FROM {commerce_ezypay_processing} WHERE order_id = :order_id",
                     array(':order_id' => $order->order_id))->fetchField(0);
  $update = array();
  if ($exists) {
    $update = array('order_id');
  }
  drupal_write_record('commerce_ezypay_processing', $record, $update);
  return TRUE;
}

/**
 * Handle result processing
 * @param $processing_dt int
 *   Date of processing
 *
 * @return bool
 *   Success/ Failure
*/
function commerce_ezypay_processing_results($processing_dt) {
  // We make the assumption that the two payment methods are the same
  // @todo check with ezypay if anyone ever has one account for credit card
  //   and another for debit card
  $instance_id = 'ezypay_credit|commerce_payment_ezypay_credit';
  $payment_method = commerce_payment_method_instance_load($instance_id);
  $settings = $payment_method['settings'];
  $return = FALSE;
  // Get our pending transactions
  $client = new SoapClient('https://ssl.ezypay.com.au/webvi/webviservice.asmx?WSDL', array(
      "trace"      => 1,
      "exceptions" => 0));
  try {
    $response = $client->GetTransactionFile(array('username' => $settings['commerce_ezypay_username'],
                                        'password' => $settings['commerce_ezypay_password'],
                                        'prinId' => $settings['commerce_ezypay_principal_id'],
                                        'vendorId' => $settings['commerce_ezypay_vendor_id'],
                                        'region' => $settings['commerce_ezypay_region'],
                                        'processingDate' => format_date($processing_dt, 'custom', 'Ymd')));
  }
  catch(SoapFault $error) {
    return (object) array(
      'errors' => array(
        (object) array(
          'message' => $e->faultString
        )
      )
    );
  }
  if ($settings['commerce_ezypay_debug']) {
    watchdog('commerce_ezypay', '<pre>' . $client->__getLastRequest() . '</pre>', array(), WATCHDOG_DEBUG);
    watchdog('commerce_ezypay', '<pre>' . $client->__getLastResponse() . '</pre>', array(), WATCHDOG_DEBUG);
  }
  if (is_soap_fault($response)) {
    watchdog('commerce_ezypay', 'Could not fetch transaction file: error was @error',
             array('@error' => $response->faultstring), WATCHDOG_CRITICAL);
    return FALSE;
  }
  if (!empty($response->GetTransactionFileResult->ErrorId) ||
      !empty($response->GetFileResponse->ErrorId)) {
    $error_code = !empty($response->GetTransactionFileResult->ErrorId) ? $response->GetTransactionFileResult->ErrorId : $response->GetFileResponse->ErrorId;
    $message = commerce_ezypay_soap_error($error_code);
    $fatals = array(1, 2, 3, 4, 5, 6, 7, 9, 11);
    $status = WATCHDOG_NOTICE;
    if (in_array($error_code, $fatals)) {
      $status = WATCHDOG_ERROR;
    }
    watchdog('commerce_ezypay', 'An error occurred whilst fetching transaction file: error was @error',
             array('@error' => $message), $status);
    return FALSE;
  }
  else {
    // Success - we now decode and process the results
    $orders = array();
    // Write our temporary file
    $contents = $response->GetTransactionFileResult->Contents;
    $temporary_file = 'temporary://'. md5(REQUEST_TIME) .'-ezp-down.csv';
    file_put_contents($temporary_file, $contents);
    if ($settings['commerce_ezypay_debug']) {
      watchdog('commerce_ezypay', '<pre>' . $contents . '</pre>', array(), WATCHDOG_DEBUG);
    }
    // Process the file
    $transaction_file = fopen($temporary_file, 'r');
    if ($transaction_file) {
      while (($row = fgetcsv($transaction_file)) !== FALSE) {
        // Check for uniqueness
        if (!commerce_ezypay_is_unique($row[COMMERCE_EZYPAY_TRANSACTION_ID_IX], $instance_id)) {
          continue;
        }
        $order_id = $row[COMMERCE_EZYPAY_ORDER_IX];
        $amount = $row[COMMERCE_EZYPAY_AMOUNT_IX];
        $order = commerce_order_load($order_id);
        // Record the payment
        $transaction = commerce_payment_transaction_new('ezypay_credit', $order->order_id);
        $transaction->instance_id = $payment_method['instance_id'];
        $transaction->amount = $row[COMMERCE_EZYPAY_AMOUNT_IX];
        $transaction->created = strtotime($row[COMMERCE_EZYPAY_DATE_IX]);
        // Currency is AUD or NZD
        $transaction->currency_code = $settings['commerce_ezypay_region'] . 'D'; 
        $transaction->payload[REQUEST_TIME] = $row;
        $transaction->remote_id = $row[COMMERCE_EZYPAY_TRANSACTION_ID_IX];
        $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
        $transaction->message = $row[COMMERCE_EZYPAY_NOTE_IX];
        if (!empty($row[COMMERCE_EZYPAY_ERROR_IX])) {
          $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
          $transaction->message = $row[COMMERCE_EZYPAY_ERROR_IX];
        }
        else {
          // Append to finalised list
          $orders[] = $order_id;
        }
        commerce_payment_transaction_save($transaction);
      }
    }
    fclose($transaction_file);
    // Remove the file
    if (!$settings['commerce_ezypay_debug']) {
      // Whilst not in debug - we remove this file
      unlink($temporary_file);
    }
    if (!empty($orders)) {
      // Update the processing status of the orders
      db_update('commerce_ezypay_processing')
        ->fields(array('status' => COMMERCE_EZYPAY_PROCESSED))
        ->condition('order_id', array_keys($orders), 'IN')
        ->execute();
      $return = TRUE;
    }
  }
  /**
   * Exception processing
  */
  // Get our transaction errors
  $client = new SoapClient('https://ssl.ezypay.com.au/webvi/webviservice.asmx?WSDL', array(
      "trace"      => 1,
      "exceptions" => 0));
  try {
    $response = $client->GetExceptionFile(array('username' => $settings['commerce_ezypay_username'],
                                        'password' => $settings['commerce_ezypay_password'],
                                        'prinId' => $settings['commerce_ezypay_principal_id'],
                                        'vendorId' => $settings['commerce_ezypay_vendor_id'],
                                        'region' => $settings['commerce_ezypay_region'],
                                        'processingDate' => format_date($processing_dt, 'custom', 'Ymd')));
  }
  catch(SoapFault $error) {
    return (object) array(
      'errors' => array(
        (object) array(
          'message' => $e->faultString
        )
      )
    );
  }
  if ($settings['commerce_ezypay_debug']) {
    watchdog('commerce_ezypay', '<pre>' . $client->__getLastRequest() . '</pre>', array(), WATCHDOG_DEBUG);
    watchdog('commerce_ezypay', '<pre>' . $client->__getLastResponse() . '</pre>', array(), WATCHDOG_DEBUG);
  }
  if (is_soap_fault($response)) {
    watchdog('commerce_ezypay', 'Could not fetch exception file: error was @error',
             array('@error' => $response->faultstring), WATCHDOG_CRITICAL);
    return FALSE;
  }
  if (!empty($response->GetExceptionFileResult->ErrorId) ||
      !empty($response->GetFileResponse->ErrorId)) {
    $error_code = !empty($response->GetExceptionFileResult->ErrorId) ? $response->GetTransactionFileResult->ErrorId : $response->GetFileResponse->ErrorId;
    $message = commerce_ezypay_soap_error($error_code);
    $fatals = array(1, 2, 3, 4, 5, 6, 7, 9, 11);
    $status = WATCHDOG_NOTICE;
    if (in_array($error_code, $fatals)) {
      $status = WATCHDOG_ERROR;
    }
    watchdog('commerce_ezypay', 'An error occurred whilst fetching exception file: error was @error',
             array('@error' => $message), $status);
    return FALSE;
  }
  else {
    // Exception file successfully fetched - we now decode and process results
    // Write our temporary file
    $contents = $response->GetExceptionFileResult->GetFileResponse->Contents;
    $temporary_file = 'temporary://'. md5(REQUEST_TIME) .'-ezp-down.csv';
    file_put_contents($temporary_file, $contents);
    if ($settings['commerce_ezypay_debug']) {
      watchdog('commerce_ezypay', '<pre>' . $contents . '</pre>', array(), WATCHDOG_DEBUG);
    }
    // Process the file
    $transaction_file = fopen($temporary_file, 'r');
    if ($transaction_file) {
      while (($row = fgetcsv($transaction_file)) !== FALSE) {
        $remote_id = $row[COMMERCE_EZYPAY_EXCEPTION_ORDER_IX] . '-' . strtotime($row[COMMERCE_EZYPAY_EXCEPTION_DATE_IX]);
        if (!commerce_ezypay_is_unique($remote_id, $instance_id)) {
          continue;
        }
        $order_id = $row[COMMERCE_EZYPAY_EXCEPTION_ORDER_IX];
        $amount = 0;
        $order = commerce_order_load($order_id);
        // Record a dummy payment
        $transaction = commerce_payment_transaction_new('ezypay_credit', $order->order_id);
        $transaction->instance_id = $payment_method['instance_id'];
        $transaction->amount = 0;
        $transaction->created = strtotime($row[COMMERCE_EZYPAY_EXCEPTION_DATE_IX]);
        // Currency is AUD or NZD
        $transaction->currency_code = $settings['commerce_ezypay_region'] . 'D'; 
        $transaction->payload[REQUEST_TIME] = $row;
        // Create a unique id
        $transaction->remote_id = $remote_id;
        // These are exceptions
        $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
        $transaction->message = $row[COMMERCE_EZYPAY_EXCEPTION_ERROR_IX];
        commerce_payment_transaction_save($transaction);
      }
    }
    fclose($transaction_file);
    // Remove the file
    if (!$settings['commerce_ezypay_debug']) {
      // Whilst not in debug - we remove this file
      unlink($temporary_file);
    }
    // Status of exceptions from orders does not influence return
    return $return;
  }
}

/**
 * Util to fetch all processing orders of a given state
 * @param $state int
 *   Processing state @see commerce_ezypay_processing_states
 *
 * @return array
 *   Array of commerce order objects keyed by order id
*/
function commerce_ezypay_fetch_orders_by_state($state) {
  $order_ids = array();
  module_load_include('inc', 'commerce_ezypay', 'commerce_ezypay.soap');
  $query = db_select('commerce_ezypay_processing', 'cep')
    ->fields('cep', array('order_id'))
    ->condition('cep.status', $state);
  $result = $query->execute();
  foreach ($result as $row) {
    $order_ids[] = $row->order_id;
  }
  return commerce_order_load_multiple($order_ids);
}

/**
 * Util to check uniqueness of a remote id
 * @param $id mixed
 *   Remote transaction id
 * @param $instance_id string
 *   Payment method instance id
 *
 * @return bool
 *   TRUE if unique
*/
function commerce_ezypay_is_unique($id, $instance_id) {
  return !db_select('commerce_payment_transaction', 'cpt')
          ->condition('cpt.remote_id', $id)
          ->condition('cpt.instance_id', $instance_id)
          ->countQuery()
          ->execute()
          ->fetchField();
}

/**
 * Util that provides wrapper to result processing error messages
 * @param $code int
 *   Error code returned from SoapService
 *
 * @return string
*/
function commerce_ezypay_soap_error($code) {
  $message = t('An unspecified error occured');
  $messages[] = t('Success');
  $messages[] = t('Invalid principal id');
  $messages[] = t('Invalid vendor id');
  $messages[] = t('Invalid region');
  $messages[] = t('Invalid contents');
  $messages[] = t('Invalid processing date');
  $messages[] = t('Invalid hash');
  $messages[] = t('Billing file already exists');
  $messages[] = t('No matching processing file for this date');
  $messages[] = t('Multiple matching processing files exist - contact EzyPay');
  $messages[] = t('No matching exception file for this date');
  $messages[] = t('Multiple matching exception files exist - contact EzyPay');
  $messages[] = t('Invalid username/password');
  $messages[] = t('Internal server error at Ezypay end - no action required');
  
  if (isset($messages[$code])) {
    return $messages[$code];
  }
  else {
    watchdog('commerce_ezypay', 'An error occurred whilst fetching transaction file and the error code reported is not known: error code was @error',
             array('@error' => $code), $status);
  }
  return $message;
}


/**
 * Util to encrypt the file
 * @param $file_data string
 *   file data
 * @param $debug bool
 *   whether to log debug output
 *
 * @return string
 *   encrypted file
*/
function commerce_ezypay_encrypt($file_data, $debug = FALSE) {
  $home_dir = variable_get('commerce_ezypay_gpg_path', '/var/gpg');
  $command = escapeshellcmd("echo \"$file_data\" ") .'|'. escapeshellcmd("gpg -e --homedir $home_dir --recipient IThelpdesk@ezypay.com.au --armour");
  $output = shell_exec($command);
  if ($debug) {
    $debug_path = 'temporary://'. md5(REQUEST_TIME) .'-ezp-enc.csv';
    $output_file = fopen($debug_path, 'w');
    fputs($output_file, $output ."\r\n");
    fclose($output_file);
    watchdog('commerce_ezypay', '<pre>' . $output . '</pre>', array(), WATCHDOG_DEBUG);
  }
  return $output;
}

