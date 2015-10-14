<?php
/**
 * @file
 * Admin settings.
 */

/**
 * Callback function for Admin settings for Mails.
 */
function total_subscription_mail_admin_form_settings() {
  $form = array();
  $form['total_subscription_email_address'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('total_subscription_email_address', ''),
    '#description' => t('Please specify the email address'),
    '#title' => t('Email address'),
  );
  $form['total_subscription_subject_published'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('total_subscription_subject_published', ''),
    '#description' => t('Please specify the Subject when some content will published.'),
    '#title' => t('Subject when content is Published.'),
  );
  $form['total_subscription_subject_subscription'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('total_subscription_subject_subscription', ''),
    '#description' => t('Please specify the Subject when someone subscribes.'),
    '#title' => t('Subscription Subject'),
  );
  $total_subscription_shorten_url = "";
  $total_subscription_shorten_url = t('Please check this if you want to shorten url being sent. For username and api key visit') . ' ' . l(t('Bitly Site'), 'https://bitly.com');
  $form['total_subscription_shorten_url'] = array(
    '#type' => 'checkbox',
    '#default_value' => variable_get('total_subscription_shorten_url', 0),
    '#title' => t('Shorten the URL'),
    '#description' => $total_subscription_shorten_url,
    '#states' => array(
      'checked' => array(
        ':input[name="total_subscription_login"]' => array('filled' => TRUE),
        ':input[name="total_subscription_api_key"]' => array('filled' => TRUE),
      ),
    ),
  );
  $form['bit_ly_information'] = array(
    '#type' => 'fieldset',
    '#title' => t('Bitly Information'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#states' => array(
      'expanded' => array(
        ':input[name="total_subscription_shorten_url"]' => array('checked' => TRUE),
      ),
    ),
  );
  $total_subscription_login = "";
  $total_subscription_login = t('Please enter your bitly username from') . ' ' . l(t('here'), 'https://bitly.com/a/your_api_key');
  $form['bit_ly_information']['total_subscription_login'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('total_subscription_login', ''),
    '#title' => t('Enter the bitly Username'),
    '#description' => $total_subscription_login,
  );
  $total_subscription_api_key = "";
  $total_subscription_api_key = t('Please enter your bitly Api Key from') . ' ' . l(t('here'), 'https://bitly.com/a/your_api_key');
  $form['bit_ly_information']['total_subscription_api_key'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('total_subscription_api_key', ''),
    '#title' => t('Enter the bitly Api Key'),
    '#description' => $total_subscription_api_key,
  );
  return system_settings_form($form);
}

/**
 * Callback function for Admin settings for Mails.
 */
function total_subscription_admin_form() {
  $form = array();
  $form['total_subscription_send_start_template'] = array(
    '#type' => 'textarea',
    '#default_value'    => (variable_get('total_subscription_send_start_template')) ? variable_get('total_subscription_send_start_template') : "",
    '#description'      => t('Please use HTML Code for the following, unless you want it as a single line.'),
    '#title'            => t('This is for the template at the start of the body for sending mail when someone subscribes.'),
  );
  $form['body_start_template']['total_subscription_email_template'] = array(
    '#type'             => 'textarea',
    '#rows'             => 2,
    '#default_value'    => (variable_get('total_subscription_email_template')) ? variable_get('total_subscription_email_template') : "",
    '#description'      => t('Please use HTML Code for the following, unless you want it as a single line.'),
    '#title'            => t('This is the template placed at the start of the body when some content is published.'),
  );
  $form['body_start_template']['token_help'] = array(
    '#title' => t('Replacement patterns'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['body_start_template']['token_help']['help'] = array(
    '#theme' => 'token_tree',
    '#token_types' => array('node'),
  );
  $form['total_unsubscription_send_start_template'] = array(
    '#type'             => 'textarea',
    '#default_value'    => (variable_get('total_unsubscription_send_start_template')) ? variable_get('total_unsubscription_send_start_template') : "",
    '#description'      => t('Please use HTML Code for the following, unless you want it as a single line.'),
    '#title'            => t('This is for the template for sending mail at the start of the body when someone wishes to unsubscribe.'),
  );
  return system_settings_form($form);
}
