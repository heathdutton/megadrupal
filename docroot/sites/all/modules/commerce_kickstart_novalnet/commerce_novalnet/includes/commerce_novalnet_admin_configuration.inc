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
 * Script : commerce_novalnet_admin_configuration.inc
 *
 */
module_load_include('inc', 'commerce_novalnet', 'includes/commerce_novalnet_core');

/**
 * Form hook to diplay the Novalnet admin portal
 *
 * @param none
 * @return array
 */
function commerce_novalnet_adminportal_form() {
    $form['adminportal'] = array(
    '#markup' => "<div class='nn_map_header'>" . t('Login here with Novalnet merchant credentials. For the activation of new payment methods please contact <a href=\'mailto:support@novalnet.de\'>support@novalnet.de</a>') . "</div><br><iframe src='https://admin.novalnet.de' width='100%' height='750px' frameborder='0' scrolling='no'></iframe>"
    );
    drupal_add_css(drupal_get_path('module', 'commerce_novalnet') . '/css/novalnet.css');

    return $form;
}

/**
 * Form hook to diplay the Novalnet settings form
 *
 * @param none
 * @return array
 */
function commerce_novalnet_settings_form() {
    global $base_url, $language;

    $config             = commerce_novalnet_load_settings();
    $statuses             = commerce_order_status_options_list();
    unset($statuses['Shopping cart'], $statuses['Checkout']);
    $options_yesno    = array('0' => t('No'), '1' => t('Yes'));
    $options_pin        = array('0' => t('None'), NOVALNET_FRAUD_MODULES_EMAIL => t('Reply via E-mail'), NOVALNET_FRAUD_MODULES_SMS => t('PIN by SMS'), NOVALNET_FRAUD_MODULES_CALLBACK => t('PIN by callback'));

    $form['adminlink'] = array(
        '#markup' =>  l('<a href="' . t('https://www.novalnet.de') . '" target="_blank"><span style="float:right"><img src="' . NOVALNET_IMAGES_PATH . 'novalnet.png" alt="Novalnet AG" title="Novalnet AG"></span></a>', 'image/path', array('html' => TRUE)) . t('For additional configurations login to <a href=\'@adminlink\' target=\'_blank\'> Novalnet Administration Portal</a>. To login to the Portal you need to have an account at Novalnet. If you don\'t have one yet, please contact <a href=\'mailto:sales@novalnet.de\'>sales@novalnet.de</a> / tel. +49 (089) 923068320<br>To use the PayPal payment method please enter your PayPal API details in <a href=\'@adminlink\' target=\'_blank\'>Novalnet Merchant Administration</a> portal', array('@adminlink' => url('admin/commerce/config/novalnet/adminportal', array('attributes' => array('target' => '_blank','absolute' => TRUE))))),
        '#prefix' => '<div style="color:gray;font-weight:bold">',
        '#suffix' => '</div>'
    );

    foreach (commerce_novalnet_set_fieldsets() as $k => $v) {
        $form[$k] = array(
            '#type'                 => 'fieldset',
            '#title'                => check_plain($v),
            '#collapsible'  => TRUE,
            '#collapsed'        => TRUE,
        );
    }

    $form['commerce_novalnet_global'][]         = commerce_novalnet_set_global_configuration($statuses, $options_yesno, $config);
    $form['commerce_novalnet_cc'][]                 = commerce_novalnet_set_credit_card_configuration($statuses, $options_yesno, $options_pin, $config);
    $form['commerce_novalnet_sepa'][]           = commerce_novalnet_set_sepa_configuration($statuses, $options_yesno, $options_pin, $config);
    $form['commerce_novalnet_invoice'][]        = commerce_novalnet_set_invoice_configuration($statuses, $options_yesno, $options_pin, $config);
    $form['commerce_novalnet_prepayment'][] = commerce_novalnet_set_prepayment_configuration($statuses, $options_yesno, $config);
    $form['commerce_novalnet_paypal'][]         = commerce_novalnet_set_paypal_configuration($statuses, $options_yesno, $config);
    $form['commerce_novalnet_instant'][]        = commerce_novalnet_set_instant_configuration($statuses, $options_yesno, $config);
    $form['commerce_novalnet_ideal'][]          = commerce_novalnet_set_ideal_configuration($statuses, $options_yesno, $config);
    $form['commerce_novalnet_eps'][]                = commerce_novalnet_set_eps_configuration($statuses, $options_yesno, $config);
    $form['commerce_novalnet_callback'][]   = commerce_novalnet_set_callback_configuration($options_yesno, $config);

    $form['novalnet_submit'] = array(
            '#type'                 => 'submit',
            '#value'                => t('Update'),
    );

    return $form;
}

