<?php
/**
 * @file
 * Admin settings for the Panels Hash Cache module.
 */

/**
 * FormAPI callback for building the settings form.
 */
function panels_hash_cache_admin_settings() {
  $form = array();
  $form['panels_hash_cache_reset_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Reset Key'),
    '#default_value' => variable_get('panels_hash_cache_reset_key', ''),
    '#description' => t('Optionally allow the cache to be regenerated via the URL using a key by hitting the page with the query "?panels-hash-cache-reset={key}". This lets, for example, to automatically re-generate a cache using cron hitting a URL. This way users never see uncached content.'),
  );
  $form['panels_hash_cache_disabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable panels-hash-cache'),
    '#default_value' => variable_get('panels_hash_cache_disabled', FALSE),
    '#description' => t('Disable panels-hash-cache for development purposes'),
  );

  return system_settings_form($form);
}
