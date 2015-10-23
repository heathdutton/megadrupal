<?php

/**
 * @file
 * Push users to SalesForce UI.
 */

module_load_include('inc', 'salespush', 'salespush.users');

/**
 * Salespush sandbox content form.
 */
function salespush_ui_users_sandbox($form, &$form_state) {
  $form['salespush_stats'] = array(
    '#type' => 'textarea',
    '#title' => t('Last job log'),
    '#default_value' => variable_get('salespush_sandbox_lastlogs', t('Data not yet pushed.')),
    '#disabled' => TRUE,
    '#rows' => 25,
  );
  $form['salespush_push'] = array(
    '#type' => 'submit',
    '#value' => t('Push users to sandbox test.salesforce.com'),
    '#weight' => -10,
  );

  $form['salespush_count'] = array(
    '#markup' => '<p>Total sandbox pushed users count: ' . variable_get(
        'salespush_total_pushed_contacts_sandbox',
        0) . '.</p>',
    '#weight' => -10,
  );

  $form['salespush_push_demo'] = array(
    '#type' => 'checkbox',
    '#default_value' => variable_get('salespush_push_demo'),
    '#title' => t('Use demo data user DemoXXX, not real users database.'),
    '#weight' => -11,
  );

  return $form;
}

/**
 * Salespush production content form.
 */
function salespush_ui_users_production($form, &$form_state) {
  $form['salespush_prod_stats'] = array(
    '#type' => 'textarea',
    '#title' => t('Last job log'),
    '#default_value' => variable_get('salespush_production_lastlogs', t('Data not yet pushed.')),
    '#disabled' => TRUE,
    '#rows' => 25,
  );
  $form['salespush_prod_push'] = array(
    '#type' => 'submit',
    '#value' => t('Push users to production salesforce.com'),
    '#weight' => -10,
  );
  $form['salespush_count'] = array(
    '#markup' => '<p>Total production pushed users count: ' . variable_get(
        'salespush_total_pushed_contacts_production',
        0) . '</p>',
    '#weight' => -10,
  );
  $form['salespush_push_demo'] = array(
    '#type' => 'checkbox',
    '#default_value' => variable_get('salespush_push_demo'),
    '#title' => t('Use demo data user DemoXXX, not real users database.'),
    '#weight' => -11,
  );

  return $form;
}

/**
 * Submit handler for sandbox salesforce.
 */
