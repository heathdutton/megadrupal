<?php
/**
 * @file
 * This script is used for real time capturing of
 * parameters passed from Novalnet AG after Payment
 * processing of customers.
 *
 * Copyright (c) Novalnet AG
 *
 * This script is only free to the use for Merchants of
 * Novalnet AG
 *
 * If you have found this script useful a small
 * recommendation as well as a comment on merchant form
 * would be greatly appreciated.
 *
 * Version : 10.0.0
 *
 * Please contact sales@novalnet.de for enquiry or Info
 *
 */
module_load_include('inc', 'commerce_novalnet', 'includes/commerce_novalnet_core');
global $_array_subscription, $_array_payments, $_array_chargebacks, $_array_collection, $_process_testmode, $_process_debugmode, $_array_capture_params, $_array_payment_groups, $_configuration;

/** @Array Type of payment available - Level : 0 */
$_array_payments = array('CREDITCARD', 'INVOICE_START', 'DIRECT_DEBIT_SEPA', 'GUARANTEED_INVOICE_START', 'PAYPAL', 'ONLINE_TRANSFER', 'IDEAL', 'EPS', 'PAYSAFECARD');

/** @Array Type of Chargebacks available - Level : 1 */
$_array_chargebacks = array('RETURN_DEBIT_SEPA', 'CREDITCARD_BOOKBACK', 'CREDITCARD_CHARGEBACK', 'REFUND_BY_BANK_TRANSFER_EU');

/** @Array Type of CreditEntry payment and Collections available - Level : 2 */
$_array_collection = array('INVOICE_CREDIT', 'GUARANTEED_INVOICE_CREDIT', 'CREDIT_ENTRY_CREDITCARD', 'CREDIT_ENTRY_SEPA', 'DEBT_COLLECTION_SEPA', 'DEBT_COLLECTION_CREDITCARD');

$_array_subscription = array('SUBSCRIPTION_STOP');

$_array_payment_groups = array(
    'commerce_novalnet_cc'          => array('CREDITCARD', 'CREDITCARD_BOOKBACK', 'CREDITCARD_CHARGEBACK', 'CREDIT_ENTRY_CREDITCARD', 'SUBSCRIPTION_STOP', 'DEBT_COLLECTION_CREDITCARD'),
    'commerce_novalnet_sepa'        => array('DIRECT_DEBIT_SEPA', 'RETURN_DEBIT_SEPA', 'SUBSCRIPTION_STOP', 'DEBT_COLLECTION_SEPA', 'CREDIT_ENTRY_SEPA'),
    'commerce_novalnet_ideal'       => array('IDEAL'),
    'commerce_novalnet_instant'     => array('ONLINE_TRANSFER'),
    'commerce_novalnet_paypal'      => array('PAYPAL'),
    'commerce_novalnet_prepayment'  => array('INVOICE_START', 'INVOICE_CREDIT', 'SUBSCRIPTION_STOP'),
    'commerce_novalnet_invoice'     => array('INVOICE_START', 'INVOICE_CREDIT', 'SUBSCRIPTION_STOP'),
    'commerce_novalnet_eps'         => array('EPS'),
);
if (is_array($_REQUEST)) {
    $_array_capture_params = array_map('trim', $_REQUEST);
    $_array_capture_params = array_map('check_plain', $_REQUEST);
}
$_configuration      = commerce_novalnet_load_settings();
$_process_testmode   = $_configuration['test_mode'];
$_process_debugmode  = $_configuration['debug_mode'];
if (isset($_array_capture_params['debug_mode']) && $_array_capture_params['debug_mode']) {
    $_process_testmode   = TRUE;
    $_process_debugmode  = TRUE;
}

/**
 * Process the callback request
 *
 * @param none
 * @return none
 */
