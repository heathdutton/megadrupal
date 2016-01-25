<?php

/**
 * @file
 * Lists module's variables.
 */

/**
 * Implements hook_group_info().
 */
function commerce_avangard_group_info() {
  $groups = array();

  $groups['commerce_avangard_main_settings'] = array(
    'title' => t('Main Avangard payment setttings'),
    'access' => 'configure commerce avangard',
  );

  return $groups;
}

/**
 * Implements hook_variable_info().
 */
function commerce_avangard_variable_info() {
  $variables = array();

  $variables['commerce_avangard_merchant_id'] = array(
    'title' => t('Merchant ID'),
    'description' => t('This is the merchant ID that Avangard sent you when you set up your account.'),
    'group' => 'commerce_avangard_main_settings',
    'element' => array(
      '#type' => 'textfield',
      '#size' => 10,
    ),
    'default' => '',
  );
  $variables['commerce_avangard_hash_password'] = array(
    'title' => t('Password'),
    'group' => 'commerce_avangard_main_settings',
    'description' => t('This is the hash password that Avangard sent you.'),
    'element' => array(
      '#type' => 'textfield',
      '#size' => 10,
    ),
    'default' => '',
  );
  $variables['commerce_avangard_server_timeout'] = array(
    'title' => t('Server connection timeout (sec)'),
    'description' => t('Time to wait till bank server respond to API request.'),
    'group' => 'commerce_avangard_main_settings',
    'element' => array(
      '#type' => 'textfield',
      '#size' => 2,
    ),
    'default' => 60,
  );
  $variables['commerce_avangard_payment_process_timeout'] = array(
    'title' => t('Time to wait till customer finishes payments (hours)'),
    'description' => t('If customer finished payment process but never returned to the site we keep checking bank if it has information about payment during this period.'),
    'group' => 'commerce_avangard_main_settings',
    'element' => array(
      '#type' => 'textfield',
      '#size' => 2,
    ),
    'default' => 24,
  );
  $variables['commerce_avangard_log_api_requests'] = array(
    'title' => t('Store all API requests in order "data" for logging purposes.'),
    'description' => t("Each time module makes request to API regarding order's state, it can log request's info and store it to order's 'data' field."),
    'group' => 'commerce_avangard_main_settings',
    'element' => array(
        '#type' => 'checkbox',
    ),
    'default' => FALSE,
  );

  return $variables;
}
