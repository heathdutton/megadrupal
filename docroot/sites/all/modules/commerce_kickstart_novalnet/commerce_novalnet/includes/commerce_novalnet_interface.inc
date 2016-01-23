<?php
/**
 * @file
 * Novalnet payment method module
 * This module is used for real time processing of
 * Novalnet transaction of customers.
 *
 * Copyright (c) Novalnet AG
 *
 * Released under the GNU General Public License
 * This free contribution made by request.
 * If you have found this script useful a small
 * recommendation as well as a comment on merchant form
 * would be greatly appreciated.
 *
 * Script : commerce_novalnet_interface.inc
 *
 */

module_load_include('inc', 'commerce_novalnet', 'includes/commerce_novalnet_core');
/**
 * build the payment data & process the payment
 *
 * @param $payment_method array
 * @param $pane_values array
 * @param $order object
 * @param $charge array
 * @return none
 */
function commerce_novalnet_do_payment_process($payment_method, $pane_values, $order, $charge) {
    global $base_url;
    $is_redirect_method = FALSE;
    $data = array();
    $payment = $payment_method['method_id'];
    $settings = commerce_novalnet_load_settings();
    commerce_novalnet_set_payment_details($data , $payment, $settings);
    commerce_novalnet_set_billing_address($data, $order);
    commerce_novalnet_set_order_details($data, $order, $charge);
    commerce_novalnet_set_system_details($data);
    commerce_novalnet_set_reference_details($data, $settings, $payment);
    commerce_novalnet_set_subscription_details($data, $settings);
    $is_affiliate = commerce_novalnet_add_affiliate_details($data);
    if ($is_affiliate)  $settings['access_key'] = $_SESSION['nn_aff_accesskey'];
    if (in_array($data['payment_type'], array('CREDITCARD', 'INVOICE', 'DIRECT_DEBIT_SEPA')))  {
        commerce_novalnet_check_onhold_limit($data, $settings);
    }
    if ($data['payment_type'] == 'CREDITCARD') {#credit card process
        $data['cc_holder'] = $data['cc_no'] = $data['cc_exp_month'] = $data['cc_exp_year'] = '';
        $data['cc_cvc2']    = $pane_values['nn_cc_cvv'];
        $data['pan_hash']   = $_SESSION['novalnet'][$payment]['hash'] = $pane_values['nn_cc_panhash'];
        $data['unique_id']  = $pane_values['nn_cc_unique_id'];
        $_SESSION['novalnet'][$payment]['card_account_holder']  = $pane_values['nn_cc_cardholder'];
        if ($settings['commerce_novalnet_cc_secure']) {
            $data['encoded_amount'] = $data['amount'];
            commerce_novalnet_encode_data($data, $settings['access_key'], array('encoded_amount'));
            $is_redirect_method = TRUE;
        }
    }
    if ($data['payment_type'] == 'DIRECT_DEBIT_SEPA') {  #SEPA process
        $data['bank_account'] = $data['bank_code'] = $data['bic'] = $data['iban'] = '';
        $data['iban_bic_confirmed'] = 1;
        $data['bank_account_holder'] = $_SESSION['novalnet'][$payment]['card_account_holder'] = $pane_values['sepa_cardholder'];
        $data['sepa_hash'] = $_SESSION['novalnet'][$payment]['hash'] = $pane_values['nn_sepa_panhash'];
        $data['sepa_unique_id'] = $pane_values['sepa_unique_id'];
        $sepa_due_date_limit = (($settings['commerce_novalnet_sepa_duration'] != '' && $settings['commerce_novalnet_sepa_duration'] >= 7) ? $settings['commerce_novalnet_sepa_duration'] : 7);
        $data['sepa_due_date'] = (date('Y-m-d', strtotime('+' . $sepa_due_date_limit . ' days')));
    }
    if (in_array($data['payment_type'], array('INVOICE', 'PREPAYMENT'))) {
        if ($data['payment_type'] == 'INVOICE' && $settings['commerce_novalnet_invoice_duration'] != '') {
            $data['due_date']  = commerce_novalnet_set_duedate($settings['commerce_novalnet_invoice_duration'], $payment);
        }
        $data['invoice_type'] = $data['payment_type'];
        $data['invoice_ref'] =  'BNR-' . $data['product'] . '-' . $data['order_no'];
    }

    $url = commerce_novalnet_set_payment_method_info($payment, 'url');
    if (in_array($data['payment_type'], array('PAYPAL', 'IDEAL', 'ONLINE_TRANSFER', 'EPS'))) {
        $data['uniqid'] = time();
        $params_to_be_encoded = array('auth_code', 'product', 'tariff', 'test_mode', 'amount', 'uniqid');
        commerce_novalnet_encode_data($data, $settings['access_key'], $params_to_be_encoded);
        commerce_novalnet_generate_hash($data, $settings['access_key']);
        $data['user_variable_0']        = $base_url;
        $is_redirect_method = TRUE;
    }
    if ($is_redirect_method) {
        $nn_callback = 'checkout/' . $order->order_id . '/payment/return/' . $order->data['payment_redirect_key'];
        $nn_errorcallback = 'checkout/' . $order->order_id . '/payment/return/' . $order->data['payment_redirect_key'];
        $data['return_url']             = url($nn_callback, array('absolute' => TRUE));
        $data['return_method']          = 'POST';
        $data['error_return_url']       = url($nn_errorcallback, array('absolute' => TRUE));
        $data['error_return_method']    = 'POST';
        $data['implementation']             = 'PHP';

    return array($data, $url);
    }
    if (in_array($payment, unserialize(NOVALNET_FRAUD_MODULES_ALLOWED_PAYMENTS))
        && isset($_SESSION['fraud_module'][$payment]) && $_SESSION['fraud_module'][$payment]) {
        foreach (unserialize(NOVALNET_FRAUD_MODULES_ALLOWED_PAYMENTS) as $allowed_payment) {
            if ($allowed_payment != $payment && isset($_SESSION['fraud_module'][$allowed_payment])) {
                unset($_SESSION['fraud_module'][$allowed_payment]);
                unset($_SESSION['fraud_module']['second_call'][$allowed_payment]);
            }
        }
        switch ($_SESSION['fraud_module'][$payment]) {
            case NOVALNET_FRAUD_MODULES_CALLBACK:
                $data['tel'] = trim($pane_values[$payment . '_fraud_tel']);
                $data['pin_by_callback'] = '1';
                $info_message = t('You will shortly receive a transaction PIN through phone call to complete the payment');
                break;
            case NOVALNET_FRAUD_MODULES_EMAIL:
                $data['email'] = trim($pane_values[$payment . '_fraud_mail']);
                $data['reply_email_check'] = '1';
                $info_message = t('You will shortly receive an information e-mail, please send the empty reply incl. the original e-mail');
                break;
            case NOVALNET_FRAUD_MODULES_SMS:
                $data['mobile'] = trim($pane_values[$payment . '_fraud_mobile']);
                $data['pin_by_sms'] = '1';
                $info_message = t('You will shortly receive an SMS containing your transaction PIN to complete the payment');
                break;
        }
    }
    $response = commerce_novalnet_drupal_request(http_build_query($data), $url['paygate_url']);

    if (isset($response->data)) {
        parse_str($response->data, $payment_response);

        if ($payment_response['status'] == NOVALNET_SUCCESS_STATUS) { #Payment success
            if (in_array($payment, unserialize(NOVALNET_FRAUD_MODULES_ALLOWED_PAYMENTS))
                && isset($_SESSION['fraud_module'][$payment]) && $_SESSION['fraud_module'][$payment]) {
                $_SESSION['fraud_module']['gateway_response'][$payment] = $payment_response;
                $_SESSION['fraud_module']['gateway_request'][$payment]  = $data;
                $_SESSION['fraud_module']['second_call'][$payment]  = TRUE;
                drupal_set_message($info_message);
                return FALSE;
            }
            $payment_response = commerce_novalnet_check_payment_status($payment, $payment_response, $data, $settings);
            $_SESSION['novalnet_trxn_comments'] = $payment_response['comments'];
            commerce_novalnet_order_completion_process($payment_method, $settings, $order, $payment_response, $data);
        }
        else {
            drupal_set_message(check_plain($payment_response['status_desc']), 'error');
            return FALSE;
        }
    }
    else {
        drupal_set_message(check_plain($response->error), 'error');
        return FALSE;
    }
}
function commerce_novalnet_order_completion_process($payment_method, $settings, $order, $payment_response, $data, $is_paypal_pending = FALSE) {

    $trans_info = commerce_novalnet_do_process_transaction($payment_method['method_id'], $settings, $order->order_number, $data, $payment_response);
    commerce_novalnet_add_payment_transaction($payment_method, $payment_response, $order->order_id, $trans_info);
    $status = $is_paypal_pending ? $settings['commerce_novalnet_paypal_pending_status'] : $settings[$payment_method['method_id'] . '_order_status'];
    commerce_checkout_complete($order);
    commerce_order_status_update($order, $status, FALSE, TRUE);
    unset($_SESSION['novalnet']);
    if (isset($_SESSION['fraud_module'])) unset($_SESSION['fraud_module']);
    if (isset($_SESSION['nn_aff_id'])) unset($_SESSION['nn_aff_id']);
    drupal_goto('checkout/' . $order->order_id . '/complete');
}
/**
 * set the affiliate details
 *
 * @param $data array
 * @return boolean
 */
