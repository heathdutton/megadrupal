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
 * Script : commerce_novalnet_validation.inc
 *
 */
module_load_include('inc', 'commerce_novalnet', 'includes/commerce_novalnet_core');

/**
 *  Novalnet data validation
 *
 * @param $payment_method array
 * @param $pane_values array
 * @return boolean
 */
function commerce_novalnet_validation($payment_method, $pane_values) {
    $status = TRUE;

    if (isset($_SESSION['fraud_module'][$payment_method['method_id']])) {
        $status = commerce_novalnet_validate_fraud_input($pane_values, $payment_method['method_id']);
    }
    if ($payment_method['method_id'] == 'commerce_novalnet_cc') {
        if ((!isset($pane_values['nn_cc_cvv']) || trim($pane_values['nn_cc_cvv']) == '')
            || (!isset($pane_values['nn_cc_panhash']) || $pane_values['nn_cc_panhash'] == '')
                || (!isset($pane_values['nn_cc_unique_id']) || $pane_values['nn_cc_unique_id'] == '')) {
                drupal_set_message(t('Your credit card details are invalid'), 'error');
                $status = FALSE;
        }
    }
    elseif ($payment_method['method_id'] == 'commerce_novalnet_sepa') {
        if ((!isset($pane_values['sepa_cardholder']) || trim($pane_values['sepa_cardholder']) == '')
            || (!isset($pane_values['nn_sepa_panhash']) || $pane_values['nn_sepa_panhash'] == '')
                || (!isset($pane_values['sepa_unique_id']) || $pane_values['sepa_unique_id'] == '')) {
                drupal_set_message(t('Your account details are invalid'), 'error');
                $status = FALSE;
        }
    }
    return $status;
}

/**
 *  fraud module input validation
 *
 * @param $payment string
 * @param $pane_values array
 * @return boolean
 */
function commerce_novalnet_validate_fraud_input($pane_values, $payment) {
    if ($_SESSION['fraud_module'][$payment] == NOVALNET_FRAUD_MODULES_EMAIL
        && !valid_email_address(trim($pane_values[$payment . '_fraud_mail']))) {
        drupal_set_message(t('Your E-mail address is invalid'), 'error');
        return FALSE;
    }
    elseif ($_SESSION['fraud_module'][$payment] == NOVALNET_FRAUD_MODULES_CALLBACK
        && !is_numeric(trim($pane_values[$payment . '_fraud_tel']))) {
        drupal_set_message(t('Please enter your telephone number'), 'error');
        return FALSE;
    }
    elseif ($_SESSION['fraud_module'][$payment] == NOVALNET_FRAUD_MODULES_SMS
        && !is_numeric(trim($pane_values[$payment . '_fraud_mobile']))) {
        drupal_set_message(t('Please enter your mobile number'), 'error');
        return FALSE;
    }
    return TRUE;
}

/**
 * check the fraud module status
 *
 * @param $settings array
 * @param $order object
 * @param $payment string
 * @return boolean
 */
function commerce_novalnet_validate_fraud_module_status($settings, $order, $payment) {
    if ($payment == 'commerce_novalnet_cc' && $settings['commerce_novalnet_cc_secure']) {
        if (isset($_SESSION['fraud_module'][$payment])) unset($_SESSION['fraud_module'][$payment]);
        return FALSE;
    }
    $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
    $billing_address = $order_wrapper->commerce_customer_billing->commerce_customer_address->value();
    $limit = $settings[$payment . '_fraud_limit'];
    $fraud_module  = $settings[$payment . '_fraud_module'];
    $_SESSION['fraud_module'][$payment] = FALSE;
    if (in_array($billing_address['country'], unserialize(NOVALNET_FRAUD_MODULES_ALLOWED_COUNTRIES))
        && in_array($fraud_module, array(NOVALNET_FRAUD_MODULES_EMAIL, NOVALNET_FRAUD_MODULES_SMS, NOVALNET_FRAUD_MODULES_CALLBACK))
        && (empty($limit) || $order->commerce_order_total['und'][0]['amount'] >= $limit)) {
        $_SESSION['fraud_module'][$payment] = $fraud_module;
        return TRUE;
    }

    return FALSE;
}

/**
 * check the fraud module input values
 *
 * @param $order object
 * @param $pane_values array
 * @param $payment string
 * @return boolean
 */
function commerce_novalnet_fraudmodule_input_validation($payment, $pane_values, $order) {

    if (in_array($_SESSION['fraud_module'][$payment], array(NOVALNET_FRAUD_MODULES_SMS, NOVALNET_FRAUD_MODULES_CALLBACK))
        && empty($pane_values[$payment . '_new_pin']) && (!isset($pane_values[$payment . '_pin_number']) || !ctype_alnum(trim($pane_values[$payment . '_pin_number'])))) {
        drupal_set_message(t('The PIN you entered is incorrect'), 'error');
        return FALSE;
    }
    if ($order->commerce_order_total['und'][0]['amount'] != $_SESSION['fraud_module']['gateway_request'][$payment]['amount']) {
        unset($_SESSION['fraud_module']);
        unset($_SESSION['novalnet']);
        drupal_set_message(t('The order amount has been changed, please proceed with the new order'), 'error');
        return FALSE;
    }
    return TRUE;
}
