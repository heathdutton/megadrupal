<?php

/**
 * @file
 *   Include file for tools page
 *
 */

/**
 * Form for "Tools" tab.
 * 
 * "Tools" tab has two forms:
 * - Manual Cron Call 
 * - Reset Data
 */
function statspro_tools($form, &$form_state) {
  $form = array();

  $form['intro'] = array(
    '#type' => 'markup',
    '#value' => '<div>' . t('On this page you find useful tools for Statistics maintenance.') . '</div>',
  );

  $form['cron_call_wrapper'] = array(
    '#type' => 'fieldset',
    '#title' => t('Manual cron call'),
  );
  $form['cron_call_wrapper']['intro_cron_call'] = array(
    '#type' => 'markup',
    '#value' => '<div>' . t('Call a cron job run to generate up to date statistics.') . '</div>',
  );
  $form['cron_call_wrapper']['cron_call'] = array(
    '#type' => 'submit',
    '#value' => t('Call cron'),
  );

  $form['reset_stats_wrapper'] = array(
    '#type' => 'fieldset',
    '#title' => t('Reset data'),
  );
  $form['reset_stats_wrapper']['intro_reset_stats'] = array(
    '#type' => 'markup',
    '#value' => '<div>' . t('Clear all Statistics Pro tables.') . '</div>',
  );
  $form['reset_stats_wrapper']['reset_stats'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
  );

  return $form;
}

/**
 * Handler for "Tools" tab buttons 'Call cron' and 'Reset'.
 */
function statspro_tools_submit($form, &$form_state) {
  switch ($form_state['values']['op']) {
    case t('Call cron'):
      drupal_set_message(t('Cron run called.'));
      drupal_goto('admin/reports/status/run-cron');
      break;

    case t('Reset'):
      drupal_set_message(t('Statistics Pro data cleared.'));
      statspro_reset_stats();
      break;
  }
}