function commerce_novalnet_add_affiliate_details(&$data) {
    if (!isset($_SESSION['nn_aff_id']) && is_numeric($data['customer_no'])) { #load affiliate id using customer number to current session
        $result = db_select('commerce_novalnet_transaction_detail', 'trxn')
                ->fields('trxn', array('vendor_id'))
                ->condition('trxn.customer_id', $data['customer_no'])
                ->condition('trxn.is_affiliate', '1')
                ->orderBy('id', 'DESC')
                ->range(0, 1)
                ->execute();
        $aff_id = $result->fetchAssoc();
        if (isset($aff_id['vendor_id']) && $aff_id['vendor_id'] != '') {
            $_SESSION['nn_aff_id'] = $aff_id['vendor_id'];
        }
    }
    if (isset($_SESSION['nn_aff_id'])) { #assign affiliate details using current session
        $result = db_select('commerce_novalnet_affiliate_account', 'aff')
                ->fields('aff', array('aff_authcode', 'aff_accesskey'))
                ->condition('aff.aff_id', $_SESSION['nn_aff_id'])
                ->execute();
        $aff_data = $result->fetchAssoc();
        if ($aff_data) {
            $data['vendor'] = $_SESSION['nn_aff_id'];
            $data['auth_code'] = $aff_data['aff_authcode'];
            $_SESSION['nn_aff_accesskey'] = $aff_data['aff_accesskey'];
            return TRUE;
        }
    }
    return FALSE;
}

