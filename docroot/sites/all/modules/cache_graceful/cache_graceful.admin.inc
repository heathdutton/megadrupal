<?php
/**
 * @file
 */

include_once 'cache_graceful.defines.inc';

/**
 * Settings form
 *
 * FAPI definition for Cache Graceful settings.
 *
 * @return array
 *   FAPI definition
 */
function cache_graceful_settings_form() {
  $form = array();

  $form['cache_graceful_wait_for_cache'] = array(
    '#title' => t("Wait for cache"),
    '#description' => t("Number of seconds to wait for cache, if update is needed but it's locked."),
    '#type' => 'textfield',
    '#default_value' => variable_get('cache_graceful_wait_for_cache', CACHE_GRACEFUL_WAIT_FOR_CACHE),
  );
  $form['cache_graceful_wait_for_cache_delay'] = array(
    '#title' => t("Wait for cache delay"),
    '#description' => t("Delay in microseconds between attempts of cache gets, when waiting for cache."),
    '#type' => 'textfield',
    '#default_value' => variable_get('cache_graceful_wait_for_cache_delay', CACHE_GRACEFUL_WAIT_FOR_CACHE_DELAY),
  );
  $form['cache_graceful_lock_timeout'] = array(
    '#title' => t("Lock timeout"),
    '#description' => t("Time in seconds for lock to timeout."),
    '#type' => 'textfield',
    '#default_value' => variable_get('cache_graceful_lock_timeout', CACHE_GRACEFUL_LOCK_TIMEOUT),
  );
  $form['cache_graceful_expire'] = array(
    '#title' => t("Expiration"),
    '#description' => t("Time in seconds to cache items."),
    '#type' => 'textfield',
    '#default_value' => variable_get('cache_graceful_expire', CACHE_GRACEFUL_EXPIRE),
  );
  $form['cache_graceful_prefetch'] = array(
    '#title' => t("Prefetch"),
    '#description' => t("Time in seconds before prefetch should be performed."),
    '#type' => 'textfield',
    '#default_value' => variable_get('cache_graceful_prefetch', CACHE_GRACEFUL_PREFETCH),
  );
  $form['cache_graceful_stale'] = array(
    '#title' => t("Stale"),
    '#description' => t("Time in seconds for how long stale content should be served."),
    '#type' => 'textfield',
    '#default_value' => variable_get('cache_graceful_stale', CACHE_GRACEFUL_STALE),
  );
  $form['cache_graceful_queue'] = array(
    '#title' => t("Queue"),
    '#description' => t("Use a queue for processing instead of directly launcing background process."),
    '#type' => 'checkbox',
    '#default_value' => variable_get('cache_graceful_queue', CACHE_GRACEFUL_QUEUE),
  );

  return system_settings_form($form);
}

