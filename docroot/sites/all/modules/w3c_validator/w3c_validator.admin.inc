<?php

/**
 * @file
 * Admin config page definition.
 */

/**
 * Module settings page.
 *
 * @param string $form_state
 *
 * @return void
 */
function w3c_validator_admin_settings($form_state) {
  $form = array();

  // Check if HTML Tidy library is available,
  // ie if tidy_get_output method exists.
  $tidy_available = function_exists('tidy_get_output');

  // Get the previously saved configuration.
  $method = variable_get('w3c_validator_method', 'w3c_markup_validator');

  // If HTML Tidy method isn't available : force w3c usage.
//   if (!$tidy_available) {
//     $method = 'w3c_markup_validator';
//   }

  // Build the form option for method validation choosing.
  $form['w3c_validator_method'] = array(
    '#type'           => 'radios',
    '#title'          => t('Validation method'),
    '#default_value'  => $method,
    '#options'  => array(
      'w3c_markup_validator' => t('W3C Markup Validator'),
    ),

  );
  
  // Build the form option for method validation choosing.
  $form['w3c_validator_options'] = array(
		'#type' 					=> 'checkboxes',
  	'#title' 					=> t('Validation options'),
  	'#default_value'  => variable_get('w3c_validator_options', array('w3c_on_run' => 1)),
		'#options'  			=> array(
      'w3c_on_run' 			=> t('Auto re-validate when a page is created/updated.'),
	 	)
  );

  // If HTML Tidy is available add the option.
//   if ($tidy_available) {
//     $form['w3c_validator_method']['#options']['tidy'] = t('Tidy library');
//   }

  // W3C validatr settings : give a URI to the W3C checker :
  // NOTICE that online W3C can be used but considered spam. User should
  // better install his own version of W3C validator.
  $form['w3c_markup_validator'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('W3C Markup validator settings'),
    '#description'  =>
      t('This module uses the official W3c Validator scripts. You must install the official w3c validator on your own server and provide the url here or you could use the official w3c validator.') .'<br />'.
      t('For instructions on how to install an instance of the w3c validator scripts you have instructions at <a href="http://validator.w3.org/docs/install.html">http://validator.w3.org/docs/install.html</a>.') .'<br />'.
      t('Debian based distributions comes with a package called w3c_markup_validator which provides a ready to use install of the validator.') .'<br />'.
      t('Using the official w3v validator endpoint url for high volume of validations could be considered abuse of service.'),
    '#collapsible'  => TRUE,
    '#collapsed'    => ($method != 'w3c_markup_validator'),
  );
  $form['w3c_markup_validator']['w3c_validator_api_endpoint_uri'] = array(
    '#type'           => 'textfield',
    '#title'          => t('W3C Validator API endpoint URI'),
    '#description'    => t('URI of the validator script where API call will be targeted. You could use http://validator.w3.org/check althought its not recommended.'),
    '#default_value'  => variable_get('w3c_validator_api_endpoint_uri', 'http://validator.w3.org/check'),
    );

  // Add the HTML Tidy configuration. This let's the user decide if validation
  // is done with anonymous permission or if special authorizaton pages
  // (ie unpublished or restricted page) can be validated too).
//   if ($tidy_available) {
//     $form['tidy'] = array(
//       '#type'         => 'fieldset',
//       '#title'        => t('Tidy Library settings'),
//       '#collapsible'  => TRUE,
//       '#collapsed'    => ($method != 'tidy'),
//     );
//     $form['tidy']['w3c_validator_tidy_authenticated'] = array(
//       '#type' => 'checkbox',
//       '#title' => t('Allow validation of authenticated pages'),
//       '#description' => t('The validation process will have the same access rights as the user running the validation. If not checked all validation is done as anonymous user.'),
//       '#default_value' => variable_get('w3c_validator_tidy_authenticated', FALSE),
//     );
//   }
  return system_settings_form($form);
}