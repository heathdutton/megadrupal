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
 * Script : commerce_novalnet_core.inc
 *
 */

module_load_include('inc', 'commerce_novalnet', 'includes/commerce_novalnet_constants');
module_load_include('inc', 'commerce_novalnet', 'includes/commerce_novalnet_validation');
module_load_include('inc', 'commerce_novalnet', 'includes/commerce_novalnet_interface');
module_load_include('inc', 'commerce_novalnet', 'includes/commerce_novalnet_order_operations');

/**
 * used to set the available payment methods
 *
 * @param none
 * @return array
 */
function commerce_novalnet_load_payments() {
    $settings = commerce_novalnet_load_settings();
    $offsite_payments = array('commerce_novalnet_instant', 'commerce_novalnet_paypal', 'commerce_novalnet_ideal', 'commerce_novalnet_eps');
    $payment_methods  =  array(
        'commerce_novalnet_prepayment'  => array('title' => t('Prepayment')),
        'commerce_novalnet_invoice'     => array('title' => t('Invoice')),
        'commerce_novalnet_cc'          => array('title' => t('Credit Card')),
        'commerce_novalnet_sepa'        => array('title' => t('Direct Debit SEPA')),
        'commerce_novalnet_paypal'      => array('title' => t('PayPal')),
        'commerce_novalnet_instant'     => array('title' => t('Instant Bank Transfer')),
        'commerce_novalnet_ideal'       => array('title' => t('iDEAL')),
        'commerce_novalnet_eps'         => array('title' => t('EPS'))
    );
    foreach ($payment_methods as $key => $value) {
        $payment_methods[$key]['display_title'] = commerce_novalnet_set_display_title($key, $settings);
        $payment_methods[$key]['description']   = '';
        $payment_methods[$key]['active']        = FALSE;
        $payment_methods[$key]['terminal']      = FALSE;
        $payment_methods[$key]['callbacks']     = array(
            'settings_form'             => 'commerce_novalnet_payment_settings_form',
            'submit_form'               => 'commerce_novalnet_payment_submit_form',
            'submit_form_validate'      => 'commerce_novalnet_payment_submit_form_validate',
            'submit_form_submit'        => 'commerce_novalnet_payment_submit_form_submit',
            'redirect_form'             => 'commerce_novalnet_payment_redirect_form',
            'redirect_form_back'        => 'commerce_novalnet_payment_redirect_form_back',
            'redirect_form_validate'    => 'commerce_novalnet_payment_redirect_form_validate',
            'redirect_form_submit'      => 'commerce_novalnet_payment_redirect_form_submit'
        );
        if ($key == 'commerce_novalnet_cc' && isset($settings['commerce_novalnet_cc_secure'])
            && $settings['commerce_novalnet_cc_secure']) {
             array_push($offsite_payments, 'commerce_novalnet_cc');
        }
        $payment_methods[$key]['offsite'] = $payment_methods[$key]['offsite_autoredirect'] = (in_array($key, $offsite_payments));
    }

    return $payment_methods;
}

/**
 * build the checkout payment form
 *
 * @param $payment string
 * @param $pane_values array
 * @param $checkout_pane array
 * @param $order object
 * @return array
 */
