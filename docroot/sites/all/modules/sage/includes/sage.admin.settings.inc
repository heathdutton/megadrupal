<?php

/**
 * @file
 * The form to build the SAGE settings on Drupal admin page.
 */

// Include inc files.
module_load_include('inc', 'sage', 'includes/sage.functions');

/**
 * The admin settings form for SAGE.
 *
 * @param array $form
 *   Nested array of form elements that comprise the form.
 * @param array &$form_state
 *   A keyed array containing the current state of the form.
 */
function sage_admin_settings(array $form, array &$form_state) {
  // SAGE API authentication.
  $form['sage_api_auth'] = array(
    '#type' => 'fieldset',
    '#title' => t('SAGE API authentication.'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['sage_api_auth']['sage_api_auth_url'] = array(
    '#type' => 'textfield',
    '#title' => t('SAGE API url'),
    '#default_value' => variable_get('sage_api_auth_url',
      "https://www.promoplace.com/ws/ws.dll/XMLDataStream"),
    '#size' => 100,
    '#maxlength' => 128,
    '#required' => TRUE,
    '#element_validate' => array('sage_valid_external_url'),
    '#description' => t("The SAGE API Datastream url."),
  );
  $form['sage_api_auth']['sage_api_auth_acctid'] = array(
    '#type' => 'textfield',
    '#title' => t('SAGE user AcctID'),
    '#default_value' => variable_get('sage_api_auth_acctid', ""),
    '#size' => 10,
    '#maxlength' => 10,
    '#required' => TRUE,
    '#element_validate' => array('element_validate_integer_positive'),
    '#description' => t("Account ID (AcctID) for SAGE API Datastream user."),
  );
  $form['sage_api_auth']['sage_api_auth_loginid'] = array(
    '#type' => 'textfield',
    '#title' => t('SAGE user LoginID'),
    '#default_value' => variable_get('sage_api_auth_loginid', ""),
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
    '#description' => t("Login ID (LoginID) for SAGE API Datastream user."),
  );
  $form['sage_api_auth']['sage_api_auth_pass'] = array(
    '#type' => 'password',
    '#title' => t('SAGE user Password'),
    '#default_value' => variable_get('sage_api_auth_pass', ""),
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
    '#description' => t("Password for SAGE API Datastream user."),
  );
  $form['sage_api_debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable debugging'),
    '#default_value' => variable_get('sage_api_debug', "0"),
    '#description' => t("Display system and watchdog messages for SAGE
      API responses with error."),
  );

  // SAGE API display settings.
  $form['sage_api_display'] = array(
    '#type' => 'fieldset',
    '#title' => t('Display settings.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['sage_api_display']['sage_api_display_maxrecs'] = array(
    '#type' => 'textfield',
    '#title' => t('Items per page'),
    '#default_value' => variable_get('sage_api_display_maxrecs', 15),
    '#size' => 4,
    '#maxlength' => 4,
    '#element_validate' => array('element_validate_integer_positive'),
    '#description' => t("Default items per page for product lists."),
  );
  $form['sage_api_display']['sage_api_display_maxtotalitems'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum products to display'),
    '#default_value' => variable_get('sage_api_display_maxtotalitems', 1000),
    '#size' => 4,
    '#maxlength' => 4,
    '#element_validate' => array('element_validate_integer_positive'),
    '#description' => t("Maximum items to display for product lists."),
  );
  $form['sage_api_display']['sage_api_display_sorting'] = array(
    '#type' => 'select',
    '#title' => t('Default sorting'),
    '#default_value' => variable_get('sage_api_display_sorting', 'DEFAULT'),
    '#options' => sage_sort_options(),
    '#description' => t("Default sorting for product lists."),
  );

  // SAGE API Search settings.
  $form['sage_api_search'] = array(
    '#type' => 'fieldset',
    '#title' => t('Search settings.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['sage_api_search']['sage_api_search_help'] = array(
    '#type' => 'textarea',
    '#title' => t('Search page help texts'),
    '#default_value' => variable_get('sage_api_search_help', ""),
    '#description' => t("The help texts that will display on the Search page."),
  );

  // Submit additional handler.
  $form['#submit'][] = 'sage_admin_settings_submit';

  return system_settings_form($form);
}

/**
 * Submit callback for Admin  Settings.
 *
 * @param array $form
 *   Nested array of form elements that comprise the form.
 * @param array &$form_state
 *   A keyed array containing the current state of the form.
 */
function sage_admin_settings_submit(array $form, array &$form_state) {

  // Make a simple post request for CategoryList to check authentication.
  $response = sage_post_datastream_categorylist();

  if (empty($response->error)) {
    drupal_set_message(
      t("SAGE authentication was enabled successfully."), 'status'
    );
  }
  else {
    drupal_set_message(t("SAGE authentication failed."), 'error');
  }
}

/**
 * Return a valid absolute url.
 *
 * @param string $url
 *   A url.
 * @param bool $absolute
 *   True if url is absolute.
 *
 * @return bool
 *   Returns TRUE if the absolute url if is valid.
 */
function sage_valid_external_url($url, $absolute = TRUE) {
  if ($url != "") {
    return valid_url($url, TRUE);
  } else {
    return FALSE;
  }
}
