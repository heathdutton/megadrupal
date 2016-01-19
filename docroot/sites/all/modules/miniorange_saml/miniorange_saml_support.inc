<?php
/**
 * @file
 * Contains support form for miniOrange SAML Login Module.
 */

 /**
 * Showing Support form info.
 */
 function miniorange_saml_support($form, &$form_state) {

  $form['markup_1'] = array(
    '#markup' => '<h3>Support</h3><div>Need any help? Just send us a query so we can help you.<br /><br /></div>',
  );

  $form['miniorange_saml_email_address'] = array(
    '#type' => 'textfield',
    '#title' => t('Email Address'),
    '#attributes' => array('placeholder' => 'Enter your email'),
  );

  $form['miniorange_saml_phone_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Phone number'),
    '#attributes' => array('placeholder' => 'Enter your phone number'),
  );

  $form['miniorange_saml_support_query'] = array(
    '#type' => 'textarea',
    '#title' => t('Query'),
    '#cols' => '10',
    '#rows' => '5',
    '#attributes' => array('placeholder' => 'Write your query here'),
    '#required' => TRUE,
  );

  $form['miniorange_saml_support_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit Query'),
    '#submit' => array('miniorange_saml_send_query'),
  );

  return $form;

 }

 /**
  * Send support query.
  */
 function miniorange_saml_send_query($form, &$form_state) {
    $email = $form['miniorange_saml_email_address']['#value'];
    $phone = $form['miniorange_saml_phone_number']['#value'];
    $query = $form['miniorange_saml_support_query']['#value'];
    $support = new MiniOrangeSamlSupport($email, $phone, $query);
    $support_response = $support->sendSupportQuery();
    if($support_response) {
      drupal_set_message(t('Support query successfully sent'));
    }
    else {
    	drupal_set_message(t('Error sending support query'), 'error');
    } 
 }