function commerce_novalnet_checkout_payment_form($payment, $pane_values, $checkout_pane, $order) {
    $settings = commerce_novalnet_load_settings();
    $from=array();
    if ($payment !='commerce_novalnet_sepa' && isset($_SESSION['novalnet']['commerce_novalnet_sepa']['hash']))
        unset($_SESSION['novalnet']['commerce_novalnet_sepa']['hash']);
    if ($payment != 'commerce_novalnet_cc' && isset($_SESSION['novalnet']['commerce_novalnet_cc']['hash']))
        unset($_SESSION['novalnet']['commerce_novalnet_cc']['hash']);
    if (isset($_SESSION['nn_payment_select']) && $_SESSION['nn_payment_select'] != $payment)
            if (isset($_SESSION['fraud_module'])) unset($_SESSION['fraud_module']);
    $_SESSION['nn_payment_select'] = $payment;
    $form['novalnet_payment_description'] = array(
        '#markup' => commerce_novalnet_load_payment_description($payment) . '<br>',
    );
    if ($settings[$payment . '_enduser_info']) {
        $form['novalnet_enduser_information'] = array(
            '#markup' => $settings[$payment . '_enduser_info'] . '<br>',
        );
    }
    if ($settings[$payment . '_testmode']) {
        $form['novalnet_test_order_notification'] = array(
            '#markup' => '<span style=color:red >' . t('The payment will be processed in the test mode therefore amount for this transaction will not be charged') . '</span>',
        );
    }
    if ($payment == 'commerce_novalnet_cc' && !isset($_SESSION['fraud_module']['second_call'][$payment])) {
        $form = array_merge($form, commerce_novalnet_build_creditcard_form($settings, $order));
    }
    if ($payment == 'commerce_novalnet_sepa' && !isset($_SESSION['fraud_module']['second_call'][$payment])) {
        $form = array_merge($form, commerce_novalnet_build_sepa_form($settings, $order));
    }
    if (in_array($payment, unserialize(NOVALNET_FRAUD_MODULES_ALLOWED_PAYMENTS))
        && isset($_SESSION['fraud_module']['second_call'][$payment])) {
        $form = array_merge($form, commerce_novalnet_build_fraudmodule_pinform($payment));
    }
    elseif (in_array($payment, unserialize(NOVALNET_FRAUD_MODULES_ALLOWED_PAYMENTS))
        && commerce_novalnet_validate_fraud_module_status($settings, $order, $payment)) {
        $form = array_merge($form, commerce_novalnet_build_fraudmodule_form($payment, $order->mail));
    }
    return $form;
}

/**
 * load the novalnet configuration details
 *
 * @param none
 * @return array
 */
function commerce_novalnet_load_settings() {
    return variable_get('novalnet_config');
}

/**
 * set the novalnet payments description details
 *
 * @param $payment string
 * @return string
 */
function commerce_novalnet_load_payment_description($payment) {
    $description =  '';
    switch ($payment) {
        case 'commerce_novalnet_prepayment':
        case 'commerce_novalnet_invoice':
            $description = t("Once you've submitted the order, you will receive an e-mail with account details to make payment");
            break;
        case 'commerce_novalnet_cc':
            $description = t('The amount will be debited from your credit card once the order is submitted');
            break;
        case 'commerce_novalnet_sepa':
            $description = t('Your account will be debited upon the order submission');
            break;
        case 'commerce_novalnet_paypal':
        case 'commerce_novalnet_instant':
        case 'commerce_novalnet_ideal':
        case 'commerce_novalnet_eps':
            $description = t('After the successful verification, you will be redirected to Novalnet secure order page to proceed with the payment');
            break;
    }

    return $description;
}

/**
 * sets the title for the payments
 *
 * @param $payment string
 * @param $settings array
 * @return string
 */
function commerce_novalnet_set_display_title($payment, $settings) {
    $title = '';
    $payment_name = commerce_novalnet_set_payment_method_name($payment);

    if ($settings[$payment . '_novalnet_logo']) {
        $title .= l(t('<a href="' . t('http://www.novalnet.com') . '" target="_blank"><img src="' . NOVALNET_IMAGES_PATH . 'novalnet.png" alt="Novalnet AG" title="Novalnet AG"></a>'), 'image/path', array('html' => TRUE));
    }
    $title .= $payment_name;
    if ($settings[$payment . '_payment_logo']) {
        $title .= l(t('<span float="right"><a href="' . t('http://www.novalnet.com') . '" target="_blank"><img src="' . NOVALNET_IMAGES_PATH . $payment . '.png" alt="' . $payment_name . '" title="' . $payment_name . '"></a></span>'), 'image/path', array('html' => TRUE));

        if ($payment == 'commerce_novalnet_cc' && $settings['commerce_novalnet_cc_amex']) {
            $title .= l(t('<span float="right"><a href="' . t('http://www.novalnet.com') . '" target="_blank"><img src="' . NOVALNET_IMAGES_PATH . 'amex.png" alt="' . $payment_name . '" title="' . $payment_name . '" width="25px;"></a></span>'), 'image/path', array('html' => TRUE));
        }
    }

    return $title;
}

