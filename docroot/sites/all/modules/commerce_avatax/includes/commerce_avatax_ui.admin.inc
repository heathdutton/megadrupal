<?php

/**
 * @file
 * Admin settings for commerce_avatax.
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
function commerce_avatax_ui_admin_settings($form, &$form_state) {
  include_once DRUPAL_ROOT . '/includes/locale.inc';

  $product_version = variable_get('commerce_avatax_product_version', COMMERCE_AVATAX_BASIC_VERSION);
  if (isset($form_state['values']['commerce_avatax_product_version']) && $form_state['values']['commerce_avatax_product_version']) {
    $product_version = $form_state['values']['commerce_avatax_product_version'];
  }

  $avatax_use_modes = array(
    COMMERCE_AVATAX_DEVELOPMENT_MODE => t('Development'),
    COMMERCE_AVATAX_PRODUCTION_MODE => t('Production'),
  );

  $use_mode = variable_get('commerce_avatax_use_mode', COMMERCE_AVATAX_DEVELOPMENT_MODE);
  if (isset($form_state['values']['commerce_avatax_use_mode']) && $form_state['values']['commerce_avatax_use_mode']) {
    $use_mode = $form_state['values']['commerce_avatax_use_mode'];
  }

  // Set AvaTax license details.
  $form['commerce_avatax_product_version'] = array(
    '#title' => t('Select AvaTax Version'),
    '#type' => 'select',
    '#options' => array(
      COMMERCE_AVATAX_BASIC_VERSION => t('AvaTax Basic'),
      COMMERCE_AVATAX_PRO_VERSION => t('AvaTax Pro'),
    ),
    '#default_value' => variable_get('commerce_avatax_product_version', COMMERCE_AVATAX_BASIC_VERSION),
    '#ajax' => array(
      'callback' => 'commerce_avatax_ui_update_form_options',
      'wrapper' => 'commerce_avatax_options',
    ),
  );

  $form['options'] = array(
    '#type' => 'container',
    '#prefix' => '<div id="commerce_avatax_options">',
    '#suffix' => '</div>',
    '#tree' => FALSE,
  );

  // Configure sales tax description to be shown to users.
  $form['options']['commerce_avatax_tax_description'] = array(
    '#title' => t('Sales Tax Description'),
    '#description' => t('The Sales Tax description to be displayed on the order check out form'),
    '#type' => 'textfield',
    '#default_value' => variable_get('commerce_avatax_tax_description', 'Sales tax'),
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

  // Select the states that will use AvaTax to calculate the sales tax amount.
  $form['options']['shipping']['commerce_avatax_select_states'] = array(
    '#title' => t('AvaTax States'),
    '#description' => t('Select States - Leave blank for all states'),
    '#type' => 'select',
    '#multiple' => TRUE,
    '#options' => $states,
    '#default_value' => variable_get('commerce_avatax_select_states', array()),
  );

  // Set Shipping Tax code to be used by AvaTax.
  $form['options']['shipping']['commerce_avatax_shipcode'] = array(
    '#title' => t('Shipping Tax Code'),
    '#description' => t('The Sales Tax code to be used for Shipping'),
    '#type' => 'textfield',
    '#default_value' => variable_get('commerce_avatax_shipcode', 'FR020100'),
  );

  $address_options = array(
    'Billing' => t('Billing'),
  );
  if (module_exists('commerce_shipping')) {
    $address_options['Shipping'] = t('Shipping');
  }
  // Configure address to use for sales tax.
  $form['options']['shipping']['commerce_avatax_tax_address'] = array(
    '#title' => t('Destination Address to Use for Sales Tax'),
    '#description' => t('Select Shipping address if you have installed Drupal Commerce Shipping Module'),
    '#type' => 'select',
    '#options' => $address_options,
    '#default_value' => variable_get('commerce_avatax_tax_address', 'Shipping'),
    '#ajax' => array(
      'callback' => 'commerce_avatax_ui_tax_address_ajax_callback',
      'wrapper' => 'address_validation_fieldset_wrapper',
    ),
  );

  // Set Street, City, State and Zip for Primary Business Office Location.
  $form['options']['shipping']['commerce_avatax_account_name'] = array(
    '#title' => t('Business or Company Name'),
    '#description' => t('Registered Name of Company or Business'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => variable_get('commerce_avatax_account_name', ''),
  );
  $form['options']['shipping']['commerce_avatax_primary_street1'] = array(
    '#title' => t('Primary Business Street 1'),
    '#description' => t('The Primary Street 1 your business is located in'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => variable_get('commerce_avatax_primary_street1', ''),
  );
  $form['options']['shipping']['commerce_avatax_primary_street2'] = array(
    '#title' => t('Primary Business Street 2'),
    '#description' => t('The Primary Street 2 your business is located in'),
    '#type' => 'textfield',
    '#default_value' => variable_get('commerce_avatax_primary_street2', ''),
  );
  $form['options']['shipping']['commerce_avatax_primary_city'] = array(
    '#title' => t('Primary Business City'),
    '#description' => t('The Primary City your business is located in'),
    '#required' => TRUE,
    '#type' => 'textfield',
    '#default_value' => variable_get('commerce_avatax_primary_city', ''),
  );
  $form['options']['shipping']['commerce_avatax_primary_state'] = array(
    '#title' => t('Primary Business State'),
    '#description' => t('The Primary State your business is located in'),
    '#type' => 'select',
    '#options' => $states,
    '#default_value' => variable_get('commerce_avatax_primary_state', ''),
  );
  $form['options']['shipping']['commerce_avatax_primary_country'] = array(
    '#title' => t('Primary Business Country'),
    '#description' => t('The primary country your business is located in'),
    '#type' => 'select',
    '#required' => TRUE,
    '#options' => country_get_list(),
    '#default_value' => variable_get('commerce_avatax_primary_country', ''),
  );
  $form['options']['shipping']['commerce_avatax_primary_zip'] = array(
    '#title' => t('Primary Business Zip'),
    '#description' => t('The Primary Zip Code your business is located in. NB - Must be a Valid 5 digit zip'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => variable_get('commerce_avatax_primary_zip', ''),
  );

  $form['options']['address_validate_wrapper'] = array(
    '#type' => 'container',
    '#prefix' => '<div id="address_validation_fieldset_wrapper">',
    '#suffix' => '</div>',
  );
  $tax_address = variable_get('commerce_avatax_tax_address', 'Shipping');
  if (isset($form_state['values']['commerce_avatax_tax_address'])) {
    $tax_address = $form_state['values']['commerce_avatax_tax_address'];
  }
  if ($tax_address == 'Shipping') {
    $form['options']['address_validate_wrapper']['address_validate'] = array(
      '#type' => 'fieldset',
      '#title' => t('Address validation'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#tree' => FALSE,
    );
    $form['options']['address_validate_wrapper']['address_validate']['commerce_avatax_validate_address'] = array(
      '#type' => 'checkbox',
      '#title' => t('Validate checkout shipping address'),
      '#default_value' => variable_get('commerce_avatax_validate_address', TRUE),
      '#ajax' => array(
        'callback' => 'commerce_avatax_ui_address_validate_ajax_callback',
        'wrapper' => 'commerce_avatax_address_validate_options',
      ),
    );
    $form['options']['address_validate_wrapper']['address_validate']['config'] = array(
      '#type' => 'container',
      '#tree' => FALSE,
      '#prefix' => '<div id="commerce_avatax_address_validate_options">',
      '#suffix' => '</div>',
    );
    $validate_address = variable_get('commerce_avatax_validate_address', TRUE);
    if (isset($form_state['values']['commerce_avatax_validate_address'])) {
      $validate_address = $form_state['values']['commerce_avatax_validate_address'];
    }
    if ($validate_address) {
      $countries = variable_get('commerce_avatax_address_validate_countries', array('US'));
      if (!is_array($countries)) {
        $countries = array($countries);
      }
      $form['options']['address_validate_wrapper']['address_validate']['config']['commerce_avatax_address_validate_countries'] = array(
        '#type' => 'select',
        '#title' => t('Countries'),
        '#description' => t('Countries to validate addresses?'),
        '#options' => array(
          'US' => t('United States'),
        ),
        '#default_value' => $countries,
        '#required' => TRUE,
      );
      $form['options']['address_validate_wrapper']['address_validate']['config']['commerce_avatax_address_postal_code'] = array(
        '#type' => 'checkbox',
        '#title' => t('Match on postal code'),
        '#description' => t('Postal codes are 9 digits, but most people enter the first 5 digits, do you want AvaTax to match all 9 digits?'),
        '#default_value' => variable_get('commerce_avatax_address_postal_code', TRUE),
      );
      $form['options']['address_validate_wrapper']['address_validate']['config']['commerce_avatax_autocomplete_postal_code'] = array(
        '#type' => 'checkbox',
        '#title' => t('Auto complete 5 digit postal code to 9 digits'),
        '#description' => t('Automatically insert the 9 digit postal code provided by AvaTax'),
        '#default_value' => variable_get('commerce_avatax_autocomplete_postal_code', TRUE),
      );
    }
  }

  $company_default = variable_get('commerce_avatax_' . $product_version . '_' . $use_mode . '_company');
  $account_default = variable_get('commerce_avatax_' . $product_version . '_' . $use_mode . '_account');
  if (isset($form_state['clicked_button']['#value']) && isset($form_state['account_no']) && !empty($form_state['account_no'])) {
    $account_default = $form_state['account_no'];
  }
  $license_default = variable_get('commerce_avatax_' . $product_version . '_' . $use_mode . '_license');
  if (isset($form_state['clicked_button']['#value']) && isset($form_state['license_key']) && !empty($form_state['license_key'])) {
    $license_default = $form_state['license_key'];
  }

  if ($product_version == COMMERCE_AVATAX_BASIC_VERSION || $product_version == COMMERCE_AVATAX_PRO_VERSION) {
    $form['options']['credentials'] = array(
      '#type' => 'fieldset',
      '#title' => t('AvaTax Credentials'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#tree' => FALSE,
    );

    $form['options']['credentials']['commerce_avatax_use_mode'] = array(
      '#title' => t('AvaTax Mode'),
      '#description' => t('Only select Production after completing the GO LIVE process with Avalara'),
      '#type' => 'select',
      '#options' => $avatax_use_modes,
      '#default_value' => $use_mode,
      '#ajax' => array(
        'callback' => 'commerce_avatax_ui_ajax_mode_credentials',
        'wrapper' => 'commerce_avatax_credentials_text_fields',
      ),
    );

    $form['options']['credentials']['fields'] = array(
      '#type' => 'container',
      '#prefix' => '<div id="commerce_avatax_credentials_text_fields">',
      '#suffix' => '</div>',
    );

    $form['options']['credentials']['fields']['commerce_avatax_' . $product_version . '_' . $use_mode . '_company'] = array(
      '#title' => t('Company Code'),
      '#description' => t('Enter the Company Code for your AvaTax version'),
      '#type' => 'textfield',
      '#default_value' => $company_default,
      '#required' => TRUE,
    );
    $form['options']['credentials']['fields']['commerce_avatax_' . $product_version . '_' . $use_mode . '_account'] = array(
      '#title' => t('@mode Account number', array('@mode' => $use_mode)),
      '#type' => 'textfield',
      '#default_value' => $account_default,
      '#required' => TRUE,
    );
    $form['options']['credentials']['fields']['commerce_avatax_' . $product_version . '_' . $use_mode . '_license'] = array(
      '#title' => t('@mode License key', array('@mode' => $use_mode)),
      '#type' => 'textfield',
      '#default_value' => $license_default,
      '#required' => TRUE,
    );
  }

  $form['options']['credentials']['fields']['validate_btn'] = array(
    '#name' => 'commerce_avatax_ui_credentials_validator_button',
    '#type' => 'button',
    '#value' => t('Validate credentials'),
    '#ajax' => array(
      'callback' => 'commerce_avatax_ui_credentials_validator_callback',
      'wrapper' => 'credentials_fields_validator_wrapper',
    ),
  );

  $form['options']['credentials']['fields']['validator_wrapper'] = array(
    '#type' => 'container',
    '#prefix' => '<div id="credentials_fields_validator_wrapper">',
    '#suffix' => '</div>',
  );

  if (isset($form_state['clicked_button']['#name']) == 'commerce_avatax_ui_credentials_validator_button') {
    if ($form_state['values']['commerce_avatax_primary_street1'] && $form_state['values']['commerce_avatax_primary_city'] && $form_state['values']['commerce_avatax_primary_state'] && $form_state['values']['commerce_avatax_primary_zip']) {
      $validated = commerce_avatax_ui_admin_form_validate_credentials($form, $form_state);
      $form['options']['credentials']['fields']['validator_wrapper']['_validation_message'] = array(
        '#type' => 'item',
        '#markup' => '<p>' . $validated[1] . '</p>',
      );
    }
    else {
      $form['options']['credentials']['validator_wrapper']['_validation_message'] = array(
        '#type' => 'item',
        '#markup' => '<p>' . t('Please complete the required shipping address fields.') . '</p>',
      );
    }
  }

  if ($product_version == COMMERCE_AVATAX_BASIC_VERSION || $product_version == COMMERCE_AVATAX_PRO_VERSION) {
    $form['options']['erp'] = array(
      '#type' => 'fieldset',
      '#title' => t('Sales Order Processing'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['options']['erp']['commerce_avatax_erp_status'] = array(
      '#title' => t('Automatic updates to AvaTax doc status'),
      '#description' => t('Select "Yes" to enable automatic updates to AvaTax doc status'),
      '#type' => 'radios',
      '#options' => array('0' => t('No'), '1' => t('Yes')),
      '#default_value' => variable_get('commerce_avatax_erp_status', '0'),
      '#ajax' => array(
        'callback' => 'commerce_avatax_ui_ajax_rules_options',
        'wrapper' => 'commerce_avatax_erp_rules',
      ),
    );
    $form['options']['erp']['rules'] = array(
      '#type' => 'container',
      '#tree' => FALSE,
      '#prefix' => '<div id="commerce_avatax_erp_rules">',
      '#suffix' => '</div>',
    );

    $rules = rules_config_load_multiple(array(
      'commerce_avatax_sop_commit',
      'commerce_avatax_sop_cancel',
      'commerce_avatax_sop_cancel_on_delete',
    ));
    if (!empty($rules)) {
      $erp_form_value = 0;
      if (isset($form_state['values']['commerce_avatax_erp_status'])) {
        $erp_form_value = $form_state['values']['commerce_avatax_erp_status'];
      }
      $erp_config_value = variable_get('commerce_avatax_erp_status', 0);
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

  if ($product_version == COMMERCE_AVATAX_BASIC_VERSION || $product_version == COMMERCE_AVATAX_PRO_VERSION) {
    $form['options']['exemptions'] = array(
      '#type' => 'fieldset',
      '#title' => t('AvaTax Exemption settings'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#tree' => FALSE,
    );
    $form['options']['exemptions']['commerce_avatax_exemptions_status'] = array(
      '#title' => t('Administer Sales Tax Exemptions'),
      '#description' => t('Select "Yes" to enter sales tax exemption codes for registered users'),
      '#type' => 'radios',
      '#options' => array('0' => t('No'), '1' => t('Yes')),
      '#default_value' => variable_get('commerce_avatax_exemptions_status', 0),
    );
    if (variable_get('commerce_avatax_exemptions_status', 0)) {
      $form['options']['exemptions']['commerce_avatax_exemptions_delete_field'] = array(
        '#type' => 'checkbox',
        '#title' => t('Delete exemption code field from user profile.'),
        '#description' => t('WARNING: This action can not be undone. All user data will be lost.'),
        '#states' => array(
          'visible' => array(
            ':input[name="commerce_avatax_exemptions_status"]' => array(
              'value' => 0,
            ),
          ),
        ),
      );
    }
  }

  if ($product_version == COMMERCE_AVATAX_PRO_VERSION) {
    $form['options']['pro'] = array(
      '#type' => 'fieldset',
      '#title' => t('AvaTax Pro settings'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#tree' => FALSE,
    );
    $product_types = commerce_product_types();
    foreach ($product_types as $product_type => $product_type_info) {
      $product_types[$product_type] = $product_type_info['name'];
    }
    $form['options']['pro']['commerce_avatax_product_types'] = array(
      '#title' => t('Product types'),
      '#type' => 'select',
      '#multiple' => TRUE,
      '#options' => $product_types,
      '#default_value' => variable_get('commerce_avatax_product_types', array()),
      '#description' => t('Which product types should have their taxes calculated by AvaTax? Leave empty to enable for all product types.'),
    );
  }

  $form['#validate'][] = 'commerce_avatax_ui_validate_address_validation_settings';
  $form['#submit'][] = 'commerce_avatax_ui_add_pro_tax_code_fields';
  $form['#submit'][] = 'commerce_avatax_ui_add_erp_rules';
  $form['#submit'][] = 'commerce_avatax_ui_exemption_field';

  return system_settings_form($form);
}

/**
 * Disable address validation when address is taken from Billing profile.
 */
