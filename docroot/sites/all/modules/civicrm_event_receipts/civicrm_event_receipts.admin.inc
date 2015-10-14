<?php

/**
 * @file
 * Admin form for CiviCRM Event Receipts module.
 */

/**
 * Admin settings for CiviCRM Event Receipts module.
 *
 * @param $form
 * @param $form_state
 * @return mixed
 */
function civicrm_event_receipts_admin($form, &$form_state) {
  $form['civicrm_event_receipts_cron'] = array(
    '#type' => 'select',
    '#options' => array(
      'na' => 'Never delete',
      '1 week' => '1 week',
      '3 weeks' => '3 weeks',
      '1 month' => '1 month',
      '3 months' => '3 months',
    ),
    '#title' => t('Delete receipts older than:'),
    '#default_value' => variable_get('civicrm_event_receipts_cron'),
  );
  return system_settings_form($form);
}
