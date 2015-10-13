<?php
/**
 * @file
 * Provides functions to render, validate, and submit the geckoboard_push
 * admin settings.
 */

/**
 * Displays Geckoboard Push administration settings page.
 */
function geckoboard_push_admin_settings() {
  $form = array();

  $form['#tree'] = TRUE;

  $form['geckoboard_global_settings_heading'] = array(
    '#value' => '<h2>' . t('Global Settings') . '</h2>',
  );

  $form['geckoboard_push_api_endpoint'] = array(
    '#type'        => 'textfield',
    '#title'       => t('Geckoboard Push Endpoint base'),
    '#description' => t('The URL to use as a base for your widget Push URL'),
    '#required'    => TRUE,
    '#size'        => 32,
    '#maxlength'   => 255,
    '#default_value' => variable_get('geckoboard_push_api_endpoint', 'https://push.geckoboard.com/v1/send'),
  );

  $form['geckoboard_push_api_key'] = array(
    '#type'        => 'textfield',
    '#title'       => t('Geckoboard API Key'),
    '#description' => t('Your API key - available from "Your Account" in Geckoboard'),
    '#required'    => TRUE,
    '#size'        => 32,
    '#maxlength'   => 255,
    '#default_value' => variable_get('geckoboard_push_api_key', ''),
  );

  $form['geckoboard_push_disable_cron'] = array(
    '#type' => 'checkbox',
    '#title' => t('Do not send data to Geckoboard when cron runs'),
    '#description' => t('Use this if you only want to send data to Geckoboard using drush or your own manual approach. Can also be used to temporarily turn off sending without disabling the module.'),
    '#default_value' => variable_get('geckoboard_push_disable_cron', 0),
  );

  $form['geckoboard_push_available_datasets'] = array(
    '#title' => t('Available datasets'),
    '#type' => 'fieldset',
  );

  $settings = variable_get('geckoboard_push_available_datasets', NULL);

  if ($settings == NULL) {
    $settings = array();
  }
  else {
    $settings = unserialize($settings);
  }

  $args = func_get_args();
  foreach (module_implements('geckoboard_push_info') as $module) {
    $function = $module . '_geckoboard_push_info';

    $datasets = call_user_func_array($function, $args);

    foreach ($datasets as $k => $dataset) {
      $dataset_function = $dataset['function'];
      $available[] = $dataset_function;
      $form['geckoboard_push_available_datasets'][$dataset_function] = array(
        '#type' => 'fieldset',
        '#title' => t('@name', array('@name' => $dataset['name'])),
      );

      $form['geckoboard_push_available_datasets'][$dataset_function]['key'] = array(
        '#type' => 'textfield',
        '#title' => t('Widget Key for "@name"', array('@name' => $dataset['name'])),
        '#default_value' => isset($settings[$dataset_function]['key']) ? $settings[$dataset_function]['key'] : '',
      );
      $form['geckoboard_push_available_datasets'][$dataset_function]['enabled'] = array(
        '#type' => 'checkbox',
        '#title' => t('Enable "@name"', array('@name' => $dataset['name'])),
        '#default_value' => isset($settings[$dataset_function]['enabled']) ? $settings[$dataset_function]['enabled'] : 0,
      );
    }
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save',
  );

  return $form;
}

/**
 * Implements hook_validate().
 */
function geckoboard_push_admin_settings_validate($form, &$form_state) {

  if (!valid_url($form_state['values']['geckoboard_push_api_endpoint'], TRUE)) {
    form_set_error('geckoboard_push_api_endpoint', t('Endpoint Base must be a valid URL'));
  }

  $seen_keys = array();
  foreach ($form_state['values']['geckoboard_push_available_datasets'] as $function => $options) {
    if ($options['enabled'] == 1) {
      if (empty($options['key'])) {
        form_set_error("geckoboard_push_available_datasets][$function][key", t('You must supply a key for an enabled dataset'));
      }
      else {
        if (in_array($options['key'], $seen_keys)) {
          form_set_error("geckoboard_push_available_datasets][$function][key", t('You cannot use the same key for two different datasets'));
        }
        $seen_keys[] = $options['key'];
      }
    }
  }
}

/**
 * Implements hook_submit().
 */
function geckoboard_push_admin_settings_submit($form, &$form_state) {

  variable_set('geckoboard_push_api_endpoint', $form_state['values']['geckoboard_push_api_endpoint']);
  variable_set('geckoboard_push_api_key', $form_state['values']['geckoboard_push_api_key']);
  variable_set('geckoboard_push_disable_cron', $form_state['values']['geckoboard_push_disable_cron']);

  if ($form_state['values']['geckoboard_push_disable_cron'] == 1) {
    drupal_set_message(t('Disabled Geckoboard Push cron hook, data will not be automatically sent to Geckoboard'));
  }

  $settings = array();
  foreach ($form_state['values']['geckoboard_push_available_datasets'] as $function => $options) {
    if (trim($options['key']) != '') {
      $settings[$function] = array('enabled' => $options['enabled'], 'key' => $options['key']);
    }
  }
  variable_set('geckoboard_push_available_datasets', serialize($settings));
  drupal_set_message(t('Options saved'));
}
