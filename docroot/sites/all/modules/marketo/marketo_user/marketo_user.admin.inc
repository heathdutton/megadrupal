<?php
/**
 * @file
 * Administrative settings for the Marketo Webform Integration
 */


/**
 * Page callback for admin/config/marketo-webform
 */
function marketo_user_settings_form() {
  $form = array();
  if (module_exists('marketo_munchkin')) {
    $form['marketo_user_soap'] = array(
      '#type' => 'radios',
      '#title' => t('Marketo API type'),
      '#description' => t('Specify the way you wish user fields to be sent to Marketo upon signup.'),
      '#options' => array(
        0 => t('Munchkin API'),
        1 => t('SOAP API'),
      ),
      '#default_value' => variable_get('marketo_user_soap', 1),
    );
  }
  else {
    $form['munchkin'] = array(
      '#markup' => '<em>' . t('Enable the @m module to use the Marketo Munchkin API', array('@m' => 'marketo_munchkin')) . '</em>',
    );
  }
  $form['marketo_user_soap_endpoint'] = array(
    '#type' => 'textfield',
    '#title' => t('Marketo SOAP API Endpoint'),
    '#description' => t('The SOAP API Endpoint for your Marketo Instance'),
    '#default_value' => variable_get('marketo_user_soap_endpoint', 'https://na-k.marketo.com/soap/mktows/1_6'),
  );
  

  $form['marketo_user_soap_access_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Marketo SOAP API Access Key'),
    '#description' => t('The SOAP API Access Key for your Marketo Instance'),
    '#default_value' => variable_get('marketo_user_soap_access_key', ''),
  );
  $form['marketo_user_soap_secret_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Marketo SOAP API Secret Key'),
    '#description' => t('The SOAP API Secret Key for your Marketo Instance'),
    '#default_value' => variable_get('marketo_user_soap_secret_key', ''),
  );


	//the Salesforce stuff  


  $form['marketo_user_sf_campaign_id'] = array(
  	'#prefix' => '<h3>Salesforce Info</h3>',
    '#type' => 'textfield',
    '#title' => t('Campaign ID'),
    '#description' => t('The Salesforce Campaign ID where the user records will live'),
    '#default_value' => variable_get('marketo_user_sf_campaign_id', 'xxxxxxxxxxxxx'),
  );

  $form['marketo_user_sf_latest_form'] = array(
    '#type' => 'textfield',
    '#title' => t('Latest Form'),
    '#default_value' => variable_get('marketo_user_sf_latest_form', 'xxxxxxxxxxxxx'),
  );

  $form['marketo_user_sf_initial_contact'] = array(
    '#type' => 'textfield',
    '#title' => t('Initial Contact'),
    '#default_value' => variable_get('marketo_user_sf_initial_contact', 'xxxxxxxxxxxxx'),
  );
  
  $form['marketo_user_sf_leadsource'] = array(
    '#type' => 'textfield',
    '#title' => t('Lead Source'),
    '#default_value' => variable_get('marketo_user_sf_leadsource', 'unknown'),
  );
  
  return system_settings_form($form);

} 