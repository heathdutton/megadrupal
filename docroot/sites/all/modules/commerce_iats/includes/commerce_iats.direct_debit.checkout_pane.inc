<?php
/**
 * @file
 * Checkout pane callback functions for direct debit payments.
 */

/**
 * Checkout pane callback: Direct debit declaration page settings form.
 */
function commerce_iats_direct_debit_declaration_pane_settings_form($checkout_pane) {
  $form = array();

  $form['commerce_iats_direct_debit_declaration_pane_paper_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Paper Direct Debit Instruction URL'),
    '#description' => t('The URL a user will be given to print a paper Direct Debit Instruction, if needed.'),
    '#size' => 64,
    '#default_value' => variable_get('commerce_iats_direct_debit_declaration_pane_paper_url', ''),
  );

  return $form;
}

/**
 * Checkout pane callback: Displays the direct debit declaration page.
 */
function commerce_iats_direct_debit_declaration_pane_checkout_form($form, &$form_state, $checkout_pane, $order) {
  // Set up payment method immediately so we have access to its settings.
  // TODO: Account for custom payment method rule name.
  $payment_method_name = 'direct_debit_validation|commerce_payment_direct_debit_validation';

  $order->data['payment_method'] = $payment_method_name;
  $order->data['payment_method_instance'] = commerce_payment_method_instance_load($payment_method_name);

  if (!commerce_iats_validate_payment_method($order->data['payment_method_instance'])) {
    drupal_set_message(t('The Direct Debit set-up process encountered an error. Please contact the site administrator.'), 'error');

    // Stop checkout process.
    $form_state['checkout_page']['buttons'] = 0;
    $form_state['checkout_page']['next_page'] = NULL;
    return NULL;
  }

  if (!commerce_iats_direct_debit_verify_order_data($order, array(
    'recurring' => 'Recurring Payment',
  ))) {
    return NULL;
  }

  $settings = $order->data['payment_method_instance']['settings'];
  $pane_form = array();

  $paper_url = variable_get('commerce_iats_direct_debit_declaration_pane_paper_url', '');

  $info = t('Note: If you are not the account holder or your account requires
   more than one signature, a paper Direct Debit Instruction will be required
   to be completed and posted to us. !paper_url to print off a paper
   Direct Debit Instruction',
    array(
      '!paper_url' => l(t('Click here'), $paper_url)
    )
  );

  $terms = t('Note: All the normal Direct Debit safeguards and guarantees apply.
   No changes in the amount, date, frequency to be debited can be made without
   notifying you at least five (5) working days in advance of your account
   being debited. In the event of any error, you are entitled to an immediate
   refund from your Bank or Building Society. You have the right to cancel a
   Direct Debit Instruction at any time simply by writing to your Bank or
   Building Society, with a copy to us.');

  $pane_form['info'] = array(
    '#type' => 'item',
    '#markup' => '<p>' . $info . '</p><p>' . t('OR') . '</p><p>' . t('Continue with the details below:') . '</p>',
  );

  $pane_form['declaration'] = array(
    '#type' => 'fieldset',
    '#title' => t('Declaration'),
  );

  $pane_form['declaration']['confirmation'] = array(
    '#type' => 'checkbox',
    '#title' => t('I wish to start a Direct Debit'),
    '#required' => TRUE,
  );

  $pane_form['declaration']['required'] = array(
    '#type' => 'markup',
    '#markup' => '<span class="form-required">*</span> ' . t('required'),
  );

  $pane_form['terms'] = array(
    '#type' => 'fieldset',
  );

  $pane_form['terms']['text'] = array(
    '#type' => 'item',
    '#markup' => $terms,
  );

  $pane_form['terms']['phone'] = array(
    '#type' => 'item',
    '#markup' => t('Call us: @phone', array(
      '@phone' => $settings['direct_debit']['company_phone'],
    )),
  );

  $pane_form['terms']['email'] = array(
    '#type' => 'item',
    '#markup' => t('Email us: @email', array(
      '@email' => $settings['direct_debit']['company_email'],
    )),
  );

  return $pane_form;
}

/**
 * Checkout pane callback: Saves the direct debit declaration status.
 */
function commerce_iats_direct_debit_declaration_pane_checkout_form_submit($form, &$form_state, $checkout_pane, &$order) {
  $order->data['declaration'] = isset($form_state['values']['checkout_direct_debit_declaration']) ?
    $form_state['values']['checkout_direct_debit_declaration'] : array();
}

/**
 * Checkout pane callback: Direct debit account / payer settings form.
 */
function commerce_iats_direct_debit_account_payer_pane_settings_form($checkout_pane) {
  $form = array();

  $form['commerce_iats_direct_debit_account_payer_first_collection_option_count'] = array(
    '#type' => 'textfield',
    '#title' => t('First Collection Option Count'),
    '#description' => t('The number of first collection date options to offer the user.'),
    '#size' => 4,
    '#default_value' => variable_get('commerce_iats_direct_debit_account_payer_first_collection_option_count', '3'),
  );

  return $form;
}

/**
 * Checkout pane callback: Displays the direct debit account / payer page.
 */
function commerce_iats_direct_debit_account_payer_pane_checkout_form($form, &$form_state, $checkout_pane, $order) {
  $pane_form = array();
  $default_values = isset($order->data['account_payer']) ?
    $order->data['account_payer'] : array();

  $currency_code = $order->commerce_order_total[LANGUAGE_NONE][0]['currency_code'];

  $pane_form['account'] = array(
    '#type' => 'fieldset',
    '#title' => t('Account Details'),
  );

  $pane_form['account']['account_holder'] = array(
    '#type' => 'textfield',
    '#title' => t('Account Holder Name'),
    '#default_value' => '',
    '#attributes' => array('autocomplete' => 'off'),
    '#required' => TRUE,
    '#maxlength' => 64,
    '#size' => 32,
  );

  $pane_form['account']['number'] = array(
    '#type' => 'textfield',
    '#title' => t('Account Number'),
    '#attributes' => array('autocomplete' => 'off'),
    '#required' => TRUE,
    '#maxlength' => 64,
    '#size' => 32,
  );

  $pane_form['account']['sort_code'] = array(
    '#type' => 'textfield',
    '#title' => t('Sort Code'),
    '#attributes' => array('autocomplete' => 'off'),
    '#required' => TRUE,
    '#maxlength' => 64,
    '#size' => 32,
  );

  $pane_form['account']['frequency_type'] = array(
    '#type' => 'item',
    '#title' => t('Frequency Type'),
    '#markup' => $order->data['recurring']['schedule_type'],
  );

  $pane_form['account']['amount'] = array(
    '#type' => 'item',
    '#title' => t('Amount'),
    '#markup' => $order->data['recurring']['amount'] . ' ' . $currency_code,
  );

  $pane_form['account']['first_collection_date'] = array(
    '#type' => 'select',
    '#title' => t('Date Of First Collection'),
    '#options' => commerce_iats_direct_debit_get_first_collection_options(
      $order->data['recurring']['schedule_type'],
      $order->data['recurring']['schedule_date']),
  );

  $pane_form['payer'] = array(
    '#type' => 'fieldset',
    '#title' => t('Payer Details'),
  );

  $pane_form['payer']['company_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Company Name'),
    '#maxlength' => 64,
    '#size' => 32,
    '#default_value' => isset($default_values['payer']['company_name']) ?
    $default_values['payer']['company_name'] : '',
  );

  $pane_form['payer']['first_name'] = array(
    '#type' => 'textfield',
    '#title' => t('First Name(s)'),
    '#required' => TRUE,
    '#maxlength' => 64,
    '#size' => 32,
    '#default_value' => isset($default_values['payer']['first_name']) ?
    $default_values['payer']['first_name'] : '',
  );

  $pane_form['payer']['last_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Last Name'),
    '#required' => TRUE,
    '#maxlength' => 64,
    '#size' => 32,
    '#default_value' => isset($default_values['payer']['last_name']) ?
    $default_values['payer']['last_name'] : '',
  );

  $pane_form['payer']['address_1'] = array(
    '#type' => 'textfield',
    '#title' => t('Address 1'),
    '#required' => TRUE,
    '#maxlength' => 64,
    '#size' => 32,
    '#default_value' => isset($default_values['payer']['address_1']) ?
    $default_values['payer']['address_1'] : '',
  );

  $pane_form['payer']['address_2'] = array(
    '#type' => 'textfield',
    '#title' => t('Address 2'),
    '#maxlength' => 64,
    '#size' => 32,
    '#default_value' => isset($default_values['payer']['address_2']) ?
    $default_values['payer']['address_2'] : '',
  );

  $pane_form['payer']['address_3'] = array(
    '#type' => 'textfield',
    '#title' => t('Address 3'),
    '#maxlength' => 64,
    '#size' => 32,
    '#default_value' => isset($default_values['payer']['address_3']) ?
    $default_values['payer']['address_3'] : '',
  );

  $pane_form['payer']['town'] = array(
    '#type' => 'textfield',
    '#title' => t('Town'),
    '#required' => TRUE,
    '#maxlength' => 64,
    '#size' => 32,
    '#default_value' => isset($default_values['payer']['town']) ?
    $default_values['payer']['town'] : '',
  );

  $pane_form['payer']['county'] = array(
    '#type' => 'textfield',
    '#title' => t('County'),
    '#maxlength' => 64,
    '#size' => 32,
    '#default_value' => isset($default_values['payer']['county']) ?
    $default_values['payer']['county'] : '',
  );

  $pane_form['payer']['postcode'] = array(
    '#type' => 'textfield',
    '#title' => t('Postcode'),
    '#required' => TRUE,
    '#maxlength' => 64,
    '#size' => 32,
    '#default_value' => isset($default_values['payer']['postcode']) ?
    $default_values['payer']['postcode'] : '',
  );

  $pane_form['payer']['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email Address'),
    '#required' => TRUE,
    '#maxlength' => 64,
    '#size' => 32,
    '#default_value' => isset($default_values['payer']['email']) ?
    $default_values['payer']['email'] : '',
  );

  $pane_form['payer']['confirm_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Retype Email Address'),
    '#required' => TRUE,
    '#maxlength' => 64,
    '#size' => 32,
    '#default_value' => '',
    '#attributes' => array('autocomplete' => 'off'),
  );

  $pane_form['contact'] = array(
    '#type' => 'fieldset',
  );

  $pane_form['contact']['phone'] = array(
    '#type' => 'item',
    '#markup' => t('Call us: @phone', array(
      '@phone' => $order->data['payment_method_instance']['settings']['direct_debit']['company_phone'],
    )),
  );

  $pane_form['contact']['email'] = array(
    '#type' => 'item',
    '#markup' => t('Email us: @email', array(
      '@email' => $order->data['payment_method_instance']['settings']['direct_debit']['company_email'],
    )),
  );

  return $pane_form;
}

