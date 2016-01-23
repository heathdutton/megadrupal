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
 * Script : commerce_novalnet_order_operations.inc
 *
 */
module_load_include('inc', 'commerce_novalnet', 'includes/commerce_novalnet_core');
/**
 * Implements Hook novalnet order operations form
 *
 * @param $form array
 * @param $form_state array
 * @param $order_id integer
 * @return none
 */
function commerce_novalnet_order_operations_form($form, &$form_state, $order_id) {

    if ($order_id) {
        $process_available = FALSE;
        #Fetch Transaction details and order details from the db
        $order = commerce_order_load_by_number($order_id);
        $trxn_details = commerce_novalnet_load_trxn_details($order->order_number);
        $gateway_status = $trxn_details['gateway_status'];
        $payment_id = $trxn_details['payment_id'];
        $form['novalnetlogo'] = array(
            '#markup' =>  l(t('<a href="' . t('http://www.novalnet.com') . '" target="_blank"><span ><img src="' . NOVALNET_IMAGES_PATH . 'novalnet.png" alt="Novalnet AG" title="Novalnet AG"></span></a>'), 'image/path', array('html' => TRUE)),
        );
        $form['novalnet_tid'] = array(
            '#markup' =>  '<br><br><b><span>' . t('Novalnet transaction ID:@tid', array('@tid' => $trxn_details['tid'])) . '</span></b>'
        );
        $form['nn_process'] = array(
            '#type'         => 'select',
            '#title'         => t('Select'),
            '#attributes'   => array('id' => 'nn_process')
        );
        #Transaction management block
        if (in_array($gateway_status, array(98, 99)) || ($gateway_status == 91 && $payment_id == 27)) {
            $process_available = TRUE;
            $nn_process['trxn_management'] = t('Manage Transaction process');
            $form['trxn_management'] = array(
                '#type'         => 'fieldset',
                '#title'        => t('Manage Transaction process'),
                '#collapsible'  => FALSE,
                '#collapsed'    => TRUE,
                '#states'       => array('visible' => array(':input[id="nn_process"]' => array('value' => 'trxn_management'))),
            );
            $form['trxn_management']['transaction_mode'] = array(
                '#type'         => 'select',
                '#title'        => t('Please select status'),
                '#options'      => array('' => t('Select'), NOVALNET_SUCCESS_STATUS => t('Confirm'), NOVALNET_DEACTIVATED_STATUS => t('Cancel')),
            );
        }
        #Refund block
        if ($gateway_status == NOVALNET_SUCCESS_STATUS && in_array($payment_id, array(6, 33, 34, 37, 49, 27, 50))) {
            $amount = $trxn_details['org_total'];
            if ($payment_id == 27) {
                $paid_amount = commerce_novalnet_get_invoice_paidamount($trxn_details['tid']);
                $amount = $paid_amount['paid_amount'];
            }

            $amount = ($amount - $trxn_details['refunded_amount']);

            if ($amount > 0) {
                $process_available = TRUE;
                $nn_process['refund_management'] = t('Refund');
                $ref_amount = $amount;
                $trxndate = !empty($trxn_details['update_date']) ? date('Y-m-d', $trxn_details['update_date']) : '';
                $form['refund_management'] = array(
                    '#type'         => 'fieldset',
                    '#title'        => t('Refund process'),
                    '#collapsible'  => FALSE,
                    '#collapsed'    => TRUE,
                    '#states'       => array('visible' => array(':input[id="nn_process"]' => array('value' => 'refund_management'))),
                );
                if (in_array($payment_id, array(27, 33, 49))) {
                    $form['refund_management']['refund_method'] = array(
                        '#type'             => 'select',
                        '#title'            => t('Select the refund option'),
                        '#attributes'       => array( 'id' => 'refund_menthod'),
                        '#options'          => array('NONE' => t('None'), 'SEPA' => t('Direct Debit SEPA')),
                        '#default_value'    => 'NONE',
                    );
                }
                $form['refund_management']['refund_amount'] = array(
                    '#type'             => 'textfield',
                    '#title'            => t('Please enter the refund amount'),
                    '#size'             => 20,
                    '#field_suffix'     => t('in cents'),
                    '#attributes'       => array( 'onkeypress' => 'return is_number_key(event);'),
                    '#default_value'    => $ref_amount
                );
                $form['refund_management']['can_refund_amount'] = array(
                    '#type'             => 'hidden',
                    '#default_value'    => $ref_amount
                );
                if ($trxndate != '' && strtotime(date('Y-m-d')) > strtotime($trxndate)) {
                    $form['refund_management']['refund_ref'] = array(
                        '#type'         => 'textfield',
                        '#title'        => t('Refund reference:'),
                        '#size'         => 20,
                    );
                }
                if (in_array($payment_id, array(27, 33, 49))) {
                    $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
                    $billing_address = $order_wrapper->commerce_customer_billing->commerce_customer_address->value();
                    $billing_address['first_name']   = isset($billing_address['first_name']) ? trim($billing_address['first_name']) : '';
                    $billing_address['last_name']    = isset($billing_address['last_name']) ? trim($billing_address['last_name']) : '';
                    $name = $billing_address['first_name'] . ' ' . $billing_address['last_name'];
                    if (isset($billing_address['name_line'])) {
                        $name = trim($billing_address['name_line']);
                    }

                    $form['refund_management']['refund_holder'] = array(
                        '#type'         => 'textfield',
                        '#title'        => t('Account holder'),
                        '#size'         => 40,
                        '#states'       => array('visible' => array(':input[id="refund_menthod"]' => array('value' => 'SEPA'))),
                        '#attributes'   => array('autocomplete' => 'off'),
                        '#default_value' => $name
                    );
                    $form['refund_management']['refund_iban'] = array(
                        '#type'         => 'textfield',
                        '#title'        => 'IBAN',
                        '#size'         => 40,
                        '#states'       => array('visible' =>   array(':input[id="refund_menthod"]' => array('value' => 'SEPA'))),
                        '#attributes'   => array('autocomplete' => 'off'),
                    );
                    $form['refund_management']['refund_bic'] = array(
                        '#type'         => 'textfield',
                        '#title'        => 'BIC',
                        '#size'         => 40,
                        '#states'       => array('visible' =>   array(':input[id="refund_menthod"]' => array('value' => 'SEPA'))),
                        '#attributes'   => array('autocomplete' => 'off'),
                    );
                }
            }
        }
        #Amount update block
        if (($payment_id == 37 && $gateway_status == 99) || ($payment_id == 27 && $gateway_status == NOVALNET_SUCCESS_STATUS)) {
            $amount = $trxn_details['org_total'];

            $nn_process['amount_update_management'] = ($payment_id == 27) ? t('Change the amount / due date'):t('Amount update');
            $amount = ($amount - $trxn_details['refunded_amount']);
            $form['amount_update_management'] = array(
                '#type'             => 'fieldset',
                '#title'            => check_plain(($payment_id == 27) ? t('Change the amount / due date'):t('Amount update')),
                '#collapsible'      => FALSE,
                '#collapsed'        => TRUE,
                '#states'           => array('visible' => array(':input[id="nn_process"]' => array('value' => 'amount_update_management'))),
            );
            if ($payment_id == 27) {
                $paid_amount = commerce_novalnet_get_invoice_paidamount($trxn_details['tid']);
                if ($paid_amount['paid_amount'] >= $trxn_details['amount']) {
                    unset($nn_process['amount_update_management']);
                }
                $amount = ($trxn_details['amount'] - $paid_amount['paid_amount']);
                $inv_bank_details = commerce_novalnet_load_invoice_details($order_id);
                $duedate = date('Y-m-d', strtotime($inv_bank_details['due_date']));
            }

            $process_available = count($nn_process) > 0 ? TRUE:FALSE;
            $form['amount_update_management']['update_amount'] = array(
                '#type'             => 'textfield',
                '#title'            => t('Amount'),
                '#attributes'       => array( 'onkeypress' => 'return is_number_key(event);'),
                '#size'             => 20,
                '#field_suffix'     => t('in cents'),
                '#default_value'    => $amount
            );
            if (isset($duedate)) {
                $form['amount_update_management']['due_date'] = array(
                 '#type' => 'date_select',
                 '#title' => t('Transaction due date'),
                 '#default_value' => $duedate,
                 '#date_format' => 'd-m-Y',
                 '#date_label_position' => 'within',
                 '#date_year_range' => '-0:+1',
                '#attributes' => array('id' => 'nn_due_date'),
                );
            }
            $form['amount_update_message'] = array(
                '#type'                 => 'hidden',
                '#value'                => t('Are you sure you want to change the order amount or due date?'),
                '#attributes'       => array('id' => 'amount_update_message')
            );
        }
        if ($trxn_details['subs_id'] != '' && $trxn_details['gateway_status'] != NOVALNET_DEACTIVATED_STATUS
            && (commerce_novalnet_load_subscription_details($order_id))) {
            $process_available = TRUE;
            $nn_process['subs_cancel'] = t('Cancel Subscription');
            $form['subs_cancel'] = array(
                '#type' => 'fieldset',
                '#title' => t('Cancel Subscription'),
                '#collapsible' => FALSE,
                '#collapsed' => TRUE,
                '#states' => array('visible' => array(':input[id="nn_process"]' => array('value' => 'subs_cancel'))),
            );
            $form['subs_cancel']['cancel_reason'] = array(
                '#type' => 'select',
                '#title' => t('Please select reason'),
                '#options' => commerce_novalnet_get_subscription_cancel_reasons(),
                '#attributes' => array('style' => 'text-align:center')
            );
        }
        $form['order_id'] = array(
            '#type'                     => 'hidden',
            '#value'                    => $order->order_id
        );
        if (!$process_available) {
             drupal_goto('admin/commerce/orders/' . $order->order_id . '/payment');
        }
        else {
            $form['novalnet_submit'] = array(
                '#type'       => 'submit',
                '#attributes' => array('id' => 'novalnet_submit', 'onclick' => 'return confirm_message()'),
                '#value'      => t('Confirm'),
            );
        }
        $form['nn_process']['#options'] = $nn_process;
        $form['nn_process']['#default_value'] = isset($nn_process['trxn_management']) ? 'trxn_management' : (isset($nn_process['amount_update_management']) ?'amount_update_management' : (isset($nn_process['refund_management']) ? 'refund_management' : ''));
        drupal_add_js(drupal_get_path('module', 'commerce_novalnet') . '/js/admin_operations.js', 'file');

    return $form;
    }
}