function commerce_novalnet_callback_process() {
    global $_array_capture_params, $_configuration;

    commerce_novalnet_callback_validate_ip();
    commerce_novalnet_callback_validate_capture_params(commerce_novalnet_get_mandatory_params());
    if (isset($_array_capture_params['vendor_activation']) && $_array_capture_params['vendor_activation'] == 1)
        commerce_novalnet_callback_add_affiliate_account();

    list($nn_transaction_details, $order, $trxn_log) = commerce_novalnet_callback_order_details();
    $payment_level = commerce_novalnet_callback_paymenttype_level();
    if ($_array_capture_params['status'] == NOVALNET_CALLBACK_PROCESS_STATUS) {
        if ($payment_level == 2) {
            if ($_array_capture_params['payment_type'] == 'INVOICE_CREDIT') {
                if ($nn_transaction_details['order_paid_amount'] < $nn_transaction_details['order_total_amount']) {
                    $db_comments = $callback_comments = 'Novalnet Callback Script executed successfully for the TID: ' . $_array_capture_params['shop_tid'] . ' with amount:' . number_format($_array_capture_params['amount']/100, 2, ',', '') . ' ' . $_array_capture_params['currency'] . ' on ' . date('m/d/Y H:i:s') . '. Please refer PAID transaction in our Novalnet Merchant Administration with the TID:' . $_array_capture_params['tid'];
                    $curr_amount = $nn_transaction_details['order_paid_amount'] + $_array_capture_params['amount'];
                    if ($nn_transaction_details['order_total_amount'] <= $curr_amount) {
                        #Full Payment paid
                        $callback_status_id = (!empty($nn_transaction_details['callback_script_status'])) ? $nn_transaction_details['callback_script_status'] :'';
                        if ($curr_amount > $nn_transaction_details['order_total_amount']) {
                            $callback_comments .= '<br> Customer has paid more than the Order amount.';
                        }
                    }
                    else {
                        #Partial Payment paid
                        $callback_status_id = $nn_transaction_details['order_current_status'];
                    }
                    commerce_novalnet_callback_create_transaction($order, $_array_capture_params['amount'], $db_comments, $_array_capture_params['shop_tid']);
                    commerce_order_status_update($order, $callback_status_id, FALSE, TRUE);
                    commerce_novalnet_callback_send_notification_mail(array('order_no' => $order->order_number,
                                                                            'message'  => $callback_comments));
                    commerce_novalnet_callback_insert_success_log(array(
                        'payment_type'  => $nn_transaction_details['payment_type'],
                        'status'                => $_array_capture_params['status'],
                        'callback_tid'  => $_array_capture_params['tid'],
                        'org_tid'           => $_array_capture_params['shop_tid'],
                        'amount'            => $_array_capture_params['amount'],
                        'order_no'          => $order->order_number,
                    ));
                     commerce_novalnet_debug_error($callback_comments);
                }
                commerce_novalnet_debug_error('Novalnet callback received. Callback Script executed already. Refer Order :' . $order->order_number);
            }
            //Subscription renewal of level 0 payments
            if ($_array_capture_params['subs_billing'] == 1) {
                #### Step1: THE SUBSCRIPTION IS RENEWED, PAYMENT IS MADE, SO JUST CREATE A NEW ORDER HERE WITHOUT A PAYMENT PROCESS AND SET THE ORDER STATUS AS PAID ####

                #### Step2: THIS IS OPTIONAL: UPDATE THE BOOKING REFERENCE AT NOVALNET WITH YOUR ORDER_NO BY CALLING NOVALNET GATEWAY, IF U WANT THE USER TO USE ORDER_NO AS PAYMENT REFERENCE ###

                #### Step3: ADJUST THE NEW ORDER CONFIRMATION EMAIL TO INFORM THE USER THAT THIS ORDER IS MADE ON SUBSCRIPTION RENEWAL ###
            }
            $error = 'Payment type ( ' . $_array_capture_params['payment_type'] . ' ) is not applicable for this process!';
            commerce_novalnet_debug_error($error);

        }
        elseif ($payment_level == 1) {
            ### DO THE STEPS TO UPDATE THE STATUS OF THE ORDER OR THE USER AND NOTE THAT THE PAYMENT WAS RECLAIMED FROM USER ###

            $callback_comments = 'Novalnet callback received. Chargeback was executed for the TID:' . $_array_capture_params['tid_payment'] . ' amount:' . number_format($_array_capture_params['amount']/100, 2, ',', '') . ' ' . $_array_capture_params['currency'] . ' on ' . date("m/d/Y H:i:s") . 'The subsequent TID: ' . $_array_capture_params['tid'];

            commerce_novalnet_callback_send_notification_mail(array('order_no' => $order->order_number,
                                                                    'message'  => $callback_comments));
            commerce_novalnet_callback_create_transaction($order, '0', $callback_comments, $_array_capture_params['shop_tid']);
            commerce_novalnet_debug_error($callback_comments);
        }
        elseif ($payment_level === 0) {
            if ($_array_capture_params['subs_billing'] == 1) {
                ##IF PAYMENT MADE ON SUBSCRIPTION RENEWAL ###
                $transaction_status = commerce_novalnet_get_trxn_details($trxn_log, $_array_capture_params['shop_tid']);

                //Update callback order status due to full payment
                $callback_comments = 'Novalnet Callback Script executed successfully for the Subscription TID: ' . $_array_capture_params['shop_tid'] . ' with amount:' . number_format($_array_capture_params['amount']/100, 2, ',', '') . ' ' . $_array_capture_params['currency'] . ' on ' . date('m/d/Y H:i:s') . '. Please refer PAID transaction in our Novalnet Merchant Administration with the TID:' . $_array_capture_params['tid'];
                commerce_novalnet_callback_send_notification_mail(array('order_no' => $order->order_number,
                                                                         'message'  => $callback_comments));
                $new_order          = commerce_novalnet_callback_create_order($order);
                $trxn_comments      = commerce_novalnet_callback_trxn_comments($new_order, $transaction_status['next_subs_cycle']);
                $trxn_comments = $trxn_comments . '<br>' . $callback_comments;
                $amount = $_array_capture_params['payment_type'] == 'INVOICE_CREDIT' ? $_array_capture_params['amount']:'0';
                commerce_novalnet_callback_create_transaction($new_order, $amount, $trxn_comments, $_array_capture_params['tid']);
                commerce_novalnet_callback_add_new_trxn_details($new_order, $transaction_status, $trxn_log);
                if (!empty($transaction_status['next_subs_cycle']))
                    $callback_comments .= '<br>Next Payment Date is: ' . $transaction_status['next_subs_cycle'];
                    commerce_novalnet_debug_error($callback_comments);
                }
                elseif ($_array_capture_params['payment_type'] == 'PAYPAL') {
                    if ($nn_transaction_details['order_paid_amount'] < $nn_transaction_details['order_total_amount']) {
                        ### PayPal payment success ###

                        $callback_comments = 'Novalnet Callback Script executed successfully for the TID: ' . $_array_capture_params['shop_tid'] . ' with amount:' . number_format($_array_capture_params['amount']/100, 2, ',', '') . ' ' . $_array_capture_params['currency'] . ' on ' . date('m/d/Y H:i:s') . ' Please refer PAID transaction in our Novalnet Merchant Administration with the TID:' . $_array_capture_params['tid'];
                        commerce_novalnet_callback_create_transaction($order, $_array_capture_params['amount'], $callback_comments, $_array_capture_params['shop_tid']);
                        commerce_order_status_update($order, $_configuration['commerce_novalnet_paypal_order_status'], FALSE, TRUE);
                        commerce_novalnet_callback_send_notification_mail(array('order_no' => $order->order_number,
                                                                               'message'  => $callback_comments));
                        commerce_novalnet_callback_insert_success_log(array(
                            'payment_type'  => $nn_transaction_details['payment_type'],
                            'status'        => $_array_capture_params['status'],
                            'callback_tid'  => $_array_capture_params['tid'],
                            'org_tid'       => $_array_capture_params['shop_tid'],
                            'amount'        => $_array_capture_params['amount'],
                            'order_no'      => $order->order_number,
                        ));
                     commerce_novalnet_debug_error($callback_comments);
                }
                commerce_novalnet_debug_error('Novalnet Callbackscript received. Order already Paid');
            }
            else {
                commerce_novalnet_debug_error('Novalnet Callbackscript received. Payment type ( ' . $_array_capture_params['payment_type'] . ' ) is not applicable for this process!');
            }
        }
        if ($_array_capture_params['payment_type'] == 'SUBSCRIPTION_STOP') {
            $subs_details = commerce_novalnet_get_subscription($_array_capture_params['signup_tid']);

            if (empty($subs_details['termination_at'])) {
            commerce_novalnet_update_subscription(array(
                'termination_reason' => (isset($_array_capture_params['termination_reason']) && $_array_capture_params['termination_reason']) ? $_array_capture_params['termination_reason'] : 'other',
                'termination_at' => time()), $_array_capture_params['signup_tid']);
            $callback_comments = 'Novalnet Callbackscript received. Subscription has been stopped for the TID: ' . $_array_capture_params['shop_tid'] . ' on ' . date('m/d/Y H:i:s');
            $callback_comments .= '<br>Reason for Cancellation: ' . $_array_capture_params['termination_reason'];

            commerce_novalnet_callback_create_transaction($order, '0', $callback_comments, $_array_capture_params['signup_tid']);
            commerce_order_status_update($order, $_configuration['subs_cancel_status'], FALSE, TRUE);
            commerce_novalnet_callback_send_notification_mail(array('order_no' => $order->order_number,
                                                        'message'  => $callback_comments));
            commerce_novalnet_debug_error($callback_comments);
            }
            else {
                commerce_novalnet_debug_error('Novalnet callbackscript received. Subscription has been canceled already.');
            }
        }
    }
}