/**
 * sets the payment name
 *
 * @param $payment string
 * @return string
 */
function commerce_novalnet_set_payment_method_name($payment) {
    $payment_list =  array(
        'commerce_novalnet_cc'          => t('Credit Card'),
        'commerce_novalnet_sepa'        => t('Direct Debit SEPA'),
        'commerce_novalnet_invoice'     => t('Invoice'),
        'commerce_novalnet_prepayment'  => t('Prepayment'),
        'commerce_novalnet_paypal'      => t('PayPal'),
        'commerce_novalnet_instant'     => t('Instant Bank Transfer'),
        'commerce_novalnet_ideal'       => t('iDEAL'),
        'commerce_novalnet_eps'         => t('EPS'),
    );

    return $payment_list[$payment];
}
function commerce_novalnet_set_payment_method_info($payment, $key) {
    $payment_info = array(
    'commerce_novalnet_cc'          => array('key' => '6', 'payment_type' => 'CREDITCARD', 'url' => array('paygate_url' => 'https://payport.novalnet.de/paygate.jsp' , 'global_pci_port'   => 'https://payport.novalnet.de/global_pci_payport')),
    'commerce_novalnet_sepa'        => array('key' => '37', 'payment_type' => 'DIRECT_DEBIT_SEPA', 'url' => array('paygate_url' => 'https://payport.novalnet.de/paygate.jsp')),
    'commerce_novalnet_invoice'     => array('key' => '27', 'payment_type' => 'INVOICE', 'url' => array('paygate_url' => 'https://payport.novalnet.de/paygate.jsp' )),
    'commerce_novalnet_prepayment'  => array('key' => '27', 'payment_type' => 'PREPAYMENT' , 'url' => array('paygate_url' => 'https://payport.novalnet.de/paygate.jsp' )),
    'commerce_novalnet_paypal'      => array('key' => '34', 'payment_type' => 'PAYPAL', 'url' => array('paygate_url' => 'https://payport.novalnet.de/paypal_payport' )),
    'commerce_novalnet_instant'     => array('key' => '33', 'payment_type' => 'ONLINE_TRANSFER', 'url' => array('paygate_url' => 'https://payport.novalnet.de/online_transfer_payport')),
    'commerce_novalnet_ideal'       => array('key' => '49', 'payment_type' => 'IDEAL', 'url' => array('paygate_url' => 'https://payport.novalnet.de/online_transfer_payport')),
    'commerce_novalnet_eps'         => array('key' => '50', 'payment_type' => 'EPS' , 'url' => array('paygate_url' => 'https://payport.novalnet.de/eps_payport' )));

 return $payment_info[$payment][$key];
}

/**
 * build the sepa form
 *
 * @param $settings array
 * @param $order object
 * @return array
 */
