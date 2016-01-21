<?php

function super_login_admin_form() {  
  $form['text'] = array(
    '#type' => 'fieldset',
    '#title' => t('Text String Options'),
  );
  $form['text']['super_login_log_in_text'] = array(
    '#type'  => 'textfield',
    '#title' => t('Log In Text'),
    '#default_value' => variable_get('super_login_log_in_text', 'Log In'),
    '#description' => 'Enter the text to be displayed above the username field.',
  );
  $form['text']['super_login_forgot_pw_text'] = array(
    '#type'  => 'textfield',
    '#title' => t('Forgot Password Link Text'),
    '#default_value' => variable_get('super_login_forgot_pw_text', 'Forgot Password?'),
    '#description' => 'Enter the text to be displayed as the forgot password\'s link anchor text.',
  );
  $form['text']['super_login_capslock_msg'] = array(
    '#type'  => 'textfield',
    '#title' => t('Caps Lock Message'),
    '#default_value' => variable_get('super_login_capslock_msg', 'Caps Lock On'),
    '#description' => 'Enter the text to be displayed when a user tries to enter a password with the caps look on.',
  );
  $form['text']['super_login_new_account_text'] = array(
    '#type'  => 'textfield',
    '#title' => t('New Account Text'),
    '#default_value' => variable_get('super_login_new_account_text', 'Create new account'),
    '#description' => 'Enter the text to be displayed for the new account link.',
  );
  $form['text']['super_login_login_title'] = array(
    '#type'  => 'textfield',
    '#title' => t('Login Title'),
    '#default_value' => variable_get('super_login_login_title', 'Username or e-mail address'),
    '#description' => 'Enter the text to be displayed above the username/email login field.',
  );
  $form['text']['super_login_password_reset_title'] = array(
    '#type'  => 'textfield',
    '#title' => t('Password Reset Title'),
    '#default_value' => variable_get('super_login_password_reset_title', 'Password Reset'),
    '#description' => 'Enter the title to be displayed on the password reset page.',
  );
  $form['text']['super_login_back_link'] = array(
    '#type'  => 'textfield',
    '#title' => t('Back to Login Page Text.'),
    '#default_value' => variable_get('super_login_back_link', 'Go back to the login page.'),
    '#description' => 'Enter the text to be displayed for the "back to login page" link.',
  );
  
 $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Configuration Options'),
  );
  $form['options']['super_login_css'] = array(
    '#type'  => 'checkbox',
    '#title' => t('Enable module CSS (stylesheet)'),    
    '#default_value' => variable_get('super_login_css', 1),    
    '#description' => t('Disable this option to turn off this module\'s CSS style sheet. If disabled, you should provide your own styling through your theme\'s stylesheet.'),    
  );
  $form['options']['super_login_button_image'] = array(
    '#type'  => 'checkbox',
    '#title' => t('Enable image replacement for submit buttons'),    
    '#default_value' => variable_get('super_login_button_image', 1),
    '#disabled' => variable_get('super_login_css', 1) ? FALSE : TRUE,    
    '#description' => t('Disable this option to remove the images on the submit buttons.'),    
  );
  $form['options']['super_login_autofill'] = array(
    '#type'  => 'checkbox',
    '#title' => t('Enable autofill features for browsers'),    
    '#default_value' => variable_get('super_login_autofill', 1),    
    '#description' => t('Disable this option to disable the autofill feature used by browsers to will fill in the login form username and password automatically. Note that it appears later versions of Chrome now ignore this setting.'),    
  );
  $form['options']['super_login_placeholder'] = array(
    '#type'  => 'checkbox',
    '#title' => t('Enable "placehold" text within fields.'),    
    '#default_value' => variable_get('super_login_placeholder', 1),    
    '#description' => t('Disable this option to remove the placeholder text within the login and password reset fields.'),    
  );

  
  return system_settings_form($form);

}