function commerce_avatax_ui_validate_address_validation_settings($form, &$form_state) {
  if ($form_state['values']['commerce_avatax_tax_address'] == 'Billing') {
    $form_state['values']['commerce_avatax_validate_address'] = FALSE;
    variable_set('commerce_avatax_validate_address', FALSE);
  }
}

/**
 * Returns address validation fieldset using ajax.
 */
function commerce_avatax_ui_tax_address_ajax_callback($form, &$form_state) {
  return $form['options']['address_validate_wrapper'];
}

/**
 * If address validation is enabled, returns its config form items.
 */
function commerce_avatax_ui_address_validate_ajax_callback($form, &$form_state) {
  return $form['options']['address_validate_wrapper']['address_validate']['config'];
}

/**
 * Ajax callback, returns updated shipping states form element.
 */
function commerce_avatax_ui_update_form_options($form, &$form_state) {
  return $form['options'];
}

/**
 * Add/Delete ERP related rules.
 */
function commerce_avatax_ui_add_erp_rules($form, &$form_state) {
  if (isset($form_state['values']['commerce_avatax_erp_status']) && $form_state['values']['commerce_avatax_erp_status']) {
    $rules_exist = FALSE;
    $rules = rules_config_load_multiple(array(
      'commerce_avatax_sop_commit',
      'commerce_avatax_sop_cancel',
      'commerce_avatax_sop_cancel_on_delete',
    ));
    if (count($rules) == 3) {
      $rules_exist = TRUE;
    }
    // Create rules if they don't already exist or the overwrite option has been
    // selected.
    if (!$rules_exist || (isset($form_state['values']['overwrite_rules']) && $form_state['values']['overwrite_rules'])) {
      // Delete rules if the delete option has been selected.
      $rules = rules_config_load_multiple(array(
        'commerce_avatax_sop_commit',
        'commerce_avatax_sop_cancel',
        'commerce_avatax_sop_cancel_on_delete',
      ));
      rules_config_delete(array_keys($rules));
      $commit_rule = new RulesReactionRule();
      $commit_rule->label = 'COMMIT order sales tax';
      $commit_rule->active = TRUE;
      $commit_rule->event('commerce_order_update');
      $commit_rule->condition(rules_condition('data_is', array(
                'data:select' => 'commerce-order:status',
                'op' => '==',
                'value:select' => 'commerce-order-unchanged:status',
              ))->negate());
      $commit_rule->condition(rules_condition('data_is', array(
        'data:select' => 'commerce-order:status',
        'op' => '==',
        'value' => 'completed',
      )));
      $commit_rule->action(rules_action('commerce_avatax_commit_transaction', array(
        'order:select' => 'commerce-order',
      )));
      $commit_rule->save('commerce_avatax_sop_commit');

      $cancel_rule = new RulesReactionRule();
      $cancel_rule->label = 'CANCEL order sales tax';
      $cancel_rule->active = TRUE;
      $cancel_rule->event('commerce_order_update');
      $cancel_rule->condition(rules_condition('data_is', array(
                'data:select' => 'commerce-order:status',
                'op' => '==',
                'value:select' => 'commerce-order-unchanged:status',
              ))->negate());
      $cancel_rule->condition(rules_condition('data_is', array(
        'data:select' => 'commerce-order:status',
        'op' => '==',
        'value' => 'canceled',
      )));
      $cancel_rule->action(rules_action('commerce_avatax_cancel_transaction', array(
        'order:select' => 'commerce-order',
      )));
      $cancel_rule->save('commerce_avatax_sop_cancel');

      $cancel_delete_rule = new RulesReactionRule();
      $cancel_delete_rule->label = 'CANCEL order sales tax on order delete';
      $cancel_delete_rule->active = TRUE;
      $cancel_delete_rule->event('commerce_order_delete');
      $cancel_delete_rule->action(rules_action('commerce_avatax_cancel_transaction', array(
        'order:select' => 'commerce-order',
      )));
      $cancel_delete_rule->save('commerce_avatax_sop_cancel_on_delete');
    }
  }
  else {
    // Delete rules if the delete option has been selected.
    if (isset($form_state['values']['delete_rules']) && $form_state['values']['delete_rules']) {
      $rules = rules_config_load_multiple(array(
        'commerce_avatax_sop_commit',
        'commerce_avatax_sop_cancel',
        'commerce_avatax_sop_cancel_on_delete',
        ));
      rules_config_delete(array_keys($rules));
    }
  }
}

