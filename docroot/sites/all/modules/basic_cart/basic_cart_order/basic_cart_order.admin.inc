<?php

/**
 * @file
 * Basic cart order admin settings forms.
 */

/**
 * Callback for the admin messages.
 */
function basic_cart_admin_checkout() {
  $form['messages'] = array(
    '#title' => t('Email messages'),
    '#type' => 'fieldset',
    '#description' => t('Here you can customize the mails sent to the site administrator and customer, after an order is placed.'),
  );
  
  $site_mail = variable_get('site_mail');
  $form['messages']['basic_cart_admin_emails'] = array(
    '#title' => t('Administrator emails'),
    '#type' => 'textarea',
    '#description' => t('After each placed order, an email with the order details will be sent to all the addresses from the list above. 
                          Please add one email address per line.'),
    '#default_value' => variable_get('basic_cart_admin_emails', $site_mail),
  );

  $form['messages']['basic_cart_admin_subject'] = array(
    '#title' => t('Subject'),
    '#type' => 'textfield',
    '#description' => t("Subject field for the administrator's email."),
    '#default_value' => variable_get('basic_cart_admin_subject'),
  );

  $form['messages']['basic_cart_admin_message'] = array(
    '#title' => t('Admin email'),
    '#type' => 'textarea',
    '#description' => t('This email will be sent to the site administrator just after an order is placed. 
      Please see all available tokens below. For listing the products, please use: [basic_cart_order:products]'),
    '#default_value' => variable_get('basic_cart_admin_message'),
  );

  $form['messages']['basic_cart_send_user_message'] = array(
    '#title' => t('Send an email to the customer after an order is placed'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('basic_cart_send_user_message'),
  );

  $form['messages']['basic_cart_user_subject'] = array(
    '#title' => t('Subject'),
    '#type' => 'textfield',
    '#description' => t("Subject field for the user's email."),
    '#default_value' => variable_get('basic_cart_user_subject'),
  );

  $form['messages']['basic_cart_user_message'] = array(
    '#title' => t('User email'),
    '#type' => 'textarea',
    '#description' => t('This email will be sent to the user just after an order is placed. 
      Please see all available tokens below. For listing the products, please use: [basic_cart_order:products]'),
    '#default_value' => variable_get('basic_cart_user_message'),
  );

  $form['messages']['tokens'] = array(
    '#theme' => 'token_tree',
    '#token_types' => array('node', 'products'),
    '#global_types' => FALSE,
    '#click_insert' => TRUE,
  );
  
  $form['thank_you'] = array(
    '#title' => t('Thank you page'),
    '#type' => 'fieldset',
    '#description' => t('Thank you page customization.'),
  );

  $form['thank_you']['basic_cart_thank_you_title'] = array(
    '#title' => t('Title'),
    '#type' => 'textfield',
    '#description' => t('Thank you page title.'),
    '#default_value' => variable_get('basic_cart_thank_you_title'),
  );

  $form['thank_you']['basic_cart_thank_you_message'] = array(
    '#title' => t('Text'),
    '#type' => 'textarea',
    '#description' => t('Thank you page text.'),
    '#default_value' => variable_get('basic_cart_thank_you_message'),
  );

  return system_settings_form($form);
}
