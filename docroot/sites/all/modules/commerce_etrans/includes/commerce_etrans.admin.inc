<?php

/**
 * @file
 * Admin functions for Commerce etrans.
 */

/**
 * Form builder function for module settings.
 */
function commerce_etrans_settings_form() {
  $form = array();

  $form['credentials'] = array(
    '#type' => 'fieldset',
    '#title' => t('Credentials'),
    '#description' => t('Enter your etrans credentials.'),
  );
  $form['credentials']['commerce_etrans_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Key'),
    '#default_value' => variable_get('commerce_etrans_key', ''),
    '#required' => TRUE,
  );
  $form['credentials']['commerce_etrans_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Secret'),
    '#default_value' => variable_get('commerce_etrans_secret', ''),
    '#required' => TRUE,
  );

  $form['commerce_etrans_pickup_time'] = array(
    '#type' => 'select',
    '#title' => t('Pickup time'),
    '#description' => t('Please select the desired pickup time window for etrans to pickup the packages.'),
    '#empty_value' => '',
    '#empty_option' => t('Please select'),
    '#options' => commerce_etrans_pickup_times(),
    '#default_value' => variable_get('commerce_etrans_pickup_time', ''),
  );

  $form['commerce_etrans_insurance'] = array(
    '#type' => 'select',
    '#title' => t('Insurance'),
    '#description' => t('Please select your insurance type. If you select Etrans the amount will be based on the sum of values declared for each package.'),
    '#empty_value' => '',
    '#empty_option' => t('Please select'),
    '#options' => commerce_etrans_insurance_types(),
    '#default_value' => variable_get('commerce_etrans_insurance', ''),
  );

  return system_settings_form($form);
}