/**
 * Adds AvaTax code term reference field to the selected content types.
 */
function commerce_avatax_ui_add_pro_tax_code_fields($form, &$form_state) {
  // Get out of here if a button other than 'Submit' was clicked.
  if ($form_state['clicked_button']['#value'] != $form['actions']['submit']['#value']) {
    return;
  }

  $product_version = $form_state['values']['commerce_avatax_product_version'];
  if ($product_version == COMMERCE_AVATAX_PRO_VERSION) {
    // Create the taxonomy vocabulary and default avatax code terms.
    $vocab = taxonomy_vocabulary_machine_name_load('avatax_tax_codes');
    if (!$vocab) {
      $vocab = (object) array(
        'name' => t('AvaTax Tax codes'),
        'machine_name' => 'avatax_tax_codes',
      );
      taxonomy_vocabulary_save($vocab);
      taxonomy_term_save((object) array(
                'name' => 'P0000000',
                'vid' => $vocab->vid,
      ));
      taxonomy_term_save((object) array(
                'name' => 'NT',
                'vid' => $vocab->vid,
      ));
    }

    // Get the old configuration.
    $old_product_types = variable_get('commerce_avatax_product_types', array());
    if (empty($old_product_types)) {
      $old_product_types = array_keys(commerce_product_types());
    }

    $field_name = 'avatax_code';
    $new_product_types = $form_state['values']['commerce_avatax_product_types'];
    if (empty($new_product_types)) {
      $new_product_types = array_keys(commerce_product_types());
    }

    // If the old field name and the new field name are identical,
    // just remove the field from product types that are no longer selected.
    if ($old_product_types != $new_product_types) {
      // Remove the field from the product types that have been unselected.
      foreach ($old_product_types as $old_type) {
        if (!in_array($old_type, $new_product_types)) {
          $instance = field_info_instance('commerce_product', $field_name, $old_type);
          if ($instance) {
            field_delete_instance($instance);
          }
        }
      }
    }

    // Create the field type if it doesn't exist.
    $field = field_info_field($field_name);
    if (!$field) {
      $field = field_create_field(array(
        'cardinality' => 1,
        'field_name' => check_plain($field_name),
        'settings' => array(
          'allowed_values' => array(
            0 => array(
              'vocabulary' => 'avatax_tax_codes',
            ),
          ),
        ),
        'type' => 'taxonomy_term_reference',
      ));
    }

    // Create the field instance for the selected product types.
    foreach ($new_product_types as $type) {
      $instance = field_info_instance('commerce_product', $field_name, $type);
      if (!$instance) {
        field_create_instance(array(
          'bundle' => $type,
          // @todo: set default_value
          'default_value' => NULL,
          'display' => array(
            'default' => array(
              'type' => 'hidden',
            ),
            'teaser' => array(
              'type' => 'hidden',
            ),
          ),
          'entity_type' => 'commerce_product',
          'field_name' => check_plain($field_name),
          'label' => 'AvaTax code',
          'required' => TRUE,
          'widget' => array(
            'module' => 'taxonomy',
            'settings' => array(
              'autocomplete_path' => 'taxonomy/autocomplete',
              'size' => 60,
            ),
            'type' => 'taxonomy_autocomplete',
            'weight' => -4,
          ),
        ));
      }
    }
  }
}