/**
 * set the field sets for configuration
 *
 * @param none
 * @return array
 */
function commerce_novalnet_set_fieldsets() {
    return array(
        'commerce_novalnet_global'      => t('Global Configuration'),
        'commerce_novalnet_cc'              => t('Credit Card'),
        'commerce_novalnet_sepa'            => t('Direct Debit SEPA'),
        'commerce_novalnet_invoice'     => t('Invoice'),
        'commerce_novalnet_prepayment' => t('Prepayment'),
        'commerce_novalnet_paypal'      => t('PayPal'),
        'commerce_novalnet_instant'     => t('Instant Bank Transfer'),
        'commerce_novalnet_ideal'           => t('iDEAL'),
        'commerce_novalnet_eps'             => t('EPS'),
        'commerce_novalnet_callback'    => t('Merchant script management'),
    );
}

/**
 * Implements hook_form_validate to validate the configuration details
 *
 * @param $form array
 * @param $form_state array
 * @return boolean
 */
function commerce_novalnet_settings_form_validate($form, &$form_state) {
    $error_status = FALSE;
    $values = array_map('trim', $form_state['values']);
    $values = array_map('strip_tags', $values);
    $email_check = array('callback_notify_to', 'callback_notify_bcc');
    $mandatory_digit = array('vendor_id', 'product_id', 'tariff_id');
    $mandatory_string = array('auth_code', 'access_key');
    $digit_ifnot_empty = array('tariff_period2_amount', 'manual_check_limit', 'referrer_id', 'novalnet_cc_fraud_limit', 'novalnet_sepa_fraud_limit', 'novalnet_invoice_fraud_limit');
    if (trim($values['tariff_period2_amount']) != '') array_push($mandatory_string, 'tariff_period2');
    foreach ($mandatory_digit as $k => $v) {
        if (!is_numeric($values[$v]))  {
            form_set_error($v);
            $error_status = TRUE;
        }
    }
    foreach ($mandatory_string as $k => $v) {
        if (empty($values[$v]))  {
            form_set_error($v);
            $error_status = TRUE;
        }
    }
    foreach ($digit_ifnot_empty as $k => $v) {
        if (!empty($values[$v]) && !is_numeric($values[$v]))  {
            form_set_error($v);
            $error_status = TRUE;
        }
    }
    if ($values['commerce_novalnet_sepa_duration'] !='' && (!is_numeric($values['commerce_novalnet_sepa_duration']) || $values['commerce_novalnet_sepa_duration']<7)) {
            form_set_error('commerce_novalnet_sepa_duration');
            $error_status = TRUE;
    }
    foreach ($email_check as $k => $v) {
        $email_values = explode(',', $values[$v]);
        $total = count($email_values);
        for ($i = 0; $i < $total; $i++) {
            $email_values[$i] = trim($email_values[$i]);
            if (!empty($email_values[$i]) && !valid_email_address($email_values[$i]))  {
                form_set_error($v);
                $error_status = TRUE;
                continue;
            }
        }
    }
    if ($error_status) {
        drupal_set_message(t('Please fill in all the mandatory fields'), 'error');
        return FALSE;
    }
    $form_state['values'] = $values;
}

/**
 * Implements hook_form_submit to save the configuration details
 *
 * @param $form array
 * @param $form_state array
 * @return boolean
 */
function commerce_novalnet_settings_form_submit($form, &$form_state) {
    $values = $form_state['values'];
    variable_set('novalnet_config', $values);
    drupal_set_message(t('Successful'));
    return TRUE;
}

