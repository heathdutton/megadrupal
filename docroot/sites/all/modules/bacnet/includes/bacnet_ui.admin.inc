<?php

/**
 * @file
 * Admin settings for BACnet.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

/**
 * Admin settings menu callback.
 */
function bacnet_ui_admin_settings($form, &$form_state) {
  $form['bacnet_product_version'] = array(
    '#title' => t('Select OpenEnergy Version'),
    '#type' => 'select',
    '#options' => array(
      BACNET_COMMUNITY => t('OpenEnergy Community'),
    ),
    '#default_value' => variable_get('bacnet_product_version', BACNET_COMMUNITY),
    '#ajax' => array(
      'callback' => 'bacnet_ui_update_form_options',
      'wrapper' => 'bacnet_options',
    ),
  );

  $form['options'] = array(
    '#type' => 'container',
    '#prefix' => '<div id="bacnet_options">',
    '#suffix' => '</div>',
    '#tree' => FALSE,
  );

  $version = variable_get('bacnet_product_version', BACNET_COMMUNITY);
  if (isset($form_state['values']['bacnet_product_version']) && $form_state['values']['bacnet_product_version']) {
    $version = $form_state['values']['bacnet_product_version'];
  }

  $mode = variable_get('bacnet_mode', BACNET_MODE_BWS1);
  if (isset($form_state['values']['bacnet_mode']) && $form_state['values']['bacnet_mode']) {
    $mode = $form_state['values']['bacnet_mode'];
  }

  if ($version == BACNET_COMMUNITY) {
    $form['options'] = array(
      '#type' => 'fieldset',
      '#title' => t('BACnet Block Configuration'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#tree' => FALSE,
    );

    $form['options']['bacnet'] = array(
      '#type' => 'container',
      '#prefix' => '<div id="bacnet_mode">',
      '#suffix' => '</div>',
      '#tree' => FALSE,
    );

    $bacnet_modes = array(
      BACNET_MODE_BWS1 => t('BWS1 SOAP'),
      BACNET_MODE_BWS2 => t('BWS2 REST'),
    );

    // Select the BACnet mode for the blocks configuration.
    $form['options']['bacnet']['bacnet_mode'] = array(
      '#title' => t('BACnet Mode'),
      '#description' => t('Select Mode applicable to this block'),
      '#type' => 'select',
      '#multiple' => FALSE,
      '#options' => $bacnet_modes,
      '#default_value' => $mode,
      '#ajax' => array(
        'callback' => 'bacnet_ui_update_form_options_mode',
        'wrapper' => 'bacnet_mode',
      ),
    );

    $form['options']['bacnet']['bacnet_bws1_server'] = array(
      '#type' => 'textfield',
      '#title' => t('BACnet Server Address'),
      '#access' => $mode == BACNET_MODE_BWS1 ? TRUE : FALSE,
      '#default_value' => variable_get('bacnet_bws1_server', ''),
      '#required' => TRUE,
      '#description' => t('Include (http:// or https://) + (domain OR IP address) + (:port) <br /> Do not include the WSDL path /_common/webservices/Eval?WSDL<br />Examples: <ul><li>Standard Port &mdash; http://192.0.43.10 or http://www.example.com</li><li>Non-standard Port &mdash; https://www.example.com:8080</li></ul>'),
    );

    $form['options']['bacnet']['bacnet_bws1_login_name'] = array(
      '#type' => 'textfield',
      '#title' => t('B-AWS Login Name'),
      '#access' => $mode == BACNET_MODE_BWS1 ? TRUE : FALSE,
      '#default_value' => variable_get('bacnet_bws1_login_name', ''),
      '#required' => TRUE,
      '#description' => t('Must have:<ul><li>Remote Data Access</li><li>Access Geographic Locations or Network Locations, as needed</li><li>Access Network items, as needed</li><li>Any privileges needed for the specific task at least Remote Data Access</li></ul>'),
    );

    $form['options']['bacnet']['bacnet_bws1_password'] = array(
      '#type' => 'password',
      '#title' => t('B-AWS Password'),
      '#access' => $mode == BACNET_MODE_BWS1 ? TRUE : FALSE,
      '#required' => TRUE,
      '#description' => t('Must have:<ul><li>Remote Data Access</li><li>Access Geographic Locations or Network Locations, as needed</li><li>Access Network items, as needed</li><li>Any privileges needed for the specific task at least Remote Data Access</li></ul>'),
    );

    $form['options']['bacnet']['bacnet_bws1_gql_expression'] = array(
      '#type' => 'textfield',
      '#title' => t('The Test Point GQL Expression (the path to the point)'),
      '#access' => $mode == BACNET_MODE_BWS1 ? TRUE : FALSE,
      '#default_value' => variable_get('bacnet_bws1_gql_expression', ''),
      '#optional' => TRUE,
      '#description' => t('Expression only needs to refer to the microblock; present_value is assumed.<br />For example, #g01_electric_meter/inst_demand'),
    );
    $form['options']['bacnet']['bacnet_bws2_vendor_id'] = array(
      '#type' => 'textfield',
      '#title' => t('BACnet Vendor id'),
      '#access' => $mode == BACNET_MODE_BWS2 ? TRUE : FALSE,
      '#default_value' => variable_get('bacnet_bws2_vendor_id', '751'),
      '#optional' => TRUE,
      '#description' => t('Vendor ID for testing integration with BWS2 REST'),
    );
  }
  return system_settings_form($form);
}

/**
 * Ajax callback, returns updated version form element.
 */
function bacnet_ui_update_form_options($form, &$form_state) {
  return $form['options'];
}

/**
 * Ajax: callback, returns container block configuration.
 */
function bacnet_ui_update_form_options_mode($form, &$form_state) {
  return $form['options']['bacnet'];
}
