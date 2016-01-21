<?php

/**
 * @file
 * This file is for anything related to the admin page.
 */

/**
 * Implements hook_form().
 */
function commerce_fulfillment_admin_form($form, &$form_state) {

  $form = array();

  $form['commerce_fulfillment_title'] = array(
    '#markup' => '<h2>' . t('Admin Page') . '</h2>',
  );

  // Creates separate information fields.
  $form['commerce_fulfillment_company_information'] = array(
    '#type' => 'fieldset',
    '#title' => t("Enter your company's information below"),
    '#description' => t('Enter information to use on Packing Slips and Shipping Labels.'),
    '#prefix' => '<div id="boxtype-field-wrapper">',
    '#suffix' => '</div>',
  );

  $form['commerce_fulfillment_company_information']['commerce_fulfillment_company'] = array(
    '#type' => 'textfield',
    '#title' => t('Company name:'),
    '#default_value' => variable_get('commerce_fulfillment_company', ''),
    '#required' => TRUE,
  );
  $form['commerce_fulfillment_company_information']['commerce_fulfillment_address'] = array(
    '#type' => 'textfield',
    '#title' => t('Address:'),
    '#default_value' => variable_get('commerce_fulfillment_address', ''),
    '#required' => TRUE,
  );
  $form['commerce_fulfillment_company_information']['commerce_fulfillment_phone'] = array(
    '#type' => 'textfield',
    '#title' => t('Company Phone Number:'),
    '#default_value' => variable_get('commerce_fulfillment_phone', ''),
    '#required' => TRUE,
  );

  $form['#attributes']['enctype'] = 'multipart/form-data';

  $form['commerce_fulfillment_company_information']['commerce_fulfillment_logo'] = array(
    '#type' => 'managed_file',
    '#title' => t('Upload your company logo:'),
    '#default_value' => variable_get('commerce_fulfillment_logo', NULL),
    '#upload_location' => 'public://logo/',
  );

  $form = system_settings_form($form);
  $form['#submit'][] = 'commerce_fulfillment_admin_form_submit';

  return $form;
}

/**
 * Implements hook_form_submit().
 *
 * Does not override system_settings_form submit.
 * Just handles submission of the package_type variable.
 */
function commerce_fulfillment_admin_form_submit($form, &$form_state) {

  // Make logo upload permanent.
  $file = file_load($form_state['values']['commerce_fulfillment_logo']);
  $file->status = FILE_STATUS_PERMANENT;
  file_save($file);
  file_usage_add($file, 'commerce_fulfillment', 'commerce_fulfillment_logo', 1);
}
