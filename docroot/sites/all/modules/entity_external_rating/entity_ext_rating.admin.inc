<?php

/**
 * @file
 *  The file contains the administration settings for this module.
 */

/**
 *
 * Administration page callback.
 *
 * @return array
 *   A drupal form representing the administration form.
 */
function entity_ext_rating_admin() {
  ctools_include('plugins');
  $plugins = ctools_get_plugins('entity_ext_rating', 'plugins');
  return drupal_get_form('entity_ext_rating_admin_form', $plugins);
}

/**
 * Administration form constructor.
 *
 * @param array $plugins
 *   An array of all the available plugins that can be enabled, disabled or
 *   for which the settings can be edited.
 *
 * @return array
 *   A system settings form.
 */
function entity_ext_rating_admin_form($form, &$form_state, $plugins) {
  // Show a set of checkboxes to enable or disable the plugins.
  $options = array();
  foreach ($plugins as $key => $plugin) {
    $options[$key] = $plugin['name'] . ' (' . l(t('Edit'), 'admin/structure/entity_ext_rating/edit/' . $key) . ')';
  }
  $form['entity_ext_rating_active_plugins'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Available plugins'),
    '#description' => t('Please select which plugins to be enabled. Please note that if you disable a plugin, the total rating for will be updated only on cron. Same for enableing plugins.'),
    '#options' => $options,
    '#default_value' => variable_get('entity_ext_rating_active_plugins', array()),
  );
  // Select field for defining the timeframe for checking the external rating.
  $rating_check_options = array(
    '1' => format_plural(1, '1 second (for testing purposes)', '@count seconds (for testing purposes)'),
    '60' => format_plural(1, '1 minute', '@count minutes'),
    '300' => format_plural(5, '1 minute', '@count minutes'),
    '600' => format_plural(10, '1 minute', '@count minutes'),
    '1800' => format_plural(30, '1 minute', '@count minutes'),
    '3600' => format_plural(1, '1 hour', '@count hours'),
    '21600' => format_plural(6, '1 hour', '@count hours'),
    '43200' => format_plural(12, '1 hour', '@count hours'),
    '86400' => format_plural(1, '1 day', '@count days'),
  );
  $form['entity_ext_rating_check_timeframe'] = array(
    '#type' => 'select',
    '#title' => t('Rating fetch timeframe'),
    '#description' => t('Select the amount of time between two consecutive rating checks. Be careful, selecting a small value can block your cron if there are a lot of ratings to be checked.'),
    '#options' => $rating_check_options,
    '#default_value' => variable_get('entity_ext_rating_check_timeframe', 1800),
  );
  // Edit the batch size for the requests to get the external rating.
  $form['entity_ext_rating_requests_batch_size'] = array(
    '#type' => 'select',
    '#title' => t('Requests batch size'),
    '#description' => t('Select the batch size for the requests when getting the external rating. This number represents how many rows will be queried at a time from the queue.'),
    '#options' => drupal_map_assoc(array(1, 5, 10, 20, 50, 100)),
    '#default_value' => variable_get('entity_ext_rating_requests_batch_size', 20),
  );
  // Add also a custom submit handler for some additional things that need to
  // be done when the plugins are enabled or disabled.
  $form['#submit'][] = 'entity_ext_rating_admin_form_submit';
  return system_settings_form($form);
}

/**
 * Additional submit handler for the settings form.
 */
function entity_ext_rating_admin_form_submit($form, &$form_state) {
  // For the disabled plugins, delete the rating values.
  // @todo: maybe invoke a hook or a method for all enabled or disabled plugins.
  foreach ($form_state['values']['entity_ext_rating_active_plugins'] as $key => $plugin_name) {
    if (!$plugin_name) {
      db_delete('entity_ext_rating_values')
        ->condition('source', $key)
        ->execute();
    }
  }
}

/**
 * Callback function for editing a rating plugin.
 *
 * @param array $rating_source
 *   The rating source plugin meta infomation, usually obtained after a call
 *   to ctools_get_plugins() function.
 *
 * @return array
 *    A drupal form for edit the rating source (the plugin) settings.
 */
function entity_ext_rating_edit_source($rating_source) {
  // Get a class instance (object) for the rating plugin.
  $plugin = entity_ext_rating_get_plugin($rating_source['name']);
  // If there is such and object, then render the edit form.
  // Else, show a message and go to frontpage.
  // TO DO: is it needed to put a watchdog entry also?
  if (is_object($plugin)) {
    return drupal_get_form('entity_ext_rating_edit_source_form', $plugin);
  }
  drupal_set_message(t('No plugin available'));
  drupal_goto();
}

/**
 * Form construcor for editing a rating source.
 *
 * @param object $plugin
 *   A plugin object, obtained after instatiating a class that implements the
 *   EntityExtRatingSourceInterface (TwitterRating, FacebookRating for example).
 */
function entity_ext_rating_edit_source_form($form, &$form_state, $plugin) {
  // Just call the plugin configiration form, and add the submit button.
  $form = $plugin->configForm($form_state);
  $form ['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['#rating_plugin'] = $plugin;
  return $form;
}

/**
 * Validation handler for edit rating plugin form.
 */
function entity_ext_rating_edit_source_form_validate($form, &$form_state) {
  // Just call the plugin validation method.
  $plugin = $form['#rating_plugin'];
  $plugin->configFormValidate($form_state['values']);
}

/**
 * Submit handler for edit rating plugin form.
 */
function entity_ext_rating_edit_source_form_submit($form, &$form_state) {
  // Just call the plugin submit method.
  $plugin = $form['#rating_plugin'];
  $plugin->configFormSubmit($form_state['values']);
}