/**
 * Implements Hook novalnet order operations form validation
 *
 * @param $form array
 * @param $form_state array
 * @return boolean
 */
function commerce_novalnet_order_operations_form_validate($form, &$form_state) {
    $values = $form_state['values'];

    if ($values['nn_process'] == 'trxn_management' && $values['transaction_mode'] == '') {
         form_set_error('', t('Please select status'));
        return FALSE;
    }
    if ($values['nn_process'] == 'refund_management' && (!is_numeric($values['refund_amount']) || (int)$values['refund_amount'] == 0)) {
        form_set_error('', t('The amount is invalid'));
        return FALSE;
    }
    if ($values['nn_process'] == 'refund_management' && isset($values['refund_method']) && $values['refund_method'] == 'SEPA'
        && (!$values['refund_holder'] || !$values['refund_iban'] || !$values['refund_bic'])) {
        form_set_error('', t('Your account details are invalid'));
        return FALSE;
    }
    if ($values['nn_process'] =='amount_update_management' && (!is_numeric($values['update_amount']) || (int)$values['update_amount'] == 0)) {
        form_set_error('', t('The amount is invalid'));
        return FALSE;
    }
    if ($values['nn_process'] =='amount_update_management' && isset($values['due_date']) && empty($values['due_date'])) {
        form_set_error('', t('Please fill in all the mandatory fields'));
        return FALSE;
    }
    if ($values['nn_process'] =='subs_cancel' && $values['cancel_reason'] == '') {
        form_set_error('', t('Please select the reason of subscription cancellation'));
        return FALSE;
    }
}