function commerce_novalnet_build_sepa_form($settings, $order) {

    $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
    $billing_address    = $order_wrapper->commerce_customer_billing->commerce_customer_address->value();
    $name = commerce_novalnet_format_customer_name($billing_address['name_line'], $billing_address['first_name'], $billing_address['last_name']);
    drupal_add_css(drupal_get_path('module', 'commerce_novalnet') . '/css/novalnet.css');
    $data['vendor']     = $settings['vendor_id'];
    $data['auth_code']  = $settings['auth_code'];
    $data['customer_no']= $order->uid;
    commerce_novalnet_add_affiliate_details($data);
    $form['sepa_p_markup'] = array(
        '#markup'            => '<noscript><div class="messages error">' . t('Please enable the Javascript in your browser to proceed further with the payment') . '</div><input type="hidden" name="noscript"><div style="display:none"></noscript>',
    );
    $form['sepa_cardholder'] = array(
        '#title'            => t('Account holder'),
        '#type'             => 'textfield',
        '#attributes'       => array('id' => 'sepa_cardholder', 'autocomplete' => 'off', 'onchange' => 'on_change();', 'onkeypress' => 'return ibanbic_validate(event);'),
        '#size'             => 45,
        '#default_value'    => $name,
    );
    $form['sepa_country'] = array(
        '#title'            => t('Bank country'),
        '#type'             => 'select',
        '#attributes'       => array('id' => 'sepa_country'),
        '#options'          => country_get_list(),
        '#default_value'    => $billing_address['country'],
    );
    $form['sepa_iban'] = array(
        '#title'            => t('IBAN or Account number'),
        '#type'             => 'textfield',
        '#attributes'       => array('id' => 'sepa_iban', 'autocomplete' => 'off', 'name' => '', 'onchange' => 'on_change();', 'onkeypress' => 'return ibanbic_validate(event);'),
        '#field_suffix'     => '<span id="nn_sepa_iban_val"></span>',
        '#size'             => 45 ,
    );
    $form['sepa_bic'] = array(
        '#title'            => t('BIC or Bank code'),
        '#type'             => 'textfield',
        '#attributes'       => array('id' => 'sepa_bic', 'autocomplete' => 'off', 'name' => '', 'onchange' => 'on_change();', 'onkeypress' => 'return ibanbic_validate(event);'),
        '#field_suffix'     => '<span id="nn_sepa_bic_val" ></span>',
        '#size'             => 45 ,
    );
    $form['nn_sepa_confirm'] = array(
        '#title'            => '<b>' . t('I hereby grant the SEPA direct debit mandate and confirm that the given IBAN and BIC are correct') . '</b>',
        '#type'             => 'checkbox',
        '#attributes'       => array('id' => 'novalnet_sepa_confirm_id' , 'onchange' => 'show_overlay(this)'),
        '#prefix'           => '<div class="mandate_div">',
        '#suffix'           => '<span id ="loading_img" style=""></span>',
    );
    $form['load_img_markup']    = array(
        '#type'             => 'markup',
        '#attributes'       => array('id' => 'load_img_markup'),
        '#markup'           => '<span id="nnloader" style="display:none;"></span>',
    );
    $form['sepa_mandate'] = array('#markup' => commerce_novalnet_load_mandate_form($billing_address, $order->mail));
    $form['sepa_unique_id'] = array(
        '#type'             =>  'hidden',
        '#attributes'       => array('id' => 'sepa_unique_id'),
        '#value'            => commerce_novalnet_build_random_string(),
    );
    $form['nn_sepa_panhash'] = array(
        '#type'             =>  'hidden',
        '#attributes'       => array('id' => 'nn_sepa_panhash'),
    );
    $form['vendor_id'] = array(
        '#type'             =>  'hidden',
        '#attributes'       => array( 'id' => 'vendor_id', 'name' => ''),
        '#value'            => $data['vendor'],
    );
    $form['sepa_bic_gen'] = array(
        '#type'             =>  'hidden',
        '#attributes'       => array( 'id' => 'sepa_bic_gen', 'name' => ''),
    );
    $form['sepa_iban_gen'] = array(
        '#type'             =>  'hidden',
        '#attributes'       => array( 'id' => 'sepa_iban_gen', 'name' => ''),
    );
    $form['sepa_mandate_ref'] = array(
        '#type'             =>  'hidden',
        '#attributes'       => array( 'id' => 'sepa_mandate_ref', 'name' => ''),
    );
    $form['sepa_mandate_date'] = array(
        '#type'             =>  'hidden',
        '#attributes'       => array( 'id' => 'sepa_mandate_date', 'name' => ''),
    );
    $form['refillpanhash'] = array(
        '#type'             => 'hidden',
        '#attributes'       => array('id' => 'refillpanhash', 'class' => 'refillpanhash', 'name' => ''),
        '#value'            => commerce_novalnet_get_refill_hash($settings, 'commerce_novalnet_sepa'),
    );
    $refill = $settings['auto_refill'] ? $settings['auto_refill'] : $settings['commerce_novalnet_sepa_payment_refill'];
    $form['is_refill'] = array(
        '#type'             => 'hidden',
        '#attributes'       => array('id' => 'is_refill', 'name' => ''),
        '#value'            => $refill
    );
    $form['nndefault_holder'] = array(
        '#type'             => 'hidden',
        '#attributes'       => array('id' => 'nndefault_holder', 'name' => ''),
        '#value'            => $name,
    );
    $form['nndefault_country'] = array(
        '#type'             => 'hidden',
        '#attributes'       => array('id' => 'nndefault_country', 'name' => ''),
        '#value'            => $billing_address['country'],
    );
    $form['vendor_authcode'] = array(
         '#type'            =>  'hidden',
         '#attributes'      => array( 'id' => 'vendor_authcode', 'name' => ''),
         '#value'           => $data['auth_code'],
    );
    $form['nn_sepa_ibanbic_confirm'] = array(
        '#type'             =>  'hidden',
        '#attributes'       => array( 'id' => 'nn_sepa_ibanbic_confirm_id')
    );
    $form['invalid_error_msg'] = array(
        '#type'             => 'hidden',
        '#attributes'       => array('id' => 'invalid_error_msg', 'name' => ''),
        '#value'            => t('Your account details are invalid'),
    );
    $form['confirm_iban_bic_msg'] = array(
        '#type'             =>  'hidden',
        '#attributes'       => array('id' => 'confirm_iban_bic_msg', 'name' => ''),
        '#value'            => t('Please accept the SEPA direct debit mandate'),
    );
    $form['loading_img_path'] = array(
        '#type'             =>  'hidden',
        '#attributes'       => array('id' => 'loading_img_path', 'name' => ''),
        '#value'            => NOVALNET_IMAGES_PATH . 'novalnetloader.gif',
    );
     $form['sepa_s_markup'] = array(
        '#markup'            => "</div>",

    );
    drupal_add_js(drupal_get_path('module', 'commerce_novalnet') . '/js/sepa.js', 'file');

    return $form;
}

