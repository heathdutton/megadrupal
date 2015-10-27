<?php

/**
 * Admin settings form.
 */
function qbo_api_admin_settings($form, &$form_state) {
  module_load_install('qbo_api');

  // Requirements
  $form['requirements'] = array(
    '#type' => 'fieldset',
    '#title' => t('Requirements'),
  );
  $form['requirements']['report'] = array(
    '#markup' => theme('status_report', array('requirements' => qbo_api_requirements('runtime'))),
  );

  // Connection
  $form['connection'] = array(
    '#type' => 'fieldset',
    '#title' => t('Connection'),
  );
  $form['connection']['help'] = array(
    '#type' => 'fieldset',
    '#title' => t('Help'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['connection']['help']['list'] = array(
    '#theme' => 'item_list',
    '#type' => 'ol',
    '#items' => array(
      t('Log in to your Quickbooks Online.'),
      t('Click your company name at the top-right then select "Your account".'),
      t('Copy the "Company ID" at the top in to the connection settings here.'),
      t('Go to https://developer.intuit.com and either create a developer account or sign-in. If you are creating a new account, it must be through the account you use to log in to Quickbooks.'),
      t('On the top-menu, click "My apps" then "Create new app" and choose "Quickbooks API". The name of the app and the URLs do not matter.'),
      t('After the app is created, copy the "App Token" under the "Development" tab.'),
      t('Go to https://appcenter.intuit.com/Playground/OAuth.'),
      t('Enter the "App token" that you\'ve copied in the first field and press "Get Key and Secret using App Token". The two textfields below will populate. Copy those values in to the connection settings.'),
      t('Click "Get Request Token using Key and Secret " and wait for the next two textfields to populate.'),
      t('Click "Authorize Request Token". It should ask you to login and/or grant this app access to your Quickbooks.'),
      t('Copy the last two textfields in to the connection settings.'),
    ),
  );
  $form['connection']['qbo_api_company_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Company ID'),
    '#required' => TRUE,
    '#default_value' => variable_get('qbo_api_company_id', ''),
  );
  $form['connection']['qbo_api_consumer_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Consumer key'),
    '#required' => TRUE,
    '#default_value' => variable_get('qbo_api_consumer_key', ''),
  );
  $form['connection']['qbo_api_consumer_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Consumer secret'),
    '#required' => TRUE,
    '#default_value' => variable_get('qbo_api_consumer_secret', ''),
  );
  $form['connection']['qbo_api_access_token'] = array(
    '#type' => 'textfield',
    '#title' => t('Access token'),
    '#required' => TRUE,
    '#default_value' => variable_get('qbo_api_access_token', ''),
  );
  $form['connection']['qbo_api_access_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Access token secret'),
    '#required' => TRUE,
    '#default_value' => variable_get('qbo_api_access_secret', ''),
  );

  return system_settings_form($form);
}