/**
 * Checkout pane callback: Validates the account / payer details.
 */
function commerce_iats_direct_debit_account_payer_pane_checkout_form_validate($form, &$form_state, $checkout_pane, &$order) {
  if ($form_state['values']['checkout_direct_debit_account_payer']['payer']['email'] !=
    $form_state['values']['checkout_direct_debit_account_payer']['payer']['confirm_email']) {
    return FALSE;
  }

  return TRUE;
}

/**
 * Checkout pane callback: Saves the account / payer details.
 */
function commerce_iats_direct_debit_account_payer_pane_checkout_form_submit($form, &$form_state, $checkout_pane, &$order) {
  $order->data['account_payer'] = (isset($form_state['values']['checkout_direct_debit_account_payer'])) ?
    $form_state['values']['checkout_direct_debit_account_payer'] : array();

  // Delegate submit to the payment method callback.
  $payment_method = $order->data['payment_method_instance'];

  if ($callback = commerce_payment_method_callback($payment_method, 'submit_form_submit')) {
    // Cast callback to string to fix IDE warning caused by mixed return type.
    $callback = (string) $callback;
    $payment_data = $form_state['values']['checkout_direct_debit_account_payer'];
    // No balance required for direct debit validation.
    $balance = 0;
    $response = $callback($payment_method, $form, $payment_data, $order, $balance);
    if ($response === FALSE) {
      // If payment fails, rebuild the checkout form without progressing.
      $form_state['rebuild'] = TRUE;
    }
    else {
      $result = $response['AUTHRESULT'];

      $order->data['validation'] = array(
        'reference_number' => $result['ACHREFNUM'],
        'bank_name' => $result['BANK_NAME'],
        'bank_branch' => $result['BANK_BRANCH'],
        'bank_address_1' => $result['BANKADDRESS1'],
        'bank_address_2' => $result['BANKADDRESS2'],
        'bank_address_3' => $result['BANKADDRESS3'],
        'bank_address_4' => $result['BANKADDRESS4'],
        'bank_city' => $result['BANK_CITY'],
        'bank_state' => $result['BANK_STATE'],
        'bank_postcode' => $result['BANK_POSTCODE'],
      );
    }
  }
}

