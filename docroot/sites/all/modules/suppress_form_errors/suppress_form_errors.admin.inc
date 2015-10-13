<?php

/**
 * @file
 * Suppress Form Errors Module Adming config form
 */

/**
 * Settings page Callback function.
 */
function suppress_form_errors_settings() {
  $form['suppress_form_errors_formids'] = array(
    '#type' => 'textarea',
    '#title' => t('List of Form Ids on which errors should be suppressed'),
    '#default_value' => variable_get('suppress_form_errors_formids', ''),
    '#description' => t('One id or pattern per Line. Allowed wild card character - * <br/>'
        . 'For Example, webform_client_form_* will match all webform form ids like webform_client_form_3 , webform_client_form_4 etc <br/>'
        . 'Use a module like https://www.drupal.org/project/get_form_id if you need help in finding your form id.'),
  );

  return system_settings_form($form);
}