/**
 * update data to subscription table
 *
 * @param $data array
 * @param $tid double
 * @return none
 */
function commerce_novalnet_update_subscription($data, $tid) {
    $query = db_update('commerce_novalnet_subscription_detail')
            ->fields($data)
            ->condition('tid', $tid)
            ->execute();
}
/**
 * update data to subscription table
 *
 * @param $data array
 * @param $tid double
 * @return none
 */
function commerce_novalnet_get_subscription($tid) {

    $query = db_select('commerce_novalnet_subscription_detail', 'subs')
            ->fields('subs', array('termination_at'))
            ->condition('tid', $tid)
            ->execute();
    if ($query) return $query->fetchAssoc();
    return FALSE;
}

/**
 * Create a new order on the recurring process
 *
 * @param $order object
 * @return object
 */
function commerce_novalnet_callback_create_order($order) {
    global $_configuration;

    $payment        = explode('|', $order->data['payment_method']);
    $order_status   = $_configuration[$payment[0] . '_order_status'];
    $order->order_id = $order->order_number = $order->revision_id  = '';
    commerce_order_save($order);
    commerce_order_status_update($order, $order_status, FALSE, TRUE);
    return $order;
}

/**
 * Create a new order on the recurring process
 *
 * @param $new_order object
 * @param $transaction_status object
 * @param $log array
 * @return none
 */