/**
 * Checkout pane callback: Displays the direct debit validation details page.
 */
function commerce_iats_direct_debit_validation_pane_checkout_form($form, &$form_state, $checkout_pane, $order) {
  if (!commerce_iats_direct_debit_verify_order_data($order, array(
    'account_payer' => 'Account Payer',
  ))) {
    return NULL;
  }

  $settings = $order->data['payment_method_instance']['settings'];
  $pane_form = array();

  $pane_form['confirmation'] = array(
    '#type' => 'fieldset',
    '#title' => t('Confirmation'),
  );

  $pane_form['confirmation']['text'] = array(
    '#type' => 'item',
    '#markup' => '<p>' . t('These are the details that you have entered to
      set up the Direct Debit Instruction:') . '<br />'
      . t('Important: In order to complete the Direct Debit set-up, please
      check the following details and then click the \'Confirm\' button lower
      down this page.') . '</p>',
  );

  $pane_form['review'] = array(
    '#type' => 'fieldset',
  );

  $pane_form['review']['dd_logo'] = array(
    '#type' => 'markup',
    '#markup' => '<div class="direct_debit_logo_header"><img src="/' . drupal_get_path('module', 'commerce_iats') . '/images/logos/dd_logo_landscape.png"></div>',
  );

  $pane_form['review']['account'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array(
        'direct_debit_pane_left'
      ),
    ),
  );

  $pane_form['review']['account']['company'] = array(
    '#type' => 'container',
  );

  $pane_form['review']['account']['company']['title'] = array(
    '#type' => 'item',
    '#markup' => '<span class="direct_debit_header">' . strtoupper($settings['direct_debit']['company_name']) . '</span>',
  );

  $combined_address = strtoupper($settings['direct_debit']['company_address']) . '</br >'
    . strtoupper($settings['direct_debit']['company_city']) . '<br />'
    . strtoupper($settings['direct_debit']['company_state']) . '<br />'
    . strtoupper($settings['direct_debit']['company_country']) . '<br />'
    . strtoupper($settings['direct_debit']['company_postcode']);

  $pane_form['review']['account']['company']['address'] = array(
    '#type' => 'item',
    '#markup' => $combined_address,
  );

  $pane_form['review']['account']['account_holder'] = array(
    '#type' => 'item',
    '#title' => t('Name of Account Holder'),
    '#markup' => '<span class="direct_debit_review_field">' . $order->data['account_payer']['account']['account_holder'] . '</span>',
  );

  $pane_form['review']['account']['account_number'] = array(
    '#type' => 'item',
    '#title' => t('Bank / Building Society Account Number'),
    '#markup' => '<span class="direct_debit_review_field">' . $order->data['account_payer']['account']['number'] . '</span>',
  );

  $pane_form['review']['account']['sort_code'] = array(
    '#type' => 'item',
    '#title' => t('Branch Sort Code'),
    '#markup' => '<span class="direct_debit_review_field">' . $order->data['account_payer']['account']['sort_code'] . '</span>',
  );

  $combined_address = '';
  for ($i = 1; $i <= 4; $i++) {
    $address_line = $order->data['validation']['bank_address_' . $i];
    if (is_string($address_line) && !empty($address_line)) {
      $combined_address .= $address_line . '<br />';
    }
  }

  $combined_address .= $order->data['validation']['bank_city'] . '<br />'
    . $order->data['validation']['bank_state'] . '<br />'
    . $order->data['validation']['bank_postcode'];

  $name_address = $order->data['validation']['bank_name'] . '<br />'
    . $order->data['validation']['bank_branch'] . '<br />'
    . $combined_address;

  $pane_form['review']['account']['name_address'] = array(
    '#type' => 'item',
    '#title' => t('Name and full postal address of your Bank or Building Society'),
    '#markup' => '<span class="direct_debit_review_field">' . $name_address . '</span>',
  );

  $pane_form['review']['instruction'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array(
        'direct_debit_pane_right'
      ),
    ),
  );

  $pane_form['review']['instruction']['title'] = array(
    '#type' => 'item',
    '#markup' => '<span class="direct_debit_header">Instruction to your Bank or Building Society to pay by Direct Debit</span>',
  );

  $pane_form['review']['instruction']['service_user_number'] = array(
    '#type' => 'item',
    '#title' => t('Service User Number'),
    '#markup' => '<span class="direct_debit_review_field">' . $settings['direct_debit']['service_user_number'] . '</span>',
  );

  $pane_form['review']['instruction']['reference_number'] = array(
    '#type' => 'item',
    '#title' => t('Reference'),
    '#markup' => '<span class="direct_debit_review_field">' . $order->data['validation']['reference_number'] . '</span>',
  );

  $instruction_info = t('Instruction to your Bank or Building Society:
    Please pay @company_name Direct Debits from the account detailed in
    this instruction subject to the safeguards assured by the
    Direct Debit Guarantee.
    I understand that this instruction may remain with @company_name and,
    if so, details will be passed electronically to my Bank / Building Society.',
    array(
      '@company_name' => $settings['direct_debit']['company_name'],
    ));

  $pane_form['review']['instruction']['info'] = array(
    '#type' => 'item',
    '#markup' => $instruction_info,
  );

  $pane_form['review']['instruction']['value'] = array(
    '#type' => 'item',
    '#markup' => '<span class="direct_debit_review_field">Date: ' . date('d/m/Y',
      strtotime($order->data['account_payer']['account']['first_collection_date'])) . '</span>',
  );

  $pane_form['review']['notice'] = array(
    '#type' => 'item',
    '#markup' => '<div style="clear:both">Banks and Building Societies may not accept Direct Debit Instructions for some types of account.</div>',
  );

  return $pane_form;
}