/**
 * Implements Hook novalnet order operations form submit
 *
 * @param $form array
 * @param $form_state array
 * @return boolean
 */
function commerce_novalnet_order_operations_form_submit($form, &$form_state) {
    $values = $form_state['values'];

    $order = commerce_order_load($values['order_id']);
    $trxn_details = commerce_novalnet_load_trxn_details($order->order_number);

    if ($values['nn_process'] == 'trxn_management') {
        $data = array(
            'vendor'            => $trxn_details['vendor_id'],
            'product'           => $trxn_details['product_id'],
            'tariff'            => $trxn_details['tariff_id'],
            'auth_code'         => $trxn_details['authcode'],
            'edit_status'       => 1,
            'tid'               => $trxn_details['tid'],
            'key'               => $trxn_details['payment_id'],
            'status'            => $values['transaction_mode']
        );
        $response = commerce_novalnet_drupal_request(http_build_query($data), 'https://payport.novalnet.de/paygate.jsp');
        if (!isset($response->data)) {
                drupal_set_message(check_plain($response->error), 'error');
                return FALSE;
        }
        parse_str($response->data, $result); #Parse Paygate response into array
        if ($result['status'] == NOVALNET_SUCCESS_STATUS) {
            $trans = commerce_payment_transaction_load_multiple('', array('remote_id' => $data['tid']));
            $trans_id = array_keys($trans);
            $transaction = commerce_payment_transaction_load($trans_id['0']);
            $settings = commerce_novalnet_load_settings();
             list($status, $comments) = ($data['status'] == NOVALNET_SUCCESS_STATUS) ? array($settings['onhold_comp_status'], t('The transaction has been confirmed on @date @time', array('@date' => date('m/d/Y'), '@time' => date('H:i:s')))) : array($settings['onhold_cancel_status'], t('The transaction has been canceled on @date @time', array('@date' => date('m/d/Y'), '@time' => date('H:i:s'))));
            $payment_method_id = explode('|', $order->data['payment_method']);
            commerce_novalnet_new_transaction( array(
                      'payment_method' => $payment_method_id['0'],
                      'order_id' => $order->order_id,
                      'instance_id' => $order->data['payment_method'],
                      'amount' => 0,
                      'currency_code' => $trxn_details['currency'],
                      'message' => $comments,
                      'remote_id' => $data['tid']));
            commerce_order_status_update($order, $status, FALSE, TRUE);
            $trans_info = commerce_novalnet_do_xml_request($data, 'TRANSACTION_STATUS');
            commerce_novalnet_live_transaction_update(array('amount' => $trans_info['amount'], 'gateway_status' => $trans_info['status']), $order->order_number);
            drupal_set_message(check_plain($comments));
        }
        else {
           drupal_set_message(check_plain(commerce_novalnet_get_errormessage($result) . '(' . $result['status'] . ')'), 'error');
        }
    }
    elseif ($values['nn_process'] == 'amount_update_management') {
        $amount =  $values['update_amount'];
        $data = array(
            'vendor'            => $trxn_details['vendor_id'],
            'product'           => $trxn_details['product_id'],
            'tariff'            => $trxn_details['tariff_id'],
            'auth_code'         => $trxn_details['authcode'],
            'edit_status'       => 1,
            'update_inv_amount' => 1,
            'tid'               => $trxn_details['tid'],
            'key'               => $trxn_details['payment_id'],
            'status'            => NOVALNET_SUCCESS_STATUS,
            'amount'            => $amount,
        );
        if (!empty($values['due_date'])) {
          $data['due_date'] = $values['due_date'];
        }
        $response = commerce_novalnet_drupal_request(http_build_query($data), 'https://payport.novalnet.de/paygate.jsp');
        if (!isset($response->data)) {
            drupal_set_message(check_plain($response->error), 'error');
            return FALSE;
        }
        parse_str($response->data, $result); #Parse Paygate response into array
        if ($result['status'] == NOVALNET_SUCCESS_STATUS) {
            $comments = t('The transaction amount @amount @curr has been updated successfully on @date @time', array('@amount' => number_format($amount/100, 2, ',', ''), '@curr' => $trxn_details['currency'], '@date' => date('m/d/Y'), '@time' => date('H:i:s')));
            $inv_bank_details = commerce_novalnet_load_invoice_details($order->order_number);
            if ($data['key'] == 27) $comments .= '<br>' . commerce_novalnet_build_amountupdate_comment($inv_bank_details, $data, $order->order_number);
                $settings = commerce_novalnet_load_settings();
                $payment_method_id = explode('|', $order->data['payment_method']);
                commerce_novalnet_new_transaction( array(
                      'payment_method' => $payment_method_id['0'],
                      'order_id' => $order->order_id,
                      'instance_id' => $order->data['payment_method'],
                      'amount' => 0,
                      'currency_code' => $trxn_details['currency'],
                      'message' => $comments,
                      'remote_id' => $data['tid']));

                $trans_info = commerce_novalnet_do_xml_request($data, 'TRANSACTION_STATUS');
                commerce_novalnet_live_transaction_update(array('amount' => $trans_info['amount'], 'org_total' => $trans_info['amount'], 'gateway_status' => $trans_info['status']), $order->order_number);
                $status = $trans_info['status'] == NOVALNET_DEACTIVATED_STATUS ?$settings['onhold_cancel_status']:$order->status;
                commerce_order_status_update($order, $status, FALSE, TRUE, $comments);
                if ($trxn_details['payment_id'] == 27) {
                    commerce_novalnet_update_invoice_details(array('amount' => $trans_info['amount'], 'due_date' => $data['due_date']), $order->order_number);
                }
                drupal_set_message(t('Successful'));
            }
            else {
                drupal_set_message(check_plain(commerce_novalnet_get_errormessage($result) . '(' . $result['status'] . ')'), 'error');
        }
    }
    elseif ($values['nn_process'] == 'refund_management') {
        if ($values['refund_amount'] > $values['can_refund_amount']) {
            drupal_set_message(t('The amount is invalid'), 'error');
            return FALSE;
        }
        $data = array(
            'vendor'         => $trxn_details['vendor_id'],
            'product'        => $trxn_details['product_id'],
            'tariff'         => $trxn_details['tariff_id'],
            'key'            => $trxn_details['payment_id'],
            'auth_code'      => $trxn_details['authcode'],
            'cancel_sub'     => '1',
            'tid'            => $trxn_details['tid'],
            'refund_request' => '1',
            'refund_param'   => $values['refund_amount'],
        );
        if (isset($values['refund_method']) && $values['refund_method'] != 'NONE') {
            if (!$values['refund_holder'] || !$values['refund_iban'] || !$values['refund_bic'] ) {
                drupal_set_message(t('Your account details are invalid'), 'error');
                return FALSE;
            }
            $data['account_holder'] = $values['refund_holder'];
            $data['iban'] = $values['refund_iban'];
            $data['bic'] = $values['refund_bic'];
        }
        if (isset($values['refund_ref']) && $values['refund_ref']) $data['refund_ref'] = $values['refund_ref'];

        $response = commerce_novalnet_drupal_request(http_build_query($data), 'https://payport.novalnet.de/paygate.jsp');

        if (!isset($response->data)) {
            drupal_set_message(check_plain($response->error), 'error');
            return FALSE;
        }
        parse_str($response->data, $result); #Parse Paygate response into array
        if ($result['status'] == NOVALNET_SUCCESS_STATUS) {
            $total_refunded_amount = (string)($trxn_details['refunded_amount'] + $data['refund_param']);

            $settings = commerce_novalnet_load_settings();
            $order_status = ($total_refunded_amount >= $trxn_details['org_total'])? $settings['onhold_cancel_status'] : $order->status;
            $formated_amount = number_format($data['refund_param']/100, 2, ',', '') . ' ' . $trxn_details['currency'];
            $comments = t('The refund has been executed for the TID:@tid with the amount of @amount.', array('@tid' => $trxn_details['tid'], '@amount' => $formated_amount));
            if (isset($result['tid']) && !empty($result['tid'])) {
                $comments .= ' ' . t('Your new TID for the refund amount: @newtid.', array('@newtid' => $result['tid']));
            }
            if ($trxn_details['payment_id'] == 34 && isset($result['paypal_refund_tid']) && $result['paypal_refund_tid'] != '') {
                    $comments .= ' - PayPal Ref: ' . $result['paypal_refund_tid'];
            }
            $payment_method_id = explode('|', $order->data['payment_method']);
            commerce_novalnet_new_transaction( array(
                  'payment_method' => $payment_method_id['0'],
                  'order_id' => $order->order_id,
                  'instance_id' => $order->data['payment_method'],
                  'amount' => 0,
                  'currency_code' => $trxn_details['currency'],
                  'message' => $comments,
                  'remote_id' => $data['tid']));
            commerce_order_status_update($order, $order_status, FALSE, TRUE, $comments);
            $trans_info = commerce_novalnet_do_xml_request($data, 'TRANSACTION_STATUS');
            commerce_novalnet_live_transaction_update(array('amount' => $trans_info['amount'], 'gateway_status' => $trans_info['status'], 'refunded_amount' => $total_refunded_amount), $order->order_number);
            if ($trxn_details['payment_id'] == 27) {
                commerce_novalnet_update_invoice_details(array('amount' => $trans_info['amount']), $order->order_number);
            }
            drupal_set_message(check_plain(commerce_novalnet_get_errormessage($result)));
        }
        else {
            drupal_set_message(check_plain(commerce_novalnet_get_errormessage($result) . '(' . $result['status'] . ')'), 'error');
        }
    }
    elseif ($values['nn_process'] == 'subs_cancel') {
        $reasons = commerce_novalnet_get_subscription_cancel_reasons();
        $data = array(
            'vendor'        => $trxn_details['vendor_id'],
            'product'       => $trxn_details['product_id'],
            'tariff'        => $trxn_details['tariff_id'],
            'key'           => $trxn_details['payment_id'],
            'auth_code'     => $trxn_details['authcode'],
            'cancel_sub'    => '1',
            'tid'           => $trxn_details['tid'],
            'cancel_reason' => $reasons[$values['cancel_reason']],
        );
        $response = commerce_novalnet_drupal_request(http_build_query($data), 'https://payport.novalnet.de/paygate.jsp');
        if (!isset($response->data)) {
            drupal_set_message(check_plain($response->error), 'error');
            return FALSE;
        }
        parse_str($response->data, $result); #Parse Paygate response into array
        if ($result['status'] == NOVALNET_SUCCESS_STATUS) {
            $comments =  t('Subscription has been canceled due to:') . $reasons[$values['cancel_reason']];

            $settings = commerce_novalnet_load_settings();
            $payment_method_id = explode('|', $order->data['payment_method']);
            commerce_novalnet_new_transaction( array(
                      'payment_method' => $payment_method_id['0'],
                      'order_id' => $order->order_id,
                      'instance_id' => $order->data['payment_method'],
                      'amount' => 0,
                      'currency_code' => $trxn_details['currency'],
                      'message' => $comments,
                      'remote_id' => $data['tid']));
            commerce_order_status_update($order, $settings['subs_cancel_status'], FALSE, TRUE, $comments);
            $trans_info = commerce_novalnet_do_xml_request($data, 'TRANSACTION_STATUS');
            commerce_novalnet_live_transaction_update(array('gateway_status' => $trans_info['status']), $order->order_number);
            commerce_novalnet_subscription_update(array('termination_reason' => $data['cancel_reason'],
                                                    'termination_at' => time()), $order->order_number);
            drupal_set_message(check_plain($comments));
        }
        else {
            drupal_set_message(check_plain(commerce_novalnet_get_errormessage($result) . '(' . $result['status'] . ')'), 'error');
        }
    }
}

