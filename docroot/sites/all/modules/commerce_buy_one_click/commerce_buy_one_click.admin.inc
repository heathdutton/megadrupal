<?php

/**
 * @file
 * Module admin pages.
 */

/**
 * Module settings form.
 */
function commerce_buy_one_click_settings_form() {
  $form['commerce_buy_one_click_email_template'] = array(
    '#title' => t('Buy one click admin email template'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['commerce_buy_one_click_email_template']['commerce_buy_one_click_send_admin_email'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send email'),
    '#description' => t('Send email to admin when one click order is completed (alternatively you can use rules for sending "one click order completed" emails)'),
    '#default_value' => variable_get('commerce_buy_one_click_send_admin_email', TRUE),
  );
  $form['commerce_buy_one_click_email_template']['commerce_buy_one_click_admin_email_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => variable_get('commerce_buy_one_click_admin_email_title', '[site:name]. "Buy with 1-Click" order [commerce-order:order-number].'),
  );
  $form['commerce_buy_one_click_email_template']['commerce_buy_one_click_admin_email_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#rows' => 7,
    '#default_value' => variable_get('commerce_buy_one_click_admin_email_body', _commerce_buy_one_click_settings_get_default_admin_email_body()),
  );
  $form['commerce_buy_one_click_email_template']['commerce_buy_one_click_admin_email_address'] = array(
    '#type' => 'textfield',
    '#title' => t('Admin email addresses'),
    '#default_value' => variable_get('commerce_buy_one_click_admin_email_address', ''),
    '#maxlength' => 1024,
  );
  
  $form['commerce_buy_one_click_quantity_widget'] = array(
    '#title' => t('How to deal with quantity widget'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['commerce_buy_one_click_quantity_widget']['commerce_buy_one_click_respect_quantity_widget'] = array(
    '#type' => 'checkbox',
    '#title' => t('Respect quantity widget'),
    '#description' => t('Enable this checkbox if you have "Quantity widget" next to "Buy now with 1-Click" button and want to use quantity from this widget with "Buy now with 1-Click" button'),
    '#default_value' => variable_get('commerce_buy_one_click_respect_quantity_widget', FALSE),
  );
  $form['commerce_buy_one_click_quantity_widget']['commerce_buy_one_click_quantity_input_selector'] = array(
    '#type' => 'textfield',
    '#title' => t('Quantity widget input selector'),
    '#description' => t('WARNING: don\'t change this line of code if you are not sure what are you doing!'),
    '#default_value' => variable_get('commerce_buy_one_click_quantity_input_selector', "input[name='quantity']"),
  );
  
  $form['commerce_buy_one_click_button_show'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show "Buy now with 1-Click" button on commerce product page'),
    '#default_value' => variable_get('commerce_buy_one_click_button_show', TRUE),
  );
  $form['commerce_buy_one_click_name_show'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show name field on popup'),
    '#default_value' => variable_get('commerce_buy_one_click_name_show', TRUE),
  );
  $form['commerce_buy_one_click_email_show'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show email field on popup'),
    '#default_value' => variable_get('commerce_buy_one_click_email_show', TRUE),
  );
  $form['commerce_buy_one_click_email_required'] = array(
    '#type' => 'checkbox',
    '#title' => t('Email field is required'),
    '#default_value' => variable_get('commerce_buy_one_click_email_required', TRUE),
  );
  $form['commerce_buy_one_click_respect_stock_module'] = array(
    '#type' => 'checkbox',
    '#title' => t('Respect "Commerce Stock" module'),
    '#description' => t('Not allow user to make 1-click order if product is out of stock'),
    '#default_value' => variable_get('commerce_buy_one_click_respect_stock_module', TRUE),
  );
  $form['commerce_buy_one_click_default_empty_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email for orders without email'),
    '#default_value' => variable_get('commerce_buy_one_click_default_empty_email', "example@example.com"),
  );
  return system_settings_form($form);
}
