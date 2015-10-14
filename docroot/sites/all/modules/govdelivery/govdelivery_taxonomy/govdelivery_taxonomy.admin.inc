<?php
/**
 * @file
 * GovDelivery Taxonomy Module Settings UI
 */

/**
 * Builds taxonomy settings admin form.
 *
 * @return array
 *   Admin form array.
 */
function govdelivery_taxonomy_admin() {
  $form = array();
  $form['govdelivery_taxonomy_username'] = array(
    '#type' => 'textfield',
    '#title' => t('WebService Administrator Username'),
    '#default_value' => variable_get('govdelivery_taxonomy_username', ''),
    '#maxlength' => 255,
    '#required' => TRUE,
  );
  $form['govdelivery_taxonomy_password'] = array(
    '#type' => 'password',
    '#title' => t('WebService Administrator Password'),
    '#default_value' => variable_get('govdelivery_taxonomy_password', ''),
    '#maxlength' => 25,
    '#required' => TRUE,
  );
  $form['govdelivery_taxonomy_clientcode'] = array(
    '#type' => 'textfield',
    '#title' => t('GovDelivery DCM Client Account Code'),
    '#default_value' => variable_get('govdelivery_taxonomy_clientcode', ''),
    '#maxlength' => 20,
    '#required' => TRUE,
  );
  $form['govdelivery_taxonomy_server'] = array(
    '#type' => 'textfield',
    '#title' => t('GovDelivery API URL (Without HTTPS://)'),
    '#default_value' => variable_get('govdelivery_taxonomy_server', ''),
    '#maxlength' => 100,
    '#required' => TRUE,
  );
  $form['govdelivery_taxonomy_drupalserver'] = array(
    '#type' => 'textfield',
    '#title' => t('Drupal Instance Base URL (Without HTTPS://) ( This is the URL to this Drupal instance without any subdirectories)'),
    '#default_value' => variable_get('govdelivery_taxonomy_drupalserver', ''),
    '#maxlength' => 100,
    '#required' => TRUE,
  );
  $form['govdelivery_taxonomy_category'] = array(
    '#type' => 'textfield',
    '#title' => t('GovDelivery DCM Parent Category Code for Created Topics '),
    '#default_value' => variable_get('govdelivery_taxonomy_category', ''),
    '#maxlength' => 25,
    '#required' => TRUE,
  );
  return system_settings_form($form);
}
