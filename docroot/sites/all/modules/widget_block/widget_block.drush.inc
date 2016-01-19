<?php

/**
 * @file
 * Defines the drush commands made available by the Widget Block module.
 */

/*
 * Hook implementations.
 */

/**
 * Implements hook_drush_command().
 */
function widget_block_drush_command() {
  return array(
    'widget-block-refresh' => array(
      'description' => 'Refresh widget block cache data.',
      'options' => array(
        'ids' => array(
          'description' => 'One or more widget identifiers to refresh. Multiple identifiers should be separated by a comma. Ommit this option to refresh all available widgets.',
          'example_value' => '1',
          'value' => 'optional',
        ),
        'languages' => array(
          'description' => 'One or more language codes. Multiple language codes should be separated by a comma. Ommit this optional to use all available languages',
          'example_value' => 'en',
          'value' => 'optional',
        ),
        'existing' => array(
          'description' => 'Indicates whether only existing cached data should be refreshed. Defaults to "true" set to "false" if all data should be updated.',
          'example_value' => 'false',
          'value' => 'optional',
        ),
      ),
      'callback' => 'drush_widget_block_refresh',
    ),
    'widget-block-purge' => array(
      'description' => 'Purge widget block cache data.',
      'options' => array(
        'ids' => array(
          'description' => 'One or more widget identifiers to purge. Multiple identifiers should be separated by a comma. Ommit this option to purge all available widgets.',
          'example_value' => '1',
          'value' => 'optional',
        ),
      ),
      'callback' => 'drush_widget_block_purge',
    ),
  );
}

/**
 * Drush callback for "widget-block-refresh".
 */
function drush_widget_block_refresh() {
  // Check if the ids option was provided.
  if (($ids = drush_get_option('ids', NULL)) !== NULL) {
    // Convert the ids argument to an array.
    $ids = explode(',', $ids);
  }
  // Check if the languages arguments was provided.
  if (($languages = drush_get_option('languages', NULL)) !== NULL) {
    // Convert the languages argument to an array.
    $languages = explode(',', $languages);
  }
  // Determine whether only existing cache data should be refreshed.
  $existing = (drush_get_option('existing', 'true') !== 'false');
  // Log wait message.
  drush_log(dt('Started refreshing widget block data, please wait.'), 'ok');
  // Refresh the widget block data.
  $results = widget_block_refresh($ids, $languages, $existing);
  // Set the error flag to FALSE as default behavior.
  $error = FALSE;
  // Iterate through the results.
  foreach ($results as $result) {
    // Iterate through the successful refreshes.
    foreach ($result['success'] as $langcode) {
      // Build the translation arguments.
      $dt_args = array(
        '@name' => $result['name'],
        '!id' => $result['id'],
        '@lang' => $langcode,
      );
      // Log the success to the console.
      drush_log(dt('Refreshed widget block "@name" (!id) for language "@lang"', $dt_args), 'ok');
    }
    // Iterate through the failed refreshes.
    foreach ($result['failed'] as $langcode) {
      // Build the translation arguments.
      $dt_args = array(
        '@name' => $result['name'],
        '!id' => $result['id'],
        '@lang' => $langcode,
      );
      // Log the failure to the console.
      drush_log(dt('Refresh widget block "@name" (!id) for language "@lang" failed', $dt_args), 'error');
      // Indicate an error occurred.
      $error = TRUE;
    }
  }
  // Check if an error was encountered.
  if ($error) {
    // Log failure message.
    drush_log(dt('Completed refreshing widget blocks. One or more widget blocks failed to refresh.'), 'error');
  }
  else {
    // Log success message.
    drush_log(dt('Completed refreshing widget blocks.'), 'success');
  }
}

/**
 * Drush callback for "widget-block-purge".
 */
function drush_widget_block_purge() {
  // Check if the ids option was provided.
  if (($ids = drush_get_option('ids', NULL)) !== NULL) {
    // Convert the ids argument to an array.
    $ids = explode(',', $ids);
  }
  // Purge cached widget block data.
  widget_block_purge($ids);
  // Log purge performed.
  drush_log(dt('Completed purging widget blocks.'), 'success');
}