/**
 * set the subscription details
 *
 * @param $data array
 * @param $settings array
 * @return none
 */
function commerce_novalnet_set_subscription_details(&$data, $settings) {
    if ($settings['tariff_period'] != '') {
        $data['tariff_period']  = $settings['tariff_period'];
    }
    if ($settings['tariff_period2'] != '' && $settings['tariff_period2_amount'] != '') {
        $data['tariff_period2'] = $settings['tariff_period2'];
        $data['tariff_period2_amount']  = $settings['tariff_period2_amount'];
    }
}

/**
 * set the reference details
 *
 * @param $data array
 * @param $order object
 * @param $charge array
 * @return none
 */
function commerce_novalnet_set_order_details(&$data, $order, $charge) {
    global $language;

    $lang = strtoupper($language->language);
    $data['currency']         = $charge['currency_code'];
    $data['order_no']         = $order->order_number;
    $data['remote_ip']        = ((ip_address() == '::1') ? '127.0.0.1' : ip_address());
    $data['lang']             = $lang;
    $data['language']         = $lang;
    $data['amount']           = $charge['amount'];
}

/**
 * set the payment details
 *
 * @param $data array
 * @param $settings array
 * @param $payment string
 * @return none
 */
function commerce_novalnet_set_payment_details(&$data, $payment, $settings) {
    $data['vendor']         = $settings['vendor_id'];
    $data['product']        = $settings['product_id'];
    $data['tariff']         = $settings['tariff_id'];
    $data['auth_code']      = $settings['auth_code'];
    $data['key']            = commerce_novalnet_set_payment_method_info($payment, 'key');
    $data['payment_type']   = commerce_novalnet_set_payment_method_info($payment, 'payment_type');
    $data['test_mode']      = $settings[$payment . '_testmode'];
}

/**
 * set the billing details
 *
 * @param $data array
 * @param $order object
 * @return none
 */
