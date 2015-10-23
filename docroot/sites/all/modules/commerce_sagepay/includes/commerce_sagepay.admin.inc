<?php

/**
 * @file
 * commerce_sagepay_admin.inc
 *
 * Provides a single admin page to contain all SagePay Settings.
 * Submodules such as sagepay_token and sagepay_3dsecure supplement this page.
 */

module_load_include('inc', 'commerce_sagepay',
  'includes/commerce_sagepay_utils');

/**
 * Settings form containing all configuration for the SagePay module.
 *
 * @param array $form
 *   The form array.
 * @param array $form_state
 *   The form state.
 *
 * @return mixed
 *   The completed form.
 */
function commerce_sagepay_settings_form($form, &$form_state) {

  $form['account_details'] = array(
    '#type'  => 'fieldset',
    '#title' => 'Your Account',
  );

  $form['account_details'][SAGEPAY_SETTING_VENDOR_NAME] = array(
    '#type'          => 'textfield',
    '#title'         => t('SagePay Vendor Name'),
    '#description'   => t('This is the vendor name that SagePay sent you when
     you set up your account.'),
    '#required'      => TRUE,
    '#default_value' => variable_get(SAGEPAY_SETTING_VENDOR_NAME, ''),
  );

  $form['integration_type'] = array(
    '#type' => 'fieldset',
    '#title' => t('Sage Pay Integration Options'),
  );

  $form['integration_type']['sagepay_server_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Server (Including InFrame)'),
    '#default_value' => variable_get(SAGEPAY_SETTING_SERVER_ENABLED, 1),
  );

  // Items only relevant to SagePay Server integration.
  $form['integration_type']['server_specific'] = array(
    '#type'        => 'fieldset',
    '#title'       => 'Sage Pay Server Integration Specific Settings',
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE,
  );

  $form['integration_type']['server_specific'][SAGEPAY_SETTING_USE_IFRAME] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Display payment form in an iFrame'),
    '#description'   => t('Use an iFrame to make the payment page appear to be an integrated part of your site.'),
    '#default_value' => variable_get(SAGEPAY_SETTING_USE_IFRAME, 1),
  );

  $form['integration_type']['server_specific'][SAGEPAY_SETTING_LOW_PROFILE] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Use low profile payment pages'),
    '#description'   => t('Use the less graphical, simpler set of payment pages which look better inside an iFrame.'),
    '#default_value' => variable_get(SAGEPAY_SETTING_LOW_PROFILE, 1),
  );

  $form['integration_type']['sagepay_form_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Form (Completely outsourcing your online payments to Sage
    Pay'),
    '#default_value' => variable_get(SAGEPAY_SETTING_FORM_ENABLED, 0),

  );

  // Items only relevant to SagePay Form integration.
  $form['integration_type']['form_fields']['form_specific'] = array(
    '#type'        => 'fieldset',
    '#title'       => 'Sage Pay Form Integration Specific Settings',
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE,
  );

  $form['integration_type']['form_fields']['form_specific'][SAGEPAY_SETTING_ENCRYPTION_KEY] = array(
    '#type'          => 'textfield',
    '#title'         => t('Encryption Key'),
    '#description'   => t('If you have requested form based integration, you will have received an encryption key from SagePay in a separate email.'),
    '#weight'        => 10,
    '#default_value' => variable_get(SAGEPAY_SETTING_ENCRYPTION_KEY, ''),
  );

  $form['integration_type']['form_fields']['form_specific'][SAGEPAY_SETTING_TEST_ENCRYPTION_KEY] = array(
    '#type'          => 'textfield',
    '#title'         => t('Test Mode Encryption Key'),
    '#description'   => t('If you have requested form based integration, you will have received an encryption key from SagePay in a separate email. The encryption key for the test server is different to the one used in your production environment.'),
    '#weight'        => 15,
    '#default_value' => variable_get(SAGEPAY_SETTING_TEST_ENCRYPTION_KEY, ''),
  );

  $form['integration_type']['sagepay_direct_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Direct (The full white-label payment solution)'),
    '#default_value' => variable_get(SAGEPAY_SETTING_DIRECT_ENABLED, 0),
  );

  $form['integration_type']['direct_specific'] = array(
    '#type'        => 'fieldset',
    '#title'       => 'Sage Pay Direct Integration Specific Settings',
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE,
  );

  $form['integration_type']['direct_specific']['transaction'][SAGEPAY_SETTING_ENABLED_CARDS] = array(
    '#type'          => 'checkboxes',
    '#title'         => t('Enabled card types'),
    '#description'   => t('Select the card types you want to accept. If none are selected then it is assumed that all card types are accepted.'),
    '#options'       => _commerce_sagepay_all_card_names(),
    '#default_value' => variable_get(SAGEPAY_SETTING_ENABLED_CARDS, array()),
  );


  // Settings related to the emails sent from SagePay during the course
  // of a transaction.
  $form['emails'] = array(
    '#type'        => 'fieldset',
    '#title'       => 'Emails',
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE,
  );

  $form['emails'][SAGEPAY_SETTING_VENDOR_EMAIL] = array(
    '#type'          => 'textfield',
    '#title'         => t('Vendor Email'),
    '#description'   => t('An e-mail address on which you can be contacted
    when a transaction completes.'),
    '#default_value' => variable_get(SAGEPAY_SETTING_VENDOR_EMAIL, ''),
  );

  $form['emails'][SAGEPAY_SETTING_SEND_EMAIL] = array(
    '#type'          => 'radios',
    '#title'         => t('Send Email from Sagepay'),
    '#description'   => t('Should SagePay sent receipts for transactions?'),
    '#options'       => array(
      '0' => t('No emails sent by Sagepay.'),
      '1' => t('Send email to customer and vendor if email(s) are provided. (default)'),
      '2' => t('Send email to vendor only.'),
    ),
    '#default_value' => variable_get(SAGEPAY_SETTING_SEND_EMAIL, 0),
  );

  $form['emails'][SAGEPAY_SETTING_EMAIL_MESSAGE] = array(
    '#type'          => 'textarea',
    '#title'         => t('Email Message'),
    '#description'   => t('If provided this message is included toward the top of the customer confirmation e-mails sent by SagePay.'),
    '#default_value' => variable_get(SAGEPAY_SETTING_EMAIL_MESSAGE, ''),
  );

  // Default transaction settings.
  // These can be overriden using hook_sagepay_order_data_alter.
  $form['transaction'] = array(
    '#type'        => 'fieldset',
    '#title'       => 'Transaction Settings',
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE,
  );

  $form['transaction'][SAGEPAY_SETTING_ORDER_DESCRIPTION] = array(
    '#type'          => 'textfield',
    '#title'         => t('Order Description'),
    '#description'   => t('The description of the order that will appear in the SagePay transaction. (For example, Your Order from sitename.com)'),
    '#default_value' => variable_get(SAGEPAY_SETTING_ORDER_DESCRIPTION, 'Your order from sitename.com'),
  );

  $form['transaction'][SAGEPAY_SETTING_TRANSACTION_PREFIX] = array(
    '#type'          => 'textfield',
    '#title'         => t('Transaction Code Prefix'),
    '#description'   => t('This allows you to add an optional prefix to all transaction codes.'),
    '#default_value' => variable_get(SAGEPAY_SETTING_TRANSACTION_PREFIX, ''),
  );

  $form['transaction'][SAGEPAY_SETTING_TRANSACTION_MODE] = array(
    '#type'          => 'radios',
    '#title'         => t('Transaction mode'),
    '#description'   => t('Adjust to live transactions when you are ready to start processing actual payments.'),
    '#options'       => array(
      SAGEPAY_TXN_MODE_LIVE     => t('Live transactions in a live account.'),
      SAGEPAY_TXN_MODE_TEST     => t('Test transactions in a test account.'),
      SAGEPAY_TXN_MODE_SHOWPOST => t('Show Post (for debugging with SagePay).'),
    ),
    '#default_value' => variable_get(SAGEPAY_SETTING_TRANSACTION_MODE, SAGEPAY_TXN_MODE_TEST),
  );

  $form['transaction'][SAGEPAY_SETTING_TRANSACTION_TYPE] = array(
    '#type'          => 'radios',
    '#title'         => t('Default credit card transaction type'),
    '#description'   => t('The default will be used to process transactions during checkout.'),
    '#options'       => array(
      COMMERCE_CREDIT_AUTH_CAPTURE => t('Authorisation and capture.'),
      COMMERCE_CREDIT_AUTH_ONLY    => t('Authorisation only (requires manual or automated capture after checkout).'),
    ),
    '#default_value' => variable_get(SAGEPAY_SETTING_TRANSACTION_TYPE, COMMERCE_CREDIT_AUTH_CAPTURE),
  );

  $form['transaction'][SAGEPAY_SETTING_ACCOUNT_TYPE] = array(
    '#type'          => 'radios',
    '#title'         => 'Account Type',
    '#description'   => 'This optional flag is used to tell the SAGE PAY System which merchant account to use.',
    '#options'       => array(
      'E' => t('Use the e-commerce merchant account (default).'),
      'C' => t('Use the continuous authority merchant account (if present).'),
      'M' => t('Use the mail order, telephone order account (if present).'),
    ),
    '#default_value' => variable_get(SAGEPAY_SETTING_ACCOUNT_TYPE, 'E'),
  );

  $form['security'] = array(
    '#type'        => 'fieldset',
    '#title'       => 'Security Checks',
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE,
  );

  $form['security'][SAGEPAY_SETTING_APPLY_AVS_CV2] = array(
    '#type'          => 'radios',
    '#title'         => t('AVS / CV2 Mode'),
    '#description'   => t('CV2 validation mode used by default on all transactions.'),
    '#options'       => array(
      '0' => t('If AVS/CV2 enabled then check them. If rules apply, use rules. (default)'),
      '1' => t('Force AVS/CV2 checks even if not enabled for the account. If rules apply, use rules.'),
      '2' => t('Force NO AVS/CV2 checks even if enabled on account.'),
      '3' => t('Force AVS/CV2 checks even if not enabled for the account but DO NOT apply any rules.'),
    ),
    '#default_value' => variable_get(SAGEPAY_SETTING_APPLY_AVS_CV2, 0),
  );

    $form['security'][SAGEPAY_SETTING_APPLY_3D_SECURE] = array(
      '#type'          => 'item',
      '#title'        => t('3D Secure module not enabled.'),
    );

  $form['order_settings'] = array(
    '#type'        => 'fieldset',
    '#title'       => 'Order Settings',
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE,
  );

  $form['order_settings'][SAGEPAY_SETTING_SEND_CART_CONTENTS] = array(
    '#type'          => 'select',
    '#title'         => t('Send cart contents to SagePay'),
    '#description'   => t('Send the order lines to SagePay as well as the order total.'),
    '#options'       => array(
      '0' => t('Do not send basket contents'),
      '1' => t('Send as text'),
      '2' => t('Send as XML'),
    ),
    '#default_value' => variable_get(SAGEPAY_SETTING_SEND_CART_CONTENTS, 0),
  );

  // Options for overriding card data during testing.
  $form['testing'] = array(
    '#type'        => 'fieldset',
    '#title'       => 'Testing',
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE,
  );

  $form['testing'][SAGEPAY_SETTING_USE_TEST_DATA] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Override entered card data with test data.'),
    '#description'   => t('When checked, any card details entered will be replaced by standard SagePay test card data when in test mode. Uncheck to test failure scenarios.'),
    '#default_value' => variable_get(SAGEPAY_SETTING_USE_TEST_DATA, 0),
  );

  $form['testing'][SAGEPAY_SETTING_BYPASS_COMMERCE_CARD_VALIDATION] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Bypass Drupal Commerce card validation in test mode.'),
    '#description'   => t('Drupal Commerce uses standard card number validation checks - however these fail using some of the supplied Sagepay test card numbers. Check this setting to bypass the Drupal Commerce card number checks when in test mode.'),
    '#default_value' => variable_get(SAGEPAY_SETTING_BYPASS_COMMERCE_CARD_VALIDATION, 0),
  );

  $form['testing'][SAGEPAY_SETTING_DEBUG_MODE] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Enable Debug mode.'),
    '#description'   => t('Debug mode will present certain information as debug messages during the checkout.'),
    '#default_value' => variable_get(SAGEPAY_SETTING_DEBUG_MODE, 0),
  );

  return system_settings_form($form);

}

function commerce_sagepay_settings_form_validate($form, &$form_state) {

  // Depending on the integration modes set, enable or disable the
  // default payment gateway rules.
  // Only do this when the setting is changed.

  if ($form_state['values']['sagepay_form_enabled'] != variable_get
    (SAGEPAY_SETTING_FORM_ENABLED)) {
    $rules_config = rules_config_load('commerce_payment_commerce_sagepay_form');
    $rules_config->active = $form_state['values']['sagepay_form_enabled'];
    $rules_config->save();
  }

  if ($form_state['values']['sagepay_server_enabled'] != variable_get
    (SAGEPAY_SETTING_SERVER_ENABLED)) {
    $rules_config = rules_config_load('commerce_payment_commerce_sagepay_server');
    $rules_config->active = $form_state['values']['sagepay_server_enabled'];;
    $rules_config->save();
  }

  if ($form_state['values']['sagepay_direct_enabled'] != variable_get
    (SAGEPAY_SETTING_DIRECT_ENABLED)) {
    $rules_config = rules_config_load('commerce_payment_commerce_sagepay_direct');
    $rules_config->active = $form_state['values']['sagepay_direct_enabled'];;
    $rules_config->save();
  }
}

function commerce_sagepay_validate_prefix($element) {
}
