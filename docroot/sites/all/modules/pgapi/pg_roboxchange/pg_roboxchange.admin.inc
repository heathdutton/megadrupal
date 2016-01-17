<?php

/*
 * @file
 * pg_roboxchange.admin.inc
 * Admin interface for ROBOX module.
 */

/**
 * Settings form for roboxchange.
 */
function pg_roboxchange_settings($form, &$form_state) {
  $form['#tree'] = TRUE;
  $form['pg_roboxchange_rate_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Rate url'),
    '#default_value' => variable_get('pg_roboxchange_rate_url', cRoboXchangeRateUrl),
    '#description' => t("Please enter rate url. Default: !url", array('!url' => cRoboXchangeRateUrl)),
  );

  if (!variable_get('pg_roboxchange_mrh', '')) {
    $form['pg_roboxchange_checked'] = array(
      '#type' => 'item',
      '#title' => t('Rate information'),
      '#description' => t('Please fill and save "Merchant Login" first. Use @demo for test.', array('@demo' => 'demo')),
    );
  }
  else {
    // Refresh rates.
    pg_roboxchange_cron();
    drupal_static_reset('pg_roboxchange_get_rates');
    $rates = pg_roboxchange_get_rates(FALSE);
    /*
    $form['currency'] = array(
      '#type' => 'markup',
      '#prefix' => '<strong>' . t('Your output currency') . '</strong>: ',
      '#markup' => variable_get('pg_roboxchange_out_curr', ''),
    );*/
    $last_update = variable_get('pg_roboxchange_last_update', '');
    $form['last_update'] = array(
      '#type' => 'markup',
      '#prefix' => '<strong>' . t('Last update time') . '</strong>:',
      '#markup' => empty($last_update) ? t('Never') : format_date($last_update, 'short'),
    );
    $options = array();
    foreach ($rates as $type => $rate) {
      $options[$type] = array(
        'type' => $type, 'name' => $rate->name, 'rate' => $rate->rate
      );
    }
    $header = array(
      'type' => t('Type'),
      'name' => t('Label'),
      'rate' => t('Rate'),
    );
    $form['pg_roboxchange_checked'] = array(
      '#type'    => 'tableselect',
      '#header'  => $header,
      '#options' => $options,
      '#caption' => t('Please check all currency that you want to accept'),
      '#empty'   => t('No accounts available.'),
      '#multiple' => TRUE,
      '#default_value' => variable_get('pg_roboxchange_checked', array()),
    );
  }

  $form['pg_roboxchange_action_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Action url'),
    '#default_value' => variable_get('pg_roboxchange_action_url', cRoboXchangeActionUrl),
    '#description' => t("Please enter action url.<br/>Default: !url<br/>Test: !url_test", array('!url' => pg_roboxchange_server_url('live'), '!url_test' => pg_roboxchange_server_url('test'))),
  );
  $form['pg_roboxchange_mrh'] = array(
    '#type' => 'textfield',
    '#title' => t('Merchant Login'),
    '#default_value' => variable_get('pg_roboxchange_mrh', ''),
    '#description' => t("Please enter Merchant Login."),
    '#required' => TRUE,
  );
  $form['pg_roboxchange_mps1'] = array(
    '#type' => 'textfield',
    '#title' => t('Merchant Pass 1'),
    '#default_value' => variable_get('pg_roboxchange_mps1', ''),
    '#description' => t("Please enter Merchant Login."),
    '#required' => TRUE,
  );
  $form['pg_roboxchange_mps2'] = array(
    '#type' => 'textfield',
    '#title' => t('Merchant Pass 2'),
    '#default_value' => variable_get('pg_roboxchange_mps2', ''),
    '#description' => t("Please enter Merchant Login."),
    '#required' => TRUE,
  );
  $form['pg_roboxchange_rate'] = array(
    '#type' => 'textfield',
    '#title' => t('Rate to site currency'),
    '#default_value' => variable_get('pg_roboxchange_rate', '1.00'),
    '#description' => t("Please enter RoboXchange rate according to site currency."),
  );
  $form['url'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#title' => t('Urls(result,success,fail) for merchant interfase'),
  );
  $form['url']['result'] = array(
    '#type' => 'textfield',
    '#title' => t('Result url'),
    '#value' => url('roboxchange/done', array('absolute' => TRUE)),
    '#description' => t("Just for copy."),
    '#attributes' => array('readonly' => '1'),
  );
  $form['url']['success'] = array(
    '#type' => 'textfield',
    '#title' => t('Success url'),
    '#value' => url('roboxchange/success', array('absolute' => TRUE)),
    '#description' => t("Just for copy."),
    '#attributes' => array('readonly' => '1'),
  );
  $form['url']['fail'] = array(
    '#type' => 'textfield',
    '#title' => t('Fail url'),
    '#value' => url('roboxchange/fail', array('absolute' => TRUE)),
    '#description' => t("Just for copy."),
    '#attributes' => array('readonly' => '1'),
  );

  $form['#validate'][] = 'pg_roboxchange_settings_validate';
  $form['#submit'][] = 'pg_roboxchange_settings_submit';
  return system_settings_form($form);
}

/**
 * Validate function for roboxchange settings form.
 */
function pg_roboxchange_settings_validate($form, &$form_state) {
  if ($form_state['values']['pg_roboxchange_rate'] <= 0) {
    form_set_error('pg_roboxchange_rate', t('%rate must be more 0.', array('%rate' => $form_state['values']['pg_roboxchange_rate'])));
  }
}

/**
 * Submit function for roboxchange settings form.
 */
function pg_roboxchange_settings_submit($form, &$form_state) {
  // Save filtered rates.
  $form_state['values']['pg_roboxchange_checked'] = array_filter($form_state['values']['pg_roboxchange_checked']);
  unset($form_state['values']['url']);
}
