<?php

/**
 * @file
 * Administrative page callbacks for the Analytics module.
 */

function analytics_settings_form($form, $form_state) {
  $form['privacy'] = array(
    '#type' => 'fieldset',
    '#title' => t('Privacy'),
    '#collapsed' => FALSE,
  );
  $form['privacy']['analytics_privacy_dnt'] = array(
    '#type' => 'checkbox',
    '#title' => t('Respect Do Not Track (DNT) cookies.'),
    '#default_value' => variable_get('analytics_privacy_dnt', TRUE),
  );
  $form['privacy']['analytics_privacy_anonymize_ip'] = array(
    '#type' => 'checkbox',
    '#title' => t('Anonymize IP addresses.'),
    '#default_value' => variable_get('analytics_privacy_anonymize_ip', FALSE),
  );

  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['advanced']['analytics_cache_urls'] = array(
    '#type' => 'checkbox',
    '#title' => t('Cache files locally where possible.'),
    '#default_value' => variable_get('analytics_cache_urls', FALSE),
  );
  $form['advanced']['analytics_disable_page_build'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable default analytics service rendering in hook_page_build().'),
    '#default_value' => variable_get('analytics_disable_page_build', FALSE),
  );

  return system_settings_form($form);
}