/**
 * set customers name
 *
 * @param $name_line string
 * @param $first_name string
 * @param $last_name string
 * @return string
 */
function commerce_novalnet_format_customer_name($name_line, $first_name, $last_name) {
    return (isset($name_line)) ? $name_line : $first_name . ' ' . $last_name;
}

/**
 * build credit card payment form
 *
 * @param $settings array
 * @param $order object
 * @return array
 */
function commerce_novalnet_build_creditcard_form($settings, $order) {

    $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
    $billing_address = $order_wrapper->commerce_customer_billing->commerce_customer_address->value();
    $name = commerce_novalnet_format_customer_name($billing_address['name_line'], $billing_address['first_name'], $billing_address['last_name']);
    drupal_add_css(drupal_get_path('module', 'commerce_novalnet') . '/css/novalnet.css');
    $data['vendor']     = $settings['vendor_id'];
    $data['auth_code']  = $settings['auth_code'];
    $data['customer_no']= $order->uid;
    commerce_novalnet_add_affiliate_details($data);

    $form['nn_cc_cardtype'] = array(
        '#title'            => t('Type of card'),
        '#type'             => 'select',
        '#attributes'       => array('id' => 'nn_cc_cardtype', 'name' => ''),
        '#options'          => commerce_novalnet_set_creditcard_types($settings['commerce_novalnet_cc_amex']),
    );
    $form['nn_cc_cardholder'] = array(
        '#title'            => t('Card holder name'),
        '#type'             => 'textfield',
        '#attributes'       => array('id' => 'nn_cc_cardholder', 'autocomplete' => 'off', 'onkeypress' => 'return accholder_validate(event,"hoder")'),
        '#size'             => 30 ,
        '#default_value'    => $name
    );
    $form['nn_cc_number'] = array(
        '#title'            => t('Card number'),
        '#type'             => 'textfield',
        '#attributes'       => array('id' => 'nn_cc_number', 'autocomplete' => 'off', 'name' => '', 'onkeypress' => 'return ccnumber_validate(event);'),
        '#size'             => 30 ,
    );
    $form['credit_card']['exp_month'] = array(
        '#type'             => 'select',
        '#title'            => t('Expiry date'),
        '#attributes'       => array('id' => 'nn_cc_expmonth', 'name' => ''),
        '#options'          => commerce_novalnet_set_creditcard_expmonth(),
        '#prefix'           => '<div class="commerce-credit-card-expiration">',
    );
    $form['credit_card']['exp_year'] = array(
        '#type'             => 'select',
        '#attributes'       => array('id' => 'nn_cc_expyear', 'name' => ''),
        '#options'          => commerce_novalnet_set_creditcard_expyear($settings['commerce_novalnet_cc_year_limit']),
        '#suffix'           => '</div>',
    );
    $form['nn_cc_cvv'] = array(
        '#title'            => t('CVC/CVV/CID'),
        '#type'             => 'textfield',
        '#attributes'       => array('id' => 'nn_cc_cvv', 'autocomplete' => 'off', 'style' => 'float:left;width:50px;', 'onkeypress' => 'return accholder_validate(event, "cvc")'),
        '#size'             => 30 ,
        '#field_suffix'     => '<div id="cvc-hint" onmouseover="show_cchint();" onmouseout="hide_cchint();" style="width:23px;height:30px;float:left;">&nbsp;<img src="' . NOVALNET_IMAGES_PATH . 'cvc_hint.png" alt="CVC HINT"><div id="cvc-hint-img" style="display:none"><img src="' . NOVALNET_IMAGES_PATH . 'creditcard_cvc.png" alt="CVC HINT"></div></div>'
    );
    $form['load_img_markup'] = array(
        '#type'             => 'markup',
        '#attributes'       => array('id' => 'load_img_markup'),
        '#markup'           => '<span id="nnloader" style="display:none;"></span>',
    );
    $form['vendor_id']    = array(
        '#type'             => 'hidden',
        '#attributes'       => array('id' => 'nn_vendor_id', 'name' => ''),
        '#value'            => $data['vendor'],
    );
    $form['vendor_authcode'] = array(
        '#type'             => 'hidden',
        '#attributes'       => array('id' => 'nn_authcode', 'name' => ''),
        '#value'            => $data['auth_code'],
    );
    $form['refillhash'] = array(
        '#type'             => 'hidden',
        '#attributes'       => array('id' => 'refillhash', 'name' => ''),
        '#value'            => commerce_novalnet_get_refill_hash($settings, 'commerce_novalnet_cc')
    );
    $form['is_refill'] = array(
        '#type'             => 'hidden',
        '#attributes'       => array('id' => 'is_refill', 'name' => ''),
        '#value'            => $settings['auto_refill']
    );
    $form['nndefault_holder'] = array(
        '#type'             => 'hidden',
        '#attributes'       => array('id' => 'nndefault_holder', 'name' => ''),
        '#value'            => $name,
    );
    $form['nn_cc_unique_id'] = array(
        '#type'             =>  'hidden',
        '#attributes'       => array('id' => 'nn_cc_unique_id'),
        '#value'            => commerce_novalnet_build_random_string(),
    );
    $form['nn_cc_panhash'] = array(
        '#type'             =>  'hidden',
        '#attributes'       => array('id' => 'nn_cc_panhash'),
    );
    $form['invalid_error_msg'] = array(
        '#type'             =>  'hidden',
        '#attributes'       => array('id' => 'invalid_error_msg', 'name' => ''),
        '#value'            => t('Your credit card details are invalid'),
    );
    $form['loading_img_path'] = array(
        '#type'             =>  'hidden',
        '#attributes'       => array('id' => 'loading_img_path', 'name' => ''),
        '#value'            => NOVALNET_IMAGES_PATH . 'novalnetloader.gif',
    );
    drupal_add_js(drupal_get_path('module', 'commerce_novalnet') . '/js/cc.js', 'file');

    return $form;
}