/**
 * sets the global configuration form details
 *
 * @param $statuses array
 * @param $options_yesno array
 * @param $config array
 * @return array
 */
function commerce_novalnet_set_global_configuration($statuses, $options_yesno, $config) {
    $form['vendor_id'] = array(
        '#type'                     => 'textfield',
        '#title'                    => t('Merchant ID'),
        '#description'      => t('Enter Novalnet merchant ID'),
        '#default_value'    => (isset($config['vendor_id']) ? $config['vendor_id'] : ''),
    );
    $form['auth_code'] = array(
        '#type'                     => 'textfield',
        '#title'                    => t('Authentication code'),
        '#description'      => t('Enter Novalnet authentication code'),
        '#default_value'    => (isset($config['auth_code']) ? $config['auth_code'] : ''),
    );
    $form['product_id'] = array(
        '#type'                     => 'textfield',
        '#title'                    => t('Project ID'),
        '#description'      => t('Enter Novalnet project ID'),
        '#default_value'    => (isset($config['product_id']) ? $config['product_id'] : '')
    );
    $form['tariff_id'] = array(
        '#type'                     => 'textfield',
        '#title'                    => t('Tariff ID'),
        '#description'      => t('Enter Novalnet tariff ID'),
        '#default_value'    => (isset($config['tariff_id']) ? $config['tariff_id'] : '')
    );

    $form['access_key'] = array(
        '#type'                     => 'textfield',
        '#title'                    => t('Payment access key'),
        '#description'      => t('Enter the Novalnet payment access key'),
        '#default_value'    => (isset($config['access_key']) ? $config['access_key'] : '')
    );
    $form['manual_check_limit'] = array(
        '#type'                     => 'textfield',
        '#title'                    => t('Set a limit for on-hold transaction') . ' (' . t('in cents') . ')',
        '#description'      => t('In case the order amount exceeds mentioned limit, the transaction will be set on hold till your confirmation of transaction'),
        '#default_value'    => (isset($config['manual_check_limit']) ? $config['manual_check_limit'] : '')
    );
    $form['referrer_id'] = array(
        '#type'                     => 'textfield',
        '#title'                    => t('Referrer ID'),
        '#description'      => t('Enter the referrer ID of the person/company who recommended you Novalnet'),
        '#default_value'    => (isset($config['referrer_id']) ? $config['referrer_id'] : '')
    );
    $form['auto_refill'] = array(
        '#type'                     => 'select',
        '#title'                    => t('Enable auto-fill'),
        '#description'      => t('The payment details will be filled automatically in the payment form during the checkout process'),
        '#options'              => $options_yesno,
        '#default_value'    => (isset($config['auto_refill']) ? $config['auto_refill'] : '0')
    );

    $form['status_mgmt'] = array(
        '#markup' => '<h3>' . t('Order status management for on-hold transaction(-s)') . '</h3>'
    );
    $form['onhold_comp_status'] = array(
        '#type'                     => 'select',
        '#title'                    => t('Confirmation order status'),
        '#description'      => '',
        '#options'              => $statuses,
        '#default_value'    => (isset($config['onhold_comp_status']) ? $config['onhold_comp_status'] : 'processing')
    );
    $form['onhold_cancel_status'] = array(
        '#type'                     => 'select',
        '#title'                    => t('Cancellation order status'),
        '#description'      => '',
        '#options'              => $statuses,
        '#default_value'    => (isset($config['onhold_cancel_status']) ? $config['onhold_cancel_status'] : 'canceled')
    );
    $form['sub_mgmt'] = array(
        '#markup' => '<h3>' . t('Dynamic subscription management') . '</h3>'
    );
    $form['tariff_period'] = array(
        '#type'                     => 'textfield',
        '#title'                    => t('Tariff period'),
        '#description'      => t('The period of the first subscription cycle (E.g: 1d/1m/1y)'),
        '#default_value'    => (isset($config['tariff_period']) ? $config['tariff_period'] : '')
    );
    $form['tariff_period2_amount'] = array(
        '#type'                     => 'textfield',
        '#title'                    => t('Amount for the subsequent subscription cycle (in cents)'),
        '#description'      => t('The amount for the subsequent subscription cycle'),
        '#default_value'    => (isset($config['tariff_period2_amount']) ? $config['tariff_period2_amount'] : '')
    );
    $form['tariff_period2'] = array(
        '#type'                     => 'textfield',
        '#title'                    => t('Period for subsequent subscription cycle'),
        '#description'      => t('The period of the subsequent subscription cycle (E.g: 1d/1m/1y)'),
        '#default_value'    => (isset($config['tariff_period2']) ? $config['tariff_period2'] : '')
    );
     $form['subs_cancel_status'] = array(
        '#type'                     => 'select',
        '#title'                    => t('Cancellation status of subscription'),
        '#description'      => '',
        '#options'              => $statuses,
        '#default_value'    => (isset($config['subs_cancel_status']) ? $config['subs_cancel_status'] : 'canceled')
    );
    return $form;
}

