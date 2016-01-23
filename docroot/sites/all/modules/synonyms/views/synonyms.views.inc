<?php

/**
 * @file
 * Views integration of Synonyms module.
 */

/**
 * Implements hook_views_plugins_alter().
 */
function synonyms_views_plugins_alter(&$plugins) {
  // Replace default taxonomy term argument validator with our extended version,
  // which can also handle a term synonym as an argument.
  $plugins['argument validator']['taxonomy_term']['handler'] = 'synonyms_views_plugin_argument_validate_taxonomy_term';
}

/**
 * Implements hook_field_views_data_alter().
 */
function synonyms_field_views_data_alter(&$result, $field, $module) {
  if ($field['type'] == 'taxonomy_term_reference') {
    // Add synonyms friendly autocomplete filter.
    foreach ($field['storage']['details']['sql'] as $table) {
      $tid_column = reset($table);
      $tid_column = $tid_column['tid'];
      $table = array_keys($table);
      $table = $table[0];
      if (isset($result[$table][$tid_column]['filter'])) {
        $result[$table][$tid_column]['filter']['handler'] = 'synonyms_views_handler_filter_term_tid';
      }
    }
  }
}
