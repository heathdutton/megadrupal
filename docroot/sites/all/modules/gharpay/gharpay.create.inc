<?php

/**
 * @file
 * Create order using Gharpay.
 */

/**
 * Create order form.
 */
function gharpay_create_order_form($form, &$form_state) {

  $form['gharpay_create_order_customer_details'] = array(
    '#type' => 'fieldset',
    '#title' => t("Customer details"),
    );
  $form['gharpay_create_order_customer_details']['contact'] = array(
    '#type' => 'textfield',
    '#title' => t("Contact"),
    '#required' => TRUE,
    '#description' => t("Your contact number."),
    );
  $form['gharpay_create_order_customer_details']['prefix'] = array(
    '#type' => 'select',
    '#title' => t("Prefix"),
    '#options' => array(
        'mr' => t("Mr."),
        'ms' => t("Ms."),
        'mrs' => t("Mrs."),
        'dr' => t("Dr."),
        'prof' => t("Prof."),
      ),
    );
  $form['gharpay_create_order_customer_details']['firstname'] = array(
    '#type' => 'textfield',
    '#title' => t("First name"),
    '#required' => TRUE,
    );
  $form['gharpay_create_order_customer_details']['lastname'] = array(
    '#type' => 'textfield',
    '#title' => t("Last name"),
    );
  $form['gharpay_create_order_customer_details']['address'] = array(
    '#type' => 'textarea',
    '#title' => t("Address"),
    '#required' => TRUE,
    );
  $form['gharpay_create_order_order_details'] = array(
    '#type' => 'fieldset',
    '#title' => t("Order details"),
    );

  return $form;
}
