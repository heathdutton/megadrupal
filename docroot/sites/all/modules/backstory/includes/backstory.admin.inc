<?php

/**
 * @file
 * Administrative form builder functions for the Backstory module.
 */


/**
 * Form callback: configure the Backstory settings.
 */
function backstory_settings_form($form, &$form_state) {
  $form['backstory_js'] = array(
    '#type' => 'textarea',
    '#title' => t('Integration code'),
    '#description' => t('Copy and paste the integration code for this site from the Domains tab on your Backstory account.'),
    '#default_value' => variable_get('backstory_js', ''),
  );

  $form['backstory_exempt_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Disable Backstory for users with the follow roles'),
    '#options' => user_roles(),
    '#default_value' => variable_get('backstory_exempt_roles', array()),
  );

  $form['backstory_paths_mode'] = array(
    '#type' => 'radios',
    '#title' => t('Use the following mode for toggling Backstory using the paths entered below'),
    '#options' => array(
      'disable' => t('Disable Backstory on pages matching the path or patterns below.'),
      'enable' => t('Enable Backstory only on pages matching the path or patterns below.'),
    ),
    '#default_value' => variable_get('backstory_paths_mode', 'disable'),
  );

  $form['backstory_paths'] = array(
    '#type' => 'textarea',
    '#title' => t('Specify one path or pattern per line'),
    '#description' => t('Path patterns may use * as a wildcard, such as admin* disabling Backstory on all admin paths. &lt;front> may be used to match the current front page.'),
    '#default_value' => variable_get('backstory_paths', 'admin*'),
  );

  return system_settings_form($form);
}