/**
 * sets the Credit card configuration form details
 *
 * @param $statuses array
 * @param $options_yesno array
 * @param $options_pin array
 * @param $config array
 * @return array
 */
function commerce_novalnet_set_credit_card_configuration($statuses, $options_yesno, $options_pin, $config) {
    $form[] = commerce_novalnet_payment_logo_markup('commerce_novalnet_cc');
     $form[] = commerce_novalnet_testmode_field('commerce_novalnet_cc', $options_yesno, $config);
    $form[] = commerce_novalnet_fraudmodule_fields('commerce_novalnet_cc', $options_pin, $config);
    $form['commerce_novalnet_cc_secure'] = array(
        '#type'                     => 'select',
        '#title'                    => t('Enable 3D secure'),
        '#description'                  => t('The 3D-Secure will be activated for credit cards. The issuing bank prompts the buyer for a password what, in turn, help to prevent a fraudulent payment. It can be used by the issuing bank as evidence that the buyer is indeed their card holder. This is intended to help decrease a risk of charge-back.'),
        '#options'              => $options_yesno,
        '#default_value'    => (isset($config['commerce_novalnet_cc_secure']) ? $config['commerce_novalnet_cc_secure'] : 0)
    );
    $form['commerce_novalnet_cc_amex'] = array(
        '#type'                     => 'select',
        '#title'                    => t('Enable AMEX'),
        '#options'              => $options_yesno,
        '#default_value'    => (isset($config['commerce_novalnet_cc_amex']) ? $config['commerce_novalnet_cc_amex'] : 0)
    );
    $form['commerce_novalnet_cc_year_limit'] = array(
        '#type'                     => 'textfield',
        '#title'                    => t('Limit for expiry year'),
        '#description'      => t('Enter the number for the maximum limit of credit card expiry year. In case if the field is empty, limit of 25 years from the current year will be set by default'),
        '#default_value'    => (isset($config['commerce_novalnet_cc_year_limit']) ? $config['commerce_novalnet_cc_year_limit'] : '25')
    );
    $form[] = commerce_novalnet_additional_fields('commerce_novalnet_cc', $statuses, $options_yesno, $config);

  return $form;
}


/**
 * sets the Direct Debit configuration form details
 *
 * @param $statuses array
 * @param $options_yesno array
 * @param $options_pin array
 * @param $config array
 * @return array
 */
function commerce_novalnet_set_sepa_configuration($statuses, $options_yesno, $options_pin, $config) {
        $form[] = commerce_novalnet_payment_logo_markup('commerce_novalnet_sepa');
        $form[] = commerce_novalnet_testmode_field('commerce_novalnet_sepa', $options_yesno, $config);
        $form[] = commerce_novalnet_fraudmodule_fields('commerce_novalnet_sepa', $options_pin, $config);
        $form['commerce_novalnet_sepa_duration'] = array(
            '#type'                     => 'textfield',
            '#title'                    => t('SEPA payment duration (in days)'),
            '#description'      => t("Enter the number of days after which the payment should be processed (must be greater than 6 days)"),
            '#default_value'    => (isset($config['commerce_novalnet_sepa_duration']) ? $config['commerce_novalnet_sepa_duration'] : '')
        );
        $form['commerce_novalnet_sepa_payment_refill'] = array(
            '#type'                     => 'select',
            '#title'                    => t('Enable auto-fill for payment data'),
            '#description'                  => t('For the registered users SEPA direct debit details will be filled automatically in the payment form'),
            '#options'              => $options_yesno,
            '#default_value'    => (isset($config['commerce_novalnet_sepa_payment_refill']) ? $config['commerce_novalnet_sepa_payment_refill'] : 0)
        );
        $form[] = commerce_novalnet_additional_fields('commerce_novalnet_sepa', $statuses, $options_yesno, $config);

  return $form;
}

