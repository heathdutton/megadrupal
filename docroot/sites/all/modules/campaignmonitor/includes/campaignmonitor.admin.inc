<?php

/**
 * @file
 * Implementation of the administration interface for the module.
 */

/**
 * Creates the Campaign Monitor settings form. This is located under the general
 * tab.
 *
 * @return array $form
 */
function campaignmonitor_admin_settings_general() {
  // Get account details.
  $account = variable_get('campaignmonitor_account', array());

  // Test if the library has been installed. If it has not been installed an
  // error message will be shown.
  $cm = CampaignMonitor::getConnector();
  $library_path = $cm->getLibraryPath();

  $form['campaignmonitor_account'] = array(
    '#type' => 'fieldset',
    '#title' => t('Account details'),
    '#description' => t('Enter your Campaign Monitor account information. See !link for more information.', array('!link' => l(t('the Campaign Monitor API documentation'), 'http://www.campaignmonitor.com/api/required/'))),
    '#collapsible' => empty($account) ? FALSE : TRUE,
    '#collapsed' => empty($account) ? FALSE : TRUE,
    '#tree' => TRUE,
  );

  $form['campaignmonitor_account']['api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('API Key'),
    '#description' => t('Your Campaign Monitor API Key. See <a href="http://www.campaignmonitor.com/api/required/">documentation</a>.'),
    '#default_value' => isset($account['api_key']) ? $account['api_key'] : '',
    '#required' => TRUE,
  );

  $form['campaignmonitor_account']['client_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Client ID'),
    '#description' => t('Your Campaign Monitor Client ID. See <a href="http://www.campaignmonitor.com/api/required/">documentation</a>.'),
    '#default_value' => isset($account['client_id']) ? $account['client_id'] : '',
    '#required' => TRUE,
  );

  if (!empty($account)) {
    $defaults = variable_get('campaignmonitor_general', array());
    $form['campaignmonitor_general'] = array(
      '#type' => 'fieldset',
      '#title' => t('General settings'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#tree' => TRUE,
    );

    $form['campaignmonitor_general']['cache_timeout'] = array(
      '#type' => 'textfield',
      '#title' => t('Cache timeout'),
      '#description' => t('Cache timeout in seconds for stats, subscribers and archive information.'),
      '#size' => 4,
      '#default_value' => isset($defaults['cache_timeout']) ? $defaults['cache_timeout'] : '360',
    );

    $form['campaignmonitor_general']['library_path'] = array(
      '#type' => 'textfield',
      '#title' => t('Library path'),
      '#description' => t('The installation path of the Campaign Monitor library, relative to the Drupal root.'),
      '#default_value' => $library_path ? $library_path : (isset($defaults['library_path']) ? $defaults['library_path'] : ''),
    );

    $form['campaignmonitor_general']['archive'] = array(
      '#type' => 'checkbox',
      '#title' => t('Newsletter archive'),
      '#description' => t('Create a block with links to HTML versions of past campaigns.'),
      '#default_value' => isset($defaults['archive']) ? $defaults['archive'] : 0,
    );

    $form['campaignmonitor_general']['logging'] = array(
      '#type' => 'checkbox',
      '#title' => t('Log errors'),
      '#description' => t('Log communication errors with the Campaign Monitor service, if any.'),
      '#default_value' => isset($defaults['logging']) ? $defaults['logging'] : 0,
    );

    $form['campaignmonitor_general']['instructions'] = array(
      '#type' => 'textfield',
      '#title' => t('Newsletter instructions'),
      '#description' => t('This message will be displayed to the user when subscribing to newsletters.'),
      '#default_value' => isset($defaults['instructions']) ? $defaults['instructions'] : t('Select the newsletters you want to subscribe to.'),
    );

    // Add cache clear button.
    $form['clear_cache'] = array(
      '#type' => 'fieldset',
      '#title' => t('Clear cached data'),
      '#description' => t('The information downloaded from Campaign Monitor is cached to speed up the website. The lists details, custom fields and other data may become outdated if these are changed at Campaign Monitor. Clear the cache to refresh this information.'),
    );

    $form['clear_cache']['clear'] = array(
      '#type' => 'submit',
      '#value' => t('Clear cached data'),
      '#submit' => array('campaignmonitor_clear_cache_submit'),
    );
  }

  $form = system_settings_form($form);
  $form['#submit'][] = 'campaignmonitor_admin_settings_general_submit';
  $form['#validate'][] = 'campaignmonitor_admin_settings_general_validate';

  return $form;
}

/**
 * Form validation handler. Tests if the given API keys are valid.
 */
function campaignmonitor_admin_settings_general_validate($form, &$form_state) {
  $cm = CampaignMonitor::getConnector($form_state['values']['campaignmonitor_account']['api_key'], $form_state['values']['campaignmonitor_account']['client_id']);
  $cm->getLists();
  $error = $cm->getLatestError();
  if ($error['code'] == 401) {
    form_set_error('campaignmonitor_account', $error['message']);
  }
}

/**
 * Form submit handler. Takes care of rehashing the block table to ensure that
 * blocks are available after Campaign Monitor activation.
 */
function campaignmonitor_admin_settings_general_submit($form, &$form_state) {
  // If archive block has been selected, rehash the block cache.
  if ((isset($form_state['values']['campaignmonitor_general']['archive']) && $form_state['values']['campaignmonitor_general']['archive']) ||
      $form['campaignmonitor_account']['api_key']['#default_value'] != $form_state['values']['campaignmonitor_account']['api_key'] ||
      $form['campaignmonitor_account']['client_id']['#default_value'] != $form_state['values']['campaignmonitor_account']['client_id']) {
    _block_rehash();
  }
}

/**
 * Clear the caches used by Campaign Monitor.
 */
function campaignmonitor_clear_cache_submit($form, &$form_state) {
  CampaignMonitor::getConnector($form_state['values']['campaignmonitor_account']['api_key'], $form_state['values']['campaignmonitor_account']['client_id'])->clearCache();
  drupal_set_message(t('Campaign Monitor cache cleared.'), 'status');

  // Rehash blocks, thereby rebuilding the DB block cache.
  _block_rehash();
}