function commerce_novalnet_set_billing_address(&$data, $order) {
    $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
    $billing_address = $order_wrapper->commerce_customer_billing->commerce_customer_address->value();
    $data['first_name']   = isset($billing_address['first_name']) ? trim($billing_address['first_name']) : '';
    $data['last_name']    = isset($billing_address['last_name']) ? trim($billing_address['last_name']) : '';

    if (empty($data['first_name']) && empty($data['last_name']) && isset($billing_address['name_line']) ) {
       $data['first_name'] = trim($billing_address['name_line']);
    }
    if (empty($data['first_name']) || empty($data['last_name'])) {
        $name = $data['first_name'] . $data['last_name'];
        list($data['first_name'], $data['last_name']) = preg_match('/\s/', $name) ? explode(' ', $name, 2) : array($name, $name);
    }
    $data['email']        = $order->mail;
    $premise                  = ((trim($billing_address['premise']) != '') ? ',' : '');
    $data['street']           = trim($billing_address['premise']) . $premise . trim($billing_address['thoroughfare']);
    $data['search_in_street'] = '1';
    $data['city']             = trim($billing_address['locality']);
    $data['zip']              = trim($billing_address['postal_code']);
    $data['country']          = strtoupper($billing_address['country']);
    $data['country_code']     = strtoupper($billing_address['country']);
    $data['gender']           = 'u';
    $data['customer_no']      = ($order->uid) > 0 ? $order->uid : 'guest';
    $data['use_utf8']         = '1';
}

/**
 * set the system details
 *
 * @param $data array
 * @return none
 */
function commerce_novalnet_set_system_details(&$data) {
    $system_details = system_get_info('module', 'commerce_kickstart');
    $data['system_name']      = 'drupal - commercekickstart';
    $data['system_version']   = VERSION . '-' . $system_details['version'] . '-NN' . NOVALNET_COMMERCE_MODULE_VERSION;
    $data['system_ip']        = ($_SERVER['SERVER_ADDR'] == '::1') ? '127.0.0.1' : $_SERVER['SERVER_ADDR'];
    $data['system_url']       = url('', array('absolute' => TRUE));
}

/**
 * set the reference details
 *
 * @param $data array
 * @param $settings array
 * @param $payment string
 * @return none
 */
function commerce_novalnet_set_reference_details(&$data, $settings, $payment) {
    if ($settings['referrer_id'] != '') $data['referrer_id']  = $settings['referrer_id'];
    if ($settings[$payment . '_trans_ref1'] != '') {
        $data['input1']  = 'Reference 1';
        $data['inputval1']  = $settings[$payment . '_trans_ref1'];
    }
    if ($settings[$payment . '_trans_ref2'] != '') {
        $data['input2']  = 'Reference 2';
        $data['inputval2']  = $settings[$payment . '_trans_ref2'];
    }
}

/**
 * check the onhold details
 *
 * @param $data array
 * @param $settings array
 * @return none
 */
function commerce_novalnet_check_onhold_limit(&$data, $settings) {
    if ($settings['manual_check_limit'] != '' && $data['amount'] >= $settings['manual_check_limit']) {
        $data['on_hold'] = 1;
    }
}

/**
 * build the due date
 *
 * @param $payment string
 * @param $days integer
 * @return date
 */
function commerce_novalnet_set_duedate($days, $payment) {
    $duration = ($payment == 'commerce_novalnet_sepa' && $days < 7) ? '7' : $days;

    return (date('Y-m-d', strtotime('+' . $duration . ' days')));
}

/**
 * check and update the payment statuses from response
 *
 * @param $payment string
 * @param $response array
 * @param $data array
 * @param $settings array
 * @return array
 */
function commerce_novalnet_check_payment_status($payment, $response, $data = array(), $settings) {
    $trans_comments = '';
    $payment_test_mode_in_shop = $settings[$payment . '_testmode'];
    $trans_comments = '<b>' . t('Novalnet transaction details') . '</b>';
    $trans_comments .= (((isset($response['test_mode']) && $response['test_mode'] == 1) || $payment_test_mode_in_shop == 1) ? '<br>' . t('Test order') : '');
    $response['test_mode'] = (($response['test_mode'] == 1) ? $response['test_mode'] : $payment_test_mode_in_shop);

    if (in_array($payment, array('commerce_novalnet_invoice', 'commerce_novalnet_prepayment'))) {
        $trans_comments .= '<br>' . commerce_novalnet_prepare_invoice_comments($response, $data);
        commerce_novalnet_log_invoice_trxn_account_info(array(
            'order_no'          => $data['order_no'],
            'tid'               => $response['tid'],
            'test_mode'         => (($response['test_mode'] == 1)? $response['test_mode'] : $payment_test_mode_in_shop),
            'account_holder'    => NOVALNET_ACCOUNT_HOLDER,
            'account_number'    => $response['invoice_account'],
            'bank_code'         => $response['invoice_bankcode'],
            'bank_name'         => $response['invoice_bankname'],
            'bank_city'         => $response['invoice_bankplace'],
            'amount'            => $response['amount'],
            'currency'          => $response['currency'],
            'bank_iban'         => $response['invoice_iban'],
            'bank_bic'          => $response['invoice_bic'],
            'due_date'          => date('Y-m-d', strtotime($response['due_date'])),
            'update_date'       => time(),
        ));
    }
    else {
        $trans_comments .= '<br>' . t('Novalnet transaction ID:@tid', array('@tid' => $response['tid']));
    }

    $result = array(
      'status' => $response['status'],
      'tid' => ((isset($response['tid'])) ? $response['tid'] : ''),
      'comments' => $trans_comments,
      'gateway_response' => $response,
    );

    return $result;
}

