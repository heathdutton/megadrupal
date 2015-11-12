<?php

/**
 * @file
 * Handle submissions with UI for salespush module.
 */

module_load_include('inc', 'salespush', 'salespush.submissions');

/**
 * Salespush sandbox content form.
 */
function salespush_ui_submissions_sandbox($form, &$form_state) {
  $form['salespush_submissions_sandbox_stats'] = array(
    '#type' => 'textarea',
    '#title' => t('Last job log'),
    '#default_value' => variable_get('salespush_submissions_sandbox_lastlogs', t('Data not yet pushed.')),
    '#disabled' => TRUE,
    '#rows' => 25,
  );
  $form['salespush_submissions_sandbox_push'] = array(
    '#type' => 'submit',
    '#value' => t('Push submissions to sandbox test.salesforce.com'),
    '#weight' => -10,
  );

  $form['salespush_submissions_sandbox_push_demo'] = array(
    '#type' => 'checkbox',
    '#default_value' => variable_get('salespush_submissions_sandbox_push_demo'),
    '#title' => t('Use demo data DemoXXX user, not real submissions from database.'),
    '#weight' => -11,
  );

  return $form;
}

/**
 * Salespush production submissions form.
 */
function salespush_ui_submissions_production($form, &$form_state) {
  $form['salespush_prod_submissions_stats'] = array(
    '#type' => 'textarea',
    '#title' => t('Last job log'),
    '#default_value' => variable_get('salespush_production_submissions_lastlogs', t('Data not yet pushed.')),
    '#disabled' => TRUE,
    '#rows' => 25,
  );
  $form['salespush_prod_submissions_push'] = array(
    '#type' => 'submit',
    '#value' => t('Push submissions to production salesforce.com'),
    '#weight' => -10,
  );
  $form['salespush_push_submissions_demo'] = array(
    '#type' => 'checkbox',
    '#default_value' => variable_get('salespush_push_submissions_demo'),
    '#title' => t('Use demo data user DemoXXX, not real users database.'),
    '#weight' => -11,
  );

  return $form;
}

/**
 * Submit handler for sandbox salesforce.
 */
function salespush_ui_submissions_sandbox_submit($form, &$form_state) {

  // Setup DemoXXX submissions content.
  variable_set('salespush_submissions_sandbox_push_demo', $form_state['values']['salespush_submissions_sandbox_push_demo'] != 0);

  // Salesforce Login information.
  // This wsdl was manually generated using Setup/Develop/API menu in web UI.
  // Generate Enterprise WSDL link.
  $wsdl = salespush_get_wsdl('sandbox');
  $username = salespush_get_username('sandbox');
  $password = salespush_get_password('sandbox');

  if (variable_get('salespush_submissions_sandbox_push_demo')) {
    $last_logs = _salespush_submissions_pusher('sandbox', $wsdl, $username, $password);
  }
  else {
    $last_logs = _salespush_submissions_pusher('sandbox', $wsdl, $username, $password, FALSE);
  }

  $form_state['values']['salespush_submissions_sandbox_lastlogs'] = $last_logs;
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for production salesforce.
 */
function salespush_ui_submissions_production_submit($form, &$form_state) {

  // Setup DemoXXX user content.
  variable_set('salespush_push_submissions_demo', $form_state['values']['salespush_push_submissions_demo'] != 0);

  // Salesforce Login information.
  // This wsdl was manually generated using Setup/Develop/API menu in web UI.
  // Generate Enterprise WSDL link.
  $wsdl = salespush_get_wsdl('production');
  $username = salespush_get_username('production');
  $password = salespush_get_password('production');

  if (variable_get('salespush_push_submissions_demo')) {
    $last_logs = _salespush_submissions_pusher('production', $wsdl, $username, $password);
  }
  else {
    $last_logs = _salespush_submissions_pusher('production', $wsdl, $username, $password, FALSE);
  }

  $form_state['values']['salespush_production_submissions_lastlogs'] = $last_logs;
  $form_state['rebuild'] = TRUE;
}
