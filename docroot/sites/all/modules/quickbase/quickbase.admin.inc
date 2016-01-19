<?php
/**
 * @file
 * 
 * This file contains the administrative page callbacks for the module
 */

/**
 * Settings form builder
 */
function quickbase_settings_form($form, &$form_state) {
  $form = array();

  // Check to see if the QuickBase PHP library is installed where the Libraries module can load it
  $lib_detected = libraries_detect('quickbase-api');
  if (empty($lib_detected['installed'])) {
    $form['library_missing'] = array(
      '#prefix' => '<h2>',
      '#suffix' => '</h2>',
      '#markup' => t('Intuit QuickBase API - PHP Class Library cannot be found on your system.'),
    );
    $form['library_download'] = array(
      '#prefix' => '<p>',
      '#suffix' => '</p>',
      '#markup' => t(
        'Please go to !url to download the latest copy, and place it in the folder %path',
        array(
          '!url' => l($lib_detected['vendor url'], $lib_detected['vendor url'], array('attributes' => array('target' => '_blank'))),
          '%path' => 'sites/all/libraries/quickbase-api',
        )
      ),
    );
    $form['library_return'] = array(
      '#prefix' => '<p>',
      '#suffix' => '</p>',
      '#markup' => t('Once the Intuit QuickBase API - PHP Class Library has been installed, please return to this page to configure your site.'),
    );
  }
  else {
    // Load current settings
    $cfg = _quickbase_settings();
    //dpm($cfg);

    $form[QB_CFG_REALM] = array(
      '#type' => 'textfield',
      '#title' => t('QuickBase domain/realm'),
      '#description' => t('Please enter the domain/realm for your account (i.e. if you access %url, your realm is %realm)', array('%url' => 'https://example.quickbase.com', '%realm' => 'example')),
      '#default_value' => !empty($cfg[QB_CFG_REALM]) ? $cfg[QB_CFG_REALM] : '',
      '#required' => TRUE,
    );

    $form[QB_CFG_TOKEN] = array(
      '#type' => 'textfield',
      '#title' => t('QuickBase application token'),
      '#description' => t('Please enter the seecurity token for your application, if one is required'),
      '#default_value' => !empty($cfg[QB_CFG_TOKEN]) ? $cfg[QB_CFG_TOKEN] : '',
      '#required' => FALSE,
    );

    $form[QB_CFG_USERNAME] = array(
      '#type' => 'textfield',
      '#title' => t('QuickBase username'),
      '#description' => t('Please enter the username for the account authorized to access QuickBase'),
      '#default_value' => !empty($cfg[QB_CFG_USERNAME]) ? $cfg[QB_CFG_USERNAME] : '',
      '#required' => TRUE,
    );

    $form[QB_CFG_PASSWORD] = array(
      '#type' => 'password',
      '#title' => t('QuickBase password'),
      '#description' => t('Please enter the password for the account authorized to access QuickBase.  Leave blank to keep currently set password.'),
    );

    $form['submit'] = array(
      '#type' => 'submit', 
      '#value' => t('Save settings'),
    );
  }

  return $form;
}

/**
 * Settings form validation
 */
function quickbase_settings_form_validate($form, &$form_state) {
  $values = $form_state['values'];
}

/**
 * Settings form submission
 */
function quickbase_settings_form_submit($form, &$form_state) {
  $cfg = _quickbase_settings();
  $values = $form_state['values'];
  if (empty($values[QB_CFG_PASSWORD])) {
    $values[QB_CFG_PASSWORD] = $cfg[QB_CFG_PASSWORD];
  }
  _quickbase_settings($values);
  drupal_set_message(t('QuickBase API settings saved.'));
}