/**
 * Prepare the invoice comments
 *
 * @param $payment string
 * @param $response array
 * @param $data array
 * @return string
 */
function commerce_novalnet_prepare_invoice_comments($response, $data) {

        $trans_comments = t('Please transfer the amount to the below mentioned account details of our payment processor Novalnet') . '<br/>';
        $trans_comments .= (($response['due_date'] != '') ? t('Due date: @date', array('@date' => date('m/d/Y', strtotime($response['due_date'])))) . '<br/>' : '');

        $trans_comments .= t('Account holder: NOVALNET AG') . '<br/>';
        $trans_comments .= t('IBAN: @iban', array('@iban' => $response['invoice_iban'])) . '<br/>';
        $trans_comments .= t('BIC: @bic' , array('@bic' => $response['invoice_bic'])) . '<br/>';
        $trans_comments .= t('Bank: @bank' , array('@bank' => $response['invoice_bankname'] . ' ' . $response['invoice_bankplace'])) . '<br/>';
        $trans_comments .= t('Amount: @amount', array('@amount' => number_format($response['amount'], 2, ',', '') . ' ' . $response['currency'])) . '<br/>';
        $trans_comments .= t('Reference 1: @ref1', array('@ref1' => $data['invoice_ref'])) . '<br/>';
        $trans_comments .= t('Reference 2: TID @ref2', array('@ref2' => $response['tid'])) . '<br/>';
        $trans_comments .= t('Reference 3: Order number @ref3', array('@ref3' => $data['order_no'])) . '<br/>';
    return $trans_comments;
    }

/**
 * log the invoice transaction informations
 *
 * @param $data array
 * @return none
 */
function commerce_novalnet_log_invoice_trxn_account_info($data) {
    $query = db_insert('commerce_novalnet_invoice_trxn_detail')
            ->fields(array_keys($data))
            ->values($data)
            ->execute();
    return TRUE;
}

/**
 * Adds the payment transaction
 *
 * @param $payment_method array
 * @param $payment_response array
 * @param $order_id integer
 * @param $trans_info array
 * @return none
 */
function commerce_novalnet_add_payment_transaction($payment_method, $payment_response, $order_id, $trans_info = array()) {
    $amount = $trans_info['amount'];
    if (in_array($payment_method['method_id'], array('commerce_novalnet_prepayment', 'commerce_novalnet_invoice'))) $amount = 0;
    $transaction = commerce_payment_transaction_new($payment_method['method_id'], $order_id);
    $transaction->instance_id = $payment_method['instance_id'];
    $transaction->payment_method = $payment_method['method_id'];
    $transaction->amount = $amount;
    $transaction->currency_code = $payment_response['gateway_response']['currency'];
    $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
    $transaction->message_variables = array('@name' => $payment_method['method_id']);
    $transaction->message =  $payment_response['comments'];
    $transaction->remote_id =  $payment_response['tid'];
    commerce_payment_transaction_save($transaction);
}

/**
 * Process the direct payment response
 *
 * @param $order_number string
 * @param $settings array
 * @param $payment string
 * @return array
 */
