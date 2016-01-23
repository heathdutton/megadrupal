<?php
/**
 * @file
 * Admin forms for Pure360 Mail
 */

/**
 * Implements hook_admin_settings().
 */
function pure360_admin_settings() {
	$form['pure360_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Pure360 Username'),
    '#default_value' => variable_get('pure360_username', ''),
    '#required' => TRUE,
  );
	$form['pure360_password'] = array(
    '#type' => 'textfield',
    '#title' => t('Pure360 Password'),
    '#default_value' => variable_get('pure360_password', ''),
    '#required' => TRUE,
  );
	$form['pure360_notify_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Pure360 Notification Alert Email'),
    '#default_value' => variable_get('pure360_notify_email', ''),
    '#required' => TRUE,
  );
	$form['proxy_host'] = array(
    '#type' => 'textfield',
    '#title' => t('Proxy Host'),
		'#description' => t('Optional: Leave Blank if your system is not in Proxy'),
    '#default_value' => variable_get('proxy_host', ''),
    '#required' => FALSE,
  );
	$form['proxy_port'] = array(
    '#type' => 'textfield',
    '#title' => t('Proxy Port'),
		'#description' => t('Optional: Leave Blank if your system is not in Proxy'),
    '#default_value' => variable_get('proxy_port', ''),
    '#required' => FALSE,
  );
	$form['proxy_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Proxy Username'),
		'#description' => t('Optional: Leave Blank if your system is not in Proxy'),
    '#default_value' => variable_get('proxy_username', ''),
    '#required' => FALSE,
  );
	$form['proxy_password'] = array(
    '#type' => 'textfield',
    '#title' => t('Proxy Password'),
		'#description' => t('Optional: Leave Blank if your system is not in Proxy'),
    '#default_value' => variable_get('proxy_password', ''),
    '#required' => FALSE,
  );
	return system_settings_form($form);
}

function pure360_test_form() {
	$form = array();
	$form['to'] = array(
    '#type' => 'textfield',
    '#title' => t('To'),
		'#description' => t('Please provide valid email.'),
    '#required' => TRUE,
  );
	$form['subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject of Email'),
    '#required' => TRUE,
  );
	$form['body'] = array(
    '#type' => 'textarea',
    '#title' => t('Mail Body'),
    '#required' => FALSE,
  );
	$form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Send test message'),
  );
	return $form;
}

function pure360_test_form_submit($form, &$form_state) {
	global $user;
	$message['to'] = $form_state['values']['to'];
	$message['subject'] = $form_state['values']['subject'];
	$message['body'] = $form_state['values']['body'];
	$message['test'] = 'yes';
	$message['testby'] = $user->name;
	include_once './'. variable_get('smtp_library', '');
	drupal_mail_wrapper($message);
	drupal_set_message("Message Send Successfully.");
}
