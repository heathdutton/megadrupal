<?php
/**
 * @file
 * Ctool access plugin
 *
 * Checks if a local selected content is present or not.
 */

$plugin = array(
  'title' => t('Shared Content active selected local content'),
  'description' => t('Controls access by the presense of a selected local content.'),
  'callback' => 'sharedcontent_ui_active_selected_local_content_access',
  'default' => array('present' => TRUE),
  'settings form' => 'sharedcontent_ui_active_selected_local_content_settings',
  'summary' => 'sharedcontent_ui_active_selected_local_content_summary',
);

/**
 * Checks if the search server supports the given features.
 */
function sharedcontent_ui_active_selected_local_content_access($conf, $context) {
  // If for some reason that $conf isn't set, we just want to be sure.
  if (empty($conf) || !isset($conf['present'])) {
    return FALSE;
  }
  $id = sharedcontent_ui_selected_sharedcontent_index_id();
  return !($id xor $conf['present']);
}

/**
 * Settings form for the 'search_api_server_support' access plugin.
 */
function sharedcontent_ui_active_selected_local_content_settings($form, &$form_state, $conf) {

  $form['settings']['present'] = array(
    '#type' => 'checkbox',
    '#title' => t('Selected content present'),
    '#description' => t('Check if a content must be selected for the pane to show up.'),
    '#default_value' => $conf['present'],
  );

  return $form;
}

/**
 * Provide a summary description based upon the entered feature.
 */
function sharedcontent_ui_active_selected_local_content_summary($conf, $context) {
  if ($conf['present']) {
    return t('Selected content present');
  }
  return t('Selected content not present');
}
