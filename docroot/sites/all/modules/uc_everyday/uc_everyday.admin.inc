<?php

/**
 * @file
 * Ubercart Everyday settings items.
 */

/**
 * Everyday Online Payment settings form.
 */
function uc_everyday_settings_form($form, &$form_state) {
  $form['uc_everyday_mode'] = array(
    '#type' => 'select',
    '#title' => t('Mode'),
    '#description' => t('In testing mode you are able to verify Everyday Online Payment service with fake orders'),
    '#options' => array(
      '1' => t('Testing'),
      '0' => t('Production'),
    ),
    '#default_value' => variable_get('uc_everyday_mode', '1'),
  );

  $form['prod_mode'] = array(
    '#title' => t('Production mode settings'),
    '#type' => 'fieldset',
    '#description' => t('This fieldset contains fields for production mode configuration'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['prod_mode']['uc_everyday_sid'] = array(
    '#type' => 'textfield',
    '#title' => t('Everyday customer ID'),
    '#description' => t('Your customer ID in Everyday service.'),
    '#default_value' => variable_get('uc_everyday_sid', ''),
    '#size' => 20,
  );
  $form['prod_mode']['uc_everyday_secret_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Everyday secret key for order verification'),
    '#description' => t('The secret key used for order vefification.'),
    '#default_value' => variable_get('uc_everyday_secret_key', ''),
    '#size' => 50,
  );
  $form['prod_mode']['uc_everyday_secret_key_version'] = array(
    '#type' => 'textfield',
    '#title' => t('Everyday secret key version'),
    '#description' => t('The secret key version.'),
    '#default_value' => variable_get('uc_everyday_secret_key_version', '1'),
    '#size' => 3,
  );
  $form['test_mode'] = array(
    '#title' => t('Test Mode settings'),
    '#type' => 'fieldset',
    '#description' => t('This fieldset contains fields for test mode configuration'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['test_mode']['uc_everyday_test_sid'] = array(
    '#type' => 'textfield',
    '#title' => t('Everyday customer ID (test mode)'),
    '#description' => t('Your customer ID in Everyday service while in test mode.'),
    '#default_value' => variable_get('uc_everyday_test_sid', '5442'),
    '#size' => 20,
  );
  $form['test_mode']['uc_everyday_test_secret_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Everyday secret key for order verification (test mode)'),
    '#description' => t('The secret key used for order vefification in test mode.'),
    '#default_value' => variable_get('uc_everyday_test_secret_key', 'NGNhODg0ZjA0NjYxNzllZmQxNWRhZA'),
    '#size' => 50,
  );
  $form['test_mode']['uc_everyday_test_secret_key_version'] = array(
    '#type' => 'textfield',
    '#title' => t('Everyday secret key version (test mode)'),
    '#description' => t('The secret key version in test mode.'),
    '#default_value' => variable_get('uc_everyday_test_secret_key_version', '1'),
    '#size' => 3,
  );
  $form['uc_everyday_method_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Payment method title'),
    '#description' => t('Payment method title shown in payment method selection list.'),
    '#default_value' => variable_get('uc_everyday_method_title', 'Everyday-verkkomaksu'),
  );
  $form['uc_everyday_method_title_icons'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show icon beside the payment method title.'),
    '#default_value' => variable_get('uc_everyday_method_title_icons', TRUE),
  );
  $form['uc_everyday_method_payment_msg'] = array(
    '#type' => 'textfield',
    '#title' => t('Payment details message'),
    '#description' => t('Payment details message when Everyday payment method selected'),
    '#default_value' => variable_get('uc_everyday_method_payment_msg', t('Continue with checkout. You are directed to Everyday secure server.')),
  );
  $form['uc_everyday_checkout_button'] = array(
    '#type' => 'textfield',
    '#title' => t('Order review submit button text'),
    '#description' => t('Provide Everyday Online Payment specific text for the submit button on the order review page.'),
    '#default_value' => variable_get('uc_everyday_checkout_button', t('Proceed to Everyday')),
  );
  $form['uc_everyday_reference_prefix'] = array(
    '#type' => 'textfield',
    '#title' => t('Reference number prefix'),
    '#description' => t("Reference number prefix to be used while providing order's reference number. Reference number's minimum length is 3 digits and check number. Reference number cannot start with 0."),
    '#default_value' => variable_get('uc_everyday_reference_prefix', '1000'),
  );

  return $form;
}