/**
 * Load transaction details for the order
 *
 * @param $order_id integer
 * @return array
 */
function commerce_novalnet_load_trxn_details($order_id) {
    $query = db_select('commerce_novalnet_transaction_detail', 'trxn')
            ->fields('trxn')
            ->condition('trxn.order_no', $order_id)
            ->execute();
    if ($query) return $query->fetchAssoc();
    return FALSE;
}

/**
 * update the transaction details
 *
 * @param $data array
 * @param $order_id integer
 * @return none
 */
function commerce_novalnet_live_transaction_update($data, $order_id) {
    $query = db_update('commerce_novalnet_transaction_detail')
            ->fields($data)
            ->condition('order_no', $order_id)
            ->execute();
}

/**
 * load the invoice transaction details
 *
 * @param $order_id integer
 * @return array
 */
function commerce_novalnet_load_invoice_details($order_id) {
    $query = db_select('commerce_novalnet_invoice_trxn_detail', 'trxn')
            ->fields('trxn', array('tid', 'bank_name', 'bank_city', 'amount', 'currency', 'bank_iban', 'bank_bic', 'due_date'))
            ->condition('trxn.order_no', $order_id)
            ->execute();
    if ($query) return $query->fetchAssoc();
    return FALSE;
}
/**
 * Update the invoice details table
 *
 * @param $data array
 * @param $order_id integer
 * @return none
 */
