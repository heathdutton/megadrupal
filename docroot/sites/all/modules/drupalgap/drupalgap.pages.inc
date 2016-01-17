<?php

/**
 * @file
 * Provides page callback functions for DrupalGap.
 */

/**
 * Returns the HTML for the DrupalGap module status page.
 */
function drupalgap_status() {
  // Add drupalgap javascript settings to Drupal settings.
  drupal_add_js(array('drupalgap' => array('services_endpoint_default' => base_path() . '?q=drupalgap/')), 'setting');

  // Add drupalgap javascript.
  drupal_add_js(drupal_get_path('module', 'drupalgap') . '/drupalgap.js');

  // Set div id for system connect status message box.
  $div_id = 'drupalgap-system-connect-status-message';

  // Create output fieldsets.
  $sdk_form = drupal_get_form('drupalgap_sdk_form');
  $output = array(

    'drupalgap_app' => array(
      '#theme' => 'fieldset',
      '#title' => t('Application Development Kit'),
      '#markup' => drupal_render($sdk_form),
    ),

    'drupalgap_system_connect_status' => array(
      '#theme' => 'fieldset',
      '#description' => '<div style="float: right;">' .l(
        t('Edit DrupalGap Service Resources'),
        'admin/structure/services/list/drupalgap/resources',
        array(
          'query' => array(
            'destination' => implode('/', arg())
          )
        )
      ) . '</div>',
      '#title' => t('App status'),
      '#markup' =>
        '<input type="button" class="form-submit" value="' . t('Test Connection') . '" onclick="drupalgap_system_connect_test();" />' .
        '<div id="' . $div_id . '"></div>' . drupalgap_modules_widget(),
    ),

  );

  return $output;
}

/**
 * Zip's up the "mobile-application" directory and downloads it for the user.
 */
function drupalgap_download_app() {
  $zipname = 'drupalgap-app.zip';
  DrupalGapZip(variable_get('drupalgap_sdk_dir', 'mobile-application'), $zipname);
  header('Content-Type: application/zip');
  header("Content-Disposition: attachment; filename='$zipname'");
  header('Content-Length: ' . filesize($zipname));
  header("Location: " . $GLOBALS['base_path'] . $zipname);
  drupal_exit();
}