function commerce_novalnet_callback_add_new_trxn_details($new_order, $transaction_status, $log) {
    global $_array_capture_params;

    if (!is_array($transaction_status)) $transaction_status = (array)$transaction_status;
    $_array_capture_params['amount'] = $_array_capture_params['amount']*100;
    $currency = (isset($_array_capture_params['currency']) && $_array_capture_params['currency'])?$_array_capture_params['currency']:$log['currency'];
    commerce_novalnet_log_initial_transaction(array(
        'tid'               => $_array_capture_params['tid'],
        'vendor'            => $log['vendor_id'],
        'auth_code'         => $log['authcode'],
        'tariff_id'         => $log['tariff_id'],
        'product_id'        => $log['product_id'],
        'subs_id'           => $log['subs_id'],
        'payment_id'        => $log['payment_id'],
        'payment_type'      => $log['payment_type'],
        'amount'            => $_array_capture_params['amount'],
        'currency'          => $currency,
        'status'            => $_array_capture_params['status'],
        'order_no'          => $new_order->order_number,
        'test_mode'         => $log['test_mode'],
        'account_holder'    => $log['account_holder'],
        'customer_id'       => $log['customer_id'],
        'process_key'       => $log['process_key'],
        'gateway_status'    => $transaction_status['status'],
        'org_total'         => $_array_capture_params['amount'],
        'refunded_amount'   => 0,
        'update_date'       => time(),
    ));

    if ($log['payment_id'] == 27) {
        commerce_novalnet_log_invoice_trxn_account_info( array(
            'order_no'          => $new_order->order_number,
            'tid'               => $_array_capture_params['tid'],
            'account_holder'    => 'Novalnet AG',
            'account_number'    => isset($_array_capture_params['invoice_account']) ? $_array_capture_params['invoice_account'] : '',
            'bank_code'         => isset($_array_capture_params['invoice_bankcode']) ? $_array_capture_params['invoice_bankcode'] : '',
            'bank_name'         => isset($_array_capture_params['invoice_bankname']) ? $_array_capture_params['invoice_bankname'] : '',
            'bank_city'         => isset($_array_capture_params['invoice_bankplace']) ? $_array_capture_params['invoice_bankplace'] : '',
            'amount'            => $_array_capture_params['amount']*100,
            'currency'          => $currency,
            'bank_iban'         => isset($_array_capture_params['invoice_iban']) ? $_array_capture_params['invoice_iban'] : '',
            'bank_bic'          => isset($_array_capture_params['invoice_bic']) ? $_array_capture_params['invoice_bic'] : '',
            'due_date'          => isset($_array_capture_params['due_date']) ? date('m/d/Y', strtotime($_array_capture_params['due_date'])): date('Y-m-d', strtotime('+14 days')),
            'update_date'       => time(),
            'test_mode'         => $_array_capture_params['test_mode']
        ));
    }
    $_array_capture_params['amount'] = $_array_capture_params['amount']/100;
}

