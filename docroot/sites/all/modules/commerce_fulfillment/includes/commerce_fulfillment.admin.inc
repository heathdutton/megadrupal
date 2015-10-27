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

  if (!isset($form_state['num_boxtypes'])) {
    $form_state['num_boxtypes'] = 1;
  }

  for ($i = 0; $i < $form_state['num_boxtypes']; $i++) {
    $form['commerce_fulfillment_company_information']['commerce_fulfillment_boxtype'][$i] = array(
      '#type' => 'textfield',
      '#title' => t('Add Package Type:'),
    );
  }

  $form['commerce_fulfillment_company_information']['add_boxtype'] = array(
    '#type' => 'submit',
    '#value' => t('Add Another Package Type'),
    '#submit' => array('commerce_fulfillment_add_more_add_one'),
    '#ajax' => array(
      'callback' => 'commerce_fulfillment_add_more_callback',
      'wrapper' => 'boxtype-field-wrapper',
      'method' => 'replace',
    ),
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

  $package_type = variable_get('commerce_fulfillment_package_type', NULL);

  if ($package_type == NULL) {
    $package_type = array();
  }

  // Appends global variable package_type
  for ($i = $form_state['num_boxtypes']; $i > 0; $i--) {

    // Checks that the user has entered something into the textbox.
    if ($form_state['values'][$i - 1] !== '') {
      $package_type[] = $form_state['values'][$i - 1];
    }
  }

  variable_set('commerce_fulfillment_package_type', $package_type);
}

/**
 * Ajax Callback for boxtypes.
 *
 * @param array $form
 *   The form that is being rebuilt.
 * @param array $form_state
 *   Values from the form.
 *
 * @return array mixed
 *
 *   Returns the fieldset for the form.
 */
function commerce_fulfillment_add_more_callback(array $form, array &$form_state) {
  return $form['commerce_fulfillment_company_information'];
}

/**
 * Submit handler for the "add-one-more" button.
 *
 * @param array $form
 *   The form that is being rebuilt.
 * @param array $form_state
 *   Values from the form.
 */
function commerce_fulfillment_add_more_add_one(array $form, array &$form_state) {
  $form_state['num_boxtypes']++;
  $form_state['rebuild'] = TRUE;
}
