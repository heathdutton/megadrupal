<?php

/**
 * AJAX page callback to load tag suggestions.
 */
function entity_email_type_autocomplete_tags($tags_typed = '') {
  // The user enters a comma-separated list of tags. We only autocomplete the
  // last tag.
  $tags_typed = drupal_explode_tags($tags_typed);
  $tag_last = drupal_strtolower(array_pop($tags_typed));

  $tag_matches = array();
  if ($tag_last != '') {
    $query = db_select('entity_email_type_tags', 'eett');
    // Do not select already entered terms.
    if (!empty($tags_typed)) {
      $query->condition('eett.tag', $tags_typed, 'NOT IN');
    }
    // Select rows that match by tag name.
    $tags_return = $query
      ->distinct()
      ->fields('eett', array('tag'))
      ->condition('eett.tag', '%' . db_like($tag_last) . '%', 'LIKE')
      ->groupBy('eett.tag')
      ->range(0, 10)
      ->execute()
      ->fetchCol('eett.tag');

    $prefix = count($tags_typed) ? drupal_implode_tags($tags_typed) . ', ' : '';

    foreach ($tags_return as $name) {
      $n = $name;
      // Tag names containing commas or quotes must be wrapped in quotes.
      if (strpos($name, ',') !== FALSE || strpos($name, '"') !== FALSE) {
        $n = '"' . str_replace('"', '""', $name) . '"';
      }
      $tag_matches[$prefix . $n] = check_plain($name);
    }
  }
  drupal_json_output($tag_matches);
}