function commerce_novalnet_update_invoice_details($data, $order_id) {
    $query = db_update('commerce_novalnet_invoice_trxn_detail')
            ->fields($data)
            ->condition('order_no', $order_id)
            ->execute();
}

/**
 * load the subscription transaction details
 *
 * @param $order_id integer
 * @return array
 */
function commerce_novalnet_load_subscription_details($order_id) {
    $query = db_select('commerce_novalnet_subscription_detail', 'trxn')
            ->fields('trxn')
            ->condition('trxn.order_no', $order_id)
            ->condition('trxn.termination_reason', NULL)
            ->execute();

    if ($query) return $query->fetchAssoc();
    return FALSE;
}

/**
 * update the subscription details
 *
 * @param $order_no integer
 * @param $data array
 * @return none
 */
function commerce_novalnet_subscription_update($data, $order_no) {
    $query = db_update('commerce_novalnet_subscription_detail')
            ->fields($data)
            ->condition('order_no', $order_no)
            ->execute();
}

/**
 * Implements process subscription cancel process
 *
 * @param $order_id integer
 * @param $form array
 * @param $form_state array
 * @return none
 */
function commerce_novalnet_user_subscription_cancel($form, &$form_state, $order_id) {
    global $user;
    if (empty($_REQUEST['cancel_reason'])) {
        drupal_set_message(t('Please select the reason of subscription cancellation'), 'error');
        drupal_goto('/user/' . $user->uid . '/orders/' . $order_id);
    }
    $reasons = commerce_novalnet_get_subscription_cancel_reasons();
    $order = commerce_order_load($order_id);
    $trxn_details = commerce_novalnet_load_trxn_details($order->order_number);
    $data = array(
        'vendor'        => $trxn_details['vendor_id'],
        'product'       => $trxn_details['product_id'],
        'tariff'        => $trxn_details['tariff_id'],
        'key'           => $trxn_details['payment_id'],
        'auth_code'     => $trxn_details['authcode'],
        'cancel_sub'    => 1,
        'tid'           => $trxn_details['tid'],
        'cancel_reason' => $reasons[$_REQUEST['cancel_reason']],
    );
    if (is_numeric($data['vendor']) && is_numeric($data['product']) && is_numeric($data['tariff'])
        && !empty($data['auth_code']) && !empty($data['tid'])&& !empty($data['key'])) {
        $response = commerce_novalnet_drupal_request(http_build_query($data), 'https://payport.novalnet.de/paygate.jsp');
        if (!isset($response->data)) {
            drupal_set_message(check_plain($response->error), 'error');
            return FALSE;
        }
        parse_str($response->data, $result); #Parse Paygate response into array
        if ($result['status'] == NOVALNET_SUCCESS_STATUS) {
            $comments =  t('Subscription has been canceled due to:') . $data['cancel_reason'];
            $settings = commerce_novalnet_load_settings();
            $payment_method_id = explode('|', $order->data['payment_method']);
            commerce_novalnet_new_transaction( array(
                          'payment_method' => $payment_method_id['0'],
                          'order_id' => $order->order_id,
                          'instance_id' => $order->data['payment_method'],
                          'amount' => 0,
                          'currency_code' => $trxn_details['currency'],
                          'message' => $comments,
                          'remote_id' => $data['tid']));
            commerce_order_status_update($order, $settings['subs_cancel_status'], FALSE, TRUE, $comments);
            $trans_info = commerce_novalnet_do_xml_request($data, 'TRANSACTION_STATUS');
            commerce_novalnet_live_transaction_update(array( 'gateway_status' => $trans_info['status']), $order->order_number);
            commerce_novalnet_subscription_update(array('termination_reason' => $data['cancel_reason'],
                                                        'termination_at' => time()), $order->order_number);
            drupal_set_message(check_plain($comments));
            drupal_goto('/user/' . $user->uid . '/orders/' . $order_id);
        }
        else {
            drupal_set_message(check_plain(commerce_novalnet_get_errormessage($result) . '(' . $result['status'] . ')'), 'error');
            drupal_goto('/user/' . $user->uid . '/orders/' . $order_id);
        }
    }
    else {
       drupal_set_message(t('Please fill in all the mandatory fields'), 'error');
       drupal_goto('/user/' . $user->uid . '/orders/' . $order_id);
    }
}

