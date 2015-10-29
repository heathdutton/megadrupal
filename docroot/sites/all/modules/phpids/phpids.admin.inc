<?php
/**
 * @file
 * Provides the administration form for PHPIDS module.
 */

/*
 * Callback function to configure PHPIDS
 *
 * @return
 *      Returns configuration form for phpids module.
 *
 * @ingroup forms
 */
function phpids_admin_settings() {
  // general settings 
  $form['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
  );
  $form['general']['phpids_path'] = array(
    '#type' => 'textfield',
    '#title' => t('PHP-IDS Path'),
    '#default_value' => variable_get('phpids_path', realpath(dirname(__FILE__))),
    '#description' => t('Defines path to the PHP-IDS. Example: /usr/share/php/phpids/lib'),
  );
  $form['general']['phpids_tmppath'] = array(
    '#type' => 'textfield',
    '#title' => t('PHP-IDS Temp Path'),
    '#default_value' => variable_get('phpids_tmppath', ini_get('upload_tmp_dir') .'phpids'),
    '#description' => t('Defines temp path used by PHP-IDS. Example: /tmp/phpids'),
  );
  $form['general']['phpids_loglevel'] = array(
    '#type' => 'textfield',
    '#title' => t('Logging impact'),
    '#default_value' => variable_get('phpids_loglevel', 1),
    '#description' => t('Impact level which must be reached to be logged by watchdog.'),
  );
  $form['general']['phpids_warnlevel'] = array(
    '#type' => 'textfield',
    '#title' => t('Warning impact'),
    '#default_value' => variable_get('phpids_warnlevel', 9),
    '#description' => t('Sends out mail when this level of impact is reached.'),
  );
  $form['general']['phpids_blocklevel'] = array(
    '#type' => 'textfield',
    '#title' => t('Blocking impact'),
    '#default_value' => variable_get('phpids_blocklevel', 27),
    '#description' => t('Redirects to a warning page after this level of impact is reached.'),
  );
  $form['general']['phpids_mail'] = array(
    '#type' => 'textfield',
    '#title' => t('Email'),
    '#default_value' => variable_get('phpids_mail', ''),
    '#description' => t("Leave empty if you don't want to send out email"),
  );
  // finetune filter settings
  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Filter settings'),
    '#description' => t("Finetune settings when PHPIDS shouldn't take action. Keep in mind that user 1 is always ignored and anonymous users are always monitored!"),
  );
  $options_anon = array(1 => t('Log anonymous users without actions'), 2 => t('Log anonymous users and take actions'));
  $form['filters']['phpids_anonymous'] = array(
    '#type' => 'select',
    '#title' => t('Anonymous users'),
    '#description' => t('Choose a setting for anonymous users.'),
    '#default_value' => variable_get('phpids_anonymous', 2),
    '#options' => $options_anon,
  );
  $options_auth = array(1 => t('Do not log authenticated users'), 2 => t('Log authenticated users without actions'), 3 => t('Log authenticated users and take actions'));
  $form['filters']['phpids_authenticated'] = array(
    '#type' => 'select',
    '#title' => t('Authenticated users'),
    '#description' => t('Choose a setting for authenticated users.'),
    '#default_value' => variable_get('phpids_authenticated', 2),
    '#options' => $options_auth,
  );
  $form['filters']['phpids_html_fields'] = array(
    '#type' => 'textfield',
    '#title' => t('HTML fields'),
    '#default_value' => variable_get('phpids_html_fields', ''),
    '#description' => t('Specify comma seperated your fields which contains HTML code. This may reduce false positives.'),
  );
  $form['filters']['phpids_json_fields'] = array(
    '#type' => 'textfield',
    '#title' => t('JSON fields'),
    '#default_value' => variable_get('phpids_json_fields', ''),
    '#description' => t('Specify comma seperated your fields which contains JSON code. This may reduce false positives.'),
  );
  $form['filters']['phpids_excl_fields'] = array(
    '#type' => 'textfield',
    '#title' => t('Excluded fields'),
    '#default_value' => variable_get('phpids_excl_fields', ''),
    '#description' => t('Specify comma seperated your fields which you want to exclude from scanning by PHP-IDS. BE CAREFUL WITH THIS OPTION!'),
  );
  $form['message'] = array(
    '#type' => 'fieldset',
    '#title' => t('Warning message settings'),
    '#description' => t('Finetune settings for warning message which is displayed if block level is reached.'),
  );  
  $form['message']['phpids_warn_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Warning message title'),
    '#default_value' => variable_get('phpids_warn_title', 'Security warning'),
    '#description' => t('Warning message title which is displayed if block level is reached.'),
  );
  $form['message']['phpids_warn_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Warning message text'),
    '#default_value' => variable_get('phpids_warn_text', 'We have detected malicious input and blocked your attempt.<br />If you keep experiencing problems but feel like you are doing nothing wrong, please contact the site administrator.'),
    '#description' => t('Warning message text which is displayed if block level is reached.'),
  );
  $form['testonly'] = array(
    '#type' => 'fieldset',
    '#title' => t('Test-only mode settings'),
    '#description' => t('Settings for PHPIDS test-only mode.'),
  );
  $form['testonly']['phpids_testonly_activated'] = array(
    '#type' => 'checkbox',
    '#title' => t('Activate PHPIDS test-only mode'),
    '#default_value' => variable_get('phpids_testonly_activated', FALSE),
    '#description' => t('When PHPIDS test-only mode is activated PHPIDS only loggs infos, warnings, alerts and does not block any user activity. So it is possible to test your PHPIDS settings.'),
  );
  $form['testonly']['phpids_testonly_withmail'] = array(
    '#type' => 'checkbox',
    '#title' => t('Activate mailing in PHPIDS test-only mode'),
    '#default_value' => variable_get('phpids_testonly_withmail', FALSE),
    '#description' => t('Sents PHPIDS mails also when PHPIDS test-only mode is activated.'),
  );

  return system_settings_form($form);
}