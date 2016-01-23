<?php

/**
 * @file
 * Administative functions for Payment Gateway 2CO module
 */

/**
 * Admin settings for 2co payment system
 */
function pg_2co_settings($form, &$form_state) {
  $form = array();
  $form['pg_2co_action_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Action url'),
    '#default_value' => variable_get('pg_2co_action_url', c2coActionUrlSingle),
    '#description' => t("Please enter action url. Default: !url", array('!url' => c2coActionUrlSingle)),
  );
  $form['pg_2co_sid'] = array(
    '#type' => 'textfield',
    '#title' => t('Vendor account number'),
    '#description' => t('Your 2Checkout vendor account number.'),
    '#size' => 16,
    '#default_value' => variable_get('pg_2co_sid', ''),
  );
  $form['pg_2co_secret_word'] = array(
    '#type' => 'textfield',
    '#title' => t('Secret word for order verification'),
    '#description' => t('The secret word entered in your 2Checkout account Look and Feel settings.'),
    '#size' => 16,
    '#default_value' => variable_get('pg_2co_secret_word', 'tango'),
  );
  $form['pg_2co_demo'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable demo mode, allowing you to process fake orders for testing purposes.'),
    '#default_value' => variable_get('pg_2co_demo', TRUE),
  );
  $form['pg_2co_language'] = array(
    '#type' => 'select',
    '#title' => t('Language preference'),
    '#description' => t('Adjust language on 2Checkout pages.'),
    '#options' => array(
      'en' => t('English'),
      'sp' => t('Spanish'),
    ),
    '#default_value' => variable_get('pg_2co_language', 'en'),
  );
  $form['pg_2co_rate'] = array(
    '#type' => 'textfield',
    '#title' => t('Rate to site currency'),
    '#default_value' => variable_get('pg_2co_rate', '1.00'),
    '#description' => t("Please enter 2co rate according to site currency."),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  $form['#validate'][] = 'pg_2co_settings_validate';
  return system_settings_form($form);
}

/**
 * Validate function for 2co settings form
 */
function pg_2co_settings_validate($form, &$form_state) {
  if ($form_state['values']['pg_2co_rate'] <= 0) {
    form_set_error('pg_2co_rate', t('Rate must be more than 0.'));
  }
}
