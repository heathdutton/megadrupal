<?php

/*
 * @file
 * pg_rbkmoney.admin.inc
 * Admin interface for RBK money module
 */

/**
 * RBK money settings form
 */
function pg_rbkmoney_settings($form, &$form_state) {
  $form['pg_rbkmoney_rate'] = array(
    '#type' => 'textfield',
    '#title' => t('Rate to site currency'),
    '#default_value' => variable_get('pg_rbkmoney_rate', '1.00'),
    '#description' => t("Please enter rbkmoney rate according to site currency."),
  );
  $form['pg_rbkmoney_action'] = array(
    '#type' => 'textfield',
    '#title' => t('Action url'),
    '#default_value' => variable_get('pg_rbkmoney_action_url', crbkmoneyPayActionUrl),
    '#description' => t("Please enter action url. Default: !url", array('!url' => crbkmoneyPayActionUrl)),
  );
  $form['pg_rbkmoney_eshopId'] = array(
    '#type' => 'textfield',
    '#title' => t('eshopId'),
    '#default_value' => variable_get('pg_rbkmoney_eshopId', ''),
    '#required' => TRUE,
    '#description' => t("Please enter rbkmoney eshopId."),
  );
  $form['pg_rbkmoney_secretkey'] = array(
    '#type' => 'textfield',
    '#title' => t('Secret Key'),
    '#default_value' => variable_get('pg_rbkmoney_secretkey', ''),
    '#description' => t("Please enter rbkmoney purse secret key."),
  );
  $form['pg_rbkmoney_currency'] = array(
    '#type' => 'select',
    '#title' => t('Currency'),
    '#default_value' => variable_get('pg_rbkmoney_currency', 'RUR'),
    '#options' => array(
      'USD' => 'USD',
      'RUR' => 'RUR',
      'EUR' => 'EUR',
      'UAH' => 'UAH',
    ),
    '#description' => t("Please select rbkmoney currency."),
  );
  return system_settings_form($form);
}