function salespush_ui_users_sandbox_submit($form, &$form_state) {

  // Setup DemoXXX user content.
  variable_set('salespush_push_demo', $form_state['values']['salespush_push_demo'] != 0);

  // Salesforce Login information.
  // This wsdl was manually generated using Setup/Develop/API menu in web UI.
  // Generate Enterprise WSDL link.
  $wsdl = salespush_get_wsdl('sandbox');
  $username = salespush_get_username('sandbox');
  $password = salespush_get_password('sandbox');

  if (variable_get('salespush_push_demo')) {
    $last_logs = _salespush_user_pusher('sandbox', $wsdl, $username, $password);
  }
  else {
    $last_logs = _salespush_user_pusher('sandbox', $wsdl, $username, $password, FALSE);
  }

  $form_state['values']['salespush_sandbox_lastlogs'] = $last_logs;
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for production salesforce.
 */
function salespush_ui_users_production_submit($form, &$form_state) {

  // Setup DemoXXX user content.
  variable_set('salespush_push_demo', $form_state['values']['salespush_push_demo'] != 0);

  // Salesforce Login information.
  // This wsdl was manually generated using Setup/Develop/API menu in web UI.
  // Generate Enterprise WSDL link.
  $wsdl = salespush_get_wsdl('production');
  $username = salespush_get_username('production');
  $password = salespush_get_password('production');

  if (variable_get('salespush_push_demo')) {
    $last_logs = _salespush_user_pusher('production', $wsdl, $username, $password);
  }
  else {
    $last_logs = _salespush_user_pusher('production', $wsdl, $username, $password, FALSE);
  }

  $form_state['values']['salespush_production_lastlogs'] = $last_logs;
  $form_state['rebuild'] = TRUE;
}

/**
 * Implements standard Drupal form for getting user's IDs from production SalesForce.
 */
function salespush_ui_users_sandbox_id_sync($form, &$form_state) {
  $form['salespush_sandboxidsync_stats'] = array(
    '#type' => 'textarea',
    '#title' => t('Last job log'),
    '#default_value' => isset($form_state['values']) && isset($form_state['values']['salespush_sandboxidsync_stats']) ? $form_state['values']['salespush_sandboxidsync_stats'] : '',
    '#disabled' => TRUE,
    '#rows' => 5,
  );
  $form['salespush_sandboxidsync_push'] = array(
    '#type' => 'submit',
    '#value' => t('Sync user IDs from sandbox test.salesforce.com locally'),
    '#weight' => -10,
  );

  return $form;
}

/**
 * Implemets submit handler for 'salespush_ui_users_sandbox_id_sync' Drupal form.
 */
function salespush_ui_users_sandbox_id_sync_submit($form, &$form_state) {
  $environment = 'sandbox';
  // Salesforce Login information.
  // This wsdl was manually generated using Setup/Develop/API menu in web UI.
  // Generate Enterprise WSDL link.
  $wsdl = salespush_get_wsdl($environment);
  $username = salespush_get_username($environment);
  $password = salespush_get_password($environment);

  $client_data = salespush_get_client($username, $password, $wsdl);
  $client = $client_data['client'];

  // Get all users from salesforce to mapper.
  $timer_start = microtime(TRUE);
  // Runs ~1 sec for 200 records.
  salespush_mapper_get_sf_user_ids($client, $environment);
  $timer_end = microtime(TRUE);
  $exec_time = round($timer_end - $timer_start, 2);
  $last_logs = 'salespush_ui_users_sandbox_id_sync_submit run: ' . $exec_time . 'sec';
  watchdog('salespush_ui', $last_logs);

  $form_state['values']['salespush_sandboxidsync_stats'] = $last_logs;
  $form_state['rebuild'] = TRUE;
}

/**
 * Implements standard Drupal form for getting user's IDs from production SalesForce.
 */
function salespush_ui_users_prod_id_sync($form, &$form_state) {
  $form['salespush_prodidsync_stats'] = array(
    '#type' => 'textarea',
    '#title' => t('Last job log'),
    '#default_value' => isset($form_state['values']) && isset($form_state['values']['salespush_prodidsync_stats']) ? $form_state['values']['salespush_prodidsync_stats'] : '',
    '#disabled' => TRUE,
    '#rows' => 5,
  );
  $form['salespush_prodidsync_push'] = array(
    '#type' => 'submit',
    '#value' => t('Sync user IDs from production salesforce.com locally'),
    '#weight' => -10,
  );

  return $form;
}

/**
 * Implemets submit handler for 'salespush_ui_users_prod_id_sync' Drupal form.
 */
function salespush_ui_users_prod_id_sync_submit($form, &$form_state) {
  $environment = 'production';
  // Salesforce Login information.
  // This wsdl was manually generated using Setup/Develop/API menu in web UI.
  // Generate Enterprise WSDL link.
  $wsdl = salespush_get_wsdl($environment);
  $username = salespush_get_username($environment);
  $password = salespush_get_password($environment);

  $client_data = salespush_get_client($username, $password, $wsdl);
  $client = $client_data['client'];

  // Get all users from salesforce to mapper.
  $timer_start = microtime(TRUE);
  // Runs ~1 sec for 200 records.
  salespush_mapper_get_sf_user_ids($client, $environment);
  $timer_end = microtime(TRUE);
  $exec_time = round($timer_end - $timer_start, 2);
  $last_logs = 'salespush_ui_users_prod_id_sync_submit run: ' . $exec_time . 'sec';
  watchdog('salespush_ui', $last_logs);

  $form_state['values']['salespush_prodidsync_stats'] = $last_logs;
  $form_state['rebuild'] = TRUE;
}