function commerce_novalnet_do_process_transaction($payment, $settings, $order_number, $req_data, $payment_response) {

    $data = array(
        'vendor'      => $req_data['vendor'],
        'auth_code'   => $req_data['auth_code'],
        'product'     => $req_data['product'],
        'tariff'      => $req_data['tariff'],
        'order_no'    => $order_number,
        'tid'         => $payment_response['gateway_response']['tid'],
        'payment'     => $payment,
        'key'         => commerce_novalnet_set_payment_method_info($payment, 'key'),
    );
    if ($data['key'] == 27)  {
        $data['invoice_ref'] = 'BNR-' . $data['product'] . '-' . $data['order_no'];
    }
    $trans_novalnet_info = commerce_novalnet_do_xml_request($data, 'TRANSACTION_STATUS');

    commerce_novalnet_log_initial_transaction(array(
        'tid'               => $data['tid'],
        'vendor'            => $data['vendor'],
        'auth_code'         => $data['auth_code'],
        'tariff_id'         => $data['tariff'],
        'product_id'        => $data['product'],
        'subs_id'           => empty($trans_novalnet_info['subs_id'])?'':$trans_novalnet_info['subs_id'],
        'payment_id'        => $data['key'],
        'payment_type'      => $payment,
        'amount'            => $trans_novalnet_info['amount'],
        'currency'          => $req_data['currency'],
        'status'            => $payment_response['gateway_response']['status'],
        'order_no'          => $order_number,
        'test_mode'         => $payment_response['gateway_response']['test_mode'],
        'account_holder'    => isset($_SESSION['novalnet'][$payment]['card_account_holder'])?$_SESSION['novalnet'][$payment]['card_account_holder']:'',
        'customer_id'       => $req_data['customer_no'],
        'process_key'       => isset($_SESSION['novalnet'][$payment]['hash'])?$_SESSION['novalnet'][$payment]['hash']:'',
        'gateway_status'    => ((isset($trans_novalnet_info['status'])) ? $trans_novalnet_info['status'] : 0),
        'org_total'         => $trans_novalnet_info['amount'],
        'refunded_amount'   => 0,
    ));
    if ($trans_novalnet_info['status'] == NOVALNET_SUCCESS_STATUS  && $payment == 'commerce_novalnet_paypal') {
        commerce_novalnet_insert_success_log(array(
            'payment_type'  => $payment,
            'status'        => $trans_novalnet_info['status'],
            'org_tid'       => $data['tid'],
            'amount'        => $trans_novalnet_info['amount'],
            'order_no'      => $order_number,
        ));
    }
    if (!empty($trans_novalnet_info['subs_id'])) {
      commerce_novalnet_log_subscription_transaction(array(
        'tid'               => $data['tid'],
        'subs_id'           => $trans_novalnet_info['subs_id'],
        'order_no'          => $order_number,
        'signup_date'       => time(),
        'update_date'   => time()
        ));
    }
    commerce_novalnet_send_acknowledgement($data);
    return $trans_novalnet_info;
}


/**
 * send acknowledgement to server
 *
 * @param $data array
 * @return boolean
 */
 function commerce_novalnet_send_acknowledgement($data) {
    $urlparam = array(
        'vendor'    => $data['vendor'],
        'product'   => $data['product'],
        'key'       => $data['key'],
        'tariff'    => $data['tariff'],
        'auth_code' => $data['auth_code'],
        'status'    => NOVALNET_SUCCESS_STATUS,
        'tid'       => $data['tid'],
        'order_no'  => $data['order_no']
    );

    if ($data['key'] == 27) $urlparam['invoice_ref'] = $data['invoice_ref'];
    if ($urlparam['vendor'] && $urlparam['product'] && $urlparam['key'] && $urlparam['tariff']
        && $urlparam['auth_code'] && $urlparam['tid']) {
        commerce_novalnet_drupal_request(http_build_query($urlparam), 'https://payport.novalnet.de/paygate.jsp');
    }
    return TRUE;
}

/**
 * Log the transaction details
 *
 * @param $data array
 * @return boolean
 */
function commerce_novalnet_log_initial_transaction($data) {

    $data = array(
        'tid'               => $data['tid'],
        'vendor_id'         => $data['vendor'],
        'authcode'          => $data['auth_code'],
        'tariff_id'         => $data['tariff_id'],
        'product_id'        => $data['product_id'],
        'subs_id'           => $data['subs_id'],
        'payment_id'        => $data['payment_id'],
        'status'            => $data['status'],
        'payment_type'      => $data['payment_type'],
        'amount'            => $data['amount'],
        'currency'          => $data['currency'],
        'order_no'          => $data['order_no'],
        'test_mode'         => $data['test_mode'],
        'account_holder'    => $data['account_holder'],
        'customer_id'       => $data['customer_id'],
        'process_key'       => $data['process_key'],
        'gateway_status'    => $data['gateway_status'],
        'org_total'         => $data['org_total'],
        'refunded_amount'   => 0,
        'update_date'       => time(),
        'is_affiliate'      => (isset($_SESSION['nn_aff_id'])? 1 : 0));

        $query = db_insert('commerce_novalnet_transaction_detail')
                ->fields(array_keys($data))
                ->values($data)
                ->execute();

    return TRUE;
}

/**
 * Log the subscription details
 *
 * @param $data array
 * @return boolean
 */
function commerce_novalnet_log_subscription_transaction($data) {

    $query = db_insert('commerce_novalnet_subscription_detail')
            ->fields(array_keys($data))
            ->values($data)
            ->execute();

    return TRUE;
}