/**
 * set credit card types
 *
 * @param $amex boolean
 * @return array
 */
function commerce_novalnet_set_creditcard_types($amex) {
    $types = array( '' => t('Select'), 'VI' => 'Visa', 'MC' => t('MasterCard'));
    if ($amex) $types['AE'] ='AMEX';

    return $types;
}

/**
 * set credit card expiry years
 *
 * @param $limit integer
 * @return array
 */
function commerce_novalnet_set_creditcard_expyear($limit) {
    $valid_years = (is_numeric($limit) && (int)$limit != 0) ? $limit:25;
    $years['0'] = t('Year');
    $current_year = date('Y');
    for ($i = 0 ;$i < $valid_years; $i++) {
        $years[$current_year + $i] = $current_year + $i;
    }

  return $years;
}

/**
 * set credit card expiry months
 *
 * @return array
 */
function commerce_novalnet_set_creditcard_expmonth() {
    $months['0'] = t('Month');
    $months = array_merge($months, drupal_map_assoc(array_keys(commerce_months())));

  return $months;
}
/**
 * build random string
 *
 * @param none
 * @return string
 */
function commerce_novalnet_build_random_string() {
    $randomwordarray=explode(",", "a,b,c,d,e,f,g,h,i,j,k,l,m,1,2,3,4,5,6,7,8,9,0");
    shuffle($randomwordarray);
    return substr(implode($randomwordarray, ""), 0, 30);
}

