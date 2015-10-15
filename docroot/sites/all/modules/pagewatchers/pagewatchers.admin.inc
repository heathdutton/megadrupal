<?php

function _pagewatchers_settings_form() {
  $form = array();

  $form['pagewatchers_for_anonymous'] = array(
    '#type' => 'checkbox',
    '#title' => t('Count anonymous users'),
    '#default_value' => variable_get('pagewatchers_for_anonymous', 0),
  );

  $form['pagewatchers_cache_duration'] = array(
    '#type' => 'textfield',
    '#title' => t('Cache Lifetime'),
    '#description' => t('Specify the duration in <em>seconds</em> before cache expires. Set to 0 to disable caching.'),
    '#default_value' => variable_get('pagewatchers_cache_duration', 0),
  );

  $form['pagewatchers_ajax_update_interval'] = array(
    '#type' => 'textfield',
    '#title' => t('AJAX Update Intervals'),
    '#description' => t('Specify the interval length in <em>seconds</em> between AJAX updates of viewers number.'),
    '#default_value' => variable_get('pagewatchers_ajax_update_interval', 60), // Default to 1 minute
  );

  return system_settings_form($form);
}