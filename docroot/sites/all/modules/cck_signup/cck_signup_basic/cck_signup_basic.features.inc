<?php
/**
 * @file
 * cck_signup_basic.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function cck_signup_basic_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function cck_signup_basic_views_api() {
  list($module, $api) = func_get_args();
  if ($module == "views" && $api == "views_default") {
    return array("version" => "3.0");
  }
}

/**
 * Implements hook_node_info().
 */
function cck_signup_basic_node_info() {
  $items = array(
    'event' => array(
      'name' => t('Event'),
      'base' => 'node_content',
      'description' => t('An event that users can signup for.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'signup' => array(
      'name' => t('Signup'),
      'base' => 'node_content',
      'description' => t('A signup record for an event.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
