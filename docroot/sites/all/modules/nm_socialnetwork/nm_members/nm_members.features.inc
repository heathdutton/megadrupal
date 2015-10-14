<?php
/**
 * @file
 * nm_members.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function nm_members_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
  list($module, $api) = func_get_args();
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function nm_members_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_default_profile2_type().
 */
function nm_members_default_profile2_type() {
  $items = array();
  $items['nm_member_profile'] = entity_import('profile2_type', '{
    "userCategory" : true,
    "userView" : true,
    "type" : "nm_member_profile",
    "label" : "Member Profile",
    "weight" : "0",
    "data" : { "registration" : 0, "use_page" : 0 },
    "metatags" : [],
    "rdf_mapping" : []
  }');
  return $items;
}