/**
 * get the transaction details from server
 *
 * @param $trxn_log array
 * @param $tid double
 * @return object
 */
function commerce_novalnet_get_trxn_details($trxn_log, $tid) {
    return commerce_novalnet_do_xml_request(array(
            'vendor'        => $trxn_log['vendor_id'],
            'auth_code'     => $trxn_log['authcode'],
            'product'       => $trxn_log['product_id'],
            'tid'           => $tid), 'TRANSACTION_STATUS');
}

/**
 * create a payment transaction for the order
 *
 * @param $order object
 * @param $amount float
 * @param $comments string
 * @param $tid double
 * @return none
 */
function commerce_novalnet_callback_create_transaction($order, $amount, $comments, $tid) {
    $payment = explode('|', $order->data['payment_method']);
    $transaction = commerce_payment_transaction_new($payment['0'], $order->order_id);
    $transaction->instance_id           = $order->data['payment_method'];
    $transaction->payment_method        = $payment['0'];
    $transaction->amount                = $amount;
    $transaction->currency_code         = $order->commerce_order_total['und'][0]['currency_code'];
    $transaction->status                = COMMERCE_PAYMENT_STATUS_SUCCESS;
    $transaction->message_variables     = array('@name' => $payment['0']);
    $transaction->message               = $comments;
    $transaction->remote_id             = $tid;
    commerce_payment_transaction_save($transaction);
}

/**
 * send notification mail to the configured mail id on successful execution
 *
 * @param $order object
 * @param $status string
 * @return none
 */
function commerce_novalnet_callback_send_notification_mail($data) {
    global $_configuration, $_array_capture_params, $language;

    if (!empty($data['order_no']))
        $data['message'] = 'Order number:' . $data['order_no'] . '<br>' . $data['message'];
    $params['subject'] = "Novalnet callback access report";
    $params['body'] = $data['message'];
    $from = variable_get('site_mail', '');
    $to_mail = str_replace(' ', '', $_configuration['callback_notify_to']);
    $bcc_mail = str_replace(' ', '', $_configuration['callback_notify_bcc']);
    if (isset($_array_capture_params['debug_mode']) && $_array_capture_params['debug_mode']) {
        $_configuration['callback_notify_bcc'] = '';
        $to_mail = 'test@novalnet.de';
        $bcc_mail = '';
    }
    if ($_configuration['send_mail'] && !empty($to_mail) && $from) {
        if (!empty($bcc_mail)) $params['headers'] = array('bcc' => $bcc_mail);
        $message = drupal_mail('commerce_novalnet', 'novalnet_callback', $to_mail, $language->language, $params, $from, TRUE);
        echo empty($message['result']) ? 'mail not sent <br>' : 'mail sent <br>';
    return TRUE;
    }
        echo 'mail not sent <br>';
    return FALSE;
}

/**
 * insert the callback process data to the table
 *
 * @param $data array
 * @return none
 */
function commerce_novalnet_callback_insert_success_log($data) {

      $data['callback_date'] = time();
      $query = db_insert('commerce_novalnet_callback_history')
                                ->fields(array_keys($data))
                                ->values($data)
                                ->execute();
}

/**
 * Load the order details for the processed transaction ID
 *
 * @param none
 * @return array
 */
