<?php

/**
 * @file
 * Renders administrative pages for Fortissimmo module
 */

function fortissimmo_admin_settings() {
  $form = array();

  $form['fortissimmo_general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
    '#weight' => -99,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['fortissimmo_general']['fortissimmo_default_account'] = array(
    '#type' => 'select',
    '#title' => t('Default account'),
    '#description' => t('Specify the default Fortissimmo account'),
    '#required' => TRUE,
    '#multiple' => FALSE,
    '#options' => fortissimmo_get_accounts(),
    '#default_value' => variable_get('fortissimmo_default_account', 0),
  );

  $streamwrappers = file_get_stream_wrappers(STREAM_WRAPPERS_VISIBLE);
  $scheme_list = array();
  if ($streamwrappers) {
    foreach ($streamwrappers as $scheme => $info) {
      $scheme_list[$scheme] = $info['name'];
    }
  }

  $form['fortissimmo_files'] = array(
    '#type' => 'fieldset',
    '#title' => t('Fortissimmo file directory folder'),
    '#weight' => -10,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['fortissimmo_files']['fortissimmo_files_scheme'] = array(
    '#type' => 'radios',
    '#title' => t('File directory scheme'),
    '#description' => t('Specify under which file scheme the Fortissimmo files are stored'),
    '#required' => TRUE,
    '#options' => $scheme_list,
    '#default_value' => variable_get('fortissimmo_files_scheme', file_default_scheme()),
  );

  $form['fortissimmo_files']['fortissimmo_file_path'] = array(
    '#type' => 'textfield',
    '#title' => 'Fortissimmo files folder',
    '#description' => t('Specify the folder inside your Drupal files directory that contains the Fortissimmo assets (no leading or trailing slashes)'),
    '#required' => TRUE,
    '#default_value' => variable_get('fortissimmo_file_path', ''),
  );

  $views = views_get_all_views();

  $form['fortissimmo_views'] = array(
    '#type' => 'fieldset',
    '#title' => t('Views integration'),
    '#description' => t('Views integration options'),
    '#weight' => -9,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $accounts = fortissimmo_get_accounts();
  if ($accounts) {
    foreach ($accounts as $accountid => $accountname) {
      // Get all views that can be used for this account
      $options = array();
      foreach ($views as $key => $view) {
        if (drupal_substr($key, 0, drupal_strlen('fortissimmo_' . $accountid . '_')) == 'fortissimmo_' . $accountid . '_') {
          $options[$key] = $view->name;
        }
      }
      // Add select form element to choose View to be used
      $form['fortissimmo_views']['fortissimmo_views_detail_view_' . $accountid] = array(
        '#type' => 'select',
        '#title' => t('View to render property detail page for account ' . check_plain($accountname)),
        '#description' => t('Specify the view to use for showing a property detail page. Note that only viewnames starting with "fortissimmo_<accountid>" are eligible, and the "default" display will be used.'),
        '#options' => $options,
        '#default_value' => variable_get('fortissimmo_views_detail_view_' . $accountid, 'fortissimmo_' . $accountid . '_property_detail'),
      );
    }
  }

  $form['fortissimmo_request_form'] = array(
    '#type' => 'fieldset',
    '#title' => t('"Stay informed" general form settings'),
    '#collapsible' => FALSE,
    '#collapsed' => TRUE,
  );

  $form['fortissimmo_request_form']['fortissimmo_request_form_location_search_type'] = array(
    '#type' => 'radios',
    '#title' => t('Type of location search'),
    '#description' => t('Specify the type of location search to use'),
    '#required' => TRUE,
    '#options' => array(
      'area_search' => t('Area search - let users specify range around a city'),
      'regions' => t('Regions - let users select from a dropdown list of regions as defined in Fortissimmo'),
    ),
    '#default_value' => variable_get('fortissimmo_request_form_location_search_type', 'area_search'),
  );

  return system_settings_form($form);
}