/**
 * Ajax: Returns container field for credential textfields.
 */
function commerce_avatax_ui_ajax_mode_credentials($form, &$form_state) {
  return $form['options']['credentials']['fields'];
}

/**
 * Ajax: Return container field for Sales Order Processing rules configuration.
 */
function commerce_avatax_ui_ajax_rules_options($form, &$form_state) {
  return $form['options']['erp']['rules'];
}

/**
 * Validate AvaTax account and license by testing the primary address.
 */
function commerce_avatax_ui_admin_form_validate_credentials($form, $form_state) {
  // Only validate if the user has clicked on the 'Validate credentials' button.
  if ($form_state['clicked_button']['#value'] != $form['options']['credentials']['fields']['validate_btn']['#value']) {
    return;
  }
  $parameters = array(
    'line1' => $form_state['values']['commerce_avatax_primary_street1'],
    'line2' => $form_state['values']['commerce_avatax_primary_street2'],
    'city' => $form_state['values']['commerce_avatax_primary_city'],
    'region' => $form_state['values']['commerce_avatax_primary_state'],
    'country' => $form_state['values']['commerce_avatax_primary_country'],
    'postalcode' => $form_state['values']['commerce_avatax_primary_zip'],
  );

  $product_version = $form_state['values']['commerce_avatax_product_version'];
  $use_mode = $form_state['values']['commerce_avatax_use_mode'];
  $account = $form_state['values']['commerce_avatax_' . $product_version . '_' . $use_mode . '_account'];
  $license = $form_state['values']['commerce_avatax_' . $product_version . '_' . $use_mode . '_license'];

  $base_url = 'https://development.avalara.net/1.0';
  if ($use_mode == COMMERCE_AVATAX_PRODUCTION_MODE) {
    $base_url = 'https://rest.avalara.net/1.0';
  }

  $querystring = http_build_query($parameters);
  $querystring = str_replace("amp;", "", $querystring);

  $curl_opts = array(
    // Return result instead of echoing.
    CURLOPT_RETURNTRANSFER => TRUE,
    // Follow redirects, Location: headers.
    CURLOPT_FOLLOWLOCATION => FALSE,
    // But do not redirect more than 10 times.
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
    return array(FALSE, t('Validate credentials failed: @body', array('@body' => $body_parsed['Messages'][0]['Summary'])));
  }
  else {
    return array(FALSE, t('Validate credentials failed: AvaTax was not able to return an error message'));
  }
}

/**
 * Ajax callback for returning credentials validation results.
 */
function commerce_avatax_ui_credentials_validator_callback($form, &$form_state) {
  return $form['options']['credentials']['fields']['validator_wrapper'];
}

/**
 * Submit callback for adding/removing exemption code field to the user profile.
 */
function commerce_avatax_ui_exemption_field($form, &$form_state) {
  $exemption_status = isset($form_state['values']['commerce_avatax_exemptions_status']) ? $form_state['values']['commerce_avatax_exemptions_status'] : FALSE;
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
    // If the previous status is YES, and user has asked to delete the fields.
    $previous_state = variable_get('commerce_avatax_exemptions_status');
    if ($previous_state) {
      if ($form_state['values']['commerce_avatax_exemptions_delete_field']) {
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