function commerce_novalnet_callback_order_details() {
    global $_array_capture_params, $_array_payment_groups, $_configuration;

    $nn_trxn = commerce_novalnet_callback_load_trxn($_array_capture_params['shop_tid']);
    if (isset($nn_trxn['order_no'])) {
        $order = commerce_order_load_by_number($nn_trxn['order_no']);

        $db_val['order_current_status'] = $order->status;
        $db_val['callback_amount'] = $_array_capture_params['amount'];
        if (in_array($nn_trxn['payment_type'], array('commerce_novalnet_prepayment', 'commerce_novalnet_invoice')))
          $db_val['callback_script_status'] = $_configuration[$nn_trxn['payment_type'] . '_callback_status'];

        $db_val['subscription_cancel_status'] = $_configuration['subs_cancel_status'];
        $db_val['order_total_amount']               = $nn_trxn['org_total'];
        $db_val['order_paid_amount']                = 0;
        $db_val['payment_type']                     = $nn_trxn['payment_type'];

        if (in_array(commerce_novalnet_callback_paymenttype_level(), array(0, 2))) {
                $paid_amount = commerce_novalnet_callback_get_paidamount($_array_capture_params['shop_tid']);
        $db_val['order_paid_amount'] = $paid_amount['paid_amount'];
        }
        if (!in_array($_array_capture_params['payment_type'], $_array_payment_groups[$nn_trxn['payment_type']])) {
          commerce_novalnet_debug_error('Novalnet callback received. Payment Type [' . $_array_capture_params['payment_type'] . '] is not valid.');
        }
        if (!empty($_array_capture_params['order_no']) && $_array_capture_params['order_no'] != $nn_trxn['order_no']) {
          commerce_novalnet_debug_error('Novalnet callback received. Order Number is not valid.');
        }
        return array($db_val, $order, $nn_trxn);
    }
     $order_no = !empty($_array_capture_params['order_no']) ? $_array_capture_params['order_no']: '';
    if ($order_no && $order = commerce_order_load_by_number($order_no)) {

        $payment = explode('|', $order->data['payment_method']);
        if (!in_array($_array_capture_params['payment_type'], $_array_payment_groups[$payment[0]])) {
          commerce_novalnet_debug_error('Novalnet callback received. Payment Type [' . $_array_capture_params['payment_type'] . '] is not valid.');
        }
        $orderstatus = $_configuration[$payment[0] . '_order_status'];
        $trans_comments = (isset($_array_capture_params['test_mode']) && $_array_capture_params['test_mode']) ? t('Test order') . '<br>': '';
        $trans_comments .= t('Novalnet transaction ID:@tid', array('@tid' => $_array_capture_params['tid']));
        $_SESSION['novalnet_trxn_comments'] = $trans_comments;
        commerce_checkout_complete($order);
        commerce_order_save($order);
        commerce_order_status_update($order, $orderstatus, FALSE, TRUE, t('Order state updated'));
        commerce_novalnet_callback_create_transaction($order, $_array_capture_params['amount'], $trans_comments, $_array_capture_params['tid']);
        commerce_novalnet_log_initial_transaction(array(
            'tid'               => $_array_capture_params['tid'],
            'vendor'            => $_configuration['vendor_id'],
            'auth_code'         => $_configuration['auth_code'],
            'tariff_id'         => $_configuration['tariff_id'],
            'product_id'        => $_configuration['product_id'],
            'subs_id'           => '',
            'payment_id'        => commerce_novalnet_set_payment_method_info($payment['0'], 'key'),
            'payment_type'      => $payment['0'],
            'amount'            => $_array_capture_params['amount'],
            'currency'          => $_array_capture_params['currency'],
            'status'            => $_array_capture_params['status'],
            'order_no'          => $order->order_number,
            'test_mode'         => $_array_capture_params['test_mode'],
            'account_holder'    => '',
            'customer_id'       => $_array_capture_params['customer_no'],
            'process_key'       => '',
            'gateway_status'    => $_array_capture_params['status'],
            'org_total'         => $_array_capture_params['amount'],
            'refunded_amount'   => 0,
            'update_date'       => time()
        ));
        commerce_novalnet_debug_error("Novalnet Callback script received.<br>" . $trans_comments);

    }
        commerce_novalnet_debug_error('Novalnet Callback script received. Transaction mapping failed');

}

/**
 * Load the order details for the processed transaction ID
 *
 * @param none
 * @return array
 */
function commerce_novalnet_callback_validate_ip() {
    global $_process_testmode;
    if (!$_process_testmode && !in_array(ip_address(), unserialize(NOVALNET_CALLBACK_ALLOWED_IP))) {

        commerce_novalnet_debug_error('Novalnet callback received. Unauthorised access from the IP [' . ip_address() . ']');
    }

}

/**
 * Load the order details for the processed transaction ID
 *
 * @param none
 * @return array
 */
