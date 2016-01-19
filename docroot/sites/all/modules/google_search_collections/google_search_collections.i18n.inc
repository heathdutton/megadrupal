<?php
/**
 * @file
 * Internationalization (i18n) hooks.
 */

/**
 * Implements hook_i18n_string_info()
 */
function google_search_collections_i18n_string_info() {
  $groups['google_search_collections'] = array(
    'title' => t('Google Search Collections'),
    'description' => t('Google Search Collection related strings.'),
    'format' => FALSE, // This group doesn't have strings with format
    'list' => TRUE, // This group can list all strings
  );
  return $groups;
}

/**
 * Implements hook_i18n_string_list().
 */
function google_search_collections_i18n_string_list($group) {
  if ($group == 'google_search_collections' || $group == 'all') {
    $strings = array();

    $groups = google_search_collections_get_definitions(TRUE);
    foreach ($groups as $id => $definition) {
      $strings['google_search_collections'][$id] = array(
        'title' => $definition->title,
      );
    }

    return $strings;
  }
}

/**
 * Implements hook_i18n_string_refresh().
 *
 * Refresh translations for all generated strings.
 */
function google_search_collections_i18n_string_refresh($group) {
  if ($group == 'google_search_collections') {
    google_search_collections_i18n_update_strings();
  }
  return TRUE;
}