/**
 * Checkout pane callback: Displays the direct debit summary page.
 */
function commerce_iats_direct_debit_summary_pane_checkout_form($form, &$form_state, $checkout_pane, $order) {
  if (!commerce_iats_direct_debit_verify_order_data($order, array(
    'validation' => 'Validation',
  ))) {
    return NULL;
  }

  $settings = $order->data['payment_method_instance']['settings'];
  $pane_form = array();

  $currency_code = $order->commerce_order_total[LANGUAGE_NONE][0]['currency_code'];

  $info = t('A summary of the details of your submission are listed below.
    Confirmation of the Direct Debit set up will be sent by email within
    3 working days or no later than 10 working days before first collection.');

  $pane_form['summary'] = array(
    '#type' => 'fieldset',
    '#title' => t('Summary Details Entered'),
  );

  $pane_form['summary']['text'] = array(
    '#type' => 'item',
    '#markup' => $info,
  );

  $pane_form['service_user'] = array(
    '#type' => 'fieldset',
    '#title' => t('Service User'),
  );

  $pane_form['service_user']['name'] = array(
    '#type' => 'item',
    '#title' => t('Name'),
    '#markup' => $settings['direct_debit']['company_name'],
  );

  $combined_address = $settings['direct_debit']['company_address'] . '<br />'
    . $settings['direct_debit']['company_city'] . '<br />'
    . $settings['direct_debit']['company_state'] . '<br />'
    . $settings['direct_debit']['company_country'] . '<br />'
    . $settings['direct_debit']['company_postcode'];

  $pane_form['service_user']['address'] = array(
    '#type' => 'item',
    '#title' => t('Address'),
    '#markup' => $combined_address,
  );

  $pane_form['service_user']['reference_number'] = array(
    '#type' => 'item',
    '#title' => t('Reference Number'),
    '#markup' => $order->data['validation']['reference_number'],
  );

  $pane_form['service_user']['company_info'] = array(
    '#type' => 'item',
    '#markup' => t('The company name that will appear on your bank statement against the Direct Debit will be "@company_name"',
      array(
        '@company_name' => $settings['direct_debit']['company_name'],
      )),
  );

  $pane_form['amount_details'] = array(
    '#type' => 'fieldset',
    '#title' => t('Amount Details'),
  );

  $pane_form['amount_details']['amount'] = array(
    '#type' => 'item',
    '#title' => t('Amount'),
    '#markup' => $order->data['recurring']['amount'] . ' ' . $currency_code,
  );

  $pane_form['amount_details']['frequency'] = array(
    '#type' => 'item',
    '#title' => t('Frequency'),
    '#markup' => $order->data['recurring']['schedule_type'],
  );

  $pane_form['amount_details']['first_collection_date'] = array(
    '#type' => 'item',
    '#title' => t('Date of first collection'),
    '#markup' => date('d-m-Y',
      strtotime($order->data['account_payer']['account']['first_collection_date'])),
  );

  $pane_form['payer_details'] = array(
    '#type' => 'fieldset',
    '#title' => t('Payer Details'),
  );

  $pane_form['payer_details']['first_name'] = array(
    '#type' => 'item',
    '#title' => t('First Name'),
    '#markup' => $order->data['account_payer']['payer']['first_name'],
  );

  $pane_form['payer_details']['last_name'] = array(
    '#type' => 'item',
    '#title' => t('Last Name'),
    '#markup' => $order->data['account_payer']['payer']['last_name'],
  );

  $combined_address = '';
  for ($i = 1; $i <= 3; $i++) {
    $address_line = $order->data['account_payer']['payer']['address_' . $i];
    if (is_string($address_line) && !empty($address_line)) {
      $combined_address .= $address_line . '<br />';
    }
  }

  $combined_address .= $order->data['account_payer']['payer']['town'] . '<br />'
    . $order->data['account_payer']['payer']['county'] . '<br />'
    . $order->data['account_payer']['payer']['postcode'];

  $pane_form['payer_details']['address'] = array(
    '#type' => 'item',
    '#title' => t('Address'),
    '#markup' => $combined_address,
  );

  $pane_form['payer_details']['email_address'] = array(
    '#type' => 'item',
    '#title' => t('Email Address'),
    '#markup' => $order->data['account_payer']['payer']['email'],
  );

  $pane_form['account_details'] = array(
    '#type' => 'fieldset',
    '#title' => t('Account Details'),
  );

  $pane_form['account_details']['name'] = array(
    '#type' => 'item',
    '#title' => t('Account Name'),
    '#markup' => $order->data['account_payer']['account']['account_holder'],
  );

  $pane_form['account_details']['number'] = array(
    '#type' => 'item',
    '#title' => t('Account Number'),
    '#markup' => $order->data['account_payer']['account']['number'],
  );

  $pane_form['account_details']['sort_code'] = array(
    '#type' => 'item',
    '#title' => t('Account Sort Code'),
    '#markup' => $order->data['account_payer']['account']['sort_code'],
  );

  return $pane_form;
}

/**
 * Checkout pane callback: Saves the account / payer details.
 */
function commerce_iats_direct_debit_summary_pane_checkout_form_submit($form, &$form_state, $checkout_pane, &$order) {
  // TODO: Account for custom payment method rule name.
  $payment_method_name = 'direct_debit_customer_code|commerce_payment_direct_debit_customer_code';

  $order->data['payment_method'] = $payment_method_name;
  $payment_method = commerce_payment_method_instance_load($payment_method_name);

  if (!commerce_iats_validate_payment_method($payment_method)) {
    drupal_set_message('The Direct Debit set-up process encountered an error.
          Please contact the site administrator.', 'error');

    // Stop checkout process.
    $form_state['checkout_page']['buttons'] = 0;
    $form_state['checkout_page']['next_page'] = NULL;
    return;
  }

  if ($callback = commerce_payment_method_callback($payment_method, 'submit_form_submit')) {
    // Cast callback to string to fix IDE warning caused by mixed return type.
    $callback = (string) $callback;
    $payment_data = $order->data;
    // No balance required for direct debit validation.
    $balance = 0;
    $response = $callback($payment_method, $form, $payment_data, $order, $balance);
    if ($response === FALSE) {
      // If payment fails, rebuild the checkout form without progressing.
      $form_state['rebuild'] = TRUE;
    }
  }
}

/**
 * Checkout pane callback: Displays the direct debit set up complete page.
 */
function commerce_iats_direct_debit_complete_pane_checkout_form($form, &$form_state, $checkout_pane, $order) {
  $settings = $order->data['payment_method_instance']['settings'];

  $pane_form = array();

  $pane_form['complete'] = array(
    '#type' => 'fieldset',
    '#title' => t('Completed'),
  );

  $pane_form['complete']['reference_number'] = array(
    '#type' => 'item',
    '#markup' => t('Your Direct Debit Reference Number is @reference_number',
      array(
        '@reference_number' => $order->data['validation']['reference_number'],
      )),
  );

  $pane_form['complete']['info'] = array(
    '#type' => 'item',
    '#markup' => '<p>' . t('Should you have any queries regarding your Direct
    Debit, please do not hesitate to contact us on @phone', array(
        '@phone' => $settings['direct_debit']['company_phone'],
      )) . '</p>'
      . '<p>That completes the Direct Debit Instruction, thank you. An email
    confirming the details will be sent within 3 working days or not later
    than 10 working days before the first collection. Please find a copy of
    the Direct Debit Guarantee below.</p>',
  );

  $pane_form['guarantee'] = array(
    '#type' => 'fieldset',
  );

  $pane_form['guarantee']['dd_logo'] = array(
    '#type' => 'markup',
    '#markup' => '<img style="float:right" src="/' . drupal_get_path('module', 'commerce_iats') . '/images/logos/dd_logo_landscape.png">',
  );

  $pane_form['guarantee']['title'] = array(
    '#type' => 'item',
    '#markup' => '<span class="direct_debit_header">The Direct Debit Guarantee</span>',
  );

  $pane_form['guarantee']['text'] = array(
    '#type' => 'item',
    '#markup' => '<ul>'
      . '<li>' . t('This Guarantee is offered by all banks and building societies
    that accept instructions to pay Direct Debits.') . '</li>'
      . '<li>' . t('If there are any changes to the amount, date or frequency of
    your Direct Debit @company_name will notify you five (5) working days in
    advance of your account being debited or as otherwise agreed. If you request
    @company_name to collect a payment, confirmation of the amount and date will
    be given to you at the time of the request.',
        array(
          '@company_name' => $settings['direct_debit']['company_name'],
        )) . '</li>'
      . '<li>' . t('If an error is made in the payment of your Direct Debit, by
    @company_name or your bank or building society you are entitled to a full
    and immediate refund of the amount paid from your bank or building society.',
        array(
          '@company_name' => $settings['direct_debit']['company_name'],
        )) . '</li>'
      . '<li>' . t('If you receive a refund you are not entitled to, you must pay
    it back when @company_name asks you to.',
        array(
          '@company_name' => $settings['direct_debit']['company_name'],
        )) . '</li>'
      . '<li>' . t('You can cancel a Direct Debit at any time by simply contacting
    your bank or building society. Written confirmation may be required.
    Please also notify us.') . '</li>'
      . '</ul>',
  );

  return $pane_form;
}

function commerce_iats_direct_debit_checkout_pane_footer_fields() {
  $path = drupal_get_path('module', 'commerce_iats');

  $form = array();

  $form['footer_logos'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array(
        'direct_debit_logos',
      ),
    ),
    '#weight' => 10,
  );

  $form['footer_logos']['bacs_logo'] = array(
    '#type' => 'markup',
    '#markup' => '<img src="/' . $path . '/images/logos/bacs_approved_logo.png">',
  );

  $form['footer_logos']['dd_logo'] = array(
    '#type' => 'markup',
    '#markup' => '<img src="/' . $path . '/images/logos/dd_logo_landscape.png">',
  );

  $form['footer_logos']['iats_payments_logo'] = array(
    '#type' => 'markup',
    '#markup' => '<img src="/' . $path . '/images/logos/iats_payments_logo.png">',
  );

  return $form;
}

/**
 * Gets an array of first collection date options, indexed by date string.
 *
 * @param string $schedule_type
 *   The iATS Schedule Type.
 * @param string $schedule_date
 *   The iATS Schedule Date.
 *
 * @see commerce_iats_recurring_pane_schedule_types
 * @see commerce_iats_recurring_pane_schedule_dates
 *
 * @return array
 *   Array of first collection date options.
 */
function commerce_iats_direct_debit_get_first_collection_options($schedule_type, $schedule_date) {
  $option_count = (int) variable_get('commerce_iats_direct_debit_account_payer_first_collection_option_count', 3);
  $schedule_date = (int) $schedule_date;

  $dates = array();
  switch ($schedule_type) {
    case 'Weekly':
      // Start date options on next chosen day of week after lead time,
      // repeat weekly.
      $current_week_day = (int) date('N', mktime(0, 0, 0, date('n'), date('j'), date('y')));

      $day_offset = ($current_week_day != $schedule_date) ?
        ($schedule_date - $current_week_day) : 0;
      // Find the next weekday that matches the chosen day of week.
      $initial_time = mktime(0, 0, 0, date('n'), date('j') + $day_offset, date('y'));

      for ($i = 0; $i < $option_count; $i++) {
        $time = mktime(0, 0, 0, date('n', $initial_time),
          date('j', $initial_time) + ($i * 7), date('y', $initial_time));
        if (!commerce_iats_direct_debit_validate_lead_time($time)) {
          $option_count++;
          continue;
        }
        $dates[commerce_iats_get_formatted_date($time)] = commerce_iats_direct_debit_get_formatted_date($time);
      }
      break;

    case 'Monthly':
      // Start date options on next chosen day of month after lead time,
      // repeat monthly.
      for ($i = 0; $i < $option_count; $i++) {
        $time = mktime(0, 0, 0, date('n') + $i, $schedule_date, date('y'));
        if (!commerce_iats_direct_debit_validate_lead_time($time)) {
          $option_count++;
          continue;
        }
        $dates[commerce_iats_get_formatted_date($time)] = commerce_iats_direct_debit_get_formatted_date($time);
      }
      break;

    case 'Quarterly':
      // Start date options on next quarter after lead time, repeat quarterly.
      for ($i = 0; $i < $option_count; $i++) {
        $time = mktime(0, 0, 0, date('n') + ($i * 4),
          date('j') + COMMERCE_IATS_DIRECT_DEBIT_LEAD_DAYS, date('y'));
        if (!commerce_iats_direct_debit_validate_lead_time($time)) {
          $option_count++;
          continue;
        }
        $dates[commerce_iats_get_formatted_date($time)] = commerce_iats_direct_debit_get_formatted_date($time);
      }
      break;

    case 'Annually':
      // Start date options after lead time, repeat annually.
      for ($i = 0; $i < $option_count; $i++) {
        $time = mktime(0, 0, 0, date('n'),
          date('j') + COMMERCE_IATS_DIRECT_DEBIT_LEAD_DAYS, date('y') + $i);
        if (!commerce_iats_direct_debit_validate_lead_time($time)) {
          $option_count++;
          continue;
        }
        $dates[commerce_iats_get_formatted_date($time)] = commerce_iats_direct_debit_get_formatted_date($time);
      }
      break;
  }

  return $dates;
}

/**
 * Determines if a UNIX timestamp provides valid lead time for direct debit.
 *
 * @param int $time
 *   UNIX timestamp to test.
 *
 * @return bool
 *   TRUE if timestamp provides valid lead time.
 */
function commerce_iats_direct_debit_validate_lead_time($time) {
  $seconds_in_day = (60 * 60 * 24);
  $time_today = mktime(0, 0, 0, date('n'), date('j'), date('y'));

  return ($time >= ($time_today + ($seconds_in_day * COMMERCE_IATS_DIRECT_DEBIT_LEAD_DAYS)));
}

/**
 * Gets a formatted date string for a given UNIX timestamp.
 *
 * @param int $time
 *   The UNIX timestamp to convert to a date string.
 *
 * @return string
 *   The formatted date string.
 */
function commerce_iats_direct_debit_get_formatted_date($time) {
  return date('l, F jS, Y', $time);
}

/**
 * Verifies order data exists and displays a message when data is missing.
 *
 * Used to ensure integrity throughout the checkout process.
 *
 * @param object $order
 *   The order object.
 * @param array $expected_data
 *   Associative array of expected order data indexes mapped to readable name.
 *
 * @return bool
 *   TRUE if order data exists.
 */
function commerce_iats_direct_debit_verify_order_data($order, array $expected_data) {
  $success = TRUE;
  foreach ($expected_data as $index => $name) {
    if (!isset($order->data[$index]) || empty($order->data[$index])) {
      drupal_set_message('The Direct Debit set-up process encountered an error.
          Please contact the site administrator.', 'error');

      watchdog('commerce_iats', 'Order is missing @name data. Please ensure Direct
          Debit checkout panes are arranged as specified in README.txt',
        array(
          '@name' => $name,
        ),
        WATCHDOG_ERROR);

      $success = FALSE;
    }
  }
  return $success;
}