/**
 * sets the Invoice configuration form details
 *
 * @param $statuses array
 * @param $options_yesno array
 * @param $options_pin array
 * @param $config array
 * @return array
 */
function commerce_novalnet_set_invoice_configuration($statuses, $options_yesno, $options_pin, $config) {
        $form[] = commerce_novalnet_payment_logo_markup('commerce_novalnet_invoice');
        $form[] = commerce_novalnet_testmode_field('commerce_novalnet_invoice', $options_yesno, $config);
        $form[] = commerce_novalnet_fraudmodule_fields('commerce_novalnet_invoice', $options_pin, $config);
        $form['commerce_novalnet_invoice_duration'] = array(
            '#type'                     => 'textfield',
            '#title'                    => t('Payment due date (in days)'),
            '#description'      => t("Enter the number of days to transfer the payment amount to Novalnet (must be greater than 7 days). In case if the field is empty, 14 days will be set as due date by default"),
            '#default_value'    => (isset($config['commerce_novalnet_invoice_duration']) ? $config['commerce_novalnet_invoice_duration'] : '')
        );
        $form[] = commerce_novalnet_callbackstatus_field('commerce_novalnet_invoice', $statuses, $config);
        $form[] = commerce_novalnet_additional_fields('commerce_novalnet_invoice', $statuses, $options_yesno, $config);

  return $form;
}

/**
 * sets the Prepayment configuration form details
 *
 * @param $statuses array
 * @param $options_yesno array
 * @param $config array
 * @return array
 */
function commerce_novalnet_set_prepayment_configuration($statuses, $options_yesno, $config) {
        $form[] = commerce_novalnet_payment_logo_markup('commerce_novalnet_prepayment');
        $form[] = commerce_novalnet_testmode_field('commerce_novalnet_prepayment', $options_yesno, $config);
        $form[] = commerce_novalnet_callbackstatus_field('commerce_novalnet_prepayment', $statuses, $config);
        $form[] = commerce_novalnet_additional_fields('commerce_novalnet_prepayment', $statuses, $options_yesno, $config);

  return $form;
}

/**
 * sets the PayPal configuration form details
 *
 * @param $statuses array
 * @param $options_yesno array
 * @param $config array
 * @return array
 */
function commerce_novalnet_set_paypal_configuration($statuses, $options_yesno, $config) {
        $form[] = commerce_novalnet_payment_logo_markup('commerce_novalnet_paypal');
        $form[] = commerce_novalnet_testmode_field('commerce_novalnet_paypal', $options_yesno, $config);
        $form['commerce_novalnet_paypal_pending_status'] = array(
            '#type'                     => 'select',
            '#title'                    => t('Order status for the pending payment'),
            '#description'      => '',
            '#options'              => $statuses,
            '#default_value'    => (isset($config['commerce_novalnet_paypal_pending_status']) ? $config['commerce_novalnet_paypal_pending_status'] : 'pending')
        );
        $form[] = commerce_novalnet_additional_fields('commerce_novalnet_paypal', $statuses, $options_yesno, $config);

  return $form;
}

/**
 * sets the Instant bank transfer configuration form details
 *
 * @param $statuses array
 * @param $options_yesno array
 * @param $config array
 * @return array
 */
function commerce_novalnet_set_instant_configuration($statuses, $options_yesno, $config) {
        $form[] = commerce_novalnet_payment_logo_markup('commerce_novalnet_instant');
        $form[] = commerce_novalnet_testmode_field('commerce_novalnet_instant', $options_yesno, $config);
        $form[] = commerce_novalnet_additional_fields('commerce_novalnet_instant', $statuses, $options_yesno, $config);

  return $form;
}

