<?php

/**
 * @file
 * Administrative page callbacks for Google Reader API module.
 */

function google_reader_api_settings() {

  $library = libraries_load('Google-Reader-API');

  $form['library'] = array(
    '#type' => 'fieldset',
    '#title' => t('Library'),
  );

  $form['library']['status'] = array(
    '#prefix' => 'Status: <strong>',
    '#suffix' => '</strong>',
    '#markup' => 'Installed'
  );

  $form['authentication'] = array(
    '#type' => 'fieldset',
    '#title' => t('Authentication'),
  );

  if ( !$library || ( empty( $library['loaded'] ) ) ) {
    $form['authentication']['#disabled'] = TRUE;
    $form['library']['status']['#markup'] = 'Missing';
    $form['library']['status']['#suffix'] = t('</strong><p><a href="@url">Download</a> and extract into a libraries directory.  Example: "sites/all/libraries/Google-Reader-API"</p>', array('@url' => $library['download url']));
  }

  $form['authentication']['google_reader_api_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email'),
    '#default_value' => variable_get('google_reader_api_email', '')
  );

  $form['authentication']['google_reader_api_password'] = array(
    '#type' => 'password',
    '#title' => t('Password')
  );

  return system_settings_form($form);

}