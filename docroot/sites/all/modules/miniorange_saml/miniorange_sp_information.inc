<?php
/**
 * @file
 * Contains Service Provider information for miniOrange SAML Login Module.
 */

 /**
 * Showing Service Provider information.
 */
 function miniorange_sp_information($form, &$form_state) {

   global $base_url;
   $acs_url = $base_url . '/?q=samlassertion';

  $form['header'] = array(
    '#markup' => '<center><h3>You will need the following information to'
    . ' configure your IdP. Copy it and keep it handy</h3></center>',
  );

  $header = array(
    'issuer' => array('data' => t('Issuer')),
    'acs' => array('data' => t('ACS(Assertion Consumer Service) URL')),
    'audience' => array('data' => t('Audience URI')),
    'nameid' => array('data' => t('NameId Format')),
    'recipient' => array('data' => t('Recipient URL')),
    'destination' => array('data' => t('Destination URL')),
  );

  $options = array();

  $options[0] = array(
    'issuer' => $base_url,
    'acs' => $acs_url,
    'audience' => $acs_url,
    'nameid' => 'urn:oasis:names:tc:SAML:1.1:nameid-format:emailAddress',
    'recipient' => $acs_url,
    'destination' => $acs_url,
  );

  $form['fieldset']['spinfo'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $options,
  );

  $form['markup_13'] = array(
    '#markup' => '<h3>Identity Provider Guides</h3>',
  );

  $form['markup_14'] = array(
    '#markup' => 'With the help of the above information, you can configure your IdP. We also have guides for configuring various IdP\'s<br>'
    . '<a href="' . $base_url . '/' . drupal_get_path('module', 'miniorange_saml') . '/idp_guides/miniorange_as_idp.pdf" target="_blank">Click Here to see the Guide for Configuring miniOrange as IdP. </a><br>',
  );

  $form['markup_15'] = array(
    '#markup' => '<b>NOTE: </b> We also have step by step do-it-yourself guides available for all known IdPs like ADFS, Centrify, Okta, '
    . 'OneLogin, OpenAM, Oracle Identity Manager, JBoss Keycloak, Salesforce, Shibboleth, SimpleSAML, WSO2 etc.',
  );

  return $form;
 }
