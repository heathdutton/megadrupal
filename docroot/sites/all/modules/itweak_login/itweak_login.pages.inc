<?php

/**
 * @file
 * Implementation of itweak_login forms.
 */

function itweak_login_admin_settings_submit($form, &$form_state) {
  module_invoke('menu', 'rebuild');
  drupal_set_message(t('Menus have been rebuilt.'));
}

/**
 * Module settings form.
 */
function _itweak_login_admin_settings() {
  $form = array();
  $form['#submit'] = array('itweak_login_admin_settings_submit');

  $lt_enabled = (module_exists('logintoboggan') && variable_get('logintoboggan_login_with_email', 0));

  $form['itweak_login_username_inpage_prefix'] = array(
    '#type' => 'textarea',
    '#title' => t('Text before "Username" field on login page'),
    '#default_value' => variable_get('itweak_login_username_inpage_prefix', ''),
    '#description' => t('Content to add before the username field such as instructions or account support contact information (Note this does not appear in login block).'),
  );

  $form['itweak_login_username_label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label for "Username" field'),
    '#default_value' => variable_get('itweak_login_username_label', ''),
    '#description' => t('Text to use for the "Username" textfield label. Leave empty to use the default.'),
  );

  $form['itweak_login_password_label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label for "Password" field'),
    '#default_value' => variable_get('itweak_login_password_label', ''),
    '#description' => t('Text to use for the "Password" textfield label. Leave empty to use the default.'),
  );

  // Avoid conflict with LoginToboggan if it is installed and enables email login.
  // Should use theme functions lt_username_title() and lt_password_title() instead.
  if ($lt_enabled) {
    global $base_url;
    $form['itweak_login_username_label']['#disabled'] = TRUE;
    $form['itweak_login_password_label']['#disabled'] = TRUE;

    $form['itweak_login_username_label']['#description'] = t('Disabled, because <a href="!url">LoginToboggan</a> controls that label (Setting "!checkbox" is enabled).',
	  array(
	    '!url' => $base_url . '/admin/config/system/logintoboggan',
	    '!checkbox' => t('Allow users to login using their e-mail address'),
	));
    $form['itweak_login_password_label']['#description'] = t('Disabled, because <a href="!url">LoginToboggan</a> controls that label (Setting "!checkbox" is enabled).',
	  array(
	    '!url' => $base_url . '/admin/config/system/logintoboggan',
	    '!checkbox' => t('Allow users to login using their e-mail address'),
	));
  }

  $form['itweak_login_login_button_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name for "Log in" button'),
    '#default_value' => variable_get('itweak_login_login_button_name', ''),
    '#description' => t('Text to use as "Log in" button label.  Leave empty to use the default.'),
  );

  $form['itweak_login_register'] = array(
    '#type' => 'fieldset',
    '#title' => t('Registration settings'),
    '#description' => t('These options adjust user interface for Registration'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['itweak_login_register']['itweak_login_register_mode'] = array(
    '#type' => 'radios',
    '#title' => t('Registration option'),
    '#default_value' => variable_get('itweak_login_register_mode', 1),
    '#options' => array(
//      0 => t('Disable "Create new account" link'),
      1 => t('Show "Create new account" link (Drupal default)'),
      2 => t('Show "Register" button'),
    ),
    '#description' => t('Select control type (link or button) for new user registration.')
  );

  $form['itweak_login_register']['itweak_login_register_button_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name for "Register" button'),
    '#default_value' => variable_get('itweak_login_register_button_name', ''),
    '#description' => t('(Only if "Registration option" is set to "Show button") Text to use as "Register" button label.  Leave empty to use the default.'),
  );

  $form['itweak_login_register']['itweak_login_register_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Text for "Create new account" link'),
    '#default_value' => variable_get('itweak_login_register_name', ''),
    '#description' => t('(Only if "Registration option" is set to "Show link") Text to use in the "Create new account" link.  Leave empty to use the default.'),
  );

  $form['itweak_login_register']['itweak_login_register_url'] = array(
    '#type' => 'textfield',
    '#title' => t('URL for "Create new account" link or "Register" button'),
    '#default_value' => variable_get('itweak_login_register_url', ''),
    '#description' => t('URL to use in the "Create new account" link or "Register" button.  Leave empty to use the default (user/register).'),
  );

  $form['itweak_login_recover'] = array(
    '#type' => 'fieldset',
    '#title' => t('Password Recovery settings'),
    '#description' => t('These options adjust user interface for Password Recovery'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['itweak_login_recover']['itweak_login_recover_mode'] = array(
    '#type' => 'radios',
    '#title' => t('Pasword recovery option'),
    '#default_value' => variable_get('itweak_login_recover_mode', 1),
    '#options' => array(
      0 => t('Disable "Request new password" link'),
      1 => t('Show "Request new password" link (Drupal default)'),
      2 => t('Show "Reset Password" button'),
    ),
    '#description' => t('Select control type (disabled, link or button) for password recovery.')
  );

  $form['itweak_login_recover']['itweak_login_recover_button_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name for "Reset Password" button'),
    '#default_value' => variable_get('itweak_login_recover_button_name', ''),
    '#description' => t('(Only if "Password recovery option" is set to "Show button") Text to use as "Reset Password" button label.  Leave empty to use the default.'),
  );

  $form['itweak_login_recover']['itweak_login_recover_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Text for "Request new password" link'),
    '#default_value' => variable_get('itweak_login_recover_name', ''),
    '#description' => t('(Only if "Password recovery option" is set to "Show link") Text to use in the "Request new password" link.  Leave empty to use the default.'),
  );

  $form['itweak_login_recover']['itweak_login_recover_url'] = array(
    '#type' => 'textfield',
    '#title' => t('URL for "Request new password" link or "Reset Password" button'),
    '#default_value' => variable_get('itweak_login_recover_url', ''),
    '#description' => t('URL to use in the "Request new password" link or "Reset Password" button.  Leave empty to use the default (user/password).'),
  );

  $form['itweak_login_samples'] = array(
    '#type' => 'fieldset',
    '#title' => t('Examples'),
    '#description' => t('Examples of user login forms (note: buttons will submit this page)'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $subform = drupal_get_form('user_login_block');
  if (is_array($subform)) {
    $form['itweak_login_samples']['user_login_block'] = array(
      '#type' => 'fieldset',
      '#title' => t('User Login Block'),
      '#description' => t('Example of user login block form'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      'form' => array('#type' => 'form'),
    );
    $form['itweak_login_samples']['user_login_block']['form'] += $subform;
    $form['itweak_login_samples']['user_login_block']['form']['#action'] = '';
  }

  global $user;
  $save = $user->uid; $user->uid = FALSE; // Prevent a redirect and tease out the form.
  $subform = drupal_get_form('user_login');
  $user->uid = $save;
  if (is_array($subform)) {
    $form['itweak_login_samples']['user_login'] = array(
      '#type' => 'fieldset',
      '#title' => t('User Login Form'),
      '#description' => t('Example of user login form'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      'form' => array('#type' => 'form'),
    );
    $form['itweak_login_samples']['user_login']['form'] += $subform;
    $form['itweak_login_samples']['user_login']['form']['#action'] = '';
  }

  return $form;
}
function itweak_login_admin_settings() {
  return system_settings_form(_itweak_login_admin_settings());
}

/**
 * After_build callback for login forms.
 */
function itweak_login_form_after_build($form, &$form_state) {
  // FAPI ensures a weight is assigned to all elements before after_build
  // callback is invoked.

  // Get the weight assigned to the submit button.
  $weight = $form['actions']['submit']['#weight'];

  // Give the "Register" button the weight originally assigned
  // to the submit button + tiny bit - that will place it right after the "Log in" button.
  if (isset($form['actions']['register'])) {
    $form['actions']['register']['#weight'] = $weight + 0.0001;
  }
  if (isset($form['actions']['recover'])) {
    $form['actions']['recover']['#weight'] = $weight + 0.0002;
  }
  $form['links']['#weight'] = (isset($form['actions']['#weight']) ? $form['actions']['#weight'] : 0) + 0.0001;

  // Ensure drupal_render() performs the sort by weight step on the form.
  unset($form['#sorted']);

  // Adjust the tabindex of the plain login form.
  if (isset($form['actions']['submit']['#attributes']) && isset($form['actions']['submit']['#attributes']['tabindex'])) {
    $tabindex = (int)$form['actions']['submit']['#attributes']['tabindex'];
    if (isset($form['actions']['register'])) {
      $form['actions']['register']['#attributes']['tabindex'] = $tabindex + 1;
    }
    if (isset($form['actions']['recover'])) {
      $form['actions']['recover']['#attributes']['tabindex'] = $tabindex + 2;
    }
  }

  return $form;
}