/**
 * Adds new transaction for the order
 *
 * @param $data array
 * @return none
 */
function commerce_novalnet_new_transaction($data) {
    $transaction = commerce_payment_transaction_new($data['payment_method'], $data['order_id']);
    $transaction->instance_id = $data['instance_id'];
    $transaction->payment_method = $data['payment_method'];
    $transaction->amount = $data['amount'];
    $transaction->currency_code = $data['currency_code'];
    $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
    $transaction->message_variables = array('@name' => $data['payment_method']);
    $transaction->message = $data['message'];
    $transaction->remote_id =  $data['remote_id'];
    commerce_payment_transaction_save($transaction);
}

/**
 * get the amount paid for invoice
 *
 * @param $tid integer
 * @return boolean|double
 */
function commerce_novalnet_get_invoice_paidamount($tid) {
    $result = db_select('commerce_novalnet_callback_history', 'trxn')
            ->fields('trxn', array('amount'))
            ->condition('trxn.org_tid', $tid);
    $result->addExpression('SUM(amount)', 'paid_amount');
    $paid_amount = $result->execute()->fetchAssoc();
    if ($paid_amount) return $paid_amount;
    return FALSE;
}

/**
 * Prepare comments for the amount update
 *
 * @param $data array
 * @param $inv_bank_details array
 * @return string
 */
function commerce_novalnet_build_amountupdate_comment($inv_bank_details, $data, $order_number) {
    $bank_details = array(
        'invoice_iban' => $inv_bank_details['bank_iban'],
        'invoice_bic' => $inv_bank_details['bank_bic'],
        'invoice_bankname' => $inv_bank_details['bank_name'],
        'invoice_bankplace' => $inv_bank_details['bank_city'],
        'amount' => $data['amount']/100,
        'currency' => $inv_bank_details['currency'],
        'tid' => $inv_bank_details['tid'],
        'due_date' => $data['due_date'],
    );
    return commerce_novalnet_prepare_invoice_comments($bank_details, array('order_no' => $order_number,
                                                                             'invoice_ref' => 'BNR-' . $data['product'] . '-' . $order_number));
}
