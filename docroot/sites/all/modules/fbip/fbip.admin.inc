<?php

/**
 * @file
 * File containing admin settings for FBI module
 */

/**
 * Page callback: Form Flood Control Settings
 *
 */
function fbip_form($form, &$form_state) {
  $form['fbip_all'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Flood Control On All Form Submissions'),
    '#default_value' => variable_get('fbip_all', 0),
    '#description' => t('Turns on Flood Control on all forms on the site'),
  );

  $form['fbip_form_ids'] = array(
    '#type' => 'textarea',
    '#title' => t('Forms to Impose Flood Control on'),
    '#default_value' => variable_get('fbip_form_ids', ''),
    '#description' => t('List of Form Ids, one in each line, to impose form flood control restrictions on. '
        . 'If you are not sure of the form Id, use a module like http://www.drupal.org/project/get_form_id '
        . 'to know the id of any form on the site.'),
    '#states' => array(
      'invisible' => array(
        ':input[name="fbip_all"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['fbip_form_whitelist'] = array(
    '#type' => 'textarea',
    '#title' => t('Whitelisted IPs'),
    '#default_value' => variable_get('fbip_form_whitelist', ''),
    '#description' => t('List of IPs to whitelist. Whitelisted IPs will bypass '
        . 'restrictions by this module and will not be blocked. Only list of IPs allowed. '
        . 'No Ranges allowed. One per line. Your current IP address is <b>:ipadr</b>', array(':ipadr' => ip_address())),
  );

  $form['fbip_threshold'] = array(
    '#type' => 'textfield',
    '#title' => t('Threshold (Number of form submissions)'),
    '#default_value' => variable_get('fbip_threshold', 100),
    '#description' => t('The maximum number of times each user can open restricted forms per time window defined below'),
  );

  $form['fbip_window'] = array(
    '#type' => 'textfield',
    '#title' => t('Time Window (In Seconds)'),
    '#default_value' => variable_get('fbip_window', 3600),
    '#description' => t('Number of seconds in the time window for restricting the form generation'),
  );

  $form['fbip_reset'] = array(
    '#type' => 'checkbox',
    '#title' => t('Reset IP Bans on cron run'),
    '#default_value' => variable_get('fbip_reset', 0),
    '#description' => t('Remove all previous IP bans on cron run. NOTE: This will affect IPs banned NOT just by this module alone but affects all banned IPs'),
  );

  return system_settings_form($form);
}
