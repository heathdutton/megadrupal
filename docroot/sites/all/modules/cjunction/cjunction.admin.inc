<?php

/**
 * @file
 * Admin part for the cjunction module.
 */

/**
 * Main cjunction's settings form.
 */
function cjunction_settings_form($form, &$form_state) {

  $form['associate_setting'] = array(
    '#type' => 'fieldset',
    '#title' => t('commission junction associate settings'),
    '#description' => t(''),
  );

  $form['associate_setting']['cjunction_website_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Website ID'),
    '#description' => t('To create a new website ID to go Account -> Website Settings -> Add a New Web Site.'),
    '#default_value' => variable_get('cjunction_website_id', ''),
    '#required' => TRUE,
  );

  $form['associate_setting']['cjunction_developer_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Developer Key'),
    '#description' => t('Register for a <a href="!more_info">contact page</a>. (Note: Registration requires an existing account in the Commission Junction network.)', array('!more_info' => url('https://api.cj.com/sign_up.cj', array('html' => TRUE)))),
    '#default_value' => variable_get('cjunction_developer_key', ''),
    '#maxlength' => 300,
    '#required' => TRUE,
  );

  $form['associate_setting']['cjunction_advertiser_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Advertiser ID'),
    '#description' => t('You will find the advertiser id by accessing the merchant page trough the Get Links menu in your cj.com account. In the URL adress of the merchant page the last thing you will see is the advertiser id. You can enter up to 2 IDs, separated by commas and no spaces or simply enter "joined" (without quotes), to egt products from the merchants you\'ve been approved by.'),
    '#default_value' => variable_get('cjunction_advertiser_id', ""),
  );

  $period = drupal_map_assoc(array(3600, 7200, 14400, 21600, 43200, 86400), 'format_interval');
  $form['cjunction_refresh_schedule'] = array(
    '#type' => 'select',
    '#title' => t('CJ refresh schedule'),
    '#description' => t('Cached information must be refreshed regularly to keep pricing and stock information up to date. Cron must be enabled for this function to work properly.'),
    '#default_value' => variable_get('cjunction_refresh_schedule', 86400),
    '#options' => $period
  );

  return system_settings_form($form);
}


