<?php

/**
 * @file
 * The Exchange Rate API functions used on module administration.
 */

function era_admin_page_content() {
  $build['era_admin_form'] = drupal_get_form('era_admin_form');

  return $build;
}

function era_admin_form($form, &$form_state) {
  $form['era_openexchangerates_app_id'] = array(
    '#title' => t("Open Exchange Rates APP ID"),
    '#type' => 'textfield',
    '#default_value' => variable_get('era_openexchangerates_app_id', ''),
    '#description' => t("Please provide your <strong>Open Exchange Rates</strong> APP ID. You can <a href='http://openexchangerates.org/'>request a free APP ID here</a>."),
    '#required' => TRUE,
  );

  $form['era_cache_expire'] = array(
    '#title' => t("Cache expiration"),
    '#type' => 'select',
    '#options' => drupal_map_assoc(range(0, 24)),
    '#default_value' => variable_get('era_cache_expire', 3),
    '#field_suffix' => ' ' . t("hours"),
    '#description' => t("The information received from <strong>Open Exchange Rates</strong> will be stored on the website's cache for this time. This could help to avoid to contact their API constantly. If you choose <strong>0</strong>, the cache will be disabled and the information will be retrieved directly from <strong>Open Exchange Rates</strong> all the time."),
  );

  $form['era_cache_use_cron'] = array(
    '#title' => t("Update cached information with Drupal's cron"),
    '#type' => 'checkbox',
    '#default_value' => variable_get('era_cache_use_cron', 1),
    '#description' => t("Enable this option to update and store the exchange rates when Drupal's cron is executed (only if the above time has been reached). It doesn't matter if this option is enabled or not, if the cached information is out of date when a user needs it, the cache will be updated."),
  );

  return system_settings_form($form);
}
