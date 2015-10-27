<?php

/**
 * @file
 *  TODO
 */

/**
 * Implements hook_variable_group_info().
 */
function healthvault_connect_variable_group_info() {

  $groups['healthvault_connect'] = array(
    'title' => t('HealthVault® Connect'),
    'description' => t(''),
    'access' => 'access administration pages',
    'path' => array('admin/config/administration/healthvault'),
  );

  return $groups;
}

/**
 * Implements hook_variable_info().
 */
function healthvault_connect_variable_info($options) {

  $variables['healthvault_connect_app_id'] = array(
    'type' => 'string',
    'title' => t('App Id', array(), $options),
    'default' => '',
    'description' => t("Your HealthVault® application's AppId."),
    'required' => TRUE,
    'localize' => FALSE,
    'group' => 'healthvault_connect',
  );

  $variables['healthvault_connect_thumb_print'] = array(
    'type' => 'string',
    'title' => t('Certificate: Thumb Print', array(), $options),
    'default' => '',
    'description' => t("Thumb print of your certificate. Get it from https://config.healthvault-ppe.com/AppEditPublicKey.aspx?appid=YOUR_APP_ID"),
    'required' => TRUE,
    'localize' => FALSE,
    'group' => 'healthvault_connect',
  );

  $variables['healthvault_connect_private_key'] = array(
    'type' => 'text',
    'title' => t('Certificate: Private Key', array(), $options),
    'default' => '',
    'description' => t("The private key you used to sign your HealthVault® application. You can copy the content of you private key directly into this field or specify a path to your private key file that must be readable for drupal."),
    'required' => TRUE,
    'localize' => FALSE,
    'group' => 'healthvault_connect',
  );

  $variables['healthvault_connect_platform_name'] = array(
    'type' => 'string',
    'title' => t('Platform Name', array(), $options),
    'default' => 'Microsoft® HealthVault®',
    'description' => t("The HealthVault® platform name. The string will be used in several status massages and might be presented to the user."),
    'required' => TRUE,
    'localize' => TRUE,
    'group' => 'healthvault_connect',
  );

  $variables['healthvault_connect_ppe'] = array(
    'type' => 'boolean',
    'title' => t('Pre Production Environment', array(), $options),
    'default' => 1,
    'description' => t("Use HealthVault® Pre Production Environment (PPE)."),
    'required' => FALSE,
    'localize' => TRUE,
    'group' => 'healthvault_connect',
  );

  $variables['healthvault_connect_instance_1'] = array(
    'type' => 'boolean',
    'title' => t('US Instance', array(), $options),
    'default' => 1,
    'description' => t("Check if your application is available at the US instance of HealthVault®."),
    'required' => FALSE,
    'localize' => FALSE,
    'group' => 'healthvault_connect',
  );

  $variables['healthvault_connect_instance_2'] = array(
    'type' => 'boolean',
    'title' => t('UK Instance', array(), $options),
    'default' => 0,
    'description' => t("Check if your application is available at the UK instance of HealthVault®."),
    'required' => FALSE,
    'localize' => FALSE,
    'group' => 'healthvault_connect',
  );

  $variables['healthvault_action_target_redirect_url'] = array(
    'type' => 'string',
    'title' => t('Action URL', array(), $options),
    'default' => 'healthvault/redirect',
    'description' => t("Copy this value to the corresponding field in your HealthVault® Application Configuration Center.") . ' <b>TODO</b> show complete URL.',
    'required' => FALSE,
    'localize' => TRUE,
    'group' => 'healthvault_connect',
  );

  $variables['healthvault_connect_action_target_home'] = array(
    'type' => 'string',
    'title' => t('Action Target "Home"', array(), $options),
    'default' => '',
    'description' => t("Internal drupal path like node/27 which shows details about the app."),
    'required' => FALSE,
    'localize' => TRUE,
    'group' => 'healthvault_connect',
  );

  $variables['healthvault_connect_action_target_serviceagreement'] = array(
    'type' => 'string',
    'title' => t('Action Target "ServiceAgreement"', array(), $options),
    'default' => '',
    'description' => t("Internal drupal path like node/27 which shows the service agreement."),
    'required' => FALSE,
    'localize' => TRUE,
    'group' => 'healthvault_connect',
  );

  $variables['healthvault_connect_action_target_privacy'] = array(
    'type' => 'string',
    'title' => t('Action Target "Privacy"', array(), $options),
    'default' => '',
    'description' => t("Internal drupal path like node/27 which shows the privacy statement."),
    'required' => FALSE,
    'localize' => TRUE,
    'group' => 'healthvault_connect',
  );

  return $variables;
}
