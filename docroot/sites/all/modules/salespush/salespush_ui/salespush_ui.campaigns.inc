<?php

/**
 * @file
 * Campaigns pusher UI for SalesPush.
 */

module_load_include('inc', 'salespush', 'salespush.campaigns');

/**
 * Salespush sandbox content form.
 */
function salespush_ui_campaigns_sandbox($form, &$form_state) {
  $form['salespush_campaigns_stats'] = array(
    '#type' => 'textarea',
    '#title' => t('Last job log'),
    '#default_value' => isset($form_state['values']) && isset($form_state['values']['salespush_campaigns_stats']) ? $form_state['values']['salespush_campaigns_stats'] : '',
    '#disabled' => TRUE,
    '#rows' => 5,
  );
  $form['salespush_campaigns_push'] = array(
    '#type' => 'submit',
    '#value' => t('Push campaigns to sandbox test.salesforce.com'),
    '#weight' => -10,
  );

  $form['salespush_campaigns_push_demo'] = array(
    '#type' => 'checkbox',
    '#default_value' => variable_get('salespush_campaigns_push_demo'),
    '#title' => t('Use demo data DemoXXXCampaign, not real campaigns from database.'),
    '#weight' => -11,
  );

  return $form;
}

/**
 * Salespush production content form.
 */
function salespush_ui_campaigns_production($form, &$form_state) {
  $form['salespush_prod_campaigns_stats'] = array(
    '#type' => 'textarea',
    '#title' => t('Last job log'),
    '#default_value' => variable_get('salespush_production_campaigns_lastlogs', t('Data not yet pushed.')),
    '#disabled' => TRUE,
    '#rows' => 25,
  );
  $form['salespush_prod_campaigns_push'] = array(
    '#type' => 'submit',
    '#value' => t('Push campaigns to production salesforce.com'),
    '#weight' => -10,
  );
  $form['salespush_push_campaigns_demo'] = array(
    '#type' => 'checkbox',
    '#default_value' => variable_get('salespush_push_campaigns_demo'),
    '#title' => t('Use demo data campaign DemoXXX, not real campaigns from database.'),
    '#weight' => -11,
  );

  return $form;
}

/**
 * Submit handler for sandbox salesforce.
 */
function salespush_ui_campaigns_sandbox_submit($form, &$form_state) {

  // Setup DemoXXX campaigns content.
  variable_set('salespush_campaigns_push_demo', $form_state['values']['salespush_campaigns_push_demo'] != 0);

  // Salesforce Login information.
  // This wsdl was manually generated using Setup/Develop/API menu in web UI.
  // Generate Enterprise WSDL link.
  $wsdl = salespush_get_wsdl('sandbox');
  $username = salespush_get_username('sandbox');
  $password = salespush_get_password('sandbox');

  if (variable_get('salespush_campaigns_push_demo')) {
    $last_logs = _salespush_campaigns_pusher('sandbox', $wsdl, $username, $password);
  }
  else {
    $last_logs = _salespush_campaigns_pusher('sandbox', $wsdl, $username, $password, FALSE);
  }
  $form_state['values']['salespush_campaigns_stats'] = $last_logs;
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for production salesforce.
 */
function salespush_ui_campaigns_production_submit($form, &$form_state) {

  // Setup DemoXXX campaigns content.
  variable_set('salespush_push_campaigns_demo', $form_state['values']['salespush_push_campaigns_demo'] != 0);

  // Salesforce Login information.
  // This wsdl was manually generated using Setup/Develop/API menu in web UI.
  // Generate Enterprise WSDL link.
  $wsdl = salespush_get_wsdl('production');
  $username = salespush_get_username('production');
  $password = salespush_get_password('production');

  if (variable_get('salespush_push_campaigns_demo')) {
    $last_logs = _salespush_campaigns_pusher('production', $wsdl, $username, $password);
  }
  else {
    $last_logs = _salespush_campaigns_pusher('production', $wsdl, $username, $password, FALSE);
  }

  $form_state['values']['salespush_production_campaigns_lastlogs'] = $last_logs;
  $form_state['rebuild'] = TRUE;
}