function commerce_novalnet_callback_validate_capture_params($params_required) {
        global $_array_capture_params, $_array_subscription, $_array_payments, $_array_chargebacks, $_array_collection;
        $error = '';
        if (empty($_array_capture_params)) {
            commerce_novalnet_debug_error('Novalnet callback received. No params passed over!');
        }
        foreach ($params_required as $k => $v) {
        if (!isset($_array_capture_params[$v]) || empty($_array_capture_params[$v])) {
          $error .= 'Required param ( ' . $v . '  ) missing!<br>';
        }
      }
        if ($error) commerce_novalnet_debug_error($error);
        if (isset($_array_capture_params['vendor_activation']) && $_array_capture_params['vendor_activation'] == 1) return TRUE;
        if (!in_array($_array_capture_params['payment_type'],
                array_merge($_array_subscription, $_array_payments, $_array_chargebacks, $_array_collection))) {
            commerce_novalnet_debug_error('Novalnet callback received. Payment type ( ' . $_array_capture_params['payment_type'] . ' ) is mismatched!');
        }
        if ($_array_capture_params['status'] != 100)  {
            commerce_novalnet_debug_error('Novalnet callback received. Status (' . $_array_capture_params['status'] . ') is not valid');
        }
        if ((!empty($_array_capture_params['signup_tid'])) || (!empty($_array_capture_params['subs_billing']))) {
          if (strlen($_array_capture_params['signup_tid']) != 17 || !is_numeric($_array_capture_params['signup_tid'])) {
                commerce_novalnet_debug_error('Novalnet callback received. Invalid TID [' . $_array_capture_params['signup_tid'] . '] for Order.');
          }
          if ($_array_capture_params['subs_billing'] == 1 && (strlen($_array_capture_params['tid']) != 17
            || !is_numeric($_array_capture_params['tid']))) {
                commerce_novalnet_debug_error('Novalnet callback received. TID [' . $_array_capture_params['tid'] . '] is not valid.');
        }
      }
      else {
        if (in_array($_array_capture_params['payment_type'], array_merge($_array_chargebacks, array('INVOICE_CREDIT')))) {
          if (!is_numeric($_array_capture_params['tid_payment']) || strlen($_array_capture_params['tid_payment']) != 17) {
                commerce_novalnet_debug_error('Novalnet callback received. Invalid TID [' . $_array_capture_params['tid_payment'] . '] for Order.');
          }
        }
        if (strlen($_array_capture_params['tid']) != 17 || !is_numeric($_array_capture_params['tid'])) {
            commerce_novalnet_debug_error('Novalnet callback received. TID [' . $_array_capture_params['tid'] . '] is not valid.');
        }
      }
            if (!is_numeric($_array_capture_params['amount'])) {
          commerce_novalnet_debug_error('Novalnet callback received. The requested amount (' . $_array_capture_params['amount'] . ') is not valid');
      }
      if (isset($_array_capture_params['signup_tid']) && $_array_capture_params['signup_tid'] != '') {
        $_array_capture_params['shop_tid'] = $_array_capture_params['signup_tid'];
      }
      elseif (in_array($_array_capture_params['payment_type'], array_merge($_array_chargebacks, array('INVOICE_CREDIT')))) {
        $_array_capture_params['shop_tid'] = $_array_capture_params['tid_payment'];
      }
      elseif ($_array_capture_params['tid'] != '') {
        $_array_capture_params['shop_tid'] = $_array_capture_params['tid'];
      }
      return TRUE;
}

/**
 * Load the order details for the processed transaction ID
 *
 * @param none
 * @return array
 */
function commerce_novalnet_get_mandatory_params() {
    global $_array_capture_params, $_array_chargebacks;

    if (isset($_array_capture_params['vendor_activation']) && $_array_capture_params['vendor_activation'] == 1) {
                $params_required = array('vendor_id', 'vendor_authcode', 'product_id', 'aff_id', 'aff_authcode', 'aff_accesskey');
    }
    else {
            $params_required = array('vendor_id', 'tid', 'payment_type', 'status', 'amount');
            if (isset($_array_capture_params['subs_billing']) && $_array_capture_params['subs_billing'] == 1) {
                array_push($params_required, 'signup_tid');
            }
            elseif (isset($_array_capture_params['payment_type']) && in_array($_array_capture_params['payment_type'], array_merge($_array_chargebacks, array('INVOICE_CREDIT')))) {
                array_push($params_required, 'tid_payment');
            }
            if (isset($_array_capture_params['payment_type']) && $_array_capture_params['payment_type'] == 'SUBSCRIPTION_STOP') {
                unset($params_required[1]);
            }
        }
return $params_required;
}

/**
 * Load the transaction details for the processed transaction ID
 *
 * @param none
 * @return array
 */