/*
 * encode the secured parameters
 *
 * @param  $data array
 * @param  $access_key string
 * @param  $params_to_be_encoded array
 * @return boolean
 */
function commerce_novalnet_encode_data(&$data, $access_key, $params_to_be_encoded = array()) {
    if (!function_exists('base64_encode') || !function_exists('pack') || !function_exists('crc32')) {
        return FALSE;
    }

    foreach ($params_to_be_encoded as $value) {
        $fields = $data[$value];
        if ($fields === '') {
            return FALSE;
        }
        try {
            $crc = sprintf('%u', crc32($fields)); //%u is a must for ccrc32 returns a signed value
            $fields = $crc . "|" . $fields;
            $fields = bin2hex($fields . $access_key);
            $fields = strrev(base64_encode($fields));
            $data[$value] = $fields;
        } catch (Exception $e) {
            return FALSE;
        }
    }
    return TRUE;
}

/**
 * Generate the hash value
 *
 * @param $data array
 * @param $access_key string
 * @return none
 */
function commerce_novalnet_generate_hash(&$data, $access_key) {
    if (!function_exists('md5')) {
        return FALSE;
    }
    $data['hash'] = md5($data['auth_code'] . $data['product'] . $data['tariff'] . $data['amount'] . $data['test_mode'] . $data['uniqid'] . strrev($access_key));
}

/**
 * Process the offsite payment response
 *
 * @param $response array
 * @param $payment_method array
 * @param $order array
 * @return boolean
 */
function commerce_novalnet_do_process_redirect_response($order, $payment_method, $response) {
    $settings = commerce_novalnet_load_settings();

    if ($response['status'] == NOVALNET_SUCCESS_STATUS || ($payment_method['method_id'] == 'commerce_novalnet_paypal'
        && $response['status'] == NOVALNET_PAYPAL_PENDING_STATUS)) { #Payment Success
        if (isset($_SESSION['nn_aff_accesskey'])) $settings['access_key'] = $_SESSION['nn_aff_accesskey'];
        $response['product'] = isset($response['product'])? $response['product'] : $response['product_id'];

        if ($payment_method['method_id'] != 'commerce_novalnet_cc') {
            if (!commerce_novalnet_validate_hash_value($response, $settings)) {
                drupal_set_message(t('While redirecting some data has been changed. The hash check failed.'), 'error');
                return FALSE;
            }
            $response['test_mode'] = commerce_novalnet_decode_data($response['test_mode'], $settings['access_key']);
            $response['amount'] = commerce_novalnet_decode_data($response['amount'], $settings['access_key']);
            $response['product'] = commerce_novalnet_decode_data($response['product'], $settings['access_key']);
            $response['tariff'] = commerce_novalnet_decode_data($response['tariff'], $settings['access_key']);
            $response['auth_code'] = commerce_novalnet_decode_data($response['auth_code'], $settings['access_key']);
        }
        $payment_details = commerce_novalnet_check_payment_status($payment_method['method_id'], $response, array(), $settings);
        $_SESSION['novalnet_trxn_comments'] = $payment_details['comments'];
        $is_paypal_pending = ($payment_method['method_id'] == 'commerce_novalnet_paypal' && $response['status'] == NOVALNET_PAYPAL_PENDING_STATUS)? TRUE : FALSE;
        commerce_novalnet_order_completion_process($payment_method, $settings, $order, $payment_details, $response, $is_paypal_pending);
    }
    else { #payment failed
        drupal_set_message(check_plain(commerce_novalnet_get_errormessage($response)), 'error');
        return FALSE;
    }
    return TRUE;
}

/**
 * get the error message from the server
 *
 * @param $response array
 * @param $settings array
 * @return boolean
 */
function commerce_novalnet_get_errormessage($response) {
    return ((isset($response['status_text']) && $response['status_text'] != '') ? $response['status_text'] : ((isset($response['status_desc']) && $response['status_desc'] != '') ? $response['status_desc'] : ''));

}

/**
 * validate the hash value
 *
 * @param $response array
 * @param $settings array
 * @return boolean
 */
function commerce_novalnet_validate_hash_value($response, $settings) {
    commerce_novalnet_generate_hash($response, $settings['access_key']);
    return (isset($response['hash2']) && $response['hash2'] == $response['hash']);
}

/**
 * decode the given data
 *
 * @param $data array
 * @param $key string
 * @return array
 */
