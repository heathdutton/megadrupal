<?php

/**
 * @file
 * Admin settings form for Capture version
 */

/**
 * Sets the Capture version.
 */
function janrain_capture_ui_version_settings() {
  $form['description'] = array(
    '#markup' => '<p>' . t('Connection to Janrain Capture requires a subscription. Once subscribed, use the values on the Janrain dashboard to complete the fields in the Settings tab.') . '</p>',
  );
  $janrain_capture_ver = variable_get('janrain_capture_ver', JANRAIN_CAPTURE_VERSION_DEFAULT);

  $form['janrain_capture_ver'] = array(
    '#type' => 'select',
    '#title' => t('Capture Version'),
    '#options' => array(
      JANRAIN_CAPTURE_VERSION_WIDGET => t('Capture'),
      JANRAIN_CAPTURE_VERSION_LEGACY => t('Capture Legacy'),
    ),
    '#default_value' => !empty($janrain_capture_ver) ? $janrain_capture_ver : JANRAIN_CAPTURE_VERSION_DEFAULT,
    '#description' => t('Which version of capture are you using? Chose and save before configuring. The settings tab will change acorrdingly.'),
  );

  return system_settings_form($form);
}

