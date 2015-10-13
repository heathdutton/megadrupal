<?php

/**
 * @file
 * This is the Devicons settngs form.
 */

define('DEVICONS_ENABLE_GLOBAL', FALSE);

/**
 * Form content for Devicons settings form.
 *
 * Simple form to enable/disable Devicons
 */
function devicons_settings_form($form, &$form_state) {

  global $base_url;
  $form = array();

  $deviconvar = variable_get('devicons_global', DEVICONS_ENABLE_GLOBAL);

  if (($library = libraries_detect('devicons')) && !empty($library['installed'])) {

    if ($deviconvar == 'TRUE') {
      $description = t('Disable/Enable Devicons for the whole page.<br />Congratulations!!! Please take a look at the demo page: !demopage', array('!demopage' => l(t('Click'), $base_url . '/admin/config/devicons/demo')));
    }
    else {
      $description = t('Disable/Enable Devicons for the whole page.');
    }

    $form['devicons_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Devicons Settings'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );

    $form['devicons_settings']['devicons_global'] = array(
      '#type' => 'select',
      '#title' => t('Enable Global'),
      '#options' => array(
        'FALSE' => t('off'),
        'TRUE' => t('on'),
      ),
      '#default_value' => variable_get('devicons_global', DEVICONS_ENABLE_GLOBAL),
      '#description' => $description,
    );

    return system_settings_form($form);
  }
  else {
    // This contains a detailed (localized) error message.
    $error_message = $library['error message'];
    drupal_set_message($error_message, 'error');
    drupal_set_message(t("<p>Please check Devicon !statuspage .</p>",
      array('!statuspage' => l(t('installation status'), $base_url . '/admin/reports/status'))));
  }
}
