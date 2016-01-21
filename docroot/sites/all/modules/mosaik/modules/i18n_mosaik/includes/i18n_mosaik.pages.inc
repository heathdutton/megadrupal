<?php

/**
 * @file
 * Page callbacks for the mosaik module, i18n remake.
 */

/**
 * Helper function for autocompletion. Select by language.
 */
function i18n_mosaik_autocomplete_language($langcode, $keys = '') {
  // The user enters a comma-separated list of mosaiks.
  // We only autocomplete the last tag.
  $keys = drupal_explode_tags($keys);
  $tag_last = drupal_strtolower(array_pop($keys));

  $mosaik_matches = array();
  if ($langcode && $tag_last != '') {
    $query = db_select('mosaik', 'm')
      ->fields('m', array('name', 'title'));
    $query->addTag('translatable');
    $query->addTag('mosaik_access');
    // Disable i18n_select for this query
    $query->addTag('i18n_select');
    // Add language condition
    $query->condition('m.language', $langcode);

    // Do not select already entered mosaiks.
    if (!empty($keys)) {
      $query->condition('m.title', $keys, 'NOT IN');
    }
    // Select rows that match by mosaik title.
    $tags_return = $query
      ->condition('m.title', '%' . db_like($tag_last) . '%', 'LIKE')
      ->range(0, 10)
      ->execute()
      ->fetchAllKeyed();

    $prefix = count($keys) ? drupal_implode_tags($keys) . ', ' : '';

    foreach ($tags_return as $name => $title) {
      $t = $title;
      // Mosaik names containing commas or quotes must be wrapped in quotes.
      if (strpos($title, ',') !== FALSE || strpos($title, '"') !== FALSE) {
        $t = '"' . str_replace('"', '""', $title) . '"';
      }
      $mosaik_matches[$prefix . $t] = check_plain($title);
    }
  }

  drupal_json_output($mosaik_matches);
}
