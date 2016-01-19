<?php
// $Id$
/**
 * @file Define admin forms for the JStats module.
 */

/**
 * Define the main settings form.
 */
function jstats_settings_form() {
  $form = array();
  
  $period = drupal_map_assoc(array(0, 60, 180, 300, 600, 900, 1800, 2700, 3600, 10800, 21600, 32400, 43200, 86400), 'format_interval');

  $form['jstats_cron_period'] = array(
    '#type' => 'select',
    '#title' => t('Statistics lifetime'),
    '#description' => t('Select how often the access statistics should be calcultated. Requires a correctly configured cron task.'),
    '#options' => $period,
    '#default_value' => variable_get('jstats_cron_period', 900)
  );

  $recent = drupal_map_assoc(array(3600, 10800, 43200, 86400, 172800, 259200, 604800), 'format_interval');
  $form['jstats_cron_recent'] = array(
    '#type' => 'select',
    '#title' => t('Freshness'),
    '#description' => t('Select for how long an access is still counted as a <em>recent</em> one. The recent access count of the node is then what this module exposes in Views.'),
    '#options' => $recent,
    '#default_value' => variable_get('jstats_cron_recent', 172800)
  );

  $keep = drupal_map_assoc(array(10800, 43200, 86400, 172800, 259200, 604800, 1209600), 'format_interval');
  $form['jstats_flush_accesslogs_after'] = array(
    '#type' => 'select',
    '#title' => t('Data expiration'),
    '#description' => t("Select for how access data should be kept. Be warned that on high-traffic site, the amount of data may grow <em>very fast</em>, so you probably don't want to keep those data for two long. On the other hand, this <em>must</em> be bigger than the freshness above for the module to work correctly."),
    '#options' => $keep,
    '#default_value' => variable_get('jstats_flush_accesslogs_after', 604800)
  );

  return system_settings_form($form);
}

/**
 * Validate that the selected values for data expiration and freshness are
 * compatible.
 */
function jstats_settings_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  if ($values['jstats_flush_accesslogs_after'] < $values['jstats_cron_recent']) {
    form_set_error('jstats_flush_accesslogs_after', t('Data expiration must be bigger than Freshness'));
  }

}
