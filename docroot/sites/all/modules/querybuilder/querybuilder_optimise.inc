<?php

/**
 * @file
 * Optimise database.
 */

/**
 * Form builder; Return form for clear cache.
 */
function querybuilder_optimise($form, &$form_state) {
  $form['querybuilder_clear_cache_by_cron'] = array(
    '#type' => 'checkbox',
    '#title' => t('Optimise database'),
    '#default_value' => variable_get('querybuilder_clear_cache_by_cron', FALSE),
    '#description' => t('When checked, clear cache every time when cron run.'),
  );
  return system_settings_form($form);
}
