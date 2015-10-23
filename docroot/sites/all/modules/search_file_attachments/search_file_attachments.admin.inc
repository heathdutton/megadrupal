<?php

/**
 * @file
 * Admin settings
 */

require_once dirname(__FILE__) . '/search_file_attachments.inc';

/**
 * Page callback to show the settings for the attachments.
 */
function search_file_attachments_settings_form() {
  $form = array();

  // Apache Tika configuration.
  $form['tika'] = array(
    '#type' => 'fieldset',
    '#title' => t('Apache Tika'),
  );
  $form['tika']['search_file_attachments_tika_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Tika directory path:'),
    '#default_value' => variable_get('search_file_attachments_tika_path', ''),
    '#description' => t('The full path to tika directory.'),
  );
  $form['tika']['search_file_attachments_tika_jar'] = array(
    '#type' => 'textfield',
    '#title' => t('Tika jar file:'),
    '#default_value' => variable_get('search_file_attachments_tika_jar', 'tika-app-1.2.jar'),
    '#description' => t('The name of the tika CLI application jar file, e.g. tika-app-1.2.jar.'),
  );
  if (!search_file_attachments_check_java()) {
    $form['tika']['search_file_attachments_java_path'] = array(
      '#type' => 'textfield',
      '#title' => t('Java path:'),
      '#default_value' => variable_get('search_file_attachments_java_path', ''),
      '#description' => t('The full path to the Java binary. This setting is only needed if Java could not automatically detected.'),
    );
  }

  // File settings.
  $form['files'] = array(
    '#type' => 'fieldset',
    '#title' => t('File settings'),
  );
  $form['files']['search_file_attachments_include_extensions'] = array(
    '#type' => 'textfield',
    '#title' => t('Include file extensions or mimetypes'),
    '#description' => t('A comma-separated list of file extensions or mimetypes that will be included to the file search index.'),
    '#default_value' => search_file_extensions_included_extensions(),
  );

  // Advanced settings.
  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['advanced']['search_file_attachments_debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Activate Debugging'),
    '#description' => t('Activate this option only for development and not on production sites.'),
    '#default_value' => variable_get('search_file_attachments_debug', FALSE),
  );

  return system_settings_form($form);
}

/**
 * Validation handler for the settings form.
 */
function search_file_attachments_settings_form_validate($form, &$form_state) {
  if (empty($form_state['values']['search_file_attachments_tika_jar'])) {
    form_set_error('search_file_attachments_tika_jar', t('Tika jar is mandatory.'));
  }
  $path = realpath($form_state['values']['search_file_attachments_tika_path']);
  if (!file_exists($path . '/' . $form_state['values']['search_file_attachments_tika_jar'])) {
    form_set_error('search_file_attachments_tika_path', t('Tika jar file not found at this path.'));
  }
  if (isset($form_state['values']['search_file_attachments_java_path']) && !search_file_attachments_check_java($form_state['values']['search_file_attachments_java_path'])) {
    form_set_error('search_file_attachments_java_path', t('Java was not found or is executable at the specified path.'));
  }
}
