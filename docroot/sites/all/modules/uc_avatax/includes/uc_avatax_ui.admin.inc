<?php

/**
 * @file
 * Admin settings for uc_avatax.
 *
 * Copyright (C) Alexander Bischoff, adTumbler.com
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

/**
 * Admin settings menu callback.
 */
function uc_avatax_ui_admin_settings($form, &$form_state) {
  // Set AvaTax license details.
  $form['uc_avatax_product_version'] = array(
    '#title' => t('AvaTax Version'),
    '#description' => t('Select AvaTax Trial - Select AvaTax Basic or AvaTax Pro to enter AvaTax Credentials'),
    '#type' => 'select',
    '#options' => array(
      UC_AVATAX_TRIAL_VERSION => t('AvaTax Trial'),
      UC_AVATAX_BASIC_VERSION => t('AvaTax Basic'),
      UC_AVATAX_PRO_VERSION => t('AvaTax Pro'),
    ),
    '#default_value' => variable_get('uc_avatax_product_version', UC_AVATAX_TRIAL_VERSION),
    '#ajax' => array(
      'callback' => 'uc_avatax_ui_update_form_options',
      'wrapper' => 'uc_avatax_options',
    ),
  );

  $version = variable_get('uc_avatax_product_version', UC_AVATAX_TRIAL_VERSION);
  if (isset($form_state['values']['uc_avatax_product_version']) && $form_state['values']['uc_avatax_product_version']) {
    $version = $form_state['values']['uc_avatax_product_version'];
  }

  $form['options'] = array(
    '#type' => 'container',
    '#prefix' => '<div id="uc_avatax_options">',
    '#suffix' => '</div>',
    '#tree' => FALSE,
  );

  if ($version != UC_AVATAX_TRIAL_VERSION) {
    $form['options']['uc_avatax_company_code'] = array(
      '#title' => t('Company Code'),
      '#description' => t('Enter the Company Code found under Organization in your AvaTax Dashboard'),
      '#type' => 'textfield',
      '#required' => TRUE,
      '#default_value' => variable_get('uc_avatax_company_code', ''),
    );
  }

  // Configure sales tax description to be shown to users.
  $form['options']['uc_avatax_tax_description'] = array(
    '#title' => t('Sales Tax Description'),
    '#description' => t('The Sales Tax description to be displayed on the order check out form'),
    '#type' => 'textfield',
    '#default_value' => variable_get('uc_avatax_tax_description', 'Sales tax'),
  );

  $form['options']['uc_avatax_show_loc'] = array(
    '#title' => t('Show location code'),
    '#description' => t('Select Yes to include the City name in your Sales Tax description'),
    '#type' => 'radios',
    '#options' => array('0' => t('No'), '1' => t('Yes')),
    '#default_value' => variable_get('uc_avatax_show_loc', '1'),
  );  

  $form['options']['uc_avatax_show_zero'] = array(
    '#title' => t('Show zero taxes'),
    '#description' => t('Select "Yes" to display a sales tax line for zero tax results'),
    '#type' => 'radios',
    '#options' => array('0' => t('No'), '1' => t('Yes')),
    '#default_value' => variable_get('uc_avatax_show_zero', '1'),
  );

  $form['options']['shipping'] = array(
    '#type' => 'fieldset',
    '#title' => t('Shipping settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => FALSE,
  );

  $states = array(
    'AL' => t('Alabama'),
    'AK' => t('Alaska'),
    'AZ' => t('Arizona'),
    'AR' => t('Arkansas'),
    'CA' => t('California'),
    'CO' => t('Colorado'),
    'CT' => t('Connecticut'),
    'DE' => t('Delaware'),
    'DC' => t('District Of Columbia'),
    'FL' => t('Florida'),
    'GA' => t('Georgia'),
    'HI' => t('Hawaii'),
    'ID' => t('Idaho'),
    'IL' => t('Illinois'),
    'IN' => t('Indiana'),
    'IA' => t('Iowa'),
    'KS' => t('Kansas'),
    'KY' => t('Kentucky'),
    'LA' => t('Louisiana'),
    'ME' => t('Maine'),
    'MD' => t('Maryland'),
    'MA' => t('Massachusetts'),
    'MI' => t('Michigan'),
    'MN' => t('Minnesota'),
    'MS' => t('Mississippi'),
    'MO' => t('Missouri'),
    'MT' => t('Montana'),
    'NE' => t('Nebraska'),
    'NV' => t('Nevada'),
    'NH' => t('New Hampshire'),
    'NJ' => t('New Jersey'),
    'NM' => t('New Mexico'),
    'NY' => t('New York'),
    'NC' => t('North Carolina'),
    'ND' => t('North Dakota'),
    'OH' => t('Ohio'),
    'OK' => t('Oklahoma'),
    'OR' => t('Oregon'),
    'PA' => t('Pennsylvania'),
    'RI' => t('Rhode Island'),
    'SC' => t('South Carolina'),
    'SD' => t('South Dakota'),
    'TN' => t('Tennessee'),
    'TX' => t('Texas'),
    'UT' => t('Utah'),
    'VT' => t('Vermont'),
    'VA' => t('Virginia'),
    'WA' => t('Washington'),
    'WV' => t('West Virginia'),
    'WI' => t('Wisconsin'),
    'WY' => t('Wyoming'),
    'AA' => t('Armed Forces (Americas)'),
    'AE' => t('Armed Forces (Europe, Canada, Middle East, Africa)'),
    'AP' => t('Armed Forces (Pacific)'),
    'AS' => t('American Samoa'),
    'FM' => t('Federated States of Micronesia'),
    'GU' => t('Guam'),
    'MH' => t('Marshall Islands'),
    'MP' => t('Northern Mariana Islands'),
    'PW' => t('Palau'),
    'PR' => t('Puerto Rico'),
    'VI' => t('Virgin Islands'),
  );

  // Limit the list of states to use AvaTax for sales tax calculations.
  $form['options']['shipping']['uc_avatax_select_states'] = array(
    '#title' => t('AvaTax Selected States'),
    '#description' => t('Select States - Leave blank for all states'),
    '#type' => 'select',
    '#multiple' => TRUE,
    '#options' => $states,
    '#default_value' => variable_get('uc_avatax_select_states', array()),
  );

  // Set Shipping Tax code to be used by AvaTax.
  $form['options']['shipping']['uc_avatax_shipcode'] = array(
    '#title' => t('Shipping Tax Code'),
    '#description' => t('The Sales Tax code to be used for Shipping'),
    '#type' => 'textfield',
    '#default_value' => variable_get('uc_avatax_shipcode', 'FR020100'),
  );

  $address_options = array(
    'Billing' => t('Billing'),
    'Shipping' => t('Shipping'),
  );

  // Configure address to use for sales tax.
  $form['options']['shipping']['uc_avatax_tax_address'] = array(
    '#title' => t('Select Destination Address to use for Sales Tax'),
    '#description' => t('Select Shipping address if you have physical goods to ship'),
    '#type' => 'select',
    '#options' => $address_options,
    '#default_value' => variable_get('uc_avatax_tax_address', 'Billing'),
  );

  // Set Street, City, State and Zip for Primary Business Office Location.
  $form['options']['shipping']['uc_avatax_primary_street1'] = array(
    '#title' => t('Primary Business Street 1'),
    '#description' => t('The Primary Street 1 your business is located in'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => variable_get('uc_avatax_primary_street1', ''),
  );

  $form['options']['shipping']['uc_avatax_primary_street2'] = array(
    '#title' => t('Primary Business Street 2'),
    '#description' => t('The Primary Street 2 your business is located in'),
    '#type' => 'textfield',
    '#default_value' => variable_get('uc_avatax_primary_street2', ''),
  );

  $form['options']['shipping']['uc_avatax_primary_city'] = array(
    '#title' => t('Primary Business City'),
    '#description' => t('The Primary City your business is located in'),
    '#required' => TRUE,
    '#type' => 'textfield',
    '#default_value' => variable_get('uc_avatax_primary_city', ''),
  );

  $form['options']['shipping']['uc_avatax_primary_state'] = array(
    '#title' => t('Primary Business State'),
    '#description' => t('The Primary State your business is located in'),
    '#type' => 'select',
    '#required' => TRUE,
    '#options' => $states,
    '#default_value' => variable_get('uc_avatax_primary_state', ''),
  );

  $form['options']['shipping']['uc_avatax_primary_zip'] = array(
    '#title' => t('Primary Business Zip'),
    '#description' => t('The Primary Zip Code your business is located in. NB - Must be a Valid 5 digit zip'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => variable_get('uc_avatax_primary_zip', ''),
  );

  $options = array(
    UC_AVATAX_DEVELOPMENT_MODE => t('Development'),
  );
  if ($version != UC_AVATAX_TRIAL_VERSION) {
    $options[UC_AVATAX_PRODUCTION_MODE] = t('Production');
  }
  $form['options']['credentials'] = array(
    '#type' => 'fieldset',
    '#title' => t('Credentials'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => FALSE,
  );
  $mode = variable_get('uc_avatax_use_mode', UC_AVATAX_DEVELOPMENT_MODE);
  if (isset($form_state['values']['uc_avatax_use_mode']) && $form_state['values']['uc_avatax_use_mode']) {
    $mode = $form_state['values']['uc_avatax_use_mode'];
  }
  if ($version == UC_AVATAX_TRIAL_VERSION) {
    $mode = UC_AVATAX_DEVELOPMENT_MODE;
  }
  $form['options']['credentials']['uc_avatax_use_mode'] = array(
    '#title' => t('AvaTax Mode'),
    '#description' => t('Select Development - Only select Production if you have completed the GO LIVE process with Avalara'),
    '#type' => 'select',
    '#access' => $version == UC_AVATAX_TRIAL_VERSION ? FALSE : TRUE,
    '#options' => $options,
    '#default_value' => $mode,
    '#ajax' => array(
      'callback' => 'uc_avatax_ui_ajax_mode_credentials',
      'wrapper' => 'uc_avatax_credentials_text_fields',
    ),
  );
  if ($version == UC_AVATAX_TRIAL_VERSION) {
    $form['options']['credentials']['user_info'] = array(
      '#type' => 'fieldset',
      '#title' => t('Register for trial usage'),
      '#tree' => FALSE,
      '#description' => t('Please enter your name and email address. This information will be used to generate a Trial AvaTax account for you.'),
    );
    $form['options']['credentials']['user_info']['uc_avatax_trial_user_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Full Name'),
      '#required' => TRUE,
      '#default_value' => variable_get('uc_avatax_trial_user_name'),
      '#disabled' => variable_get('uc_avatax_trial_user_name') ? TRUE : FALSE,
    );
    $form['options']['credentials']['user_info']['uc_avatax_trial_user_email'] = array(
      '#type' => 'textfield',
      '#title' => t('Email'),
      '#required' => TRUE,
      '#default_value' => variable_get('uc_avatax_trial_user_email'),
      '#disabled' => variable_get('uc_avatax_trial_user_email') ? TRUE : FALSE,
    );
    $form['options']['credentials']['user_info']['get_trial_account'] = array(
      '#type' => 'submit',
      '#value' => t('Get trial account'),
      '#validate' => array('uc_avatax_ui_validate_user_info'),
      '#ajax' => array(
        'callback' => 'uc_avatax_ui_get_trial_account',
        'wrapper' => 'uc_avatax_trial_Development_credentials',
      ),
    );
  }
  $form['options']['credentials']['fields'] = array(
    '#type' => 'container',
    '#prefix' => '<div id="uc_avatax_credentials_text_fields">',
    '#suffix' => '</div>',
  );
  $form['options']['credentials']['fields'][$mode] = array(
    '#type' => 'container',
    '#prefix' => '<div id="uc_avatax_' . $version . '_' . $mode . '_credentials">',
    '#suffix' => '</div>',
  );

  $ac_default = variable_get('uc_avatax_' . $version . '_' . $mode . '_account');
  if ($version == UC_AVATAX_TRIAL_VERSION && isset($form_state['account_no']) && !empty($form_state['account_no'])) {
    $ac_default = $form_state['account_no'];
  }
  $license_key_default = variable_get('uc_avatax_' . $version . '_' . $mode . '_license');
  if ($version == UC_AVATAX_TRIAL_VERSION && isset($form_state['license_key']) && !empty($form_state['license_key'])) {
    $license_key_default = $form_state['license_key'];
  }
  $form['options']['credentials']['fields'][$mode]['uc_avatax_' . $version . '_' . $mode . '_account'] = array(
    '#title' => t('@mode Account number', array('@mode' => $options[$mode])),
    '#type' => 'textfield',
    '#default_value' => $ac_default,
    '#disabled' => $version == UC_AVATAX_TRIAL_VERSION ? TRUE : FALSE,
    '#required' => TRUE,
  );
  $form['options']['credentials']['fields'][$mode]['uc_avatax_' . $version . '_' . $mode . '_license'] = array(
    '#title' => t('@mode License key', array('@mode' => $options[$mode])),
    '#type' => 'textfield',
    '#default_value' => $version == UC_AVATAX_TRIAL_VERSION ? '************' . substr($license_key_default, -4) : $license_key_default,
    '#disabled' => $version == UC_AVATAX_TRIAL_VERSION ? TRUE : FALSE,
    '#required' => TRUE,
  );

  $form['options']['credentials']['fields']['validate_btn'] = array(
    '#name' => 'uc_avatax_ui_credentials_validator_button',
    '#type' => 'button',
    '#value' => t('Validate credentials'),
    '#ajax' => array(
      'wrapper' => 'credentials_fields_validator_wrapper',
      'callback' => 'uc_avatax_ui_credentials_validator_callback',
    ),
  );

  $form['options']['credentials']['fields']['validator_wrapper'] = array(
    '#type' => 'container',
    '#prefix' => '<div id="credentials_fields_validator_wrapper">',
    '#suffix' => '</div>',
  );

  if (isset($form_state['clicked_button']['#name']) == 'uc_avatax_ui_credentials_validator_button') {
    if ($form_state['values']['uc_avatax_primary_street1'] && $form_state['values']['uc_avatax_primary_city'] && $form_state['values']['uc_avatax_primary_state'] && $form_state['values']['uc_avatax_primary_zip']) {
      $address = array(
        'line1' => $form_state['values']['uc_avatax_primary_street1'],
        'line2' => $form_state['values']['uc_avatax_primary_street2'],
        'city' => $form_state['values']['uc_avatax_primary_city'],
        'state' => $form_state['values']['uc_avatax_primary_state'],
        'postal_code' => $form_state['values']['uc_avatax_primary_zip'],
      );
      $validated = uc_avatax_ui_admin_form_validate_credentials($address, $form_state['values']);
      $form['options']['credentials']['fields']['validator_wrapper']['_validation_message'] = array(
        '#type' => 'item',
        '#markup' => '<p>' . $validated[1] . '</p>',
      );
    }
    else {
      $form['options']['credentials']['validator_wrapper']['_validation_message'] = array(
        '#type' => 'item',
        '#markup' => '<p>' . t('Please enter a valid primary shipping address.') . '</p>',
      );
    }
  }

  if ($version == UC_AVATAX_PRO_VERSION || $version == UC_AVATAX_BASIC_VERSION) {
    $form['options']['erp'] = array(
      '#type' => 'fieldset',
      '#title' => t('Sales Order Processing'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['options']['erp']['uc_avatax_erp_status'] = array(
      '#title' => t('Automatic updates to AvaTax doc status'),
      '#description' => t('Select "Yes" to enable automatic updates to AvaTax doc status'),
      '#type' => 'radios',
      '#options' => array('0' => t('No'), '1' => t('Yes')),
      '#default_value' => variable_get('uc_avatax_erp_status', '0'),
      '#ajax' => array(
        'callback' => 'uc_avatax_ui_ajax_rules_options',
        'wrapper' => 'uc_avatax_erp_rules',
      ),
    );
    $form['options']['erp']['rules'] = array(
      '#type' => 'container',
      '#tree' => FALSE,
      '#prefix' => '<div id="uc_avatax_erp_rules">',
      '#suffix' => '</div>',
    );

    $rules = rules_config_load_multiple(array(
      'uc_avatax_sop_commit',
      'uc_avatax_sop_cancel',
      'uc_avatax_sop_cancel_on_delete',
    ));
    if (!empty($rules)) {
      $erp_form_value = 0;
      if (isset($form_state['values']['uc_avatax_erp_status'])) {
        $erp_form_value = $form_state['values']['uc_avatax_erp_status'];
      }
      $erp_config_value = variable_get('uc_avatax_erp_status', 0);
      if ($erp_form_value || $erp_config_value) {
        $form['options']['erp']['rules']['overwrite_rules'] = array(
          '#type' => 'checkbox',
          '#title' => t('Overwrite existing Sales Order Processing Rules'),
        );
      }
      if ($erp_config_value && !$erp_form_value) {
        $form['options']['erp']['rules']['delete_rules'] = array(
          '#type' => 'checkbox',
          '#title' => t('Delete Sales Order Processing Rules.'),
        );
      }
    }
  }

  if ($version == UC_AVATAX_PRO_VERSION || $version == UC_AVATAX_BASIC_VERSION) {
    $form['options']['exemptions'] = array(
      '#type' => 'fieldset',
      '#title' => t('AvaTax Exemption settings'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#tree' => FALSE,
    );
    $form['options']['exemptions']['uc_avatax_exemptions_status'] = array(
      '#title' => t('Administer Sales Tax Exemptions'),
      '#description' => t('Select "Yes" to enter sales tax exemption codes for registered users'),
      '#type' => 'radios',
      '#options' => array('0' => t('No'), '1' => t('Yes')),
      '#default_value' => variable_get('uc_avatax_exemptions_status', 0),
    );
    if (variable_get('uc_avatax_exemptions_status', 0)) {
      $form['options']['exemptions']['uc_avatax_exemptions_delete_field'] = array(
        '#type' => 'checkbox',
        '#title' => t('Delete exemption code field from user profile.'),
        '#description' => t('WARNING: This action can not be undone. All user data will be lost.'),
        '#states' => array(
          'visible' => array(
            ':input[name="uc_avatax_exemptions_status"]' => array(
              'value' => 0,
            ),
          ),
        ),
      );
    }
  }

  $form['#validate'][] = 'uc_avatax_ui_fix_license_key';
  $form['#submit'][] = 'uc_avatax_ui_add_erp_rules';
  $form['#submit'][] = 'uc_avatax_ui_exemption_field';

  return system_settings_form($form);
}

/**
 * Save the actual license key instead of the masked key shown to the user.
 */
function uc_avatax_ui_fix_license_key($form, &$form_state) {
  $version = variable_get('uc_avatax_product_version', UC_AVATAX_TRIAL_VERSION);
  if (isset($form_state['values']['uc_avatax_product_version']) && $form_state['values']['uc_avatax_product_version']) {
    $version = $form_state['values']['uc_avatax_product_version'];
  }
  if ($version == UC_AVATAX_TRIAL_VERSION) {
    if (isset($form_state['license_key']) && $form_state['license_key']) {
      $license_key = $form_state['license_key'];
      $form_state['values']['uc_avatax_trial_Development_license'] = $license_key;
    }
    else {
      $form_state['values']['uc_avatax_trial_Development_license'] = variable_get('uc_avatax_trial_Development_license');
    }
  }
}

/**
 * Ajax callback, returns updated shipping states form element.
 */
function uc_avatax_ui_update_form_options($form, &$form_state) {
  return $form['options'];
}

/**
 * Ajax callback, returns trial credentials form element.
 */
function uc_avatax_ui_get_trial_account($form, &$form_state) {
  return $form['options']['credentials']['fields']['Development'];
}

/**
 * Add/Delete ERP related rules.
 */
function uc_avatax_ui_add_erp_rules($form, &$form_state) {
  if (isset($form_state['values']['uc_avatax_erp_status']) && $form_state['values']['uc_avatax_erp_status']) {
    $rules_exist = FALSE;
    $rules = rules_config_load_multiple(array(
      'uc_avatax_sop_commit',
      'uc_avatax_sop_cancel',
      'uc_avatax_sop_cancel_on_delete',
    ));
    if (count($rules) == 3) {
      $rules_exist = TRUE;
    }
    // Create rules if they don't already exist or the overwrite option has been
    // selected.
    if (!$rules_exist || (isset($form_state['values']['overwrite_rules']) && $form_state['values']['overwrite_rules'])) {
      // Delete rules if the delete option has been selected.
      $rules = rules_config_load_multiple(array(
        'uc_avatax_sop_commit',
        'uc_avatax_sop_cancel',
        'uc_avatax_sop_cancel_on_delete',
      ));
      rules_config_delete(array_keys($rules));
      $commit_rule = new RulesReactionRule();
      $commit_rule->label = 'COMMIT order sales tax';
      $commit_rule->active = TRUE;
      $commit_rule->event('uc_order_status_update');
      $commit_rule->condition(rules_condition('data_is', array(
        'data:select' => 'updated-order:order-status',
        'op' => '==',
        'value' => 'completed',
      )));
      $commit_rule->action(rules_action('uc_avatax_commit_transaction', array(
        'order:select' => 'order',
      )));
      $commit_rule->save('uc_avatax_sop_commit');

      $cancel_rule = new RulesReactionRule();
      $cancel_rule->label = 'CANCEL order sales tax';
      $cancel_rule->active = TRUE;
      $cancel_rule->event('uc_order_status_update');
      $cancel_rule->condition(rules_condition('data_is', array(
        'data:select' => 'updated-order:order-status',
        'op' => '==',
        'value' => 'canceled',
      )));
      $cancel_rule->action(rules_action('uc_avatax_cancel_transaction', array(
        'order:select' => 'order',
      )));
      $cancel_rule->save('uc_avatax_sop_cancel');

      $cancel_delete_rule = new RulesReactionRule();
      $cancel_delete_rule->label = 'CANCEL order sales tax on order delete';
      $cancel_delete_rule->active = TRUE;
      $cancel_delete_rule->event('uc_order_delete');
      $cancel_delete_rule->action(rules_action('uc_avatax_cancel_transaction', array(
        'order:select' => 'order',
      )));
      $cancel_delete_rule->save('uc_avatax_sop_cancel_on_delete');
    }
  }
  else {
    // Delete rules if the delete option has been selected.
    if (isset($form_state['values']['delete_rules']) && $form_state['values']['delete_rules']) {
      $rules = rules_config_load_multiple(array(
        'uc_avatax_sop_commit',
        'uc_avatax_sop_cancel',
        'uc_avatax_sop_cancel_on_delete',
        ));
      rules_config_delete(array_keys($rules));
    }
  }
}

/**
 * Validation callback for generating a new Trial account.
 */
function uc_avatax_ui_validate_user_info(&$form, &$form_state) {
  // Only validate if the user has clicked on the 'Get trial account' button.
  if ($form_state['clicked_button']['#value'] != $form['options']['credentials']['user_info']['get_trial_account']['#value']) {
    return;
  }

  $name = $form_state['values']['uc_avatax_trial_user_name'];
  $email = $form_state['values']['uc_avatax_trial_user_email'];
  if ($message = user_validate_mail($email)) {
    form_set_error('uc_avatax_trial_user_email', $message);
  }
  else {
    $params = array(
      'name' => $name,
      'email' => $email,
    );
    $result = uc_avatax_post('/api/avatax/account/trial/new', $params, 'http://drupalsalestax.com/');
    if (!$result['body']) {
      form_set_error('user_info', t('Could not connect to DrupalSalesTax.com, Please make sure that server internet connection is working.'));
    }
    elseif ($result['body']['ResultCode'] != 'Success') {
      form_set_error('user_info', t('DrupalSalesTax.com error: %code - %msg', array('%code' => $result['body']['ResultCode'], '%msg' => $result['body']['Message'])));
    }
    else {
      $form_state['account_no'] = $result['body']['ClientNo'];
      $form_state['license_key'] = $result['body']['LicenseKey'];
    }
  }
  $form_state['rebuild'] = TRUE;
}

/**
 * Ajax: Returns container field for credential textfields.
 */
function uc_avatax_ui_ajax_mode_credentials($form, &$form_state) {
  return $form['options']['credentials']['fields'];
}

/**
 * Ajax: Return container field for Sales Order Processing rules configuration.
 */
function uc_avatax_ui_ajax_rules_options($form, &$form_state) {
  return $form['options']['erp']['rules'];
}

/**
 * Confirm AvaTax account and license by validating the primary address.
 */
function uc_avatax_ui_admin_form_validate_credentials($address, $form_values) {
  $parameters = array();
  if ($address['line1']) {
    $parameters['Line1'] = $address['line1'];
  }
  if ($address['line2']) {
    $parameters['Line2'] = $address['line2'];
  }
  if ($address['city']) {
    $parameters['City'] = $address['city'];
  }
  if ($address['state']) {
    $parameters['Region'] = $address['state'];
  }
  if ($address['postal_code']) {
    $parameters['PostalCode'] = $address['postal_code'];
  }

  $version = $form_values['uc_avatax_product_version'];
  $mode = $form_values['uc_avatax_use_mode'];
  $account = $form_values['uc_avatax_' . $version . '_' . $mode . '_account'];
  $license = $form_values['uc_avatax_' . $version . '_' . $mode . '_license'];
  
  $base_url = 'https://development.avalara.net/1.0';
  if ($mode == UC_AVATAX_PRODUCTION_MODE) {
    $base_url = 'https://rest.avalara.net/1.0';
  }

  $querystring = http_build_query($parameters);
  $curl_opts = array(
    // Return result instead of echoing.
    CURLOPT_RETURNTRANSFER => TRUE,
    // Follow redirects, Location: headers.
    CURLOPT_FOLLOWLOCATION => FALSE,
    // But dont redirect more than 10 times.
    CURLOPT_MAXREDIRS => 10,
    // Abort if network connection takes more than 5 seconds.
    CURLOPT_CONNECTTIMEOUT => 10,
    CURLOPT_SSL_VERIFYPEER => TRUE,
  );
  
  $curl_opts[CURLOPT_HTTPHEADER] = array(
    'Content-Type: text/json',
    'Authorization: Basic ' . base64_encode("$account:$license"),
    'Date: ' . date(DATE_RFC1123, REQUEST_TIME),
  );

  $url = rtrim($base_url, '/') . '/address/validate';
  if ($querystring) {
    $url .= '?' . $querystring;
  }
  $curl = curl_init($url);
  foreach ($curl_opts as $opt => $val) {
    curl_setopt($curl, $opt, $val);
  }
  $body = curl_exec($curl);
  curl_close($curl);
  if ($body === FALSE) {
    return array(FALSE, t('AvaTax request failed. This may be an out of date SSL certificates on your server.'));
  }
  elseif ($body) {
    $body_parsed = json_decode($body, TRUE);
    if ($body_parsed['ResultCode'] == 'Success') {
      return array(TRUE, t('AvaTax response confirmed using the account and license key above.'));
    }
  }
  if ($body_parsed['Messages'][0]['Summary']) {
    return array(FALSE, t('Validate credentials failed: ' . $body_parsed['Messages'][0]['Summary'] . ''));
  }
  else {
    return array(FALSE, t('Validate credentials failed: AvaTax was not able to return an error message'));
  } 
}

/**
 * Ajax callback for returning credentials validation results.
 */
function uc_avatax_ui_credentials_validator_callback($form, &$form_state) {
  return $form['options']['credentials']['fields']['validator_wrapper'];
}

/**
 * Submit callback for adding/removing exemption code field to the user profile.
 */
function uc_avatax_ui_exemption_field($form, &$form_state) {
  $exemption_status = (isset($form_state['values']['uc_avatax_exemptions_status']));
  if ($exemption_status) {
    // Exemption status is YES.
    // Create the field and instance if they do not exist.
    $field = field_info_field('avatax_exemption_code');
    if (!$field) {
      field_create_field(array(
        'cardinality' => 1,
        'field_name' => 'avatax_exemption_code',
        'settings' => array(
          'allowed_values' => array(
            'E' => 'Charitable or benevolent org',
            'H' => 'Commercial agricultural production',
            'J' => 'Direct pay permit',
            'K' => 'Direct mail',
            'A' => 'Federal government',
            'D' => 'Foreign diplomat',
            'I' => 'Industrial production / manufacturer',
            'N' => 'Local government',
            'B' => 'State government',
            'C' => 'Tribe / Status Indian / Indian Band',
            'F' => 'Religious or educational org',
            'G' => 'Resale',
            'L' => 'Other',
          ),
        ),
        'type' => 'list_text',
      ));
    }
    $instance = field_info_instance('user', 'avatax_exemption_code', 'user');
    if (!$instance) {
      field_create_instance(array(
        'bundle' => 'user',
        'display' => array(
          'default' => array(
            'label' => 'above',
            'module' => 'list',
            'settings' => array(),
            'type' => 'list_default',
            'weight' => 0,
          ),
        ),
        'entity_type' => 'user',
        'field_name' => 'avatax_exemption_code',
        'label' => 'AvaTax Exemption Code',
        'required' => 0,
        'settings' => array(
          'user_register_form' => 0,
        ),
        'widget' => array(
          'active' => 1,
          'module' => 'options',
          'settings' => array(),
          'type' => 'options_select',
          'weight' => 7,
        ),
      ));
    }
  }
  else {
    // Exemption status is NO.
    // If the previous status is YES, and the user has asked to delete the fields, do it!
    $previous_state = variable_get('uc_avatax_exemptions_status');
    if ($previous_state) {
      if ($form_state['values']['uc_avatax_exemptions_delete_field']) {
        $instance = field_info_instance('user', 'avatax_exemption_code', 'user');
        if ($instance) {
          field_delete_instance($instance);
        }

        $field = field_info_field('avatax_exemption_code');
        if ($field) {
          field_delete_field('avatax_exemption_code');
        }
      }
    }
  }
}
