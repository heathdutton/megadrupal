<?php

/**
 * @file
 * Administration settings for the Manticore module.
 */

/**
 * Menu callback; Manticore administration settings.
 */
function manticore_admin($form, &$form_state) {
  $form['#attributes'] = array('enctype' => "multipart/form-data");
  $form['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
    '#description' => t('These are settings that apply to your site globally.'),
    '#collapsed' => FALSE,
    '#collapsible' => FALSE,
  );
  $form['general']['manticore_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Key'),
    '#description' => t('The Manticore MTC_KEY value.'),
    '#default_value' => variable_get('manticore_key', ''),
    '#required' => TRUE,
  );
  $form['general']['manticore_group'] = array(
    '#type' => 'textfield',
    '#title' => t('Group'),
    '#description' => t('The Manticore MTC_GROUP value.'),
    '#default_value' => variable_get('manticore_group', ''),
    '#required' => TRUE,
  );
  $form['general']['manticore_id'] = array(
    '#type' => 'textfield',
    '#title' => t('ID'),
    '#description' => t('The Manticore MTC_ID value.'),
    '#default_value' => variable_get('manticore_id', ''),
    '#required' => TRUE,
  );

  $form['settings'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => 50,
  );
  $form['fields'] = array(
    '#type' => 'fieldset',
    '#title' => t('Fields'),
    '#description' => t('Choose which fields can be sent off to Manticore.'),
    '#group' => 'settings',
  );
  $form['fields']['manticore_availablefields'] = array(
    '#type' => 'textarea',
    '#title' => t('Fields'),
    '#description' => t('Each field is seperated by a new line. Each field has a name, and a type, seperated by a "|". Example: fieldname|type . The type can be text, token, integer, decimal, date, duration, or uri. The default type is "text".'),
    '#default_value' => manticore_availablefields(FALSE),
    '#rows' => count(manticore_availablefields()),
  );

  // The Installation Code and Application File
  $form['tracking'] = array(
    '#type' => 'fieldset',
    '#title' => t('Tracking'),
    '#description' => t('Allows injection of the installation code for Manticore tracking.'),
    '#group' => 'settings',
  );
  $form['tracking']['manticore_installationcode'] = array(
    '#type' => 'checkbox',
    '#title' => t('Installation Code'),
    '#description' => t('Inject the Manticore installation code at the footer of the site.'),
    '#default_value' => variable_get('manticore_installationcode', FALSE),
  );
  $form['tracking']['manticore_applicationfile_filepath'] = array(
    '#type' => 'textfield',
    '#title' => t('Application File Path'),
    '#description' => t('The relative path to the Manticore application file.'),
    '#default_value' => variable_get('manticore_applicationfile_filepath', ''),
  );
  $form['tracking']['manticore_applicationfile'] = array(
    '#type' => 'file',
    '#title' => t('Application File'),
    '#description' => t('Upload the downloaded Manticore application file here from the Installation Code tab.'),
    '#size' => 20,
  );
  $form['tracking']['manticore_downloadtracking'] = array(
    '#type' => 'checkbox',
    '#title' => t('Download tracking'),
    '#description' => t('When enabled, will track downloads of given file types on your site.'),
    '#default_value' => variable_get('manticore_downloadtracking', FALSE),
  );
  $form['tracking']['manticore_downloadtracking_filetypes'] = array(
    '#type' => 'textfield',
    '#title' => t('Files to track'),
    '#description' => t('Comma-seperated list of file types to track downloads of.'),
    '#size' => 20,
    '#default_value' => variable_get('manticore_downloadtracking_filetypes', 'pdf,txt,zip,tar,gz'),
  );
  $form['#submit'][] = 'manticore_admin_applicationfile_submit';

  // Advanced settings.
  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced'),
    '#description' => t('The more advanced settings associated with Manticore.'),
    '#group' => 'settings',
  );
  $form['advanced']['manticore_method'] = array(
    '#type' => 'radios',
    '#title' => t('Method'),
    '#description' => t('The method in which the Manticore API will be called.'),
    '#default_value' => variable_get('manticore_method', 'ManticoreDrupal'),
    '#required' => TRUE,
    '#options' => array(
      'ManticoreDrupal' => t('drupal_http_request()'),
      'ManticoreCurl' => t('cURL'),
    ),
  );

  // Since we are modifying what parameters are passed to Manticore, we have
  // to clear the Rules cache.
  $form['#submit'][] = 'manticore_clear_rules_cache';
  return system_settings_form($form);
}

/**
 * Clear the Rules cache.
 */
function manticore_clear_rules_cache() {
  // Clear both the static cache and the Rules database cache.
  drupal_static('rules_get_cache', array(), TRUE);
  $cid = 'data:' . $GLOBALS['language']->language;
  cache_clear_all($cid, 'cache_rules', TRUE);
}

/**
 * Form API callback to handle the file upload.
 */
function manticore_admin_applicationfile_submit($form, &$form_state) {
  $values = $form_state['values'];

  $filepath = 'public://manticore/';
  $filename = 'mtcJSAPI.js';

  file_prepare_directory($filepath, FILE_CREATE_DIRECTORY);
  $file = file_save_upload('manticore_applicationfile', array('file_validate_extensions' => array()), $filepath);
  if ($file) {
    variable_set('manticore_applicationfile_filepath', $file->uri);
    $form_state['values']['manticore_applicationfile_filepath'] = $file->uri;
  }
}