/**
 * sets the iDEAL configuration form details
 *
 * @param $statuses array
 * @param $options_yesno array
 * @param $config array
 * @return array
 */
function commerce_novalnet_set_ideal_configuration($statuses, $options_yesno, $config) {
        $form[] = commerce_novalnet_payment_logo_markup('commerce_novalnet_ideal');
        $form[] = commerce_novalnet_testmode_field('commerce_novalnet_ideal', $options_yesno, $config);
        $form[] = commerce_novalnet_additional_fields('commerce_novalnet_ideal', $statuses, $options_yesno, $config);

  return $form;
}

/**
 * sets the EPS Payment configuration form details
 *
 * @param $statuses array
 * @param $options_yesno array
 * @param $config array
 * @return array
 */
function commerce_novalnet_set_eps_configuration($statuses, $options_yesno, $config) {
        $form[] = commerce_novalnet_payment_logo_markup('commerce_novalnet_eps');
        $form[] = commerce_novalnet_testmode_field('commerce_novalnet_eps', $options_yesno, $config);
        $form[] = commerce_novalnet_additional_fields('commerce_novalnet_eps', $statuses, $options_yesno, $config);

  return $form;
}

/**
 * sets the Callbackscript configuration form details
 *
 * @param $options_yesno array
 * @param $config array
 * @return array
 */

function commerce_novalnet_set_callback_configuration($options_yesno, $config) {

    $form['send_mail'] = array(
        '#type'                     => 'select',
        '#title'                    => t('Enable E-mail notification for callback'),
        '#description'      => '',
        '#options'              => $options_yesno,
        '#default_value'    => (isset($config['send_mail']) ? $config['send_mail'] : 1)
    );
    $form['test_mode'] = array(
        '#type'                     => 'select',
        '#title'                    => t('Enable test mode'),
        '#options'              => $options_yesno,
        '#default_value'    => (isset($config['test_mode']) ? $config['test_mode'] : 0)
    );
    $form['debug_mode'] = array(
        '#type'                     => 'select',
        '#title'                    => t('Enable debug mode'),
        '#description'      => t('Set the debug mode to execute the merchant script in debug mode'),
        '#options'              => $options_yesno,
        '#default_value'    => (isset($config['debug_mode']) ? $config['debug_mode'] : 0)
    );
    $form['callback_notify_to'] = array(
        '#type'                     => 'textfield',
        '#title'                    => t('E-mail address (To)'),
        '#description'    => t('E-mail address of the recipient'),
        '#default_value'    => (isset($config['callback_notify_to']) ? $config['callback_notify_to'] : '')
    );
    $form['callback_notify_bcc'] = array(
        '#type'                     => 'textfield',
        '#title'                    => t('E-mail address (Bcc)'),
        '#description'      => t('E-mail address of the recipient for BCC'),
        '#default_value'    => (isset($config['callback_notify_bcc']) ? $config['callback_notify_bcc'] : '')
    );

    return $form;
}

/**
 * sets the fraud modules configuration form details
 *
 * @param $payment array
 * @param $pin_options array
 * @param $config array
 * @return array
 */
function commerce_novalnet_fraudmodule_fields($payment, $pin_options, $config) {

    $form[$payment . '_fraud_module'] = array(
        '#type'                         => 'select',
        '#title'                        => t('Enable fraud prevention'),
        '#options'                  => $pin_options,
        '#description'          => t("To authenticate the buyer for a transaction, the PIN or E-Mail will be automatically generated and sent to the buyer. This service is only available for customers from DE, AT, CH"),
        '#default_value'        => (isset($config[$payment . '_fraud_module']) ? $config[$payment . '_fraud_module'] : 0));

    $form[$payment . '_fraud_limit'] = array(
        '#type'                     => 'textfield',
        '#title'                    => t('Minimum value of goods for the fraud module (in cents)'),
        '#description'      => t('Enter the minimum value of goods from which the fraud module should be activated'),
        '#default_value'    => (isset($config[$payment . '_fraud_limit']) ? $config[$payment . '_fraud_limit'] : ''));

    return $form;
}

/**
 * Set payment logo
 *
 * @param $payment string
 * @return array
 */
