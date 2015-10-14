<?php

/**
 * @file
 * Handles admin settings page for NoMoreCaptchas module.
 */

/**
 * Implements hook_form().
 */
function no_more_captchas_admin_register_key($form, &$form_state) {
  $no_more_captchas_info = '<h3>' . t('Subscription License') . '</h3>';
  $no_more_captchas_info .= '<div><b>' . t('Don\'t have a License Key? Register <a href="@nmc-link" target="_blank">here</a> for one now!', array('@nmc-link' => 'http://www.nomorecaptchas.com/register/')) . '</b></div>';
  $no_more_captchas_info .= '<div>' . t('Your License Key holds details about your organisation, domain and the subscription level you have purchased. In order to use NoMoreCapthas, you must first validate your License Key.') . '</div>';
  $no_more_captchas_info .= '<div>' . t('Enter it in the field below, click <strong>"Validate License Key"</strong> and the information you registered with will appear, along with your Authenticating Code. You can use your Authenticating Code to access your NoMoreCaptchas <a href="@nmc-dashboard-link" target="_blank">dashboard</a> and see it in action!', array('@nmc-dashboard-link' => 'http://nomorecaptchas.com/customer-dashboard/')) . '</div>';

  $form['no_more_captchas_admin_settings_text'] = array(
    '#type' => 'item',
    '#markup' => $no_more_captchas_info,
  );

  $form['no_more_captchas_authkey'] = array(
    '#type' => 'textarea',
    '#title' => t('License Key'),
    '#description' => t('Paste your NoMoreCaptchas License Key in above text area.'),
    '#default_value' => variable_get('no_more_captchas_authkey', ''),
    '#required' => TRUE,
  );

  $form['no_more_captchas_sub_level'] = array(
    '#type' => 'textfield',
    '#title' => t('Subscription Level'),
    '#default_value' => variable_get('no_more_captchas_sub_level', 'not-set'),
    '#disabled' => TRUE,
  );

  $form['no_more_captchas_valid_from'] = array(
    '#type' => 'textfield',
    '#title' => t('Valid From'),
    '#default_value' => variable_get('no_more_captchas_valid_from', 'not-set'),
    '#disabled' => TRUE,
  );

  $form['no_more_captchas_org_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Organisation Name'),
    '#default_value' => variable_get('no_more_captchas_org_name', 'not-set'),
    '#disabled' => TRUE,
  );

  $form['no_more_captchas_org_address'] = array(
    '#type' => 'textfield',
    '#title' => t('Organisation City & Country'),
    '#default_value' => variable_get('no_more_captchas_org_address', 'not-set'),
    '#disabled' => TRUE,
  );

  $form['no_more_captchas_domain'] = array(
    '#type' => 'textfield',
    '#title' => t('Primary Domain'),
    '#default_value' => variable_get('no_more_captchas_domain', 'not-set'),
  );

  $form['no_more_captchas_contact_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Contact Person'),
    '#default_value' => variable_get('no_more_captchas_contact_name', 'not-set'),
    '#disabled' => TRUE,
  );

  $form['no_more_captchas_contact_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Contact E-mail Address'),
    '#default_value' => variable_get('no_more_captchas_contact_email', 'not-set'),
    '#disabled' => TRUE,
  );

  $form['no_more_captchas_authcode'] = array(
    '#type' => 'textfield',
    '#title' => t('Authenticating Code'),
    '#default_value' => variable_get('no_more_captchas_authcode', ''),
  );

  // Submit button.
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Validate License Key'),
  );

  return $form;
}

/**
 * Implements hook_form_validate().
 */
function no_more_captchas_admin_register_key_validate($form, &$form_state) {
  $xb_authkey64 = $form_state['values']['no_more_captchas_authkey'];

  if (strlen($xb_authkey64) > 50) {
    $xb_authkey64 = str_replace("=== START OF KEY ===", "", $xb_authkey64);
    $xb_authkey64 = str_replace("=== END OF KEY ===", "", $xb_authkey64);
    $xb_authkey = base64_decode($xb_authkey64);
    $xb_authkey_array_raw = explode("|", $xb_authkey);
    if (count($xb_authkey_array_raw) < 9) {
      form_set_error('no_more_captchas_authkey', t('Your key is not a valid license key.'));
    }
  }
  else {
    form_set_error('no_more_captchas_authkey', t('Your key is not a valid license key.'));
  }
}

/**
 * Implements hook_form_submit().
 */
function no_more_captchas_admin_register_key_submit($form, &$form_state) {
  $xb_authkey64 = $form_state['values']['no_more_captchas_authkey'];
  $xb_authkey_array = array('not-set', 'not-set', 'not-set', 'not-set',
    'not-set', 'not-set', 'not-set', 'not-set');
  if (strlen($xb_authkey64) > 50) {
    $xb_authkey64 = str_replace("=== START OF KEY ===", "", $xb_authkey64);
    $xb_authkey64 = str_replace("=== END OF KEY ===", "", $xb_authkey64);
    $xb_authkey = base64_decode($xb_authkey64);
    $xb_authkey_array_raw = explode("|", $xb_authkey);
    if (count($xb_authkey_array_raw) == 9) {
      $xb_authkey_array = $xb_authkey_array_raw;
      $xb_authkey_array[3] = $xb_authkey_array[3] . ' ' . $xb_authkey_array[4];
    }
  }

  variable_set('no_more_captchas_authkey', $xb_authkey64);
  variable_set('no_more_captchas_sub_level', $xb_authkey_array[1]);
  variable_set('no_more_captchas_valid_from', $xb_authkey_array[0]);
  variable_set('no_more_captchas_org_name', $xb_authkey_array[2]);
  variable_set('no_more_captchas_org_address', $xb_authkey_array[3]);
  variable_set('no_more_captchas_domain', $xb_authkey_array[5]);
  variable_set('no_more_captchas_contact_name', $xb_authkey_array[6]);
  variable_set('no_more_captchas_contact_email', $xb_authkey_array[7]);
  variable_set('no_more_captchas_authcode', $xb_authkey_array[8]);
}