/**
 * set refill hash
 *
 * @param $settings array
 * @param $payment string
 * @return string
 */
function commerce_novalnet_get_refill_hash($settings, $payment) {
    global $user;
    if ($settings['auto_refill'] && isset($_SESSION['novalnet'][$payment]['hash'])) {
        return $_SESSION['novalnet'][$payment]['hash'];
    }
    if ($payment == 'commerce_novalnet_sepa' && $settings['commerce_novalnet_sepa_payment_refill']) {
        $result = db_select('commerce_novalnet_transaction_detail', 'trxn')
                                ->fields('trxn', array('process_key', 'payment_type'))
                                ->condition('trxn.customer_id', $user->uid)
                                ->orderBy('id', 'DESC')
                                ->range(0, 1)
                                ->execute();
        $hash = $result->fetchAssoc();
        if ($hash['payment_type'] == 'commerce_novalnet_sepa' && !empty($hash['process_key'])) {
            return $hash['process_key'];
        }
    }

    return '';
}

/**
 * set fraud module form
 *
 * @param $mail string
 * @param $payment string
 * @return array
 */
function commerce_novalnet_build_fraudmodule_form($payment, $mail) {
    $form = array();
    if ($_SESSION['fraud_module'][$payment] == NOVALNET_FRAUD_MODULES_EMAIL) {
        $form[$payment . '_fraud_mail'] = array(
            '#title'            => t('E-mail address'),
            '#type'             => 'textfield',
            '#size'             => 30,
            '#prefix'   => "<div><br>",
            '#suffix'   => "</div>",
            '#default_value'    => $mail,
        );
    }
    elseif ($_SESSION['fraud_module'][$payment] == NOVALNET_FRAUD_MODULES_CALLBACK) {
        $form[$payment . '_fraud_tel'] = array(
            '#title'    => t('Telephone number'),
            '#type'     => 'textfield',
            '#size'     => 30,
            '#attributes'   => array('autocomplete' => 'off'),
            '#prefix'   => "<div><br>",
            '#suffix'   => "</div>",
        );
    }
    else {
        $form[$payment . '_fraud_mobile'] = array(
            '#title'    => t('Mobile number'),
            '#type'     => 'textfield',
            '#size'     => 30,
            '#attributes'   => array('autocomplete' => 'off'),
            '#prefix'   => "<div><br>",
            '#suffix'   => "</div>",
        );
    }

    return $form;
}

