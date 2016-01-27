<?php
/**
 * @file
 * AJAX callback functions.
 */
/**
 * Validate whether a list name exists.
 *
 * @param $listname
 *   The Lyris list name from the form.
 */
function lyris_ajax_name_validate($listname) {
  print theme('lyris_list_name_indicator', array('exists' => lyris_list_name_exists($listname), 'list' => $listname));
}

/**
 * Autocomplete for list names.
 */
function lyris_ajax_name_autocomplete($str = '') {
  $lyris = lyris();

  // First load ALL lists into an array in the object, then we can filter on
  // that.
  $lyris->loadUserAllowedLists('', 0);
  
  // Get allowed lists from Lyris
  $lists = $lyris->loadUserAllowedLists($str);

  // Get local lists
  $res = db_select(LYRIS_LIST_TABLE, 'l')->fields('l', array(llf('ListID'), llf('ListName')))->execute();

  // Remove existing lists from Lyris lists.
  while ($row = $res->fetchAssoc()) {
    if (isset($lists[$row[llf('ListID')]])) {
      unset($lists[$row[llf('ListID')]]);
    }
  }

  drupal_json_output(drupal_map_assoc($lists));
}
