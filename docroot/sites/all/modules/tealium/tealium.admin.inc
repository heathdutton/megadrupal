<?php
/**
 * @file
 * Administration settings for Tealium.
 */


/**
 * @return array
 *  Drupal form API array
 */
function tealium_admin_form() {
  $form = array();

  $form['tealium'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Tealium Universal Tag settings'),
    '#collapsible' => FALSE,
  );

  $form['tealium']['tealium_account'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Tealium account'),
    '#default_value' => variable_get('tealium_account', TEALIUM_DEFAULT_ACCOUNT),
    '#size'          => 20,
    '#required'      => TRUE,
  );

  $form['tealium']['tealium_profile'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Tealium profile'),
    '#default_value' => variable_get('tealium_profile', TEALIUM_DEFAULT_PROFILE),
    '#size'          => 20,
    '#required'      => TRUE,
  );

  $tealium_environment_options = array(
    'dev'  => t('Development'),
    'qa'   => t('Testing / QA'),
    'prod' => t('Production'),
  );

  $form['tealium']['tealium_environment'] = array(
    '#type'          => 'radios',
    '#title'         => t('Tealium environment'),
    '#description' => t(
      'Set as either your Development, internal Testing or publicly available Production web-site.'
    ),
    '#default_value' => variable_get('tealium_environment', TEALIUM_DEFAULT_ENVIRONMENT),
    '#options'       => $tealium_environment_options,
    '#required'      => TRUE,
  );

  $form['tealium']['tealium_utag_async'] = array(
    '#type' => 'checkbox',
    '#title' => t('Load Asynchronously'),
    '#description' => t(
      'Load the Tealium Universal Tag asynchronously (recommended).',
      array(
        '%settings_file' => 'sites/domain.name/settings.php',
        '%variable_name' => '$conf[\'tealium_async\']',
      )
    ),
    '#default_value' => variable_get('tealium_utag_async', TEALIUM_DEFAULT_UTAG_ASYNC),
  );

  return system_settings_form($form);
}
