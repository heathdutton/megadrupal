<?php
/**
 * @file
 * Provides the page callbacks for the A12 Connect module
 */

/**
 * This function sets up A12 connection settings against Axistwelve webservices.
 *
 * @todo We want to do a few validations of existing data before we show the
 * form so as to set appropriate status messages when the form is shown for
 * the first time $status_messages variable will have the appropriate status
 * message.
 */
function a12_connect_settings() {
  $form = array();
  $form['details'] = array(
    '#type' => 'fieldset',
    '#title' => t('Connection Details'),
  );
  $form['details']['a12_markup'] = array(
    '#markup' => t('Your connection details can be found by logging into your account at <a href="http://www.axistwelve.com/user" target="_blank">www.axistwelve.com</a> and selecting \'Access Keys\' from the dashboard. If you don\'t yet have an account, click <a href="http://www.axistwelve.com/free-trial" target="_blank">here</a> to start a 30 day Free trial.'),
  );
  $form['details']['a12_identifier'] = array(
    '#type' => 'textfield',
    '#title' => t('Access Key Identifier'),
    '#default_value' => variable_get('a12_identifier', FALSE),
    '#description' => t('The Identifier of the access key you wish to use to connect your site to the Find service.'),
  );
  $form['details']['a12_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Secret Key'),
    '#default_value' => variable_get('a12_key', FALSE),
    '#description' => t('The Secret Key of the access key you wish to use to connect your site to the Find service.'),
  );
  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
    '#validate' => array('a12_connect_credentials_validate'),
    '#submit' => array('a12_connect_credentials_submit'),
  );
  $form['buttons']['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete subscription information'),
    '#submit' => array('a12_connect_delete_submit'),
  );

  return $form;
}