function commerce_novalnet_decode_data($data, $key) {
    try {
        $data = base64_decode(strrev($data));
        $data = pack("H" . strlen($data), $data);
        $data = substr($data, 0, stripos($data, $key));
        $pos = strpos($data, "|");
        if ($pos === FALSE) {
            return FALSE;
        }
        $crc = substr($data, 0, $pos);
        $value = trim(substr($data, $pos + 1));
        if ($crc != sprintf('%u', crc32($value))) {
            return FALSE;
        }
        return $value;
    } catch (Exception $e) {
      return $e;
    }

    return $data;
}

/**
 * Process the fraud module status
 *
 * @param $payment_method array
 * @param $pane_values array
 * @param $order object
 * @param $charge array
 * @param $pane_form array
 * @return array
 */
function commerce_novalnet_do_fraud_status_process($payment_method, $pane_values, $order, $charge) {
    $payment = $payment_method['method_id'];
    $settings = commerce_novalnet_load_settings();
    $request_type = 'REPLY_EMAIL_STATUS';
    $data['tid'] = $_SESSION['fraud_module']['gateway_response'][$payment]['tid'];
    if (in_array($_SESSION['fraud_module'][$payment], array('CALLBACK', 'SMS'))) {
        if ($pane_values[$payment . '_new_pin']) {
            $request_type = 'TRANSMIT_PIN_AGAIN';
        }
        else {
            $data['pin'] =  trim($pane_values[$payment . '_pin_number']);
            $request_type = 'PIN_STATUS';
        }
    }

    commerce_novalnet_set_payment_details($data, $payment, $settings);
    $response = commerce_novalnet_do_xml_request($data, $request_type);
    if (!$response) return FALSE;
    if (!isset($response['status']) || $response['status'] != NOVALNET_SUCCESS_STATUS) {
        $error_msg = isset($response['status_message']) ? $response['status_message'] : $response['pin_status']['status_message'];
        drupal_set_message(check_plain($error_msg), 'error');
        if (isset($response['status']) && $response['status'] == '0529006') {
            $_SESSION['nn_payment_lock'][$payment] = TRUE;
            $_SESSION['nn_payment_lock']['time'][$payment] = time() + (30 * 60);
            drupal_goto('checkout/' . $order->order_id . '/review');
        }
        return FALSE;
    }
    else {
        $data = $_SESSION['fraud_module']['gateway_request'][$payment];
        $payment_response = $_SESSION['fraud_module']['gateway_response'][$payment];
        $payment_response = commerce_novalnet_check_payment_status($payment, $payment_response, $data, $settings);
        $_SESSION['novalnet_trxn_comments'] = $payment_response['comments'];
        commerce_novalnet_order_completion_process($payment_method, $settings, $order, $payment_response, $data);
    }
}

/**
 * Process the fraud module status check
 *
 * @param $data array
 * @param $request_type string
 * @return array
 */
function commerce_novalnet_do_xml_request($data, $request_type = NULL) {
    $xml = '<?xml version="1.0" encoding="UTF-8"?>
        <nnxml>
            <info_request>
                <vendor_id>' . $data['vendor'] . '</vendor_id>
                <vendor_authcode>' . $data['auth_code'] . '</vendor_authcode>
                <request_type>' . $request_type . '</request_type>
                <tid>' . $data['tid'] . '</tid>';
                if ($request_type == 'PIN_STATUS')
                    $xml .= '<pin>' . $data['pin'] . '</pin>';
                if ($request_type == 'TRANSACTION_STATUS')
                    $xml .= '<product_id>' . $data['product'] . '</product_id>';
                $xml .= '</info_request></nnxml>';

    $response = commerce_novalnet_drupal_request($xml, 'https://payport.novalnet.de/nn_infoport.xml');
    if (!isset($response->data)) {
        drupal_set_message(check_plain($response->error), 'error');
    return FALSE;
    }
    return json_decode(json_encode(simplexml_load_string($response->data)), TRUE);
}

/**
 * Perform the server request
 *
 * @param $data array
 * @param $url string
 * @return none
 */

function commerce_novalnet_drupal_request($data, $url) {
    $options = array('headers' => array('Content-Type' => 'application/x-www-form-urlencoded'),
                                        'method' => 'POST',
                                        'data' => $data);

    return drupal_http_request($url , $options);
}
/**
 * Insert the log for the transaction
 *
 * @param $data array
 * @return none
 */
function commerce_novalnet_insert_success_log($data) {
    $data['callback_date'] = time();
    $query = db_insert('commerce_novalnet_callback_history')
            ->fields(array_keys($data))
            ->values($data)
            ->execute();
}
