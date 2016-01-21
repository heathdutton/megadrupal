<?php

/**
 * @file
 * This is the Synchronize Translation Paths admin include which provides 
 * an interface to configure the module.
 */

/**
 * Admin settings form.
 *
 * @return
 *   The form.
 */
function synchronize_translation_paths_settings() {

  $form = array();

  // Keep all permanent settings in array for easier storage.
  $form['settings'] = array(
    '#tree' => TRUE,
  );

  // Only the enabled content types will be processed.
  $content_types = array();
  foreach (node_type_get_types() as $content_type) {

    // Only content types using entity translation for now.
    if (entity_translation_node_supported_type($content_type->type)) {
      $content_types[$content_type->type] = $content_type->name;
    }
  }
  $default = synchronize_translation_paths_get_setting('content_types');
  $form['settings']['content_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Content Types'),
    '#options' => $content_types,
    '#description' => t('Enable the content types that you want to synchronize paths.'),
    '#default_value' => ($default ? $default : array()),
  );

  // Keep temporary settings out of the array.
  $form['refresh'] = array(
    '#type' => 'checkbox',
    '#title' => t('Refresh all paths now?'),
    '#description' => t('If checked, all paths for all nodes for the content types enabled will be updated. This cannot be undone. Make sure to backup your database before enabling! <em>Not yet implemented.  See <a href="@url">Synchronize paths for all existing content</a> for details.</em>', array(
      '@url' => url('http://drupal.org/node/2205011'),
    )),
    '#disabled' => TRUE,
    '#default_value' => FALSE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#submit' => array('synchronize_translation_paths_settings_submit'),
    '#value' => t('Save Configuration'),
  );

  return $form;
}

/**
 * Save the admin settings.
 *
 * @param $form_id
 *   The form ID.
 * @param $form_state
 *   The form state.
 */
function synchronize_translation_paths_settings_submit($form_id, &$form_state) {

  // If set to refresh, run the update on all configured content.
  if ($form_state['values']['refresh']) {
    synchronize_translation_paths_refresh_paths();
  }

  variable_set('synchronize_translation_paths_settings', $form_state['values']['settings']);
  drupal_set_message(t('Synchronize Translation Paths settings have been saved.'));
}

