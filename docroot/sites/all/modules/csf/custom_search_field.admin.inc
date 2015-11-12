<?php

/**
 * @file
 * Admin settings for custom search Field.
 */

/**
 * Implements hook_help().
 */
function custom_search_field_help($path, $arg) {
  switch ($path) {
    case 'admin/config/search/custom_search/field':
      $output = t('Select the authors to present as search options in the search block. If none is selected, no selector will be displayed.');
      break;
  }
  return $output;
}

/**
 * Sets up the admin field display.
 */
function custom_search_field_admin() {
  $myfields = array();
  foreach (node_type_get_names() as $nodename) {
    $myfields[] = field_info_instances('node', $nodename);
  }
  if (count($myfields)) {
    $form = _custom_search_field_admin_form($myfields);
    return system_settings_form($form);
  }
}
