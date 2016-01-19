<?php

function commerce_todopago_admin_settings() {
  $form = array();

  $form['settings'] = array(
    '#type' => 'vertical_tabs',
  );

  /**
   * Cron configs
   */
  $form['cron'] = array(
    '#type' => 'fieldset',
    '#title' => t('Cron'),
    '#collapsible' => true,
    '#collapsed' => true,
    '#weight' => 5,
    '#group' => 'settings',
  );
  $cron_range = array(
    3600 => t('1 hour'),
    7200 => t('2 hours'),
    14400 => t('4 hours'),
    21600 => t('6 hours'),
    43200 => t('12 hours'),
    86400 => t('1 day'),
  );
  $form['cron']['commerce_todopago_cron_range'] = array(
    '#type' => 'select',
    '#title' => t('Execution range'),
    '#description' => t('Time range to check the remote status of pending transactions.'),
    '#options' => $cron_range,
    '#default_value' => variable_get('commerce_todopago_cron_range', 21600),
  );

  return system_settings_form($form);
}

