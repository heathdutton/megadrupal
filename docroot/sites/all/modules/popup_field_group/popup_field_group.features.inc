<?php

/**
 * @file Features-related hooks.
 */

/**
 * Implements hook_features_export_alter().
 *
 * Add ourselves as a dependency if a feature uses popup fieldgroups.
 */
function popup_field_group_features_export_alter(&$export, $module_name) {

  // Make sure we have fresh data by loading directly.
  ctools_include('export');
  $field_groups = ctools_export_load_object('field_group');

  // Get an array of all the names of popup field groups.
  $popup_field_groups = array();
  foreach ($field_groups as $name => $info) {
    if (isset($info->data['format_type']) && ($info->data['format_type'] === 'popup')) {
      $popup_field_groups[$name] = $name;
    }
  }

  if (!empty($export['features']['field_group'])) {
    if (count(array_intersect($export['features']['field_group'], $popup_field_groups))) {
      $export['dependencies']['popup_field_group'] = 'popup_field_group';
    }
  }
}