function commerce_novalnet_callback_load_trxn($tid) {
 $result = db_select('commerce_novalnet_transaction_detail', 'trxn')
        ->fields('trxn')
    ->condition('trxn.tid', $tid)
    ->execute();
    if ($result) return $result->fetchAssoc();
    return FALSE;
}

/**
 * get the paid amount for the order transaction
 *
 * @param $tid double
 * @return array
 */
function commerce_novalnet_callback_get_paidamount($tid) {
  $result = db_select('commerce_novalnet_callback_history', 'trxn')
         ->fields('trxn', array('amount'))
     ->condition('trxn.org_tid', $tid);
    $result->addExpression('SUM(amount)', 'paid_amount');
    if ($result) return $result->execute()->fetchAssoc();
    return array();
}

/**
 * return the process type according to the payment type
 *
 * @param none
 * @return mixed
 */
  function commerce_novalnet_callback_paymenttype_level() {
        global $_array_payments, $_array_chargebacks, $_array_collection, $_array_capture_params;

      if (in_array($_array_capture_params['payment_type'], $_array_payments)) return 0;
      if (in_array($_array_capture_params['payment_type'], $_array_chargebacks)) return 1;
      if (in_array($_array_capture_params['payment_type'], $_array_collection)) return 2;

    return '';
  }

/**
 * Insert affiliate details on vendor activation
 *
 * @param none
 * @return none
 */
function commerce_novalnet_callback_add_affiliate_account() {
            global $_array_capture_params;
            $params = array(
                'vendor_id'             => $_array_capture_params['vendor_id'],
                'authcode'              => $_array_capture_params['vendor_authcode'],
                'product_id'            => $_array_capture_params['product_id'],
                'product_url'       => $_array_capture_params['product_url'],
                'activation_date' => (isset($_array_capture_params['activation_date']) && $_array_capture_params['activation_date'] ? date('Y-m-d H:i:s', strtotime($_array_capture_params['activation_date'])):date('Y-m-d')),
                'aff_id'                    => $_array_capture_params['aff_id'],
                'aff_authcode'      => $_array_capture_params['aff_authcode'],
                'aff_accesskey'     => $_array_capture_params['aff_accesskey'],
                'update_date'       => time());
        $query = db_insert('commerce_novalnet_affiliate_account')
                                ->fields(array_keys($params))
                                ->values($params)
                                ->execute();
        $callback_comments = 'Novalnet Callback received. Affiliate details has been added successfully.';
        commerce_novalnet_callback_send_notification_mail(array('order_no' => '',
                                                                'message'  => $callback_comments ));
        commerce_novalnet_debug_error($callback_comments);
}

/**
 * print the message on the debug mode
 *
 * @param $message array
 * @param $force_display boolean
 * @return none
 */
function commerce_novalnet_debug_error($message, $force_display = FALSE) {
    global $_process_debugmode;
        if ($_process_debugmode || $force_display) {
            echo $message;
        }
        exit;
}

/**
 * Build transaction comments for the newly created order
 *
 * @param $order object
 * @return comments
 */
function commerce_novalnet_callback_trxn_comments($order, $next_subs_cycle ='') {
        global $_array_capture_params, $_configuration;
        $_array_capture_params['amount'] =$_array_capture_params['amount']/100;
        $payment = explode('|', $order->data['payment_method']);
        $trans_comments = t('Novalnet transaction details') . "<br>";
        $trans_comments .= (isset($_array_capture_params['test_mode']) && $_array_capture_params['test_mode']) ? t('Test order') . '<br>': '';
        if (in_array($payment[0], array('commerce_novalnet_invoice', 'commerce_novalnet_prepayment')) && isset($_array_capture_params['due_date']) && isset($_array_capture_params['invoice_iban'])&& isset($_array_capture_params['invoice_bic']) && isset($_array_capture_params['invoice_bankname']) && isset($_array_capture_params['invoice_bankplace'])) {
            $trans_comments .= commerce_novalnet_prepare_invoice_comments($_array_capture_params, array(                                             'invoice_ref' => 'BNR-' . $_configuration['product_id'] . '-' . $order->order_number,
                                                        'order_no' => $order->order_number));
        }
        else {
                $trans_comments .= t('Novalnet Transaction ID:@tid', array('@tid' => $_array_capture_params['tid']));
        }
        if ($next_subs_cycle) $trans_comments .= '<br>Next Payment Date is: ' . $next_subs_cycle;

    return $trans_comments;
    }
