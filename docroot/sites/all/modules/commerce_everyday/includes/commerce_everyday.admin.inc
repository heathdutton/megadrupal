<?php

/**
 * @file
 * Commerce Everyday settings items.
 */

/**
 * Payment method callback: settings form.
 */
function commerce_everyday_settings_form($settings = NULL) {
  $form = array();

  $settings = (array) $settings + array(
    'commerce_everyday_mode' => '',
    'prod_mode' => array(
      'commerce_everyday_sid' => '',
      'commerce_everyday_secret_key' => '',
      'commerce_everyday_secret_key_version' => '1',
    ),
    'test_mode' => array(
      'commerce_everyday_test_sid' => '5442',
      'commerce_everyday_test_secret_key' => 'NGNhODg0ZjA0NjYxNzllZmQxNWRhZA',
      'commerce_everyday_test_secret_key_version' => '1',
    ),
    'commerce_everyday_method_title' => t('Everyday online payment on a secure server'),
    'commerce_everyday_method_title_icons' => TRUE,
    'commerce_everyday_method_title_icon_select' => '',
    'commerce_everyday_method_offsite_autoredirect' => FALSE,
    'commerce_everyday_method_payment_msg' => t('Continue with checkout. You are directed to Everyday secure server.'),
    'commerce_everyday_checkout_button' => t('Proceed to Everyday'),
    'commerce_everyday_reference_prefix' => '1000',
  );

  $form['commerce_everyday_mode'] = array(
    '#type' => 'select',
    '#title' => t('Mode'),
    '#description' => t('In testing mode you are able to verify Everyday service with fake orders'),
    '#options' => array(
      'test' => t('Testing'),
      'prod' => t('Production'),
    ),
    '#default_value' => $settings['commerce_everyday_mode'],
  );
  // Production mode.
  $form['prod_mode'] = array(
    '#title' => t('Production mode settings'),
    '#type' => 'fieldset',
    '#description' => t('This fieldset contains fields for production mode configuration'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['prod_mode']['commerce_everyday_sid'] = array(
    '#type' => 'textfield',
    '#title' => t('Merchant account ID'),
    '#description' => t('Your merchant account ID in everyday service.'),
    '#default_value' => $settings['prod_mode']['commerce_everyday_sid'],
    '#size' => 20,
  );
  $form['prod_mode']['commerce_everyday_secret_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Secret key for order verification'),
    '#description' => t('The secret key used for order vefification.'),
    '#default_value' => $settings['prod_mode']['commerce_everyday_secret_key'],
    '#size' => 50,
  );
  $form['prod_mode']['commerce_everyday_secret_key_version'] = array(
    '#type' => 'textfield',
    '#title' => t('Secret key version'),
    '#description' => t('The secret key version.'),
    '#default_value' => $settings['prod_mode']['commerce_everyday_secret_key_version'],
    '#size' => 3,
  );
  // Test Mode.
  $form['test_mode'] = array(
    '#title' => t('Test Mode settings'),
    '#type' => 'fieldset',
    '#description' => t('This fieldset contains fields for test mode configuration'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['test_mode']['commerce_everyday_test_sid'] = array(
    '#type' => 'textfield',
    '#title' => t('Merchant account ID (test mode)'),
    '#description' => t('Your merchant account ID in everyday service while in test mode.'),
    '#default_value' => $settings['test_mode']['commerce_everyday_test_sid'],
    '#size' => 20,
  );
  $form['test_mode']['commerce_everyday_test_secret_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Secret key for order verification (test mode)'),
    '#description' => t('The secret key used for order vefification in test mode.'),
    '#default_value' => $settings['test_mode']['commerce_everyday_test_secret_key'],
    '#size' => 50,
  );
  $form['test_mode']['commerce_everyday_test_secret_key_version'] = array(
    '#type' => 'textfield',
    '#title' => t('Secret key version (test mode)'),
    '#description' => t('The secret key version in test mode.'),
    '#default_value' => $settings['test_mode']['commerce_everyday_test_secret_key_version'],
    '#size' => 3,
  );
  // Module.
  $form['commerce_everyday_method_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Payment method title'),
    '#description' => t('Payment method title shown in payment method selection list.'),
    '#default_value' => $settings['commerce_everyday_method_title'],
  );
  $form['commerce_everyday_method_title_icons'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show icon beside the payment method title.'),
    '#default_value' => $settings['commerce_everyday_method_title_icons'],
  );
  $form['commerce_everyday_method_title_icon_select'] = array(
    '#type' => 'select',
    '#title' => t('Select icon type'),
    '#options' => array(
      'everyday_banner.jpg' => t('JPG'),
      'everyday_banner_animated.gif' => t('Animated GIF'),
    ),
    '#default_value' => $settings['commerce_everyday_method_title_icon_select'],
  );
  $form['commerce_everyday_method_offsite_autoredirect'] = array(
    '#type' => 'checkbox',
    '#title' => t('Redirect automatically to Everyday Online Payment site at checkout'),
    '#default_value' => $settings['commerce_everyday_method_offsite_autoredirect'],
  );
  $form['commerce_everyday_method_payment_msg'] = array(
    '#type' => 'textfield',
    '#title' => t('Payment details message'),
    '#description' => t('Payment details message when Everyday payment method selected'),
    '#default_value' => $settings['commerce_everyday_method_payment_msg'],
  );
  $form['commerce_everyday_checkout_button'] = array(
    '#type' => 'textfield',
    '#title' => t('Order review submit button text'),
    '#description' => t('Provide Everyday Online Payment specific text for the submit button on the order review page.'),
    '#default_value' => $settings['commerce_everyday_checkout_button'],
  );
  $form['commerce_everyday_reference_prefix'] = array(
    '#type' => 'textfield',
    '#title' => t('Reference number prefix'),
    '#description' => t("Reference number prefix to be used while providing order's reference number. Reference number\'s minimum length is 3 digits and check number. Reference number cannot start with 0."),
    '#default_value' => $settings['commerce_everyday_reference_prefix'],
  );

  return $form;
}
