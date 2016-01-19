<?php
/**
 * @file
 * including support functions for commerce_techprocess
 */

/**
  * Implements conguration form to save the  variables in  tech process
  * payment.
  *
  */
function commerce_techprocess_keyset_configuration_form($form, &$form_state) {
  variable_get('commerce_techprocess_dualverf', TRUE);
  $mode_option = array();
  $mode_option = array(
    'sandbox' => t('Sandbox'),
    'live' => t('Live')
  );
  
  $form['commerce_tech_process_mode'] = array(
    '#type' => 'select',
    '#title' => t('Set the server mode'),
    '#options' => $mode_option,
    '#default_value' => variable_get('commerce_tech_process_mode', 'live')
  ); 
  $form['commerce_tech_process_mid'] = array(
    '#type' => 'textfield',
    '#size' => 15,
    '#maxlength' => 255,
    '#title' => t('Set the Merchant ID'),
    '#default_value' => variable_get('commerce_tech_process_mid', 0)
  );
  $form['commerce_tech_process_iv'] = array(
    '#type' => 'textfield',
    '#size' => 15,
    '#maxlength' => 255,
    '#title' => t('Set the Techprocess IV'),
    '#default_value' => variable_get('commerce_tech_process_iv', 0)
  );
  $form['commerce_tech_process_key'] = array(
    '#type' => 'textfield',
    '#size' => 15,
    '#maxlength' => 255,
    '#title' => t('Set the Techprocess Key'),
    '#default_value' => variable_get('commerce_tech_process_key', 0)
  );
  $form['commerce_tech_process_scheme'] = array(
    '#type' => 'textfield',
    '#size' => 15,
    '#maxlength' => 255,
    '#title' => t('Set the Techprocess Scheme Code'),
    '#default_value' => variable_get('commerce_tech_process_scheme', 0)
  );
  $form['commerce_tech_process_comm_mode'] = array(
    '#type' => 'radios',
    '#title' => t('Set the Techprocess Commission Mode'),
    '#options' => array(
     "p"=>t('Percentage'),
     "c"=>t('Fixed cash'),
      ),
    '#default_value' => variable_get('commerce_tech_process_comm_mode', 'p')
  );
  $form['commerce_tech_process_comm_amt'] = array(
    '#type' => 'textfield',
    '#size' => 15,
    '#maxlength' => 255,
    '#title' => t('Set the Techprocess Commission amount'),
    '#default_value' => variable_get('commerce_tech_process_comm_amt', 0)
  );
  return system_settings_form($form);
}


/**
 * Page to dispay payment configuration for tech process payment method.
 */
function commerce_techprocess_configurations() {
  $page['techprocess_method_config'] = drupal_get_form('commerce_techprocess_keyset_configuration_form');
  return $page;
}