function commerce_novalnet_payment_logo_markup($payment) {
        $payment_title = commerce_novalnet_set_fieldsets();
        $form['payment_logo'] = array(
            '#markup' => '<a href="' . t('http://www.novalnet.de') . '" target="_blank"><span style="float:right"><img src="' . NOVALNET_IMAGES_PATH . $payment . '.png" alt="' . $payment_title[$payment] . '" title="' . $payment_title[$payment] . '"></span></a>',
            '#prefix' => '<div style="color:gray;font-weight:bold">',
            '#suffix' => '</div>'
        );

  return $form;
}

/**
 * Set test mode form field
 *
 * @param $payment string
 * @param $options_yesno array
 * @param $config array
 * @return array
 */
function commerce_novalnet_testmode_field($payment, $options_yesno, $config) {
    $form[$payment . '_testmode'] = array(
        '#type'                     => 'select',
        '#title'                    => t('Enable test mode'),
        '#description'                  => t('The payment will be processed in the test mode therefore amount for this transaction will not be charged'),
        '#options'              => $options_yesno,
        '#default_value'    => (isset($config[$payment . '_testmode']) ? $config[$payment . '_testmode'] : 0),

        );

  return $form;
}

/**
 * Set callbackstatus form field
 *
 * @param $payment string
 * @param $statuses array
 * @param $config array
 * @return array
 */
function commerce_novalnet_callbackstatus_field($payment, $statuses, $config) {
    $form[$payment . '_callback_status'] = array(
        '#type'                     => 'select',
        '#title'                    => t('Callback order status'),
        '#options'              => $statuses,
        '#default_value'    => (isset($config[$payment . '_callback_status']) ? $config[$payment . '_callback_status'] : 'processing')
    );

  return $form;
}

/**
 * Set additional form fields
 *
 * @param $payment string
 * @param $statuses array
 * @param $options_yesno array
 * @param $config array
 * @return array
 */
function commerce_novalnet_additional_fields($payment, $statuses, $options_yesno, $config) {
     $form[$payment . '_order_status'] = array(
            '#type'                     => 'select',
            '#title'                    => t('Order completion status'),
            '#options'              => $statuses,
            '#default_value'    => (isset($config[$payment . '_order_status']) ? $config[$payment . '_order_status'] : 'pending'),
            );
    $form[$payment . '_payment_logo'] = array(
            '#type'                     => 'select',
            '#title'                    => t('Display payment method logo'),
            '#description'      => t('The payment method logo will be displayed on the checkout page'),
            '#options'              => $options_yesno,
            '#default_value'    => (isset($config[$payment . '_payment_logo']) ? $config[$payment . '_payment_logo'] : 1));
    $form[$payment . '_novalnet_logo'] = array(
            '#type'                     => 'select',
            '#title'                    => t('Display Novalnet logo'),
            '#description'      => t('The Novalnet logo will be displayed on the checkout page'),
            '#options'              => $options_yesno,
            '#default_value'    => (isset($config[$payment . '_novalnet_logo']) ? $config[$payment . '_novalnet_logo'] : 1));
    $form[$payment . '_enduser_info'] = array(
            '#type'                     => 'textfield',
            '#title'                    => t('Notification for the buyer'),
            '#description'      => t('The entered text will be displayed on the checkout page'),
            '#default_value'    => (isset($config[$payment . '_enduser_info']) ? $config[$payment . '_enduser_info'] : ''));
    $form[$payment . '_trans_ref1'] = array(
            '#type'                     => 'textfield',
            '#title'                    => t('Transaction reference 1'),
            '#description'      => t('This reference will appear in your bank account statement'),
            '#default_value'    => (isset($config[$payment . '_trans_ref1']) ? $config[$payment . '_trans_ref1'] : ''));
    $form[$payment . '_trans_ref2'] = array(
            '#type'                     => 'textfield',
            '#title'                    => t('Transaction reference 2'),
            '#description'      => t('This reference will appear in your bank account statement'),
            '#default_value'    => (isset($config[$payment . '_trans_ref2']) ? $config[$payment . '_trans_ref2'] : ''));

    return $form;
}
