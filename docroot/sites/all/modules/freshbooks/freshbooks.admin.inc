<?php

/**
 * @file
 *  The logic and form builder for the FreshBook settings page.
 *
 * @author Guy Paddock (guy.paddock@redbottledesign.com)
 * @author Xavier L. (xavier@openconcept.ca)
 */

/**
 * Form builder callback for the FreshBooks settings form.
 *
 * @param $form Array The form array.
 * @param $form_state Array The state of the form.
 * @return Array The $form array.
 */
function freshbooks_settings_form($form, &$form_state) {
  $form['api_access'] = array(
    '#type' => 'fieldset',
    '#title' => t('FreshBooks API access'),
    '#description' => t('Here you will need to specify the Service URL and Authentication token for your FreshBooks account, as described in the !docs.',
      array('!docs' => l('FreshBooks API documentation', 'http://developers.freshbooks.com/'))),
  );

  $form['api_access'][FRESHBOOKS_VAR_DOMAIN] = array(
    '#type' => 'textfield',
    '#title' => t('API Domain'),
    '#default_value' => variable_get(FRESHBOOKS_VAR_DOMAIN, ''),
  );

  $form['api_access'][FRESHBOOKS_VAR_TOKEN] = array(
    '#type' => 'textfield',
    '#title' => t('API Authentication Token'),
    '#default_value' => variable_get(FRESHBOOKS_VAR_TOKEN, ''),
  );

  $form['api_access']['test'] = array(
    '#type' => 'button',
    '#value' => 'Test Settings',
    '#prefix' => '<div id="freshbooks_test_result_wrapper">',
    '#suffix' => '</div>',
    '#ajax' => array(
      'callback' => 'freshbooks_settings_test_callback',
      'wrapper' => 'freshbooks_test_result_wrapper',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );

  return system_settings_form($form);
}

/**
 * Form Validation for the setting form.
 *
 * @param $form
 * @param $form_state
 */
function freshbooks_settings_form_validate($form, &$form_state) {
  form_set_value($form['api_access'][FRESHBOOKS_VAR_DOMAIN], trim($form_state['values'][FRESHBOOKS_VAR_DOMAIN]), $form_state);
  form_set_value($form['api_access'][FRESHBOOKS_VAR_TOKEN], trim($form_state['values'][FRESHBOOKS_VAR_TOKEN]), $form_state);
}

/**
 * The AJAX callback for the "Test Settings" button.
 *
 * This performs a simple test of the API settings that the user has provided,
 * and returns the result as a status message that is rendered in-place.
 * @param $form
 * @param $form_state
 * @return NULL|string
 */
function freshbooks_settings_test_callback($form, &$form_state) {
  $apiDomain = trim($form_state['values'][FRESHBOOKS_VAR_DOMAIN]);
  $apiToken = trim($form_state['values'][FRESHBOOKS_VAR_TOKEN]);
  $return = '';

  if (empty($apiDomain) || empty($apiToken)) {
    $return = t('Please specify both the API Service URL and API Authentication Token.');
  }
  else {
    if (!($loaded = freshbooks_is_loaded())) {
      // Load the FreshBooks API and attempt to initialize it with the data the user provided.
      $loaded = _freshbooks_load($apiDomain, $apiToken);
    }
    else {
      // Re-initialize the FreshBooks HTTP client with the data the user provided.
      \Freshbooks\FreshBooksApi::init($apiDomain, $apiToken);
    }

    if (!$loaded) {
      $return = t('Could not load the FreshBooks-API library. Please ensure that it is !installed.',
        array('!installed' => l('installed properly', 'admin/reports/status')));
    }
    else {
      $fb = new \Freshbooks\FreshBooksApi('client.list');
      $fb->request();
      if ($fb->success()) {
        $return = t('Connected to FreshBooks successfully. The API settings you have provided appear correct; remember to save them!');
      }
      else {
        $return = t('Could not connect to FreshBooks. Please check the API settings you have provided.');
      }
    }
  }

  return $return;
}
