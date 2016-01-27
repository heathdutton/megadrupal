<?php
/**
 * @file
 * Twitterstream page callbacks.
 */

/**
 * Module settings form.
 */
function twitterstream_admin_settings($form, &$form_state) {
  $form = array();

  $form['auth'] = array(
    '#type' => 'fieldset',
    '#title' => t('Authentication'),
    '#tree' => FALSE,
  );

  $form['auth']['twitterstream_account'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter Account'),
    '#size' => 20,
    '#default_value' => variable_get('twitterstream_account', ''),
    '#description' => t('Twitter account name or id to access Streaming API with. Must be authorized through the <a href="@url">Twitter module settings</a>.',
      array(
        '@url' => url('admin/config/services/twitter'),
      )
    ),
    '#element_validate' => array('_twitterstream_element_validate_account_authenticated'),
  );

  $form['daemon'] = array(
    '#type' => 'fieldset',
    '#title' => t('Daemon Settings'),
    '#tree' => FALSE,
  );

  $form['daemon']['twitterstream_status_period'] = array(
    '#type' => 'textfield',
    '#title' => t('Minimum status update inverval'),
    '#description' => t('Minimum period between writing status updates to the log file in seconds.<br>Default is 10 minutes.'),
    '#size' => 5,
    '#default_value' => variable_get('twitterstream_status_perdiod', 600),
  );

  $form['daemon']['twitterstream_filter_check'] = array(
    '#type' => 'textfield',
    '#title' => t('Minimum filter check interval'),
    '#description' => t('Minimum period between checking for changes to the filter parameters in seconds.<br>Filter changes will only be reflected in the stream at most every 2 minutes, even if the parameters are checked more frequently.<br>Default is 1 minute.'),
    '#size' => 5,
    '#default_value' => variable_get('twitterstream_filter_check', 60),
  );

  $form['daemon']['twitterstream_daemon_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Daemon name'),
    '#size' => 16,
    '#maxlength' => 16,
    '#default_value' => variable_get('twitterstream_daemon_name', 'twitterstream'),
    '#description' => t('App name of the daemon.  Only needs to be changed if running multiple instances on the same server.<br>Default value is \'twitterstream\'.'),
  );

  $form['daemon']['twitterstream_daemon_user'] = array(
    '#type' => 'textfield',
    '#title' => t('Name of system user to run daemon as'),
    '#description' => t('Default is the Ubuntu Apache user, \'www-data\'.'),
    '#size' => 20,
    '#default_value' => variable_get('twitterstream_daemon_user', 'www-data'),
  );

  $form['daemon']['twitterstream_daemon_group'] = array(
    '#type' => 'textfield',
    '#title' => t('Name of system group to run daemon as'),
    '#size' => 20,
    '#default_value' => variable_get('twitterstream_daemon_group', ''),
    '#description' => t('Leave blank to use the user\'s default group.'),
  );

  return system_settings_form($form);
}
