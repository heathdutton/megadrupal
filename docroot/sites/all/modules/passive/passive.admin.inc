<?php

/**
 * Settings form for passive caching strategy.
 */
function passive_admin_settings() {
  module_load_include('inc', 'passive', 'passive.cache');

  $form = array();

  $variables = array(
    'passive_signature_salt_update_delay' => PASSIVE_SIGNATURE_SALT_UPDATE_DELAY,
    'passive_signature_salt_update_interval' => PASSIVE_SIGNATURE_SALT_UPDATE_INTERVAL,
    'passive_request_try_limit' => PASSIVE_REQUEST_TRY_LIMIT,
    'passive_human_user_agent_pattern' => PASSIVE_HUMAN_USER_AGENT_PATTERN,
    'passive_timeout' => PASSIVE_REQUEST_QUICK_TIMEOUT,
    'passive_request_capacity_lock_timeout' => PASSIVE_REQUEST_CAPACITY_LOCK_TIMEOUT,
    'passive_request_concurrency_limit' => PASSIVE_REQUEST_CONCURRENCY_LIMIT,
  );
  $summary_rows = array();
  foreach ($variables as $variable => $default) {
    $summary_row = array('<code>' . $variable . '</code>');
    $variable_value = variable_get($variable);
    $summary_value = '';

    if (!isset($variable_value)) {
      $summary_value .= '<em>Default</em><br>';
      $variable_value = $default;
    }

    $summary_value .= '<pre>';
    if (is_bool($variable_value)) {
      $summary_value .= $variable_value ? 'TRUE' : 'FALSE';
    }
    elseif (is_scalar($variable_value)) {
      $summary_value .= check_plain($variable_value);
    }
    else {
      $summary_value .= print_r($variable_value, TRUE);
    }
    $summary_value .= '</pre>';

    $summary_row[] = $summary_value;
    $summary_rows[] = $summary_row;
  }
  $summary_table = array(
    'header' => array(
      t('Option'),
      t('Value'),
    ),
    'rows' => $summary_rows,
  );

  $form['summary']['#markup'] = theme('table', $summary_table);

  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings'),
  );
  $form['settings']['passive_queue_runtime'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum queue runtime'),
    '#size' => 10,
    '#default_value' => variable_get('passive_queue_runtime'),
    '#description' => t('Specify the maximum amount of time in seconds queued passive requests (i.e. failed to run during client-initiated request) are allowed to run at each system cron run.'),
  );

  $form['settings']['passive_flush'] = array(
    '#type' => 'checkbox',
    '#title' => t('Flush passive cache on clear'),
    '#default_value' => variable_get('passive_flush'),
    '#description' => t('If enabled, passive cache is cleared when all Drupal caches are cleared, including the "Performance" form and Drush command.'),
  );

  return system_settings_form($form);
}