/**
 * set fraud module pin input form
 *
 * @param $payment string
 * @return array
 */
function commerce_novalnet_build_fraudmodule_pinform($payment) {
    $form = array();
    if (in_array($_SESSION['fraud_module'][$payment], array(NOVALNET_FRAUD_MODULES_SMS, NOVALNET_FRAUD_MODULES_CALLBACK))) {
        $form[$payment . '_pin_number'] = array(
            '#title'    => t('Transaction PIN'),
            '#type'     => 'textfield',
            '#size'     => 20,
            '#attributes'   => array('id' => 'novalnet_trxn_pin', 'autocomplete' => 'off'),
        );
        $form[$payment . '_new_pin'] = array(
            '#title'        => t('Forgot your PIN?'),
            '#type'         => 'checkbox',
            '#attributes'   => array('id' => 'novalnet_new_pin'),
        );
        drupal_add_js(drupal_get_path('module', 'commerce_novalnet') . '/js/disable_check_box.js', 'file');
    }

    return $form;
}

/**
 * set mandate template
 *
 * @param $billing_address array
 * @param $email string
 * @return string
 */
function commerce_novalnet_load_mandate_form($billing_address, $email) {

    $premise            = ((trim($billing_address['premise']) != '') ? ',' : '');
    $content = array(
        'confirm_title'                     => t('Direct debit SEPA mandate confirmation'),
        'payee'                             => t('Creditor'),
        'creditor_identification_number'    => t('Creditor identifier'),
        'mandate_reference'                 => t('Mandate reference'),
        'mandate_confirm_paragraph'         => t('By granting this mandate form, I authorize (A) the creditor to send instructions to my bank to debit my account and (B) my bank to debit my account in accordance with the instructions from the creditor for this and future payments.') . '<br><br>' . t('As part of my rights, I am entitled to a refund from my bank under the terms and conditions of my agreement with my bank. A refund must be claimed within eight weeks from the date on which my account was debited.'),
        'enduser_fullname'                  => t('Name of the payee'),
        'address'                           => t('Street name and number'),
        'zipcode_and_city'                  => t('Postal code and City'),
        'country'                           => t('Country'),
        'email'                             => t('E-Mail'),
        'iban'                              => t('IBAN'),
        'bic'                               => t('BIC'),
        'confirm_btn'                       => t('Confirm'),
        'cancel_btn'                        => t('Cancel'),
        'zipcode_and_city_value'            => trim($billing_address['postal_code']) . ' ' . trim($billing_address['locality']),
        'address_value'                     => trim($billing_address['premise']) . $premise . trim($billing_address['thoroughfare']),
        'city_value'                        => trim($billing_address['locality']),
        'email_value'                       => $email,
        'form_style'                        => NOVALNET_CSS_PATH . 'sepa.css',
        'plugin_img_dir'                    => NOVALNET_IMAGES_PATH,
    );
    return theme('commerce_novalnet', $content);
}
/**
 * set the reasons for subscription cancel
 *
 * @param none
 * @return array
 */
function commerce_novalnet_get_subscription_cancel_reasons() {
    return array(
        ''  => t('Select'),
        '1' => t('Product is costly'),
        '2' => t('Cheating'),
        '3' => t('Partner interfered'),
        '4' => t('Financial problem'),
        '5' => t('Content does not match my likes'),
        '6' => t('Content is not enough'),
        '7' => t('Interested only for a trial'),
        '8' => t('Page is very slow'),
        '9' => t('Satisfied customer'),
        '10' => t('Logging in problems'),
        '11' => t('Other'),
    );
}

