<?php

/**
 * @file
 * Admin page callback file for the user module.
 */

/**
 * Form builder; Configure user settings for this site.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function ip2location_admin_settings($form, &$form_state) {
  $form['#validate'][] = 'ip2location_admin_settings_validate';

  $form['ip2location_basic_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Basic Settings'),
  );

  $form['ip2location_basic_settings']['ip2location_bin_path'] = array(
    '#type' => 'textfield',
    '#title' => t('IP2Location BIN database path'),
    '#description' => t('Relative path to your Drupal installation of to where the IP2Location BIN database was uploaded. For example: sites/default/files/IP2Location-LITE-DB11.BIN. Note: You can get the latest BIN data at !lite (free LITE edition) or !ip2location (commercial edition).', array(
      '!lite' => l(t('http://lite.ip2location.com'), 'http://lite.ip2location.com/?r=drupal'),
      '!ip2location' => l(t('http://www.ip2location.com'), 'http://www.ip2location.com/?r=drupal'),
    )),
    '#default_value' => variable_get('ip2location_bin_path', ''),
    '#states' => array(
      'visible' => array(
        ':input[name="ip2location_source"]' => array(
          'value' => 'ip2location_bin',
        ),
      ),
    ),
  );

  $form['ip2location_basic_settings']['ip2location_cache'] = array(
    '#type' => 'select',
    '#title' => t('Cache Mode'),
    '#description' => t('"No cache" - standard lookup with no cache. "Memory cache" - cache the database into memory to accelerate lookup speed. "Shared memory" - cache whole database into system memory and share among other scripts and websites. Please make sure your system have sufficient RAM if enabling "Memory cache" or "Shared memory".'),
    '#options' => array(
      'no_cache' => t('No cache'),
      'memory_cache' => t('Memory cache'),
      'shared_memory' => t('Shared memory'),
    ),
    '#default_value' => variable_get('ip2location_cache', 'no_cache'),
  );

  return system_settings_form($form);
}

/**
 * Form validation handler for the user_admin_role() form.
 */
function ip2location_admin_settings_validate($form, &$form_state) {
  if (empty($form_state['values']['ip2location_bin_path'])) {
    form_set_error('ip2location_bin_path', t('Please provide IP2Location binary database path.'));
  }

  if (!empty($form_state['values']['ip2location_bin_path'])) {
    // Check IP2Location binary file path if valid.
    if (!is_file($form_state['values']['ip2location_bin_path'])) {
      form_set_error('ip2location_bin_path', t('The IP2Location binary database path is not valid.'));
    }
    else {
      // Check IP2Location binary file if valid.
      try {
        module_load_include('inc', 'ip2location', 'includes/IP2Location');
        $location_data = new IP2Location($form_state['values']['ip2location_bin_path'], IP2Location::FILE_IO);
        $record = $location_data->lookup('8.8.8.8', IP2Location::ALL);

        if (empty($record->ipNumber)) {
          form_set_error('ip2location_bin_path', t('The IP2Location binary database is not valid or corrupted.'));
        }
      } catch (Exception $error) {
        form_set_error('ip2location_bin_path', t('The IP2Location binary database is not valid or corrupted.'));
      }
    }
  }
}
