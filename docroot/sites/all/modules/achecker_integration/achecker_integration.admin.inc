<?php

/**
 * @file
 * Administrative page callbacks for the AChecker integration module.
 */

/**
 * Menu callback for settings form.
 */
function achecker_integration_admin_settings_form($form, $form_state) {

  // Achecker service settings.
  $form['achecker'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Achecker settings'),
    '#collapsible' => FALSE,
  );
  $form['achecker']['achecker_integration_webservice_url'] = array(
    '#type'             => 'textfield',
    '#title'            => t('Webservice url'),
    '#default_value'    => variable_get('achecker_integration_webservice_url', 'http://achecker.ca'),
    '#description'      => t('Url of AChecker REST webservice.'),
    '#element_validate' => array('achecker_integration_webservice_url_validate'),
  );
  $form['achecker']['achecker_integration_webservice_id'] = array(
    '#type'             => 'textfield',
    '#title'            => t('Webservice ID'),
    '#default_value'    => variable_get('achecker_integration_webservice_id', ''),
    '#description'      => t('The "Web Service ID" generated once successfully registering into AChecker. This ID is a 40 characters long string. It can always be retrieved from users AChecker profile page.'),
    '#element_validate' => array('achecker_integration_webservice_id_validate'),
  );

  // Scan settings.
  $form['scan'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Scan settings'),
    '#collapsible' => FALSE,
  );

  // Guidelines to apply.
  $guides = variable_get('achecker_integration_guides', array());
  $form['scan']['achecker_integration_scan_guides'] = array(
    '#type'          => 'checkboxes',
    '#title'         => t('Guidelines to check against'),
    '#default_value' => variable_get('achecker_integration_scan_guides', array()),
    '#options'       => array_map('check_plain', $guides),
    '#description'   => t('Apply each of the following guidelines for accessability.'),
  );

  return system_settings_form($form);
}

/**
 * Validation for field achecker_integration_webservice_id.
 */
function achecker_integration_webservice_id_validate($element, &$form_state) {
  if (strlen($element['#value']) != 40) {
    form_error(
      $element,
      t('@id is not a valid id (must be exactly 40 characters).', array('@id' => $element['#value']))
    );
  }
}

/**
 * Validation for field achecker_integration_webservice_url.
 */
function achecker_integration_webservice_url_validate($element, &$form_state) {
  if (!filter_var($element['#value'], FILTER_VALIDATE_URL)) {
    form_error(
      $element,
      t('@url is not a valid url.', array('@url' => $element['#value']))
    );
  }
}
