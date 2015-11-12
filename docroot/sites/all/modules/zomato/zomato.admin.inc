<?php

/**
 * @file
 * Everything related to Zomato's Admin.
 */

/**
 * Implements hook_form.
 */
function zomato_config_form() {
  $item = array();

  $item['zomato'] = array(
    '#type' => 'fieldset',
    '#title' => t('Zomato API Configuration'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $item['zomato']['zomato_api_key'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Zomato API Key'),
    '#default_value' => variable_get('zomato_api_key', ''),
  );

  $item['zomato']['zomato_base_url'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Zomato Base URL'),
    '#default_value' => variable_get('zomato_base_url', 'https://api.zomato.com/v1/'),
  );

  return system_settings_form($item);
}
