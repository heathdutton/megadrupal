<?php
/**
 * @file
 * Callback for settings page
 */
function simple_mobile_redirect_admin_settings_form($form, &$form_state) {
  $form['simple_mobile_redirect_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Simple Mobile Redirect Settings'),
      '#description' => t('Link back to /?nomobi=true to set a cookie to stay on the full version of the site.<br/>Link to /clearsimplemobileredirect to clear the cookie and redirect to Mobile sites.'),
  );
  $form['simple_mobile_redirect_settings']['simple_mobile_redirect_home'] = array(
    '#type' => 'textfield',
    '#title' => t('non-mobile site'),
    '#default_value' => variable_get('simple_mobile_redirect_home', '/'),
      '#size' => 40,
      '#maxlength' => 255,
      '#required' => FALSE,
    '#description' => t('Enter a the non-mobile site'),
    );
  $form['simple_mobile_redirect_settings']['simple_mobile_redirect_ipad'] = array(
    '#type' => 'textfield',
    '#title' => t('Redirect iPad'),
    '#default_value' => variable_get('simple_mobile_redirect_ipad', ''),
      '#size' => 40,
      '#maxlength' => 255,
      '#required' => FALSE,
    '#description' => t('Enter a URL for iPad sites'),
    );
  $form['simple_mobile_redirect_settings']['simple_mobile_redirect_iphone'] = array(
    '#type' => 'textfield',
    '#title' => t('Redirect iPhone'),
    '#default_value' => variable_get('simple_mobile_redirect_iphone', ''),
      '#size' => 40,
      '#maxlength' => 255,
      '#required' => FALSE,
    '#description' => t('Enter a URL for iPhone/iPod touch sites'),
    );
  $form['simple_mobile_redirect_settings']['simple_mobile_redirect_android'] = array(
    '#type' => 'textfield',
    '#title' => t('Redirect Android'),
    '#default_value' => variable_get('simple_mobile_redirect_android', ''),
      '#size' => 40,
      '#maxlength' => 255,
      '#required' => FALSE,
    '#description' => t('Enter a URL for Android sites'),
    );
  $form['simple_mobile_redirect_settings']['simple_mobile_redirect_opera'] = array(
    '#type' => 'textfield',
    '#title' => t('Redirect Opera Mini'),
    '#default_value' => variable_get('simple_mobile_redirect_opera', ''),
      '#size' => 40,
      '#maxlength' => 255,
      '#required' => FALSE,
    '#description' => t('Enter a URL for Opera Mini sites'),
    );
  $form['simple_mobile_redirect_settings']['simple_mobile_redirect_blackberry'] = array(
    '#type' => 'textfield',
    '#title' => t('Redirect Blackbery'),
    '#default_value' => variable_get('simple_mobile_redirect_blackberry', ''),
      '#size' => 40,
      '#maxlength' => 255,
      '#required' => FALSE,
    '#description' => t('Enter a URL for Blackberry sites'),
    );
  $form['simple_mobile_redirect_settings']['simple_mobile_redirect_palm'] = array(
    '#type' => 'textfield',
    '#title' => t('Redirect Palm Web OS'),
    '#default_value' => variable_get('simple_mobile_redirect_palm', ''),
      '#size' => 40,
      '#maxlength' => 255,
      '#required' => FALSE,
    '#description' => t('Enter a URL for Palm OS sites'),
    );
  $form['simple_mobile_redirect_settings']['simple_mobile_redirect_windows'] = array(
    '#type' => 'textfield',
    '#title' => t('Redirect Window Mobile'),
    '#default_value' => variable_get('simple_mobile_redirect_windows', ''),
      '#size' => 40,
      '#maxlength' => 255,
      '#required' => FALSE,
    '#description' => t('Enter a URL for Windows Mobile sites'),
    );
  $form['simple_mobile_redirect_settings']['simple_mobile_redirect_mobileredirect'] = array(
    '#type' => 'textfield',
    '#title' => t('Mobile Web Site'),
    '#default_value' => variable_get('simple_mobile_redirect_mobileredirect', ''),
      '#size' => 40,
      '#maxlength' => 255,
      '#required' => FALSE,
    '#description' => t('URL to redirect to if not one of the mobile browsers above'),
    );
  return system_settings_form($form);
}

