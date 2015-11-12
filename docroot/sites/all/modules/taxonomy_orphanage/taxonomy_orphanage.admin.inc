<?php

/**
 * @file
 * Admin callbacks for taxonomy_orphanage.
 */

/**
 * Returns the system settings form.
 */
function taxonomy_orphanage_settings() {
  $form = array();

  $form['batch'] = array(
    '#type' => 'fieldset',
    '#title' => t('Manual Roundup'),
    '#description' => t('Manually start the process to remove all orphaned term references.'),
  );
  $form['batch']['roundup'] = array(
    '#type' => 'submit',
    '#value' => t('Round up'),
    '#submit' => array('taxonomy_orphanage_admin_roundup'),
  );
  $form['taxonomy_orphanage_cron_roundup'] = array(
    '#type' => 'checkbox',
    '#title' => t('Round up orphan references on cron'),
    '#default_value' => variable_get('taxonomy_orphanage_cron_roundup', TRUE),
    '#description' => t('Check this to enable orphan roundups on cron runs.'),
  );
  $form['taxonomy_orphanage_cron_limit'] = array(
    '#type' => 'select',
    '#title' => t('Cron roundup limit'),
    '#default_value' => variable_get('taxonomy_orphanage_cron_limit', 50),
    '#description' => t('The number of entities per field per cron run to process. NOTE: setting this to a high value can cause instability if your crons are executed via the web server and not by drush.'),
    '#options' => array('-1' => t('All'), 10 => 10, 20 => 20, 50 => 50, 100 => 100, 200 => 200, 500 => 500, 1000 => 1000),
  );

  return system_settings_form($form);
}

/**
 * Admin callback to remove orphaned term references.
 */
function taxonomy_orphanage_admin_roundup($form, &$form_state) {
  taxonomy_orphanage_roundup();
}

