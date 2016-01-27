<?php

/**
* Page callback: Reform settings
*
* @see reform_menu(). reform.module
*/
function reform_admin_settings($form, &$form_state) {

  $form['reform_user_form'] = array(
    '#type' => 'fieldset',
	'#title' => 'User Form',
  );

  $form['reform_user_form']['reform_name'] = array(
    '#type' => 'fieldset',
	'#title' => 'Username',
	'#collapsible' => 'TRUE',
  );
  
  $form['reform_user_form']['reform_name']['reform_name_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#default_value' => variable_get('reform_name_title', 'Username'),
    '#size' => 60,
    '#maxlength' => 60,
    '#description' => t('The label of the username field.'),
  );
  
  $form['reform_user_form']['reform_name']['reform_name_min'] = array(
    '#type' => 'textfield',
    '#title' => t('Minimum length'),
    '#default_value' => variable_get('reform_name_min', 3),
    '#size' => 2,
    '#maxlength' => 2,
    '#description' => t('Minimum username length allowed.'),
    '#required' => TRUE,
  );
  
  $form['reform_user_form']['reform_name']['reform_name_max'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum length'),
    '#default_value' => variable_get('reform_name_max', 35),
    '#size' => 2,
    '#maxlength' => 2,
    '#description' => t('Maximum username length allowed.'),
    '#required' => TRUE,
  );
  
  $form['reform_user_form']['reform_name']['reform_name_help'] = array(
    '#type' => 'textarea',
    '#title' => t('Help text'),
    '#default_value' => variable_get('reform_name_help'),
    '#description' => t('Instructions to present to the user below the username field. HTML tags allowed.'),
  );
  
  $form['reform_user_form']['reform_mail'] = array(
    '#type' => 'fieldset',
	'#title' => 'E-mail address',
	'#collapsible' => 'TRUE',
    '#collapsed' => 'TRUE',
  );
  
  $form['reform_user_form']['reform_mail']['reform_mail_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#default_value' => variable_get('reform_mail_title', 'E-mail address'),
    '#size' => 60,
    '#maxlength' => 60,
    '#description' => t('The label of the e-mail field.'),
  );
  
  $form['reform_user_form']['reform_mail']['reform_mail_help'] = array(
    '#type' => 'textarea',
    '#title' => t('Help text'),
    '#default_value' => variable_get('reform_mail_help'),
    '#description' => t('Instructions to present to the user below the e-mail field. HTML tags allowed.'),
  );
  
  $form['reform_user_form']['reform_register_button'] = array(
    '#type' => 'textfield',
    '#title' => t('User registration submit button'),
    '#default_value' => variable_get('reform_register_button', 'Create new account'),
    '#size' => 60,
    '#maxlength' => 60,
    '#description' => t('Value of the user registration submit button text.'),
  );
  
  $form['reform_login'] = array(
    '#type' => 'fieldset',
	'#title' => 'User Login',
	'#collapsible' => 'TRUE',
    '#collapsed' => 'TRUE',
  );
  
  $form['reform_login']['reform_login_name'] = array(
    '#type' => 'fieldset',
	'#title' => 'Username',
	'#collapsible' => 'TRUE',
    '#collapsed' => 'TRUE',
  );
  
  $form['reform_login']['reform_login_name']['reform_login_name_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#default_value' => variable_get('reform_login_name_title', 'Username'),
    '#size' => 60,
    '#maxlength' => 60,
    '#description' => t('The label of the username field on login page.'),
  );
  
  $form['reform_login']['reform_login_name']['reform_login_name_help'] = array(
    '#type' => 'textarea',
    '#title' => t('Help text'),
    '#default_value' => variable_get('reform_login_name_help'),
    '#description' => t('Instructions to present to the user below the username field. HTML tags allowed.'),
  );
  
  $form['reform_login']['reform_login_pass'] = array(
    '#type' => 'fieldset',
	'#title' => 'Password',
	'#collapsible' => 'TRUE',
    '#collapsed' => 'TRUE',
  );
  
  $form['reform_login']['reform_login_pass']['reform_login_pass_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#default_value' => variable_get('reform_login_pass_title', 'Password'),
    '#size' => 60,
    '#maxlength' => 60,
    '#description' => t('The label of the password field on login page.'),
  );
  
  $form['reform_login']['reform_login_pass']['reform_login_pass_help'] = array(
    '#type' => 'textarea',
    '#title' => t('Help text'),
    '#default_value' => variable_get('reform_login_pass_help'),
    '#description' => t('Instructions to present to the user below the password field. HTML tags allowed.'),
  );
  
  $form['reform_login']['reform_login_button'] = array(
    '#type' => 'textfield',
    '#title' => t('User login submit button'),
    '#default_value' => variable_get('reform_login_button', 'Log in'),
    '#size' => 60,
    '#maxlength' => 60,
    '#description' => t('Value of the user login submit button text.'),
  );
  
  $form['reform_request_pass'] = array(
    '#type' => 'fieldset',
	'#title' => 'Request password',
	'#collapsible' => 'TRUE',
    '#collapsed' => 'TRUE',
  );
  
  $form['reform_request_pass']['reform_request_pass_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#default_value' => variable_get('reform_request_pass_title', 'Username or e-mail address'),
    '#size' => 60,
    '#maxlength' => 60,
    '#description' => t('The label of the request password field on request password page.'),
  );
  
  $form['reform_request_pass']['reform_request_pass_help'] = array(
    '#type' => 'textarea',
    '#title' => t('Help text'),
    '#default_value' => variable_get('reform_request_pass_help'),
    '#description' => t('Instructions to present to the user below the request password field. HTML tags allowed.'),
  );
  
  $form['reform_request_pass']['reform_request_button'] = array(
    '#type' => 'textfield',
    '#title' => t('Request password submit button'),
    '#default_value' => variable_get('reform_request_button', 'E-mail new password'),
    '#size' => 60,
    '#maxlength' => 60,
    '#description' => t('Value of the request password submit button text.'),
  );

  return system_settings_form($form);
}

/**
* Implements hook_validate().
*/
function reform_admin_settings_validate($form, &$form_state){
  $min_length = $form_state['values']['reform_name_min'];
  $max_length = $form_state['values']['reform_name_max'];
  
  if (!is_numeric($min_length)){
    form_set_error('reform_name_min', t('You must enter a number for the minimum length.'));
  }
  if ($min_length <= 0){
    form_set_error('reform_name_min', t('Minimum length must be positive.'));
  }
  if ($min_length > $max_length){
    form_set_error('reform_name_min', t('Minimum length can not be greater than maximum length.'));
  }
  if (!is_numeric($max_length)){
    form_set_error('reform_name_max', t('You must enter a number for the maximum length.'));
  }
  if ($max_length <= 0){
    form_set_error('reform_name_max', t('Maximum length must be positive.'));
  }
  if ($max_length < $min_length){
    form_set_error('reform_name_max', t('Maximum length can not be smaller than minumum length.'));
  }
  
}