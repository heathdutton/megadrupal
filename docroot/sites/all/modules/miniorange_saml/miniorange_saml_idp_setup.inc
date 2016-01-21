<?php
/**
 * @file
 * Contains Service Provider information for miniOrange SAML Login Module.
 */

 /**
 * Showing IdP Setup form info.
 */
 function miniorange_saml_idp_setup($form, &$form_state) {
   
  $form['miniorange_saml_idp_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Identity Provider Name'),
    '#default_value' => variable_get('miniorange_saml_idp_name', ''),
    '#attributes' => array('placeholder' => 'Identity Provider Name'),
    '#required' => TRUE,
  );

  $form['miniorange_saml_idp_issuer'] = array(
    '#type' => 'textfield',
    '#title' => t('IdP Entity ID or Issuer'),
    '#default_value' => variable_get('miniorange_saml_idp_issuer', ''),
    '#attributes' => array('placeholder' => 'IdP Entity ID or Issuer'),
    '#required' => TRUE,
  );

  $form['miniorange_saml_idp_login_url'] = array(
    '#type' => 'textfield',
    '#title' => t('SAML Login URL'),
    '#default_value' => variable_get('miniorange_saml_idp_login_url', ''),
    '#attributes' => array('placeholder' => 'SAML Login URL'),
    '#required' => TRUE,
  );

  $form['miniorange_saml_idp_x509_certificate'] = array(
    '#type' => 'textarea',
    '#title' => t('x.509 Certificate Value'),
    '#cols' => '10',
    '#rows' => '5',
    '#default_value' => variable_get('miniorange_saml_idp_x509_certificate', ''),
    '#attributes' => array('placeholder' => 'Enter x509 Certificate Value'),
    '#required' => TRUE,
  );

  $form['markup_1'] = array(
    '#markup' => '<b>NOTE:</b> Format of the certificate:<br><b>-----BEGIN CERTIFICATE-----<br>'
  );

  $form['markup_2'] = array(
    '#markup' => 'XXXXXXXXXXXXXXXXXXXXXXXXXXX<br>-----END CERTIFICATE-----</b><br><br>'
  );

  $form['miniorange_saml_response_signed'] = array(
    '#type' => 'checkbox',
    '#title' => t('Check if your IdP is signing SAML response. Leave checked by default'),
    '#default_value' => variable_get('miniorange_saml_response_signed', TRUE),
  );

  $form['miniorange_saml_assertion_signed'] = array(
    '#type' => 'checkbox',
    '#title' => t('Check if your IdP is signing SAML assertion. Leave unchecked by default'),
    '#default_value' => variable_get('miniorange_saml_assertion_signed', FALSE),
  );  

  $form['miniorange_saml_enable_login'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable login with SAML'),
    '#default_value' => variable_get('miniorange_saml_enable_login', FALSE),
  ); 

  $form['miniorange_saml_test_config_button'] = array(
    '#markup' => '<a class="btn btn-primary btn-large" style="padding:6px 12px;" onclick="testConfig(\'' . getTestUrl() . '\');">'
    . 'Test Configuration</a><br><br>'
  );

  $form['miniorange_saml_idp_config_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Configuration'),
    '#submit' => array('miniorange_saml_save_idp_config'),
  );

  return $form;

 }

 function getTestUrl() {
  global $base_url;
  $host_name = MiniorangeSAMLConstants::BASE_URL;
  $customer_key = variable_get('miniorange_saml_customer_id', '');
  $customer_token = variable_get('miniorange_saml_customer_admin_token', '');
  $url = $host_name . '/moas/idptest/?id=' . $customer_key . '&key=' . $customer_token;

  $testUrl = $base_url . '/?q=testConfig';

  return $testUrl;
 }

 /**
 * Configure IdP.
 */
 function miniorange_saml_save_idp_config($form, &$form_state) {
  global $base_url;
  $issuer = $form['miniorange_saml_idp_issuer']['#value'];
  $idp_name = $form['miniorange_saml_idp_name']['#value'];
  $login_url = $form['miniorange_saml_idp_login_url']['#value'];
  $x509_cert_value = $form['miniorange_saml_idp_x509_certificate']['#value'];
  $response_signed_value = $form['miniorange_saml_response_signed']['#value'];
  $assertion_signed_value = $form['miniorange_saml_assertion_signed']['#value'];
  $enable_login = $form['miniorange_saml_enable_login']['#value'];

  if ($response_signed_value == 1) {
    $response_signed = TRUE;
  }
  else {
    $response_signed = FALSE;
  }

  if ($assertion_signed_value == 1) {
    $assertion_signed = TRUE;
  }
  else {
    $assertion_signed = FALSE;
  }

  if ($enable_login == 1) {
    $enable_login = TRUE;
  }
  else {
    $enable_login = FALSE;
  }
  
  // Check for -----BEGIN CERTIFICATE----- .
  if (strpos($x509_cert_value, "-----BEGIN CERTIFICATE-----") === FALSE) {
    $x509_cert_value = "-----BEGIN CERTIFICATE-----\n" . $x509_cert_value;
  }
  
  // Check for -----END CERTIFICATE----- .
  if (strpos($x509_cert_value, "-----END CERTIFICATE-----") === FALSE) {
    $x509_cert_value = $x509_cert_value . "\n-----END CERTIFICATE-----";
  }



  $sp_issuer = $base_url;
  variable_set('miniorange_saml_idp_name', $idp_name);
  variable_set('miniorange_saml_sp_issuer', $sp_issuer);
  variable_set('miniorange_saml_idp_issuer', $issuer);
  variable_set('miniorange_saml_idp_login_url', $login_url);
  variable_set('miniorange_saml_idp_x509_certificate', $x509_cert_value);
  variable_set('miniorange_saml_response_signed', $response_signed);
  variable_set('miniorange_saml_assertion_signed', $assertion_signed);
  variable_set('miniorange_saml_enable_login', $enable_login);

  drupal_set_message(t('Identity Provider Configuration successfully saved'));

 }
