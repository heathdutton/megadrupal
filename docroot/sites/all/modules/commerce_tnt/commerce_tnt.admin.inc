<?php

/**
 * @file
 * TNT administration menu items.
 */

/**
 * TNT Quote settings.
 *
 * Record TNT account information neccessary to use the service.
 * Configure which TNT services are quoted to customers.
 *
 * @ingroup forms
 * @see uc_admin_settings_validate()
 */
function commerce_tnt_settings_form() {
  $form = array();

  $form['commerce_tnt_post_url'] = array(
    '#type' => 'textfield',
    '#title' => t('TNT POST URL'),
    '#description' => t('The URL that we post our request to.'),
    '#default_value' => variable_get('commerce_tnt_post_url', ''),
    '#required' => TRUE,
  );

  $form['commerce_tnt_account_number'] = array(
    '#type' => 'textfield',
    '#title' => t('TNT Account number'),
    '#description' => t('This can be found in the TNT Express Price Checker'),
    '#default_value' => variable_get('commerce_tnt_account_number', ''),
    '#required' => TRUE,
  );

  $form['commerce_tnt_user_id'] = array(
    '#type' => 'textfield',
    '#title' => t('TNTExpress.com.au user ID'),
    '#default_value' => variable_get('commerce_tnt_user_id', ''),
    '#required' => TRUE,
  );

  $form['commerce_tnt_password'] = array(
    '#type' => 'password',
    '#title' => t('Password'),
    '#default_value' => variable_get('commerce_tnt_password', ''),
  );

  foreach (_commerce_tnt_service_list() as $key => $service) {
    $array_options[$key] = $service['title'];
  }

  $form['commerce_tnt_services'] = array(
    '#type' => 'checkboxes',
    '#title' => t('TNT Services'),
    '#options' => $array_options,
    '#default_value' => variable_get('commerce_tnt_services', array()),
    '#description' => t('Select the TNT services that are available to customers.'),
  );
  
  $form['commerce_tnt_collection_suburb'] = array(
    '#type' => 'textfield',
    '#title' => t('Collection Suburb'),
    '#default_value' => variable_get('commerce_tnt_collection_suburb', ''),
    '#required' => TRUE,
  );
  
  $form['commerce_tnt_collection_postcode'] = array(
    '#type' => 'textfield',
    '#title' => t('Collection Postcode'),
    '#default_value' => variable_get('commerce_tnt_collection_postcode', ''),
    '#required' => TRUE,
  );
  
  $form['commerce_tnt_collection_state'] = array(
    '#type' => 'textfield',
    '#title' => t('Collection State'),
    '#default_value' => variable_get('commerce_tnt_collection_state', ''),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}


/**
 * Validation handler for commerce_tnt_settings_form.
 *
 * Require password only if it hasn't been set.
 *
 * @see commerce_tnt_settings_form()
 */
function commerce_tnt_settings_form_validate($form, &$form_state) {
  $old_password = variable_get('commerce_tnt_password', '');
  if (!$form_state['values']['commerce_tnt_password']) {
    if ($old_password) {
      form_set_value($form['commerce_tnt_password'], $old_password, $form_state);
    }
    else {
      form_set_error('commerce_tnt_password', t('Password field is required.'));
    }
  }
}
