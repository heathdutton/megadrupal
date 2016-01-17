<?php

/**
 * @file
 * Default Profile Lite Categories. We create a default category for account, so
 * that fields can be added and removed from this form.
 */

/**
 * Implements hook_profile_lite_default_categories().
 */
function profile_lite_profile_lite_default_categories() {
  $export = $fields = array();

  foreach (field_info_instances('user', 'user') as $field_name => $field) {
    $fields[$field_name]['weight'] = 0;
  }

  $category = new stdClass;
  $category->disabled = FALSE; /* Edit this to true to make a default category disabled initially */
  $category->api_version = 1;
  $category->name = PROFILE_LITE_DEFAULT_CATEGORY;
  $category->label = 'Account';
  $category->weight = -50;
  $category->fields = $fields;
  $export[PROFILE_LITE_DEFAULT_CATEGORY] = $category;

  return $export;
}